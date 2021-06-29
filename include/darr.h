
#ifndef DARR
#define DARR

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

#endif /* DARR */
