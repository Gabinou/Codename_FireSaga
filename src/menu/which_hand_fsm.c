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

#include "item.h"
#include "globals.h"

#include "game/game.h"
#include "game/popup.h"

#include "menu/menu.h"
#include "menu/item_select.h"
#include "menu/which_hand_fsm.h"

#include "unit/equipment.h"
#include "unit/loadout.h"


const fsm_whm_t fsm_WHM_m[MENU_TYPE_END] = {
    [MENU_TYPE_ITEM_ACTION] = NULL,
};

const fsm_whm_t fsm_WHM_mIAM_mo[IAM_OPTION_NUM] = {
    /* EQUIP */     &fsm_whm_mIAM_moEquip,
    /* USE   */     &fsm_whm_mIAM_moUse,
    /* DROP  */     NULL,
    /* TRADE */     NULL
};

void fsm_whm_mIAM_moUse(Game *IES, Menu *mc_WHM) {
    //  Ex: mIAM & moUse
    //      Equip, use item, pop all menus, make unit wait...

    /* --- Decided which hand to equip item ---
    **  1. Each item needs to be equipped
    **  2. For now: All items are one hand only.
    */

    /* -- Getting the hand -- */
    WhichHandMenu *whm = mc_WHM->data;
    i32 hand = WhichHandMenu_Selected_Hand(whm);
    SDL_assert( (hand == UNIT_EQUIP_LEFT) ||
                (hand == UNIT_EQUIP_RIGHT));


    /* --- Action with item: Use it --- */
    /* -- Getting the item -- */
    Menu *mc_ISM = IES_GET_C(gl_world, IES->menus.item_select, Menu);
    SDL_assert(mc_ISM->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ISM->data;

    const Unit *unit = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);

    Inventory_item *invitem = Unit_InvItem(unit, ism->selected_eq);
    ;
    const Item *item = Item_Get(invitem);


    /* - Turn popups invisible - */
    Game_PopUp_Loadout_Stats_Hide(IES);

    /* -- Saving previous loadout -- */
    Menu *mc_IAM = IES_GET_C(gl_world, IES->menus.item_action, Menu);
    SDL_assert(mc_IAM->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc_IAM->data;

    Unit_Loadout_Export(unit, &iam->previous_loadout);

    /* - Turn menus invisible - */
    mc_IAM->visible = false;
    mc_ISM->visible = false;

    /* -- Equipping item -- */
    if (hand == UNIT_EQUIP_LEFT) {
        Unit_Equip(unit, UNIT_HAND_LEFT, ism->selected_eq);
    } else {
        SDL_assert(hand == UNIT_EQUIP_RIGHT);
        Unit_Equip(unit, UNIT_HAND_RIGHT, ism->selected_eq);
    }

    /* -- Use item directly OR map candidates -- */
    if (item->ids.target == TARGET_SELF) {
        /* -- Directly using item on self -- */
        /* - User is selected unit - */
        tnecs_E user_E = IES->selected.unit_entity;

        /* -- Directly using item on self -- */
        fsm_Item_Use(IES, user_E, user_E);
    } else {
        /* Target is NOT self, player picks it.
        ** Even if only one possible target.
        **  - Hint that item can be used on others. */

        /* - Switch to Map_Candidates substate - */
        Game_Switch_toCandidates(
                IES, IES->targets.patients,
                "Using item on targets"
        );
        Game_Cursor_Move_toCandidate(IES);
    }


}

void fsm_whm_mIAM_moEquip(Game *IES, Menu *mc) {
    //  Ex: mIAM & moEquip
    //      Equip item, pop WHM, pop parent IAM
}
