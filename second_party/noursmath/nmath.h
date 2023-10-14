#ifndef NOURSMATH_H
#define NOURSMATH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

/******************************* NOURS_MATH v0.3 *****************************/
/* Math library I made during the development of my game, Codename:Firesaga
* 3 Main modules:
*     q_math, quick math module
*     linalg, linear algebra utilities for n-dimensional matrices (like numpy)
*     pathfinding, utilities for games set on 2D a grid/matrix
* + Utility module DARR for dynamic arrays
*/

#ifndef DARR
#define DARR

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/********************** DARR: DYNAMIC ARRAYS FOR C99 v1.0 ******************/
// A DARR is an array with two additional elements:
//   -> allocated length (len) at [-2] and number of active element (num) at [-1]

#define DARR_GROWTH_FACTOR 2
#define DARR_LEN_INDEX 2
#define DARR_NUM_INDEX 1

#define DARR_LEN(darr) (*((size_t *)darr - DARR_LEN_INDEX)) // allocated length
#define DARR_NUM(darr) (*((size_t *)darr - DARR_NUM_INDEX)) // number of active elements

// DARR_INIT: a DARR is an array with  size_t num at -1 and size_t len at -2,
#define DARR_INIT(darr, type, len) (type*)(((size_t* )malloc(sizeof(size_t)*DARR_LEN_INDEX + sizeof(type)*(len))) + DARR_LEN_INDEX);\
    DARR_LEN(darr) = len;\
    DARR_NUM(darr) = 0;

// DARR_REALLOC: DARR internal. Not to be called directly by users.
#define DARR_REALLOC(darr, len) (void *)((size_t* )realloc(((size_t* )darr - DARR_LEN_INDEX), (sizeof(size_t)*DARR_LEN_INDEX + (sizeof(*darr))*(len))) + DARR_LEN_INDEX)

// DARR_GROW: increase array length by multiplying DARR_GROWTH_FACTOR
#define DARR_GROW(darr) do {\
    DARR_LEN(darr)*=DARR_GROWTH_FACTOR;\
    darr = DARR_REALLOC(darr, DARR_LEN(darr));\
} while(0)

// DARR_PUT: put elem on top of darr, at DARR_NUM
#define DARR_PUT(darr, elem) do {if ((DARR_NUM(darr) + 1) >= (DARR_LEN(darr))) {\
DARR_GROW(darr);\
}\
darr[DARR_NUM(darr)++] = elem;\
} while(0)

// DARR_INSERT: Put elem at inde, moving other elements over
#define DARR_INSERT(darr, elem, index) do {if ((++DARR_NUM(darr)) >= (DARR_LEN(darr))) {\
DARR_GROW(darr);\
}\
memmove((darr + index + 1), (darr + index), sizeof(*darr) * (DARR_NUM(darr) - index));\
darr[index] = elem;\
} while (0)

// DARR_POP: get top element of darr, and "remove" it by decrementing DARR_NUM
#define DARR_POP(darr) darr[--DARR_NUM(darr)]

// DARR_DEL_SCRAMBLE: delete elem by copying top element over it, and decrementing DARR_NUM
#define DARR_DEL_SCRAMBLE(darr, elem) do {\
    if (elem < DARR_NUM(darr)) {\
        memmove((darr + elem), (darr + --DARR_NUM(darr)), sizeof(*darr));\
    }\
} while(0)

// DARR_DEL: delete elem by moving all subsequent elements over
#define DARR_DEL(darr, elem) do {\
    if (elem < DARR_NUM(darr)) {\
        memmove((darr + elem), (darr + elem + 1), sizeof(*darr) * (--DARR_NUM(darr) - elem));\
    }\
} while(0)

// DARR_FREE: free whole darr
#define DARR_FREE(darr) do {free((((size_t* )darr) - DARR_LEN_INDEX));} while(0)

#endif /* DARR */

#ifndef DTAB
#define DTAB

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/********************* DTAB: DYNAMIC HASH TABLE FOR C99 v1.0 *****************/
typedef unsigned char dtab_byte_t; // for convenience

extern uint64_t dtab_hash_djb2(const char * str); // slightly faster
extern uint64_t dtab_hash_sdbm(const char * str);
#define DTAB_HASH(name) dtab_hash_djb2(name)

#define DTAB_LEN_INIT 16
#define DTAB_NUM_INIT 1
#define DTAB_NULL 0
#define DTAB_GROWTH_FACTOR 2

struct dtab {
    size_t bytesize;
    size_t len; /* allocated length */
    size_t num; /* number of active elements (num < len) */
    size_t * keys;
    void * values;
};

extern void * dtab_get(struct dtab * dtab_ptr, uint64_t in_hash);
extern void dtab_add(struct dtab * dtab_ptr, void * value, uint64_t in_hash);
extern size_t dtab_found(struct dtab * dtab_ptr, uint64_t in_hash);
extern void dtab_del(struct dtab * dtab_ptr, uint64_t in_hash);
extern void dtab_del_scramble(struct dtab * dtab_ptr, uint64_t in_hash);

#define DTAB_STRINGIFY(name) #name

/* NOTE: no do {} while(0); for DTAB_INIT to be able to do:
        dtab_ptr = DTAB_INIT(dtab_ptr, type)
which becomes
dtab_ptr = dtab_ptr = malloc(sizeof(*dtab_ptr));
...
*/
#define DTAB_INIT(dtab_ptr, type) dtab_ptr = malloc(sizeof(*dtab_ptr));\
dtab_ptr->len = DTAB_LEN_INIT;\
dtab_ptr->num = DTAB_NUM_INIT;\
dtab_ptr->values = calloc(DTAB_LEN_INIT, sizeof(type));\
dtab_ptr->keys = malloc(sizeof(*dtab_ptr->keys) * (DTAB_LEN_INIT));\
dtab_ptr->keys[DTAB_NULL] = DTAB_NULL;\
dtab_ptr->bytesize = sizeof(type);

