
#include "controller/fsm.h"

/* --- ACTIONS --- */
fsm_Input_s_t fsm_Input_s[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_Input_sGAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   &fsm_Input_sTITLE_SCREEN,
    /* Animation */      NULL,
};

/* --- SUB-ACTIONS --- */
fsm_Input_s_t fsm_Input_sGmpMap_ss[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_Input_sGmpMap_ssMenu,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         &fsm_Input_sGmpMap_ssStby,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* Gamecube controller has NO shoulder_left */
void fsm_Input_sGAMEPLAY_MAP(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_Input_sGmpMap_ss[sota->substate] != NULL)
        fsm_Input_sGmpMap_ss[sota->substate](sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_Input_sTITLE_SCREEN(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_Input_sGmpMap_ss[sota->substate] != NULL)
        fsm_Input_sGmpMap_ss[sota->substate](sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_Input_sGmpMap_ssMenu(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->inputs[SOTA_BUTTON_DPAD_RIGHT]      = 0;
    sota->inputs[SOTA_BUTTON_DPAD_UP]         = 0;
    sota->inputs[SOTA_BUTTON_DPAD_LEFT]       = 0;
    sota->inputs[SOTA_BUTTON_DPAD_DOWN]       = 0;
    sota->inputs[SOTA_BUTTON_A]               = event_Input_ACCEPT;
    sota->inputs[SOTA_BUTTON_B]               = event_Input_CANCEL;
    sota->inputs[SOTA_BUTTON_X]               = 0;
    sota->inputs[SOTA_BUTTON_Y]               = event_Input_STATS;
    sota->inputs[SOTA_BUTTON_START]           = event_Input_PAUSE;
    sota->inputs[SOTA_BUTTON_SHOULDER_LEFT]   = 0;
    sota->inputs[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_FASTER;
    sota->inputs[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_MENULEFT;
    sota->inputs[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_MENURIGHT;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_Input_sGmpMap_ssStby(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->inputs[SOTA_BUTTON_DPAD_RIGHT]      = 0;
    sota->inputs[SOTA_BUTTON_DPAD_UP]         = 0;
    sota->inputs[SOTA_BUTTON_DPAD_LEFT]       = 0;
    sota->inputs[SOTA_BUTTON_DPAD_DOWN]       = 0;
    sota->inputs[SOTA_BUTTON_A]               = event_Input_ACCEPT;
    sota->inputs[SOTA_BUTTON_B]               = event_Input_CANCEL;
    sota->inputs[SOTA_BUTTON_X]               = event_Input_MINIMAP;
    sota->inputs[SOTA_BUTTON_Y]               = event_Input_STATS;
    sota->inputs[SOTA_BUTTON_START]           = event_Input_PAUSE;
    sota->inputs[SOTA_BUTTON_SHOULDER_LEFT]   = 0;
    sota->inputs[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_FASTER;
    sota->inputs[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_ZOOM_IN;
    sota->inputs[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_ZOOM_OUT;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
