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
** fsm: Finite State Machine for game.menu
**  - Different behavior according to menu & game state
**
*/

#include "item.h"
#include "tnecs.h"
#include "nmath.h"
#include "arrow.h"
#include "sprite.h"
#include "events.h"
#include "weapon.h"
#include "globals.h"
#include "position.h"
#include "cutscene.h"

#include "bars/map_hp.h"

#include "game/unit.h"
#include "game/game.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"
#include "game/map.h"

#include "map/path.h"
#include "map/render.h"
#include "map/ontile.h"

#include "menu/fsm.h"
#include "menu/menu.h"
#include "menu/trade.h"
#include "menu/first.h"
#include "menu/deployment.h"
#include "menu/map_action.h"
#include "menu/item_select.h"
#include "menu/unit_action.h"
#include "menu/staff_select.h"

#include "popup/popup.h"
#include "popup/loadout_stats.h"

#include "unit/unit.h"
#include "unit/range.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

/* --- Menu FSMs --- */

/* -- Menu-type FSMs -- */

/* - menu_elem_move - */
const menu_elem_move_t menu_elem_move[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT  */  &WeaponSelectMenu_Elem_Move,
    /* MENU_TYPE_STAFF_SELECT  */   &Menu_Elem_Move,
    /* MENU_TYPE_ITEM_SELECT  */    &ItemSelectMenu_Elem_Move,
    /* MENU_TYPE_STATS */           &Menu_Elem_Move,
    /* MENU_TYPE_RESCUE */          &Menu_Elem_Move,
    /* MENU_TYPE_SUPPORTS */        &Menu_Elem_Move,
    /* MENU_TYPE_GROWTHS */         &Menu_Elem_Move,
    /* MENU_TYPE_TRADE */           &Menu_Elem_Move,
    /* MENU_TYPE_ITEM_DROP */       &Menu_Elem_Move,
    /* MENU_TYPE_DEPLOYMENT */      &DeploymentMenu_Elem_Move,
    /* MENU_TYPE_FIRST */           &FirstMenu_Elem_Move,
    /* MENU_TYPE_UNIT_ACTION */     &UnitActionMenu_Elem_Move,
    /* MENU_TYPE_ITEM_ACTION */     &MapActionMenu_Elem_Move,
    /* MENU_TYPE_MAP_ACTION */      &MapActionMenu_Elem_Move,
};

/* - Menu substate FSMs - */
const fsm_menu_t fsm_eStart_sPrep_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT  */  NULL,
    /* MENU_TYPE_STAFF_SELECT  */   NULL,
    /* MENU_TYPE_ITEM_SELECT  */    NULL,
    /* MENU_TYPE_STATS */           &fsm_eStart_sPrep_ssMenu_mSM,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      &fsm_eStart_sPrep_ssMenu_mDM,
    /* MENU_TYPE_FIRST */           NULL,
};

const fsm_menu_t fsm_eStats_sPrep_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT  */  NULL,
    /* MENU_TYPE_STAFF_SELECT  */   NULL,
    /* MENU_TYPE_ITEM_SELECT  */    NULL,
    /* MENU_TYPE_STATS */           &fsm_eStats_sPrep_ssMenu_mSM,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      &fsm_eStats_sPrep_ssMenu_mDM,
    /* MENU_TYPE_FIRST */           NULL,
};

const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT  */  &fsm_eAcpt_sGmpMap_ssMenu_mLSM,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_eAcpt_sGmpMap_ssMenu_mSSM,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_eAcpt_sGmpMap_ssMenu_mISM,
    /* MENU_TYPE_STATS */           &fsm_eAcpt_sGmpMap_ssMenu_mSM,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           &fsm_eAcpt_sGmpMap_ssMenu_mTM,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      &fsm_eAcpt_sGmpMap_ssMenu_mDM,
    /* MENU_TYPE_FIRST */           NULL,
    /* MENU_TYPE_UNIT_ACTION */     &fsm_eAcpt_sGmpMap_ssMenu_mUAM,
    /* MENU_TYPE_ITEM_ACTION */     &fsm_eAcpt_sGmpMap_ssMenu_mIAM,
    /* MENU_TYPE_MAP_ACTION */      &fsm_eAcpt_sGmpMap_ssMenu_mMAM,
};

const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mIAM_mo[UAM_OPTION_NUM] = {
    /* MENU_OPTION_EQUIP */     &fsm_eAcpt_sGmpMap_ssMenu_mIAM_moEquip,
    /* MENU_OPTION_USE   */     &fsm_eAcpt_sGmpMap_ssMenu_mIAM_moUse,
    /* MENU_OPTION_DROP  */     &fsm_eAcpt_sGmpMap_ssMenu_mIAM_moDrop,
    /* MENU_OPTION_TRADE */     &fsm_eAcpt_sGmpMap_ssMenu_mIAM_moTrade
};

const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mMAM_mo[UAM_OPTION_NUM] = {
    /* MENU_OPTION_UNITS    */  &fsm_eAcpt_sGmpMap_ssMenu_mMAM_moUnit,
    /* MENU_OPTION_CONVOY   */  &fsm_eAcpt_sGmpMap_ssMenu_mMAM_moCnvy,
    /* MENU_OPTION_SETTINGS */  &fsm_eAcpt_sGmpMap_ssMenu_mMAM_moStts,
    /* MENU_OPTION_QUIT     */  &fsm_eAcpt_sGmpMap_ssMenu_mMAM_moQuit,
    /* MENU_OPTION_END_TURN */  &fsm_eAcpt_sGmpMap_ssMenu_mMAM_moEndT
};

const fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mUAM_mo[UAM_OPTION_NUM] = {
    /* MENU_OPTION_ITEMS    */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moItem,
    /* MENU_OPTION_TRADE    */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTrade,
    /* MENU_OPTION_SEIZE    */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moSeize,
    /* MENU_OPTION_TALK     */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTalk,
    /* MENU_OPTION_ATTACK   */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moAtk,
    /* MENU_OPTION_STAFF    */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moStaff,
    /* MENU_OPTION_DANCE    */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moDance,
    /* MENU_OPTION_RESCUE   */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moRescue,
    /* MENU_OPTION_OPEN     */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moOpen,
    /* MENU_OPTION_WAIT     */  &fsm_eAcpt_sGmpMap_ssMenu_mUAM_moWait
};

