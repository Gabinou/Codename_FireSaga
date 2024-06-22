
#include "menu/fsm.h"

/* --- Menu FSMs --- */
// NOTE: my menu naming convention is BAD
// - player_select_menu -> player is SELECTING
// - staff_select_menu -> staff is BEING SELECTED
// -> MAKE COHERENT

/* -- Menu-type FSMs -- */

/* - menu_elem_move - */
menu_elem_move_t menu_elem_move[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &PlayerSelectMenu_Elem_Move,
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
};

/* - Menu substate FSMs - */
fsm_menu_t fsm_eStart_sPrep_ssMenu_m[MENU_TYPE_END] = {
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
};

fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &fsm_eAcpt_sGmpMap_ssMenu_mPSM,
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
};

fsm_menu_t fsm_eCncl_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &fsm_eCncl_sGmpMap_ssMenu_mPSM,
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
};

fsm_menu_t fsm_eCrsMvs_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
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
};

/* - Menu option FSMs - */
fsm_menu_t fsm_eCrsMvs_sGmpMap_mo[MENU_OPTION_NUM] = {
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

fsm_menu_t fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moItem,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moStaff,
    /* MENU_OPTION_DANCE */         &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moSeize,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moAtk,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moTrade,
    /* MENU_OPTION_MAP */           NULL,
    /* MENU_OPTION_WAIT */          &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moWait,
    /* MENU_OPTION_OPEN */          NULL,
    /* MENU_OPTION_QUIT */          &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moQuit,
    /* MENU_OPTION_END_TURN */      &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moEndT,
    /* MENU_OPTION_UNITS */         NULL,
    /* MENU_OPTION_CONVOY */        NULL,
    /* MENU_OPTION_GLOBAL_RANGE */  NULL,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_LOAD */          NULL,
    /* MENU_OPTION_ERASE */         NULL,
    /* MENU_OPTION_COPY */          NULL,
    /* MENU_OPTION_SETTINGS */      NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     &fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDbgMap,
};

// Cancel depending on previous menu_option when selecting map candidates
fsm_menu_t fsm_eCncl_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM] = {
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
};

// Accept depending on previous menu_option when selecting map candidates
fsm_menu_t fsm_eAcpt_sGmpMap_ssMapCndt_mo[MENU_OPTION_NUM] = {
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
};

/* -- Menu Pop FSM -- */
fsm_menu_t fsm_Pop_sGmpMap_ssMenu_m[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &fsm_Pop_sGmpMap_ssMenu_mPSM,
    /* MENU_TYPE_WEAPON_SELECT */   NULL,
    /* MENU_TYPE_STAFF_SELECT  */   NULL,
    /* MENU_TYPE_ITEM_SELECT  */    NULL,
    /* MENU_TYPE_STATS */           NULL,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TRADE */           NULL,
    /* MENU_TYPE_ITEW_DROP */       NULL,
    /* MENU_TYPE_DEPLOYMENT */      NULL,
};

