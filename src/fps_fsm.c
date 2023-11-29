
#include "fps_fsm.h"

fsm_main_t fsm_cFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START         */ NULL,
    /* GAME_STATE_Combat        */ &fsm_cFrame_sCmbt,
    /* GAME_STATE_Scene_Talk    */ &fsm_cFrame_sScnTalk,
    /* GAME_STATE_Scene_FMV     */ &fsm_cFrame_sScnFMV,
    /* GAME_STATE_Gameplay_Map  */ &fsm_cFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */ &fsm_cFrame_sGmpCamp,
    /* GAME_STATE_Preparation   */ &fsm_cFrame_sPrep,
    /* GAME_STATE_Title_Screen  */ &fsm_cFrame_sTtlScrn,
    /* GAME_STATE_Animation     */ NULL
};

fsm_main_t fsm_rFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START         */ NULL,
    /* GAME_STATE_Combat        */ &fsm_rFrame_sCmbt,
    /* GAME_STATE_Scene_Talk    */ &fsm_rFrame_sScnTalk,
    /* GAME_STATE_Scene_FMV     */ &fsm_rFrame_sScnFMV,
    /* GAME_STATE_Gameplay_Map  */ &fsm_rFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */ &fsm_rFrame_sGmpCamp,
    /* GAME_STATE_Preparation   */ &fsm_rFrame_sPrep,
    /* GAME_STATE_Title_Screen  */ &fsm_rFrame_sTtlScrn,
    /* GAME_STATE_Animation     */ NULL,
};

fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START          */ NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP    */ &fsm_cFrame_sGmpMap_ssMapMini,
    /* GAME_SUBSTATE_MENU           */ &fsm_cFrame_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */ &fsm_cFrame_sGmpMap_ssMapUnitMv,
    /* GAME_SUBSTATE_MAP_COMBAT     */ &fsm_cFrame_sGmpMap_ssMapCmbt,
    /* GAME_SUBSTATE_MAP_NPCTURN    */ &fsm_cFrame_sGmpMap_ssMapNPC,
    /* GAME_SUBSTATE_SAVING         */ &fsm_cFrame_sGmpMap_ssSave,
    /* GAME_SUBSTATE_STANDBY        */ &fsm_cFrame_sGmpMap_ssStby,
    /* GAME_SUBSTATE_PAUSED         */ &fsm_cFrame_sGmpMap_ssPause,
    /* GAME_SUBSTATE_MAP_CANDIDATES */ &fsm_cFrame_sGmpMap_ssMapCndt,
    /* GAME_SUBSTATE_CUTSCENE       */ NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION  */ &fsm_cFrame_sGmpMap_ssMapAnim,
};

fsm_main_t fsm_rFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START */           NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP */     &fsm_rFrame_sGmpMap_ssMapMini,
    /* GAME_SUBSTATE_MENU */            &fsm_rFrame_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */  &fsm_rFrame_sGmpMap_ssMapUnitMv,
    /* GAME_SUBSTATE_MAP_COMBAT */      &fsm_rFrame_sGmpMap_ssMapCmbt,
    /* GAME_SUBSTATE_MAP_NPCTURN */     &fsm_rFrame_sGmpMap_ssMapNPC,
    /* GAME_SUBSTATE_SAVING */          &fsm_rFrame_sGmpMap_ssSave,
    /* GAME_SUBSTATE_STANDBY */         &fsm_rFrame_sGmpMap_ssStby,
    /* GAME_SUBSTATE_PAUSED */          &fsm_rFrame_sGmpMap_ssPause,
    /* GAME_SUBSTATE_MAP_CANDIDATES */  &fsm_rFrame_sGmpMap_ssMapCndt,
    /* GAME_SUBSTATE_CUTSCENE */        NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION */   &fsm_rFrame_sGmpMap_ssMapAnim,
};

/* --- CONTROL ---*/
/* -- substates --*/
void fsm_cFrame_sGmpMap_ssMapMini(struct Game *sota) {

}

void fsm_cFrame_sGmpMap_ssMenu(struct Game *sota) {
    Game_CursorfollowsMouse_onMenu(sota); /* CONTROL */
    Game_Cursor_Moves_onMenu(sota); /* CONTROL */
}

void fsm_cFrame_sGmpMap_ssMapUnitMv(struct Game *sota) {
    Game_CursorfollowsMouse_onMap(sota);  /* CONTROL */
    Game_Cursor_Moves_onMap(sota); /* CONTROL */
}
void fsm_cFrame_sGmpMap_ssMapCmbt(struct Game *sota) {

}

