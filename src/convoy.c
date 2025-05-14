
#include "convoy.h"
#include "weapon.h"
#include "nmath.h"
#include "equations.h"
#include "utilities.h"
#include "jsonio.h"
#include "macros.h"
#include "globals.h"

const struct Convoy Convoy_default = {
    .jsonio_header.json_element    = JSON_CONVOY,

    .bank           = 0,
    .books_num      = 0,
    .items_num      = 0,
    .size           = SOTA_CONVOY_SIZE_START,
    .sort_direction = SOTA_CONVOY_SORT_HIGH2LOW,
};

void Convoy_Free( struct Convoy *convoy) {
    s8_free(&convoy->jsonio_header.json_filename);
}

void Convoy_Clear(struct Convoy *in_convoy) {
    in_convoy->items_num = in_convoy->books_num = 0;
}

b32 Convoy_isFull(struct Convoy *in_convoy) {
    return (in_convoy->items_num >= in_convoy->size);
}

i16 Convoy_Earn(struct Convoy *in_convoy, i16 in_money) {
    return (in_convoy->bank += in_money);
}

i16 Convoy_Spend(struct Convoy *in_convoy, i16 out_money) {
    return (in_convoy->bank -= out_money);
}

u16 Convoy_TypeCode2Exp(struct Convoy *in_convoy, u16 typecode) {
    return (nmath_firstsetBit_uint16_t(typecode));
}

void Convoy_Shift_Plus(struct Convoy *in_convoy, i16 i, i16 type_exp) {
    SDL_assert(i <= in_convoy->items_num);
    memmove(in_convoy->items + i + 1, in_convoy->items + i,
            (++in_convoy->items_num - i - 1) * sizeof(*in_convoy->items));
    for (i16 j = type_exp; j < ITEM_TYPE_NUM; j++)
        in_convoy->cumnum[j]++;
}

void Convoy_Shift_Minus(struct Convoy *in_convoy, i16 i) {
    memmove(in_convoy->items + i, in_convoy->items + i + 1,
            ((in_convoy->items_num--) - i) * sizeof(*in_convoy->items));
    for (i16 j = 0; j < ITEM_TYPE_NUM; j++) {
        if (in_convoy->cumnum[j] > i)
            in_convoy->cumnum[j]--;
    }
}

struct Inventory_item Convoy_Withdraw(struct Convoy *in_convoy, i16 i) {
    struct Inventory_item out = in_convoy->items[i];
    Convoy_Shift_Minus(in_convoy, i);
    return (out);
}

u16 Convoy_Id2TypeExp(struct Convoy *in_convoy, i16 i) {
    u16 out;
    for (out = ITEM_TYPE_EXP_NULL + 1; out < ITEM_TYPE_NUM; out++) {
        if (i < in_convoy->cumnum[out])
            break;
    }
    return (out);
}

void Convoy_Swap(struct Convoy *in_convoy, i16 i1, i16 i2) {
    SDL_assert(i1 < in_convoy->size);
    SDL_assert(i2 < in_convoy->size);
    u16 typecode1 = Item_Typecode(((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab,
                                   in_convoy->items[i1].id))->item);
    u16 typecode2 = Item_Typecode(((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab,
                                   in_convoy->items[i2].id))->item);
    SDL_assert(typecode1 & typecode2); /* can weapons be swapped? */
    struct Inventory_item buffer = in_convoy->items[i1];
    in_convoy->items[i1] = in_convoy->items[i2];
    in_convoy->items[i2] = buffer;
}

void Convoy_Deposit_byIndex(struct Convoy *in_convoy, struct Inventory_item in_item,
                            i16 i) {
    // SDL_Log("Depositing: %ld at %ld", in_item.id, i);
    SDL_assert(i < in_convoy->size);
    u16 stored_type_exp = Convoy_Id2TypeExp(in_convoy, i);
    Convoy_Shift_Plus(in_convoy, i, stored_type_exp);
    in_convoy->items[i] = in_item;
}

void Convoy_Deposit_byType(struct Convoy *in_convoy, struct Inventory_item in_item,
                           i16 type_exp) {
    // SDL_Log("Depositing: %ld", in_item.id);
    SDL_assert(gl_weapons_dtab != NULL);
    Weapon_Load(gl_weapons_dtab, in_item.id);
    u16 typecode = Item_Typecode(((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab,
                                  in_item.id))->item);
    SDL_assert(flagsum_isIn((1UL << (type_exp - 1)), typecode));
    int32_t max = in_convoy->cumnum[type_exp];
    Convoy_Shift_Plus(in_convoy, max, type_exp);
    in_convoy->items[max] = in_item;
    SDL_assert(in_item.used == in_convoy->items[max].used);
    SDL_assert(in_item.id == in_convoy->items[max].id);
    SDL_assert(in_item.infusion == in_convoy->items[max].infusion);
}

void Convoy_Sort(struct Convoy *in_convoy, i16 stattype) {
    u8 sort_arr[SOTA_CONVOY_SIZE_MAX];
    for (i16 i = 0; i < in_convoy->items_num; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
        sort_arr[i] = Weapon_Stat(weapon, stattype);
    }
    for (i16 i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++)
        Convoy_Quicksort(in_convoy, sort_arr, in_convoy->cumnum[i - 1], in_convoy->cumnum[i] - 1);
}

