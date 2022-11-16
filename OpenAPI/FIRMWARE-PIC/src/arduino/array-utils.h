#ifndef ARRAY_UTILS_H_
#define ARRAY_UTILS_H_

#include <stdint.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( (x) [ 0 ] ) )
#endif // ARRAY_SIZE

#undef _decl_rev
#define _decl_rev( _type, _short_type ) \
	void array_reverse_ ## _short_type ( _type *a, unsigned len, unsigned start, unsigned end )

_decl_rev( uint8_t, u8 );
_decl_rev( uint16_t, u16 );
_decl_rev( uint32_t, u32 );
_decl_rev( uint64_t, u64 );

_decl_rev( int8_t, s8 );
_decl_rev( int16_t, s16 );
_decl_rev( int32_t, s32 );
_decl_rev( int64_t, s64 );

_decl_rev( float, f32 );
_decl_rev( double, f64 );

_decl_rev( void *, voidp );

#undef _decl_rev

#undef _decl_shift
#define _decl_shift( _type, _short_type ) \
	void array_shift_ ## _short_type ( _type *a, unsigned len, unsigned m )

_decl_shift( uint8_t, u8 );
_decl_shift( uint16_t, u16 );
_decl_shift( uint32_t, u32 );
_decl_shift( uint64_t, u64 );

_decl_shift( int8_t, s8 );
_decl_shift( int16_t, s16 );
_decl_shift( int32_t, s32 );
_decl_shift( int64_t, s64 );

_decl_shift( float, f32 );
_decl_shift( double, f64 );

_decl_shift( void *, voidp );

#undef _decl_shift

#endif /* ARRAY_UTILS_H_ */
