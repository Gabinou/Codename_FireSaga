
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

    .bank           = 0,
    .books_num      = 0,
    .items_num      = 0,
    .size           = CONVOY_SIZE_START,
    .sort_direction = CONVOY_SORT_HIGH2LOW,
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

i32 Convoy_Earn(struct Convoy *in_convoy, i32 in_money) {
    return (in_convoy->bank += in_money);
}

i32 Convoy_Spend(struct Convoy *in_convoy, i32 out_money) {
    return (in_convoy->bank -= out_money);
}

struct Inventory_item Convoy_Withdraw(struct Convoy *in_convoy, i32 i) {
    struct Inventory_item out = in_convoy->items[i];
    Convoy_Shift_Minus(in_convoy, i);
    return (out);
}
void Convoy_AllStats_Print(struct Convoy *in_convoy, i32 type_exp) {
    // u8 start = 0, end = in_convoy->items_num;
    // if (type_exp > ITEM_TYPE_EXP_NULL) {
    //     start = in_convoy->cumnum[type_exp - 1];
    //     end = in_convoy->cumnum[type_exp];
    // }
    // for (i32 i = start; i < end; i++) {
    //     struct Inventory_item item = in_convoy->items[i];
    //     struct Weapon *weapon = ((struct Weapon *)DTAB_GET_CONST(gl_weapons_dtab, item.id));
    //     char *wpn_stats_string = Utilities_Print_wpnStats(weapon->stats);
    //     char *item_stats_string = Utilities_Print_itemStats(weapon->item.stats);
    //     s8 name = Item_Name(weapon->item.ids.id);
    //     SDL_Log("%-20s \t %s %s", name.data, item_stats_string, wpn_stats_string);
    //     SDL_free(wpn_stats_string);
    //     SDL_free(item_stats_string);
    // }
}

void Convoy_Stats_Print(struct Convoy *in_convoy, i32 type_exp, i32 stattype) {
    // SDL_assert(in_convoy != NULL);
    // SDL_assert(type_exp > ITEM_TYPE_EXP_NULL);
    // u8 start = in_convoy->cumnum[type_exp - 1];
    // u8 end = in_convoy->cumnum[type_exp];
    // for (i32 i = start; i < end; i++) {
    //     struct Inventory_item item = in_convoy->items[i];
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

void Convoy_readJSON(void *input, const cJSON *in_jconvoy) {
    // Convoy *in_convoy = (Convoy *) input;
    // SDL_assert(in_jconvoy != NULL);
    // cJSON *jsize    = cJSON_GetObjectItem(in_jconvoy, "size");
    // cJSON *jbank    = cJSON_GetObjectItem(in_jconvoy, "bank");
    // cJSON *jitems   = cJSON_GetObjectItem(in_jconvoy, "Items");
    // in_convoy->size = cJSON_GetNumberValue(jsize);
    // in_convoy->bank = cJSON_GetNumberValue(jbank);
    // cJSON *jitem = NULL;
    // cJSON_ArrayForEach(jitem, jitems) {
    //     struct Inventory_item tempitem = Inventory_item_default;
    //     Inventory_item_readJSON(&tempitem, jitem);
    //     struct cJSON *jstored_type_exp = cJSON_GetObjectItem(jitem, "stored_type_exp");
    //     u16 stored_type_exp = cJSON_GetNumberValue(jstored_type_exp);
    //     Convoy_Deposit_byType(in_convoy, tempitem, stored_type_exp);
    // }
}

void Convoy_writeJSON(const void *input, cJSON *in_jconvoy) {
    // Convoy *in_convoy = (Convoy *) input;
    // SDL_assert(in_jconvoy != NULL);
    // cJSON *jsize        = cJSON_CreateNumber(in_convoy->size);
    // cJSON *jbank        = cJSON_CreateNumber(in_convoy->bank);
    // cJSON *jitems       = cJSON_CreateArray();
    // cJSON *jitems_num   = cJSON_CreateNumber(in_convoy->items_num);
    // cJSON_AddItemToObject(in_jconvoy, "size", jsize);
    // cJSON_AddItemToObject(in_jconvoy, "items_num", jitems_num);
    // cJSON_AddItemToObject(in_jconvoy, "bank", jbank);
    // for (u8 i = 0; i < in_convoy->items_num; i++) {
    //     cJSON *jitem = cJSON_CreateObject();
    //     Inventory_item_writeJSON(&in_convoy->items[i], jitem);
    //     u16 stored_type_exp    = Convoy_Id2TypeExp(in_convoy, i);
    //     cJSON *jstored_type     = cJSON_CreateNumber(stored_type_exp);
    //     cJSON_AddItemToObject(jitem, "stored_type_exp", jstored_type);
    //     cJSON_AddItemToArray(jitems, jitem);
    // }
    // cJSON_AddItemToObject(in_jconvoy, "Items", jitems);
}