#define DTAB_GROW(dtab_ptr) do {\
    dtab_ptr->len*=DTAB_GROWTH_FACTOR;\
    dtab_ptr->keys = realloc(dtab_ptr->keys, dtab_ptr->len * sizeof(uint64_t));\
    dtab_ptr->values = realloc(dtab_ptr->values, dtab_ptr->len * dtab_ptr->bytesize);} while(0)
#define DTAB_FREE(dtab_ptr) do {free(dtab_ptr->keys) ;\
free(dtab_ptr->values);\
free(dtab_ptr); } while(0)

/* DTAB macros
Might be faster to put hash in variable and call functions directly?
S for stringify, H for hash */

#define DTAB_ADD(dtab_ptr, value, key) dtab_add(dtab_ptr, value, key)
#define DTAB_ADDH(dtab_ptr, value, name) dtab_add(dtab_ptr, value, DTAB_HASH(name))
#define DTAB_ADDSH(dtab_ptr, value, name) dtab_add(dtab_ptr, value, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_GET(dtab_ptr, name) dtab_get(dtab_ptr, name)
#define DTAB_GETH(dtab_ptr, name) dtab_get(dtab_ptr, DTAB_HASH(name))
#define DTAB_GETSH(dtab_ptr, name) dtab_get(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_DEL(dtab_ptr, name) dtab_del(dtab_ptr, name)
#define DTAB_DELH(dtab_ptr, name) dtab_del(dtab_ptr, DTAB_HASH(name))
#define DTAB_DELSH(dtab_ptr, name) dtab_del(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))
#define DTAB_DEL_SCRAMBLE(dtab_ptr, name) dtab_del_scramble(dtab_ptr, name)
#define DTAB_DEL_SCRAMBLEH(dtab_ptr, name) dtab_del_scramble(dtab_ptr, DTAB_HASH(name))
#define DTAB_DEL_SCRAMBLESH(dtab_ptr, name) dtab_del_scramble(dtab_ptr, DTAB_HASH(DTAB_STRINGIFY(name)))

#endif /* DTAB */

/******************************** CONSTANTS *********************************/

enum NMATH_ITERATIONS_LIMIT {
    NMATH_ITERATIONS_LIMIT = 1000,
};

enum NMATH_ENDPOINTS {
    NMATH_ENDPOINTS_NUM = 2,
};


enum NMATH_2DGRID_NEIGHBORS {
    NMATH_SQUARE_NEIGHBOURS = 4,
    NMATH_HEXAGON_NEIGHBOURS = 6,
};

enum NMATH_SIGHTMAP {
    NMATH_SIGHTMAP_BLOCKED  = 0,
    NMATH_SIGHTMAP_VISIBLE  = 1,
    NMATH_SIGHTMAP_OBSERVER = 2,
    NMATH_SIGHTMAP_WALL     = 3,
    NMATH_SIGHTMAP_ENEMY    = 4,
    NMATH_SIGHTMAP_FRIENDLY = 5,
};

enum NMATH_MODE_MOVETILE {
    NMATH_MOVETILE_EXCLUDE = 0,
    NMATH_MOVETILE_INCLUDE = 1,
};

enum NMATH_ATTACKMAP {
    NMATH_ATTACKMAP_BLOCKED = 0,
    NMATH_ATTACKMAP_MOVEABLEMIN = 1,
};

enum NMATH_ATTACKFROMMAP {
    NMATH_ATTACKFROM_BLOCKED = 0,
    NMATH_ATTACKFROM_MOVEABLEMIN = 1,
};

enum NMATH_PUSHPULLMAP {
    NMATH_PUSHPULLMAP_UNIT = 0,
    NMATH_PUSHPULLMAP_BLOCKED = -1,
    NMATH_PUSHPULLMAP_MINDIST = 1, // minimal moveable distance
};

enum NMATH_COSTMAP {
    NMATH_COSTMAP_BLOCKED = 0,
    NMATH_COSTMAP_MOVEABLEMIN = 1,
};

#define NMATH_COSTMAP_fBLOCKED 0.0f

enum NMATH_MOVEMAP {
    NMATH_MOVEMAP_BLOCKED = 0,
    NMATH_MOVEMAP_MOVEABLEMIN = 1,
};

enum NMATH_BLOCKMAP {
    NMATH_BLOCKMAP_INIT = 0,
    NMATH_BLOCKMAP_BLOCKED = 0,
    NMATH_BLOCKMAP_MIN = 1,
};

enum NMATH_GRADIENTMAP {
    NMATH_GRADIENTMAP_UNIT = 0,
    NMATH_GRADIENTMAP_BLOCKED = -1,
    NMATH_GRADIENTMAP_MINDIST = 1, // minimal moveable distance
};

enum NMATH_POINTS_MODE {
    NMATH_POINTS_MODE_MATRIX = 0,
    NMATH_POINTS_MODE_LIST = 1,
};

enum NMATH_MODE_PATHS {
    NMATH_PATH_STEP = 0, // i.e. relative path
    NMATH_PATH_POSITION = 1,  // i.e. absolute path
};

#define uint64_t_MAX UINT64_MAX
#define uint_fast64_t_MAX UINT64_MAX
#define uint32_t_MAX UINT32_MAX
#define uint_fast32_t_MAX UINT32_MAX
#define uint16_t_MAX UINT16_MAX
#define uint_fast16_t_MAX UINT16_MAX
#define uint8_t_MAX UINT8_MAX
#define uint_fast8_t_MAX UINT8_MAX
#define int64_t_MAX INT64_MAX
#define int_fast64_t_MAX INT64_MAX
#define int32_t_MAX INT32_MAX
#define int_fast32_t_MAX INT32_MAX
#define int16_t_MAX INT16_MAX
#define int_fast16_t_MAX INT16_MAX
#define int8_t_MAX INT8_MAX
#define int_fast8_t_MAX INT8_MAX

