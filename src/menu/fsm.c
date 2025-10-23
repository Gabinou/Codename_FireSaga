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
** menu_fsm: Finite State Machine for game.menu
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
#include "platform.h"
#include "utilities.h"

#include "bars/map_hp.h"

#include "game/map.h"
#include "game/unit.h"
#include "game/game.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "map/path.h"
#include "map/find.h"
#include "map/render.h"
#include "map/ontile.h"

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
    /* START */          NULL,
    /* PLAYER_SELECT */  NULL,
    /* WEAPON_SELECT */ &WeaponSelectMenu_Elem_Move,
    /* STAFF_SELECT */  &Menu_Elem_Move,
    /* ITEM_SELECT */   &ItemSelectMenu_Elem_Move,
    /* STATS */         &Menu_Elem_Move,
    /* RESCUE */        &Menu_Elem_Move,
    /* SUPPORTS */      &Menu_Elem_Move,
    /* GROWTHS */       &Menu_Elem_Move,
    /* TRADE */         &Menu_Elem_Move,
    /* ITEM_DROP */     &Menu_Elem_Move,
    /* DEPLOYMENT */    &DeploymentMenu_Elem_Move,
    /* FIRST */         &FirstMenu_Elem_Move,
    /* UNIT_ACTION */   &UnitActionMenu_Elem_Move,
    /* ITEM_ACTION */   &MapActionMenu_Elem_Move,
    /* MAP_ACTION */    &MapActionMenu_Elem_Move,
    /* WHICH_HAND */    &ActionMenu_Elem_Move,
};

/* - Menu substate FSMs - */
const fsm_menu_t fsm_eStats_m[MENU_TYPE_END] = {
    /* START */          NULL,
    /* PLAYER_SELECT */  NULL,
    /* WEAPON_SELECT */  NULL,
    /* STAFF_SELECT */   NULL,
    /* ITEM_SELECT */    NULL,
    /* STATS */         &fsm_eStats_mSM,
    /* RESCUE */         NULL,
    /* SUPPORTS */       NULL,
    /* GROWTHS */        NULL,
    /* TRADE */          NULL,
    /* ITEM_DROP */      NULL,
    /* DEPLOYMENT */    &fsm_eStats_mDM,
    /* FIRST */          NULL,
};

const fsm_menu_t fsm_eAcpt_m[MENU_TYPE_END] = {
    /* START */          NULL,
    /* PLAYER_SELECT */  NULL,
    /* WEAPON_SELECT */ &fsm_eAcpt_mLSM,
    /* STAFF_SELECT */  &fsm_eAcpt_mSSM,
    /* ITEM_SELECT  */  &fsm_eAcpt_mISM,
    /* STATS */         &fsm_eAcpt_mSM,
    /* RESCUE */         NULL,
    /* SUPPORTS */       NULL,
    /* GROWTHS */        NULL,
    /* TRADE */         &fsm_eAcpt_mTM,
    /* ITEM_DROP */      NULL,
    /* DEPLOYMENT */    &fsm_eAcpt_mDM,
    /* FIRST */         &fsm_eAcpt_mFM,
    /* UNIT_ACTION */   &fsm_eAcpt_mUAM,
    /* ITEM_ACTION */   &fsm_eAcpt_mIAM,
    /* MAP_ACTION */    &fsm_eAcpt_mMAM,
    /* WHICH_HAND */    &fsm_eAcpt_mWHM
};

const fsm_menu_t fsm_eCncl_m[MENU_TYPE_END] = {
    /* START */          NULL,
    /* PLAYER_SELECT */  NULL,
    /* WEAPON_SELECT */ &fsm_eCncl_mLSM,
    /* STAFF_SELECT  */ &fsm_eCncl_mSSM,
    /* ITEM_SELECT  */  &fsm_eCncl_mISM,
    /* STATS */         &fsm_eCncl_mSM,
    /* RESCUE */         NULL,
    /* SUPPORTS */       NULL,
    /* GROWTHS */       &fsm_eCncl_mSM,
    /* TRADE */          NULL,
    /* ITEM_DROP */      NULL,
    /* DEPLOYMENT */     NULL,
    /* FIRST */          NULL,
    /* UNIT_ACTION */   &fsm_eCncl_mUAM,
    /* ITEM_ACTION */   &fsm_eCncl_mIAM,
    /* MAP_ACTION */    &fsm_eCncl_mMAM,
    /* WHICH_HAND */    &fsm_eCncl_mWHM
};