/* --- fsm_eAcpt_sGmpMap_ssMapCndt_mo --- */
void fsm_eAcpt_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *in_mc) {
    /* - Open Trade menu - */
    tnecs_entity active   = sota->selected_unit_entity;
    tnecs_entity passive  = sota->candidates[sota->candidate];
    SDL_assert(active       > TNECS_NULL);
    SDL_assert(passive      > TNECS_NULL);
    // Game_TradeMenu_Enable(sota, active, passive);

    strncpy(sota->reason, "time to trade", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    /* DESIGN QUESTION: should unit wait if weapon was traded? */
}

void fsm_eAcpt_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *in_mc) {

    /* - Refresh spectator - */
    SDL_assert(sota->candidates != NULL);
    SDL_assert(sota->candidates[sota->candidate] > TNECS_NULL);
    tnecs_entity spectator = sota->candidates[sota->candidate];
    SDL_assert(spectator > TNECS_NULL);
    Game_Unit_Refresh(sota, spectator);

    /* - Dancer waits - */
    tnecs_entity dancer = sota->selected_unit_entity;
    SDL_assert(dancer > TNECS_NULL);
    Game_Unit_Wait(sota, dancer);

    /* - hide movemap - */
    Map_Stacked_Dangermap_Reset(sota->map);
    Map_Palettemap_Reset(sota->map);

    /* -- Deselect unit and go back to map -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected_unit_entity);
    if (DARR_NUM(sota->menu_stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    sota->map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);
}

void fsm_eAcpt_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *in_mc) {

    tnecs_entity healer = sota->selected_unit_entity;
    /* - Healer uses staff on patient - */
    /* IF no one hand staff SKILL */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, healer, Unit);
    tnecs_entity patient_ent = sota->candidates[sota->candidate];
    struct Unit *patient = TNECS_GET_COMPONENT(sota->world, patient_ent, Unit);
    struct Inventory_item *staff_inv = Unit_InvItem(unit, Unit_Hand_Strong(unit));
    struct Weapon *staff = DTAB_GET(unit->weapons_dtab, staff_inv->id);
    SDL_assert(TNECS_TYPEFLAG_HAS_TYPE(staff->item->type, ITEM_TYPE_STAFF));
    SDL_assert(staff->item->active != NULL);
    staff->item->active(staff->item, unit, patient);

    /* TODO: IF one hand staff SKILL */

    /* - Update maphpbar - */
    struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, patient_ent, MapHPBar);
    SDL_assert(map_hp_bar           != NULL);
    SDL_assert(map_hp_bar->unit_ent == patient_ent);
    map_hp_bar->update =    true;
    map_hp_bar->visible =   true;

    /* - Healer waits - */
    SDL_assert(healer > TNECS_NULL);
    Game_Unit_Wait(sota, healer);

    /* - hide movemap - */
    Map_Stacked_Dangermap_Reset(sota->map);
    Map_Palettemap_Reset(sota->map);

    /* -- Deselect unit and go back to map -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, NULL, &sota->selected_unit_entity);
    if (DARR_NUM(sota->menu_stack) == 0)
        Game_cursorFocus_onMap(sota);

    /* - hide arrow - */
    sota->map->arrow->show = false;

    /* - Go back to standby - */
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);

}

void fsm_eAcpt_sGmpMap_ssMapCndt_moAtk(struct Game *sota, struct Menu *in_mc) {
    /* - Set Defendant to selected unit - */
    SDL_assert(sota->candidates != NULL);
    SDL_assert(sota->candidates[sota->candidate] > TNECS_NULL);
    sota->defendant = sota->candidates[sota->candidate];
    sota->previous_candidate = sota->candidate;
    SDL_assert(sota->defendant > TNECS_NULL);
    SDL_assert(sota->aggressor > TNECS_NULL);

    /* - Make cursor invisible - */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    sprite->visible = false;

    /* - Send Defendant_Select event - */
    Event_Emit(__func__, SDL_USEREVENT, event_Defendant_Select, data1_entity, data2_entity);

}

/* --- fsm_eCncl_sGmpMap_ssMapCndt_mo --- */
void fsm_eCncl_sGmpMap_ssMapCndt_moDance(struct Game *sota, struct Menu *in_mc) {

    /* 1. Turn menu_player_select_unit_action visible */
    tnecs_entity menu = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_PLAYER_SELECT);

    /* 2. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 3. Move cursor to Dance menu option */
    struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc->data;
    int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_DANCE);
    Menu_Elem_Set(mc, sota, new_elem);

}

void fsm_eCncl_sGmpMap_ssMapCndt_moStaff(struct Game *sota, struct Menu *in_mc) {

    /* 1. Turn top menu (Staff_Select_Menu) visible */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top = sota->menu_stack[stack_top];
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu_top, Menu);
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
    tnecs_entity cursor = sota->entity_cursor;
    Menu_Elem_Set(mc, sota, 0);

}

void fsm_eCncl_sGmpMap_ssMapCndt_moTrade(struct Game *sota, struct Menu *in_mc) {
    /* 1. Turn Player_Select_Menu visible */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top = sota->menu_stack[stack_top];
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_PLAYER_SELECT);

    /* 2. Update psm options */
    Game_preUnitAction_Targets(sota, sota->selected_unit_entity);
    Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_UNIT_ACTION);

    /* 3. Focus on menu */
    Game_cursorFocus_onMenu(sota);

}

void fsm_eCncl_sGmpMap_ssMapCndt_moAtk(struct Game *sota, struct Menu *in_mc) {

    /* 1. Turn Item_Select_Menu visible */
    int stack_top               = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity menu_top     = sota->menu_stack[stack_top];
    struct Menu *mc    = TNECS_GET_COMPONENT(sota->world, menu_top, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);

    /* 2. Cancel one of the selection of item select menu */
    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(wsm->selected[UNIT_HAND_LEFT]  > -1);
    SDL_assert(wsm->selected[UNIT_HAND_RIGHT] > -1);
    LoadoutSelectMenu_Deselect(wsm);
    SDL_assert((wsm->selected[UNIT_HAND_LEFT] != -1) || (wsm->selected[UNIT_HAND_RIGHT] != -1));

    /* 3. Set previous_cs to new loadout */
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;
    PopUp_Loadout_Stats_New(pls);

    /* 4. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 5. Move cursor to weak hand, strong hand on top */
    Menu_Elem_Set(mc, sota, 1);

    /* 6. HUD reappear */
    popup->visible = true;

    /* 7. Remove PopupPre_Combat */
    Game_PopUp_Pre_Combat_Hide(sota);
}

