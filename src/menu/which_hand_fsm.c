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

#include "fsm.h"
#include "item.h"
#include "events.h"
#include "globals.h"
#include "platform.h"

#include "game/game.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "menu/fsm.h"
#include "menu/menu.h"
#include "menu/item_select.h"
#include "menu/option_fsm.h"
#include "menu/which_hand_fsm.h"

#include "popup/popup.h"
#include "popup/loadout_stats.h"

#include "unit/equipment.h"
#include "unit/loadout.h"

/* --- Parent menu --- */
const fsm_whm_t fsm_WHM_eAcpt_m[MENU_TYPE_END] = {
    [MENU_TYPE_ITEM_ACTION] = fsm_WHM_eAcpt_mIAM,
};

/* --- Menu Option --- */
const fsm_whm_t fsm_WHM_eAcpt_mIAM_mo[IAM_OPTION_NUM] = {
    /* EQUIP    */ &fsm_WHM_eAcpt_mIAM_moEquip,
    /* USE      */ &fsm_WHM_eAcpt_mIAM_moUse,
    /* DROP     */ NULL,
    /* TRADE    */ NULL
};

const fsm_whm_t fsm_WHM_eCrsMvs_m[MENU_TYPE_END] = {
    [MENU_TYPE_ITEM_ACTION] = fsm_WHM_eCrsMvs_mIAM,
};

const fsm_whm_t fsm_WHM_eCrsMvs_mIAM_mo[IAM_OPTION_NUM] = {
    /* EQUIP    */ &fsm_WHM_eCrsMvs_mIAM_moEquip,
    /* USE      */ NULL,
    /* DROP     */ NULL,
    /* TRADE    */ NULL

};

