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
** mo_fsm: Finite State Machine for game.menu.option
**  - Different behavior according to menu & game state
**
*/

#include "menu/which_hand_fsm.h"


const fsm_whm_t fsm_WHM_m[MENU_TYPE_END] = {
    [MENU_TYPE_ITEM_ACTION] = NULL,
}

const fsm_whm_t fsm_WHM_mIAM_mo[IAM_OPTION_NUM] {
    /* EQUIP */     &fsm_whm_mIAM_moEquip,
    /* USE   */     &fsm_whm_mIAM_moUse,
    /* DROP  */     NULL,
    /* TRADE */     NULL

}