/* event_Cursor_Moves */
void fsm_eCrsMvs_sGmpMap_moAtk(struct Game *sota, struct Menu *mc) {
    /* Set defendant to current candidate */
    SDL_assert(sota->candidates[sota->candidate] != TNECS_NULL);
    sota->defendant = sota->candidates[sota->candidate];
    SDL_assert(sota->defendant != TNECS_NULL);
}

/* --- fsm_eCncl_sGmpMap_ssMenu_m --- */
void fsm_eCrsMvs_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc) {
    // TODO: update healmap when STAFF CHANGES
}

void fsm_eCrsMvs_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc) {
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *wsm = mc->data;
    wsm->update = true;

    /* - Get Popup_Loadout_Stats -- */
    tnecs_entity popup_ent        = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
    struct PopUp *popup             = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    // Map_Attacktomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
    //                                  pls->item_left, pls->item_right);
    // Map_Healtomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
    //                                pls->item_left, pls->item_right);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    PopUp_Loadout_Stats_New(pls);
}

void fsm_eCrsMvs_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc) {
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *ism = mc->data;

    /* - Get Popup_Loadout_Stats -- */
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
    SDL_assert(popup_ent > TNECS_NULL);
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Loadout_Stats *pls = (struct PopUp_Loadout_Stats *)popup->data;

    /* - Select left hand item first - */
    // Note: caller should have set mc->elem to new_elem
    if (ism->selected[UNIT_HAND_LEFT] > -1) {
        // left hand item was previously switched
        // -> is currently in the left hand
        pls->item_left = UNIT_HAND_LEFT;
        /* - Select right hand item second - */
        if (ism->selected[UNIT_HAND_RIGHT] > -1) {
            // right hand item was previously selected, and switched
            // -> is currently in the right hand
            pls->item_right = UNIT_HAND_RIGHT;
        } else {
            // right hand item was NOT previously switched (is not in hand)
            // -> new_elem selected, switching it to right hand
            pls->item_right = mc->elem;
        }
    } else {
        // left hand item was NOT previously switched (is not in hand)
        // -> new_elem selected, switching it to left hand
        pls->item_left = mc->elem;
    }
    PopUp_Loadout_Stats_New(pls);
}

/* --- fsm_eCncl_sGmpMap_ssMenu_m --- */
void fsm_eCncl_sGmpMap_ssMenu_mTM(struct Game *sota, struct Menu *mc) {
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If item is selected, deselect item */
    b32 isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < DEFAULT_EQUIPMENT_SIZE);
    b32 isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    if (isItem || isTrader) {
        // TradeMenu_Deselect(tm);
        return;
    }

    /* - Hide TradeMenu - */
    b32 destroy = false;
    tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity == sota->trade_menu);

    /* - Go back to MapCandidates with traders - */

    /* - Turn staff_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    strncpy(sota->reason, "trade was selected, time to select passive trade", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->passives);
    Game_Cursor_Move_toCandidate(sota);
}

void fsm_eCncl_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc) {
    // IF not staff skill
    // - Destroy staff_select menu
    SDL_assert(mc->type == MENU_TYPE_STAFF_SELECT);
    struct LoadoutSelectMenu *ssm = mc->data;
    int tophand = Unit_Hand_Strong(ssm->unit);
    int bothand = 1 - tophand;

    if ((ssm->selected[tophand] > -1) && (StaffSelectMenu_canEqItem(ssm))) {
        /* move cursor to first hand */
        int new_elem = LSM_ELEM_ITEM1;
        tnecs_entity cursor = sota->entity_cursor;
        Menu_Elem_Set(mc, sota, new_elem);

        /* Item selected in hand 0, swapping */
        LoadoutSelectMenu_Deselect(ssm);

        /* Switching to staves because menu was in items mode */
        StaffSelectMenu_Switch_Staves(ssm);

        SDL_assert(ssm->update == true);
    } else {
        /* move cursor to second hand */
        b32 destroy = false;
        tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
        SDL_assert(menu_popped_entity == sota->staff_select_menu);

        /* -- No item was selected, destroying ism menu -- */
        /* 2. Focus on psm */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Staff menu option on psm */
        tnecs_entity menu = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
        struct Menu *mc_ua = TNECS_GET_COMPONENT(sota->world, menu, Menu);
        struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc_ua->data;
        int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_STAFF);
        Menu_Elem_Set(mc_ua, sota, new_elem);

    }
}