const fsm_menu_t fsm_eCrsMvs_m[MENU_TYPE_END] = {
    /* START */          NULL,
    /* PLAYER_SELECT */  NULL,
    /* WEAPON_SELECT */ &fsm_eCrsMvs_mLSM,
    /* STAFF_SELECT  */ &fsm_eCrsMvs_mSSM,
    /* ITEM_SELECT  */  &fsm_eCrsMvs_mISM,
    /* STATS */          NULL,
    /* RESCUE */         NULL,
    /* SUPPORTS */       NULL,
    /* GROWTHS */        NULL,
    /* TRADE */          NULL,
    /* ITEM_DROP */      NULL,
    /* DEPLOYMENT */     NULL,
    /* FIRST */          NULL,
};

/* --- fsm_eCncl_m --- */
void fsm_eCrsMvs_mSSM(   Game *IES,
                         Menu *mc) {
    // TODO: update healmap when STAFF CHANGES
}

void fsm_eCrsMvs_mLSM(   Game *IES,
                         Menu *mc) {
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *wsm = mc->data;
    wsm->update = true;

    /* - Get Popup_Loadout_Stats -- */
    tnecs_E popup_ent          = IES->popups.arr[POPUP_TYPE_HUD_LOADOUT_STATS];
    struct PopUp *popup             = IES_GET_C(gl_world, popup_ent, PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
    PopUp_Loadout_Stats_Selected_Stats(pls);

    /* - Compute new attackmap with equipped - */
    Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);

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
    map_to.aggressor    = IES->combat.aggressor;

    /* - healtopmap - */
    Map *map = Game_Map(IES);
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

void fsm_eCrsMvs_mISM(   Game *IES,
                         Menu *mc) {
    /* -- Do Nothing -- */
}

/* --- fsm_eCncl_m --- */
void fsm_eCncl_mTM(  Game *IES,
                     Menu *mc) {
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
    tnecs_E popped_E = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped_E == IES->menus.trade);

    /* - Go back to MapCandidates with traders - */

    /* - Turn staff_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    Game_Switch_toCandidates(
            IES, IES->targets.passives,
            "Trade was selected, time to select passive trade"
    );
    Game_Cursor_Move_toCandidate(IES);
}

void fsm_eCncl_mSSM( Game *IES,
                     Menu *mc) {
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
        // tnecs_E cursor = IES->cursor.entity;
        Menu_Elem_Set(mc, IES, new_elem);

        /* Item selected in hand 0, swapping */
        LoadoutSelectMenu_Deselect(ssm);

        /* Switching to staves because menu was in items mode */
        StaffSelectMenu_Switch_Staves(ssm);

        SDL_assert(ssm->update == true);
    } else {
        /* move cursor to second hand */
        b32 destroy = false;
        tnecs_E popped_E = Game_menuStack_Pop(IES, destroy);
        SDL_assert(popped_E == IES->menus.staff_select);

        /* -- No item was selected, destroying ism menu -- */
        /* 2. Focus on */
        Game_cursorFocus_onMenu(IES);

        /* 3. Move cursor to Staff menu option  */
        tnecs_E menu = IES->menus.unit_action;
        Menu *mc_ua = IES_GET_C(gl_world, menu, Menu);
        UnitActionMenu *uam = mc_ua->data;
        int new_elem = ActionMenu_Option_Index(uam, MENU_OPTION_STAFF);
        Menu_Elem_Set(mc_ua, IES, new_elem);
    }
    // TODO: revert to previous equipment
}

