#ifndef GAME_COMBAT_H
#define GAME_COMBAT_H

#include "combat.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* --- Combat --- */
void Game_Combat_Outcome( struct Game *sota);
void Game_Combat_Animate( struct Game *sota);
void Game_Combat_Resolves(struct Game *sota);

#endif /* GAME_COMBAT_H */