// void fsm_eCncl_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc) {
//     struct Menu *menu;
//     menu = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, Menu);
//     struct LoadoutSelectMenu *wsm = menu->data;
//     SDL_assert(wsm->selected[UNIT_HAND_LEFT]    > -1);
//     SDL_assert(wsm->selected[UNIT_HAND_RIGHT]   > -1);

//     // 1. Destroy pre_combat menu -> Move to input cancel defendant select
//     SDL_assert(DARR_NUM(sota->menu_stack) > 0);
//     b32 destroy = false;
//     tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
//     SDL_assert(menu_popped_entity > 0);

//     // 2. Make cursor visible -> Remove
//     struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
//     sprite->visible = true;

//     // 3. Go back to selecting defendant -> Go back to loadout select instead
//     Event_Emit(__func__, SDL_USEREVENT, event_Loadout_Selected, data1_entity, data2_entity);

//     // 4. Attacktomap: recompute for aggressor
//     Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);
//     Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
//     Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

//     SDL_assert(wsm->selected[UNIT_HAND_LEFT]    > -1);
//     SDL_assert(wsm->selected[UNIT_HAND_RIGHT]   > -1);
// }

void fsm_eCncl_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc) {
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    b32 destroy = false;
    tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity > 0);
    struct Menu *mc_pop;
    mc_pop = TNECS_GET_COMPONENT(sota->world, menu_popped_entity, Menu);

    if (fsm_Pop_sGmpMap_ssMenu_m[mc_pop->type] != NULL)
        fsm_Pop_sGmpMap_ssMenu_m[mc_pop->type](sota, mc);

}

void fsm_eCncl_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);
    struct LoadoutSelectMenu *wsm = mc->data;

    int tophand = Unit_Hand_Strong(wsm->unit);
    int bothand = 1 - tophand;

    if (wsm->selected[tophand] > -1) {
        /* move cursor to first hand */
        int new_elem = LSM_ELEM_ITEM1;
        tnecs_entity cursor = sota->entity_cursor;
        Menu_Elem_Set(mc, sota, new_elem);

        Unit_Find_Usable(wsm->unit, ITEM_ARCHETYPE_WEAPON);

        /* Set previous_cs to new loadout */
        int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
        // struct PopUp_Loadout_Stats *pls = (struct PopUp_Loadout_Stats *)popup->data;
        // pls->item_left  = wsm->unit->eq_usable[mc->elem];
        // pls->item_right = wsm->unit->eq_usable[mc->elem];
        // PopUp_Loadout_Stats_Previous(pls);
        // pls->previous_cs = Unit_computedStats_wLoadout(wsm->unit, pls->item_left, pls->item_right);

        LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
        LoadoutSelectMenu_Elem_Reset(wsm, mc);
        LoadoutSelectMenu_Elem_Pos(wsm, mc);
        Menu_Elem_Boxes_Check(mc);

        wsm->update = true;
    } else {
        /* -- No item was selected, destroying wsm menu -- */
        b32 destroy = false;
        tnecs_entity popped_ent = Game_menuStack_Pop(sota, destroy);
        SDL_assert(popped_ent > 0);

        /* 2. Focus on psm */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Attack menu option on psm */
        tnecs_entity menu          = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
        struct Menu *mc     = TNECS_GET_COMPONENT(sota->world, menu, Menu);
        struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc->data;
        int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_ATTACK);
        Menu_Elem_Set(mc, sota, new_elem);

        /* 4. Hide loadout stats Popup */
        Game_PopUp_Loadout_Stats_Hide(sota);
        LoadoutSelectMenu_Select_Reset(wsm);

        /* 5. Reset previous candidate */
        sota->previous_candidate = -1;
    }

    /* Item selected in hand 0, swapping */
    LoadoutSelectMenu_Deselect(wsm);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    Map_Healtomap_Compute(  sota->map, sota->world, sota->aggressor, false, true);
    Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

}

