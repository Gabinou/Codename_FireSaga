
#include "fps_fsm.h"

fsm_main_t fsm_cFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START */          NULL,
    /* GAME_STATE_Combat */         &fsm_cFrame_sCmbt,
    /* GAME_STATE_Scene_Talk */     &fsm_cFrame_sScnTalk,
    /* GAME_STATE_Scene_FMV */      &fsm_cFrame_sScnFMV,
    /* GAME_STATE_Gameplay_Map */   &fsm_cFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */  &fsm_cFrame_sGmpCamp,
    /* GAME_STATE_Preparation */    &fsm_cFrame_sPrep,
    /* GAME_STATE_Title_Screen */   &fsm_cFrame_sTtlScrn,
    /* GAME_STATE_Animation */      NULL
};

fsm_main_t fsm_rFrame_s[GAME_STATE_NUM] = {
    /* GAME_STATE_START */          NULL,
    /* GAME_STATE_Combat */         &fsm_rFrame_sCmbt,
    /* GAME_STATE_Scene_Talk */     &fsm_rFrame_sScnTalk,
    /* GAME_STATE_Scene_FMV */      &fsm_rFrame_sScnFMV,
    /* GAME_STATE_Gameplay_Map */   &fsm_rFrame_sGmpMap,
    /* GAME_STATE_Gameplay_Camp */  &fsm_rFrame_sGmpCamp,
    /* GAME_STATE_Preparation */    &fsm_rFrame_sPrep,
    /* GAME_STATE_Title_Screen */   &fsm_rFrame_sTtlScrn,
    /* GAME_STATE_Animation */      NULL,
};

fsm_main_t fsm_cFrame_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* GAME_SUBSTATE_START */           NULL,
    /* GAME_SUBSTATE_MAP_MINIMAP */     &fsm_cFrame_sGmpMap_ssMapMini,
    /* GAME_SUBSTATE_MENU */            &fsm_cFrame_sGmpMap_ssMenu,
    /* GAME_SUBSTATE_MAP_UNIT_MOVES */  &fsm_cFrame_sGmpMap_ssMapUnitMv,
    /* GAME_SUBSTATE_MAP_COMBAT */      &fsm_cFrame_sGmpMap_ssMapCmbt,
    /* GAME_SUBSTATE_MAP_NPCTURN */     &fsm_cFrame_sGmpMap_ssMapNPC,
    /* GAME_SUBSTATE_SAVING */          &fsm_cFrame_sGmpMap_ssSave,
    /* GAME_SUBSTATE_STANDBY */         &fsm_cFrame_sGmpMap_ssStby,
    /* GAME_SUBSTATE_PAUSED */          &fsm_cFrame_sGmpMap_ssPause,
    /* GAME_SUBSTATE_MAP_CANDIDATES */  &fsm_cFrame_sGmpMap_ssMapCndt,
    /* GAME_SUBSTATE_CUTSCENE */        NULL,
    /* GAME_SUBSTATE_MAP_ANIMATION */   &fsm_cFrame_sGmpMap_ssMapAnim,
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

void fsm_cFrame_sGmpMap_ssMapNPC(struct Game *sota) {

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
