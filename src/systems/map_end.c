
#include "item.h"
#include "map_end.h"
#include "platform.h"

#include "unit/flags.h"

void System_Map_Repair(tnecs_In *input) {
    /* Repair items at the end of chapter. */
    InvItem *inv_item_arr = TNECS_C_ARRAY(input, InvItem_ID);
    for (size_t o = 0; o < input->num_Es; o++) {
        InvItem *inv_item = (inv_item_arr + o);
        inv_item->used = 0;
    }
}

void System_Map_Item_Cooldown(tnecs_In *input) {
    /* Tick down cooldowns of Items, IF army matches */

    InvItem     *inv_item_arr = TNECS_C_ARRAY(input, InvItem_ID);
    Cooldown    *cd_arr = TNECS_C_ARRAY(input, InvItem_ID);
    
    i32 turn_end_army = * (i32 *)input->data 
    for (size_t o = 0; o < input->num_Es; o++) {
        InvItem     *inv_item   = (inv_item_arr + o);
        Cooldown    *cd         = (cd_arr       + o);

        /* Skip if no keeper */
        if (inv_item->keeper == TNECS_NULL) {
            continue;
        }

        const Unit *unit = IES_GET_C(gl_world, inv_item->keeper, Unit);

        if (unit != NULL) {
            IES_assert(0);
            continue;
        }

        i32 item_army = Unit_Army(unit);

        /* Skip if army doesn't match */
        if ((item_army != turn_end_army) &&
            (item_army != ARMY_NULL))  {
            continue;
        }

        Cooldown_Tick(cd);
    }
}