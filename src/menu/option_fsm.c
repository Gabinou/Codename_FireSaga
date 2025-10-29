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

#include "AI.h"
#include "fsm.h"
#include "item.h"
#include "arrow.h"
#include "nmath.h"
#include "scene.h"
#include "events.h"
#include "slider.h"
#include "sprite.h"
#include "octant.h"
#include "globals.h"
#include "cutscene.h"
#include "position.h"
#include "platform.h"
#include "utilities.h"
#include "pathfinding.h"

#include "bars/map_hp.h"

#include "game/map.h"
#include "game/game.h"
#include "game/unit.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "map/path.h"
#include "map/ontile.h"
#include "map/render.h"

#include "menu/fsm.h"
#include "menu/menu.h"
#include "menu/trade.h"
#include "menu/first.h"
#include "menu/option_fsm.h"
#include "menu/deployment.h"
#include "menu/map_action.h"
#include "menu/item_select.h"
#include "menu/unit_action.h"
#include "menu/staff_select.h"
#include "menu/which_hand_fsm.h"

#include "popup/unit.h"
#include "popup/tile.h"
#include "popup/popup.h"
#include "popup/loadout_stats.h"

#include "systems/slide.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

/* --- Parent menu dependent mo_fsm --- */
const fsm_menu_t fsm_eAcpt_mIAM_mo[IAM_OPTION_NUM] = {
    /* EQUIP */     &fsm_eAcpt_mIAM_moEquip,
    /* USE   */     &fsm_eAcpt_mIAM_moUse,
    /* DROP  */     &fsm_eAcpt_mIAM_moDrop,
    /* TRADE */     &fsm_eAcpt_mIAM_moTrade
};

const fsm_menu_t fsm_eAcpt_mMAM_mo[MAM_OPTION_NUM] = {
    /* UNITS    */  &fsm_eAcpt_mMAM_moUnit,
    /* CONVOY   */  &fsm_eAcpt_mMAM_moCnvy,
    /* SETTINGS */  &fsm_eAcpt_mMAM_moStts,
    /* QUIT     */  &fsm_eAcpt_mMAM_moQuit,
    /* END_TURN */  &fsm_eAcpt_mMAM_moEndT
};

const fsm_menu_t fsm_eAcpt_mUAM_mo[UAM_OPTION_NUM] = {
    /* ITEMS    */  &fsm_eAcpt_mUAM_moItem,
    /* TRADE    */  &fsm_eAcpt_mUAM_moTrade,
    /* SEIZE    */  &fsm_eAcpt_mUAM_moSeize,
    /* TALK     */  &fsm_eAcpt_mUAM_moTalk,
    /* ATTACK   */  &fsm_eAcpt_mUAM_moAtk,
    /* STAFF    */  &fsm_eAcpt_mUAM_moStaff,
    /* DANCE    */  &fsm_eAcpt_mUAM_moDance,
    /* RESCUE   */  &fsm_eAcpt_mUAM_moRescue,
    /* OPEN     */  &fsm_eAcpt_mUAM_moOpen,
    /* WAIT     */  &fsm_eAcpt_mUAM_moWait,
    /* USE      */  &fsm_eAcpt_mUAM_moUse
};

const fsm_menu_t fsm_eAcpt_mFM_mo[FM_OPTION_NUM] = {
    /* DEBUG_MAP */ &fsm_eAcpt_mFM_moDbgMap,
    /* NEW_GAME */   NULL,
    /* SETTINGS */   NULL,
};