void fsm_eCncl_mWHM(Game *IES, Menu *mc) {
    /* --- Cancel hand selection ---
    ** - Always go back to previous menu on stack */

    /* -- 1. Popping WHM -- */
    WhichHandMenu *whm = mc->data;

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped == IES->menus.which_hand);

    /* TODO: out of scope of menu fsm */
    Game_cursorFocus_onMenu(IES);

    /* -- 2. Make grandparent of WHM visible too -- */
    int stack_top = DARR_NUM(IES->menus.stack) - 1;
    if (stack_top >= 1) {
        tnecs_E grand_E     = IES->menus.stack[stack_top - 1];
        Menu *mc_grand      = IES_GET_C(gl_world, grand_E, Menu);
        mc_grand->visible   = true;
    }
}

void fsm_eCncl_mIAM( Game *IES,
                     Menu *mc) {
    /* Popping IAM */
    ItemActionMenu *iam = mc->data;

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped == IES->menus.item_action);

    /* TODO: out of scope of menu fsm */
    Game_cursorFocus_onMenu(IES);
}

void fsm_eCncl_mMAM( Game *IES,
                     Menu *mc) {
    /* Popping MAM, going back to map */

    MapActionMenu *mam = mc->data;
    Menu *Emc = IES_GET_C(gl_world, IES->menus.map_action, Menu);
    SDL_assert(Emc == mc);
    pActionMenu_Check_Texture(mam->platform);

    Game_subState_Set(  IES, GAME_SUBSTATE_STANDBY,
                        "Stops showing MAM");

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped == IES->menus.map_action);

    Game_cursorFocus_onMap(IES);
}

void fsm_eCncl_mUAM( Game *IES,
                     Menu *mc) {
    /* Popping UAM, going back to unit movement */

    UnitActionMenu *uam = mc->data;
    mc->visible = false;
    /* "Unit action is taken after Map_unit moves only */
    tnecs_E      unit_ent   = IES->selected.unit_entity;
    Unit        *unit       = IES_GET_C(gl_world, unit_ent, Unit);
    Position    *unit_pos   = IES_GET_C(gl_world, unit_ent, Position);

    // 1. Moving entity back to original spot in map
    Map *map = Game_Map(IES);
    struct Point moved_pos = IES->selected.unit_moved_position;
    struct Point init_pos  = IES->selected.unit_initial_position;
    if ((init_pos.x != moved_pos.x) || (init_pos.y != moved_pos.y))
        Map_Unit_Move(map,  moved_pos.x,    moved_pos.y,
                      init_pos.x,     init_pos.y);

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
    map_to.aggressor    = IES->selected.unit_entity;

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
    // tnecs_E cursor = IES->cursor.entity;
    // struct Position *cursor_pos = IES_GET_C(gl_world, IES->cursor.entity, Position);

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
    sprite = IES_GET_C(gl_world, IES->selected.unit_entity, Sprite);
    // TODO: REMOVE IF WHEN ALL MAP_UNITS HAVE SPRITESHEETS.
    if ((sprite->spritesheet != NULL) && (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM)) {
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVER, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, IES->render.er);
    }

    SDL_assert(Map_isUpdate(map));

    Game_subState_Set(  IES, GAME_SUBSTATE_MAP_UNIT_MOVES,
                        "Stops showing UAM");

    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped == IES->menus.unit_action);

    Game_cursorFocus_onMap(IES);
}