const fsm_menu_t fsm_eAcpt_sTtlScrn_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT  */  NULL,
    /* MENU_TYPE_STAFF_SELECT  */   NULL,
    /* MENU_TYPE_ITEM_SELECT  */    NULL,
    /* MENU_TYPE_STATS */           NULL,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      NULL,
    /* MENU_TYPE_FIRST */           &fsm_eAcpt_sTtlScrn_ssMenu_mFM,
};

const fsm_menu_t fsm_eCncl_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT */   &fsm_eCncl_sGmpMap_ssMenu_mLSM,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_eCncl_sGmpMap_ssMenu_mSSM,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_eCncl_sGmpMap_ssMenu_mISM,
    /* MENU_TYPE_STATS */           &fsm_eCncl_sGmpMap_ssMenu_mSM,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         &fsm_eCncl_sGmpMap_ssMenu_mSM,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      NULL,
    /* MENU_TYPE_FIRST */           NULL,
    /* MENU_TYPE_UNIT_ACTION */     &fsm_eCncl_sGmpMap_ssMenu_mUAM,
    /* MENU_TYPE_ITEM_ACTION */     &fsm_eCncl_sGmpMap_ssMenu_mIAM,
    /* MENU_TYPE_MAP_ACTION */      &fsm_eCncl_sGmpMap_ssMenu_mMAM,
};

const fsm_menu_t fsm_eCncl_sPrep_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT */   NULL,
    /* MENU_TYPE_STAFF_SELECT  */   NULL,
    /* MENU_TYPE_ITEM_SELECT  */    NULL,
    /* MENU_TYPE_STATS */           &fsm_eCncl_sPrep_ssMenu_mSM,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      &fsm_eCncl_sPrep_ssMenu_mDM,
    /* MENU_TYPE_FIRST */           NULL,
};

const fsm_menu_t fsm_eCrsMvs_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT */   &fsm_eCrsMvs_sGmpMap_ssMenu_mLSM,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_eCrsMvs_sGmpMap_ssMenu_mSSM,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_eCrsMvs_sGmpMap_ssMenu_mISM,
    /* MENU_TYPE_STATS */           NULL,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEM_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      NULL,
    /* MENU_TYPE_FIRST */           NULL,
};

/* - Menu option FSMs - */
/* TODO: move to separate file*/
const fsm_menu_t fsm_eCrsMvs_sGmpMap_mo[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         NULL,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         NULL,
    /* MENU_OPTION_DANCE */         NULL,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_eCrsMvs_sGmpMap_moAtk,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         NULL,
    /* MENU_OPTION_MAP */           NULL,
    /* MENU_OPTION_WAIT */          NULL,
    /* MENU_OPTION_OPEN */          NULL,
    /* MENU_OPTION_QUIT */          NULL,
    /* MENU_OPTION_END_TURN */      NULL,
    /* MENU_OPTION_UNITS */         NULL,
    /* MENU_OPTION_CONVOY */        NULL,
    /* MENU_OPTION_GLOBAL_RANGE */  NULL,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_LOAD */          NULL,
    /* MENU_OPTION_ERASE */         NULL,
    /* MENU_OPTION_COPY */          NULL,
    /* MENU_OPTION_SETTINGS */      NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     NULL,
};

const fsm_menu_t fsm_eAcpt_sTtlScrn_ssMenu_mFM_mo[FM_OPTION_NUM] = {
    /* MENU_OPTION_DEBUG_MAP */     &fsm_eAcpt_sTtlScrn_ssMenu_mFM_moDbgMap,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_SETTINGS */      NULL,
};

// Cancel depending on previous menu_option when selecting map candidates
const fsm_menu_t fsm_eCncl_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         NULL,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_eCncl_sGmpMap_ssMapCndt_moStaff,
    /* MENU_OPTION_DANCE */         &fsm_eCncl_sGmpMap_ssMapCndt_moDance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_eCncl_sGmpMap_ssMapCndt_moAtk,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_eCncl_sGmpMap_ssMapCndt_moTrade,
    /* MENU_OPTION_MAP */           NULL,
    /* MENU_OPTION_WAIT */          NULL,
    /* MENU_OPTION_OPEN */          NULL,
    /* MENU_OPTION_QUIT */          NULL,
    /* MENU_OPTION_END_TURN */      NULL,
    /* MENU_OPTION_UNITS */         NULL,
    /* MENU_OPTION_CONVOY */        NULL,
    /* MENU_OPTION_GLOBAL_RANGE */  NULL,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_LOAD */          NULL,
    /* MENU_OPTION_ERASE */         NULL,
    /* MENU_OPTION_COPY */          NULL,
    /* MENU_OPTION_SETTINGS */      NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     NULL,
    /* MENU_OPTION_DROP */          NULL,
    /* MENU_OPTION_EQUIP */         NULL,
    /* MENU_OPTION_EQUIP_LEFT */    NULL,
    /* MENU_OPTION_EQUIP_RIGHT */   NULL,
    /* MENU_OPTION_EQUIP_TWOHAND */ NULL,
    /* MENU_OPTION_USE */           &fsm_eCncl_sGmpMap_ssMapCndt_moUse,
};

// Accept depending on previous menu_option when selecting map candidates
const fsm_menu_t fsm_eAcpt_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         NULL,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_eAcpt_sGmpMap_ssMapCndt_moStaff,
    /* MENU_OPTION_DANCE */         &fsm_eAcpt_sGmpMap_ssMapCndt_moDance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_eAcpt_sGmpMap_ssMapCndt_moAtk,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_eAcpt_sGmpMap_ssMapCndt_moTrade,
    /* MENU_OPTION_MAP */           NULL,
    /* MENU_OPTION_WAIT */          NULL,
    /* MENU_OPTION_OPEN */          NULL,
    /* MENU_OPTION_QUIT */          NULL,
    /* MENU_OPTION_END_TURN */      NULL,
    /* MENU_OPTION_UNITS */         NULL,
    /* MENU_OPTION_CONVOY */        NULL,
    /* MENU_OPTION_GLOBAL_RANGE */  NULL,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_LOAD */          NULL,
    /* MENU_OPTION_ERASE */         NULL,
    /* MENU_OPTION_COPY */          NULL,
    /* MENU_OPTION_SETTINGS */      NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     NULL,
    /* MENU_OPTION_DROP */          NULL,
    /* MENU_OPTION_EQUIP */         NULL,
    /* MENU_OPTION_EQUIP_LEFT */    NULL,
    /* MENU_OPTION_EQUIP_RIGHT */   NULL,
    /* MENU_OPTION_EQUIP_TWOHAND */ NULL,
    /* MENU_OPTION_USE */           &fsm_eAcpt_sGmpMap_ssMapCndt_moUse,

};

