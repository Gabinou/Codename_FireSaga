#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "types.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* --- Map utilities --- */
void Game_Map_Free(  struct Game *sota);
void Game_Map_Load(  struct Game *sota, i32 mapi);
void Game_Map_Unload(struct Game *sota);

void Game_Gameplay_Start(struct Game *sota, i32 state, i32 substate);

/* --- Reinforcements --- */
void Game_Map_Reinforcements_Load(struct Game *sota);
void GameMap_Reinforcements_Free(struct Game *sota);

/* --- Tiles & tilesets  --- */
void Game_Tiles_Unload( struct Game *sota);
void Game_Tilesets_Dump(struct Game *sota);

#endif /* GAME_MAP_H */
