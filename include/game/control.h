#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "structs.h"
#include "game/game.h"
#include "controller/gamepad.h"
#include "controller/keyboard.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Convoy;
struct Game;
struct controllerGamepad;
struct controllerKeyboard;

/* -- Control -- */
void Game_Control_Gamepad( struct Game *sota);
void Game_Control_Keyboard(struct Game *sota);
void Game_Control_Touchpad(struct Game *sota);

/* - utilities - */
void Control_Cursor_Moves(struct Game *s, struct Point cm, struct Point t,
                          struct Point px, i32 ct);

void Control_Pressed( i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
                      b32 o, i32 t, i32 m);
void Gamepad_Pressed( i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
                      struct controllerGamepad  *gp);
void Keyboard_Pressed(i8 b, i8 *l, i8 *p, i32 *ct, u32 e,
                      struct controllerKeyboard *kb);

#endif /* GAME_CONTROL_H */
