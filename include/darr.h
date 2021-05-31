
#ifndef DARR_H
#define DARR_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "SDL2/SDL.h"

// Matrices are not DARR -> row_len and col_len are known
// Lists are DARR -> len is unknown

#define DARR_GROWTH_FACTOR 2

#define DARR_LEN(arr) (*((size_t *)arr - 2))
#define DARR_NUM(arr) (*((size_t *)arr - 1))

#define DARR_INIT(type, len) (type*)(((size_t* )malloc(sizeof(size_t)*2 + sizeof(type)*(len)))+2)
#define DARR_REALLOC(arr, len) ((size_t* )realloc(((size_t* )arr - 2), (sizeof(size_t)*2 + (sizeof(*arr))*(len)))+2)
#define DARR_DEL(arr, elem, bytesize, num) memcpy((arr + elem), (arr + (num - 1)), bytesize)

#define DARR_GROW(arr) DARR_LEN(arr)*=DARR_GROWTH_FACTOR;\
arr = DARR_REALLOC(arr, DARR_LEN(arr));

#define DARR_PUT(arr, elem) if ((DARR_NUM(arr) + 1) >= (DARR_LEN(arr))) {\
DARR_GROW(arr);\
}\
arr[DARR_NUM(arr)++] = elem;

#define DARR_POP(arr) arr[--DARR_NUM(arr)]

#define DARR_FREE(arr) free((((size_t* )arr) - 2))

#endif /* DARR_H */
