#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

/* -- Standard integer types -- */
typedef signed char         i8;
typedef unsigned char       u8;
typedef signed short        i16;
typedef unsigned short      u16;
typedef signed int          i32;
typedef unsigned int        u32; /* 42u */
typedef signed long long    i64; /* 42ll */
typedef unsigned long long  u64; /* 42ull */

/* -- Standard integers literasl -- */
// Todo test suffix macro
#define i8_C(c)  c
#define i16_C(c) c
#define i32_C(c) c
#define i64_C(c) c ## ll
#define u8_C(c)  c
#define u16_C(c) c
#define u32_C(c) c ## u
#define u64_C(c) c ## ull

/* -- Bitflags -- */
typedef unsigned char       byte;

typedef u8  bitflag8_t;
typedef u16 bitflag16_t;
typedef u32 bitflag32_t;
typedef u64 bitflag64_t;

/* -- Standard float types -- */
typedef float       f32;    /* 42.0f */
typedef double      f64;
typedef long double f128;   /* 42.0l */

/* -- Standard float literasl -- */
#define f32_C(c)    c ## f
#define f64_C(c)    c
#define f128_C(c)   c ## l

/* -- Boolean types -- */
/* - Why define my own boolean? -
**   - bool type size depends on architecture
**   - Other weirdness I should write down... */
typedef i32 b32;
#define true 1
#define false 0

/* -- Component typeflags -- */
typedef b32 CursorFlag;
typedef b32 MouseFlag;
typedef b32 RenderTop;

/* Automatically repair items on camp start. */
typedef b32 RepairFlag;

/* For computer's turn */
typedef b32 AI_Control;

#endif /* TYPES_H */