void Convoy_Quicksort(struct Convoy *in_convoy, u8 arr[], u8 low, u8 high) {
    if (low < high) {
        u8 pivot = Convoy_Partition_wDuplicates(in_convoy, arr, low, high);
        Convoy_Quicksort(in_convoy, arr, low, pivot); // quicksort left of pivot
        Convoy_Quicksort(in_convoy, arr, pivot + 1, high); // quicksort right of pivot
    }
}

u8 Convoy_Partition_wDuplicates(struct Convoy *in_convoy, u8 arr[], u8 low, u8 high) {
    u8 pivot = arr[low]; // hoare pivot
    u8 left = low - 1, right = high + 1;
    while (true) {
        // Find all values >/< than pivot and swap them
        // sign: if a > b is true, -a > -b is false
        i16 sign = -1 * (in_convoy->sort_direction == SOTA_CONVOY_SORT_LOW2HIGH) +
                   1 * (in_convoy->sort_direction == SOTA_CONVOY_SORT_HIGH2LOW);
        do
            left++;
        while (sign * arr[left] > sign * pivot);
        do
            right--;
        while (sign * arr[right] < sign * pivot);
        if (left >= right) // Next pivot found
            break;
        // Swapping values >/< than pivot
        if ((left >= low) && (right <= high)) {
            Inventory_item_Swap(in_convoy->items, left, right);
            nmath_swap_uint_fast8_t(arr, left, right);
        }
    }
    return (right);
}

void Convoy_AllStats_Print(struct Convoy *in_convoy, i16 type_exp) {
    u8 start = 0, end = in_convoy->items_num;
    if (type_exp > ITEM_TYPE_EXP_NULL) {
        start = in_convoy->cumnum[type_exp - 1];
        end = in_convoy->cumnum[type_exp];
    }
    for (i16 i = start; i < end; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
        char *wpn_stats_string = Utilities_Print_wpnStats(weapon->stats);
        char *item_stats_string = Utilities_Print_itemStats(weapon->item->stats);
        SDL_Log("%-20s \t %s %s", weapon->item->name.data, item_stats_string, wpn_stats_string);
        SDL_free(wpn_stats_string);
        SDL_free(item_stats_string);
    }
}

void Convoy_Stats_Print(struct Convoy *in_convoy, i16 type_exp, i16 stattype) {
    SDL_assert(in_convoy != NULL);
    SDL_assert(type_exp > ITEM_TYPE_EXP_NULL);
    u8 start = in_convoy->cumnum[type_exp - 1];
    u8 end = in_convoy->cumnum[type_exp];
    for (i16 i = start; i < end; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
        SDL_Log("%-20s %d ", weapon->item->name.data, Weapon_Stat(weapon, stattype));
    }
}

void Convoy_readJSON(void *input, const cJSON *in_jconvoy) {
    struct Convoy *in_convoy = (struct Convoy *) input;
    SDL_assert(in_jconvoy != NULL);
    cJSON *jsize    = cJSON_GetObjectItem(in_jconvoy, "size");
    cJSON *jbank    = cJSON_GetObjectItem(in_jconvoy, "bank");
    cJSON *jitems   = cJSON_GetObjectItem(in_jconvoy, "Items");
    in_convoy->size = cJSON_GetNumberValue(jsize);
    in_convoy->bank = cJSON_GetNumberValue(jbank);
    cJSON *jitem = NULL;
    cJSON_ArrayForEach(jitem, jitems) {
        struct Inventory_item tempitem = Inventory_item_default;
        Inventory_item_readJSON(&tempitem, jitem);
        struct cJSON *jstored_type_exp = cJSON_GetObjectItem(jitem, "stored_type_exp");
        u16 stored_type_exp = cJSON_GetNumberValue(jstored_type_exp);
        Convoy_Deposit_byType(in_convoy, tempitem, stored_type_exp);
    }
}

void Convoy_writeJSON(const void *input, cJSON *in_jconvoy) {
    struct Convoy *in_convoy = (struct Convoy *) input;
    SDL_assert(in_jconvoy != NULL);
    cJSON *jsize        = cJSON_CreateNumber(in_convoy->size);
    cJSON *jbank        = cJSON_CreateNumber(in_convoy->bank);
    cJSON *jitems       = cJSON_CreateArray();
    cJSON *jitems_num   = cJSON_CreateNumber(in_convoy->items_num);
    cJSON_AddItemToObject(in_jconvoy, "size", jsize);
    cJSON_AddItemToObject(in_jconvoy, "items_num", jitems_num);
    cJSON_AddItemToObject(in_jconvoy, "bank", jbank);
    for (u8 i = 0; i < in_convoy->items_num; i++) {
        cJSON *jitem = cJSON_CreateObject();
        Inventory_item_writeJSON(&in_convoy->items[i], jitem);
        u16 stored_type_exp    = Convoy_Id2TypeExp(in_convoy, i);
        cJSON *jstored_type     = cJSON_CreateNumber(stored_type_exp);
        cJSON_AddItemToObject(jitem, "stored_type_exp", jstored_type);
        cJSON_AddItemToArray(jitems, jitem);
    }
    cJSON_AddItemToObject(in_jconvoy, "Items", jitems);
}