/* --- Parent menu independent mo_fsm --- */
const fsm_menu_t fsm_eCrsMvs_mo[MENU_OPTION_NUM] = {
    /* START */          NULL,
    /* ITEMS */          NULL,
    /* TALK */           NULL,
    /* STAFF */          NULL,
    /* DANCE */          NULL,
    /* RESCUE */         NULL,
    /* SEIZE */          NULL,
    /* ESCAPE */         NULL,
    /* ATTACK */        &fsm_eCrsMvs_moAtk,
    /* VILLAGE */        NULL,
    /* TRADE */          NULL,
    /* MAP */            NULL,
    /* WAIT */           NULL,
    /* OPEN */           NULL,
    /* QUIT */           NULL,
    /* END_TURN */       NULL,
    /* UNITS */          NULL,
    /* CONVOY */         NULL,
    /* GLOBAL_RANGE */   NULL,
    /* NEW_GAME */       NULL,
    /* LOAD */           NULL,
    /* ERASE */          NULL,
    /* COPY */           NULL,
    /* SETTINGS */       NULL,
    /* EXTRAS */         NULL,
    /* DEBUG_MAP */      NULL,
};

const fsm_menu_t fsm_eCncl_mo[MENU_OPTION_NUM] = {
    /* START */          NULL,
    /* ITEMS */          NULL,
    /* TALK */           NULL,
    /* STAFF */         &fsm_eCncl_moStaff,
    /* DANCE */         &fsm_eCncl_moDance,
    /* RESCUE */         NULL,
    /* SEIZE */          NULL,
    /* ESCAPE */         NULL,
    /* ATTACK */        &fsm_eCncl_moAtk,
    /* VILLAGE */        NULL,
    /* TRADE */         &fsm_eCncl_moTrade,
    /* MAP */            NULL,
    /* WAIT */           NULL,
    /* OPEN */           NULL,
    /* QUIT */           NULL,
    /* END_TURN */       NULL,
    /* UNITS */          NULL,
    /* CONVOY */         NULL,
    /* GLOBAL_RANGE */   NULL,
    /* NEW_GAME */       NULL,
    /* LOAD */           NULL,
    /* ERASE */          NULL,
    /* COPY */           NULL,
    /* SETTINGS */       NULL,
    /* EXTRAS */         NULL,
    /* DEBUG_MAP */      NULL,
    /* DROP */           NULL,
    /* EQUIP */          NULL,
    /* EQUIP_LEFT */     NULL,
    /* EQUIP_RIGHT */    NULL,
    /* EQUIP_TWOHAND */  NULL,
    /* USE */           &fsm_eCncl_moUse,
};

const fsm_menu_t fsm_eAcpt_mo[MENU_OPTION_NUM] = {
    /* START */          NULL,
    /* ITEMS */          NULL,
    /* TALK */           NULL,
    /* STAFF */         &fsm_eAcpt_moStaff,
    /* DANCE */         &fsm_eAcpt_moDance,
    /* RESCUE */         NULL,
    /* SEIZE */          NULL,
    /* ESCAPE */         NULL,
    /* ATTACK */        &fsm_eAcpt_moAtk,
    /* VILLAGE */        NULL,
    /* TRADE */         &fsm_eAcpt_moTrade,
    /* MAP */            NULL,
    /* WAIT */           NULL,
    /* OPEN */           NULL,
    /* QUIT */           NULL,
    /* END_TURN */       NULL,
    /* UNITS */          NULL,
    /* CONVOY */         NULL,
    /* GLOBAL_RANGE */   NULL,
    /* NEW_GAME */       NULL,
    /* LOAD */           NULL,
    /* ERASE */          NULL,
    /* COPY */           NULL,
    /* SETTINGS */       NULL,
    /* EXTRAS */         NULL,
    /* DEBUG_MAP */      NULL,
    /* DROP */           NULL,
    /* EQUIP */          NULL,
    /* EQUIP_LEFT */     NULL,
    /* EQUIP_RIGHT */    NULL,
    /* EQUIP_TWOHAND */  NULL,
    /* USE */           &fsm_eAcpt_moUse,
};

const fsm_menu_t fsm_eAcpt_mIDM_mo[IDM_OPTION_NUM] = {
    &fsm_eAcpt_mIDM_moYes,
    &fsm_eAcpt_mIDM_moNo
};