void fsm_eCncl_mLSM( Game *IES,
                     Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);
    struct LoadoutSelectMenu *wsm = mc->data;
    struct Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);

    int stronghand  = Unit_Hand_Strong(unit);
    int weakhand    = Unit_Hand_Weak(unit);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
    struct PopUp_Loadout_Stats *pls = (struct PopUp_Loadout_Stats *)popup->data;

    if (Loadout_isEquipped(&wsm->selected, stronghand)) {
        /* move cursor to first hand */
        int new_elem = LSM_ELEM_ITEM1;
        // tnecs_E cursor = IES->cursor.entity;
        Menu_Elem_Set(mc, IES, new_elem);

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
        tnecs_E popped_ent = Game_menuStack_Pop(IES, destroy);
        SDL_assert(popped_ent > 0);

        /* 2. Focus on */
        Game_cursorFocus_onMenu(IES);

        /* 3. Move cursor to Attack menu option  */
        Menu *mc     = IES_GET_C(gl_world, IES->menus.unit_action, Menu);
        UnitActionMenu *uam    = mc->data;
        int new_elem = ActionMenu_Option_Index(uam, MENU_OPTION_ATTACK);
        Menu_Elem_Set(mc, IES, new_elem);

        /* 4. Hide loadout stats Popup */
        Game_PopUp_Loadout_Stats_Hide(IES);
        LoadoutSelectMenu_Select_Reset(wsm);

        /* 5. Reset previous candidate */
        IES->targets.previous_order = -1;
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
    map_to.aggressor    = IES->combat.aggressor;

    /* - healtopmap - */
    Map *map = Game_Map(IES);
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

void fsm_eCncl_mISM( Game *IES,
                     Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_ITEM_SELECT);

    /* -- Popping ism -- */
    b32 destroy = false;
    tnecs_E popped = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped > 0);

    /* Focus on unit action menu */
    Game_cursorFocus_onMenu(IES);

    /* Hide loadout stats Popup */
    Game_PopUp_Loadout_Stats_Hide(IES);

    /* -- Hide ItemSelectMenu  -- */
    mc->visible = false;
}

void fsm_eCncl_mSM(Game *IES, Menu *mc) {
    /* -- Destroy stats menu and go back to standby -- */
    b32 destroy = false;
    tnecs_E popped_E = Game_menuStack_Pop(IES, destroy);
    SDL_assert(popped_E > 0);

    i8 new_substate = GAME_SUBSTATE_STANDBY;
    Game_subState_Set(
            IES, new_substate,
            "Stop showing stats menu during gameplay"
    );

    /* - Make popup_tile visible - */
    tnecs_E  popup_ent  = IES->popups.arr[POPUP_TYPE_HUD_TILE];
    PopUp   *popup      = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = true;

    /* - Make popup_unit invisible - */
    popup_ent   = IES->popups.arr[POPUP_TYPE_HUD_UNIT];
    popup       = IES_GET_C(gl_world, popup_ent, PopUp);
    if (popup != NULL) {
        popup->visible = true;
    }
}

void fsm_eAcpt_mWHM(Game *IES, Menu *mc) {
    /* --- Call WHM FSM for parent menu --- */

    /* -- Checking WHM -- */
    Menu *mc_WHM = IES_GET_C(gl_world, IES->menus.which_hand, Menu);
    IES_assert(mc_WHM->type == MENU_TYPE_WHICH_HAND);
    int num = DARR_NUM(IES->menus.stack);
    IES_assert(num > 1);
    IES_assert(IES->menus.which_hand == IES->menus.stack[num - 1]);

    /* -- Get Parent Menu C -- */
    tnecs_E parent_E = IES->menus.stack[num - 2];
    Menu *parent_mc = IES_GET_C(gl_world, parent_E, Menu);

    if (fsm_WHM_m[parent_mc->type] != NULL) {
        fsm_WHM_m[parent_mc->type](IES, parent_mc);
    }
}