/* --- fsm_eAcpt_sGmpMap_ssMapCndt_mo --- */
void fsm_eAcpt_sGmpMap_ssMapCndt_moTrade(Game *sota, Menu *in_mc) {
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

void fsm_eAcpt_sGmpMap_ssMapCndt_moDance(Game *sota, Menu *in_mc) {

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
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected.unit_entity);
    if (DARR_NUM(sota->menus.stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);
}

void fsm_eAcpt_sGmpMap_ssMapCndt_moStaff(Game *sota, Menu *_mc) {
    Menu *mc = IES_GET_C(gl_world, sota->menus.staff_select, Menu);
    SDL_assert(mc != NULL);
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(ssm != NULL);

    /* - Healer uses staff on patient - */
    tnecs_E healer_ent     = sota->selected.unit_entity;
    SDL_assert(healer_ent == ssm->_unit);
    tnecs_E patient_ent    = sota->targets.candidates[sota->targets.order];
    struct Unit *healer     = IES_GET_C(gl_world, healer_ent, Unit);
    struct Unit *patient    = IES_GET_C(gl_world, patient_ent, Unit);
    i32 stronghand  = Unit_Hand_Strong(healer);
    i32 weakhand    = Unit_Hand_Weak(healer);

    SDL_assert(Unit_isEquipped(healer, weakhand));
    SDL_assert(Unit_isEquipped(healer, stronghand));

    Unit_Staff_Use(healer, patient);

    /* - Update maphpbar - */
    struct MapHPBar *map_hp_bar = IES_GET_C(gl_world, patient_ent, MapHPBar);
    SDL_assert(map_hp_bar           != NULL);
    SDL_assert(map_hp_bar->unit_ent == patient_ent);
    map_hp_bar->update  = true;
    map_hp_bar->visible = true;

    /* - Healer waits - */
    SDL_assert(healer_ent > TNECS_NULL);
    Game_Unit_Wait(sota, healer_ent);

    /* - hide movemap - */
    Map *map = Game_Map(sota);
    Map_Stacked_Dangermap_Reset(map);
    Map_Palettemap_Reset(map);

    /* -- Deselect unit and go back to map -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected.unit_entity);
    if (DARR_NUM(sota->menus.stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit( __func__, SDL_USEREVENT,
                event_Gameplay_Return2Standby,
                data1_entity, NULL);
}

void fsm_eAcpt_sGmpMap_ssMapCndt_moAtk(Game *sota, Menu *in_mc) {
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
                data1_entity, data2_entity);
}

/* --- fsm_eCncl_sGmpMap_ssMapCndt_mo --- */
void fsm_eCncl_sGmpMap_ssMapCndt_moDance(Game *sota, Menu *in_mc) {
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

void fsm_eCncl_sGmpMap_ssMapCndt_moStaff(Game *sota, Menu *in_mc) {

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

void fsm_eCncl_sGmpMap_ssMapCndt_moTrade(Game *sota, Menu *in_mc) {
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

void fsm_eCncl_sGmpMap_ssMapCndt_moUse(Game *sota, Menu *in_mc) {
    /* Cancelling choice of target to use item on.
    Go back to IAM */
    /* 1. Turn ItemActionMenu visible */
    int stack_top = DARR_NUM(sota->menus.stack) - 1;
    tnecs_E menu_top = sota->menus.stack[stack_top];
    Menu *mc = IES_GET_C(gl_world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_ITEM_ACTION);

    /* 2. Focus on IAM */
    Game_cursorFocus_onMenu(sota);
}

void fsm_eAcpt_sGmpMap_ssMapCndt_moUse(Game *sota, Menu *in_mc) {
    /* Item target was chosen, use it */

    /* - Set patient/target to candidate - */
    SDL_assert(sota->targets.candidates != NULL);
    SDL_assert(sota->targets.candidates[sota->targets.order] > TNECS_NULL);
    tnecs_E patient_E = sota->targets.candidates[sota->targets.order];
    sota->targets.previous_order = sota->targets.order;
    SDL_assert(sota->combat.defendant > TNECS_NULL);
    SDL_assert(sota->combat.aggressor > TNECS_NULL);

    Unit *patient = IES_GET_C(gl_world, patient_E, Unit);
    /* - User is selected unit - */
    Unit *user = IES_GET_C(gl_world, sota->selected.unit_entity, Unit);

    /* - User is selected unit - */
    Inventory_item *invitem = IES_GET_C(gl_world, sota->selected.item, Inventory_item);
    const Item *item = Item_Get(invitem);

    /* - Using item on patient - */
    Item_Use(item, user, &patient, 1);
}

void fsm_eCncl_sGmpMap_ssMapCndt_moAtk(Game *sota, Menu *in_mc) {
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
void fsm_eCrsMvs_sGmpMap_moAtk(Game *sota, Menu *mc) {
    /* Set defendant to current candidate */
    SDL_assert(sota->targets.candidates[sota->targets.order] != TNECS_NULL);
    sota->combat.defendant = sota->targets.candidates[sota->targets.order];
    SDL_assert(sota->combat.defendant != TNECS_NULL);
}

/* --- fsm_eCncl_sGmpMap_ssMenu_m --- */
void fsm_eCrsMvs_sGmpMap_ssMenu_mSSM(Game *sota, Menu *mc) {
    // TODO: update healmap when STAFF CHANGES
}

void fsm_eCrsMvs_sGmpMap_ssMenu_mLSM(Game *sota, Menu *mc) {
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *wsm = mc->data;
    wsm->update = true;

    /* - Get Popup_Loadout_Stats -- */
    tnecs_E popup_ent          = sota->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS];
    struct PopUp *popup             = IES_GET_C(gl_world, popup_ent, PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
    PopUp_Loadout_Stats_Selected_Stats(pls);

    /* - Compute new attackmap with equipped - */
    struct Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;
    Loadout *loadout = (Loadout *)&map_to;
    i32 eq_left     = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_LEFT);
    i32 eq_right    = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_RIGHT);

    if (eq_valid(eq_left))
        Loadout_Set(loadout, UNIT_HAND_LEFT,   eq_left);
    if (eq_valid(eq_right))
        Loadout_Set(loadout, UNIT_HAND_RIGHT,  eq_right);

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

    int rangemap = Unit_Rangemap_Get(unit);
    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL, wsm->_unit);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, wsm->_unit);
    }
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    PopUp_Loadout_Stats_Selected_Stats(pls);
}

void fsm_eCrsMvs_sGmpMap_ssMenu_mISM(   Game *sota, Menu *mc) {
    /* -- Do Nothing -- */
}

/* --- fsm_eCncl_sGmpMap_ssMenu_m --- */
void fsm_eCncl_sGmpMap_ssMenu_mTM(Game *sota, Menu *mc) {
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If item is selected, deselect item */
    b32 isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < SOTA_EQUIPMENT_SIZE);
    b32 isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    if (isItem || isTrader) {
        // TradeMenu_Deselect(tm);
        return;
    }

    /* - Hide TradeMenu - */
    b32 destroy = false;
    tnecs_E popped_E = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped_E == sota->menus.trade);

    /* - Go back to MapCandidates with traders - */

    /* - Turn staff_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    Game_Switch_toCandidates(
            sota, sota->targets.passives,
            "Trade was selected, time to select passive trade"
    );
    Game_Cursor_Move_toCandidate(sota);
}

void fsm_eCncl_sGmpMap_ssMenu_mSSM(Game *sota, Menu *mc) {
    // IF not staff skill
    // - Destroy staff_select menu
    SDL_assert(mc->type == MENU_TYPE_STAFF_SELECT);
    struct LoadoutSelectMenu *ssm = mc->data;
    struct Unit *unit = IES_GET_C(gl_world, ssm->_unit, Unit);

    int tophand = Unit_Hand_Strong(unit);
    // int bothand = Unit_Hand_Weak(unit);

    if (Loadout_isEquipped(&ssm->selected, tophand) && StaffSelectMenu_canEqItem(ssm)) {
        /* move cursor to first hand */
        int new_elem = LSM_ELEM_ITEM1;
        // tnecs_E cursor = sota->cursor.entity;
        Menu_Elem_Set(mc, sota, new_elem);

        /* Item selected in hand 0, swapping */
        LoadoutSelectMenu_Deselect(ssm);

        /* Switching to staves because menu was in items mode */
        StaffSelectMenu_Switch_Staves(ssm);

        SDL_assert(ssm->update == true);
    } else {
        /* move cursor to second hand */
        b32 destroy = false;
        tnecs_E popped_E = Game_menuStack_Pop(sota, destroy);
        SDL_assert(popped_E == sota->menus.staff_select);

        /* -- No item was selected, destroying ism menu -- */
        /* 2. Focus on */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Staff menu option  */
        tnecs_E menu = sota->menus.unit_action;
        Menu *mc_ua = IES_GET_C(gl_world, menu, Menu);
        UnitActionMenu *uam = mc_ua->data;
        int new_elem = ActionMenu_Option_Index(uam, MENU_OPTION_STAFF);
        Menu_Elem_Set(mc_ua, sota, new_elem);
    }
    // TODO: revert to previous equipment
}

