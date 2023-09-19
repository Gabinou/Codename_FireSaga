#ifndef GAME_CURSOR_H
#define GAME_CURSOR_H

#include "game/game.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/mouse.h"
#include "controller/touchpad.h"
#include "systems/control.h"

/* --- Cursor --- */
extern void Game_Cursor_Free(   struct Game *sota);
extern void Game_Cursor_Create( struct Game *sota);
extern void Game_Cursor_Enable( struct Game *sota);
extern void Game_Cursor_Disable(struct Game *sota);

/* -- Follows mouse -- */
extern void Game_CursorfollowsMouse_onMap( struct Game *sota);
extern void Game_CursorfollowsMouse_onMenu(struct Game *sota);

/* -- Focus -- */
extern void Game_cursorFocus_onMap( struct Game *sota);
extern void Game_cursorFocus_onMenu(struct Game *sota);

/* -- Moves -- */
extern void Game_Cursor_Moves_onMap(     struct Game *sota);
extern void Game_Cursor_Moves_onMenu(    struct Game *sota);
extern void Game_Cursor_Moves_Straight(  struct Game *sota);
extern void Game_Cursor_Next_Candidate(  struct Game *sota);
extern void Game_Cursor_Move_toCandidate(struct Game *sota);

extern void Game_Cursor_movedTime_Compute(struct Game *sota, u64 time_ms);

extern bool Game_isCursoronTilemap(struct Game *sota);

/* --- Mouse --- */
extern void Game_Mouse_Free(   struct Game *sota);
extern void Game_Mouse_Create( struct Game *sota);
extern void Game_Mouse_Enable( struct Game *sota);
extern void Game_Mouse_Disable(struct Game *sota);

#endif /* GAME_CURSOR_H */
