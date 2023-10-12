
#include "controller/fsm.h"

/* --- ACTIONS --- */
fsm_state_inputs_t fsm_state_inputs[GAME_STATE_NUM] = {
    /* NULL */           NULL,
    /* Combat */         NULL,
    /* Scene_Talk */     NULL,
    /* Scene_FMV */      NULL,
    /* Gameplay_Map */   NULL,
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
    sota->inputs[SOTA_BUTTON_A]               = event_Input_Accept;
    sota->inputs[SOTA_BUTTON_B]               = event_Input_Cancel;
    sota->inputs[SOTA_BUTTON_X]               = event_Input_Minimap;
    sota->inputs[SOTA_BUTTON_Y]               = event_Input_Stats;
    sota->inputs[SOTA_BUTTON_START]           = event_Input_Pause;
    sota->inputs[SOTA_BUTTON_SHOULDER_LEFT]   = event_Input_globalRange;
    sota->inputs[SOTA_BUTTON_SHOULDER_RIGHT]  = event_Input_Faster;
    sota->inputs[SOTA_BUTTON_TRIGGER_LEFT]    = event_Input_menuLeft;
    sota->inputs[SOTA_BUTTON_TRIGGER_RIGHT]   = event_Input_menuRight;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