/* --- fsm_eAcpt_mo --- */
void fsm_eAcpt_mIDM_moYes(Game *IES, Menu *mc_IDM) {
    /* - Drop item - */
    /* - Go back to ISM (grandparent menu) - */
    /* - TODO: If unit moved, unit waits after popping menu stack - */

    /* -- Drop item. -- */
    SDL_assert(IES->selected.unit_entity    > TNECS_NULL);
    Unit *unit  = IES_GET_C(gl_world, IES->selected.unit_entity,
                            Unit);
    SDL_assert(unit != NULL);

    Menu *mc_ISM = IES_GET_C(   gl_world, IES->menus.item_select,
                                Menu);

    ItemSelectMenu *ism = mc_ISM->data;

    Unit_Item_Drop(unit, ism->selected_eq);

    /* -- Go back to ISM -- */
    /* - Pop IDM - */
    b32 destroy = false;
    Game_menuStack_Pop(IES, destroy);
    i32 num = DARR_NUM(IES->menus.stack);
    tnecs_E top = IES->menus.stack[num - 1];
    SDL_assert(top == IES->menus.item_action);

    /* - Pop IAM - */
    Game_menuStack_Pop(IES, destroy);
    num = DARR_NUM(IES->menus.stack);
    top = IES->menus.stack[num - 1];
    SDL_assert(top == IES->menus.item_select);

    /* - Update ISM - */
    SDL_assert(IES->selected.unit_entity    > TNECS_NULL);
    Game_ItemSelectMenu_Enable(IES, IES->selected.unit_entity);
    SDL_assert(IES->menus.item_select       > TNECS_NULL);

    /* - Focus on ISM - */
    mc_ISM->elem = 0;
    Game_cursorFocus_onMenu(IES);

    /* -- Make IAM, invisible -- */
    Menu *mc_IAM = IES_GET_C(gl_world, IES->menus.item_action, Menu);
    SDL_assert(mc_IAM != NULL);
    mc_IAM->visible = false;

    /* -- Make ISM, PopupLoadoutStats visible -- */
    SDL_assert(mc_ISM != NULL);
    mc_ISM->visible = true;

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(   gl_world, IES->popups.arr[popup_ind],
                                PopUp);
    popup->visible = true;

    /* -- If unit moved -> Unit waits. -- */
    Point initial   = IES->selected.unit_initial_position;
    Point moved     = IES->selected.unit_moved_position;
    if ((initial.x != moved.x) || (initial.y != moved.y)) {
        /* - TODO: Make unit wait, AFTER ALL MENUS POPPED - */
        IES->menus.allpopped_event = event_Unit_Wait;
    }
}

void fsm_eAcpt_mIDM_moNo(Game *IES, Menu *mc_IDM) {
    /* -- Go back to IAM -- */
    b32 destroy = false;
    Game_menuStack_Pop(IES, destroy);

    i32 num = DARR_NUM(IES->menus.stack);
    tnecs_E top = IES->menus.stack[num - 1];
    SDL_assert(top == IES->menus.item_action);
    Game_cursorFocus_onMenu(IES);

    /* -- Make IAM, ISM, PopupLoadoutStats visible -- */
    Menu *mc_IAM = IES_GET_C(gl_world, IES->menus.item_action, Menu);
    SDL_assert(mc_IAM != NULL);
    mc_IAM->visible = true;

    Menu *mc_ISM = IES_GET_C(gl_world, IES->menus.item_select, Menu);
    SDL_assert(mc_ISM != NULL);
    mc_ISM->visible = true;

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
    popup->visible = true;
}

void fsm_eAcpt_moTrade(Game *sota, Menu *in_mc) {
    /* - Open Trade menu - */
    tnecs_E active   = sota->selected.unit_entity;
    tnecs_E passive  = sota->targets.candidates[sota->targets.order];
    SDL_assert(active       > TNECS_NULL);
    SDL_assert(passive      > TNECS_NULL);
    // Game_TradeMenu_Enable(sota, active, passive);

    Game_subState_Set(
            sota, GAME_SUBSTATE_MENU,
            "Time to trade"
    );

    /* DESIGN QUESTION: should unit wait if weapon was traded? */
}

