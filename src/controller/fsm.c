
#include "controller/fsm.h"

/* --- ACTIONS --- */
fsm_state_inputs_t fsm_state_inputs[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   &fsm_state_inputs_GAMEPLAY_MAP,
    /* Gameplay_Camp */  NULL,
    /* Preparation */    NULL,
    /* Title_Screen */   &fsm_state_inputs_TITLE_SCREEN,
    /* Animation */      NULL,
};

/* --- SUB-ACTIONS --- */
fsm_state_inputs_t fsm_substate_inputs[GAME_SUBSTATE_NUM] = {
    /* NULL */            NULL,
    /* MAP_MINIMAP */     NULL,
    /* MENU */            &fsm_substate_inputs_MENU,
    /* MAP_UNIT_MOVES */  NULL,
    /* MAP_COMBAT */      NULL,
    /* MAP_NPCTURN */     NULL,
    /* SAVING */          NULL,
    /* STANDBY */         NULL,
    /* PAUSED */          NULL,
    /* MAP_CANDIDATES */  NULL,
    /* CUTSCENE */        NULL,
    /* MAP_ANIMATION */   NULL,
};

/* Gamecube controller has NO shoulder_left */
void fsm_state_inputs_GAMEPLAY_MAP(struct Game *sota) {
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

void fsm_state_inputs_TITLE_SCREEN(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (fsm_substate_inputs[sota->substate] != NULL)
        fsm_substate_inputs[sota->substate](sota);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void fsm_substate_inputs_MENU(struct Game *sota) {
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
