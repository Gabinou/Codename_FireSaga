
#include "systemControl.h"

struct Point * cursor_move = NULL;
int32_t * controller_keyboard;
int32_t * controller_gamepad;
int32_t * controller_touchpad;
int32_t * controller_mouse;

void controlMalloc() {
    cursor_move = SDL_malloc(sizeof(*cursor_move));
    controller_keyboard = SDL_malloc(sizeof(*controller_keyboard));
    controller_gamepad = SDL_malloc(sizeof(*controller_gamepad));
    controller_touchpad = SDL_malloc(sizeof(*controller_touchpad));
    controller_mouse = SDL_malloc(sizeof(*controller_mouse));
    *controller_keyboard = CONTROLLER_KEYBOARD;
    *controller_gamepad = CONTROLLER_GAMEPAD;
    *controller_mouse = CONTROLLER_MOUSE;
    *controller_touchpad = CONTROLLER_TOUCHPAD;
}

void controlFree() {
    free(cursor_move);
    free(controller_keyboard);
    free(controller_gamepad);
    free(controller_mouse);
    free(controller_touchpad);
}


void tnecs_controlMouse(tnecs_system_input_t * in_input) {

}

void tnecs_controlKeyboard(tnecs_system_input_t * in_input) {
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Sprite * sprite_ptr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct controllerKeyboard * keyboard_ptr = TNECS_COMPONENTS_LIST(in_input, controllerKeyboard);

    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        // SDL_Log("controlKeyboard");

        const uint8_t * kb_state = SDL_GetKeyboardState(NULL);
        int8_t * pressed_button = NULL;

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->accept, keyboard_ptr[ent].inputmap->accept_mapped)) {
            arrput(pressed_button, GAME_BUTTON_ACCEPT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Accept, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->cancel, keyboard_ptr[ent].inputmap->cancel_mapped)) {
            arrput(pressed_button, GAME_BUTTON_CANCEL);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Cancel, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->minimap, keyboard_ptr[ent].inputmap->minimap_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MINIMAP);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Minimap, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->faster, keyboard_ptr[ent].inputmap->faster_mapped)) {
            arrput(pressed_button, GAME_BUTTON_FASTER);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Faster, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->pause, keyboard_ptr[ent].inputmap->pause_mapped)) {
            arrput(pressed_button, GAME_BUTTON_PAUSE);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Pause, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->menuright, keyboard_ptr[ent].inputmap->menuright_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENURIGHT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_menuRight, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->menuleft, keyboard_ptr[ent].inputmap->menuleft_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENULEFT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_menuLeft, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->stats, keyboard_ptr[ent].inputmap->stats_mapped)) {
            arrput(pressed_button, GAME_BUTTON_STATS);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Stats, controller_keyboard, NULL);
                }
            }
        }

        Keyboard_checkButton(&keyboard_ptr[ent], pressed_button, arrlen(pressed_button), in_input->deltat);

        int8_t * pressed_move = NULL;
        cursor_move->x = 0;
        cursor_move->y = 0;

        bool up = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveup, keyboard_ptr[ent].inputmap->moveup_mapped);
        bool down = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->movedown, keyboard_ptr[ent].inputmap->movedown_mapped);
        bool left = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveleft, keyboard_ptr[ent].inputmap->moveleft_mapped);
        bool right = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveright, keyboard_ptr[ent].inputmap->moveright_mapped);

        if (up && !down) {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if (down && !up) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        if (left && !right) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        } else if (right && !left) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        }

        Keyboard_checkMove(&keyboard_ptr[ent], pressed_move, arrlen(pressed_move), in_input->deltat);
        if (keyboard_ptr[ent].timeheld_button == 0) {
            keyboard_ptr[ent].block_buttons = false;
        }

        // position_ptr->updatable = ((sprite_ptr[ent].objectivepos.x == sprite_ptr[ent].slidepos.x) && (sprite_ptr[ent].objectivepos.y == sprite_ptr[ent].slidepos.y));

        if (((cursor_move->x != 0) || (cursor_move->y != 0)) && (position_ptr[ent].updatable)) {
            if (Position_Pos_Add(&position_ptr[ent], cursor_move->x, cursor_move->y)) {
                // SDL_Log("Emit controlKeyboard");
                Event_Emit(SDL_USEREVENT, event_Cursor_Moved, cursor_move, NULL);
            }
        }

    }
}

