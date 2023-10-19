
#include "fps_fsm.h"

fsm_main_t fsm_C_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START */          NULL,
    /* GAME_STATE_Combat */         &fsm_C_sCmbt,
    /* GAME_STATE_Scene_Talk */     &fsm_C_sScnTalk,
    /* GAME_STATE_Scene_FMV */      &fsm_C_sScnFMV,
    /* GAME_STATE_Gameplay_Map */   &fsm_C_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */  &fsm_C_sGmpCamp,
    /* GAME_STATE_Preparation */    &fsm_C_sPrep,
    /* GAME_STATE_Title_Screen */   &fsm_C_sTtlScrn,
    /* GAME_STATE_Animation */      NULL
};

fsm_main_t fsm_R_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START */          NULL,
    /* GAME_STATE_Combat */         &fsm_R_sCmbt,
    /* GAME_STATE_Scene_Talk */     &fsm_R_sScnTalk,
    /* GAME_STATE_Scene_FMV */      &fsm_R_sScnFMV,
    /* GAME_STATE_Gameplay_Map */   &fsm_R_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */  &fsm_R_sGmpCamp,
    /* GAME_STATE_Preparation */    &fsm_R_sPrep,
    /* GAME_STATE_Title_Screen */   &fsm_R_sTtlScrn,
    /* GAME_STATE_Animation */      NULL,
};

fsm_main_t fsm_C_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START */           NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP */     &fsm_C_sGmpMap_ssMap_Minimap,
    /* GAME_SUBSTATE_MENU */            &fsm_C_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */  &fsm_C_sGmpMap_ssMap_Unit_Moves,
    /* GAME_SUBSTATE_MAP_COMBAT */      &fsm_C_sGmpMap_ssMap_Combat,
    /* GAME_SUBSTATE_MAP_NPCTURN */     &fsm_C_sGmpMap_ssMap_NPCTurn,
    /* GAME_SUBSTATE_SAVING */          &fsm_C_sGmpMap_ssSaving,
    /* GAME_SUBSTATE_STANDBY */         &fsm_C_sGmpMap_ssStandby,
    /* GAME_SUBSTATE_PAUSED */          &fsm_C_sGmpMap_ssPaused,
    /* GAME_SUBSTATE_MAP_CANDIDATES */  &fsm_C_sGmpMap_ssMap_Candidates,
    /* GAME_SUBSTATE_CUTSCENE */        NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION */   &fsm_C_sGmpMap_ssMap_Animation,
};

fsm_main_t fsm_R_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START */           NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP */     &fsm_R_sGmpMap_ssMap_Minimap,
    /* GAME_SUBSTATE_MENU */            &fsm_R_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */  &fsm_R_sGmpMap_ssMap_Unit_Moves,
    /* GAME_SUBSTATE_MAP_COMBAT */      &fsm_R_sGmpMap_ssMap_Combat,
    /* GAME_SUBSTATE_MAP_NPCTURN */     &fsm_R_sGmpMap_ssMap_NPCTurn,
    /* GAME_SUBSTATE_SAVING */          &fsm_R_sGmpMap_ssSaving,
    /* GAME_SUBSTATE_STANDBY */         &fsm_R_sGmpMap_ssStandby,
    /* GAME_SUBSTATE_PAUSED */          &fsm_R_sGmpMap_ssPaused,
    /* GAME_SUBSTATE_MAP_CANDIDATES */  &fsm_R_sGmpMap_ssMap_Defendants,
    /* GAME_SUBSTATE_CUTSCENE */        NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION */   &fsm_R_sGmpMap_ssMap_Animation,
};