void fsm_eCncl_sGmpMap_ssMenu_mIAM(Game *sota, Menu *mc) {
    /* Popping IAM */
    ItemActionMenu *iam = mc->data;

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped == sota->menus.item_action);

    /* TODO: out of scope of menu fsm */
    Game_cursorFocus_onMenu(sota);
}

void fsm_eCncl_sGmpMap_ssMenu_mMAM(Game *sota, Menu *mc) {
    /* Popping MAM, going back to map */

    MapActionMenu *mam = mc->data;
    Menu *Emc = IES_GET_C(gl_world, sota->menus.map_action, Menu);
    SDL_assert(Emc == mc);
    pActionMenu_Check_Texture(mam->platform);

    Game_subState_Set(  sota, GAME_SUBSTATE_STANDBY,
                        "Stops showing MAM");

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped == sota->menus.map_action);

    Game_cursorFocus_onMap(sota);
}

void fsm_eCncl_sGmpMap_ssMenu_mUAM(Game *sota, Menu *mc) {
    /* Popping UAM, going back to unit movement */

    UnitActionMenu *uam = mc->data;
    mc->visible = false;
    /* "Unit action is taken after Map_unit moves only */
    tnecs_E      unit_ent   = sota->selected.unit_entity;
    Unit        *unit       = IES_GET_C(gl_world, unit_ent, Unit);
    Position    *unit_pos   = IES_GET_C(gl_world, unit_ent, Position);

    // 1. Moving entity back to original spot in map
    Map *map = Game_Map(sota);
    struct Point moved_pos = sota->selected.unit_moved_position;
    struct Point init_pos  = sota->selected.unit_initial_position;
    if ((init_pos.x != moved_pos.x) || (init_pos.y != moved_pos.y))
        Map_Unit_Move(map, moved_pos.x, moved_pos.y, init_pos.x, init_pos.y);

    // 2. Moving pos ptr to initial position to compute initial attacktomap
    // 2.1 inital pos != moved pos, so cursor would move...
    Position_Pos_Set(unit_pos, init_pos.x, init_pos.y);
    // SDL_Log("init_pos %d %d", init_pos.x, init_pos.y);

    map->flags.update = true;

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    map_to.move         = true;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = sota->selected.unit_entity;

    /* - healtopmap - */
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    // printf("movemap\n");
    // matrix_print(map->darrs.movemap, Map_row_len(map), Map_col_len(map));
    // printf("attacktomap\n");
    // matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map));

    // 2.2 BUT: Moving pos ptr to selected position so that cursor doesn't move
    // Position_Pos_Set(unit_pos, init_pos.x, init_pos.y);
    // tnecs_E cursor = sota->cursor.entity;
    // struct Position *cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);

    unit_pos->tilemap_pos.x = moved_pos.x;
    unit_pos->tilemap_pos.y = moved_pos.y;
    unit_pos->pixel_pos.x   = unit_pos->tilemap_pos.x * unit_pos->scale[0];
    unit_pos->pixel_pos.y   = unit_pos->tilemap_pos.y * unit_pos->scale[1];

    // 3. Compute new stackmap with recomputed attacktomap
    int rangemap = Unit_Rangemap_Get(unit);

    /* - Compute new stackmap with recomputed attacktomap - */
    int overlays = MAP_OVERLAY_MOVE + MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER;
    if (rangemap        == RANGEMAP_HEALMAP) {
        overlays += MAP_OVERLAY_HEAL;
        Map_Palettemap_Autoset(map, overlays, unit_ent);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        overlays += MAP_OVERLAY_ATTACK;
        Map_Palettemap_Autoset(map, overlays, TNECS_NULL);
    }

    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    // 4. Revert Unit animation state to move
    struct Sprite *sprite;
    sprite = IES_GET_C(gl_world, sota->selected.unit_entity, Sprite);
    // TODO: REMOVE IF WHEN ALL MAP_UNITS HAVE SPRITESHEETS.
    if ((sprite->spritesheet != NULL) && (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM)) {
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVER, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->render.er);
    }

    SDL_assert(Map_isUpdate(map));

    Game_subState_Set(  sota, GAME_SUBSTATE_MAP_UNIT_MOVES,
                        "Stops showing UAM");

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped == sota->menus.unit_action);

    Game_cursorFocus_onMap(sota);
}

