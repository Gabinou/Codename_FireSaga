
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
    /* GAME_STATE_Combat        */ NULL,
    /* GAME_STATE_Scene_Talk    */ NULL,
    /* GAME_STATE_Scene_FMV     */ NULL,
    /* GAME_STATE_Gameplay_Map  */ &fsm_cFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */ NULL,
    /* GAME_STATE_Preparation   */ NULL,
    /* GAME_STATE_Title_Screen  */ NULL,
    /* GAME_STATE_Animation     */ NULL
};

const fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START          */ NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP    */ NULL,
    /* GAME_SUBSTATE_MENU           */ NULL,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */ NULL,
    /* GAME_SUBSTATE_MAP_COMBAT     */ NULL,
    /* GAME_SUBSTATE_MAP_NPCTURN    */ &fsm_cFrame_sGmpMap_ssMapNPC,
    /* GAME_SUBSTATE_SAVING         */ NULL,
    /* GAME_SUBSTATE_STANDBY        */ NULL,
    /* GAME_SUBSTATE_MAP_CANDIDATES */ NULL,
    /* GAME_SUBSTATE_CUTSCENE       */ NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION  */ NULL,
};

/* --- CONTROL ---*/
//  - What is fsm_cFrame used for?
//      - AI        -> system?

void fsm_cFrame_sGmpMap(              Game *IES) {
    if (fsm_cFrame_sGmpMap_ss[Game_Substate_Current(IES)] != NULL) {
        fsm_cFrame_sGmpMap_ss[Game_Substate_Current(IES)](IES);
    }
}

/* --- AI CONTROL HAPPENS HERE --- */
void fsm_cFrame_sGmpMap_ssMapNPC(struct Game *IES) {
    Game_AI_Enemy_Turn(IES);
}

/* --- RENDER ---*/
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

// Design: make into systems? -> YES
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
    // TODO:  Convert into system
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
