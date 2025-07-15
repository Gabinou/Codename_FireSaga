
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
    .jsonio_header.json_element    = JSON_CONVOY,
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
i32 Convoy_Earn(Convoy *convoy, i32 money) {
    return (convoy->bank += money);
}

i32 Convoy_Spend(Convoy *convoy, i32 money) {
    return (convoy->bank -= money);
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
void Convoy_Deposit(Convoy          *convoy,
                    Inventory_item   invitem) {
    /* Get item type */
    u64 archetype = Item_Archetype(invitem.id);
    u16 type;
    if (archetype == ITEM_ARCHETYPE_ITEM) {
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, invitem.id);
        type = Item_Typecode(&weapon->item);
    } else {
        const Item *item = DTAB_GET_CONST(gl_items_dtab, invitem.id);
        type = Item_Typecode(item);
    }

    /* Get items 2d array row */
    Inventory_item *row = convoy->items[type];
    i32 num_items = convoy->num_items[type];

    /* Insert item in row */
    i32 insert = 0;
    for (insert = 0; insert < num_items; insert++) {
        /* 1. Sort by id: small first   */
        if (invitem.id >= row[insert].id) {
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
        memmove(row + insert, row + insert + 1, bytesize);
    }

    /* Place invitem at insert */
    row[insert] = invitem;
}

Inventory_item Convoy_Withdraw(Convoy *convoy, i32 i) {
    Inventory_item out = Inventory_item_default;
    return (out);
}

void Convoy_AllStats_Print(const Convoy *convoy,
                           i32 type_exp) {
    // u8 start = 0, end = convoy->num_items;
    // if (type_exp > ITEM_TYPE_EXP_NULL) {
    //     start = convoy->cumnum[type_exp - 1];
    //     end = convoy->cumnum[type_exp];
    // }
    // for (i32 i = start; i < end; i++) {
    //     struct Inventory_item item = convoy->items[i];
    //     struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
    //     char *wpn_stats_string = Utilities_Print_wpnStats(weapon->stats);
    //     char *item_stats_string = Utilities_Print_itemStats(weapon->item.stats);
    //     s8 name = Item_Name(weapon->item.ids.id);
    //     SDL_Log("%-20s \t %s %s", name.data, item_stats_string, wpn_stats_string);
    //     SDL_free(wpn_stats_string);
    //     SDL_free(item_stats_string);
    // }
}

void Convoy_Stats_Print(const Convoy *convoy, i32 type_exp, i32 stattype) {
    // SDL_assert(convoy != NULL);
    // SDL_assert(type_exp > ITEM_TYPE_EXP_NULL);
    // u8 start = convoy->cumnum[type_exp - 1];
    // u8 end = convoy->cumnum[type_exp];
    // for (i32 i = start; i < end; i++) {
    //     struct Inventory_item item = convoy->items[i];
    //     struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
    //     s8 name = Item_Name(weapon->item.ids.id);
    //     WeaponStatGet get = {
    //         .stat       = stattype,
    //         .distance   = DISTANCE_INVALID,
    //         .hand       = WEAPON_HAND_ONE,
    //         .infuse     = 0
    //     };
    //     SDL_Log("%-20s %d ", name.data, _Weapon_Stat_Raw(weapon, get));
    // }
}

/* --- I/O --- */
void Convoy_readJSON(void *input, const cJSON *jconvoy) {
    // Convoy *convoy = (Convoy *) input;
    // SDL_assert(jconvoy != NULL);
    // cJSON *jsize    = cJSON_GetObjectItem(jconvoy, "size");
    // cJSON *jbank    = cJSON_GetObjectItem(jconvoy, "bank");
    // cJSON *jitems   = cJSON_GetObjectItem(jconvoy, "Items");
    // convoy->size = cJSON_GetNumberValue(jsize);
    // convoy->bank = cJSON_GetNumberValue(jbank);
    // cJSON *jitem = NULL;
    // cJSON_ArrayForEach(jitem, jitems) {
    //     struct Inventory_item tempitem = Inventory_item_default;
    //     Inventory_item_readJSON(&tempitem, jitem);
    //     struct cJSON *jstored_type_exp = cJSON_GetObjectItem(jitem, "stored_type_exp");
    //     u16 stored_type_exp = cJSON_GetNumberValue(jstored_type_exp);
    //     Convoy_Deposit_byType(convoy, tempitem, stored_type_exp);
    // }
}

void Convoy_writeJSON(const void *input, cJSON *jconvoy) {
    // Convoy *convoy = (Convoy *) input;
    // SDL_assert(jconvoy != NULL);
    // cJSON *jsize        = cJSON_CreateNumber(convoy->size);
    // cJSON *jbank        = cJSON_CreateNumber(convoy->bank);
    // cJSON *jitems       = cJSON_CreateArray();
    // cJSON *jnum_items   = cJSON_CreateNumber(convoy->num_items);
    // cJSON_AddItemToObject(jconvoy, "size", jsize);
    // cJSON_AddItemToObject(jconvoy, "num_items", jnum_items);
    // cJSON_AddItemToObject(jconvoy, "bank", jbank);
    // for (u8 i = 0; i < convoy->num_items; i++) {
    //     cJSON *jitem = cJSON_CreateObject();
    //     Inventory_item_writeJSON(&convoy->items[i], jitem);
    //     u16 stored_type_exp    = Convoy_Id2TypeExp(convoy, i);
    //     cJSON *jstored_type     = cJSON_CreateNumber(stored_type_exp);
    //     cJSON_AddItemToObject(jitem, "stored_type_exp", jstored_type);
    //     cJSON_AddItemToArray(jitems, jitem);
    // }
    // cJSON_AddItemToObject(jconvoy, "Items", jitems);
}
