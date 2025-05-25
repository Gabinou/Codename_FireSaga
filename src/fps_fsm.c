
#include "fps_fsm.h"
#include "scene.h"
#include "nmath.h"
#include "game/game.h"
#include "map/map.h"
#include "game/cursor.h"
#include "AI.h"
#include "map/render.h"
#include "events.h"
#include "log.h"

const fsm_main_t fsm_cFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START         */ NULL,
    /* GAME_STATE_Combat        */ &fsm_cFrame_sCmbt,
    /* GAME_STATE_Scene_Talk    */ &fsm_cFrame_sScnTalk,
    /* GAME_STATE_Scene_FMV     */ &fsm_cFrame_sCutScn,
    /* GAME_STATE_Gameplay_Map  */ &fsm_cFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */ &fsm_cFrame_sGmpCamp,
    /* GAME_STATE_Preparation   */ &fsm_cFrame_sPrep,
    /* GAME_STATE_Title_Screen  */ &fsm_cFrame_sTtlScrn,
    /* GAME_STATE_Animation     */ NULL
};

const fsm_main_t fsm_rFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START         */ NULL,
    /* GAME_STATE_Combat        */ &fsm_rFrame_sCmbt,
    /* GAME_STATE_Scene_Talk    */ &fsm_rFrame_sScnTalk,
    /* GAME_STATE_Cutscene      */ &fsm_rFrame_sCutScn,
    /* GAME_STATE_Gameplay_Map  */ &fsm_rFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */ &fsm_rFrame_sGmpCamp,
    /* GAME_STATE_Preparation   */ &fsm_rFrame_sGmpMap,
    /* GAME_STATE_Title_Screen  */ &fsm_rFrame_sTtlScrn,
    /* GAME_STATE_Animation     */ NULL,
};

const fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START          */ NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP    */ &fsm_cFrame_sGmpMap_ssMapMini,
    /* GAME_SUBSTATE_MENU           */ &fsm_cFrame_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */ &fsm_cFrame_sGmpMap_ssMapUnitMv,
    /* GAME_SUBSTATE_MAP_COMBAT     */ &fsm_cFrame_sGmpMap_ssMapCmbt,
    /* GAME_SUBSTATE_MAP_NPCTURN    */ &fsm_cFrame_sGmpMap_ssMapNPC,
    /* GAME_SUBSTATE_SAVING         */ &fsm_cFrame_sGmpMap_ssSave,
    /* GAME_SUBSTATE_STANDBY        */ &fsm_cFrame_sGmpMap_ssStby,
    /* GAME_SUBSTATE_MAP_CANDIDATES */ &fsm_cFrame_sGmpMap_ssMapCndt,
    /* GAME_SUBSTATE_CUTSCENE       */ NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION  */ &fsm_cFrame_sGmpMap_ssMapAnim,
};

const fsm_main_t fsm_cFrame_sPrep_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START          */ NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP    */ NULL,
    /* GAME_SUBSTATE_MENU           */ &fsm_cFrame_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */ NULL,
    /* GAME_SUBSTATE_MAP_COMBAT     */ NULL,
    /* GAME_SUBSTATE_MAP_NPCTURN    */ NULL,
    /* GAME_SUBSTATE_SAVING         */ NULL,
    /* GAME_SUBSTATE_STANDBY        */ NULL,
    /* GAME_SUBSTATE_MAP_CANDIDATES */ &fsm_cFrame_sPrep_ssMapCndt,
    /* GAME_SUBSTATE_CUTSCENE       */ NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION  */ NULL,
};

/* --- CONTROL ---*/
//  - What is fsm_cFrame used for?
//      - Cursor    -> system OK
//      - Camera    -> system OK
//      - AI        -> system?
// Design: make into systems?
//  - Check for game state in relevant system
/* -- substates --*/
void fsm_cFrame_sGmpMap_ssMapMini(struct Game *sota) {

}

