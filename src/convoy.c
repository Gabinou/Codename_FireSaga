
#include "convoy.h"
#include "weapon.h"
#include "nmath.h"
#include "equations.h"
#include "utilities.h"
#include "jsonio.h"
#include "macros.h"
#include "names.h"
#include "unit/equipment.h"
#include "globals.h"

const struct Convoy Convoy_default = {
    .jsonio_header.json_element     = JSON_CONVOY,
    .num_wagons                     = 1,
};

/* --- Constructors/Destructors --- */
void Convoy_Free( struct Convoy *convoy) {
    s8_free(&convoy->jsonio_header.json_filename);
}

void Convoy_Clear(struct Convoy *convoy) {
    for (int i = 0; i < ITEM_TYPE_NUM; ++i) {
        convoy->num_items[i] = 0;
    }
}

/* --- Money --- */

i32 Convoy_Bank(Convoy *convoy) {
    SDL_assert(convoy != NULL);
    return (convoy->bank);
}

i32 Convoy_Earn(Convoy *convoy, i32 money) {
    SDL_assert(convoy != NULL);
    if (money < 0) {
        /* Invalid operation */
        return (convoy->bank);
    }

    if ((money + convoy->bank) > CONVOY_MONEY_MAX) {
        /* Fill bank */
        return (convoy->bank = CONVOY_MONEY_MAX);
    }
    convoy->bank += money;
    SDL_assert(convoy->bank <= CONVOY_MONEY_MAX);
    SDL_assert(convoy->bank >= 0);
    return (convoy->bank);
}

i32 Convoy_Spend(Convoy *conv, i32 m) {
    SDL_assert(conv != NULL);
    if (m < 0) {
        /* Invalid operation */
        return (conv->bank);
    }

    conv->bank -= conv->bank > m ? m : conv->bank;
    SDL_assert(conv->bank <= CONVOY_MONEY_MAX);
    SDL_assert(conv->bank >= 0);
    return (conv->bank);
}

/* --- Utils --- */
i32 _Convoy_Index2Type(i32 index) {
    // Each item type is in a row of a 2d array.
    // - Index encods both row col
    return (sota_ss_y(index, CONVOY_SIZE_MAX));
}

i32 _Convoy_Index2Order(i32 index) {
    // Order of item in its row of a 2d array.
    return (sota_ss_x(index, CONVOY_SIZE_MAX));
}

i32 _Convoy_Num_Items(const Convoy *convoy) {
    // Sum number of items of all types
    i32 sum = 0;
    for (int i = 0; i < ITEM_TYPE_NUM; ++i) {
        sum += convoy->num_items[i];
    }
    return (sum);
}
i32 _Convoy_Num_Wagons(const Convoy *convoy) {
    return (convoy->num_wagons);
}

i32 _Convoy_Size(const Convoy *convoy) {
    return (convoy->num_wagons * CONVOY_WAGON_SIZE);
}

b32 Convoy_isFull(const Convoy *convoy) {
    i32 size        = _Convoy_Size(convoy);
    i32 num_items   = _Convoy_Num_Items(convoy);
    return (num_items >= size);
}

/* --- Items --- */
/* Sorting happens on deposit/withdraw */
i32 Convoy_Deposit(Convoy          *convoy,
                   Inventory_item   invitem) {

    if (Convoy_isFull(convoy)) {
        return (0);
    }

    /* Get item type */
    u16 type = Item_ID2Type(invitem.id);

    /* Get items 2d array row */
    Inventory_item *row = convoy->items[type];
    i32 num_items = convoy->num_items[type];

    /* Insert item in row */
    i32 insert = 0;
    for (insert = 0; insert < num_items; insert++) {
        /* 1. Sort by id: small first   */
        if (invitem.id > row[insert].id) {
            continue;
        }

        /* 2. Sort by used: large first */
        if (invitem.used <= row[insert].used) {
            continue;
        }
        /* insert index found. */
        break;
    }

    /* Move over elements after insert */
    if (insert < num_items) {
        size_t bytesize = sizeof(*row) * (num_items - insert);
        memmove(row + insert + 1, row + insert, bytesize);
    }

    /* Place invitem at insert */
    row[insert] = invitem;
    convoy->num_items[type]++;
    return (insert + type * CONVOY_SIZE_MAX);
}

Inventory_item Convoy_Withdraw(Convoy *convoy, i32 i) {
    /* Get item at index */
    i32 type    = _Convoy_Index2Type(i);
    i32 order   = _Convoy_Index2Order(i);

    SDL_assert(convoy->num_items[type] > 0);
    i32 size = _Convoy_Size(convoy);
    SDL_assert(convoy->num_items[type] < size);
    i32 num_items = convoy->num_items[type];

    Inventory_item *row = convoy->items[type];
    Inventory_item  out = row[order];

    /* Move over elements after index */
    size_t bytesize = sizeof(*row) * (num_items - order);
    memmove(row + order, row + order + 1, bytesize);
    convoy->num_items[type]--;

    return (out);
}

Inventory_item Convoy_Item(Convoy *convoy, i32 i) {
    /* Get item at index */
    i32 type    = _Convoy_Index2Type(i);
    i32 order   = _Convoy_Index2Order(i);

    return (convoy->items[type][order]);
}

void Convoy_AllStats_Print(const Convoy *convoy,
                           i32 type_exp) {
}

void Convoy_Stats_Print(const Convoy *convoy, i32 type_exp, i32 stattype) {
}

/* --- I/O --- */
void Convoy_readJSON(void *input, const cJSON *jconvoy) {
}

void Convoy_writeJSON(const void *input, cJSON *jconvoy) {
}