void fsm_eCncl_sGmpMap_ssMenu_mLSM(Game *sota, Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);
    struct LoadoutSelectMenu *wsm = mc->data;
    struct Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);

    int stronghand  = Unit_Hand_Strong(unit);
    int weakhand    = Unit_Hand_Weak(unit);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
    struct PopUp_Loadout_Stats *pls = (struct PopUp_Loadout_Stats *)popup->data;

    if (Loadout_isEquipped(&wsm->selected, stronghand)) {
        /* move cursor to first hand */
        int new_elem = LSM_ELEM_ITEM1;
        // tnecs_E cursor = sota->cursor.entity;
        Menu_Elem_Set(mc, sota, new_elem);

        canEquip can_equip  = canEquip_default;
        canEquip_Loadout(&can_equip, UNIT_HAND_LEFT, ITEM1);
        canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, ITEM2);
        can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
        Unit_canEquip_Equipment(unit, can_equip);
        LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
        LoadoutSelectMenu_Elem_Reset(wsm, mc);
        LoadoutSelectMenu_Elem_Pos(wsm, mc);
        Menu_Elem_Boxes_Check(mc);

        wsm->update = true;
    } else {
        /* -- No item was selected, destroying wsm menu -- */
        b32 destroy = false;
        tnecs_E popped_ent = Game_menuStack_Pop(sota, destroy);
        SDL_assert(popped_ent > 0);

        /* 2. Focus on */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Attack menu option  */
        Menu *mc     = IES_GET_C(gl_world, sota->menus.unit_action, Menu);
        UnitActionMenu *uam    = mc->data;
        int new_elem = ActionMenu_Option_Index(uam, MENU_OPTION_ATTACK);
        Menu_Elem_Set(mc, sota, new_elem);

        /* 4. Hide loadout stats Popup */
        Game_PopUp_Loadout_Stats_Hide(sota);
        LoadoutSelectMenu_Select_Reset(wsm);

        /* 5. Reset previous candidate */
        sota->targets.previous_order = -1;
    }

    /* Item selected in hand 0, swapping */
    LoadoutSelectMenu_Deselect(wsm);

    /* Set previous_cs to new loadout */
    // i32 eq_left     = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_LEFT);
    // i32 eq_right    = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_RIGHT);

    if (weakhand == UNIT_HAND_LEFT) {
        Loadout_Set(&pls->loadout_selected,
                    UNIT_HAND_LEFT,
                    Loadout_Eq(&pls->loadout_initial, UNIT_HAND_LEFT));
    } else {
        Loadout_Set(&pls->loadout_selected,
                    UNIT_HAND_RIGHT,
                    Loadout_Eq(&pls->loadout_initial, UNIT_HAND_RIGHT));
    }

    PopUp_Loadout_Stats_Initial_Loadout(pls);
    PopUp_Loadout_Stats_Initial_Stats(pls);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(unit);

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = sota->combat.aggressor;

    /* - healtopmap - */
    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL, wsm->_unit);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
    }
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);

    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
    PopUp_Loadout_Stats_Selected_Stats(pls);
}

void fsm_eCncl_sGmpMap_ssMenu_mISM(Game *sota, Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_ITEM_SELECT);

    /* -- Popping ism -- */
    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* Focus on unit action menu */
    Game_cursorFocus_onMenu(sota);

    /* Hide loadout stats Popup */
    Game_PopUp_Loadout_Stats_Hide(sota);

    /* -- Hide ItemSelectMenu  -- */
    mc->visible = false;
}

