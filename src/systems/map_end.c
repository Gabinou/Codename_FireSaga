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
**  System run on turn end in map
**
*/

#include "item.h"
#include "map_end.h"
#include "platform.h"

#include "game/game.h"

#include "unit/bonus.h"
#include "unit/flags.h"

/* Note this is map end */
void System_Map_Repair(tnecs_In *input) {
    /* Repair items at the end of chapter. */
    InvItem *inv_item_arr = TNECS_C_ARRAY(input, InvItem_ID);
    for (size_t o = 0; o < input->num_Es; o++) {
        InvItem *inv_item = (inv_item_arr + o);
        inv_item->used = 0;
    }
}