void fsm_eAcpt_mIAM(Game *IES, Menu *mc_IAM) {
    /* --- Action to do with item was selected ---  */
    SDL_assert(Game_State_Current(IES) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(IES) == GAME_SUBSTATE_MENU);

    ItemActionMenu *iam = mc_IAM->data;
    Menu *Emc = IES_GET_C(gl_world, IES->menus.item_action, Menu);
    SDL_assert(Emc == mc_IAM);

    pActionMenu_Check_Texture(iam->am->platform);

    i32 option_num = ItemActionMenu_Options_Num(iam);
    SDL_assert(option_num == mc_IAM->elem_num);
    SDL_assert(mc_IAM->elem < option_num);

    i32 menu_option = ItemActionMenu_Option(iam, mc_IAM);
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    IES->selected.menu_option = menu_option;
    i32 order = _ItemActionMenu_Option_Order(iam, menu_option);

    /* -- Saving unit current loadout --  */
    Unit *unit = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);
    Unit_Loadout_Export(unit, &iam->previous_loadout);

    if (fsm_eAcpt_mIAM_mo[order] != NULL) {
        fsm_eAcpt_mIAM_mo[order](IES, mc_IAM);
    }
}


void fsm_eAcpt_mMAM(Game *IES, Menu *mc) {
    SDL_assert(Game_State_Current(IES) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(IES) == GAME_SUBSTATE_MENU);

    MapActionMenu *mam = mc->data;
    Menu *Emc = IES_GET_C(gl_world, IES->menus.map_action, Menu);
    SDL_assert(Emc == mc);

    pActionMenu_Check_Texture(mam->platform);

    i32 option_num = MapActionMenu_Options_Num(mam);
    SDL_assert(option_num == mc->elem_num);
    SDL_assert(mc->elem < option_num);

    i32 menu_option = mam->options[mc->elem].id;
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    IES->selected.menu_option = menu_option;
    i32 order = MapActionMenu_Option_Order(mam, menu_option);

    if (fsm_eAcpt_mMAM_mo[order] != NULL) {
        fsm_eAcpt_mMAM_mo[order](IES, mc);
    }
}

void fsm_eAcpt_mDM(Game *IES, Menu *mc) {
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
    Map *map = Game_Map(IES);
    struct Point pos = map->start_pos.arr[dm_order];

    tnecs_E unit_ent = IES->party.Es[unit_id];
    SDL_assert(unit_ent > TNECS_NULL);

    /* Add or remove unit from map */
    Game_Unit_Refresh(IES, unit_ent);

    if (dm->_selected[unit_order] >= 0) {
        Map_Unit_Put(map, pos.x, pos.y, unit_ent);
    } else {
        Map_Unit_Remove(map, unit_ent);
    }
}

