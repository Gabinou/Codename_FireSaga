#ifndef CONVOY_H
#define CONVOY_H

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "enums.h"
#include "nmath.h"
#include "physfs.h"
#include "jsonio.h"
#include "debug.h"
#include "SDL2/SDL.h"
#include "cJSON.h"
#include "utilities.h"
#include "platform.h"

enum SOTA_CONVOY {
    SOTA_CONVOY_SIZE_MAX = 200,
    SOTA_CONVOY_NULL = 0,
    SOTA_CONVOY_SIZE_START = 50,
    SOTA_CONVOY_SORT_HIGH2LOW = 0,
    SOTA_CONVOY_SORT_LOW2HIGH = 1,
};

typedef struct Convoy {
    char JSONElement[DEFAULT_BUFFER_SIZE]; // JSON_ELEM_bOFFSET = 0
    struct dtab *weapons_dtab;

    struct Inventory_item books[SOTA_BOOKS_NUM];
    struct Inventory_item items[SOTA_CONVOY_SIZE_MAX];
    u8 cumnum[ITEM_TYPE_NUM + 1]; // items are [cumnum1, cumnum2)

    uint_fast16_t bank; // [gold]
    u8 books_num;
    u8 items_num;
    u8 size;
    bool sort_direction;
} Convoy;

extern struct Convoy Convoy_default;

/* --- (De)Constructors --- */
extern void Convoy_Clear(struct Convoy *in_convoy);

/* --- Items --- */
extern void Convoy_Swap(struct Convoy *in_convoy, int_fast16_t i1, int_fast16_t i2);
// Convoy_Deposit_byType inputs type_exp because some weapons have multiple types
extern void Convoy_Deposit_byType(struct Convoy *c, struct Inventory_item i, int_fast16_t exp);
extern void Convoy_Deposit_byIndex(struct Convoy *c, struct Inventory_item i, int_fast16_t exp);

extern struct Inventory_item Convoy_Withdraw(struct Convoy *in_convoy, int_fast16_t i);
extern bool Convoy_isFull(struct Convoy *in_convoy);

/* --- Conversion --- */
extern uf16 Convoy_Id2TypeExp(struct Convoy *in_convoy, int_fast16_t i);
extern uf16 Convoy_TypeCode2Exp(struct Convoy *in_convoy, uint_fast16_t typecode);

/* --- Money --- */
extern int_fast16_t Convoy_Earn(struct Convoy *in_convoy, int_fast16_t in_money);
extern int_fast16_t Convoy_Spend(struct Convoy *in_convoy, int_fast16_t out_money);

/* --- Sorting --- */
extern void Convoy_Sort(struct Convoy *in_convoy, int_fast16_t stattype);
/* quicksort */
extern void Convoy_Quicksort(struct Convoy *c, u8 a[], u8 l, u8 h);
extern u8 Convoy_Partition_wDuplicates(struct Convoy *c, u8 a[],
                                       u8 l, u8 h);
/* cunum shift */
extern void Convoy_Shift_Plus(struct Convoy *in_convoy, int_fast16_t i, int_fast16_t exp);
extern void Convoy_Shift_Minus(struct Convoy *in_convoy, int_fast16_t i);

/* --- Debug --- */
extern void Convoy_Stats_Print(struct Convoy *c, int_fast16_t wpnType, int_fast16_t stattype);
extern void Convoy_AllStats_Print(struct Convoy *in_convoy, int_fast16_t wpnType);

/* --- I/O --- */
extern void Convoy_readJSON(void        *input, const cJSON *const in_json);
extern void Convoy_writeJSON(const void *input,       cJSON       *in_json);

#endif /* CONVOY_H */