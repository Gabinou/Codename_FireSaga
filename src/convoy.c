/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Convoy to store InvItem (all items)
**
*/

#include "nmath.h"
#include "names.h"
#include "jsonio.h"
#include "macros.h"
#include "convoy.h"
#include "weapon.h"
#include "globals.h"
#include "equations.h"
#include "utilities.h"
#include "unit/equipment.h"

const Convoy Convoy_default = {
    .jsonio_header.json_element     = JSON_CONVOY,
    .num_wagons                     = 1,
};

/* --- Constructors/Destructors --- */
void Convoy_Free(Convoy *convoy) {
    s8_free(&convoy->jsonio_header.json_filename);
}

void Convoy_Clear(Convoy *convoy) {
    size_t bytesize;
    if (convoy == NULL) {
        return;
    }

    bytesize = ITEM_TYPE_NUM * sizeof(*convoy->num_items);
    memset(convoy->num_items,   0, bytesize);

    bytesize = ITEM_TYPE_NUM * CONVOY_SIZE_MAX *
               sizeof(**convoy->items);
    memset(convoy->items,       0, bytesize);
}

/* --- Money --- */

i32 Convoy_Bank(const Convoy *convoy) {
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
                   InvItem   invitem) {

    if (Convoy_isFull(convoy)) {
        return (0);
    }

    /* Get item type */
    u16 type = _Item_Type(invitem.id);

    /* Get items 2d array row */
    InvItem *row = convoy->items[type];
    i32 num_items = convoy->num_items[type];

    /* Insert item in row */
    i32 insert = 0;
    for (insert = 0; insert < num_items; insert++) {
        if (row[insert].id <= 0) {
            /* 1. No item here, insert found   */
            break;
        }
        /* There is an item at insert.   */
        if (invitem.id > row[insert].id) {
            /* convoy item has greater id: no insert    */
            continue;
        }

        /* Item at insert has SEQ id.   */
        if (invitem.id < row[insert].id) {
            /* Only insert here if id is smaller */
            break;
        }

        /* Item at insert has equal id.   */
        if (invitem.used >= row[insert].used) {
            /* Only insert if used is GEQ */
            break;
        }

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

InvItem Convoy_Withdraw(Convoy *convoy, i32 i) {
    /* Get item at index */
    i32 type    = _Convoy_Index2Type(i);
    i32 order   = _Convoy_Index2Order(i);

    SDL_assert(convoy->num_items[type] > 0);
    i32 size = _Convoy_Size(convoy);
    SDL_assert(convoy->num_items[type] < size);
    i32 num_items = convoy->num_items[type];

    InvItem *row = convoy->items[type];
    InvItem  out = row[order];

    /* Move over elements after index */
    size_t bytesize = sizeof(*row) * (num_items - order);
    memmove(row + order, row + order + 1, bytesize);
    convoy->num_items[type]--;

    return (out);
}

InvItem Convoy_Item(const Convoy *convoy, i32 i) {
    /* Get item at index */
    i32 type    = _Convoy_Index2Type(i);
    i32 order   = _Convoy_Index2Order(i);

    return (convoy->items[type][order]);
}

/* --- I/O --- */
void Convoy_readJSON(void *input, const cJSON *jconvoy) {
    Convoy *convoy = (Convoy *)input;
    /* Bank */
    cJSON *jbank = cJSON_GetObjectItem(jconvoy, "Bank");
    if (jbank != NULL)
        convoy->bank = cJSON_GetNumberValue(jbank);

    /* Wagons */
    cJSON *jwagons = cJSON_GetObjectItem(jconvoy, "Wagons");
    if (jwagons != NULL)
        convoy->num_wagons = cJSON_GetNumberValue(jwagons);

    /* Items */
    cJSON *jitems = cJSON_GetObjectItem(jconvoy, "Items");
    if (jitems == NULL) {
        return;
    }
    Convoy_Clear(convoy);
    SDL_assert(cJSON_IsArray(jitems));

    i32 jitems_size = cJSON_GetArraySize(jitems);
    SDL_assert(jitems_size == ITEM_TYPE_NUM);

    for (int type = 0; type < ITEM_TYPE_NUM; ++type) {
        SDL_assert(convoy->num_items[type] == 0);
        /* Write items to array */
        cJSON *jitems_arr = cJSON_GetArrayItem(jitems, type);
        i32 jtype_size = cJSON_GetArraySize(jitems_arr);

        for (int order = 0; order < jtype_size; ++order) {
            cJSON *jitem = cJSON_GetArrayItem(jitems_arr, order);

            InvItem invitem = Inventory_item_default;
            Inventory_item_readJSON(&invitem, jitem);
            if (invitem.id != ITEM_NULL) {
                Convoy_Deposit(convoy, invitem);
            }
        }
    }
}

void Convoy_writeJSON(const void *input, cJSON *jconvoy) {
    const Convoy *convoy = input;

    /* Bank */
    cJSON *jbank  = cJSON_CreateNumber(convoy->bank);
    cJSON_AddItemToObject(jconvoy, "Bank",  jbank);

    /* Wagons */
    cJSON *jwagons  = cJSON_CreateNumber(convoy->num_wagons);
    cJSON_AddItemToObject(jconvoy, "Wagons",  jwagons);

    /* Items */
    cJSON *jitems       = cJSON_CreateArray();

    for (int type = 0; type < ITEM_TYPE_NUM; ++type) {
        const InvItem *row = convoy->items[type];

        /* Create new array for weapon type */
        cJSON *jitems_arr   = cJSON_CreateArray();

        /* Write items to array */
        int num = convoy->num_items[type];
        for (int order = 0; order < num; ++order) {

            cJSON *jitem        = cJSON_CreateObject();
            const InvItem *invitem = &row[order];

            Inventory_item_writeJSON(invitem, jitem);
            cJSON_AddItemToArray(jitems_arr,  jitem);
        }

        /* Add array of items of type to
        ** items nested array */
        cJSON_AddItemToArray(jitems,  jitems_arr);
    }
    cJSON_AddItemToObject(jconvoy, "Items",  jitems);
}