/* --- CONTROL ---*/
/* -- substates --*/
void fsm_C_sGmpMap_ssMap_Minimap(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssMenu(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_CursorfollowsMouse_onMenu(sota); /* CONTROL */
    Game_Cursor_Moves_onMenu(sota); /* CONTROL */
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssMap_Unit_Moves(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_CursorfollowsMouse_onMap(sota);  /* CONTROL */
    Game_Cursor_Moves_onMap(sota); /* CONTROL */
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
void fsm_C_sGmpMap_ssMap_Combat(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssMap_NPCTurn(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssSaving(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssStandby(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_CursorfollowsMouse_onMap(sota);  /* CONTROL */
    Game_Cursor_Moves_onMap(sota); /* CONTROL */
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssPaused(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssMap_Candidates(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Cursor_Next_Candidate(sota);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap_ssMap_Animation(struct Game *sota) {
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->combat_forecast.attack_num > 0);

    /* - Skip if not map_animation, cause it might have been deleted - */
    if (sota->map_animation == TNECS_NULL) {
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* - getting attacker - */
    struct CombatAnimation *map_anim;
    struct Timer *combat_timer;
    combat_timer = TNECS_GET_COMPONENT(sota->world, sota->map_animation, Timer);
    SDL_assert(combat_timer != NULL);
    map_anim = TNECS_GET_COMPONENT(sota->world, sota->map_animation, CombatAnimation);
    SDL_assert(map_anim != NULL);

    if (map_anim->attack_ind >= sota->combat_forecast.attack_num) {
        /* - Check for remaining attack, ending combat after pause - */
        bool paused = ((combat_timer->time_ns / SOTA_ms) < map_anim->pause_after_ms);
        if (!paused) {
            tnecs_entity_destroy(sota->world, sota->map_animation);
            sota->map_animation = TNECS_NULL;
            Event_Emit(__func__, SDL_USEREVENT, event_Combat_End, NULL, NULL);
        }
        /* - Skip if no more attacks to animate - */
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* - pausing attacker - */
    int attacker_i = sota->combat_phases[map_anim->attack_ind].attacker;
    tnecs_entity_t attacker = attacker_i ? sota->aggressor : sota->defendant;

    struct Timer *att_timer = TNECS_GET_COMPONENT(sota->world, attacker, Timer);
    SDL_assert(att_timer != NULL);
    att_timer->paused = ((combat_timer->time_ns / SOTA_us) < map_anim->pause_before_ms);
    if (att_timer->paused) {
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* - Add RenderTop component to attacker - */
    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop)) {
        TNECS_ADD_COMPONENT(sota->world, attacker, RenderTop);
    }

    /* - map_anim's frame count only grows - */
    struct Sprite *att_sprite = TNECS_GET_COMPONENT(sota->world, attacker, Sprite);
    SDL_assert(att_sprite != NULL);
    int current_frame = att_sprite->spritesheet->current_frame;
    int frame_count = map_anim->frame_count;
    map_anim->frame_count = current_frame > frame_count ? current_frame : frame_count;

    /* - Checking if map_anim->attack_ind should be incremented - */
    if (att_timer->paused || (map_anim->frame_count == 0) || (current_frame > 0)) {
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    map_anim->attack_ind++;
    Event_Emit(__func__, SDL_USEREVENT, event_Increment_Attack, NULL, NULL);

    /* - reset combat timer for next attack - */
    combat_timer->time_ns = 0;
    /* - reset frame_count for next attack - */
    map_anim->frame_count = 0;
    att_timer->paused = true;

    /* - pause defender - */
    tnecs_entity_t defender = attacker_i ? sota->defendant : sota->aggressor;
    struct Timer *def_timer = TNECS_GET_COMPONENT(sota->world, defender, Timer);
    SDL_assert(def_timer != NULL);
    def_timer->paused = true;

    /* - Remove RenderTop component from attacker - */
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, attacker, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, attacker, RenderTop);
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, defender, RenderTop))
        TNECS_REMOVE_COMPONENTS(sota->world, defender, RenderTop);

    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- states --*/
void fsm_C_sCmbt(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sScnTalk(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sScnFMV(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpMap(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Camera_Scroll(sota); /* CONTROL */
    /* RENDER only one map -> no entity */
    fsm_C_sGmpMap_ss[sota->substate](sota);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sGmpCamp(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sPrep(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sTtlScrn(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    fsm_C_sGmpMap_ss[sota->substate](sota);
    // Game_CursorfollowsMouse_onMenu(sota);  /* CONTROL */
    // Game_Cursor_Moves_onMenu(sota);
    /* - Title - */
    /* -  Background - */
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_C_sAnimation(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Control not in the hands of the Player EXCEPT to go faster or cancel animation

    // TO PLACE: switch to Animation state event
    //  - set animation_attack to 0
    //  - set animation frame to 0

    // Run animations until (animation_attack == combat_forecast.attack_num)
    //  if equal to last frame/0,
    //      increment animation_attack
    //      check what is the animation state
    //      PAUSE for a certain time
    //      START ANIMATION

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- RENDER ---*/
/* -- substates --*/
void fsm_R_sGmpMap_ssMap_Minimap(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssMenu(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssMap_Unit_Moves(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
void fsm_R_sGmpMap_ssMap_Combat(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssMap_NPCTurn(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssSaving(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssStandby(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssPaused(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssMap_Defendants(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpMap_ssMap_Animation(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- states --*/
void fsm_R_sAnimation(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sCmbt(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sScnTalk(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sScnFMV(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);

}

void fsm_R_sGmpMap(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* RENDER only one map -> no entity */
    Map_Draw(sota->map, &sota->settings, &sota->camera, sota->render_target);
    Map_Grid_Draw(sota->map, &sota->settings, &sota->camera);
    fsm_R_sGmpMap_ss[sota->substate](sota);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sGmpCamp(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sPrep(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_R_sTtlScrn(struct Game *sota) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