void fsm_eAcpt_mTM(Game *IES, Menu *mc) {
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

// void fsm_eAcpt_mPCP(Game *IES, Menu *mc) {
/* Start Combat */
// Necessary criteria:
//  - IES->combat.aggressor
//  - IES->combat.defendant
// Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
// }

void fsm_eAcpt_mSM(Game *IES, Menu *mc) {

}

void fsm_eAcpt_mISM(Game *IES, Menu *mc_ism) {
    /* Player selected Item, prepare to give choice
    **  about what to do with it.*/
    SDL_assert(IES->selected.unit_entity   > TNECS_NULL);

    /* -- ISM selects menu elem -- */
    SDL_assert(mc_ism->type == MENU_TYPE_ITEM_SELECT);
    ItemSelectMenu *ism = mc_ism->data;
    IES->selected.item = ItemSelectMenu_Select(ism, mc_ism->elem);
    Inventory_item *invitem = IES_GET_C( gl_world,
                                         IES->selected.item,
                                         Inventory_item);

    /* -- Compute healtomap with item -- */
    Map *map = Game_Map(IES);

    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = Item_Archetype(invitem->id);
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = IES->selected.unit_entity;
    map_to.eq_type      = LOADOUT_EQ;
    map_to._eq          = ism->selected_eq;

    /* - healtopmap - */
    map->darrs.healtolist = Map_Act_To(map, map_to);

    // printf("healtomap\n");
    // matrix_print(   map->darrs.healtomap,
    //                 Map_row_len(map),
    //                 Map_col_len(map));

    /* -- Find patients with Item -- */
    MapFind mapfind = MapFind_default;

    mapfind.list        = map->darrs.healtolist;
    mapfind.found       = IES->targets.patients;
    mapfind.seeker      = IES->selected.unit_entity;
    mapfind.fastquit    = false;
    mapfind.eq_type     = LOADOUT_EQ;
    mapfind._eq         = ism->selected_eq;

    DARR_NUM(IES->targets.patients) = 0;
    IES->targets.patients = Map_Find_Patients(map, mapfind);

    /* -- Enable ItemActionMenu -- */
    SDL_assert(mc_ism->visible);
    Game_ItemActionMenu_Enable(IES, IES->selected.unit_entity);

    /* - E was created, C might have moved. - */
    mc_ism = IES_GET_C( gl_world,
                        IES->menus.item_select,
                        Menu);

    SDL_assert(IES->menus.item_action);

    Menu *mc_iam = IES_GET_C(   gl_world,
                                IES->menus.item_action,
                                Menu);
    Menu_Elem_Set(mc_iam, IES, 0);

    /* -- Focus on menu -- */
    Game_cursorFocus_onMenu(IES);

    /* Game_cursorFocus_onMenu disables other menus.
    **  Normally fine, but we want ISM visible,
    **  to show player selected item.  */
    mc_ism->visible = true;
    mc_iam->visible = true;
}

void fsm_eAcpt_mLSM(Game *IES, Menu *mc) {
    /* Swap weapons */
    LoadoutSelectMenu *wsm = mc->data;
    Unit *unit = IES_GET_C(gl_world, wsm->_unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);
    SDL_assert(mc->elem >= ITEM_NULL);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
    popup->visible = true;

    LoadoutSelectMenu_Select(wsm, mc->elem);

    /* - Compute new attackmap with equipped - */

    /* - MapAct settings for attacktolist - */
    Map *map = Game_Map(IES);
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = IES->combat.aggressor;

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
        struct PopUp *popup = IES_GET_C(gl_world, IES->popups.arr[popup_ind], PopUp);
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
        Menu_Elem_Set(mc, IES, 0);

        // Use wsm loadout to compute loadout after changing
        // to new usable items
        PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);
        PopUp_Loadout_Stats_Selected_Stats(pls);
    } else {
        /* Loadout selected, find new defendants*/
        Game_postLoadout_Defendants(IES, IES->combat.aggressor);

        /* - A defendant SHOULD be in range of current loadout - */
        SDL_assert(DARR_NUM(IES->targets.defendants) > 0);

        Event_Emit( __func__, SDL_USEREVENT,
                    event_Loadout_Selected,
                    NULL, NULL);
    }
}

void fsm_eAcpt_mUAM(Game *IES, Menu *mc) {
    SDL_assert(Game_State_Current(IES) == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(IES) == GAME_SUBSTATE_MENU);

    UnitActionMenu *uam = mc->data;

    i32 option_num = UnitActionMenu_Options_Num(uam);
    SDL_assert(option_num == mc->elem_num);
    SDL_assert(mc->elem < option_num);

    i32 menu_option = uam->options[mc->elem].id;
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    IES->selected.menu_option = menu_option;
    i32 option_order = UnitActionMenu_Option_Order(uam, menu_option);

    /* TODO: this should be on called on state fsm side */
    /* Menus don't care about menu options */
    if (fsm_eAcpt_mUAM_mo[option_order] != NULL) {
        fsm_eAcpt_mUAM_mo[option_order](IES, mc);
    }
}

void fsm_eAcpt_mFM(Game *IES, Menu *mc) {
    SDL_assert(Game_State_Current(IES)     == GAME_STATE_Title_Screen);
    SDL_assert(Game_Substate_Current(IES)  == GAME_SUBSTATE_MENU);

    FirstMenu *fm = mc->data;
    FirstMenu_Options_Num(fm);

    i32 option_num = FirstMenu_Options_Num(fm);
    SDL_assert(option_num == mc->elem_num);

    i32 menu_option = fm->options[mc->elem].id;
    /* TODO: check if menu_option is part of FM_OPTIONS */
    IES->selected.menu_option = menu_option;
    i32 option_order = FirstMenu_Option_Order(fm, menu_option);

    if (fsm_eAcpt_mFM_mo[option_order] != NULL)
        fsm_eAcpt_mFM_mo[option_order](IES, mc);
}

