
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "structs.h"
#include "utilities.h"

const struct KeyboardInputMap KeyboardInputMap_default = {
    .dpad_right         = {SDL_SCANCODE_D,     SDL_SCANCODE_RIGHT},
    .dpad_up            = {SDL_SCANCODE_W,     SDL_SCANCODE_UP},
    .dpad_left          = {SDL_SCANCODE_A,     SDL_SCANCODE_LEFT},
    .dpad_down          = {SDL_SCANCODE_S,     SDL_SCANCODE_DOWN},
    .a                  = {SDL_SCANCODE_Z},
    .b                  = {SDL_SCANCODE_X,     SDL_SCANCODE_BACKSPACE},
    .x                  = {SDL_SCANCODE_R},
    .trigger_right      = {SDL_SCANCODE_E},
    .trigger_left       = {SDL_SCANCODE_Q},
    .start              = {SDL_SCANCODE_RETURN},
    .y                  = {SDL_SCANCODE_C},
    .shoulder_left      = {SDL_SCANCODE_P},
    .shoulder_right     = {SDL_SCANCODE_TAB},
    .m                  = {SDL_SCANCODE_M},
    .space              = {SDL_SCANCODE_SPACE},

    .dpad_right_len     = 2,
    .dpad_up_len        = 2,
    .dpad_left_len      = 2,
    .dpad_down_len      = 2,
    .a_len              = 1,
    .b_len              = 2,
    .x_len              = 1,
    .y_len              = 1,
    .start_len          = 1,
    .shoulder_left_len  = 1,
    .shoulder_right_len = 1,
    .trigger_right_len  = 1,
    .trigger_left_len   = 1,
    .m_len              = 1,
    .space_len          = 1,
};

const struct controllerKeyboard controllerKeyboard_default = {
    .controller_type    = CONTROLLER_KEYBOARD,
};

void Keyboard_Init(struct controllerKeyboard *kb) {
    *kb = controllerKeyboard_default;
}

void Keyboard_Init_tnecs(void *voidkb) {
    Keyboard_Init(voidkb);
}

void Keyboard_Held(i8 *held, size_t *h_num, i32 *timeheld, i8 *pressed, size_t p_num, i32 dt_ns) {
    SDL_assert(p_num < SOTA_INPUT_END);
    SDL_assert(p_num >= 0);
    b32 arrequal = false;
    if ((*h_num == p_num) && (p_num != 0))
        arrequal = i8_all_equal(held, pressed, p_num);

    if (arrequal) {
        /* - Keep pressing same buttons - */
        *timeheld += dt_ns;
    } else if (p_num > 0) {
        /* - Keep pressing different buttons or press no buttons - */
        memcpy(held, pressed, p_num * sizeof(*held));
        *h_num = p_num;
        *timeheld += dt_ns;
    } else if (p_num == 0) {
        *timeheld = 0.0f;
    }
}

b32 Keyboard_isPressed( struct controllerKeyboard *kb,
                        const u8 *state_array,
                        int button) {

    /* -- Preliminaries -- */
    struct KeyboardInputMap *map = kb->inputmap;
    /* -- Get mapped button from button index -- */
    SDL_Scancode *buttons = (SDL_Scancode *)&map->dpad_right;
    buttons += button * SOTA_MAPPABLE_BUTTONS_NUM;

    /* -- Check if button/axis is pressed, -- */
    b32 out = false;
    u8 len  = *(&map->dpad_right_len + button);

    for (int i = 0; i < len; i++) {
        // TODO: Check all mappend buttons using the _len parameters!
        if (state_array[buttons[i]]) {
            out = true;
            break;
        }
    }

    return (out);
}
