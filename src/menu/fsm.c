
#include "menu/fsm.h"

/* --- Menu FSMs --- */
// NOTE: my menu naming convention is BAD
// - player_select_menu -> player is SELECTING
// - staff_select_menu -> staff is BEING SELECTED
// -> MAKE COHERENT

/* -- Menu-type FSMs -- */
fsm_menu_t fsm_menu_type_IA[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &fsm_menu_type_IA_player_select,
    /* MENU_TYPE_WEAPON_SELECT  */  &fsm_menu_type_IA_weapon_select,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_menu_type_IA_staff_select,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_menu_type_IA_item_select,
    /* MENU_TYPE_TEXT_ICONS */      NULL,
    /* MENU_TYPE_STATS */           &fsm_menu_type_IA_stats,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TILE_HUD */        NULL,
    /* MENU_TYPE_TILE_COSTS */      NULL,
    /* MENU_TYPE_PRE_COMBAT */      &fsm_menu_type_IA_pre_combat,
    /* MENU_TYPE_TRADE */           &fsm_menu_type_IA_trade,
};

fsm_menu_t fsm_menu_type_IC[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   &fsm_menu_type_IC_player_select,
    /* MENU_TYPE_WEAPON_SELECT */   &fsm_menu_type_IC_weapon_select,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_menu_type_IC_staff_select,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_menu_type_IC_item_select,
    /* MENU_TYPE_TEXT_ICONS */      NULL,
    /* MENU_TYPE_STATS */           &fsm_menu_type_IC_stats,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         &fsm_menu_type_IC_stats,
    /* MENU_TYPE_TILE_HUD */        NULL,
    /* MENU_TYPE_TILE_COSTS */      NULL,
    /* MENU_TYPE_PRE_COMBAT */      &fsm_menu_type_IC_pre_combat,
    /* MENU_TYPE_TRADE */           &fsm_menu_type_IC_trade,
};

fsm_menu_t fsm_menu_type_CMs[MENU_TYPE_END] = {
    /* MENU_TYPE_START */           NULL,
    /* MENU_TYPE_PLAYER_SELECT */   NULL,
    /* MENU_TYPE_WEAPON_SELECT */   &fsm_menu_type_CMs_weapon_select,
    /* MENU_TYPE_STAFF_SELECT  */   &fsm_menu_type_CMs_staff_select,
    /* MENU_TYPE_ITEM_SELECT  */    &fsm_menu_type_CMs_item_select,
    /* MENU_TYPE_TEXT_ICONS */      NULL,
    /* MENU_TYPE_STATS */           NULL,
    /* MENU_TYPE_RESCUE */          NULL,
    /* MENU_TYPE_SUPPORTS */        NULL,
    /* MENU_TYPE_GROWTHS */         NULL,
    /* MENU_TYPE_TILE_HUD */        NULL,
    /* MENU_TYPE_TILE_COSTS */      NULL,
    /* MENU_TYPE_PRE_COMBAT */      NULL,
    /* MENU_TYPE_TRADE */           NULL,
};

/* -- Menu-option FSMs -- */
fsm_menu_t fsm_psm_options_IA[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         &fsm_psm_option_IA_items,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_psm_option_IA_staff,
    /* MENU_OPTION_DANCE */         &fsm_psm_option_IA_dance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_psm_option_IA_attack,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_psm_option_IA_trade,
    /* MENU_OPTION_MAP */           NULL,
    /* MENU_OPTION_WAIT */          &fsm_psm_option_IA_wait,
    /* MENU_OPTION_OPEN */          NULL,
    /* MENU_OPTION_QUIT */          &fsm_psm_option_IA_quit,
    /* MENU_OPTION_END_TURN */      &fsm_psm_option_IA_end_turn,
    /* MENU_OPTION_UNITS */         NULL,
    /* MENU_OPTION_CONVOY */        NULL,
    /* MENU_OPTION_GLOBAL_RANGE */  NULL,
    /* MENU_OPTION_NEW_GAME */      NULL,
    /* MENU_OPTION_LOAD */          NULL,
    /* MENU_OPTION_ERASE */         NULL,
    /* MENU_OPTION_COPY */          NULL,
    /* MENU_OPTION_OPTIONS */       NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     &fsm_psm_option_IA_debug_map,
};