void fsm_eAcpt_moDance(   Game *sota,
                          Menu *in_mc) {
    /* - Refresh spectator - */
    SDL_assert(sota->targets.candidates != NULL);
    SDL_assert(sota->targets.candidates[sota->targets.order] > TNECS_NULL);
    tnecs_E spectator = sota->targets.candidates[sota->targets.order];
    SDL_assert(spectator > TNECS_NULL);
    Game_Unit_Refresh(sota, spectator);

    /* - Dancer waits - */
    tnecs_E dancer = sota->selected.unit_entity;
    SDL_assert(dancer > TNECS_NULL);
    Game_Unit_Wait(sota, dancer);

    /* - hide movemap - */
    Map *map = Game_Map(sota);
    Map_Stacked_Dangermap_Reset(map);
    Map_Palettemap_Reset(map);

    /* -- Deselect unit and go back to map -- */
    tnecs_E *data2 = IES_calloc(1, sizeof(*data2));
    *data2 = sota->selected.unit_entity;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Unit_Deselect,
                NULL, data2);
    if (DARR_NUM(sota->menus.stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby,
                NULL, NULL);
}

void fsm_eAcpt_moStaff(   Game *sota,
                          Menu *_mc) {
    Menu *mc = IES_GET_C(gl_world, sota->menus.staff_select, Menu);
    SDL_assert(mc != NULL);
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(ssm != NULL);

    /* - Healer uses staff on patient - */
    tnecs_E healer_E     = sota->selected.unit_entity;
    SDL_assert(healer_E == ssm->_unit);
    tnecs_E patient_ent    = sota->targets.candidates[sota->targets.order];
    struct Unit *healer     = IES_GET_C(gl_world, healer_E, Unit);
    struct Unit *patient    = IES_GET_C(gl_world, patient_ent, Unit);
    i32 stronghand  = Unit_Hand_Strong(healer);
    i32 weakhand    = Unit_Hand_Weak(healer);

    SDL_assert(Unit_isEquipped(healer, weakhand));
    SDL_assert(Unit_isEquipped(healer, stronghand));

    Unit_Staff_Use(healer, patient);

    /* - Update maphpbar - */
    MapHPBar *map_hp_bar = IES_GET_C(   gl_world, patient_ent,
                                        MapHPBar);
    SDL_assert(map_hp_bar           != NULL);
    SDL_assert(map_hp_bar->unit_ent == patient_ent);
    map_hp_bar->update  = true;
    map_hp_bar->visible = true;

    /* - Healer waits - */
    SDL_assert(healer_E > TNECS_NULL);
    Game_Unit_Wait(sota, healer_E);

    /* - hide movemap - */
    Map *map = Game_Map(sota);
    Map_Stacked_Dangermap_Reset(map);
    Map_Palettemap_Reset(map);

    /* -- Deselect unit and go back to map -- */
    tnecs_E *data2 = IES_calloc(1, sizeof(*data2));
    *data2 = sota->selected.unit_entity;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Unit_Deselect,
                NULL, data2);
    if (DARR_NUM(sota->menus.stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby,
                NULL, NULL);
}

