#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include "game/game.h"
#include "enums.h"
#include "bitfields.h"
#include "weapon.h"
#include "unit.h"
#include "position.h"
#include "sprite.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;

/* --- Constructors/Destructors --- */
void Game_Items_Free(  struct dtab **items_dtab);
void Game_Weapons_Free(struct dtab **weapons_dtab);

/* --- Party utilities --- */
void Game_Party_Load(  struct Game *s, i16 *to_load_ids, size_t load_num);
void Game_Party_Clear( struct Game *s);
void Game_Party_Unload(struct Game *s, i16 *to_unload,   size_t unload_num);

tnecs_entity_t Game_Unit_Entity_Create(struct Game *s, i16 in_unit, struct Point in_pos);

/* --- Unitmap --- */
void Game_UnitsonMap_Free(struct Game *s);
void Game_putPConMap(struct Game *s, i16 *u, struct Point *p, size_t l);

/* --- Utilities --- */
void Game_Weapons_Rewrite(struct Game *s);

/* --- Wait/Refresh --- */
void Game_Unit_Wait(   struct Game *s, tnecs_entity_t u);
void Game_Unit_Refresh(struct Game *s, tnecs_entity_t u);

#endif /* GAME_UNIT_H */