void tnecs_controlGamepad(tnecs_system_input_t * in_input) {
    struct Position * position_ptr = TNECS_COMPONENTS_LIST(in_input, Position);
    struct Sprite * sprite_ptr = TNECS_COMPONENTS_LIST(in_input, Sprite);
    struct controllerGamepad * gamepad_ptr = TNECS_COMPONENTS_LIST(in_input, controllerGamepad);

    for (uint16_t ent = 0; ent < in_input->num_entities; ent++) {
        // SDL_Log("controlGamepad");
        int8_t * pressed_button = NULL;
        int32_t joystick_deadzone = gamepad_ptr[ent].joystick_deadzone;

        SDL_GameController * controller = gamepad_ptr[ent].controller;
        Sint16 mainxaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->mainxaxis[0]);
        Sint16 mainyaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->mainyaxis[0]);
        Sint16 secondxaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->secondxaxis[0]);
        Sint16 secondyaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->secondyaxis[0]);

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->cancel, gamepad_ptr[ent].inputmap->cancel_mapped)) {
            arrput(pressed_button, GAME_BUTTON_CANCEL);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Cancel, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->accept, gamepad_ptr[ent].inputmap->accept_mapped)) {
            arrput(pressed_button, GAME_BUTTON_ACCEPT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Accept, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->minimap, gamepad_ptr[ent].inputmap->minimap_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MINIMAP);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Minimap, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->faster, gamepad_ptr[ent].inputmap->faster_mapped)) {
            arrput(pressed_button, GAME_BUTTON_FASTER);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Faster, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->pause, gamepad_ptr[ent].inputmap->pause_mapped)) {
            arrput(pressed_button, GAME_BUTTON_PAUSE);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Pause, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->stats, gamepad_ptr[ent].inputmap->stats_mapped)) {
            arrput(pressed_button, GAME_BUTTON_STATS);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Stats, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->menuright, gamepad_ptr[ent].inputmap->menuright_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENURIGHT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_menuRight, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->menuleft, gamepad_ptr[ent].inputmap->menuleft_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENULEFT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_menuLeft, controller_gamepad, NULL);
            }
        }

        int8_t * pressed_move = NULL;
        cursor_move->x = 0;
        cursor_move->y = 0;

        Gamepad_checkButton(&gamepad_ptr[ent], pressed_button, arrlen(pressed_button), in_input->deltat);
        if (gamepad_ptr[ent].timeheld_button == 0.0f) {
            gamepad_ptr[ent].block_buttons = false;
        }

        if ((mainxaxis > joystick_deadzone) || (secondxaxis > joystick_deadzone)) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        } else if ((mainxaxis < -joystick_deadzone) || (secondxaxis < -joystick_deadzone)) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        }
        if ((mainyaxis > joystick_deadzone) || (secondyaxis > joystick_deadzone)) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if ((mainyaxis < -joystick_deadzone) || (secondyaxis < -joystick_deadzone))  {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveright, gamepad_ptr[ent].inputmap->moveright_mapped)) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        } else if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveleft, gamepad_ptr[ent].inputmap->moveleft_mapped)) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        }
        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveup, gamepad_ptr[ent].inputmap->moveup_mapped)) {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->movedown, gamepad_ptr[ent].inputmap->movedown_mapped)) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        // Gamepad_checkMove(&gamepad_ptr[ent], pressed_move, arrlen(pressed_move), it->delta_time);
        // if (gamepad_ptr[ent].timeheld_move == 0.0f) {
        // gamepad_ptr[ent].block_move = false;
        // }
        // SDL_Log("%d %d", cursor_move->x, cursor_move->y);
        position_ptr->updatable = ((sprite_ptr[ent].objectivepos.x == sprite_ptr[ent].slidepos.x) && (sprite_ptr[ent].objectivepos.y == sprite_ptr[ent].slidepos.y));

        if (((cursor_move->x != 0) || (cursor_move->y != 0)) && (position_ptr[ent].updatable)) {
            if (Position_Pos_Add(&position_ptr[ent], cursor_move->x, cursor_move->y)) {
                Event_Emit(SDL_USEREVENT, event_Cursor_Moved, cursor_move, NULL);
            }
        }
    }
}

void tnecs_controlTouchpad(tnecs_system_input_t * in_input) {

}


void controlMouse(ecs_iter_t * it) {
// SDL takes charges of mouse movements through events?
}

