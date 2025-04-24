#ifndef CONVOY_H
#define CONVOY_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATINOS --- */
struct Game;
struct Unit;
struct Inventory_item;
struct cJSON;

/* --- Destructors --- */
void Convoy_Free( struct Convoy *conv);
void Convoy_Clear(struct Convoy *conv);

/* --- Items --- */
void Convoy_Swap(struct Convoy *conv, i16 i1, i16 i2);
// Convoy_Deposit_byType inputs type_exp because some weapons have multiple types
void Convoy_Deposit_byType(struct Convoy *c, struct Inventory_item i, i16 exp);
void Convoy_Deposit_byIndex(struct Convoy *c, struct Inventory_item i, i16 exp);

struct Inventory_item Convoy_Withdraw(struct Convoy *conv, i16 i);
b32 Convoy_isFull(struct Convoy *conv);

/* --- Conversion --- */
u16 Convoy_Id2TypeExp(struct Convoy *conv, i16 i);
u16 Convoy_TypeCode2Exp(struct Convoy *conv, u16 typecode);

/* --- Money --- */
i16 Convoy_Earn(struct Convoy *conv, i16 in_money);
i16 Convoy_Spend(struct Convoy *conv, i16 out_money);

/* --- Sorting --- */
void Convoy_Sort(struct Convoy *conv, i16 stattype);
/* quicksort */
void Convoy_Quicksort(struct Convoy *c, u8 a[], u8 l, u8 h);
u8 Convoy_Partition_wDuplicates(struct Convoy *c, u8 a[], u8 l, u8 h);
/* cunum shift */
void Convoy_Shift_Plus(struct Convoy *conv, i16 i, i16 exp);
void Convoy_Shift_Minus(struct Convoy *conv, i16 i);

/* --- Debug --- */
void Convoy_Stats_Print(struct Convoy *c, i16 wpnType, i16 stattype);
void Convoy_AllStats_Print(struct Convoy *conv, i16 wpnType);

/* --- I/O --- */
void Convoy_readJSON(       void *input,  const cJSON *json);
void Convoy_writeJSON(const void *input,        cJSON *json);

#endif /* CONVOY_H */