void fsm_eAcpt_moAtk(Game *sota, Menu *in_mc) {
    /* Defendant was selected, we can attack */
    /* - Set Defendant to selected unit - */
    SDL_assert(sota->targets.candidates != NULL);
    SDL_assert(sota->targets.candidates[sota->targets.order] > TNECS_NULL);
    sota->combat.defendant = sota->targets.candidates[sota->targets.order];
    sota->targets.previous_order = sota->targets.order;
    SDL_assert(sota->combat.defendant > TNECS_NULL);
    SDL_assert(sota->combat.aggressor > TNECS_NULL);

    /* - Make cursor invisible - */
    Sprite *sprite = IES_GET_C(gl_world, sota->cursor.entity, Sprite);
    sprite->visible = false;

    /* - Send Defendant_Select event - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Defendant_Select,
                NULL, NULL);
}

/* --- fsm_eCncl_mo --- */
void fsm_eCncl_moDance(Game *sota, Menu *in_mc) {
    /* 1. Turn menu_player_select_unit_action visible */
    tnecs_E menu = sota->menus.unit_action;
    Menu *mc = IES_GET_C(gl_world, menu, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_UNIT_ACTION);

    /* 2. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 3. Move cursor to Dance menu option */
    UnitActionMenu *uam = mc->data;
    int new_elem = ActionMenu_Option_Index(uam, MENU_OPTION_DANCE);
    Menu_Elem_Set(mc, sota, new_elem);
}

void fsm_eCncl_moStaff(   Game *sota,
                          Menu *in_mc) {

    /* 1. Turn top menu (Staff_Select_Menu) visible */
    int stack_top = DARR_NUM(sota->menus.stack) - 1;
    tnecs_E menu_top = sota->menus.stack[stack_top];
    Menu *mc = IES_GET_C(gl_world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_STAFF_SELECT);

    /* 2. Cancel one of the selection of item select menu */
    struct LoadoutSelectMenu *ssm = mc->data;
    LoadoutSelectMenu_Deselect(ssm);

    /* 3. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 4. Move cursor to correct hand */
    // new_elem is hand 0 if its -1, else its hand 1
    // tnecs_E cursor = sota->cursor.entity;
    Menu_Elem_Set(mc, sota, 0);
}

void fsm_eCncl_moTrade(   Game *sota,
                          Menu *in_mc) {
    /* 1. Turn Player_Select_Menu visible */
    int stack_top = DARR_NUM(sota->menus.stack) - 1;
    tnecs_E menu_top = sota->menus.stack[stack_top];
    Menu *mc = IES_GET_C(gl_world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;

    /* 2. Update options */
    Game_preUnitAction_Targets(sota, sota->selected.unit_entity);

    /* 3. Focus on menu */
    Game_cursorFocus_onMenu(sota);
}

void fsm_eCncl_moUse( Game *sota, Menu *_) {
    /* Cancelling choice of target to use item on.
    Go back to WHM */

    /* 1. Focus on top stack menu */
    Game_cursorFocus_onMenu(sota);
    /* Game_cursorFocus_onMenu disables other menus.
    **  - Done first to make other menus visible.  */

    /* 2.1 Turn ItemActionMenu visible */
    int stack_top = DARR_NUM(sota->menus.stack) - 1;
    tnecs_E top_E = sota->menus.stack[stack_top];
    Menu *mc_top = IES_GET_C(gl_world, top_E, Menu);
    SDL_assert(mc_top           != NULL);
    SDL_assert(mc_top->elem_pos != NULL);
    mc_top->visible = true;

    if (mc_top->type == MENU_TYPE_WHICH_HAND) {
        /* 2.2 Also turn WHM parent menu visible*/
        SDL_assert(stack_top >= 1);
        tnecs_E parent_E = sota->menus.stack[stack_top - 1];
        Menu *mc_parent = IES_GET_C(gl_world, parent_E, Menu);
        mc_parent->visible = true;
    }

    /* 3. Turn PopupLoadoutStats Visible */
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
    popup->visible = true;
}

/* --- fsm utils --- */
// Maybe Game_Item_Use?
void fsm_Item_Use(  Game *IES,
                    tnecs_E user_E, tnecs_E patient_E) {

    /* Use item, start animation, go back to standby */

    /* - User is selected unit - */
    Inventory_item *invitem = IES_GET_C(gl_world, IES->selected.item, Inventory_item);
    const Item *item = Item_Get(invitem);

    Unit *patient   = IES_GET_C(gl_world, patient_E, Unit);
    /* - User is selected unit - */
    Unit *user      = IES_GET_C(gl_world, user_E, Unit);

    /* - Using item on patient - */
    Item_Use(item, user, &patient, 1);

    /* - Make unit wait, RIGHT NOW - */
    SDL_Event ev;
    SDL_zero(ev);
    receive_event_Unit_Wait(IES, &ev);

    /* - Update maphpbar - */
    MapHPBar *map_hp_bar = IES_GET_C(   gl_world, patient_E,
                                        MapHPBar);
    SDL_assert(map_hp_bar           != NULL);
    SDL_assert(map_hp_bar->unit_ent == patient_E);
    map_hp_bar->update  = true;
    map_hp_bar->visible = true;

    /* - TODO: Start Use animation - */

    /* - Go back to standby - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby,
                NULL, NULL);
}

void fsm_eAcpt_moUse(Game *IES, Menu *in_mc) {
    /* Item target was chosen, use it */

    /* - Set patient to candidate - */
    SDL_assert(IES->targets.candidates != NULL);
    SDL_assert(IES->targets.candidates[IES->targets.order] > TNECS_NULL);
    tnecs_E user_E      = IES->selected.unit_entity;
    tnecs_E patient_E   = IES->targets.candidates[IES->targets.order];

    fsm_Item_Use(IES, user_E, patient_E);
}

void fsm_eCncl_moAtk( Game *sota,
                      Menu *in_mc) {
    /* Cancelling attack defendant choice. Go back to WSM
    /* 1. Turn Weapon_Select_Menu visible */
    int stack_top           = DARR_NUM(sota->menus.stack) - 1;
    tnecs_E menu_top   = sota->menus.stack[stack_top];
    Menu *mc = IES_GET_C(gl_world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);

    /* 2. Cancel one of the selection of weapon select menu */
    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(Loadout_isEquipped(&wsm->selected, UNIT_HAND_LEFT)
               || Loadout_isEquipped(&wsm->selected, UNIT_HAND_RIGHT));
    LoadoutSelectMenu_Deselect(wsm);
    SDL_assert(!Loadout_isEquipped(&wsm->selected, UNIT_HAND_LEFT) ||
               !Loadout_isEquipped(&wsm->selected, UNIT_HAND_RIGHT));

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);

    /* 3. Focus on WSM */
    Game_cursorFocus_onMenu(sota);

    /* 4. HUD reappear */
    popup->visible = true;

    /* 5. Remove PopupPre_Combat */
    Game_PopUp_Pre_Combat_Hide(sota);
}

/* event_Cursor_Moves */
void fsm_eCrsMvs_moAtk( Game *sota,
                        Menu *mc) {
    /* Set defendant to current candidate */
    SDL_assert(sota->targets.candidates[sota->targets.order] != TNECS_NULL);
    sota->combat.defendant = sota->targets.candidates[sota->targets.order];
    SDL_assert(sota->combat.defendant != TNECS_NULL);
}

void fsm_eAcpt_mUAM_moWait(Game *sota, Menu *mc) {
    /* -- Pop all menus -- */
    b32 destroy = false;
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    /* -- Make unit wait, RIGHT NOW --- */
    SDL_Event ev;
    SDL_zero(ev);
    receive_event_Unit_Wait(sota, &ev);
}

void fsm_eAcpt_mUAM_moUse(Game *IES, Menu *mc) {
    SDL_Log(__func__);

    /* -- Pop all menus -- */
    b32 destroy = false;
    while (DARR_NUM(IES->menus.stack) > 0) {
        Game_menuStack_Pop(IES, destroy);
    }

    /* -- Use item, RIGHT NOW --- */
    SDL_assert(DARR_NUM(IES->targets.patients) > 0);
    SDL_Event ev;
    SDL_zero(ev);
    receive_event_Item_Use(IES, &ev);
}

void fsm_eAcpt_mUAM_moOpen(Game *sota, Menu *mc) {

}

void fsm_eAcpt_mUAM_moRescue(Game *sota, Menu *mc) {

}

void fsm_eAcpt_mUAM_moTalk(Game *sota, Menu *mc) {

}

void fsm_eAcpt_mUAM_moSeize(Game *sota, Menu *mc) {
    SDL_Log("Throne was seized: Map was won!");
    Map *map = Game_Map(sota);
    map->flags.win = true;

    /* - Go back to standby - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby,
                NULL, NULL);
}

void fsm_eAcpt_mUAM_moDance(Game *sota, Menu *mc) {
    SDL_assert(DARR_NUM(sota->targets.spectators) > 0);

    /* - Turn player_select_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    SDL_assert(Game_State_Current(sota)      == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);
    Game_Switch_toCandidates(
            sota, sota->targets.spectators,
            "Dance was selected, time to select spectator"
    );
}

void fsm_eAcpt_mUAM_moStaff(Game *sota, Menu *mc) {
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    /* -- Create StaffSelectMenu -- */
    if (sota->menus.staff_select == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);

    SDL_assert(sota->menus.staff_select    > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity > TNECS_NULL);
    Game_StaffSelectMenu_Enable(sota, sota->selected.unit_entity);

    /* -- Enable healmap rangemap to choose patients -- */
    struct Unit *unit = IES_GET_C(gl_world, sota->selected.unit_entity, Unit);
    canEquip can_equip  = canEquip_default;
    canEquip_Loadout_None(&can_equip, UNIT_HAND_LEFT);
    canEquip_Loadout_None(&can_equip, UNIT_HAND_RIGHT);
    can_equip.archetype         = ITEM_ARCHETYPE_STAFF;
    can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;
    struct Unit_Equippable equippable  = Unit_canEquip_Equipment(unit,
                                         can_equip);
    SDL_assert(equippable.num > 0);

    // TODO: save rangemap previous state? how to go back
    Map *map = Game_Map(sota);
    Unit_Rangemap_set(unit, RANGEMAP_HEALMAP);
    Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL, sota->selected.unit_entity);
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    /* -- Create PopUp_Loadout_Stats -- */
    // TODO: only if selecting weapon?
    // if (sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
    //     Game_PopUp_Loadout_Stats_Create(sota);

    // PopUp_Loadout_Stats_Unit(pls, unit);

    /* -- TODO: Render Face -- */
}

