#ifndef CONVOY_H
#define CONVOY_H

#include <stdio.h>
#include <math.h>
#include "filesystem.h"
#include "enums.h"
#include "narrative.h"
#include "physfs.h"
#include "jsonio.h"
#include "SDL2/SDL.h"
#include "cJSON.h"
#include "utilities.h"
#include "platform.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct Convoy {
    char JSONElement[DEFAULT_BUFFER_SIZE];
    uint16_t bank;
    uint8_t booksnum;
    bool full;
    struct Weapons_hash * weapons;
    struct Quantity quantity;
    struct Inventory_item books[DEFAULT_BOOKS_NUM];
    struct Inventory_item swords[DEFAULT_CONVOY_SIZE];
    struct Inventory_item lances[DEFAULT_CONVOY_SIZE];
    struct Inventory_item axes[DEFAULT_CONVOY_SIZE];
    struct Inventory_item bows[DEFAULT_CONVOY_SIZE];
    struct Inventory_item trinkets[DEFAULT_CONVOY_SIZE];
    struct Inventory_item offhands[DEFAULT_CONVOY_SIZE];
    struct Inventory_item elemental[DEFAULT_CONVOY_SIZE];
    struct Inventory_item demonic[DEFAULT_CONVOY_SIZE];
    struct Inventory_item angelic[DEFAULT_CONVOY_SIZE];
    struct Inventory_item shields[DEFAULT_CONVOY_SIZE];
    struct Inventory_item staffs[DEFAULT_CONVOY_SIZE];
    struct Inventory_item claws[DEFAULT_CONVOY_SIZE];
    struct Inventory_item items[DEFAULT_CONVOY_SIZE];
} Convoy;

extern struct Convoy Convoy_default;

extern void Convoy_deposit(struct Convoy * in_convoy, struct Inventory_item  in_item);
extern struct Inventory_item  Convoy_withdraw(struct Convoy * in_convoy, int16_t index, int16_t utilities_wpnType);
extern void Convoy_earn(struct Convoy * in_convoy, int16_t in_money);
extern void Convoy_spend(struct Convoy * in_convoy, int16_t out_money);

extern void Convoy_quicksort(struct Convoy * in_convoy, int16_t arr[], int16_t low, int16_t high, int16_t utilities_wpnType);
extern int16_t Convoy_partition(struct Convoy * in_convoy, int16_t arr[], int16_t low, int16_t high, int16_t utilities_wpnType);

extern void Convoy_swap(int16_t arr[], int16_t ind1, int16_t ind2);
extern void Convoy_Wpn_Swaps(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t ind1, int16_t ind2);
extern void Convoy_Wpn_Swap(struct Inventory_item arr[], int16_t ind1, int16_t ind2);

extern void Convoy_Stats_Sort(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype);
extern void Convoy_Used_Sort(struct Convoy * in_convoy, int16_t utilities_wpnType);

extern bool Convoy_isFull(struct Convoy * in_convoy);
extern int16_t * Convoy_Stats_Get(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype);
extern struct Inventory_item * Convoy_Items_Get(struct Convoy * in_convoy, int16_t utilities_wpnType);
extern void Convoy_Items_Set(struct Convoy * in_convoy, int16_t utilities_wpnType, struct Inventory_item * in_items);
extern void Convoy_Weapons_Set(struct Convoy * in_convoy, struct Weapons_hash * weapons);
extern void Convoy_Weapon_Check(struct Convoy * in_convoy, int16_t in_id);

extern void Convoy_Contents_Print(struct Convoy * in_convoy, int16_t utilities_wpnType); // Useless?
extern void Convoy_Stats_Print(struct Convoy * in_convoy, int16_t utilities_wpnType, int16_t stattype);

extern void Convoy_clear(struct Convoy * in_convoy);

extern struct Quantity Convoy_Quantities_Get(struct Convoy * in_convoy);
extern int16_t Convoy_getQuantity(struct Convoy * in_convoy, int16_t utilities_wpnType);

extern void Convoy_readJSON(struct Convoy * in_convoy, cJSON * in_json);
extern void Convoy_writeJSON(struct Convoy * in_convoy, cJSON * in_json);

#endif /* CONVOY_H */