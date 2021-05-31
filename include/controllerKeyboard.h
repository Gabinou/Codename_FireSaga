#ifndef CONTROLLERKEYBOARD_H
#define CONTROLLERKEYBOARD_H

#include <math.h>
#include "flecs.h"
#include "SDL2/SDL.h"
#include "stb_ds.h"
#include "narrative.h"

typedef struct controllerKeyboard {
    struct KeyboardInputMap * inputmap;
    int8_t * held_button;
    size_t held_button_num;
    int8_t * held_move;
    size_t held_move_num;

    bool block_buttons;
    bool block_move;

    float timeheld_button;
    float timeheld_move;
    struct Point tilesize;
} controllerKeyboard;
extern struct controllerKeyboard controllerKeyboard_default;

typedef struct controllerKeyboardModule {
    ECS_DECLARE_COMPONENT(controllerKeyboard);
} controllerKeyboardModule;

void controllerKeyboardModuleImport(ecs_world_t * in_world);
#define controllerKeyboardModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, controllerKeyboard);


extern void Keyboard_checkMove(struct controllerKeyboard * in_keyboard, int8_t * in_pressed, size_t pressed_num, float dt);
extern void Keyboard_checkButton(struct controllerKeyboard * in_keyboard, int8_t * in_pressed, size_t pressed_num, float dt);
extern bool Keyboard_isPressed(struct controllerKeyboard * in_keyboard, const uint8_t * state_array, SDL_Scancode * to_find, size_t mapped_len);


#endif /* CONTROLLERKEYBOARD_H */