void fsm_eAcpt_mUAM_moAtk(Game *sota, Menu *mc_bad) {
    /* -- Create WeaponSelectMenu -- */
    //      -> WeaponsSelectMenu should only show weapons USABLE ON TILE

    /* Select loadout first */
    if (sota->menus.weapon_select == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);

    SDL_assert(sota->menus.weapon_select    > TNECS_NULL);
    SDL_assert(sota->selected.unit_entity   > TNECS_NULL);
    Game_WeaponSelectMenu_Enable(sota, sota->selected.unit_entity);

    Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.weapon_select, Menu);
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    /* 4. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 5. Move cursor to weak hand, strong hand on top */
    Menu_Elem_Set(mc, sota, 0);

    struct LoadoutSelectMenu *wsm = mc->data;
    wsm->update = true;

    /* -- Create PopUp_Loadout_Stats -- */
    if (sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
        Game_PopUp_Loadout_Stats_Create(sota);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
    popup->visible = true;
    struct PopUp_Loadout_Stats *pls = popup->data;

    struct Unit *unit = IES_GET_C(gl_world, sota->selected.unit_entity, Unit);
    PopUp_Loadout_Stats_Unit(pls, sota->selected.unit_entity);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Selected_Loadout(pls);
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
    PopUp_Loadout_Stats_Selected_Stats(pls);

    /* - Compute new attackmap with equipped - */
    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;
    Loadout *loadout = (Loadout *)&map_to;
    Loadout_Set(loadout, UNIT_HAND_LEFT,   Loadout_Eq(&pls->loadout_selected, UNIT_HAND_LEFT));
    Loadout_Set(loadout, UNIT_HAND_RIGHT,  Loadout_Eq(&pls->loadout_selected, UNIT_HAND_RIGHT));

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_LOADOUT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = sota->combat.aggressor;

    /* - healtopmap - */
    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    /* -- Enable attack rangemap to choose defendants -- */
    // TODO: save rangemap previous state? how to go back
    Unit_Rangemap_set(unit, RANGEMAP_ATTACKMAP);

    Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    /* -- TODO: Render Face -- */
}

