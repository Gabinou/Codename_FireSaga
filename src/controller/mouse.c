
#include "controller/mouse.h"

struct MouseInputMap MouseInputMap_default = {
    .accept =         SDL_BUTTON_LEFT,
    .cancel =         SDL_BUTTON_RIGHT,
    .stats =          SDL_BUTTON_MIDDLE,
    .menuright =      0,
    .menuleft =       0,
    .minimap =        0,
    .faster =         0,
    .pause =          0,
    .accept_len =     1,
    .cancel_len =     1,
    .stats_len =      1,
    .menuright_len =  1,
    .menuleft_len =   1,
    .minimap_len =    1,
    .faster_len =     1,
    .pause_len =      1,
};

void Mouse_checkButton(struct controllerMouse *mouse, u8 button) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (button == mouse->inputmap.accept)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_Accept, mouse, NULL);
    else if (button == mouse->inputmap.cancel)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_Cancel, mouse, NULL);
    else if (button == mouse->inputmap.stats)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_Stats, mouse, NULL);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