void controlKeyboard(ecs_iter_t * it) {
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct controllerKeyboard * keyboard_ptr = ecs_column(it, struct controllerKeyboard, 2);
    struct Sprite * sprite_ptr = ecs_column(it, struct Sprite, 2);

    for (uint16_t ent = 0; ent < it->count; ent++) {
        // SDL_Log("controlKeyboard");

        const uint8_t * kb_state = SDL_GetKeyboardState(NULL);
        int8_t * pressed_button = NULL;

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->accept, keyboard_ptr[ent].inputmap->accept_mapped)) {
            arrput(pressed_button, GAME_BUTTON_ACCEPT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Accept, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->cancel, keyboard_ptr[ent].inputmap->cancel_mapped)) {
            arrput(pressed_button, GAME_BUTTON_CANCEL);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Cancel, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->minimap, keyboard_ptr[ent].inputmap->minimap_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MINIMAP);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Minimap, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->faster, keyboard_ptr[ent].inputmap->faster_mapped)) {
            arrput(pressed_button, GAME_BUTTON_FASTER);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Faster, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->pause, keyboard_ptr[ent].inputmap->pause_mapped)) {
            arrput(pressed_button, GAME_BUTTON_PAUSE);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Pause, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->menuright, keyboard_ptr[ent].inputmap->menuright_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENURIGHT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_menuRight, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->menuleft, keyboard_ptr[ent].inputmap->menuleft_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENULEFT);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_menuLeft, controller_keyboard, NULL);
                }
            }
        }

        if (Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->stats, keyboard_ptr[ent].inputmap->stats_mapped)) {
            arrput(pressed_button, GAME_BUTTON_STATS);
            if (!keyboard_ptr[ent].block_buttons) {
                if (keyboard_ptr[ent].timeheld_button > KEYBOARD_MINHELD) {
                    Event_Emit(SDL_USEREVENT, event_Input_Stats, controller_keyboard, NULL);
                }
            }
        }

        Keyboard_checkButton(&keyboard_ptr[ent], pressed_button, arrlen(pressed_button), it->delta_time);

        int8_t * pressed_move = NULL;
        cursor_move->x = 0;
        cursor_move->y = 0;

        bool up = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveup, keyboard_ptr[ent].inputmap->moveup_mapped);
        bool down = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->movedown, keyboard_ptr[ent].inputmap->movedown_mapped);
        bool left = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveleft, keyboard_ptr[ent].inputmap->moveleft_mapped);
        bool right = Keyboard_isPressed(&keyboard_ptr[ent], kb_state, keyboard_ptr[ent].inputmap->moveright, keyboard_ptr[ent].inputmap->moveright_mapped);

        if (up && !down) {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if (down && !up) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        if (left && !right) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        } else if (right && !left) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        }

        Keyboard_checkMove(&keyboard_ptr[ent], pressed_move, arrlen(pressed_move), it->delta_time);
        if (keyboard_ptr[ent].timeheld_button == 0) {
            keyboard_ptr[ent].block_buttons = false;
        }

        // position_ptr->updatable = ((sprite_ptr[ent].objectivepos.x == sprite_ptr[ent].slidepos.x) && (sprite_ptr[ent].objectivepos.y == sprite_ptr[ent].slidepos.y));

        if (((cursor_move->x != 0) || (cursor_move->y != 0)) && (position_ptr[ent].updatable)) {
            if (Position_Pos_Add(&position_ptr[ent], cursor_move->x, cursor_move->y)) {
                // SDL_Log("Emit controlKeyboard");
                Event_Emit(SDL_USEREVENT, event_Cursor_Moved, cursor_move, NULL);
            }
        }

    }
}