void fsm_WHM_eAcpt_mIAM(Game *IES, Menu *mc_IAM) {
    IES_check(IES);
    IES_check(mc_IAM);

    IES_assert(mc_IAM->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc_IAM->data;
    i32 mo_order = ItemActionMenu_Option_Order(iam, mc_IAM);

    if (fsm_WHM_eAcpt_mIAM_mo[mo_order] != NULL) {
        fsm_WHM_eAcpt_mIAM_mo[mo_order](IES, mc_IAM);
    }
}

void fsm_WHM_eAcpt_mIAM_moUse(Game *IES, Menu *mc_IAM) {
    IES_check(IES);
    IES_check(mc_IAM);

    /* --- Decided which hand to equip item to use ---
    **  1. Equip item
    **  2. If target is self: use item, pop menus, make unit wait
    **  3. If target is not self: go to map candidates  */

    /* -- Getting the hand -- */
    Menu *mc_WHM = IES_GET_C(gl_world, IES->menus.which_hand, Menu);
    WhichHandMenu *whm = mc_WHM->data;
    i32 hand = WhichHandMenu_Selected_Hand(whm);

    /* --- Action with item: Use it --- */
    /* -- Getting the item -- */
    Menu *mc_ISM = IES_GET_C(   gl_world, IES->menus.item_select,
                                Menu);
    SDL_assert(mc_ISM->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ISM->data;

    Unit *unit = IES_GET_C( gl_world, IES->selected.unit_entity,
                            Unit);

    InvItem *invitem = Unit_InvItem(unit, ism->selected_eq);
    const Item *item = Item_Get(invitem);

    /* - Turn popups invisible - */
    Game_PopUp_Loadout_Stats_Hide(IES);

    /* -- Saving previous loadout -- */
    SDL_assert(mc_IAM->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc_IAM->data;

    Unit_Loadout_Export(unit, &iam->previous_loadout);

    /* - Turn menus invisible - */
    mc_IAM->visible = false;
    mc_ISM->visible = false;
    mc_WHM->visible = false;

    /* -- Equipping item -- */
    if (hand == UNIT_EQUIP_LEFT) {
        Unit_Equip(unit, UNIT_HAND_LEFT,    ism->selected_eq);
    } else if (hand == UNIT_EQUIP_RIGHT) {
        Unit_Equip(unit, UNIT_HAND_RIGHT,   ism->selected_eq);
    } else if (hand == UNIT_EQUIP_TWO_HANDS) {
        Unit_Equip(unit, UNIT_HAND_LEFT,    ism->selected_eq);
        Unit_Equip(unit, UNIT_HAND_RIGHT,   ism->selected_eq);
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

void fsm_WHM_eAcpt_mIAM_moEquip(Game *IES, Menu *mc_IAM) {
    IES_check(IES);
    IES_check(mc_IAM);

    /* --- Decided which hand to equip item to use ---
    **  1. Equip item
    **  2. Go back to IAM? or ISM? */

    /* --- Action with item: equipping it --- */

    /* -- hand from WHM -- */
    Menu *mc_WHM = IES_GET_C(gl_world, IES->menus.which_hand, Menu);
    WhichHandMenu *whm = mc_WHM->data;
    WhichHandMenu_Select(whm, mc_WHM->elem);
    i32 hand = WhichHandMenu_Selected_Hand(whm);

    /* -- item from ISM -- */
    Menu *mc_ISM = IES_GET_C(   gl_world, IES->menus.item_select,
                                Menu);
    SDL_assert(mc_ISM->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ISM->data;

    Unit *unit = IES_GET_C( gl_world, IES->selected.unit_entity,
                            Unit);

    InvItem *invitem = Unit_InvItem(unit, ism->selected_eq);
    const Item *item = Item_Get(invitem);

    /* -- Saving previous loadout -- */
    SDL_assert(mc_IAM->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc_IAM->data;

    Unit_Loadout_Export(unit, &iam->previous_loadout);

    /* - Turn menus invisible - */
    mc_IAM->visible = false;
    mc_ISM->visible = false;
    mc_WHM->visible = false;

    /* -- Equipping item -- */
    i32 eq_L = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
    i32 eq_R = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT);
    if (hand == UNIT_EQUIP_LEFT) {
        /* Need to swap if item aready equipped in other hand 
        **  to not twohand. twohanding is for UNIT_EQUIP_TWO_HANDS */
        
        b32 swap = (eq_R == ism->selected_eq);
        if (swap) {
            Unit_Equipped_Swap(unit);
        } else {
            Unit_Equip(unit, UNIT_HAND_LEFT,    ism->selected_eq);
        }
    } else if (hand == UNIT_EQUIP_RIGHT) {
        /* Need to swap if item aready equipped in other hand 
        **  to not twohand. twohanding is for UNIT_EQUIP_TWO_HANDS */

        b32 swap = (eq_L == ism->selected_eq);
        if (swap) {
            Unit_Equipped_Swap(unit);
        } else {
            Unit_Equip(unit, UNIT_HAND_RIGHT,   ism->selected_eq);
        }

    } else if (hand == UNIT_EQUIP_TWO_HANDS) {
        Unit_Equip(unit, UNIT_HAND_LEFT,    ism->selected_eq);
        Unit_Equip(unit, UNIT_HAND_RIGHT,   ism->selected_eq);
    }

    /* --- Go back to IAM --- */

    /* - Pop WHM - */
    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped == IES->menus.which_hand);
    i32 num = DARR_NUM(IES->menus.stack);
    i32 top = IES->menus.stack[num - 1];
    SDL_assert(top == IES->menus.item_action);

    /* - Update IAM - */
    SDL_assert(IES->selected.unit_entity    > TNECS_NULL);
    Game_ItemActionMenu_Update(IES, IES->selected.unit_entity);
    SDL_assert(IES->menus.item_select       > TNECS_NULL);

    /* - Update PLS - */
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
    PopUp_Loadout_Stats *pls = popup->data;

    PopUp_Loadout_Stats_Unit(pls, IES->selected.unit_entity);

    /* - Focus on IAM - */
    mc_ISM->elem = 0;
    Game_cursorFocus_onMenu(IES);

    /* -- If unit moved -> Unit waits. -- */
    Point initial   = IES->selected.unit_initial_position;
    Point moved     = IES->selected.unit_moved_position;
    if ((initial.x != moved.x) || (initial.y != moved.y)) {
        /* - Make unit wait, AFTER ALL MENUS POPPED - */
        IES->menus.allpopped_event = event_Unit_Wait;
    }
}

/* --- eCrsMvs on WHM, for parent menu and mo --- */
void fsm_WHM_eCrsMvs_mIAM(Game *IES, Menu *mc_IAM) {
    /* --- Call fsm_WHM for menu_option --- */
    IES_check(IES);
    IES_check(mc_IAM);

    IES_assert(mc_IAM->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc_IAM->data;
    i32 mo_order = ItemActionMenu_Option_Order(iam, mc_IAM);

    if (fsm_WHM_eCrsMvs_mIAM_mo[mo_order] != NULL) {
        fsm_WHM_eCrsMvs_mIAM_mo[mo_order](IES, mc_IAM);
    }
}

void fsm_WHM_eCrsMvs_mIAM_moEquip(Game *IES, Menu *mc_IAM) {
    /* --- Update PLS for selected equipment --- */
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
    PopUp_Loadout_Stats *pls = popup->data;

    /* -- eq from ISM -- */
    Menu *mc_ISM = IES_GET_C(gl_world, IES->menus.item_select, Menu);
    SDL_assert(mc_ISM->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ISM->data;
    i32 eq = ism->selected_eq;

    /* -- hand from WHM -- */
    Menu *mc_WHM = IES_GET_C(gl_world, IES->menus.which_hand, Menu);
    WhichHandMenu *whm = mc_WHM->data;
    i32 hand  = WhichHandMenu_Hand(whm, mc_WHM->elem);

    /* -- Setting selected loadout to eq in hand -- */
    PopUp_Loadout_Stats_Selected_Reset(pls);
    _PopUp_Loadout_Stats_Select(pls, eq, hand);
    PopUp_Loadout_Stats_Selected_Stats(pls);
}