#ifndef TEMPLATE_TYPES_UINT
#define TEMPLATE_TYPES_UINT REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(uint64_t) \
REGISTER_ENUM(uint_fast8_t) \
REGISTER_ENUM(uint_fast16_t) \
REGISTER_ENUM(uint_fast32_t) \
REGISTER_ENUM(uint_fast64_t)
#endif  /* TEMPLATE_TYPES_UINT */

#ifndef TEMPLATE_TYPES_SINT
#define TEMPLATE_TYPES_SINT REGISTER_ENUM(int8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(int_fast8_t) \
REGISTER_ENUM(int_fast16_t) \
REGISTER_ENUM(int_fast32_t) \
REGISTER_ENUM(int_fast64_t)
#endif  /* TEMPLATE_TYPES_SINT */

#ifndef TEMPLATE_TYPES_INT
#define TEMPLATE_TYPES_INT  TEMPLATE_TYPES_SINT \
TEMPLATE_TYPES_UINT
#endif  /* TEMPLATE_TYPES_INT */

#ifndef TEMPLATE_TYPES_FLOAT
#define TEMPLATE_TYPES_FLOAT REGISTER_ENUM(float) \
REGISTER_ENUM(double)
#endif  /*TEMPLATE_TYPES_FLOAT */

#ifndef TEMPLATE_TYPES_BOOL
#define TEMPLATE_TYPES_BOOL REGISTER_ENUM(bool)
#endif  /* TEMPLATE_TYPES_BOOL */

enum NMATH_ZEROS_INT {
    NMATH_ZERO_uint8_t = 0,
    NMATH_ZERO_uint_fast8_t = 0,
    NMATH_ZERO_uint16_t = 0,
    NMATH_ZERO_uint_fast16_t = 0,
    NMATH_ZERO_uint32_t = 0,
    NMATH_ZERO_uint_fast32_t = 0,
    NMATH_ZERO_uint64_t = 0,
    NMATH_ZERO_uint_fast64_t = 0,
    NMATH_ZERO_int8_t = 0,
    NMATH_ZERO_int_fast8_t = 0,
    NMATH_ZERO_int16_t = 0,
    NMATH_ZERO_int_fast16_t = 0,
    NMATH_ZERO_int32_t = 0,
    NMATH_ZERO_int_fast32_t = 0,
    NMATH_ZERO_int64_t = 0,
    NMATH_ZERO_int_fast64_t = 0,
};

enum NMATH_ONES_INT {
    NMATH_ONE_uint8_t = 1,
    NMATH_ONE_uint_fast8_t = 1,
    NMATH_ONE_uint16_t = 1,
    NMATH_ONE_uint_fast16_t = 1,
    NMATH_ONE_uint32_t = 1,
    NMATH_ONE_uint_fast32_t = 1,
    NMATH_ONE_uint64_t = 1,
    NMATH_ONE_uint_fast64_t = 1,
    NMATH_ONE_int8_t = 1,
    NMATH_ONE_int_fast8_t = 1,
    NMATH_ONE_int16_t = 1,
    NMATH_ONE_int_fast16_t = 1,
    NMATH_ONE_int32_t = 1,
    NMATH_ONE_int_fast32_t = 1,
    NMATH_ONE_int64_t = 1,
    NMATH_ONE_int_fast64_t = 1,
};


#define NMATH_ZERO_float 0.0f
#define NMATH_ZERO_double 0.0f
#define NMATH_ONE_float 1.0f
#define NMATH_ONE_double 1.0f

enum DIMENSIONS {
    NMATH_ONE_D = 1,
    NMATH_TWO_D = 2,
    NMATH_THREE_D = 3,
};
#define NMATH_MINLEN 12
#define NMATH_MIN(a, b) ((a) >= (b) ? (b) : (a))
#define NMATH_MAX(a, b) ((a) >= (b) ? (a) : (b))

/******************************** STRUCTS ****************************/

#define REGISTER_ENUM(type) extern struct nmath_sq_neighbors_##type {\
type right;\
type top;\
type left;\
type bottom;\
} nmath_sq_neighbors_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hex_neighbors_##type {\
type right;\
type top;\
type left;\
type bottom;\
type front;\
type behind;\
} nmath_hex_neighbors_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_point_##type {\
type x;\
type y;\
} nmath_point_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_point3D_##type {\
type x;\
type y;\
type z;\
} nmath_point3D_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hexpoint_##type {\
type x;\
type y;\
type z;\
} nmath_hexpoint_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_node_##type {\
type x;\
type y;\
type distance;\
type cost;\
} nmath_node_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_nodeq_##type {\
type x;\
type y;\
type priority;\
type cost;\
} nmath_nodeq_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_node_##type {\
int32_t x;\
int32_t y;\
type distance;\
} nmath_node_##type##_default;
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_node3D_##type {\
type x;\
type y;\
type z;\
type distance;\
} nmath_node3D_##type##_default;
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern struct nmath_hexnode_##type {\
type x;\
type y;\
type z;\
type distance;\
} nmath_nodehex_##type##_default;
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

/******************************** UTILITIES **********************************/

#define REGISTER_ENUM(type) extern type nmath_inbounds_##type(type pos, type boundmin, type boundmax);

TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

/* --- QUICK MATH --- */

#ifndef log2 /* because tcc SUCKS, does NOT DEFINE log2 */
#define log2(x) (log(x)/log(2.0f))
#endif

/* -- SAFE MATH -- */

/* -- bounded operations -- */
/* NOTE: INTENDED FOR INTEGERS, WORKS FOR FLOATS SOMETIMES */
/* No division, integer division always tends to 0 */
/* Integer multiplication always tends to bounds. */

