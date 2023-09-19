
#include "controller/keyboard.h"

struct KeyboardInputMap KeyboardInputMap_default = {
    .moveright        = {SDL_SCANCODE_D,     SDL_SCANCODE_RIGHT},
    .moveup           = {SDL_SCANCODE_W,     SDL_SCANCODE_UP},
    .moveleft         = {SDL_SCANCODE_A,     SDL_SCANCODE_LEFT},
    .movedown         = {SDL_SCANCODE_S,     SDL_SCANCODE_DOWN},
    .accept           = {SDL_SCANCODE_Z,     SDL_SCANCODE_SPACE},
    .cancel           = {SDL_SCANCODE_X,     SDL_SCANCODE_BACKSPACE},
    .minimap          = {SDL_SCANCODE_R},
    .menuright        = {SDL_SCANCODE_E},
    .menuleft         = {SDL_SCANCODE_Q},
    .pause            = {SDL_SCANCODE_RETURN},
    .stats            = {SDL_SCANCODE_C},
    .options          = {SDL_SCANCODE_P},
    .faster           = {SDL_SCANCODE_TAB},
    .globalRange      = {SDL_SCANCODE_F},

    .moveright_len    = 2,
    .moveup_len       = 2,
    .moveleft_len     = 2,
    .movedown_len     = 2,
    .accept_len       = 2,
    .cancel_len       = 2,
    .minimap_len      = 1,
    .menuright_len    = 1,
    .menuleft_len     = 1,
    .pause_len        = 1,
    .stats_len        = 1,
    .options_len      = 1,
    .faster_len       = 1,
    .globalRange_len  = 1,
};

struct controllerKeyboard controllerKeyboard_default = {
    .controller_type    = CONTROLLER_KEYBOARD,
    .timeheld_button_ns = 0,
    .timeheld_move_ns   = 0,
    .block_buttons      = false,
    .block_move         = false,
    .inputmap           = NULL,
};

void Keyboard_Held(if8 *held, size_t *h_num, if32 *timeheld, if8 *pressed, size_t p_num, if32 dt) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(p_num < SOTA_BUTTON_END);
    SDL_assert(p_num >= 0);
    bool arrequal = false;
    if ((*h_num == p_num) && (p_num != 0))
        arrequal = if8_all_equal(held, pressed, p_num);

    if (arrequal) {
        /* - Keep pressing same buttons - */
        *timeheld += dt / SOTA_ns;
    } else {
        /* - Keep pressing different buttons or press no buttons - */
        memcpy(held, pressed, p_num * sizeof(*held));
        *h_num = p_num;
        *timeheld = 0.0f;
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Keyboard_isPressed(struct controllerKeyboard *kb, const uf8 *state_array, int button) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Preliminaries -- */
    struct KeyboardInputMap *map = kb->inputmap;
    /* -- Get mapped button from button index -- */
    SDL_Scancode *buttons = (SDL_Scancode *)&map->moveright;
    buttons += button * SOTA_MAPPABLE_BUTTONS_NUM;

    /* -- Check if button/axis is pressed, -- */
    bool out = false;
    uf8 len  = *(&map->moveright_len + button);

    for (int i = 0; i < len; i++) {
        if (state_array[buttons[i]]) {
            out = true;
            break;
        }
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}
