
#include "platform.h"
#include "turn_end.h"

#include "game/game.h"

#include "unit/bonus.h"
#include "unit/flags.h"

void System_Turn_End_Item_Cooldown_Tick(tnecs_In *input) {
    /* Tick down cooldowns of Items, IF army matches */
    IES_check(input         != NULL);
    IES_check(input->data   != NULL);
    Game *IES = input->data;

    InvItem     *inv_item_arr = TNECS_C_ARRAY(input, InvItem_ID);
    Cooldown    *cd_arr = TNECS_C_ARRAY(input, Cooldown_ID);

    for (size_t o = 0; o < input->num_Es; o++) {
        InvItem     *inv_item   = (inv_item_arr + o);
        Cooldown    *cd         = (cd_arr       + o);

        /* Skip if army doesn't match */
        if ((inv_item->army != IES->turn_end.army) &&
            (inv_item->army != ARMY_NULL))  {
            continue;
        }

        Cooldown_Tick(cd);
    }
}

/* TODO add to names/systems.h */
void System_Turn_End_Bonus_Remove_Persistent(tnecs_In *input) {
    /* Make unit in army bonuses decay */

    Game *IES = input->data;
    Unit *unit_arr = TNECS_C_ARRAY(input, Unit_ID);

    for (size_t o = 0; o < input->num_Es; o++) {
        Unit *unit = (unit_arr + o);

        i32 item_army = Unit_Army(unit);

        /* Skip if army doesn't match */
        if ((item_army != IES->turn_end.army) &&
            (item_army != ARMY_NULL))  {
            continue;
        }

        Unit_Bonus_Persistent_Decay(unit);
    }
}
