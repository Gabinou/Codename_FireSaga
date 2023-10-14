#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

/* -- Bitflags -- */
typedef uint_fast8_t  bitflag8_t;
typedef uint_fast16_t bitflag16_t;
typedef uint32_t      bitflag32_t;
typedef uint64_t      bitflag64_t;

/* -- Standard Integer types -- */
typedef uint8_t       u8;
typedef uint16_t      u16;
typedef uint32_t      u32;
typedef uint64_t      u64;
typedef int8_t        i8;
typedef int16_t       i16;
typedef int32_t       i32;
typedef int64_t       i64;
typedef uint_fast8_t  uf8;
typedef uint_fast16_t uf16;
typedef uint_fast32_t uf32;
typedef uint_fast64_t uf64;
typedef int_fast8_t   if8;
typedef int_fast16_t  if16;
typedef int_fast32_t  if32;
typedef int_fast64_t  if64;

#endif /* TYPES_H */