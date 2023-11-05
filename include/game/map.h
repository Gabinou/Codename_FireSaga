#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "game/game.h"
#include "enums.h"
#include "map/render.h"
#include "utilities.h"
#include "tile.h"
#include "unit.h"

/* --- Map utilities --- */
void Game_Map_SDL_free(  struct Game *sota);
void Game_Map_Load(  struct Game *sota, i16 in_map_index);
void Game_Map_Unload(struct Game *sota);

void Game_debugMap_Load(struct Game *sota);
void Game_debugMap_SDL_free(struct Game *sota);

/* --- Reinforcements --- */
void Game_Map_Reinforcements_Load(struct Game *sota);
void Game_Map_Reinforcements_SDL_free(struct Game *sota);

/* --- Tiles & tilesets  --- */
void Game_Tiles_Unload( struct Game *sota);
void Game_Tilesets_Dump(struct Game *sota);

#endif /* GAME_MAP_H */
