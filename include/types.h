#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

/* -- Bitflags -- */
typedef uint8_t  bitflag8_t;
typedef uint16_t bitflag16_t;
typedef uint32_t bitflag32_t;
typedef uint64_t bitflag64_t;

/* -- Standard Integer types -- */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef char     byte;
/* - Why no fast types? - */
//   - Variable size type make byte-alignment more difficult
//   - int should be the fastest

/* -- Boolean types -- */
/* - Why define my own boolean? - */
//   - bool type size depends on architecture
typedef int32_t  b32;

#endif /* TYPES_H */