void fsm_eCncl_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc) {
    SDL_assert(mc->type == MENU_TYPE_ITEM_SELECT);
    struct LoadoutSelectMenu *ism = mc->data;

    /* -- Popping ism -- */
    b32 destroy = false;
    tnecs_entity popped_ent = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped_ent > 0);

    /* Focus on unit action menu */
    Game_cursorFocus_onMenu(sota);

    /* Hide loadout stats Popup */
    Game_PopUp_Loadout_Stats_Hide(sota);
    ism->selected[UNIT_HAND_LEFT]   = -1;
    ism->selected[UNIT_HAND_RIGHT]  = -1;

}

void fsm_eCncl_sGmpMap_ssMenu_mSM(struct Game *sota, struct Menu *mc) {
    /* -- Destroy stats menu and go back to standby -- */
    b32 destroy = false;
    tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity > 0);

    i8 new_substate = GAME_SUBSTATE_STANDBY;
    strncpy(sota->reason, "Stops showing stats menu during gameplay", sizeof(sota->reason));
    Game_subState_Set(sota, new_substate, sota->reason);

    /* - Make popup_tile visible - */
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup    = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = true;

    /* - Make popup_unit invisible - */
    popup_ent   = sota->popups[POPUP_TYPE_HUD_UNIT];
    popup       = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    if (popup != NULL)
        popup->visible = true;


}

/* --- fsm_eAcpt_sGmpMap_ssMenu_m --- */
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moTrade(struct Game *sota, struct Menu *mc) {

    /* - Turn player_select_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    strncpy(sota->reason, "trade was selected, time to select passive trade", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->passives);

    // IF staff skill
    // - IF one item selected
    //  - Deselect item
    // - IF no item selected
    //  - Destroy staff_select menu
}

void fsm_eAcpt_sGmpMap_ssMenu_mDM(struct Game *sota, struct Menu *mc) {
    /* Select or deselect unit */
    struct DeploymentMenu *dm = mc->data;

    i32 unit_order = DeploymentMenu_Select(dm, mc->elem);
    /* Skip if no empty tile to put unit on */
    if (unit_order < 0)
        return;
    i32 dm_order = dm->_selected[unit_order];
    i16 unit_id = dm->_party_id_stack[unit_order];

    SDL_assert(Unit_ID_Valid(unit_id));
    // TODO: get start_pos_order from dm_order with list in map

    i32 start_pos_i = DeploymentMenu_Map_StartPos(dm, dm_order);
    struct Point pos = sota->map->start_pos[dm_order];

    if (sota->units_loaded[unit_id] <= TNECS_NULL)
        Game_Party_Entity_Create(sota, unit_id, pos);

    tnecs_entity unit_ent = sota->units_loaded[unit_id];
    SDL_assert(unit_ent > TNECS_NULL);

    /* Add or remove unit from map */
    SDL_assert(sota->map->world == sota->world);
    Game_Unit_Refresh(sota, unit_ent);

    if (dm->_selected[unit_order] >= 0) {
        Map_Unit_Put(sota->map, pos.x, pos.y, unit_ent);
    } else {
        Map_Unit_Remove(sota->map, unit_ent);
    }
}

void fsm_eAcpt_sGmpMap_ssMenu_mTM(struct Game *sota, struct Menu *mc) {
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If no item is selected, select item */
    // b32 isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < DEFAULT_EQUIPMENT_SIZE);
    // b32 isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    // if (!isItem || !isTrader) {
    //     TradeMenu_Select(tm);
    //     //     return;
    // }

    // TradeMenu_Select(tm, mc->elem);
}

// void fsm_eAcpt_sGmpMap_ssMenu_mPCP(struct Game *sota, struct Menu *mc) {
/* Start Combat */
// Necessary criteria:
//  - sota->aggressor
//  - sota->defendant
// Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
// }

