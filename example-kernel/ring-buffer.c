#include <stdint.h>
#include <string.h>
#include "ring-buffer.h"
#include "minmax.h"
#include "array-utils.h"

// same as ring_buffer_available()
static inline unsigned rbavail( ring_buffer_t *rb ) {
	unsigned r;

	if ( NULL == rb ) {
		r = 0;
		goto out;
	}

	// assert( rb->capacity >= rb->len );
	r = rb->capacity - rb->len;

out:
	return r;
}

// the index of the next writable element
static inline unsigned rbtail( ring_buffer_t *rb ) {
	unsigned r;

	if ( NULL == rb || 0 == rb->capacity ) {
		r = 0;
		goto out;
	}

	r = ( rb->head + rb->len ) % rb->capacity;

out:
	return r;
}

static int      ring_buffer_peek( ring_buffer_t *rb, void *data, unsigned data_len );
static int      ring_buffer_read( ring_buffer_t *rb, void *data, unsigned data_len );
static int      ring_buffer_write( ring_buffer_t *rb, void *data, unsigned data_len );
static int      ring_buffer_send( ring_buffer_t *out, ring_buffer_t *in, unsigned data_len );
static int      ring_buffer_skip( ring_buffer_t *rb, unsigned data_len );
static unsigned ring_buffer_size( ring_buffer_t *rb );
static unsigned ring_buffer_available( ring_buffer_t *rb );
static void     ring_buffer_reset( ring_buffer_t *rb );
static void     ring_buffer_realign( ring_buffer_t *rb );

int ring_buffer_init( ring_buffer_t *rb, unsigned capacity, void *buffer ) {
	int r;

	if ( NULL == rb || NULL == buffer ) {
		r = -1;
		goto out;
	}

	*( (unsigned *) & rb->capacity ) = capacity;
	rb->head = 0;
	rb->len = 0;

	rb->peek = ring_buffer_peek;
	rb->read = ring_buffer_read;
	rb->write = ring_buffer_write;
	rb->send = ring_buffer_send;
	rb->skip = ring_buffer_skip;
	rb->size = ring_buffer_size;
	rb->available = ring_buffer_available;
	rb->reset = ring_buffer_reset;
	rb->realign = ring_buffer_realign;

	rb->buffer = buffer;

	r = 0;//EXIT_SUCCESS;

out:
	return r;
}

static int ring_buffer_peek( ring_buffer_t *rb, void *data, unsigned data_len ) {
	int r;
	unsigned t1, t2;

	if ( NULL == rb || NULL == data ) {
		r = -1;
		goto out;
	}

	r = min( rb->len, data_len );
	if ( 0 == r ) {
		goto out;
	}
	if ( rb->head + r >= rb->capacity ) {
		t1 = rb->capacity - rb->head;
		memcpy( data, & ( (uint8_t *)rb->buffer ) [ rb->head ], t1 );
		t2 = r - t1;
		memcpy( & ( (uint8_t *) data )[ t1 ], & ( (uint8_t *)rb->buffer )[ 0 ], t2 );
	} else {
		memcpy( data, & ( (uint8_t *)rb->buffer )[ rb->head ], r );
	}

out:
	return r;
}

static int ring_buffer_read( ring_buffer_t *rb, void *data, unsigned data_len ) {
	int r;
	r = ring_buffer_peek( rb, data, data_len );
	if ( r > 0 ) {
		ring_buffer_skip( rb, r );
	}
	return r;
}

static int ring_buffer_write( ring_buffer_t *rb, void *data, unsigned data_len ) {
	int r;
	unsigned tail;
	unsigned t1, t2;

	if ( NULL == rb || NULL == data ) {
		r = -1;
		goto out;
	}

	r = min( rbavail( rb ), data_len );
	if ( 0 == r ) {
		goto out;
	}

	tail = rbtail( rb );
	if ( tail + r >= rb->capacity ) {
		t1 = rb->capacity - tail;
		memcpy( & ( (uint8_t *)rb->buffer )[ tail ], data, t1 );
		t2 = r - t1;
		memcpy( & ( (uint8_t *)rb->buffer )[ 0 ], & ( (uint8_t *) data )[ t1 ],  t2 );
	} else {
		memcpy( & ( (uint8_t *)rb->buffer )[ tail ], data, r );
	}

	rb->len += r;

out:
	return r;
}

static int ring_buffer_send( ring_buffer_t *out, ring_buffer_t *in, unsigned data_len ) {

	int r;

	unsigned orig_head_in;
	unsigned orig_head_out;
	unsigned tail_out;

	if ( NULL == out || NULL == in ) {
		r = -1;
		goto out;
	}

	r = min( in->size( in ), out->available( out ) );
	r = min( r, data_len );
	if ( 0 == r ) {
		goto out;
	}

	orig_head_out = out->head;
	orig_head_in = in->head;

	// XXX: @CF: FIXME: Realigning both buffers simplifies the op, but is not optimized
	out->realign( out );
	in->realign( in );

	tail_out = rbtail( out );
	memcpy( & ( (uint8_t *)out->buffer )[ tail_out ], & ( (uint8_t *)in->buffer )[ 0 ], r );

	// update output length
	out->len += r;

	// XXX: @CF: FIXME: Realigning requires that both heads are put back where they came from
	array_shift_u8( out->buffer, out->capacity, orig_head_out );
	out->head = orig_head_out;
	array_shift_u8( in->buffer, in->capacity, orig_head_in );
	in->head = orig_head_in;

	// advance input by r
	in->skip( in, r );

out:
	return r;
}

static int ring_buffer_skip( ring_buffer_t *rb, unsigned data_len ) {
	int r;

	if ( NULL == rb ) {
		r = -1;
		goto out;
	}

	r = min( rb->len, data_len );
	if ( r > 0 ) {
		rb->len -= r;
		rb->head += r;
		if ( rb->capacity > 0 ) {
			rb->head %= rb->capacity;
		}
	}

out:
	return r;
}

static unsigned ring_buffer_size( ring_buffer_t *rb ) {
	unsigned r;

	if ( NULL == rb ) {
		r = 0;
		goto out;
	}

	return rb->len;

out:
	return r;
}

static unsigned ring_buffer_available( ring_buffer_t *rb ) {
	return rbavail( rb );
}

static void ring_buffer_reset( ring_buffer_t *rb ) {
	if ( NULL == rb ) {
		goto out;
	}
	ring_buffer_init( rb, rb->capacity, rb->buffer );
out:
	return;
}

static void ring_buffer_realign( ring_buffer_t *rb ) {

	if ( NULL == rb ) {
		goto out;
	}

	if ( 0 == rb->head ) {
		goto out;
	}

	array_shift_u8( rb->buffer, rb->capacity, rb->capacity - rb->head );

	rb->head = 0;

out:
	return;
}