/* b > 0*/
#define nmath_bplus(a, b, max) (a) > (max - (b)) ? max : (a) + (b);
#define nmath_bplus_int8_t(       a, b) a > (INT8_MAX        - b) ? INT8_MAX        : a + b;
#define nmath_bplus_uint8_t(      a, b) a > (UINT8_MAX       - b) ? UINT8_MAX       : a + b;
#define nmath_bplus_int16_t(      a, b) a > (INT16_MAX       - b) ? INT16_MAX       : a + b;
#define nmath_bplus_uint16_t(     a, b) a > (UINT16_MAX      - b) ? UINT16_MAX      : a + b;
#define nmath_bplus_int32_t(      a, b) a > (INT32_MAX       - b) ? INT32_MAX       : a + b;
#define nmath_bplus_uint32_t(     a, b) a > (UINT32_MAX      - b) ? UINT32_MAX      : a + b;
#define nmath_bplus_int64_t(      a, b) a > (INT64_MAX       - b) ? INT64_MAX       : a + b;
#define nmath_bplus_uint64_t(     a, b) a > (UINT64_MAX      - b) ? UINT64_MAX      : a + b;
#define nmath_bplus_int_fast8_t(  a, b) a > (INT_FAST8_MAX   - b) ? INT_FAST8_MAX   : a + b;
#define nmath_bplus_int_fast16_t( a, b) a > (INT_FAST16_MAX  - b) ? INT_FAST16_MAX  : a + b;
#define nmath_bplus_int_fast32_t( a, b) a > (INT_FAST32_MAX  - b) ? INT_FAST32_MAX  : a + b;
#define nmath_bplus_int_fast64_t( a, b) a > (INT_FAST64_MAX  - b) ? INT_FAST64_MAX  : a + b;
#define nmath_bplus_uint_fast8_t( a, b) a > (UINT_FAST8_MAX  - b) ? UINT_FAST8_MAX  : a + b;
#define nmath_bplus_uint_fast16_t(a, b) a > (UINT_FAST16_MAX - b) ? UINT_FAST16_MAX : a + b;
#define nmath_bplus_uint_fast32_t(a, b) a > (UINT_FAST32_MAX - b) ? UINT_FAST32_MAX : a + b;
#define nmath_bplus_uint_fast64_t(a, b) a > (UINT_FAST64_MAX - b) ? UINT_FAST64_MAX : a + b;

#define nmath_bminus(a, b, min) (a < min + b) ? min : a - b;
#define nmath_bminus_int8_t(       a, b) a < (INT8_MIN       + b) ? INT8_MIN        : a - b;
#define nmath_bminus_uint8_t(      a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_int16_t(      a, b) a < (INT16_MIN      + b) ? INT16_MIN       : a - b;
#define nmath_bminus_uint16_t(     a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_int32_t(      a, b) a < (INT32_MIN      + b) ? INT32_MIN       : a - b;
#define nmath_bminus_uint32_t(     a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_int64_t(      a, b) a < (INT64_MIN      + b) ? INT64_MIN       : a - b;
#define nmath_bminus_uint64_t(     a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_int_fast8_t(  a, b) a < (INT_FAST8_MIN  + b) ? INT_FAST8_MIN   : a - b;
#define nmath_bminus_int_fast16_t( a, b) a < (INT_FAST16_MIN + b) ? INT_FAST16_MIN  : a - b;
#define nmath_bminus_int_fast32_t( a, b) a < (INT_FAST32_MIN + b) ? INT_FAST32_MIN  : a - b;
#define nmath_bminus_int_fast64_t( a, b) a < (INT_FAST64_MIN + b) ? INT_FAST64_MIN  : a - b;
#define nmath_bminus_uint_fast8_t( a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_uint_fast16_t(a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_uint_fast32_t(a, b) a <          b           ?       0         : a - b;
#define nmath_bminus_uint_fast64_t(a, b) a <          b           ?       0         : a - b;

/* positive result */
#define nmath_bmultp(a, b, max) a > (max / b)? max : a * b;
#define nmath_bmultp_int8_t(       a, b) a > (INT8_MAX        / b) ? INT8_MAX        : a * b;
#define nmath_bmultp_uint8_t(      a, b) a > (UINT8_MAX       / b) ? UINT8_MAX       : a * b;
#define nmath_bmultp_int16_t(      a, b) a > (INT16_MAX       / b) ? INT16_MAX       : a * b;
#define nmath_bmultp_uint16_t(     a, b) a > (UINT16_MAX      / b) ? UINT16_MAX      : a * b;
#define nmath_bmultp_int32_t(      a, b) a > (INT32_MAX       / b) ? INT32_MAX       : a * b;
#define nmath_bmultp_uint32_t(     a, b) a > (UINT32_MAX      / b) ? UINT32_MAX      : a * b;
#define nmath_bmultp_int64_t(      a, b) a > (INT64_MAX       / b) ? INT64_MAX       : a * b;
#define nmath_bmultp_uint64_t(     a, b) a > (UINT64_MAX      / b) ? UINT64_MAX      : a * b;
#define nmath_bmultp_int_fast8_t(  a, b) a > (INT_FAST8_MAX   / b) ? INT_FAST8_MAX   : a * b;
#define nmath_bmultp_int_fast16_t( a, b) a > (INT_FAST16_MAX  / b) ? INT_FAST16_MAX  : a * b;
#define nmath_bmultp_int_fast32_t( a, b) a > (INT_FAST32_MAX  / b) ? INT_FAST32_MAX  : a * b;
#define nmath_bmultp_int_fast64_t( a, b) a > (INT_FAST64_MAX  / b) ? INT_FAST64_MAX  : a * b;
#define nmath_bmultp_uint_fast8_t( a, b) a > (UINT_FAST8_MAX  / b) ? UINT_FAST8_MAX  : a * b;
#define nmath_bmultp_uint_fast16_t(a, b) a > (UINT_FAST16_MAX / b) ? UINT_FAST16_MAX : a * b;
#define nmath_bmultp_uint_fast32_t(a, b) a > (UINT_FAST32_MAX / b) ? UINT_FAST32_MAX : a * b;
#define nmath_bmultp_uint_fast64_t(a, b) a > (UINT_FAST64_MAX / b) ? UINT_FAST64_MAX : a * b;