void fsm_eAcpt_sGmpMap_ssMenu_mSM(struct Game *sota, struct Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mISM(struct Game *sota, struct Menu *mc) {

}

void fsm_eAcpt_sGmpMap_ssMenu_mLSM(struct Game *sota, struct Menu *mc) {
    /* Swap weapons */
    struct LoadoutSelectMenu *wsm = mc->data;
    i32 stronghand  = Unit_Hand_Strong(wsm->unit);
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);

    LoadoutSelectMenu_Select(wsm, mc->elem);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    Map_Healtomap_Compute(  sota->map, sota->world, sota->aggressor, false, true);
    Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    // TODO: Change to if other item in invetory
    if (WeaponSelectMenu_Usable_Remains(wsm)) {
        SDL_assert(mc->n9patch.scale.x > 0);
        SDL_assert(mc->n9patch.scale.y > 0);
        i32 stronghand  = Unit_Hand_Strong(wsm->unit);
        if (wsm->selected[stronghand] >= 0) {
            SDL_Log("ITEM_ARCHETYPE_WEAKHAND");
            Unit_Find_Usable(wsm->unit, ITEM_ARCHETYPE_WEAKHAND);
        }

        int popup_ind       = POPUP_TYPE_HUD_LOADOUT_STATS;
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
        struct PopUp_Loadout_Stats *pls = popup->data;

        LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
        LoadoutSelectMenu_Elem_Reset(wsm, mc);
        LoadoutSelectMenu_Elem_Pos(wsm, mc);
        Menu_Elem_Boxes_Check(mc);

        /* move cursor to top hand */
        Menu_Elem_Set(mc, sota, 0);

    } else {
        /* Loadout selected, find new defendants*/
        // TODO: use WeaponSelectMenu_Loadout_Valid/remove it
        Game_postLoadout_Defendants(sota, sota->aggressor);

        /* - Check that a defendant is in range of current loadout - */
        if (DARR_NUM(sota->defendants) == 0) {
            SDL_Log("Invalid Loadout: no defendant in range");
            LoadoutSelectMenu_Deselect(wsm);
        } else {
            Event_Emit(__func__, SDL_USEREVENT, event_Loadout_Selected, data1_entity, data2_entity);
        }
    }

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    popup->visible = true;
    struct PopUp_Loadout_Stats *pls = popup->data;

    PopUp_Loadout_Stats_Select(pls, wsm);
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc) {
    SDL_assert((sota->state == GAME_STATE_Gameplay_Map) || (sota->state == GAME_STATE_Title_Screen));
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);

    struct PlayerSelectMenu *psm_ptr = mc->data;
    SDL_assert(psm_ptr->option_num == mc->elem_num);
    sota->selected_menu_option = psm_ptr->options[mc->elem];
    if (fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo[sota->selected_menu_option] != NULL)
        fsm_eAcpt_sGmpMap_ssMenu_mPSM_mo[sota->selected_menu_option](sota, mc);
}

void fsm_eAcpt_sGmpMap_ssMenu_mSSM(struct Game *sota, struct Menu *mc) {
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);

    LoadoutSelectMenu_Select(ssm, mc->elem);

    /* - Switch to Map_Candidates substate - */
    SDL_assert(sota->state    == GAME_STATE_Gameplay_Map);
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);

    if (StaffSelectMenu_canEqItem(ssm)) {
        /* move cursor to second hand */
        int new_elem            = LSM_ELEM_ITEM2;
        tnecs_entity cursor   = sota->entity_cursor;
        Menu_Elem_Set(mc, sota, new_elem);
        StaffSelectMenu_Switch_Items(ssm);
    } else {
        /* - Check that a patient is in range of current loadout - */
        /* TODO: remove the check, ssm should only have staves with patients in range */

        if (DARR_NUM(sota->patients) == 0) {
            SDL_Log("Invalid Loadout: no patient in range");
            SDL_assert(false);
            // LoadoutSelectMenu_Deselect(ssm);
        } else {
            Game_postLoadout_Patients(sota, sota->aggressor);
            mc->visible = false;
            strncpy(sota->reason, "staff was selected, time to select patient", sizeof(sota->reason));
            Game_Switch_toCandidates(sota, sota->patients);
        }
    }
}

