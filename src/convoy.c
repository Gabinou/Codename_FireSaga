
#include "convoy.h"

struct Convoy Convoy_default = {
    .JSONElement = {JSON_CONVOY},
    .weapons_dtab = NULL,

    .books = {0},
    .items = {0},
    .cumnum = {0},

    .bank = 0,
    .books_num = 0,
    .items_num = 0,
    .size = SOTA_CONVOY_SIZE_START,
    .sort_direction = SOTA_CONVOY_SORT_HIGH2LOW,
};

void Convoy_Clear(struct Convoy *in_convoy) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    in_convoy->items_num = in_convoy->books_num = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Convoy_isFull(struct Convoy *in_convoy) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (in_convoy->items_num >= in_convoy->size);
}

if16 Convoy_Earn(struct Convoy *in_convoy, if16 in_money) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (in_convoy->bank += in_money);
}

if16 Convoy_Spend(struct Convoy *in_convoy, if16 out_money) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (in_convoy->bank -= out_money);
}

uf16 Convoy_TypeCode2Exp(struct Convoy *in_convoy, uf16 typecode) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (nmath_firstsetBit_uint_fast16_t(typecode));
}

void Convoy_Shift_Plus(struct Convoy *in_convoy, if16 i, if16 type_exp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(i <= in_convoy->items_num);
    memmove(in_convoy->items + i + 1, in_convoy->items + i,
            (++in_convoy->items_num - i - 1) * sizeof(*in_convoy->items));
    for (if16 j = type_exp; j < ITEM_TYPE_NUM; j++)
        in_convoy->cumnum[j]++;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Shift_Minus(struct Convoy *in_convoy, if16 i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    memmove(in_convoy->items + i, in_convoy->items + i + 1,
            ((in_convoy->items_num--) - i) * sizeof(*in_convoy->items));
    for (if16 j = 0; j < ITEM_TYPE_NUM; j++) {
        if (in_convoy->cumnum[j] > i)
            in_convoy->cumnum[j]--;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Inventory_item Convoy_Withdraw(struct Convoy *in_convoy, if16 i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Inventory_item out = in_convoy->items[i];
    Convoy_Shift_Minus(in_convoy, i);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

uf16 Convoy_Id2TypeExp(struct Convoy *in_convoy, if16 i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    uf16 out;
    for (out = ITEM_TYPE_EXP_NULL + 1; out < ITEM_TYPE_NUM; out++) {
        if (i < in_convoy->cumnum[out])
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void Convoy_Swap(struct Convoy *in_convoy, if16 i1, if16 i2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(i1 < in_convoy->size);
    SDL_assert(i2 < in_convoy->size);
    uf16 typecode1 = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab,
                                                in_convoy->items[i1].id))->item->type;
    uf16 typecode2 = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab,
                                                in_convoy->items[i2].id))->item->type;
    SDL_assert(typecode1 & typecode2); /* can weapons be swapped? */
    struct Inventory_item buffer = in_convoy->items[i1];
    in_convoy->items[i1] = in_convoy->items[i2];
    in_convoy->items[i2] = buffer;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Deposit_byIndex(struct Convoy *in_convoy, struct Inventory_item in_item,
                            if16 i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Depositing: %ld at %ld", in_item.id, i);
    SDL_assert(i < in_convoy->size);
    uf16 stored_type_exp = Convoy_Id2TypeExp(in_convoy, i);
    Convoy_Shift_Plus(in_convoy, i, stored_type_exp);
    in_convoy->items[i] = in_item;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Deposit_byType(struct Convoy *in_convoy, struct Inventory_item in_item,
                           if16 type_exp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Depositing: %ld", in_item.id);
    SDL_assert(in_convoy->weapons_dtab != NULL);
    Weapon_Load(in_convoy->weapons_dtab, in_item.id);
    uf16 typecode = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab,
                                               in_item.id))->item->type;
    SDL_assert(flagsum_isIn((1UL << (type_exp - 1)), typecode));
    int32_t max = in_convoy->cumnum[type_exp];
    Convoy_Shift_Plus(in_convoy, max, type_exp);
    in_convoy->items[max] = in_item;
    SDL_assert(in_item.used == in_convoy->items[max].used);
    SDL_assert(in_item.id == in_convoy->items[max].id);
    SDL_assert(in_item.infusion == in_convoy->items[max].infusion);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Sort(struct Convoy *in_convoy, if16 stattype) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 sort_arr[SOTA_CONVOY_SIZE_MAX];
    for (if16 i = 0; i < in_convoy->items_num; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab, item.id));
        sort_arr[i] = Weapon_Stat(weapon, stattype);
    }
    for (if16 i = ITEM_TYPE_EXP_NULL + 1; i < ITEM_TYPE_NUM; i++)
        Convoy_Quicksort(in_convoy, sort_arr, in_convoy->cumnum[i - 1], in_convoy->cumnum[i] - 1);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Quicksort(struct Convoy *in_convoy, u8 arr[], u8 low, u8 high) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (low < high) {
        u8 pivot = Convoy_Partition_wDuplicates(in_convoy, arr, low, high);
        Convoy_Quicksort(in_convoy, arr, low, pivot); // quicksort left of pivot
        Convoy_Quicksort(in_convoy, arr, pivot + 1, high); // quicksort right of pivot
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

u8 Convoy_Partition_wDuplicates(struct Convoy *in_convoy, u8 arr[], u8 low, u8 high) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 pivot = arr[low]; // hoare pivot
    u8 left = low - 1, right = high + 1;
    while (true) {
        // Find all values >/< than pivot and swap them
        // sign: if a > b is true, -a > -b is false
        if16 sign = -1 * (in_convoy->sort_direction == SOTA_CONVOY_SORT_LOW2HIGH) +
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
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (right);
}

void Convoy_AllStats_Print(struct Convoy *in_convoy, if16 type_exp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 stat, start = 0, end = in_convoy->items_num;
    if (type_exp > ITEM_TYPE_EXP_NULL) {
        start = in_convoy->cumnum[type_exp - 1];
        end = in_convoy->cumnum[type_exp];
    }
    for (if16 i = start; i < end; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab, item.id));
        char *wpn_stats_string = Utilities_Print_wpnStats(weapon->stats);
        char *item_stats_string = Utilities_Print_itemStats(weapon->item->stats);
        SOTA_Log_Debug("%-20s \t %s %s", weapon->item->name, item_stats_string, wpn_stats_string);
        SDL_free(wpn_stats_string);
        SDL_free(item_stats_string);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_Stats_Print(struct Convoy *in_convoy, if16 type_exp, if16 stattype) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(in_convoy != NULL);
    SDL_assert(type_exp > ITEM_TYPE_EXP_NULL);
    u8 start = in_convoy->cumnum[type_exp - 1];
    u8 end = in_convoy->cumnum[type_exp];
    for (if16 i = start; i < end; i++) {
        struct Inventory_item item = in_convoy->items[i];
        struct Weapon *weapon = ((struct Weapon *)DTAB_GET(in_convoy->weapons_dtab, item.id));
        SOTA_Log_Debug("%-20s %d ", weapon->item->name, Weapon_Stat(weapon, stattype));
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_readJSON(void *input, const cJSON *const in_jconvoy) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
        Filesystem_readJSON_Item(jitem, &tempitem);
        struct cJSON *jstored_type_exp = cJSON_GetObjectItem(jitem, "stored_type_exp");
        uf16 stored_type_exp = cJSON_GetNumberValue(jstored_type_exp);
        Convoy_Deposit_byType(in_convoy, tempitem, stored_type_exp);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Convoy_writeJSON(const void *input, cJSON *in_jconvoy) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
        Filesystem_writeJSON_item(jitem, &in_convoy->items[i]);
        uf16 stored_type_exp    = Convoy_Id2TypeExp(in_convoy, i);
        cJSON *jstored_type     = cJSON_CreateNumber(stored_type_exp);
        cJSON_AddItemToObject(jitem, "stored_type_exp", jstored_type);
        cJSON_AddItemToArray(jitems, jitem);
    }
    cJSON_AddItemToObject(in_jconvoy, "Items", jitems);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
