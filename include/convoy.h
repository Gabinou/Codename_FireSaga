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
#include "SDL.h"
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
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    struct dtab *weapons_dtab;

    struct Inventory_item books[SOTA_BOOKS_NUM];
    struct Inventory_item items[SOTA_CONVOY_SIZE_MAX];
    u8 cumnum[ITEM_TYPE_NUM + 1]; // items are [cumnum1, cumnum2)

    i16 bank; // [gold]
    u8 books_num;
    u8 items_num;
    u8 size;
    bool sort_direction;
} Convoy;

extern struct Convoy Convoy_default;

/* --- (De)Constructors --- */
extern void Convoy_Clear(struct Convoy *in_convoy);

/* --- Items --- */
extern void Convoy_Swap(struct Convoy *in_convoy, i16 i1, i16 i2);
// Convoy_Deposit_byType inputs type_exp because some weapons have multiple types
extern void Convoy_Deposit_byType(struct Convoy *c, struct Inventory_item i, i16 exp);
extern void Convoy_Deposit_byIndex(struct Convoy *c, struct Inventory_item i, i16 exp);

extern struct Inventory_item Convoy_Withdraw(struct Convoy *in_convoy, i16 i);
extern bool Convoy_isFull(struct Convoy *in_convoy);

/* --- Conversion --- */
extern u16 Convoy_Id2TypeExp(struct Convoy *in_convoy, i16 i);
extern u16 Convoy_TypeCode2Exp(struct Convoy *in_convoy, u16 typecode);

/* --- Money --- */
extern i16 Convoy_Earn(struct Convoy *in_convoy, i16 in_money);
extern i16 Convoy_Spend(struct Convoy *in_convoy, i16 out_money);

/* --- Sorting --- */
extern void Convoy_Sort(struct Convoy *in_convoy, i16 stattype);
/* quicksort */
extern void Convoy_Quicksort(struct Convoy *c, u8 a[], u8 l, u8 h);
extern u8 Convoy_Partition_wDuplicates(struct Convoy *c, u8 a[], u8 l, u8 h);
/* cunum shift */
extern void Convoy_Shift_Plus(struct Convoy *in_convoy, i16 i, i16 exp);
extern void Convoy_Shift_Minus(struct Convoy *in_convoy, i16 i);

/* --- Debug --- */
extern void Convoy_Stats_Print(struct Convoy *c, i16 wpnType, i16 stattype);
extern void Convoy_AllStats_Print(struct Convoy *in_convoy, i16 wpnType);

/* --- I/O --- */
extern void Convoy_readJSON(void        *input, const cJSON *const in_json);
extern void Convoy_writeJSON(const void *input,       cJSON       *in_json);

#endif /* CONVOY_H */