/* --- fsm_psm_option_accept --- */
void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moQuit(struct Game *sota, struct Menu *mc) {

    Event_Emit(__func__, SDL_USEREVENT, event_Quit, data1_entity, data2_entity);
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moWait(struct Game *sota, struct Menu *mc) {

    /* Pop all menus */
    b32 destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, data1_entity, data2_entity);

}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moEndT(struct Game *sota, struct Menu *mc) {

    // int army_i = Map_Army_Next(sota->map);
    // Todo start transition animation
    // Todo switch control to next army

    Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, data1_entity, data2_entity);
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moSeize( struct Game *sota, struct Menu *mc) {
    SDL_Log("Throne was seized: Map was won!");
    sota->map->win = true;

    /* - Go back to standby - */
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);

}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDance(struct Game *sota, struct Menu *mc) {
    SDL_assert(DARR_NUM(sota->spectators) > 0);

    /* - Turn player_select_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    SDL_assert(sota->state      == GAME_STATE_Gameplay_Map);
    SDL_assert(sota->substate   == GAME_SUBSTATE_MENU);
    strncpy(sota->reason, "dance was selected, time to select spectator", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->spectators);
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moStaff(struct Game *sota, struct Menu *mc) {

    /* -- Create StaffSelectMenu -- */
    if (sota->staff_select_menu == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);

    SDL_assert(sota->staff_select_menu    > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);
    Game_StaffSelectMenu_Enable(sota, sota->selected_unit_entity);

    /* -- Enable healmap rangemap to choose patients -- */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    Unit_Find_Usable(unit, ITEM_ARCHETYPE_STAFF);

    // TODO: save rangemap previous state? how to go back
    unit->rangemap = RANGEMAP_HEALMAP;
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    /* -- Create PopUp_Loadout_Stats -- */
    // TODO: only if selecting weapon?
    // if (sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
    //     Game_PopUp_Loadout_Stats_Create(sota);

    // PopUp_Loadout_Stats_Unit(pls, unit);

    /* -- TODO: Render Face -- */
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moAtk(struct Game *sota, struct Menu *mc_bad) {
    /* -- Create WeaponSelectMenu -- */
    //      -> WeaponsSelectMenu should only show weapons USABLE ON TILE

    /* Select loadout first */
    if (sota->weapon_select_menu == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);

    SDL_assert(sota->weapon_select_menu     > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    Game_WeaponSelectMenu_Enable(sota, sota->selected_unit_entity);

    struct Menu *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, Menu);
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    /* 4. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 5. Move cursor to weak hand, strong hand on top */
    Menu_Elem_Set(mc, sota, 0);

    struct LoadoutSelectMenu *wsm = mc->data;

    /* -- Create PopUp_Loadout_Stats -- */
    if (sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
        Game_PopUp_Loadout_Stats_Create(sota);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    popup->visible = true;
    struct PopUp_Loadout_Stats *pls = popup->data;

    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    PopUp_Loadout_Stats_Unit(pls, unit);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    // Map_Attacktomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
    //                                  pls->item_left, pls->item_right);
    // Map_Healtomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
    //                                pls->item_left, pls->item_right);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    /* -- Enable attack rangemap to choose defendants -- */
    // TODO: save rangemap previous state? how to go back
    unit->rangemap = RANGEMAP_ATTACKMAP;
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

    /* -- TODO: Render Face -- */
}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moItem(struct Game *sota, struct Menu *mc) {
    /* -- Create ItemSelectMenu -- */
    if (sota->item_select_menu == TNECS_NULL)
        Game_ItemSelectMenu_Create(sota);

    SDL_assert(sota->item_select_menu > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);
    Game_ItemSelectMenu_Enable(sota, sota->selected_unit_entity);

    /* -- Create PopUp_Loadout_Stats -- */
    if (sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
        Game_PopUp_Loadout_Stats_Create(sota);

    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;

    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    PopUp_Loadout_Stats_Unit(pls, unit);

    /* -- TODO: Render Face -- */

}

void fsm_eAcpt_sGmpMap_ssMenu_mPSM_moDbgMap(struct Game *sota, struct Menu *mc) {
    SDL_assert(sota->state      == GAME_STATE_Title_Screen);
    SDL_assert(sota->substate   == GAME_SUBSTATE_MENU);
    Event_Emit(__func__, SDL_USEREVENT, event_Load_Debug_Map, data1_entity, data2_entity);
}

/* -- Menu Pop/Exit FSM -- */
// For Last menu popped? for Any menu popped?
void fsm_Pop_sGmpMap_ssMenu_mPSM(struct Game *sota, struct Menu *mc) {
    /* Popped menu reverter */
    // TODO fsm_Pop_sGmpMap_ssMenu_m -> for menu popping
    struct PlayerSelectMenu *menu_ptr = (struct PlayerSelectMenu *)mc->data;
    i8 new_substate = -1;

    switch (menu_ptr->id) {
        case MENU_PLAYER_SELECT_UNIT_ACTION:
            ;

            tnecs_entity unit_ent       = sota->selected_unit_entity;
            struct Unit *unit             = TNECS_GET_COMPONENT(sota->world, unit_ent, Unit);
            struct Position *selected_pos = TNECS_GET_COMPONENT(sota->world, unit_ent, Position);
            new_substate                  = GAME_SUBSTATE_MAP_UNIT_MOVES;
            strncpy(sota->reason, "Unit action is taken after Map_unit moves only",
                    sizeof(sota->reason));

            // 1. Moving entity back to original spot in map
            struct Point moved_pos = sota->selected_unit_moved_position;
            struct Point init_pos  = sota->selected_unit_initial_position;
            if ((init_pos.x != moved_pos.x) || (init_pos.y != moved_pos.y))
                Map_Unit_Move(sota->map, moved_pos.x, moved_pos.y, init_pos.x, init_pos.y);

            // 2. Moving pos ptr to initial position to compute initial attacktomap
            // 2.1 inital pos != moved pos, so cursor would move...
            Position_Pos_Set(selected_pos, init_pos.x, init_pos.y);
            Map_Healtomap_Compute(  sota->map, sota->world, sota->selected_unit_entity, true, true);
            Map_Attacktomap_Compute(sota->map, sota->world, sota->selected_unit_entity, true, true);
            // 2.2 BUT: Moving pos ptr to selected position so that cursor doesn't move
            Position_Pos_Set(selected_pos, moved_pos.x, moved_pos.y);

            // 3. Compute new stackmap with recomputed attacktomap
            if (Unit_canStaff(unit)) {
                int overlay = MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_HEAL + MAP_OVERLAY_MOVE;
                Map_Palettemap_Autoset(sota->map, overlay);
            } else if (Unit_canAttack(unit)) {
                int overlay = MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK +
                              MAP_OVERLAY_DANGER;
                Map_Palettemap_Autoset(sota->map, overlay);
            }
            Map_Stacked_Dangermap_Compute(sota->map, sota->map->dangermap);

            // 4. Revert Unit animation state to move
            struct Sprite *sprite;
            sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
            // TODO: REMOVE IF WHEN ALL MAP_UNITS HAVE SPRITESHEETS.
            if ((sprite->spritesheet != NULL) && (sprite->spritesheet->loop_num == MAP_UNIT_LOOP_NUM)) {
                Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_LOOP_MOVER, sprite->flip);
                Sprite_Animation_Loop(sprite);
                Sprite_Draw(sprite, sota->renderer);
            }

            break;
        case MENU_PLAYER_SELECT_MAP_ACTION:
            new_substate = GAME_SUBSTATE_STANDBY;
            strncpy(sota->reason, "Map action is taken on standby only", sizeof(sota->reason));
            break;
        default:
            SDL_Log("invalid PlayerSelectMenu id");
    }

    strncpy(sota->reason, "stops showing player select menu", sizeof(sota->reason));
    if ((sota->substate != new_substate) && (new_substate > 0))
        Game_subState_Set(sota, new_substate, sota->reason);

}

/* event_Input_Start */
void fsm_eStart_sPrep_ssMenu_mSM(struct Game *sota, struct Menu *mc) {
    /* --- Start battle? --- */

}

void fsm_eStart_sPrep_ssMenu_mDM(struct Game *sota, struct Menu *mc) {
    /* --- Start battle --- */

    /* -- Game substate to on Map -- */
    strncpy(sota->reason, "Preparation done. March!", sizeof(sota->reason));
    if (sota->state != GAME_STATE_Gameplay_Map)
        Game_State_Set(sota, GAME_STATE_Gameplay_Map, sota->reason);
    if (sota->substate != GAME_SUBSTATE_STANDBY)
        Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);

    /* -- Place units in deployment slots on map -- */
    struct DeploymentMenu *dm = mc->data;

    /* -- Load map -- */
    Game_PopUp_Tile_Create(sota);
    Game_cursorFocus_onMap(sota);

    /* -- Destroy Deployment Menu -- */
    b32 destroy = true;
    tnecs_entity menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity == sota->deployment_menu);

    /* -- Disable palette map during turn transition -- */
    Map_Palettemap_Autoset(sota->map, 0);

    /* -- Set cursor position to first starting position -- */
    SDL_assert(sota                     != NULL);
    SDL_assert(sota->entity_cursor      != TNECS_NULL);
    struct Position *position = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(position                 != NULL);
    SDL_assert(sota->map                != NULL);
    SDL_assert(sota->map->start_pos     != NULL);
    SDL_assert(DARR_NUM(sota->map->start_pos) > 0);
    Position_Pos_Set(position, sota->map->start_pos[0].x, sota->map->start_pos[0].y);

    // Game_cursorFocus_onMap resets position to cursor_lastpos;
    // TODO: bool flag to disable Game_cursorFocus_onMap resetting cursor pos.
    sota->cursor_lastpos.x = sota->map->start_pos[0].x;
    sota->cursor_lastpos.y = sota->map->start_pos[0].y;

    /* -- Start turn transition -- */
    Event_Emit(__func__, SDL_USEREVENT, event_Turn_Transition, NULL, NULL);
}