/* negative result */
#define nmath_bmultn(a, b, min) (a < min / b) ? min : a - b;
#define nmath_bmultn_int8_t(       a, b) a < (INT8_MIN       / b) ? INT8_MIN         : a * b;
#define nmath_bmultn_uint8_t(      a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_int16_t(      a, b) a < (INT16_MIN      / b) ? INT16_MIN        : a * b;
#define nmath_bmultn_uint16_t(     a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_int32_t(      a, b) a < (INT32_MIN      / b) ? INT32_MIN        : a * b;
#define nmath_bmultn_uint32_t(     a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_int64_t(      a, b) a < (INT64_MIN      / b) ? INT64_MIN        : a * b;
#define nmath_bmultn_uint64_t(     a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_int_fast8_t(  a, b) a < (INT_FAST8_MIN  / b) ? INT_FAST8_MIN    : a * b;
#define nmath_bmultn_int_fast16_t( a, b) a < (INT_FAST16_MIN / b) ? INT_FAST16_MIN   : a * b;
#define nmath_bmultn_int_fast32_t( a, b) a < (INT_FAST32_MIN / b) ? INT_FAST32_MIN   : a * b;
#define nmath_bmultn_int_fast64_t( a, b) a < (INT_FAST64_MIN / b) ? INT_FAST64_MIN   : a * b;
#define nmath_bmultn_uint_fast8_t( a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_uint_fast16_t(a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_uint_fast32_t(a, b) a <            b         ?     0            : a * b;
#define nmath_bmultn_uint_fast64_t(a, b) a <            b         ?     0            : a * b;


/* -- MULTIPLY N TIMES -- */

extern float nmath_intpow( float base, uint32_t exponent);
extern float nmath_slowpow(float base, uint32_t exponent);