void controlGamepad(ecs_iter_t * it) {
    struct Position * position_ptr = ecs_column(it, struct Position, 1);
    struct controllerGamepad * gamepad_ptr = ecs_column(it, struct controllerGamepad, 2);
    struct Sprite * sprite_ptr = ecs_column(it, struct Sprite, 3);

    for (uint16_t ent = 0; ent < it->count; ent++) {
        // SDL_Log("controlGamepad");
        int8_t * pressed_button = NULL;
        int32_t joystick_deadzone = gamepad_ptr[ent].joystick_deadzone;

        SDL_GameController * controller = gamepad_ptr[ent].controller;
        Sint16 mainxaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->mainxaxis[0]);
        Sint16 mainyaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->mainyaxis[0]);
        Sint16 secondxaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->secondxaxis[0]);
        Sint16 secondyaxis = SDL_GameControllerGetAxis(controller, gamepad_ptr[ent].inputmap->secondyaxis[0]);

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->cancel, gamepad_ptr[ent].inputmap->cancel_mapped)) {
            arrput(pressed_button, GAME_BUTTON_CANCEL);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Cancel, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->accept, gamepad_ptr[ent].inputmap->accept_mapped)) {
            arrput(pressed_button, GAME_BUTTON_ACCEPT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Accept, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->minimap, gamepad_ptr[ent].inputmap->minimap_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MINIMAP);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Minimap, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->faster, gamepad_ptr[ent].inputmap->faster_mapped)) {
            arrput(pressed_button, GAME_BUTTON_FASTER);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Faster, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->pause, gamepad_ptr[ent].inputmap->pause_mapped)) {
            arrput(pressed_button, GAME_BUTTON_PAUSE);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Pause, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->stats, gamepad_ptr[ent].inputmap->stats_mapped)) {
            arrput(pressed_button, GAME_BUTTON_STATS);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_Stats, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->menuright, gamepad_ptr[ent].inputmap->menuright_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENURIGHT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_menuRight, controller_gamepad, NULL);
            }
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->menuleft, gamepad_ptr[ent].inputmap->menuleft_mapped)) {
            arrput(pressed_button, GAME_BUTTON_MENULEFT);
            if (!gamepad_ptr[ent].block_buttons) {
                Event_Emit(SDL_USEREVENT, event_Input_menuLeft, controller_gamepad, NULL);
            }
        }

        int8_t * pressed_move = NULL;
        cursor_move->x = 0;
        cursor_move->y = 0;

        Gamepad_checkButton(&gamepad_ptr[ent], pressed_button, arrlen(pressed_button), it->delta_time);
        if (gamepad_ptr[ent].timeheld_button == 0.0f) {
            gamepad_ptr[ent].block_buttons = false;
        }

        if ((mainxaxis > joystick_deadzone) || (secondxaxis > joystick_deadzone)) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        } else if ((mainxaxis < -joystick_deadzone) || (secondxaxis < -joystick_deadzone)) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        }
        if ((mainyaxis > joystick_deadzone) || (secondyaxis > joystick_deadzone)) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if ((mainyaxis < -joystick_deadzone) || (secondyaxis < -joystick_deadzone))  {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveright, gamepad_ptr[ent].inputmap->moveright_mapped)) {
            cursor_move->x = 1;
            arrput(pressed_move, GAME_BUTTON_RIGHT);
        } else if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveleft, gamepad_ptr[ent].inputmap->moveleft_mapped)) {
            cursor_move->x = -1;
            arrput(pressed_move, GAME_BUTTON_LEFT);
        }
        if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->moveup, gamepad_ptr[ent].inputmap->moveup_mapped)) {
            cursor_move->y = -1;
            arrput(pressed_move, GAME_BUTTON_UP);
        } else if (Gamepad_isPressed(&gamepad_ptr[ent], gamepad_ptr[ent].inputmap->movedown, gamepad_ptr[ent].inputmap->movedown_mapped)) {
            cursor_move->y = 1;
            arrput(pressed_move, GAME_BUTTON_DOWN);
        }

        // Gamepad_checkMove(&gamepad_ptr[ent], pressed_move, arrlen(pressed_move), it->delta_time);
        // if (gamepad_ptr[ent].timeheld_move == 0.0f) {
        // gamepad_ptr[ent].block_move = false;
        // }
        // SDL_Log("%d %d", cursor_move->x, cursor_move->y);
        position_ptr->updatable = ((sprite_ptr[ent].objectivepos.x == sprite_ptr[ent].slidepos.x) && (sprite_ptr[ent].objectivepos.y == sprite_ptr[ent].slidepos.y));

        if (((cursor_move->x != 0) || (cursor_move->y != 0)) && (position_ptr[ent].updatable)) {
            if (Position_Pos_Add(&position_ptr[ent], cursor_move->x, cursor_move->y)) {
                Event_Emit(SDL_USEREVENT, event_Cursor_Moved, cursor_move, NULL);
            }
        }
    }
}

void controlTouchpad(ecs_iter_t * it) {

}

void systemControlModuleImport(ecs_world_t * in_world) {
    ECS_IMPORT(in_world, PositionModule);
    ECS_IMPORT(in_world, controllerMouseModule);
    ECS_IMPORT(in_world, controllerGamepadModule);
    ECS_IMPORT(in_world, controllerKeyboardModule);
    ECS_IMPORT(in_world, controllerTouchpadModule);
    ECS_MODULE(in_world, systemControlModule);
    ECS_IMPORT(in_world, SpriteModule);
    ECS_SYSTEM(in_world, controlGamepad, EcsOnUpdate, position.module.Position, controller.gamepad.module.controllerGamepad, sprite.module.Sprite);
    ECS_SYSTEM(in_world, controlKeyboard, EcsOnUpdate, position.module.Position, controller.keyboard.module.controllerKeyboard, sprite.module.Sprite);
    ECS_SYSTEM(in_world, controlMouse, EcsOnUpdate, position.module.Position, controller.mouse.module.controllerMouse, sprite.module.Sprite);
    ECS_SYSTEM(in_world, controlTouchpad, EcsOnUpdate, position.module.Position, controller.touchpad.module.controllerTouchpad, sprite.module.Sprite);
    ECS_SET_ENTITY(controlGamepad);
    ECS_SET_ENTITY(controlKeyboard);
    ECS_SET_ENTITY(controlMouse);
    ECS_SET_ENTITY(controlTouchpad);
}