/* --- AI CONTORL HAPPENS HERE --- */
void fsm_cFrame_sGmpMap_ssMapNPC(struct Game *sota) {
    /* --- AI CONTROL --- */

    /* -- Reinforcements timer: pause before moving units -- */
    // TODO: Animate reinforcements
    if (sota->reinf_timer != TNECS_NULL) {
        struct Timer *timer = TNECS_GET_COMPONENT(sota->world, sota->reinf_timer, Timer);
        SDL_assert(timer != NULL);
        u64 limit = sota->settings.enemy_turn_settings.pause_post_reinforcement;
        if (timer->time_ns <= limit)
            return;

        tnecs_entity_destroy(sota->world, sota->reinf_timer);
        sota->reinf_timer = TNECS_NULL;
    }

    /* -- Skip if turn is over -- */
    if (sota->ai_state.turn_over)
        return;

    /* -- Build list of npcs to control -- */
    if (sota->ai_state.init == false) {
        SDL_LogDebug(SOTA_LOG_AI, "Building NPC list");
        AI_State_Init(&sota->ai_state, sota->world, sota->map);
    }
    SDL_assert(sota->ai_state.npcs != NULL);

    /* -- Decide next NPC to act -- */
    if (sota->ai_state.npc_i < 0) {
        entity debug = AI_Decide_Next(sota);
        SDL_LogDebug(SOTA_LOG_AI, "Next npc entity: %d", debug);
    }
    tnecs_entity npc_ent = sota->ai_state.npcs[sota->ai_state.npc_i];

    SDL_assert(npc_ent != TNECS_NULL);
    /* -- AI decides what to do with unit -- */
    // If not previously decided for npc_ent, decide
    b32 decided     = sota->ai_state.decided;
    if (!decided) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Decide");
        AI_Decide_Action(sota, npc_ent, &sota->ai_state.action);
        AI_Decide_Move(  sota, npc_ent, &sota->ai_state.action);
        sota->ai_state.decided = true;
    }

    decided         = sota->ai_state.decided;
    b32 act_anim    = sota->ai_state.act_anim;
    b32 move_anim   = sota->ai_state.move_anim;

    /* -- AI moves unit -- */
    if (decided && !move_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Move");
        SDL_assert(!act_anim);
        AI_Move(sota, npc_ent, &sota->ai_state.action);
        // TODO: Move animation
        sota->ai_state.move_anim = true;
    }

    /* Check if move_anim updated during frame */
    move_anim   = sota->ai_state.move_anim;
    act_anim    = sota->ai_state.act_anim;

    /* -- AI acts unit -- */
    if (decided && move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Act");
        AI_Act(sota, npc_ent, &sota->ai_state.action);
        // TODO: Act animation
        sota->ai_state.act_anim = true;
    }

    /* Check if act_anim updated during frame */
    act_anim    = sota->ai_state.act_anim;

    /* -- Pop unit from list in AI_State -- */
    if (act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Pop");
        AI_State_Pop(&sota->ai_state, sota->world);
    }

    /* -- If no more NPCs, end NPC turn. -- */
    if (DARR_NUM(sota->ai_state.npcs) < 1) {
        SDL_Log("AI Turn Finished");
        AI_State_Turn_Finish(&sota->ai_state);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, NULL, NULL);
    }
}

void fsm_cFrame_sGmpMap_ssSave(struct Game *sota) {

}

void fsm_cFrame_sGmpMap_ssStby(struct Game *sota) {
    Game_CursorfollowsMouse_onMap(sota);  /* CONTROL */
    Game_Cursor_Moves_onMap(sota); /* CONTROL */
}

void fsm_cFrame_sGmpMap_ssPause(struct Game *sota) {

}

void fsm_cFrame_sGmpMap_ssMapCndt(struct Game *sota) {
    Game_Cursor_Next_Candidate(sota);
}

/* Map animation for units fighting on map */
void fsm_cFrame_sGmpMap_ssMapAnim(struct Game *sota) {
    // Should do nothing, animation is a system

}

/* -- states --*/
void fsm_cFrame_sCmbt(struct Game *sota) {
}

void fsm_cFrame_sScnTalk(struct Game *sota) {
}

void fsm_cFrame_sScnFMV(struct Game *sota) {
}

void fsm_cFrame_sGmpMap(struct Game *sota) {
    Game_Camera_Scroll(sota); /* CONTROL */
    /* RENDER only one map -> no entity */
    fsm_cFrame_sGmpMap_ss[sota->substate](sota);
}

void fsm_cFrame_sGmpCamp(struct Game *sota) {
}

void fsm_cFrame_sPrep(struct Game *sota) {
}

void fsm_cFrame_sTtlScrn(struct Game *sota) {
    fsm_cFrame_sGmpMap_ss[sota->substate](sota);
    // Game_CursorfollowsMouse_onMenu(sota);  /* CONTROL */
    // Game_Cursor_Moves_onMenu(sota);
    /* - Title - */
    /* -  Background - */
}

void fsm_cFrame_sAnimation(struct Game *sota) {
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

}

/* --- RENDER ---*/
/* -- substates --*/
void fsm_rFrame_sGmpMap_ssMapMini(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssMenu(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssMapUnitMv(struct Game *sota) {

}
void fsm_rFrame_sGmpMap_ssMapCmbt(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssMapNPC(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssSave(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssStby(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssPause(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssMapCndt(struct Game *sota) {

}

void fsm_rFrame_sGmpMap_ssMapAnim(struct Game *sota) {

}

/* -- states --*/
void fsm_rFrame_sAnimation(struct Game *sota) {

}

void fsm_rFrame_sCmbt(struct Game *sota) {

}

void fsm_rFrame_sScnTalk(struct Game *sota) {

}

void fsm_rFrame_sScnFMV(struct Game *sota) {

}

void fsm_rFrame_sGmpMap(struct Game *sota) {
    /* RENDER only one map -> no entity */
    Map_Draw(sota->map, &sota->settings, &sota->camera, sota->render_target);
    Map_Grid_Draw(sota->map, &sota->settings, &sota->camera);
    fsm_rFrame_sGmpMap_ss[sota->substate](sota);
}

void fsm_rFrame_sGmpCamp(struct Game *sota) {
}

void fsm_rFrame_sPrep(struct Game *sota) {
}

void fsm_rFrame_sTtlScrn(struct Game *sota) {
}