// Cancel depending on previous menu_option when selecting map candidates
fsm_menu_t fsm_map_candidates_IC[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         NULL,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_map_candidate_IC_staff,
    /* MENU_OPTION_DANCE */         &fsm_map_candidate_IC_dance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_map_candidate_IC_attack,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_map_candidate_IC_trade,
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
    /* MENU_OPTION_OPTIONS */       NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     NULL,
};

// Accept depending on previous menu_option when selecting map candidates
fsm_menu_t fsm_map_candidates_IA[MENU_OPTION_NUM] = {
    /* MENU_OPTION_START */         NULL,
    /* MENU_OPTION_ITEMS */         NULL,
    /* MENU_OPTION_TALK */          NULL,
    /* MENU_OPTION_STAFF */         &fsm_map_candidate_IA_staff,
    /* MENU_OPTION_DANCE */         &fsm_map_candidate_IA_dance,
    /* MENU_OPTION_RESCUE */        NULL,
    /* MENU_OPTION_SEIZE */         NULL,
    /* MENU_OPTION_ESCAPE */        NULL,
    /* MENU_OPTION_ATTACK */        &fsm_map_candidate_IA_attack,
    /* MENU_OPTION_VILLAGE */       NULL,
    /* MENU_OPTION_TRADE */         &fsm_map_candidate_IA_trade,
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
    /* MENU_OPTION_OPTIONS */       NULL,
    /* MENU_OPTION_EXTRAS */        NULL,
    /* MENU_OPTION_DEBUG_MAP */     NULL,
};