void fsm_eAcpt_mUAM_moItem(Game *sota, Menu *mc) {
    /* -- Create ItemSelectMenu -- */
    SDL_assert(sota->selected.unit_entity   > TNECS_NULL);
    Game_ItemSelectMenu_Enable(sota, sota->selected.unit_entity);
    SDL_assert(sota->menus.item_select      > TNECS_NULL);

    /* -- Create PopUp_Loadout_Stats -- */
    if (sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
        Game_PopUp_Loadout_Stats_Create(sota);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
    popup->visible = true;
    PopUp_Loadout_Stats *pls = popup->data;

    PopUp_Loadout_Stats_Unit(pls, sota->selected.unit_entity);
    /* -- TODO: unit face -- */
}

void fsm_eAcpt_mFM_moDbgMap(Game *sota, Menu *mc) {
    SDL_assert((Game_State_Current(sota) == GAME_STATE_Title_Screen));

    Event_Emit( __func__, SDL_USEREVENT,
                event_Load_Debug_Map,
                NULL, NULL);
}

void fsm_eAcpt_mIAM_moEquip(Game *IES, Menu *mc_IAM) {
}

void fsm_eAcpt_mIAM_moUse(Game *IES, Menu *mc_IAM) {
    /* --- Decided to use item ---
    **  1. Each item needs to be equipped
    **  2. Need to decide which hand to equip
    **      - Items are 1H only
    */

    /* --- 1. Enable WHM --- */
    SDL_assert(IES->selected.unit_entity    != TNECS_NULL);
    SDL_assert(IES->selected.item           != TNECS_NULL);
    Game_WHM_Create(IES);
    Game_WHM_Enable(IES);

    /* --- 2. IAM stays visible --- */
    // Show player what option what selected
    SDL_assert(IES->menus.item_action > TNECS_NULL);
    Menu *mc_iam = IES_GET_C(   gl_world,
                                IES->menus.item_action,
                                Menu);
    SDL_assert(mc_iam != NULL);
    mc_iam->visible = true;
}

void fsm_eAcpt_mIAM_moDrop(Game *IES, Menu *mc) {
    /* -- Enable ItemDrop menu -- */
    SDL_assert(IES->selected.unit_entity   > TNECS_NULL);
    SDL_assert(IES->menus.item_select      > TNECS_NULL);
    Game_ItemDropMenu_Enable(IES);

}

void fsm_eAcpt_mIAM_moTrade(Game *s, Menu *mc) {

}
void fsm_eAcpt_mMAM_moUnit(Game *sota, Menu *mc) {

}
void fsm_eAcpt_mMAM_moCnvy(Game *sota, Menu *mc) {

}
void fsm_eAcpt_mMAM_moStts(Game *sota, Menu *mc) {

}
void fsm_eAcpt_mMAM_moQuit(Game *sota, Menu *mc) {
    Event_Emit( __func__, SDL_USEREVENT,
                event_Quit,
                NULL, NULL);
}

void fsm_eAcpt_mMAM_moEndT(Game *sota, Menu *mc) {
    // int armies.current = Map_Army_Next(map);
    // Todo start transition animation
    // Todo switch control to next army

    Event_Emit( __func__,       SDL_USEREVENT,
                event_Turn_End,
                NULL, NULL);
}

/* --- fsm_eAcpt_mUAM --- */
void fsm_eAcpt_mUAM_moTrade(Game *sota, Menu *mc) {

    /* - Turn player_select_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    Game_Switch_toCandidates(
            sota, sota->targets.passives,
            "Trade was selected, time to select passive trade"
    );

    // IF staff skill
    // - IF one item selected
    //  - Deselect item
    // - IF no item selected
    //  - Destroy staff_select menu
}