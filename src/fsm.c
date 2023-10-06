
#include "fsm.h"

/* --- ACTIONS --- */
fsm_state_GR2S_t fsm_state_GR2S[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_GR2S_GAMEPLAY_MAP,
    /* Gameplay_Camp */  &fsm_substate_GR2S_GAMEPLAY_CAMP,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

fsm_state_IA_t fsm_state_IA[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_IA_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   &fsm_state_IA_TITLE_SCREEN,
    /* Animation */      NULL,
};

fsm_state_IC_t fsm_state_IC[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_IC_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

fsm_state_ImR_t fsm_state_ImR[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_ImR_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

fsm_state_ImL_t fsm_state_ImL[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_ImL_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

fsm_state_CMd_t fsm_state_CMd[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_CMd_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   NULL,
    /* Animation */      NULL,
};

/* --- SUB-ACTIONS --- */
fsm_state_GR2S_t fsm_substate_GR2S[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  &fsm_substate_GR2S_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_CMs_t fsm_sub_event_CMs[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_sub_event_CMs_MENU,
    /* MAP_UNIT_MOVES */  &fsm_sub_event_CMs_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_CMs_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  &fsm_sub_event_CMs_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_CMd_t fsm_sub_event_CMd[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  &fsm_sub_event_CMd_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_CMd_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  &fsm_sub_event_CMd_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_IA_t fsm_substate_IA[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     &fsm_substate_IA_MAP_MINIMAP,
    /* MENU */            &fsm_substate_IA_MENU,
    /* MAP_UNIT_MOVES */  &fsm_substate_IA_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      &fsm_substate_IA_MAP_COMBAT,
    /* MAP_NPCTURN */     &fsm_substate_IA_MAP_NPCTURN,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_IA_STANDBY,
    /* PAUSED */          &fsm_substate_IA_PAUSED,
    /* MAP_CANDIDATES */  &fsm_substate_IA_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_IC_t fsm_substate_IC[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_substate_IC_MENU,
    /* MAP_UNIT_MOVES */  &fsm_substate_IC_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      &fsm_substate_IC_MAP_COMBAT,
    /* MAP_NPCTURN */     &fsm_substate_IC_MAP_NPCTURN,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_IC_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  &fsm_substate_IC_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_IS_t fsm_sub_event_IS[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_IS_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_UD_t fsm_substate_UD[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_substate_UD_MENU,
    /* MAP_UNIT_MOVES */  &fsm_substate_UD_MAP_UNIT_MOVES,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_US_t fsm_substate_US[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_US_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_UDgr_t fsm_substate_UDgr[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_UDgr_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_ImR_t fsm_sub_event_ImR[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_sub_event_ImR_MENU,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_ImR_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_ImL_t fsm_sub_event_ImL[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_sub_event_ImL_MENU,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_ImL_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_CHU_t fsm_substate_CHU[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_CHU_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  &fsm_substate_CHU_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

fsm_state_CDU_t fsm_substate_CDU[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_substate_CDU_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  &fsm_substate_CDU_MAP_CANDIDATES,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* -- FSM: Input_globalRange EVENT -- */
fsm_state_IGR_t fsm_sub_event_IGR[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            NULL,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_sub_event_IGR_STANDBY,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* -- FSM: Input_globalRange EVENT -- */
void fsm_sub_event_IGR_STANDBY(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - if a unit is hovered or selected toggle rangemap - */
    tnecs_entity_t entity = TNECS_NULL;
    if (sota->selected_unit_entity != TNECS_NULL) {
        entity = sota->selected_unit_entity;
    } else if (sota->hovered_unit_entity != TNECS_NULL) {
        entity = sota->hovered_unit_entity;
    }

    if (entity != TNECS_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        /* - Toggle rangemap - */
        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, entity, Unit);
        Unit_Rangemap_Toggle(unit);
        int rangemap = Unit_Rangemap_Get(unit);

        /* - Compute new stackmap with recomputed attacktomap - */
        Map_Healtomap_Compute(sota->map,   sota->world, entity, true, false);
        Map_Attacktomap_Compute(sota->map, sota->world, entity, true, false);
        if (rangemap        == RANGEMAP_HEALMAP) {
            Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
        } else if (rangemap == RANGEMAP_ATTACKMAP) {
            Map_Palettemap_Autoset(sota->map,
                                   MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
        }
        Map_Stacked_Dangermap_Compute(sota->map);
        return;
    }

    if (sota->map->show_globalRange) {
        sota->map->show_globalRange = false;
        Map_Global_Danger_Sub(sota->map, sota->map->global_rangemap);
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_DANGER);

    } else {
        sota->map->show_globalRange = true;
        Map_globalRange(sota->map, sota->world, ALIGNMENT_ENEMY);
        Map_Global_Danger_Add(sota->map, sota->map->global_rangemap);

        /* Stack all overlay maps */
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// --- FSM ACTION AND SUBACTION DEFINITIONS ---

// -- FSM: Cursor_Hovers_Unit --
void fsm_substate_CHU_STANDBY(struct Game *sota, tnecs_entity_t hov_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(hov_ent > TNECS_NULL);

    /* -- Show popup_unit -- */
    // TODO: put unit popup Loading into Map/Gameplay loading functions
    tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    if (popup_ent == TNECS_NULL)
        Game_PopUp_Unit_Create(sota);
    popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];

    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Unit *popup_unit = popup->data;
    PopUp_Unit_Set(popup_unit, sota);
    SDL_assert(popup_unit->unit != NULL);
    popup->visible = true;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    // TODO: use struct Point everywhere, replace .x = with struct equality
    struct Point pos = cursor_pos->tilemap_pos;

    Game_PopUp_Unit_Place(sota, pos);

    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, hov_ent, Unit);
    if (unit_ontile->waits) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* -- Compute attackmap and movemap -- */
    sota->map->update = true;
    Map_Healtomap_Compute(  sota->map, sota->world, hov_ent, true, false);
    Map_Attacktomap_Compute(sota->map, sota->world, hov_ent, true, false);
    int rangemap = Unit_Rangemap_Get(unit_ontile);

    /* - Compute new stackmap with recomputed attacktomap - */
    int all_overlays = MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL + MAP_OVERLAY_ATTACK;
    all_overlays += MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER;
    if (rangemap        == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, all_overlays);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map, all_overlays);
    }

    /* Stack all overlay maps */
    Map_Stacked_Dangermap_Compute(sota->map);
    sota->map->show_icons = SotA_isPC(unit_ontile->army);

    /* -- Changing animation loop to Taunt -- */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, hov_ent, Sprite);
    bool animated = TNECS_ENTITY_HASCOMPONENT(sota->world, hov_ent, Timer);
    if ((sprite->spritesheet != NULL) && (animated) && (!unit_ontile->waits)) {
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_TAUNT, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->renderer);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_CHU_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t hov_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(hov_ent > TNECS_NULL);

    /* -- Start unit combat stance loop -- */
    // ONLY FOR ATTACK
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, hov_ent, Sprite);
    Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_STANCE, sprite->flip);
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: Cursor_Dehovers_Unit --
void fsm_substate_CDU_STANDBY(struct Game *sota, tnecs_entity_t dehov_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Re-computing overlay -- */
    Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_DANGER + MAP_OVERLAY_GLOBAL_DANGER);
    Map_Stacked_Dangermap_Reset(sota->map);

    /* -- Placing popup_unit out of view -- */
    tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Unit *popup_unit = popup->data;
    popup_unit->unit = NULL;
    struct SliderOffscreen *offscreen;
    offscreen = TNECS_GET_COMPONENT(sota->world, popup_ent, SliderOffscreen);
    offscreen->go_offscreen = false;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;

    /* -- Changing animation loop to IDLE -- */
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, dehov_ent, Sprite);
    SDL_assert(sprite != NULL);
    bool animated = TNECS_ENTITY_HASCOMPONENT(sota->world, dehov_ent, Timer);
    if ((sprite->spritesheet != NULL) && (animated)) {
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);
        Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_IDLE, sprite->flip);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->renderer);
    }

    Game_PopUp_Unit_Place(sota, pos);

    // In case an enemy unit was selected.
    sota->selected_unit_entity = TNECS_NULL;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_CDU_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t dehov_ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Reset unit loop to Idle --  */
    // ONLY FOR ATTACK
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, dehov_ent, Sprite);
    Spritesheet_Loop_Set(sprite->spritesheet, MAP_UNIT_SPRITE_LOOP_IDLE, sprite->flip);
    Sprite_Animation_Loop(sprite);
    Sprite_Draw(sprite, sota->renderer);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: INPUT_CANCEL EVENT --
void fsm_state_IC_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_substate_IC[sota->substate] != NULL)
        fsm_substate_IC[sota->substate](sota, canceller);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_UDgr_STANDBY(struct Game *sota, tnecs_entity_t selector_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    tnecs_entity_t selected = sota->selected_unit_entity;
    SDL_assert(selector_entity > TNECS_NULL);
    SDL_assert(sota->selected_unit_entity > TNECS_NULL);

    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, selected, Unit);

    /* -- Skip if enemy can't attack -- */
    if (!Unit_canAttack(unit)) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* -- Computing new dangermap -- */
    struct Position *pos    = TNECS_GET_COMPONENT(sota->world, selector_entity, Position);
    i32 *temp_danger        = Map_Danger_Compute(sota->map, sota->world, selected);
    int map_index = pos->tilemap_pos.y * sota->map->col_len + pos->tilemap_pos.x;
    if (unit->show_danger) {
        Map_Danger_Sub(sota->map, temp_danger);
        Map_Palettemap_Autoset(sota->map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
        unit->show_danger = false;
    } else {
        Map_Danger_Add(sota->map, temp_danger);
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER);
        unit->show_danger = true;
    }

    // linalg_matrix_print_int32_t(sota->map->attacktomap, sota->map->row_len, sota->map->col_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_STANDBY(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(canceller > 0);
    /* -- Preliminaries -- */
    *data1_entity = canceller;
    const struct Unit *unit_ontile;
    const struct Position *pos = TNECS_GET_COMPONENT(sota->world, canceller, Position);
    struct Point cpos = pos->tilemap_pos;
    tnecs_entity_t ontile = sota->map->unitmap[cpos.y * sota->map->col_len + cpos.x];

    if (ontile == TNECS_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* -- Show danger map if enemy -- */
    unit_ontile = TNECS_GET_COMPONENT(sota->world, ontile, Unit);
    SDL_assert(unit_ontile);
    SDL_assert(data2_entity != NULL);
    *data2_entity = ontile;
    if (!SotA_isPC(unit_ontile->army) && unit_ontile->show_danger)
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* 1. Dehover candidate defendant */
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    struct Point pos = cursor_pos->tilemap_pos;
    int current_i = pos.y * sota->map->col_len + pos.x;
    tnecs_entity_t ontile = sota->map->unitmap[current_i];

    SDL_assert(ontile > TNECS_NULL);
    // Try to limit calling FSM function directly.
    fsm_substate_CDU_MAP_CANDIDATES(sota, ontile);

    /* 2. set state to Menu */
    strncpy(sota->reason, "Cancel Selection of candidates", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    if (fsm_map_candidates_IC[sota->selected_menu_option] != NULL)
        fsm_map_candidates_IC[sota->selected_menu_option](sota, NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_MENU(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool destroy = false;
    tnecs_entity_t ent_topop = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    SDL_assert(ent_topop > TNECS_NULL);
    struct MenuComponent *mc_topop = TNECS_GET_COMPONENT(sota->world, ent_topop, MenuComponent);

    if (fsm_menu_type_IC[mc_topop->type] != NULL)
        fsm_menu_type_IC[mc_topop->type](sota, mc_topop);

    if (DARR_NUM(sota->menu_stack) == 0)
        Game_cursorFocus_onMap(sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->map->arrow->show = false;

    /* Return unit to initial pos, deselect */
    if (sota->selected_unit_entity > TNECS_NULL) {
        *data2_entity = sota->selected_unit_entity;
        SDL_assert(*data1_entity > 0);
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
    }

    /* Make Popup_Tile visible */
    tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    SDL_assert(popup != NULL);
    popup->visible = true;

    /* Reset cursor lastpos to current pos */
    // prevents cursor dehovering from far away
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    sota->cursor_lastpos.x = cursor_pos->tilemap_pos.x;
    sota->cursor_lastpos.y = cursor_pos->tilemap_pos.y;


    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_ANIMATION(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_MAP_COMBAT(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IC_MAP_NPCTURN(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// void fsm_substate_IC_MAP_GLOBAL_RANGE(struct Game * sota,
//         tnecs_entity_t canceller) {
//     SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
//     strncpy(sota->reason, "stops showing global Range", sizeof(sota->reason));
//     // Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);
//     Event_Emit(__func__, SDL_USEREVENT, event_Map_globalRange_Hide, NULL, NULL);
//     SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
// }

// -- FSM: CURSOR_MOVES EVENT --
void fsm_sub_event_CMs_STANDBY(struct Game *sota, tnecs_entity_t mover_entity,
                               struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t cursor = sota->entity_cursor;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    struct Slider   *cursor_sl  = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    struct Sprite   *cursor_sp  = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);

    /* Actually move the cursor from cursor_move_data set by systemControl */
    // Note: always on tilemap
    Position_Pos_Add(cursor_pos, sota->cursor_move.x, sota->cursor_move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    /* -- Give tile to popup -- */
    tnecs_entity_t ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMs_MENU(struct Game *sota, tnecs_entity_t mover_entity,
                            struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Find menu elem in direction */
    tnecs_entity_t menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, menu, MenuComponent);
    if8 new_elem;
    switch (mc->type) {
        case MENU_TYPE_PLAYER_SELECT:
            new_elem = Periodic_Elem_Move(mc, sota->moved_direction, 0, mc->elem_num);
            break;
        case MENU_TYPE_WEAPON_SELECT: {
            struct LoadoutSelectMenu *wsm = mc->data;
            int min = (wsm->selected[UNIT_HAND_STRONG] > -1) ? 1 : 0;
            new_elem = Periodic_Elem_Move(mc, sota->moved_direction, min, mc->elem_num);
            break;
        }
        case MENU_TYPE_ITEM_SELECT:
            new_elem = Periodic_Elem_Move(mc, sota->moved_direction, 0, DEFAULT_EQUIPMENT_SIZE);
            break;
        default:
            new_elem = MenuComponent_Elem_Move(mc, sota->moved_direction);
            break;
    }

    /* - TODO: MAKE FUNCTION - */
    /* - Move to cursor to new_elem - */
    SDL_assert(new_elem > MENU_ELEM_NULL);
    MenuComponent_Elem_Set(mc, sota, new_elem);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    if (fsm_menu_type_CMs[mc->type] != NULL)
        fsm_menu_type_CMs[mc->type](sota, mc);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMs_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t mover_entity,
                                      struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* Find menu elem in direction */
    SDL_assert(sota->candidates != NULL);
    int num = DARR_NUM(sota->candidates);
    SDL_assert(num > 0);
    if ((sota->cursor_move.x > 0) || (sota->cursor_move.y > 0)) {
        sota->candidate = (sota->candidate + 1) % num;
    } else if ((sota->cursor_move.x < 0) || (sota->cursor_move.y < 0)) {
        sota->candidate = (sota->candidate - 1 + num) % num;
    }

    Game_Cursor_Move_toCandidate(sota);

    // Reset cursor;
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMs_MAP_UNIT_MOVES(struct Game *sota,
                                      tnecs_entity_t mover_entity, struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Move cursor -- */
    tnecs_entity_t cursor       = sota->entity_cursor;
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, cursor, Position);
    struct Slider *cursor_sl    = TNECS_GET_COMPONENT(sota->world, cursor, Slider);
    struct Sprite *cursor_sp    = TNECS_GET_COMPONENT(sota->world, cursor, Sprite);

    // Always on tilemap
    Position_Pos_Add(cursor_pos, sota->cursor_move.x, sota->cursor_move.y);
    Cursor_Target(cursor_sl, cursor_sp, cursor_pos);
    sota->cursor_move.x = 0;
    sota->cursor_move.y = 0;

    /* -- give tile to popup -- */
    tnecs_entity_t ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, ent, PopUp);
    struct PopUp_Tile *popup_tile = popup->data;
    PopUp_Tile_Set(popup_tile, sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: CURSOR_MOVED EVENT --
void fsm_state_CMd_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t mover_entity,
                                struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;

    Game_PopUp_Unit_Place(sota, pos);
    Game_PopUp_Tile_Place(sota, pos);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMd_STANDBY(struct Game *sota, tnecs_entity_t mover_entity,
                               struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    // SDL_assert(sota->moved_direction > -1);
    SDL_assert(sota->entity_cursor != TNECS_NULL);
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    struct Point pos          = cursor_pos->tilemap_pos;
    struct Point previous_pos = sota->cursor_lastpos;
    int previous_i = previous_pos.y * sota->map->col_len + previous_pos.x;
    int current_i  = pos.y * sota->map->col_len + pos.x;
    tnecs_entity_t unit_entity_previoustile = sota->map->unitmap[previous_i];
    // NOTE: unit_entity_previoustile might be different than selected_unit_entity
    //     because
    tnecs_entity_t ontile = sota->map->unitmap[current_i];
    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        *data1_entity = unit_entity_previoustile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Dehovers_Unit, data1_entity, NULL);
    }
    if (ontile != TNECS_NULL) {
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
    }

    sota->cursor_lastpos.x = pos.x;
    sota->cursor_lastpos.y = pos.y;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMd_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t mover_entity,
                                      struct Point *cursor_move) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    // SDL_assert(sota->moved_direction > -1);
    struct Point previous_pos;

    struct Point moved = Ternary_Moved(sota->moved_direction);
    previous_pos.x = pos.x + moved.x;
    previous_pos.y = pos.y + moved.y;

    int previous_i = previous_pos.y * sota->map->col_len + previous_pos.x;
    int current_i = pos.y * sota->map->col_len + pos.x;
    tnecs_entity_t unit_entity_previoustile = sota->map->unitmap[previous_i];
    tnecs_entity_t ontile = sota->map->unitmap[current_i];

    /* unit hovering/dehovering */
    if (unit_entity_previoustile != TNECS_NULL) {
        *data1_entity = unit_entity_previoustile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Dehovers_Unit, data1_entity, NULL);
    }
    if (ontile != TNECS_NULL) {
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_CMd_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t mover_entity,
                                      struct Point *cursor_pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Unit follows cursor movement  -- */
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    selected_pos->tilemap_pos.x = cursor_pos->x;
    selected_pos->tilemap_pos.y = cursor_pos->y;

    selected_pos->pixel_pos.x = selected_pos->tilemap_pos.x * selected_pos->scale[0];
    selected_pos->pixel_pos.y = selected_pos->tilemap_pos.y * selected_pos->scale[1];

    /* -- Move arrow -- */
    struct Arrow *arrow = sota->map->arrow;

    Arrow_Path_Add(arrow, cursor_pos->x, cursor_pos->y);

    /* -- Update map_unit loop to follow arrow direction -- */
    struct Sprite *sprite;
    sprite = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Sprite);
    int path_num = DARR_NUM(arrow->pathlist) / TWO_D;

    if ((path_num > 0) && (sprite->spritesheet != NULL)) {
        i32 previous_x = arrow->pathlist[(path_num - 2) * TWO_D];
        i32 previous_y = arrow->pathlist[((path_num - 2) * TWO_D) + 1];
        SDL_assert(sprite->spritesheet->loop_num == MAP_UNIT_SPRITE_LOOP_NUM);

        struct Point move = {cursor_pos->x - previous_x, cursor_pos->y - previous_y};
        int direction = Ternary_Direction(move);
        int loop      = Utilities_Loop(direction, sprite->flip);
        Spritesheet_Loop_Set(sprite->spritesheet, loop, sprite->flip);
        Sprite_Dstrect_Relative(sprite, &selected_pos->pixel_pos, &sota->camera);
        Sprite_Animation_Loop(sprite);
        Sprite_Draw(sprite, sota->renderer);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: Gameplay_Return2Standby EVENT --
void fsm_substate_GR2S_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t ent) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    if (sota->selected_unit_entity > 0)
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Icon_Return, NULL, NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_state_GR2S_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t controller_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->selected_unit_entity > TNECS_NULL) {

        *data1_entity = controller_entity;
        *data2_entity = sota->selected_unit_entity;
        if (sota->selected_unit_entity > 0)
            Event_Emit(__func__, SDL_USEREVENT, event_Unit_Deselect, data1_entity, data2_entity);
    }

    sota->map->show_overlay = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_GR2S_GAMEPLAY_CAMP(struct Game *sota, tnecs_entity_t controller_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: INPUT_ACCEPT EVENT --
void fsm_state_IA_GAMEPLAY_MAP(struct Game *sota, tnecs_entity_t accepter) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_substate_IA[sota->substate] != NULL)
        fsm_substate_IA[sota->substate](sota, accepter);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// Only fsm_substate_IA_MENU called for other states
void fsm_state_IA_TITLE_SCREEN(struct Game *sota, tnecs_entity_t accepter) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->substate != GAME_SUBSTATE_MENU) {
        SOTA_Log_Debug("Wrong substate %d on Title_Screen state", sota->substate);
        exit(EPERM);
    }
    fsm_substate_IA_MENU(sota, accepter);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Input_Accept -- */
void fsm_substate_IA_MAP_CANDIDATES(struct Game *sota, tnecs_entity_t canceller) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    // /* - Set Defendant to selected unit - */
    // // ONLY FOR ATTACK! HOW TO SWITCH FOR SPECTATOR/PATIENTS?
    // SDL_assert(sota->candidates != NULL);
    // SDL_assert(sota->candidates[sota->candidate] > TNECS_NULL);
    // sota->defendant = sota->candidates[sota->candidate];
    // SDL_assert(sota->defendant > TNECS_NULL);
    // SDL_assert(sota->aggressor > TNECS_NULL);

    // /* - Make cursor invisible - */
    // struct Sprite * sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    // sprite->visible = false;

    // /* - Send Defendant_Select event - */
    // // ONLY FOR ATTACK! HOW TO SWITCH FOR SPECTATOR/PATIENTS?
    // Event_Emit(__func__, SDL_USEREVENT, event_Defendant_Select, data1_entity, data2_entity);

    if (fsm_map_candidates_IA[sota->selected_menu_option] != NULL)
        fsm_map_candidates_IA[sota->selected_menu_option](sota, NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_STANDBY(struct Game *sota, tnecs_entity_t accepter) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // for state == GAME_STATE_GAMEPLAY
    SDL_assert(sota->entity_cursor);
    struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity_t ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];
    if (ontile != TNECS_NULL) {
        /* -- select unit -- */
        *data2_entity = ontile;
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Select, data1_entity, data2_entity);
    } else {
        /* -- draw map_action menu -- */
        if (sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] == TNECS_NULL) {
            SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] == TNECS_NULL);
            Game_PlayerSelectMenu_Create(sota, MENU_PLAYER_SELECT_MAP_ACTION);
            SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] > TNECS_NULL);
        }
        Game_menuStack_Push(sota, sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION]);
        SDL_assert(sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION] > TNECS_NULL);
        Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_MAP_ACTION);
        Game_cursorFocus_onMenu(sota);
        strncpy(sota->reason, "no unit was selected", sizeof(sota->reason));
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);
        Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->stats_menu, NULL);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_MENU(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Possible state pairs: [?, MENU]

    SDL_assert(DARR_NUM(sota->menu_stack) > 0);
    tnecs_entity_t top_menu = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
    SDL_assert(top_menu > TNECS_NULL);
    struct MenuComponent *mc_topop = TNECS_GET_COMPONENT(sota->world, top_menu, MenuComponent);

    if (fsm_menu_type_IA[mc_topop->type] != NULL)
        fsm_menu_type_IA[mc_topop->type](sota, mc_topop);

    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Select, NULL, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_PAUSED(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Event_Emit(__func__, SDL_USEREVENT, event_Gameplay_Return2Standby, data1_entity, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* - Unit should have been selected - */
    SDL_assert(sota->selected_unit_entity != TNECS_NULL);
    tnecs_entity_t unit_ent = sota->selected_unit_entity;

    /* - Reset potential candidates - */
    sota->candidate = 0;
    sota->candidates = NULL;

    /* - Make popup_tile invisible - */
    tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_TILE];
    struct PopUp *popup      = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    SDL_assert(popup != NULL);
    popup->visible = false;

    /* - Get selected unit on tile - */
    struct Position *accepter_position, *selected_pos;
    struct Unit *unit       = TNECS_GET_COMPONENT(sota->world, unit_ent, Unit);
    selected_pos            = TNECS_GET_COMPONENT(sota->world, unit_ent, Position);
    accepter_position       = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    /* - Unit should be PC - */
    SDL_assert(SotA_isPC(unit->army));

    /* -- Creating Unit Action Menu -- */
    tnecs_entity_t *menu = &sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
    if (*menu == 0)
        Game_PlayerSelectMenu_Create(sota, MENU_PLAYER_SELECT_UNIT_ACTION);
    Game_menuStack_Push(sota, *menu);

    /* - Send event_Menu_Created Event to set substate - */
    strncpy(sota->reason, "friendly unit has moved, time to choose action", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, menu, NULL);
    sota->cursor_lastpos.x = accepter_position->tilemap_pos.x;
    sota->cursor_lastpos.y = accepter_position->tilemap_pos.y;

    /* - Moving unit to new tile - */
    sota->selected_unit_moved_position.x    = accepter_position->tilemap_pos.x;
    sota->selected_unit_moved_position.y    = accepter_position->tilemap_pos.y;
    struct Point initial      = sota->selected_unit_initial_position;
    struct Point moved        = sota->selected_unit_moved_position;
    if ((initial.x != moved.x) || (initial.y != moved.y))
        Map_Unit_Move(sota->map, initial.x, initial.y, moved.x, moved.y);

    Position_Pos_Set(selected_pos, moved.x, moved.y);

    /* - Compute new stackmap with recomputed attacktomap - */
    Map_Healtomap_Compute(sota->map,   sota->world, unit_ent, false, false);
    Map_Attacktomap_Compute(sota->map, sota->world, unit_ent, false, false);

    int rangemap = Unit_Rangemap_Get(unit);
    if (rangemap == RANGEMAP_HEALMAP) {
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_HEAL);
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        Map_Palettemap_Autoset(sota->map,
                               MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_MOVE + MAP_OVERLAY_ATTACK);
    }
    Map_Stacked_Dangermap_Compute(sota->map);

    /* - Pre-menu update computation for content - */
    Game_preUnitAction_Targets(sota, unit_ent);

    /* - Update menu to create content - */
    Game_PlayerSelectMenu_Update(sota, MENU_PLAYER_SELECT_UNIT_ACTION);

    /* - Focusing cursor on Menu - */
    Game_cursorFocus_onMenu(sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_MAP_MINIMAP(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_ANIMATION(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_MAP_COMBAT(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_IA_MAP_NPCTURN(struct Game *sota, tnecs_entity_t accepter_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// void fsm_substate_IA_MAP_GLOBAL_RANGE(struct Game * sota,
//         tnecs_entity_t accepter_entity) {
//     SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
//     SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
// }

/* Input_Stats */
void fsm_sub_event_IS_STANDBY(struct Game *sota, tnecs_entity_t accepter) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor);
    struct Position *cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity_t ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];
    if (ontile > TNECS_NULL)
        Game_StatsMenu_Enable(sota, ontile);
    else {
        /* -- TODO: draw a map marker here, like tear ring saga -- */
        SOTA_Log_Debug("NO UNIT ON TILE");
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: UNIT_SELECT EVENT --
void fsm_substate_US_STANDBY(struct Game *sota, tnecs_entity_t selector_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *selected_unit;
    selected_unit = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Unit);
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    sota->selected_unit_initial_position.x = selected_pos->tilemap_pos.x;
    sota->selected_unit_initial_position.y = selected_pos->tilemap_pos.y;

    if (!SotA_isPC(selected_unit->army)) {
        /* - Enemy unit was selected - */
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Danger, data1_entity, data2_entity);
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    if (!selected_unit->waits) {
        /* - Friendly unit was selected and can move - */
        sota->aggressor             = sota->selected_unit_entity;

        /* Make popup_unit invisible */
        // TODO: GO OFFSCREEN
        tnecs_entity_t popup_ent = sota->popups[POPUP_TYPE_HUD_UNIT];
        struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
        SDL_assert(popup != NULL);
        popup->visible = false;
        Event_Emit(__func__, SDL_USEREVENT, event_Unit_Moves, data1_entity, data2_entity);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: UNIT_DESELECT EVENT --
void fsm_substate_UD_MENU(struct Game *sota, tnecs_entity_t selector) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    strncpy(sota->reason, "Unit was deselected during menu (unit waits?)", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_UD_MAP_UNIT_MOVES(struct Game *sota, tnecs_entity_t selector) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /*  -- Hide arrow -- */
    sota->map->arrow->show = false;

    /*  -- Reset map overlay to danger only -- */
    struct Position *selected_pos;
    selected_pos = TNECS_GET_COMPONENT(sota->world, sota->selected_unit_entity, Position);
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, selector, Position);
    struct Point initial = sota->selected_unit_initial_position;
    if ((pos->tilemap_pos.x != initial.x) || (pos->tilemap_pos.y != initial.y)) {
        // Only if cursor not on unit.
        // If cursor is on unit, movemap and attackmap should be shown
        Map_Palettemap_Reset(sota->map);
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_GLOBAL_DANGER + MAP_OVERLAY_DANGER);
        Map_Stacked_Dangermap_Reset(sota->map);
    }

    strncpy(sota->reason, "Unit was deselected during movement", sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: INPUT_MENURIGHT EVENT --
void fsm_state_ImR_GAMEPLAY_MAP(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_sub_event_ImR[sota->substate] != NULL)
        fsm_sub_event_ImR[sota->substate](sota, controller_type);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_ImR_MENU(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Pop previous menu -- */
    bool destroy = false;
    tnecs_entity_t popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct MenuComponent *mc_popped = TNECS_GET_COMPONENT(sota->world, popped, MenuComponent);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity_t ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

    /* - Determine which menu is the next one - */
    SDL_assert((mc_popped->type == MENU_TYPE_STATS) || (mc_popped->type == MENU_TYPE_GROWTHS));
    if32 current_id = stats_menu_cycle_inv[mc_popped->type];
    if32 new_id = (current_id == (STATS_MENU_CYCLE_NUM - 1)) ? 0 : ++current_id;
    struct MenuComponent *new_menu_comp;
    switch (stats_menu_cycle[new_id]) {
        case MENU_TYPE_STATS:
            SOTA_Log_Debug("NEW MENU_TYPE_STATS");
            Game_StatsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, MenuComponent);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            SOTA_Log_Debug("NEW MENU_TYPE_GROWTHS");
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->GM_menu, MenuComponent);
            new_menu_comp->visible = true;
            break;
        default:
            SDL_assert(false);
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_ImR_STANDBY(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Zoom in to sprite_atorigin -- */
    const struct Sprite *sprite_atorigin;
    const struct Sprite *cursor_sprite;
    const struct Sprite *mouse_sprite;
    cursor_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    mouse_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(cursor_sprite != NULL);
    SDL_assert(mouse_sprite != NULL);

    /* - Choose which sprite to zoom into - */
    switch (controller_type) {
        case CONTROLLER_GAMEPAD:
            sprite_atorigin = cursor_sprite;
        case CONTROLLER_MOUSE:
            sprite_atorigin = mouse_sprite;
        default:
            sprite_atorigin = cursor_sprite;
    }
    /* - Zoom in - */
    struct Camera *cam = &sota->camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom + CAMERA_INCREMENT;
    cam->zoom = new_zoom < MAX_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    sota->map->camera_moved = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// -- FSM: INPUT_MENULEFT EVENT --
void fsm_state_ImL_GAMEPLAY_MAP(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_sub_event_ImL[sota->substate] != NULL)
        fsm_sub_event_ImL[sota->substate](sota, controller_type);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_ImL_MENU(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Pop previous menu -- */
    bool destroy = false;
    tnecs_entity_t popped = Game_menuStack_Pop(sota, destroy);
    SDL_assert(popped > 0);

    /* - Hide previous menu - */
    struct MenuComponent *mc_popped = TNECS_GET_COMPONENT(sota->world, popped, MenuComponent);
    mc_popped->visible = false;

    /* -- Create New menu -- */
    /* - Get unit ontile - */
    const struct Position *cursor_pos;
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    struct Point pos = cursor_pos->tilemap_pos;
    tnecs_entity_t ontile = sota->map->unitmap[pos.y * sota->map->col_len + pos.x];

    /* - Determine which menu is the next one - */
    SDL_assert((mc_popped->type == MENU_TYPE_STATS) || (mc_popped->type == MENU_TYPE_GROWTHS));
    if32 current_id = stats_menu_cycle_inv[mc_popped->type];
    if32 new_id = (current_id <= 0) ? (STATS_MENU_CYCLE_NUM - 1) : --current_id;
    struct MenuComponent *new_menu_comp;
    switch (stats_menu_cycle[new_id]) {
        case MENU_TYPE_STATS:
            SOTA_Log_Debug("NEW MENU_TYPE_STATS");
            Game_StatsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, MenuComponent);
            new_menu_comp->visible = true;
            break;
        case MENU_TYPE_GROWTHS:
            SOTA_Log_Debug("NEW MENU_TYPE_GROWTHS");
            Game_GrowthsMenu_Enable(sota, ontile);
            new_menu_comp = TNECS_GET_COMPONENT(sota->world, sota->GM_menu, MenuComponent);
            new_menu_comp->visible = true;
            break;
        default:
            SDL_assert(false);
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_sub_event_ImL_STANDBY(struct Game *sota, i32 controller_type) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Zoom out to sprite_atorigin -- */
    const struct Sprite *cursor_sprite;
    const struct Sprite *mouse_sprite;
    const struct Sprite *sprite_atorigin;
    cursor_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Sprite);
    mouse_sprite = TNECS_GET_COMPONENT(sota->world, sota->entity_mouse, Sprite);
    SDL_assert(cursor_sprite != NULL);
    SDL_assert(mouse_sprite != NULL);

    /* - Choose which sprite to zoom into - */
    switch (controller_type) {
        case CONTROLLER_GAMEPAD:
            sprite_atorigin = cursor_sprite;
        case CONTROLLER_MOUSE:
            sprite_atorigin = mouse_sprite;
        default:
            sprite_atorigin = cursor_sprite;
    }
    /* - Zoom out - */
    struct Camera *cam = &sota->camera;
    struct SDL_Rect dstrect = sprite_atorigin->dstrect;
    float previous_zoom = cam->zoom;
    float new_zoom = cam->zoom - CAMERA_INCREMENT;
    cam->zoom = new_zoom > MIN_CAMERA_ZOOM ? new_zoom : cam->zoom;
    cam->offset.x = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.x, dstrect.x);
    cam->offset.y = SOTA_ZOOMTOPOINT(cam->zoom / previous_zoom, cam->offset.y, dstrect.y);
    sota->map->camera_moved = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
