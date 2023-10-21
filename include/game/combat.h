#ifndef GAME_COMBAT_H
#define GAME_COMBAT_H

#include "game/game.h"
#include "combat.h"

/* --- Combat --- */
void Game_Combat_Outcome( struct Game *sota);
void Game_Combat_Animate( struct Game *sota);
void Game_Combat_Resolves(struct Game *sota);

#endif /* GAME_COMBAT_H */
