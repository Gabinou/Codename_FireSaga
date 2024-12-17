
#include "controller/mouse.h"
#include "events.h"
#include "structs.h"
#include "game/game.h"

const struct MouseInputMap MouseInputMap_default = {
    .accept          = SDL_BUTTON_LEFT,
    .cancel          = SDL_BUTTON_RIGHT,
    .stats           = SDL_BUTTON_MIDDLE,
    .accept_len      = 1,
    .cancel_len      = 1,
    .stats_len       = 1,
    .menuright_len   = 1,
    .menuleft_len    = 1,
    .minimap_len     = 1,
    .faster_len      = 1,
    .pause_len       = 1
};

void Mouse_checkButton(struct controllerMouse *mouse, u8 button) {
    if (button == mouse->inputmap.accept)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_ACCEPT, mouse, NULL);
    else if (button == mouse->inputmap.cancel)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_CANCEL, mouse, NULL);
    else if (button == mouse->inputmap.stats)
        Event_Emit(__func__, SDL_USEREVENT, event_Input_STATS, mouse, NULL);
}
