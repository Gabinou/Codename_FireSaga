
#include "enums.h"
#include "events.h"
#include "structs.h"
#include "platform.h"

#include "controller/mouse.h"
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

    struct controllerMouse *data1 = IES_calloc(1, sizeof(*data1));
    data1 = mouse;
    i32 *data2 = IES_calloc(1, sizeof(*data1));
    *data2 = mouse->controller_type;

    Event_Emit( __func__, SDL_USEREVENT,
                event,
                data1, data2);
}