void fsm_eAcpt_mSSM(Game *IES, Menu *mc) {
    SDL_assert(IES->targets.patients != IES->targets.defendants);
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(mc->elem >= ITEM_NULL);
    SDL_assert(mc->elem < SOTA_EQUIPMENT_SIZE);

    StaffSelectMenu_Select(ssm, mc->elem);

    Unit *unit      = IES_GET_C(gl_world, ssm->_unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);
    SDL_assert(ssm->_unit == IES->combat.aggressor);
    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

    /* - Switch to Map_Candidates substate - */
    SDL_assert(Game_State_Current(IES)    == GAME_STATE_Gameplay_Map);
    SDL_assert(Game_Substate_Current(IES) == GAME_SUBSTATE_MENU);

    if (StaffSelectMenu_canEqItem(ssm)) {
        /* Unit can use staff in one hand */
        /* move cursor to second hand */
        int new_elem            = LSM_ELEM_ITEM2;
        // tnecs_E cursor     = IES->cursor.entity;
        Menu_Elem_Set(mc, IES, new_elem);

        /* Switch to selecting items */
        StaffSelectMenu_Switch_Items(ssm);
    } else {
        /* - Check that a patient is in range of current loadout - */
        /* TODO: remove the check, ssm should only have staves with patients in range */

        if (DARR_NUM(IES->targets.patients) == 0) {
            SDL_assert(false);
            // LoadoutSelectMenu_Deselect(ssm);
        } else {
            Game_postLoadout_Patients(IES, IES->combat.aggressor);

            mc->visible = false;
            Game_Switch_toCandidates(
                    IES, IES->targets.patients,
                    "Staff was selected, time to select patient"
            );

            unit        = IES_GET_C(gl_world, IES->combat.aggressor, Unit);
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

/* event_Input_Stats */
void fsm_eStats_mSM( Game *IES, Menu *mc) {
    /* Top menu is stats menu: do nothing */
    SDL_assert(mc != NULL);
    int num_menu_stack  = DARR_NUM(IES->menus.stack);
    tnecs_E top_menu    = IES->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack > 1);
    SDL_assert(top_menu == IES->menus.stats);
}

void fsm_eStats_mDM( Game *IES, Menu *mc) {
    /* Top menu is deployments menu: enable stats menu */
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(IES->menus.stack);
    tnecs_E top_menu   = IES->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == IES->menus.deployment);

    /* Find which unit is hovered in deployment menu */
    SDL_assert(IES->cursor.entity);

    /* - Get unit hovered in deployment menu - */
    struct DeploymentMenu *dm = mc->data;
    tnecs_E hovered = DeploymentMenu_Hovered_Entity(dm, mc->elem);
    SDL_assert(hovered > TNECS_NULL);

    /* Enabling stats menu for hovered unit */
    Game_StatsMenu_Enable(IES, hovered);
}

void fsm_eStats_sPrep_ssMapCndt_mSM( Game *IES, Menu *mc) {
    // Top menu is stats menu: do nothing
    SDL_assert(mc != NULL);
    int num_menu_stack      = DARR_NUM(IES->menus.stack);
    tnecs_E top_menu   = IES->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == IES->menus.stats);
}

void fsm_eCncl_sPrep_ssMapCndt_mSM( Game *IES, Menu *mc) {
    // Top menu is stats menu: DISABLE IT
    SDL_assert(mc != NULL);

    int num_menu_stack      = DARR_NUM(IES->menus.stack);
    tnecs_E top_menu   = IES->menus.stack[num_menu_stack - 1];

    SDL_assert(num_menu_stack == 1);
    SDL_assert(top_menu == IES->menus.stats);

    Game_menuStack_Pop(IES, false);

    /* - Focus on new menu - */
    mc->visible = true;
    Game_cursorFocus_onMenu(IES);
}