/* --- fsm_map_candidates_IA --- */
void fsm_map_candidate_IA_trade(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Open Trade menu - */
    tnecs_entity_t active   = sota->selected_unit_entity;
    tnecs_entity_t passive  = sota->candidates[sota->candidate];
    SDL_assert(active       > TNECS_NULL);
    SDL_assert(passive      > TNECS_NULL);
    // Game_TradeMenu_Enable(sota, active, passive);

    strncpy(sota->reason, "time to trade", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    /* DESIGN QUESTION: should unit wait if weapon was traded? */
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IA_dance(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* - Refresh spectator - */
    SDL_assert(sota->candidates != NULL);
    SDL_assert(sota->candidates[sota->candidate] > TNECS_NULL);
    tnecs_entity_t spectator = sota->candidates[sota->candidate];
    SDL_assert(spectator > TNECS_NULL);
    Game_Unit_Refresh(sota, spectator);

    /* - Dancer waits - */
    tnecs_entity_t dancer = sota->selected_unit_entity;
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IA_staff(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    tnecs_entity_t healer = sota->selected_unit_entity;
    /* - Healer uses staff on patient - */
    /* IF no one hand staff SKILL */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, healer, Unit);
    tnecs_entity_t patient_ent = sota->candidates[sota->candidate];
    struct Unit *patient = TNECS_GET_COMPONENT(sota->world, patient_ent, Unit);
    struct Inventory_item *staff_inv = Unit_Item_Strong(unit, UNIT_HAND_STRONG);
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IA_attack(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* - Set Defendant to selected unit - */
    // ONLY FOR ATTACK! HOW TO SWITCH FOR SPECTATOR/PATIENTS?
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
    // ONLY FOR ATTACK! HOW TO SWITCH FOR SPECTATOR/PATIENTS?
    Event_Emit(__func__, SDL_USEREVENT, event_Defendant_Select, data1_entity, data2_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- fsm_map_candidates_IC --- */
void fsm_map_candidate_IC_dance(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* 1. Turn menu_player_select_unit_action visible */
    tnecs_entity_t menu = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, menu, MenuComponent);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_PLAYER_SELECT);

    /* 2. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 3. Move cursor to Dance menu option */
    struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc->data;
    int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_DANCE);
    MenuComponent_Elem_Set(mc, sota, new_elem);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IC_staff(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* 1. Turn top menu (Staff_Select_Menu) visible */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity_t menu_top = sota->menu_stack[stack_top];
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, menu_top, MenuComponent);
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
    tnecs_entity_t cursor = sota->entity_cursor;
    MenuComponent_Elem_Set(mc, sota, 0);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IC_trade(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* 1. Turn Player_Select_Menu visible */
    int stack_top = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity_t menu_top = sota->menu_stack[stack_top];
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, menu_top, MenuComponent);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_PLAYER_SELECT);

    /* 2. Update psm options */
    Game_preUnitAction_Targets(sota, sota->selected_unit_entity);
    Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_UNIT_ACTION);

    /* 3. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_map_candidate_IC_attack(struct Game *sota, struct MenuComponent *in_mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* 1. Turn Item_Select_Menu visible */
    int stack_top               = DARR_NUM(sota->menu_stack) - 1;
    tnecs_entity_t menu_top     = sota->menu_stack[stack_top];
    struct MenuComponent *mc    = TNECS_GET_COMPONENT(sota->world, menu_top, MenuComponent);
    SDL_assert(mc != NULL);
    SDL_assert(mc->elem_pos != NULL);
    mc->visible = true;
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);

    /* 2. Cancel one of the selection of item select menu */
    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(wsm->selected[UNIT_HAND_LEFT]  > -1);
    SDL_assert(wsm->selected[UNIT_HAND_RIGHT] > -1);
    LoadoutSelectMenu_Deselect(wsm);
    SDL_assert((wsm->selected[UNIT_HAND_LEFT] > -1) ^ (wsm->selected[UNIT_HAND_RIGHT] > -1));

    /* 3. Set previous_cs to new loadout */
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;

    // pls->item_left  = wsm->unit->eq_usable[UNIT_HAND_LEFT];
    // pls->item_right = wsm->unit->eq_usable[UNIT_HAND_RIGHT];
    PopUp_Loadout_Stats_New(pls);

    /* 4. Focus on menu */
    Game_cursorFocus_onMenu(sota);

    /* 5. Move cursor to weak hand, strong hand on top */
    MenuComponent_Elem_Set(mc, sota, UNIT_HAND_WEAK);

    /* 6. HUD reappear */
    popup->visible = true;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- fsm_menu_type_IC --- */