void fsm_cFrame_sGmpMap_ssMenu(struct Game *sota) {
    // Game_CursorfollowsMouse_onMenu(sota);   /* CONTROL */
    // Game_Cursor_Moves_onMenu(sota);         /* CONTROL */
}

void fsm_cFrame_sGmpMap_ssMapUnitMv(struct Game *sota) {
    // Game_CursorfollowsMouse_onMap(sota);    /* CONTROL */
    // Game_Cursor_Moves_onMap(sota);          /* CONTROL */
}

void fsm_cFrame_sGmpMap_ssMapCmbt(struct Game *sota) {

}

/* --- AI CONTROL HAPPENS HERE --- */
void fsm_cFrame_sGmpMap_ssMapNPC(struct Game *sota) {
    /* --- AI CONTROL --- */
    // TODO: ai_control entity
    // TODO: Don't check for loss every frame
    if (Map_isLost(sota->map)) {
        // SDL_Log("AI CONTROL -> LOSS");
        Event_Emit(__func__, SDL_USEREVENT, event_Game_Over, NULL, NULL);
        return;
    }

    /* -- Reinforcements timer: pause before moving units -- */
    /* -- TODO: Rename to AI timer? */
    // TODO: Animate reinforcements
    if (sota->timers.reinf != TNECS_NULL) {
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        SDL_assert(timer != NULL);
        // u64 limit = sota->settings.enemy_turn_settings.pause_post_reinforcement;
        if (timer->time_ns <= timer->limit_ns)
            return;

        tnecs_entity_destroy(sota->ecs.world, sota->timers.reinf);
        sota->timers.reinf = TNECS_NULL;
    }

    /* -- Skip if turn is over -- */
    if (sota->state.ai.turn_over)
        return;

    /* -- Build list of npcs to control -- */
    if (sota->state.ai.init == false) {
        SDL_LogDebug(SOTA_LOG_AI, "Building NPC list");
        AI_State_Init(&sota->state.ai, sota->ecs.world, sota->map);
    }
    SDL_assert(sota->state.ai.npcs != NULL);

    /* -- If no more NPCs, end NPC turn. -- */
    if (sota->state.ai.npcs && (DARR_NUM(sota->state.ai.npcs) < 1)) {
        // SDL_Log("AI Turn Finished");
        AI_State_Turn_Finish(&sota->state.ai);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, NULL, NULL);
        return;
    }

    /* -- Decide next NPC to act -- */
    if (sota->state.ai.npc_i < 0) {
        tnecs_entity debug = AI_Decide_Next(sota);
        SDL_LogDebug(SOTA_LOG_AI, "Next npc entity: %lld", debug);
    }
    tnecs_entity npc_ent = sota->state.ai.npcs[sota->state.ai.npc_i];

    SDL_assert(npc_ent != TNECS_NULL);

    /* -- AI decides what to do with unit -- */
    // If not previously decided for npc_ent, decide
    b32 decided     = sota->state.ai.decided;
    if (!decided) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Decide");
        AI_Decide_Action(sota, npc_ent, &sota->state.ai.action);
        AI_Decide_Move(  sota, npc_ent, &sota->state.ai.action);
        sota->state.ai.decided = true;
    }

    decided         = sota->state.ai.decided;
    b32 act_anim    = sota->state.ai.act_anim;
    b32 move_anim   = sota->state.ai.move_anim;

    /* -- AI moves unit -- */
    // TODO: wait on combat to finish!
    if (decided && !move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Move");
        SDL_assert(!act_anim);
        AI_Move(sota, npc_ent, &sota->state.ai.action);
        // TODO: Move animation
        sota->state.ai.move_anim = true;
    }

    /* Check if move_anim updated during frame */
    move_anim   = sota->state.ai.move_anim;
    act_anim    = sota->state.ai.act_anim;

    /* -- AI acts unit -- */
    if (decided && move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Act");
        AI_Act(sota, npc_ent, &sota->state.ai.action);
        // TODO: Act animation
        sota->state.ai.act_anim = true;
    }

    /* Check if act_anim updated during frame */
    act_anim    = sota->state.ai.act_anim;

    /* -- Pop unit from list in AI_State -- */
    if ((act_anim) && ((DARR_NUM(sota->state.ai.npcs) > 0))) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Pop");
        AI_State_Pop(&sota->state.ai, sota->ecs.world);

        SDL_LogDebug(SOTA_LOG_AI, "AI: Pause AFTER AI_act");
        /* Pause AFTER AI action */
        sota->timers.reinf   = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Timer_ID);
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        *timer = Timer_default;
        timer->limit_ns = sota->settings.enemy_turn_settings.pause_post_move;
    }
}

