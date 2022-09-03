#include <stdint.h>
#include "array-utils.h"

// using reversal algorithm from "Programming Pearls, 2nd Edition"
#define ARRAY_REVERSE( _type, _short_type ) \
void array_reverse_ ## _short_type ( _type *a, unsigned len, unsigned start, unsigned end ) { \
	_type tmp; \
	if ( start >= len || end >= len ) { \
		goto out; \
	} \
	for( ; start < end; start++, end-- ) { \
		tmp = a[ start ]; \
		a[ start ] = a[ end ]; \
		a[ end ] = tmp; \
	} \
out: \
	return; \
}

#define ARRAY_SHIFT( _type, _short_type ) \
void array_shift_ ## _short_type ( _type *a, unsigned len, unsigned m ) { \
	m = 0 == m ? 0 : m % len; \
	if ( 0 == m ) { \
		goto out; \
	} \
	array_reverse_ ## _short_type ( a, len, 0, len - 1 ); \
	array_reverse_ ## _short_type ( a, len, 0, m - 1 ); \
	array_reverse_ ## _short_type ( a, len, m, len - 1 ); \
out: \
	return; \
}

/*###########################################################################
  #                            ARRAY REVERSAL
  ###########################################################################*/

ARRAY_REVERSE( uint8_t, u8 );
ARRAY_REVERSE( uint16_t, u16 );
ARRAY_REVERSE( uint32_t, u32 );
ARRAY_REVERSE( uint64_t, u64 );

ARRAY_REVERSE( int8_t, s8 );
ARRAY_REVERSE( int16_t, s16 );
ARRAY_REVERSE( int32_t, s32 );
ARRAY_REVERSE( int64_t, s64 );

ARRAY_REVERSE( float, f32 );
ARRAY_REVERSE( double, f64 );

ARRAY_REVERSE( void *, voidp );

/*###########################################################################
  #                            ARRAY SHIFT
  ###########################################################################*/

ARRAY_SHIFT( uint8_t, u8 );
ARRAY_SHIFT( uint16_t, u16 );
ARRAY_SHIFT( uint32_t, u32 );
ARRAY_SHIFT( uint64_t, u64 );

ARRAY_SHIFT( int8_t, s8 );
ARRAY_SHIFT( int16_t, s16 );
ARRAY_SHIFT( int32_t, s32 );
ARRAY_SHIFT( int64_t, s64 );

ARRAY_SHIFT( float, f32 );
ARRAY_SHIFT( double, f64 );

ARRAY_SHIFT( void *, voidp );
