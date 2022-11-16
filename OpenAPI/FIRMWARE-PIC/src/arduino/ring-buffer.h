#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stddef.h>
#include <stdint.h>

#define PACKED_IAR
#define PACKED_GNU

PACKED_IAR struct PACKED_GNU _ring_buffer;
typedef struct _ring_buffer ring_buffer_t;

PACKED_IAR struct PACKED_GNU _ring_buffer {
#ifdef __cplusplus
	unsigned         capacity;
#else
	const unsigned   capacity;
#endif
	unsigned         head;
	unsigned         len;
	// extract items from the head of the buffer without advancing the head
	int            (*peek)( ring_buffer_t *rb, void *data, unsigned data_len );
	// extract items from the head of the buffer, advancing the head
	int            (*read)( ring_buffer_t *rb, void *data, unsigned data_len );
	// inject items at the tail of the buffer, increasing its length
	int            (*write)( ring_buffer_t *rb, void *data, unsigned data_len );
	// like write, except take the input from another ring buffer instead of from an array
	int            (*send)( ring_buffer_t *rb, ring_buffer_t *input, unsigned data_len );
	// advance the head of the buffer, decreasing its length
	int            (*skip)( ring_buffer_t *rb, unsigned data_len );
	// the length of the buffer
	unsigned       (*size)( ring_buffer_t *rb );
	// the length of the unused portion of the buffer
	unsigned       (*available)( ring_buffer_t *rb );
	// set the buffer's head and length to zero
	void           (*reset)( ring_buffer_t *rb );
	// circularly shift the underling buffer so that the head is in the 0th position
	void           (*realign)( ring_buffer_t *rb );
	void            *buffer;
};

#define RING_BUFFER_DECL_CONTIG( cap, name ) \
uint8_t name ## _buffer[ cap + sizeof( ring_buffer_t ) ];

#define RING_BUFFER_CONTIG_HANDLE( name ) ( (ring_buffer_t *) name ## _buffer )
#define RING_BUFFER_CONTIG_BUFFER( name ) ( (uint8_t *)( name ## _buffer ) + sizeof( ring_buffer_t ) )

int ring_buffer_init( ring_buffer_t *rb, unsigned capacity, void *buffer );

#endif /* RING_BUFFER_H_ */