void fsm_cFrame_sGmpMap_ssSave(struct Game *sota) {

}

void fsm_cFrame_sGmpMap_ssStby(struct Game *sota) {
    // Game_CursorfollowsMouse_onMap(sota);    /* CONTROL */
    // Game_Cursor_Moves_onMap(sota);          /* CONTROL */
}

void fsm_cFrame_sGmpMap_ssMapCndt(struct Game *sota) {
    Game_Cursor_Next_Candidate(sota);
}

void fsm_cFrame_sPrep_ssMapCndt(struct Game *sota) {
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

void fsm_cFrame_sCutScn(struct Game *sota) {
}

void fsm_cFrame_sGmpMap(struct Game *sota) {
    fsm_cFrame_sGmpMap_ss[Game_Substate_Current(sota)](sota);
}

void fsm_cFrame_sGmpCamp(struct Game *sota) {

}

void fsm_cFrame_sPrep(struct Game *sota) {
    if (fsm_cFrame_sPrep_ss[Game_Substate_Current(sota)] != NULL)
        fsm_cFrame_sPrep_ss[Game_Substate_Current(sota)](sota);
}

void fsm_cFrame_sTtlScrn(struct Game *sota) {
    fsm_cFrame_sGmpMap_ss[Game_Substate_Current(sota)](sota);
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
// Design: make into systems?
// + Everything render-related is a system
//      + Timing controlled through ECS
// -/+ System-specific fsm/switch e.g for cursor
//      System always runs, but might skip depending on game state
// - Only one entity per system for Camera, Scene, cursor, kinda annoying.
//      - Already some cursor-related stuff

/* -- states --*/
void fsm_rFrame_sAnimation(struct Game *sota) {

}

void fsm_rFrame_sCmbt(struct Game *sota) {

}

void fsm_rFrame_sScnTalk(struct Game *sota) {
    // TODO:  Convert into system?
    //

    SDL_assert(sota->narrative.scene > TNECS_NULL);
    Scene *scene = IES_GET_COMPONENT(sota->ecs.world, sota->narrative.scene, Scene);
    SDL_assert(scene != NULL);

    Scene_Draw(scene, &sota->settings, sota->render.target, sota->render.er);
}

void fsm_rFrame_sCutScn(struct Game *sota) {

}

void fsm_rFrame_sGmpMap(struct Game *sota) {
    /* --- Render Map: only one map --- */
    // TODO:  Convert into system?
    Map_Draw(sota->map, &sota->settings, sota->render.target);
    Map_Grid_Draw(sota->map, &sota->settings);
    Map_Perimeter_Draw_Danger(sota->map, &sota->settings);

    // TODO: Draw weapon auras.
    // void Map_Perimeter_Draw_Aura(struct Map     *map,    struct Settings *settings,
    //                          struct Camera  *camera, struct Point pos,
    //                          struct Range    range,  sota->map->perimiter_aura_color) {

#ifdef DEBUG_SUPPORT_PERIMITER
    /* Draw support auras perimiters for all friendlies */
    Map_Perimeter_Draw_Support(map, settings, sota->ecs.world);
#endif /* DEBUG_SUPPORT_PERIMITER */

}

void fsm_rFrame_sGmpCamp(struct Game *sota) {
}

void fsm_rFrame_sPrep(struct Game *sota) {
}

void fsm_rFrame_sTtlScrn(struct Game *sota) {
}
