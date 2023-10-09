#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "game/game.h"
#include "enums.h"
#include "map/render.h"
#include "utilities.h"
#include "tile.h"
#include "unit.h"

/* --- Map utilities --- */
extern void Game_Map_Free(  struct Game *sota);
extern void Game_Map_Load(  struct Game *sota, if16 in_map_index);
extern void Game_Map_Unload(struct Game *sota);

extern void Game_debugMap_Load(struct Game *sota);
extern void Game_debugMap_Free(struct Game *sota);

/* --- Reinforcements --- */
extern void Game_Map_Reinforcements_Load(struct Game *sota);
extern void Game_Map_Reinforcements_Free(struct Game *sota);

/* --- Tiles & tilesets  --- */
extern void Game_Tiles_Unload( struct Game *sota);
extern void Game_Tilesets_Dump(struct Game *sota);

#endif /* GAME_MAP_H */