/* DOES NOT WORK IF n is NOT A NUMBER TOKEN */
#define nmath_fastpow(s, n) nmath_fastpow##n(s)
#define nmath_fastpow1(s)  s
#define nmath_fastpow2(s)  nmath_fastpow1(s)  * nmath_fastpow1(s)
#define nmath_fastpow3(s)  nmath_fastpow1(s)  * nmath_fastpow2(s)
#define nmath_fastpow4(s)  nmath_fastpow1(s)  * nmath_fastpow3(s)
#define nmath_fastpow5(s)  nmath_fastpow1(s)  * nmath_fastpow4(s)
#define nmath_fastpow6(s)  nmath_fastpow1(s)  * nmath_fastpow5(s)
#define nmath_fastpow7(s)  nmath_fastpow1(s)  * nmath_fastpow6(s)
#define nmath_fastpow8(s)  nmath_fastpow1(s)  * nmath_fastpow7(s)
#define nmath_fastpow9(s)  nmath_fastpow1(s)  * nmath_fastpow8(s)
#define nmath_fastpow10(s) nmath_fastpow1(s)  * nmath_fastpow9(s)
#define nmath_fastpow11(s) nmath_fastpow1(s)  * nmath_fastpow10(s)
#define nmath_fastpow12(s) nmath_fastpow2(s)  * nmath_fastpow10(s)
#define nmath_fastpow13(s) nmath_fastpow3(s)  * nmath_fastpow10(s)
#define nmath_fastpow14(s) nmath_fastpow4(s)  * nmath_fastpow10(s)
#define nmath_fastpow15(s) nmath_fastpow5(s)  * nmath_fastpow10(s)
#define nmath_fastpow16(s) nmath_fastpow6(s)  * nmath_fastpow10(s)
#define nmath_fastpow17(s) nmath_fastpow7(s)  * nmath_fastpow10(s)
#define nmath_fastpow18(s) nmath_fastpow8(s)  * nmath_fastpow10(s)
#define nmath_fastpow19(s) nmath_fastpow9(s)  * nmath_fastpow10(s)
#define nmath_fastpow20(s) nmath_fastpow10(s) * nmath_fastpow10(s)
#define nmath_fastpow21(s) nmath_fastpow1(s)  * nmath_fastpow20(s)
#define nmath_fastpow22(s) nmath_fastpow2(s)  * nmath_fastpow20(s)
#define nmath_fastpow23(s) nmath_fastpow3(s)  * nmath_fastpow20(s)
#define nmath_fastpow24(s) nmath_fastpow4(s)  * nmath_fastpow20(s)
#define nmath_fastpow25(s) nmath_fastpow5(s)  * nmath_fastpow20(s)
#define nmath_fastpow26(s) nmath_fastpow6(s)  * nmath_fastpow20(s)
#define nmath_fastpow27(s) nmath_fastpow7(s)  * nmath_fastpow20(s)
#define nmath_fastpow28(s) nmath_fastpow8(s)  * nmath_fastpow20(s)
#define nmath_fastpow29(s) nmath_fastpow9(s)  * nmath_fastpow20(s)
#define nmath_fastpow30(s) nmath_fastpow10(s) * nmath_fastpow20(s)
#define nmath_fastpow31(s) nmath_fastpow1(s)  * nmath_fastpow30(s)
#define nmath_fastpow32(s) nmath_fastpow2(s)  * nmath_fastpow30(s)
#define nmath_fastpow33(s) nmath_fastpow3(s)  * nmath_fastpow30(s)
#define nmath_fastpow34(s) nmath_fastpow4(s)  * nmath_fastpow30(s)
#define nmath_fastpow35(s) nmath_fastpow5(s)  * nmath_fastpow30(s)
#define nmath_fastpow36(s) nmath_fastpow6(s)  * nmath_fastpow30(s)
#define nmath_fastpow37(s) nmath_fastpow7(s)  * nmath_fastpow30(s)
#define nmath_fastpow38(s) nmath_fastpow8(s)  * nmath_fastpow30(s)
#define nmath_fastpow39(s) nmath_fastpow9(s)  * nmath_fastpow30(s)
#define nmath_fastpow40(s) nmath_fastpow10(s) * nmath_fastpow30(s)
#define nmath_fastpow41(s) nmath_fastpow1(s)  * nmath_fastpow40(s)
#define nmath_fastpow42(s) nmath_fastpow2(s)  * nmath_fastpow40(s)
#define nmath_fastpow43(s) nmath_fastpow3(s)  * nmath_fastpow40(s)
#define nmath_fastpow44(s) nmath_fastpow4(s)  * nmath_fastpow40(s)
#define nmath_fastpow45(s) nmath_fastpow5(s)  * nmath_fastpow40(s)
#define nmath_fastpow46(s) nmath_fastpow6(s)  * nmath_fastpow40(s)
#define nmath_fastpow47(s) nmath_fastpow7(s)  * nmath_fastpow40(s)
#define nmath_fastpow48(s) nmath_fastpow8(s)  * nmath_fastpow40(s)
#define nmath_fastpow49(s) nmath_fastpow9(s)  * nmath_fastpow40(s)
#define nmath_fastpow50(s) nmath_fastpow10(s) * nmath_fastpow40(s)
#define nmath_fastpow51(s) nmath_fastpow1(s)  * nmath_fastpow50(s)
#define nmath_fastpow52(s) nmath_fastpow2(s)  * nmath_fastpow50(s)
#define nmath_fastpow53(s) nmath_fastpow3(s)  * nmath_fastpow50(s)
#define nmath_fastpow54(s) nmath_fastpow4(s)  * nmath_fastpow50(s)
#define nmath_fastpow55(s) nmath_fastpow5(s)  * nmath_fastpow50(s)
#define nmath_fastpow56(s) nmath_fastpow6(s)  * nmath_fastpow50(s)
#define nmath_fastpow57(s) nmath_fastpow7(s)  * nmath_fastpow50(s)
#define nmath_fastpow58(s) nmath_fastpow8(s)  * nmath_fastpow50(s)
#define nmath_fastpow59(s) nmath_fastpow9(s)  * nmath_fastpow50(s)
#define nmath_fastpow60(s) nmath_fastpow10(s) * nmath_fastpow50(s)
#define nmath_fastpow61(s) nmath_fastpow1(s)  * nmath_fastpow60(s)
#define nmath_fastpow62(s) nmath_fastpow2(s)  * nmath_fastpow60(s)
#define nmath_fastpow63(s) nmath_fastpow3(s)  * nmath_fastpow60(s)
#define nmath_fastpow64(s) nmath_fastpow4(s)  * nmath_fastpow60(s)
#define nmath_fastpow65(s) nmath_fastpow5(s)  * nmath_fastpow60(s)
#define nmath_fastpow66(s) nmath_fastpow6(s)  * nmath_fastpow60(s)
#define nmath_fastpow67(s) nmath_fastpow7(s)  * nmath_fastpow60(s)
#define nmath_fastpow68(s) nmath_fastpow8(s)  * nmath_fastpow60(s)
#define nmath_fastpow69(s) nmath_fastpow9(s)  * nmath_fastpow60(s)
#define nmath_fastpow70(s) nmath_fastpow10(s) * nmath_fastpow60(s)
#define nmath_fastpow71(s) nmath_fastpow1(s)  * nmath_fastpow70(s)
#define nmath_fastpow72(s) nmath_fastpow2(s)  * nmath_fastpow70(s)
#define nmath_fastpow73(s) nmath_fastpow3(s)  * nmath_fastpow70(s)
#define nmath_fastpow74(s) nmath_fastpow4(s)  * nmath_fastpow70(s)
#define nmath_fastpow75(s) nmath_fastpow5(s)  * nmath_fastpow70(s)
#define nmath_fastpow76(s) nmath_fastpow6(s)  * nmath_fastpow70(s)
#define nmath_fastpow77(s) nmath_fastpow7(s)  * nmath_fastpow70(s)
#define nmath_fastpow78(s) nmath_fastpow8(s)  * nmath_fastpow70(s)
#define nmath_fastpow79(s) nmath_fastpow9(s)  * nmath_fastpow70(s)
#define nmath_fastpow80(s) nmath_fastpow10(s) * nmath_fastpow70(s)
#define nmath_fastpow81(s) nmath_fastpow1(s)  * nmath_fastpow80(s)
#define nmath_fastpow82(s) nmath_fastpow2(s)  * nmath_fastpow80(s)
#define nmath_fastpow83(s) nmath_fastpow3(s)  * nmath_fastpow80(s)
#define nmath_fastpow84(s) nmath_fastpow4(s)  * nmath_fastpow80(s)
#define nmath_fastpow85(s) nmath_fastpow5(s)  * nmath_fastpow80(s)
#define nmath_fastpow86(s) nmath_fastpow6(s)  * nmath_fastpow80(s)
#define nmath_fastpow87(s) nmath_fastpow7(s)  * nmath_fastpow80(s)
#define nmath_fastpow88(s) nmath_fastpow8(s)  * nmath_fastpow80(s)
#define nmath_fastpow89(s) nmath_fastpow9(s)  * nmath_fastpow80(s)
#define nmath_fastpow90(s) nmath_fastpow10(s) * nmath_fastpow80(s)
#define nmath_fastpow91(s) nmath_fastpow1(s)  * nmath_fastpow90(s)
#define nmath_fastpow92(s) nmath_fastpow2(s)  * nmath_fastpow90(s)
#define nmath_fastpow93(s) nmath_fastpow3(s)  * nmath_fastpow90(s)
#define nmath_fastpow94(s) nmath_fastpow4(s)  * nmath_fastpow90(s)
#define nmath_fastpow95(s) nmath_fastpow5(s)  * nmath_fastpow90(s)
#define nmath_fastpow96(s) nmath_fastpow6(s)  * nmath_fastpow90(s)
#define nmath_fastpow97(s) nmath_fastpow7(s)  * nmath_fastpow90(s)
#define nmath_fastpow98(s) nmath_fastpow8(s)  * nmath_fastpow90(s)
#define nmath_fastpow99(s) nmath_fastpow9(s)  * nmath_fastpow90(s)