void fsm_eCncl_sGmpMap_ssMenu_mSM(Game *sota, Menu *mc) {
    /* -- Destroy stats menu and go back to standby -- */
    b32 destroy = false;
    tnecs_E popped_E = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped_E > 0);

    i8 new_substate = GAME_SUBSTATE_STANDBY;
    Game_subState_Set(
            sota, new_substate,
            "Stop showing stats menu during gameplay"
    );

    /* - Make popup_tile visible - */
    tnecs_E popup_ent = sota->popups.arr[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup    = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = true;

    /* - Make popup_unit invisible - */
    popup_ent   = sota->popups.arr[POPUP_TYPE_HUD_UNIT];
    popup       = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL) {
        popup->visible = true;
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mIAM(Game *sota, Menu *mc) {
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota) == GAME_SUBSTATE_MENU);

    ItemActionMenu *iam = mc->data;
    Menu *Emc = IES_GET_C(gl_world, sota->menus.item_action, Menu);
    SDL_assert(Emc == mc);

    pActionMenu_Check_Texture(iam->platform);

    i32 option_num = ItemActionMenu_Options_Num(iam);
    SDL_assert(option_num == mc->elem_num);
    SDL_assert(mc->elem < option_num);

    i32 menu_option = iam->options[mc->elem].id;
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    sota->selected.menu_option = menu_option;
    i32 order = ItemActionMenu_Option_Order(iam, menu_option);

    if (fsm_eAcpt_sGmpMap_ssMenu_mIAM_mo[order] != NULL) {
        fsm_eAcpt_sGmpMap_ssMenu_mIAM_mo[order](sota, mc);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moEquip(Game *s, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moUse(Game *IES, Menu *mc) {
    /* --- Action with item: Use it --- */

    /* -- Getting the item -- */
    Menu *ismC = IES_GET_C(gl_world, IES->menus.item_select, Menu);
    SDL_assert(mc->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = ismC->data;

    const Unit *unit = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);

    Inventory_item *invitem = Unit_InvItem(unit, ism->selected_eq);
    ;
    const Item *item = Item_Get(invitem);

    /* -- 1. Item target is TARGET_SELF -- */
    if (item->ids.target == TARGET_SELF) {
        /* -- Directly using item on self -- */
        /* - Set patient/target to candidate - */
        /* - User is selected unit - */
        Unit *user = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);

        /* - Using item on patient - */
        Item_Use(item, user, &user, 1);
    }

    /* -- 2. Item target is NOT TARGET_SELF */
    /* -- Find potential targets -- */
    // NOTE: need target type + RANGE

    /* -- Change to MapCandidates state -- */

    /* - Turn IAM invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    Game_Switch_toCandidates(
            IES, IES->targets.passives,
            "Using item on targets"
    );
    Game_Cursor_Move_toCandidate(IES);
}

void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moDrop(Game *s, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mIAM_moTrade(Game *s, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mMAM( Game *sota, Menu *mc) {
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota) == GAME_SUBSTATE_MENU);

    MapActionMenu *mam = mc->data;
    Menu *Emc = IES_GET_C(gl_world, sota->menus.map_action, Menu);
    SDL_assert(Emc == mc);

    pActionMenu_Check_Texture(mam->platform);

    i32 option_num = MapActionMenu_Options_Num(mam);
    SDL_assert(option_num == mc->elem_num);
    SDL_assert(mc->elem < option_num);

    i32 menu_option = mam->options[mc->elem].id;
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    sota->selected.menu_option = menu_option;
    i32 order = MapActionMenu_Option_Order(mam, menu_option);

    if (fsm_eAcpt_sGmpMap_ssMenu_mMAM_mo[order] != NULL) {
        fsm_eAcpt_sGmpMap_ssMenu_mMAM_mo[order](sota, mc);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moUnit(Game *sota, Menu *mc) {

}
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moCnvy(Game *sota, Menu *mc) {

}
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moStts(Game *sota, Menu *mc) {

}
void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moQuit(Game *sota, Menu *mc) {
    Event_Emit(__func__, SDL_USEREVENT, event_Quit, data1_entity, data2_entity);
}

void fsm_eAcpt_sGmpMap_ssMenu_mMAM_moEndT(Game *sota, Menu *mc) {
    // int armies.current = Map_Army_Next(map);
    // Todo start transition animation
    // Todo switch control to next army

    Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, data1_entity, data2_entity);
}

/* --- fsm_eAcpt_sGmpMap_ssMenu_mUAM --- */
void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTrade(Game *sota, Menu *mc) {

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

void fsm_eAcpt_sGmpMap_ssMenu_mDM(Game *sota, Menu *mc) {
    /* Select or deselect unit */
    struct DeploymentMenu *dm = mc->data;

    i32 unit_order = DeploymentMenu_Select(dm, mc->elem);
    /* Skip if no empty tile to put unit on */
    if (unit_order < 0)
        return;
    i32 dm_order = dm->_selected[unit_order];
    i16 unit_id = dm->party->id_stack[unit_order];

    SDL_assert(Unit_ID_Valid(unit_id));
    // TODO: get start_pos_order from dm_order with list in map

    // i32 start_pos_i = DeploymentMenu_Map_StartPos(dm, dm_order);
    Map *map = Game_Map(sota);
    struct Point pos = map->start_pos.arr[dm_order];

    tnecs_E unit_ent = sota->party.Es[unit_id];
    SDL_assert(unit_ent > TNECS_NULL);

    /* Add or remove unit from map */
    Game_Unit_Refresh(sota, unit_ent);

    if (dm->_selected[unit_order] >= 0) {
        Map_Unit_Put(map, pos.x, pos.y, unit_ent);
    } else {
        Map_Unit_Remove(map, unit_ent);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mTM(Game *sota, Menu *mc) {
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If no item is selected, select item */
    // b32 isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < SOTA_EQUIPMENT_SIZE);
    // b32 isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    // if (!isItem || !isTrader) {
    //     TradeMenu_Select(tm);
    //     //     return;
    // }

    // TradeMenu_Select(tm, mc->elem);
}

// void fsm_eAcpt_sGmpMap_ssMenu_mPCP(Game *sota, Menu *mc) {
/* Start Combat */
// Necessary criteria:
//  - sota->combat.aggressor
//  - sota->combat.defendant
// Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
// }

void fsm_eAcpt_sGmpMap_ssMenu_mSM(Game *sota, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mISM( Game *sota,
                                    Menu *mc_ism) {
    /* Player selected Item, prepare to give choice
    **  about what to do with it.*/

    /* -- ISM selects menu elem -- */
    SDL_assert(mc_ism->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ism->data;
    sota->selected.item = ItemSelectMenu_Select(ism, mc_ism->elem);

    /* -- Enable ItemActionMenu -- */
    SDL_assert(sota->selected.unit_entity   > TNECS_NULL);
    SDL_assert(mc_ism->visible);
    Game_ItemActionMenu_Enable(sota, sota->selected.unit_entity);

    /* - E was created, C might have moved. - */
    mc_ism = IES_GET_C( gl_world,
                        sota->menus.item_select,
                        Menu);

    SDL_assert(sota->menus.item_action);

    Menu *mc_iam = IES_GET_C(   gl_world,
                                sota->menus.item_action,
                                Menu);
    Menu_Elem_Set(mc_iam, sota, 0);

    /* -- Focus on menu -- */
    Game_cursorFocus_onMenu(sota);

    /* Game_cursorFocus_onMenu disables other menus.
    **  Normally fine, but we want ISM visible,
    **  to show player selected item.  */
    mc_ism->visible = true;
}

void fsm_eAcpt_sGmpMap_ssMenu_mLSM( Game *sota,
                                    Menu *mc) {
    /* Swap weapons */
    LoadoutSelectMenu *wsm = mc->data;
    Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);
    SDL_assert(mc->elem >= ITEM_NULL);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
    popup->visible = true;

    LoadoutSelectMenu_Select(wsm, mc->elem);

    /* - Compute new attackmap with equipped - */

    /* - MapAct settings for attacktolist - */
    Map *map = Game_Map(sota);
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = sota->combat.aggressor;

    /* - healtopmap - */
    Map_Act_To(map, map_to);

    /* - attacktomap - */
    map_to.archetype     = ITEM_ARCHETYPE_WEAPON;
    Map_Act_To(map, map_to);

    int rangemap = Unit_Rangemap_Get(unit);
    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL, wsm->_unit);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK, TNECS_NULL);
    }
    Map_Stacked_Dangermap_Compute(map, map->darrs.dangermap);


    /* - Find usable equipment for weakhand - */
    if (Loadout_isEquipped(&wsm->selected, stronghand)) {
        canEquip can_equip  = canEquip_default;
        int eq = Loadout_Eq(&wsm->selected, stronghand);
        canEquip_Loadout(&can_equip, stronghand, eq);
        can_equip.archetype = ITEM_ARCHETYPE_WEAKHAND;
        can_equip.hand      = weakhand;
        Unit_canEquip_Equipment(unit, can_equip);
        // TODO: option to equip nothing in weakhand
    }

    /* - Decide what to do with remaining equippables - */
    if (WeaponSelectMenu_Usable_Remains(wsm)) {
        SDL_assert(mc->n9patch.scale.x > 0);
        SDL_assert(mc->n9patch.scale.y > 0);

        int popup_ind       = POPUP_TYPE_HUD_LOADOUT_STATS;
        struct PopUp *popup = IES_GET_C(gl_world, sota->popups.arr[popup_ind], PopUp);
        struct PopUp_Loadout_Stats *pls = popup->data;

        LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
        LoadoutSelectMenu_Elem_Reset(wsm, mc);
        LoadoutSelectMenu_Elem_Pos(wsm, mc);
        Menu_Elem_Boxes_Check(mc);

        /* Use wsm loadout to compute "previous" loadout */
        PopUp_Loadout_Stats_Select(pls, wsm);
        PopUp_Loadout_Stats_Initial_Loadout(pls);
        PopUp_Loadout_Stats_Initial_Stats(pls);

        /* move cursor to top hand */
        Menu_Elem_Set(mc, sota, 0);

        // Use wsm loadout to compute loadout after changing
        // to new usable items
        PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
        PopUp_Loadout_Stats_Selected_Stats(pls);
    } else {
        /* Loadout selected, find new defendants*/
        Game_postLoadout_Defendants(sota, sota->combat.aggressor);

        /* - A defendant SHOULD be in range of current loadout - */
        SDL_assert(DARR_NUM(sota->targets.defendants) > 0);

        Event_Emit(__func__, SDL_USEREVENT, event_Loadout_Selected, data1_entity, data2_entity);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM(Game *sota, Menu *mc) {
    SDL_assert(Game_State_Current(sota) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota) == GAME_SUBSTATE_MENU);

    UnitActionMenu *uam = mc->data;

    i32 option_num = UnitActionMenu_Options_Num(uam);
    SDL_assert(option_num == mc->elem_num);
    SDL_assert(mc->elem < option_num);

    i32 menu_option = uam->options[mc->elem].id;
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    sota->selected.menu_option = menu_option;
    i32 option_order = UnitActionMenu_Option_Order(uam, menu_option);

    /* TODO: this should be on called on state fsm side */
    /* Menus don't care about menu options */
    if (fsm_eAcpt_sGmpMap_ssMenu_mUAM_mo[option_order] != NULL) {
        fsm_eAcpt_sGmpMap_ssMenu_mUAM_mo[option_order](sota, mc);
    }
}

void fsm_eAcpt_sTtlScrn_ssMenu_mFM(Game *sota, Menu *mc) {
    SDL_assert(Game_State_Current(sota)     == GAME_STATE_Title_Screen);
    SDL_assert(Game_Substate_Current(sota)  == GAME_SUBSTATE_MENU);

    FirstMenu *fm = mc->data;
    FirstMenu_Options_Num(fm);

    i32 option_num = FirstMenu_Options_Num(fm);
    SDL_assert(option_num == mc->elem_num);

    i32 menu_option = fm->options[mc->elem].id;
    /* TODO: check if menu_option is part of FM_OPTIONS */
    sota->selected.menu_option = menu_option;
    i32 option_order = FirstMenu_Option_Order(fm, menu_option);

    if (fsm_eAcpt_sTtlScrn_ssMenu_mFM_mo[option_order] != NULL)
        fsm_eAcpt_sTtlScrn_ssMenu_mFM_mo[option_order](sota, mc);
}

void fsm_eAcpt_sGmpMap_ssMenu_mSSM(Game *sota, Menu *mc) {
    SDL_assert(sota->targets.patients != sota->targets.defendants);
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(mc->elem >= ITEM_NULL);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);

    StaffSelectMenu_Select(ssm, mc->elem);

    Unit *unit      = IES_GET_C(gl_world, ssm->_unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);
    SDL_assert(ssm->_unit == sota->combat.aggressor);
    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

    /* - Switch to Map_Candidates substate - */
    SDL_assert(Game_State_Current(sota)    == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(sota) == GAME_SUBSTATE_MENU);

    if (StaffSelectMenu_canEqItem(ssm)) {
        /* Unit can use staff in one hand */
        /* move cursor to second hand */
        int new_elem            = LSM_ELEM_ITEM2;
        // tnecs_E cursor     = sota->cursor.entity;
        Menu_Elem_Set(mc, sota, new_elem);

        /* Switch to selecting items */
        StaffSelectMenu_Switch_Items(ssm);
    } else {
        /* - Check that a patient is in range of current loadout - */
        /* TODO: remove the check, ssm should only have staves with patients in range */

        if (DARR_NUM(sota->targets.patients) == 0) {
            SDL_assert(false);
            // LoadoutSelectMenu_Deselect(ssm);
        } else {
            Game_postLoadout_Patients(sota, sota->combat.aggressor);

            mc->visible = false;
            Game_Switch_toCandidates(
                    sota, sota->targets.patients,
                    "Staff was selected, time to select patient"
            );

            unit        = IES_GET_C(gl_world, sota->combat.aggressor, Unit);
            stronghand  = Unit_Hand_Strong(unit);
            weakhand    = Unit_Hand_Weak(unit);
            SDL_assert(Unit_isEquipped(unit, stronghand));
            SDL_assert(Unit_isEquipped(unit, weakhand));

            unit        = IES_GET_C(gl_world, ssm->_unit, Unit);
            stronghand  = Unit_Hand_Strong(unit);
            weakhand    = Unit_Hand_Weak(unit);

            SDL_assert(Unit_isEquipped(unit, stronghand));
            SDL_assert(Unit_isEquipped(unit, weakhand));

        }
    }
    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));
}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moWait(Game *sota, Menu *mc) {

    /* Pop all menus */
    b32 destroy = false;
    while (DARR_NUM(sota->menus.stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, data1_entity, data2_entity);

}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moOpen(Game *sota, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moRescue(Game *sota, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moTalk(Game *sota, Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moSeize(Game *sota, Menu *mc) {
    SDL_Log("Throne was seized: Map was won!");
    Map *map = Game_Map(sota);
    map->flags.win = true;

    /* - Go back to standby - */
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);
}

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moDance(Game *sota, Menu *mc) {
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

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moStaff(Game *sota, Menu *mc) {
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

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moAtk(Game *sota, Menu *mc_bad) {
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

void fsm_eAcpt_sGmpMap_ssMenu_mUAM_moItem(Game *sota, Menu *mc) {
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

void fsm_eAcpt_sTtlScrn_ssMenu_mFM_moDbgMap(Game *sota, Menu *mc) {
    SDL_assert((Game_State_Current(sota) == GAME_STATE_Title_Screen));

    Event_Emit( __func__, SDL_USEREVENT,
                event_Load_Debug_Map,
                data1_entity, data2_entity);
}

/* event_Input_Start */
void fsm_eStart_sPrep_ssMenu_mSM(Game *sota, Menu *mc) {
    /* --- Start battle? --- */

}

void fsm_eStart_sPrep_ssMenu_mDM(Game *sota, Menu *mc) {
    /* --- Start battle --- */
    Game_Battle_Start(sota, mc);
}

/* event_Input_Stats */
void fsm_eStats_sPrep_ssMenu_mSM( Game *sota, Menu *mc) {
    // Top menu is stats menu: do nothing
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(sota->menus.stack);
    tnecs_E top_menu   = sota->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack > 1);
    SDL_assert(top_menu == sota->menus.stats);
}

void fsm_eStats_sPrep_ssMenu_mDM( Game *sota, Menu *mc) {
    // Top menu is deployments menu: enable stats menu
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(sota->menus.stack);
    tnecs_E top_menu   = sota->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == sota->menus.deployment);


    /* Find which unit is hovered in deployment menu */
    SDL_assert(sota->cursor.entity);

    /* - Get unit hovered in deployment menu - */
    struct DeploymentMenu *dm = mc->data;
    tnecs_E hovered = DeploymentMenu_Hovered_Entity(dm, mc->elem);
    SDL_assert(hovered > TNECS_NULL);

    /* Enabling stats menu for hovered unit */
    Game_StatsMenu_Enable(sota, hovered);
}

void fsm_eStats_sPrep_ssMapCndt_mSM( Game *sota, Menu *mc) {
    // Top menu is stats menu: do nothing
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(sota->menus.stack);
    tnecs_E top_menu   = sota->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == sota->menus.stats);
}

void fsm_eCncl_sPrep_ssMenu_mSM( Game *sota, Menu *mc) {
    /* Top menu is stats menu: DISABLE IT */
    SDL_assert(mc != NULL);

    Game_menuStack_Pop(sota, false);

    /* - Focus on new menu - */
    if (DARR_NUM(sota->menus.stack) > 0) {
        /* Return to deployment menu */
        mc->visible = false;
        Game_cursorFocus_onMenu(sota);
    } else {
        /* TODO: move this to state FSM side */
        /* Return to Map candidates */
        Game_subState_Set(
                sota, GAME_SUBSTATE_MAP_CANDIDATES,
                "Change to map candidates"
        );

        /* - Reset potential candidates - */
        sota->targets.order     = 0;

        /* - Focus on map - */
        mc->visible = false;
        Game_cursorFocus_onMap(sota);
    }
}

void fsm_eCncl_sPrep_ssMenu_mDM( Game *sota, Menu *mc) {
    /* topmenu is deployment menu */
    /* Go back to map candidate selection */
    /* Candidates get switched around map starting positions */
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(sota->menus.stack);
    tnecs_E top_menu   = sota->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == sota->menus.deployment);

    SDL_assert(sota->menus.stack[0] != TNECS_NULL);
    SDL_assert(DARR_NUM(sota->menus.stack) == 1);
    DARR_POP(sota->menus.stack);
    SDL_assert(DARR_NUM(sota->menus.stack) == 0);

    Game_subState_Set(
            sota, GAME_SUBSTATE_MAP_CANDIDATES,
            "Change to map candidates"
    );

    /* - Reset potential candidates - */
    sota->targets.order     = 0;

    /* - Focus on map - */
    mc->visible = false;
    Game_cursorFocus_onMap(sota);
}

void fsm_eCncl_sPrep_ssMapCndt_mSM( Game *sota, Menu *mc) {
    // Top menu is stats menu: DISABLE IT
    SDL_assert(mc != NULL);

    int num_menu_stack      = DARR_NUM(sota->menus.stack);
    tnecs_E top_menu   = sota->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == sota->menus.stats);

    Game_menuStack_Pop(sota, false);

    /* - Focus on new menu - */
    mc->visible = true;
    Game_cursorFocus_onMenu(sota);
}
