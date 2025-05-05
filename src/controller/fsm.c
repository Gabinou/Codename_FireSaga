
#include "controller/fsm.h"
#include "game/game.h"
#include "events.h"
#include "debug.h"

/* --- ACTIONS --- */
const fsm_Input_s_t fsm_Input_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     &fsm_Input_sCUTSCENE,
    /* Cutscene */       &fsm_Input_sCUTSCENE,
    /* Gameplay_Map */   &fsm_Input_sGAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   &fsm_Input_sTITLE_SCREEN,
    /* Animation */      NULL,
};

/* --- SUB-ACTIONS --- */
const fsm_Input_s_t fsm_Input_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_Input_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_Input_sGmpMap_ssStby,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* Gamecube controller has NO shoulder_left */
void fsm_Input_sGAMEPLAY_MAP(struct Game *sota) {
    if (fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)](sota);

}

void fsm_Input_sTITLE_SCREEN(struct Game *sota) {
    if (fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)] != NULL)
        fsm_Input_sGmpMap_ss[Game_Substate_Current(sota)](sota);
}

void fsm_Input_sCUTSCENE(struct Game *sota) {
    /* Cutscene input map: all buttons can skip */
    sota->inputs.arr[SOTA_BUTTON_DPAD_RIGHT]      = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_DPAD_UP]         = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_DPAD_LEFT]       = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_DPAD_DOWN]       = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_A]               = event_Input_ACCEPT;
    sota->inputs.arr[SOTA_BUTTON_B]               = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_X]               = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_Y]               = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_START]           = event_Input_PAUSE;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_LEFT]   = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_FAST_FORWARD;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_CANCEL;
#ifdef SOTA_MUSIC_TOGGLE
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = event_Music_Toggle;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = 0;
#endif
#ifdef SOTA_INTERACTIVE_RELOAD
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = event_Reload;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = 0;
#endif
}

void fsm_Input_sGmpMap_ssMenu(struct Game *sota) {
    sota->inputs.arr[SOTA_BUTTON_DPAD_RIGHT]      = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_UP]         = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_LEFT]       = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_DOWN]       = 0;
    sota->inputs.arr[SOTA_BUTTON_A]               = event_Input_ACCEPT;
    sota->inputs.arr[SOTA_BUTTON_B]               = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_X]               = 0;
    sota->inputs.arr[SOTA_BUTTON_Y]               = event_Input_STATS;
    sota->inputs.arr[SOTA_BUTTON_START]           = event_Input_PAUSE;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_LEFT]   = 0;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_FAST_FORWARD;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_MENULEFT;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_MENURIGHT;
#ifdef SOTA_MUSIC_TOGGLE
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = event_Music_Toggle;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = 0;
#endif
#ifdef SOTA_INTERACTIVE_RELOAD
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = event_Reload;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = 0;
#endif
}

// TODO: button global range
void fsm_Input_sGmpMap_ssStby(struct Game *sota) {
    sota->inputs.arr[SOTA_BUTTON_DPAD_RIGHT]      = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_UP]         = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_LEFT]       = 0;
    sota->inputs.arr[SOTA_BUTTON_DPAD_DOWN]       = 0;
    sota->inputs.arr[SOTA_BUTTON_A]               = event_Input_ACCEPT;
    sota->inputs.arr[SOTA_BUTTON_B]               = event_Input_CANCEL;
    sota->inputs.arr[SOTA_BUTTON_X]               = event_Input_MINIMAP;
    sota->inputs.arr[SOTA_BUTTON_Y]               = event_Input_STATS;
    sota->inputs.arr[SOTA_BUTTON_START]           = event_Input_PAUSE;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_LEFT]   = event_Input_GLOBALRANGE;
    sota->inputs.arr[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_FAST_FORWARD;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_ZOOM_IN;
    sota->inputs.arr[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_ZOOM_OUT;
#ifdef SOTA_MUSIC_TOGGLE
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = event_Music_Toggle;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_M]      = 0;
#endif
#ifdef SOTA_INTERACTIVE_RELOAD
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = event_Reload;
#else
    sota->inputs.arr[SOTA_BUTTON_KEYBOARD_SPACE]  = 0;
#endif
}
