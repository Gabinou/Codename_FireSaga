#ifndef GAME_CURSOR_H
#define GAME_CURSOR_H

#include "types.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* Need to be just a bit above 1.0 for cursor to sound good */
#define SOTA_CURSOR_SLIDEFACTOR 1.05f
#define SOTA_CURSOR_RATIO 57 /* ratio = fps / rate -> 57 = 60 / 1.05 */

/* --- Cursor --- */
void Game_Cursor_Free(   struct Game *sota);
void Game_Cursor_Create( struct Game *sota);
void Game_Cursor_Enable( struct Game *sota);
void Game_Cursor_Disable(struct Game *sota);

/* -- Follows mouse -- */
void Game_CursorfollowsMouse_onMap( struct Game *sota);
void Game_CursorfollowsMouse_onMenu(struct Game *sota);

/* -- Focus -- */
void Game_cursorFocus_onMap( struct Game *sota);
void Game_cursorFocus_onMenu(struct Game *sota);

/* -- Moves -- */
void Game_Cursor_Moves_onMap(     struct Game *sota);
void Game_Cursor_Moves_onMenu(    struct Game *sota);
void Game_Cursor_Moves_Straight(  struct Game *sota);
void Game_Cursor_Next_Candidate(  struct Game *sota);
void Game_Cursor_Move_toCandidate(struct Game *sota);

void Game_Cursor_movedTime_Compute(struct Game *sota, u64 time_ms);

b32 Game_isCursoronTilemap(struct Game *sota);

/* --- Mouse --- */
void Game_Mouse_Free(   struct Game *sota);
void Game_Mouse_Create( struct Game *sota);
void Game_Mouse_Enable( struct Game *sota);
void Game_Mouse_Disable(struct Game *sota);

#endif /* GAME_CURSOR_H */
