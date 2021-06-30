#ifndef LINALG_H
#define LINALG_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**********************DARR: DYNAMIC ARRAYS v1.0******************/

// DARR: Dynamic arrays for C99.
// A darr is a simple array with two additional elements: allocated length and number of active element, saved at positions -2 and -1 respectively.

#define DARR_GROWTH_FACTOR 2
#define DARR_LEN_INDEX 2
#define DARR_NUM_INDEX 1

#define DARR_LEN(darr) (*((size_t *)darr - DARR_LEN_INDEX)) // allocated length
#define DARR_NUM(darr) (*((size_t *)darr - DARR_NUM_INDEX)) // number of active elements

// DARR_INIT: a darr is an array with two size_t at indices -1 and -2, respectively num and len.
#define DARR_INIT(darr, type, len)(type*)(((size_t* )malloc(sizeof(size_t)*DARR_LEN_INDEX + sizeof(type)*(len))) + DARR_LEN_INDEX);\
    DARR_LEN(darr) = len;\
    DARR_NUM(darr) = 0;\

// DARR_REALLOC: DARR internal. Not to be called directly by users.
#define DARR_REALLOC(darr, len) (void *)((size_t* )realloc(((size_t* )darr - DARR_LEN_INDEX), (sizeof(size_t)*DARR_LEN_INDEX + (sizeof(*darr))*(len))) + DARR_LEN_INDEX)

// DARR_GROW: double length of array (depending on DARR_GROWTH_FACTOR)
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

/************************* CONSTANTS *****************************/
#define LINALG_TEMPLATE_TYPES_INT REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t)\
REGISTER_ENUM(bool)

#define LINALG_TEMPLATE_TYPES_FLOAT REGISTER_ENUM(float) \
REGISTER_ENUM(double)

#define LINALG_TEMPLATE_TYPES REGISTER_ENUM(int8_t) \
REGISTER_ENUM(uint8_t) \
REGISTER_ENUM(int16_t) \
REGISTER_ENUM(uint16_t) \
REGISTER_ENUM(int32_t) \
REGISTER_ENUM(uint32_t) \
REGISTER_ENUM(int64_t) \
REGISTER_ENUM(uint64_t) \
REGISTER_ENUM(bool) \
REGISTER_ENUM(float) \
REGISTER_ENUM(double)

enum LINALG_DIMENSIONS {
    ONE_D = 1,
    TWO_D = 2,
    THREE_D = 3,
};
#define LINALG_MINLEN 12

/**********************N-DIMENSIONAL UTILITIES*********************/

// linalg uses unraveled arrays as n-dimensional matrices
// col->x, row->y, depth->z
#define linalg_index_arr2D(row, col, col_len) (row * col_len + col)
#define linalg_index_arr3D(row, col, depth, row_len, col_len) (row * col_len * row_len + col * row_len + depth)

#define REGISTER_ENUM(type) extern type linalg_determinant_##type(type * square_mat, size_t sq_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_trace_##type(type * square_mat, size_t sq_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type linalg_dotProduct_##type(type * arr1, type * arr2, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_crossProduct_##type(type * vec3D1, type * vec3D2);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_isIn_##type(type * array, type to_find, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_1D_##type(type * list_1D, size_t list_len, type x);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_2D_##type(type * list_1D, size_t list_len, type x, type y);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_list_isIn_3D_##type(type * list_1D, size_t list_len, type x, type y, type z);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern size_t * linalg_where_##type(type * array, type to_find, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern void linalg_matrix_print_##type(type * array, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len);
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_any_##type(type * matrix1, size_t arr_len);
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool linalg_all_##type(type * matrix1, size_t arr_len);
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern bool * linalg_equal_##type(type * matrix1, type * matrix2, size_t arr_len, type tolerance);
LINALG_TEMPLATE_TYPES_FLOAT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_plus_##type(type * matrix1, type * matrix2, size_t arr_len, int8_t sign);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_and_##type(type * matrix1, type * matrix2, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_or_##type(type * matrix1, type * matrix2, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_mask_##type(type * matrix, type * mask, size_t arr_len);
LINALG_TEMPLATE_TYPES
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_matrix2list_##type(type * matrix, size_t row_len, size_t col_len);
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#define REGISTER_ENUM(type) extern type * linalg_list2matrix_##type(type * list, size_t row_len, size_t col_len, size_t list_len);
LINALG_TEMPLATE_TYPES_INT
#undef REGISTER_ENUM

#endif /* LINALG_H */