void fsm_menu_type_CMs_staff_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // TODO: update healmap when STAFF CHANGES
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_CMs_weapon_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *wsm = mc->data;

    /* - Get Popup_Loadout_Stats -- */
    tnecs_entity_t popup_ent        = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
    struct PopUp *popup             = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Loadout_Stats *pls = popup->data;
    SDL_assert(popup_ent > TNECS_NULL);

    /* -- Hover on new item -- */
    PopUp_Loadout_Stats_Hover(pls, wsm, mc->elem);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    Map_Attacktomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
                                     pls->item_left, pls->item_right);
    Map_Healtomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
                                   pls->item_left, pls->item_right);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map);

    PopUp_Loadout_Stats_New(pls);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_CMs_item_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Update Popup_Loadout_Stats to potential loadout -- */
    struct LoadoutSelectMenu *ism = mc->data;

    /* - Get Popup_Loadout_Stats -- */
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);
    tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS];
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- fsm_menu_type_IC --- */
void fsm_menu_type_IC_trade(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If item is selected, deselect item */
    bool isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < DEFAULT_EQUIPMENT_SIZE);
    bool isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    if (isItem || isTrader) {
        // TradeMenu_Deselect(tm);
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* - Hide TradeMenu - */
    bool destroy = false;
    tnecs_entity_t menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity == sota->trade_menu);

    /* - Go back to MapCandidates with traders - */

    /* - Turn staff_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    strncpy(sota->reason, "trade was selected, time to select passive trade", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->passives);
    Game_Cursor_Move_toCandidate(sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_staff_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // IF not staff skill
    // - Destroy staff_select menu
    SDL_assert(mc->type == MENU_TYPE_STAFF_SELECT);
    struct LoadoutSelectMenu *ssm = mc->data;
    int tophand = Unit_Hand_Strong(ssm->unit);
    int bothand = 1 - tophand;

    if ((ssm->selected[tophand] > -1) && (StaffSelectMenu_canEqItem(ssm))) {
        /* move cursor to first hand */
        int new_elem = WSM_ELEM_ITEM1;
        tnecs_entity_t cursor = sota->entity_cursor;
        MenuComponent_Elem_Set(mc, sota, new_elem);

        /* Item selected in hand 0, swapping */
        LoadoutSelectMenu_Deselect(ssm);

        /* Switching to staves because menu was in items mode */
        StaffSelectMenu_Switch_Staves(ssm);

        SDL_assert(ssm->update == true);
    } else {
        /* move cursor to second hand */
        bool destroy = false;
        tnecs_entity_t menu_popped_entity = Game_menuStack_Pop(sota, destroy);
        SDL_assert(menu_popped_entity == sota->staff_select_menu);

        /* -- No item was selected, destroying ism menu -- */
        /* 2. Focus on psm */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Staff menu option on psm */
        tnecs_entity_t menu = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
        struct MenuComponent *mc_ua = TNECS_GET_COMPONENT(sota->world, menu, MenuComponent);
        struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc_ua->data;
        int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_STAFF);
        MenuComponent_Elem_Set(mc_ua, sota, new_elem);

    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_pre_combat(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct MenuComponent *menu;
    menu = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, MenuComponent);
    struct LoadoutSelectMenu *wsm = menu->data;
    SDL_assert(wsm->selected[UNIT_HAND_LEFT]    > -1);
    SDL_assert(wsm->selected[UNIT_HAND_RIGHT]   > -1);

    // 1. Destroy pre_combat menu
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    bool destroy = false;
    tnecs_entity_t menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity > 0);
    struct MenuComponent *menu_comp_popped;
    menu_comp_popped = TNECS_GET_COMPONENT(sota->world, menu_popped_entity, MenuComponent);

    // 2. Make cursor visible
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    sprite->visible = true;

    // 3. Go back to selecting defendant
    Event_Emit(__func__, SDL_USEREVENT, event_Loadout_Selected, data1_entity, data2_entity);

    // 4. Attacktomap: recompute for aggressor
    Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    Map_Stacked_Dangermap_Compute(sota->map);

    SDL_assert(wsm->selected[UNIT_HAND_LEFT]    > -1);
    SDL_assert(wsm->selected[UNIT_HAND_RIGHT]   > -1);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_player_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    bool destroy = false;
    tnecs_entity_t menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity > 0);
    struct MenuComponent *menu_comp_popped;
    menu_comp_popped = TNECS_GET_COMPONENT(sota->world, menu_popped_entity, MenuComponent);

    struct PlayerSelectMenu *menu_ptr = (struct PlayerSelectMenu *)mc->data;
    if8 new_substate = -1;

    switch (menu_ptr->id) {
        case MENU_PLAYER_SELECT_UNIT_ACTION:
            ;

            tnecs_entity_t unit_ent       = sota->selected_unit_entity;
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
            Map_Healtomap_Compute(sota->map, sota->world, sota->selected_unit_entity,   true, true);
            Map_Attacktomap_Compute(sota->map, sota->world, sota->selected_unit_entity, true, true);
            // 2.2 BUT: Moving pos ptr to selected position so that cursor doesn't move
            Position_Pos_Set(selected_pos, moved_pos.x, moved_pos.y);

            // 3. Compute new stackmap with recomputed attacktomap
            if (Unit_canStaff(unit)) {
                Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_HEAL + MAP_OVERLAY_MOVE);
            } else if (Unit_canAttack(unit)) {
                Map_Palettemap_Autoset(sota->map,
                                       MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK + MAP_OVERLAY_DANGER);
            }
            Map_Stacked_Dangermap_Compute(sota->map);

            // 4. Revert Unit animation state to move
            struct Sprite *sprite;
            sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
            // TODO: REMOVE IF WHEN ALL MAP_UNITS HAVE SPRITESHEETS.
            if (sprite->spritesheet != NULL) {
                SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
                Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_MOVER, sprite->flip);
                Sprite_Animation_Loop(sprite);
                Sprite_Draw(sprite, sota->renderer);
            }

            break;
        case MENU_PLAYER_SELECT_MAP_ACTION:
            new_substate = GAME_SUBSTATE_STANDBY;
            strncpy(sota->reason, "Map action is taken on standby only", sizeof(sota->reason));
            break;
        default:
            SOTA_Log_Debug("invalid PlayerSelectMenu id");
    }
    strncpy(sota->reason, "stops showing player select menu", sizeof(sota->reason));
    if ((sota->substate != new_substate) && (new_substate > 0))
        Game_subState_Set(sota, new_substate, sota->reason);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_weapon_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(mc->type == MENU_TYPE_WEAPON_SELECT);
    struct LoadoutSelectMenu *wsm = mc->data;

    int tophand = Unit_Hand_Strong(wsm->unit);
    int bothand = 1 - tophand;

    if (wsm->selected[tophand] > -1) {
        /* move cursor to first hand */
        int new_elem = WSM_ELEM_ITEM1;
        tnecs_entity_t cursor = sota->entity_cursor;
        MenuComponent_Elem_Set(mc, sota, new_elem);

        /* Item selected in hand 0, swapping */
        LoadoutSelectMenu_Deselect(wsm);

        /* Set previous_cs to new loadout */
        int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
        struct PopUp_Loadout_Stats *pls = (struct PopUp_Loadout_Stats *)popup->data;
        pls->item_left  = wsm->unit->eq_usable[UNIT_HAND_LEFT];
        pls->item_right = wsm->unit->eq_usable[UNIT_HAND_RIGHT];
        PopUp_Loadout_Stats_Previous(pls);
        // pls->previous_cs = Unit_computedStats_wLoadout(wsm->unit, pls->item_left, pls->item_right);

        SDL_assert(wsm->update == true);
    } else {
        /* -- No item was selected, destroying wsm menu -- */
        bool destroy = false;
        tnecs_entity_t popped_ent = Game_menuStack_Pop(sota, destroy);
        SDL_assert(popped_ent > 0);

        /* 2. Focus on psm */
        Game_cursorFocus_onMenu(sota);

        /* 3. Move cursor to Attack menu option on psm */
        tnecs_entity_t menu          = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
        struct MenuComponent *mc     = TNECS_GET_COMPONENT(sota->world, menu, MenuComponent);
        struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc->data;
        int new_elem = PlayerSelectMenu_Option_Index(psm, MENU_OPTION_ATTACK);
        MenuComponent_Elem_Set(mc, sota, new_elem);

        /* 4. Hide loadout stats Popup */
        Game_PopUp_Loadout_Stats_Hide(sota);
        LoadoutSelectMenu_Select_Reset(wsm);

        /* 5. Reset previous candidate */
        sota->previous_candidate = -1;
    }

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    Map_Healtomap_Compute(  sota->map, sota->world, sota->aggressor, false, true);
    Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_item_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(mc->type == MENU_TYPE_ITEM_SELECT);
    struct LoadoutSelectMenu *ism = mc->data;

    /* -- Popping ism -- */
    bool destroy = false;
    tnecs_entity_t popped_ent = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped_ent > 0);

    /* Focus on unit action menu */
    Game_cursorFocus_onMenu(sota);

    /* Hide loadout stats Popup */
    Game_PopUp_Loadout_Stats_Hide(sota);
    ism->selected[UNIT_HAND_LEFT]   = -1;
    ism->selected[UNIT_HAND_RIGHT]  = -1;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IC_stats(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool destroy = false;
    tnecs_entity_t menu_popped_entity = Game_menuStack_Pop(sota, destroy);
    SDL_assert(menu_popped_entity > 0);
    struct MenuComponent *menu_comp_popped;
    menu_comp_popped = TNECS_GET_COMPONENT(sota->world, menu_popped_entity, MenuComponent);

    if8 new_substate = GAME_SUBSTATE_STANDBY;
    strncpy(sota->reason, "stops showing stats menu", sizeof(sota->reason));
    Game_subState_Set(sota, new_substate, sota->reason);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- fsm_menu_type_IA --- */
void fsm_psm_option_IA_trade(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_trade(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct TradeMenu *tm = mc->data;
    SDL_assert(tm);

    /* If no item is selected, select item */
    // bool isItem = (tm->selected_item > ITEM_NULL) && (tm->selected_item < DEFAULT_EQUIPMENT_SIZE);
    // bool isTrader = (tm->selected_trader == TRADER_PASSIVE) || (tm->selected_trader == TRADER_ACTIVE);
    // if (!isItem || !isTrader) {
    //     TradeMenu_Select(tm);
    //     SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    //     return;
    // }

    // TradeMenu_Select(tm, mc->elem);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_pre_combat(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* Start Combat */
    Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_stats(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_item_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_weapon_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* Swap weapons */
    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);
    LoadoutSelectMenu_Select(wsm, mc->elem);

    /* - Compute new attackmap with equipped - */
    int rangemap = Unit_Rangemap_Get(wsm->unit);
    Map_Healtomap_Compute(sota->map,   sota->world, sota->aggressor, false, true);
    Map_Attacktomap_Compute(sota->map, sota->world, sota->aggressor, false, true);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map);

    // TODO: Change to if other item in invetory
    if (WeaponSelectMenu_Usable_Remains(wsm)) {

        /* move cursor to second hand */
        int new_elem            = WSM_ELEM_ITEM2;
        tnecs_entity_t cursor   = sota->entity_cursor;
        MenuComponent_Elem_Set(mc, sota, new_elem);

        int popup_ind       = POPUP_TYPE_HUD_LOADOUT_STATS;
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
        struct PopUp_Loadout_Stats *pls = popup->data;

        // Set previous_cs to new loadout
        pls->item_left  = UNIT_HAND_LEFT;
        pls->item_right = UNIT_HAND_RIGHT;
        PopUp_Loadout_Stats_Previous(pls);

        LoadoutSelectMenu_Elem_Reset(wsm, mc);
        LoadoutSelectMenu_Elem_Pos(  wsm, mc);
        MenuComponent_Elem_Boxes_Check(mc);

    } else {
        /* Loadout selected, find new defendants*/
        // TODO: use WeaponSelectMenu_Loadout_Valid/remove it
        Game_postLoadout_Defendants(sota, sota->aggressor);

        /* - Check that a defendant is in range of current loadout - */
        if (DARR_NUM(sota->defendants) == 0) {
            SOTA_Log_Debug("Invalid Loadout: no defendant in range");
            LoadoutSelectMenu_Deselect(wsm);
        } else {
            Event_Emit(__func__, SDL_USEREVENT, event_Loadout_Selected, data1_entity, data2_entity);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_player_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert((sota->state == GAME_STATE_Gameplay_Map) || (sota->state == GAME_STATE_Title_Screen));
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);

    struct PlayerSelectMenu *psm_ptr = mc->data;
    SDL_assert(psm_ptr->option_num == mc->elem_num);
    sota->selected_menu_option = psm_ptr->options[mc->elem];
    if (fsm_psm_options_IA[sota->selected_menu_option] != NULL)
        fsm_psm_options_IA[sota->selected_menu_option](sota, mc);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_menu_type_IA_staff_select(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(mc->elem >= 0);
    SDL_assert(mc->elem < DEFAULT_EQUIPMENT_SIZE);

    LoadoutSelectMenu_Select(ssm, mc->elem);

    /* - Switch to Map_Candidates substate - */
    SDL_assert(sota->state    == GAME_STATE_Gameplay_Map);
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);

    if (StaffSelectMenu_canEqItem(ssm)) {
        /* move cursor to second hand */
        int new_elem            = WSM_ELEM_ITEM2;
        tnecs_entity_t cursor   = sota->entity_cursor;
        MenuComponent_Elem_Set(mc, sota, new_elem);
        StaffSelectMenu_Switch_Items(ssm);
    } else {
        /* - Check that a patient is in range of current loadout - */
        /* TODO: remove the check, ssm should only have staves with patients in range */

        if (DARR_NUM(sota->patients) == 0) {
            SOTA_Log_Debug("Invalid Loadout: no patient in range");
            LoadoutSelectMenu_Deselect(ssm);
        } else {
            Game_postLoadout_Patients(sota, sota->aggressor);
            mc->visible = false;
            strncpy(sota->reason, "staff was selected, time to select patient", sizeof(sota->reason));
            Game_Switch_toCandidates(sota, sota->patients);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- fsm_psm_option_accept --- */
void fsm_psm_option_IA_quit(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    Event_Emit(__func__, SDL_USEREVENT, event_Quit, data1_entity, data2_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_wait(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* Pop all menus */
    bool destroy = false;
    while (DARR_NUM(sota->menu_stack) > 0)
        Game_menuStack_Pop(sota, destroy);

    Event_Emit(__func__, SDL_USEREVENT, event_Unit_Wait, data1_entity, data2_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_end_turn(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, data1_entity, data2_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_dance(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(DARR_NUM(sota->spectators) > 0);

    /* - Turn player_select_menu invisible - */
    mc->visible = false;

    /* - Switch to Map_Candidates substate - */
    SDL_assert(sota->state      == GAME_STATE_Gameplay_Map);
    SDL_assert(sota->substate   == GAME_SUBSTATE_MENU);
    strncpy(sota->reason, "dance was selected, time to select spectator", sizeof(sota->reason));
    Game_Switch_toCandidates(sota, sota->spectators);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_staff(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Create StaffSelectMenu -- */
    if (sota->staff_select_menu == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);

    SDL_assert(sota->staff_select_menu    > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);
    Game_StaffSelectMenu_Enable(sota, sota->selected_unit_entity);

    /* -- Enable healmap rangemap to choose patients -- */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    // TODO: save rangemap previous state? how to go back
    unit->rangemap = RANGEMAP_HEALMAP;
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    Map_Stacked_Dangermap_Compute(sota->map);

    /* -- Create PopUp_Loadout_Stats -- */
    // TODO: only if selecting weapon?
    // if (sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] == TNECS_NULL)
    //     Game_PopUp_Loadout_Stats_Create(sota);

    // struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    // PopUp_Loadout_Stats_Unit(pls, unit);

    /* -- TODO: Render Face -- */

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_attack(struct Game *sota, struct MenuComponent *mc_bad) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Create WeaponSelectMenu -- */
    //      -> WeaponsSelectMenu should only show weapons USABLE ON TILE

    /* Select loadout first */
    if (sota->weapon_select_menu == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);

    SDL_assert(sota->weapon_select_menu     > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity   > TNECS_NULL);
    Game_WeaponSelectMenu_Enable(sota, sota->selected_unit_entity);

    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, MenuComponent);
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
    Map_Attacktomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
                                     pls->item_left, pls->item_right);
    Map_Healtomap_Compute_wLoadout(sota->map, sota->world, sota->aggressor, false,
                                   pls->item_left, pls->item_right);

    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map);

    /* -- Enable attack rangemap to choose defendants -- */
    // TODO: save rangemap previous state? how to go back
    unit->rangemap = RANGEMAP_ATTACKMAP;
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    Map_Stacked_Dangermap_Compute(sota->map);

    /* -- TODO: Render Face -- */

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_items(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_psm_option_IA_debug_map(struct Game *sota, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->state == GAME_STATE_Title_Screen);
    SDL_assert(sota->substate == GAME_SUBSTATE_MENU);
    Event_Emit(__func__, SDL_USEREVENT, event_Load_Debug_Map, data1_entity, data2_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}