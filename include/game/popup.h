#ifndef GAME_POPUP_H
#define GAME_POPUP_H

#include "game/game.h"
#include "popup/popup.h"
#include "popup/tile.h"
#include "popup/unit.h"
#include "popup/map_combat.h"
#include "popup/loadout_stats.h"
#include "systems/render.h"

/* --- ConstConstructors/Destructors --- */
void Game_PopUps_Free(struct Game                *sota);

/* --- PopUp_Loadout_Stats --- */
void Game_PopUp_Loadout_Stats_Hide(  struct Game *sota);
void Game_PopUp_Loadout_Stats_Free(  struct Game *sota);
void Game_PopUp_Loadout_Stats_Create(struct Game *sota);

/* --- PopUp_Map_Combat --- */
void Game_PopUp_Map_Combat_Hide(     struct Game *sota);
void Game_PopUp_Map_Combat_Free(     struct Game *sota);
void Game_PopUp_Map_Combat_Create(   struct Game *sota);
void Game_PopUp_Map_Combat_Update(   struct Game *sota);

/* --- PopUp_Tile --- */
void Game_PopUp_Tile_Hide(           struct Game *sota);
void Game_PopUp_Tile_Free(           struct Game *sota);
void Game_PopUp_Tile_Place(          struct Game *sota, struct Point pos);
void Game_PopUp_Tile_Create(         struct Game *sota);

/* --- PopUp_Unit --- */
void Game_PopUp_Unit_Hide(           struct Game *sota);
void Game_PopUp_Unit_Free(           struct Game *sota);
void Game_PopUp_Unit_Place(          struct Game *sota, struct Point pos);
void Game_PopUp_Unit_Create(         struct Game *sota);

/* --- PopUp_Pre_Combat --- */
void Game_PopUp_Pre_Combat_Free(    struct Game *sota);
void Game_PopUp_Pre_Combat_Hide(    struct Game *sota);
void Game_PopUp_Pre_Combat_Create(  struct Game *sota);
void Game_PopUp_Pre_Combat_Enable(  struct Game *sota);

#endif /* GAME_POPUP_H */
