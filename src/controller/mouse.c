
#include "controller/mouse.h"
#include "events.h"
#include "enums.h"
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
const struct controllerMouse controllerMouse_default = {
    .controller_type = CONTROLLER_MOUSE,
};

void Mouse_checkButton(struct controllerMouse *mouse, u8 button) {
    u32 event = 0;
    if (button == mouse->inputmap.accept)
        event = event_Input_ACCEPT;
    else if (button == mouse->inputmap.cancel)
        event = event_Input_CANCEL;
    else if (button == mouse->inputmap.stats)
        event = event_Input_STATS;

    Event_Emit(__func__, SDL_USEREVENT, event, mouse, &mouse->controller_type);
}