/************************* PERIODIC CYCLES *****************************/
/* m:-1, p:1, z:0 */
#define q_cycle2_zp(i) (i % 2)
#define q_cycle2_pz(i) ((i + 1) % 2)
#define q_cycle2_mz(i) ((i % 2) - 1)
#define q_cycle2_zm(i) (((i + 1) % 2) - 1)
#define q_cycle2_mp(i) (q_cycle2_mz(i) + q_cycle2_zp(i))
#define q_cycle2_pm(i) (q_cycle2_pz(i) + q_cycle2_zm(i))

#define q_cycle3_mzp(i) ((i % 3) - 1)
#define q_cycle3_zpm(i) (((i + 1) % 3) - 1)
#define q_cycle3_pmz(i) (((i + 2) % 3) - 1)
#define q_cycle3_pzm(i) (1 - (i % 3))
#define q_cycle3_zmp(i) (1 - ((i + 1) % 3))
#define q_cycle3_mpz(i) (1 - ((i + 2) % 3))

#define q_cycle4_mzp2(i) ((i % 4) - 1)
#define q_cycle4_zp2m(i) (((i + 1) % 4) - 1)
#define q_cycle4_p2mz(i) (((i + 2) % 4) - 1)
#define q_cycle4_2mzp(i) (((i + 3) % 4) - 1)
#define q_cycle4_mzpz(i) (q_cycle2_pz(i) * q_cycle4_mzp2(i))
#define q_cycle4_pzmz(i) (q_cycle2_mz(i) * q_cycle4_mzp2(i))
#define q_cycle4_zmzp(i) (q_cycle2_zp(i) * q_cycle4_2mzp(i))
#define q_cycle4_zpzm(i) (q_cycle2_zp(i) * q_cycle4_zp2m(i))

#define q_cycle4_mmpp(i) (q_cycle4_mzpz(i) + q_cycle4_zmzp(i))
#define q_cycle4_pmmp(i) (q_cycle4_pzmz(i) + q_cycle4_zmzp(i))
#define q_cycle4_mppm(i) (q_cycle4_mzpz(i) + q_cycle4_zpzm(i))
#define q_cycle4_ppmm(i) (q_cycle4_pzmz(i) + q_cycle4_zpzm(i))

#define q_cycle6_mzpzzz(i) (q_cycle3_mzp(i) * q_cycle2_pz(i))
#define q_cycle6_zzzmzp(i) (q_cycle3_mzp(i) * q_cycle2_zp(i))
#define q_cycle6_zzmzpz(i) (q_cycle3_zpm(i) * q_cycle2_pz(i))
#define q_cycle6_zpzzzm(i) (q_cycle3_zpm(i) * q_cycle2_zp(i))
#define q_cycle6_pzzzmz(i) (q_cycle3_pmz(i) * q_cycle2_pz(i))
#define q_cycle6_zmzpzz(i) (q_cycle3_pmz(i) * q_cycle2_zp(i))

#define q_cycle6_mzzzpz(i) (q_cycle3_mpz(i) * q_cycle2_pz(i))
#define q_cycle6_zpzmzz(i) (q_cycle3_mpz(i) * q_cycle2_zp(i))
#define q_cycle6_pzmzzz(i) (q_cycle3_pzm(i) * q_cycle2_pz(i))
#define q_cycle6_zzzpzm(i) (q_cycle3_pzm(i) * q_cycle2_zp(i))
#define q_cycle6_zzpzmz(i) (q_cycle3_zmp(i) * q_cycle2_pz(i))
#define q_cycle6_zmzzzp(i) (q_cycle3_zmp(i) * q_cycle2_zp(i))

#define q_cycle6_mppmzz(i) (q_cycle6_mzpzzz(i) + q_cycle6_zpzmzz(i))
#define q_cycle6_zzmppm(i) (q_cycle6_zzzpzm(i) + q_cycle6_zzmzpz(i))
#define q_cycle6_pmzzmp(i) (q_cycle6_zmzzzp(i) + q_cycle6_pzzzmz(i))

extern int32_t q_sequence_fgeometric_int32_t(int32_t start, int32_t destination, float geo_factor);

#define REGISTER_ENUM(type) extern type q_sequence_geometric_##type(type start, type destination, type geo_factor);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type q_sequence_pingpong_##type(type current, type upper, type lower);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern float q_sqrt_##type(type in_int);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define  carmack_sqrt_int8_t    q_sqrt_int8_t
#define  carmack_sqrt_uint8_t   q_sqrt_uint8_t
#define  carmack_sqrt_int16_t   q_sqrt_int16_t
#define  carmack_sqrt_uint16_t  q_sqrt_uint16_t
#define  carmack_sqrt_int32_t   q_sqrt_int32_t
#define  carmack_sqrt_uint32_t  q_sqrt_uint32_t
#define  carmack_sqrt_int64_t   q_sqrt_int64_t
#define  carmack_sqrt_uint64_t  q_sqrt_uint64_t
#define  carmack_sqrt_float     q_sqrt_float
#define  carmack_sqrt_double    q_sqrt_double

/***************************** FIRST SET BIT **********************************/
#define REGISTER_ENUM(type) type nmath_firstsetBit_##type(type flags);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

/***************************** SWAPPING **********************************/
// swapping two elements in an array.
#define REGISTER_ENUM(type) void nmath_swap_##type(type * arr, type i1, type i2);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

/***************************** INDICES&ORDERS **********************************/
// indices: array of unique indices with a certain order.
// sparseOrders: order is the position (+1) of the index in the indices array.
//      sparse cause many elements might be 0 (which is NULL).
// indices[order1 - 1] = index1 <-> sparseOrders[index1] = order1
// EXAMPLE:
//      indices is {4,6,12,5}
//      sparseOrders is {0,0,0,0,1,3,2,0,0,0,0,0,4} -> 0 means NULL
//      indices[1 - 1] = 4 <-> sparseOrders[4] = 1
// USE CASE: finding order of index in indices is O(n), sparseOrders[index] is O(1)
//           if necessary to find order of index many times.


#define REGISTER_ENUM(type) extern type * indices2sparseOrders_##type(type * in_orders_sparse, size_t orders_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * sparseOrders2indices_##type(type * in_indices, size_t num_indices);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

/********************************* LINALG ************************************/
// linalg uses unraveled arrays as n-dimensional matrices
// linalg design: let caller take care of memory. -> NO MEMORY CREATION FOR EACH AND EVERY linalg FUNCTION CALLS.
// col->x, row->y, depth->z
#define linalg_index_arr2D(row, col, col_len) (row * col_len + col)
#define linalg_index_arr3D(row, col, depth, row_len, col_len) (row * col_len * depth_len + col * depth_len + depth)

#define REGISTER_ENUM(type) extern type linalg_determinant_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_trace_##type(type * square_mat, size_t sq_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_crossProduct_noM_##type(type * product, type * vec3D1, type * vec3D2);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_uniques_##type(type * array, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_isIn_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_1D_##type(type * list_1D, size_t list_len, type x);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_2D_##type(type * list_1D, size_t list_len, type x, type y);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_3D_##type(type * list_1D, size_t list_len, type x, type y, type z);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * linalg_where_##type(type * array, type to_find, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern void linalg_matrix_print_##type(type * array, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_greater_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_greater_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_geq_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_geq_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_smaller_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_smaller_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

// s for scalar
#define REGISTER_ENUM(type) extern type * linalg_seq_noM_h##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_seq_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sgreater_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM


#define REGISTER_ENUM(type) extern type * linalg_sgreater_##type(type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sgeq_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sgeq_##type(type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_ssmaller_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_ssmaller_##type(type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sseq_noM_##type(type * out, type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sseq_##type(type * matrix1, type tocompare, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_equal_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool * linalg_equal_noM_##type(bool * out, type * matrix1, type * matrix2, size_t arr_len, type tolerance);
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance);
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_any_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_all_##type(type * matrix1, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_circ_noM_##type(type * out_mat, type origin_x, type origin_y, size_t radius, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_circ_##type(type origin_x, type origin_y, size_t radius, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_rect_noM_##type(type * out_mat, type origin_x, type origin_y, size_t width, size_t height, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_draw_rect_##type(type origin_x, type origin_y, size_t width, size_t height, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_sub_noM_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_noM_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_scalar_noM_##type(type * out, type * matrix, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_scalar_##type(type * matrix, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_minus_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_minus_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_minus_scalar_noM_##type(type * out, type * matrix, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_minus_scalar_##type(type * matrix, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mult_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mult_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mult_scalar_##type(type * matrix1, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_div_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_div_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_div_scalar_noM_##type(type * out, type * matrix1, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_div_scalar_##type(type * matrix1, type value, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_and_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_or_noM_##type(type * out, type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mask_noM_##type(type * out, type * matrix, type * mask, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_matrix2list_noM_##type(type * matrix, type * list, size_t row_len, size_t col_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_list2matrix_noM_##type(type * out, type * list, size_t row_len, size_t col_len, size_t list_len);
TEMPLATE_TYPES_INT
TEMPLATE_TYPES_BOOL
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_distance_manhattan_point_##type(struct nmath_point_##type start, struct nmath_point_##type end);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_distance_manhattan_##type(type x_0, type y_0, type x_1, type y_1);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_distance_euclidian_##type(struct nmath_point_##type start, struct nmath_point_##type end);
TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

/******************************* PATHFINDING ***********************************/

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_noM_##type(type * sightmap, type * blockmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type sight);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_##type(type * blockmap, size_t row_len, size_t col_len, struct nmath_point_##type start, type sight, uint8_t mode_output);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Attackto_noM_##type(type * attackto_matrix, type * move_matrix, size_t row_len, size_t col_len, type move, uint8_t range[2], uint8_t mode_movetile);
// TEMPLATE_TYPES_INT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Attackto_##type(type * move_matrix, size_t row_len, size_t col_len, type move, uint8_t range[2], uint8_t mode_output, uint8_t mode_movetile);
// TEMPLATE_TYPES_INT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Moveto_Hex_##type(type * costmap, size_t row_len, size_t depth_len, struct nmath_hexpoint_##type start, type move, uint8_t mode_output);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Visible_Hex_##type(type * blockmap, size_t depth_len, size_t col_len, struct nmath_hexpoint_##type  start, type sight, uint8_t mode);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Attackfrom_noM_##type(type * attackfrommap, type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t range[2]);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Attackfrom_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t range[2], uint8_t mode_output);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Pushto_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_Pullto_##type(type * in_movemap, size_t row_len, size_t col_len, struct nmath_point_##type in_target, uint8_t mode_output);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_unitGradient_noM_##type(type * unitgradientmap, type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

// #define REGISTER_ENUM(type) extern type * pathfinding_Map_unitGradient_##type(type * in_costmap, size_t row_len, size_t col_len, struct nmath_point_##type * in_targets, size_t unit_num);
// TEMPLATE_TYPES_SINT
// #undef REGISTER_ENUM

#endif /* NOURSMATH_H */
