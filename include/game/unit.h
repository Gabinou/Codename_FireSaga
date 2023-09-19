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
extern void Game_Items_Free(  struct dtab *items_dtab);
extern void Game_Weapons_Free(struct dtab *weapons_dtab);

/* --- Party utilities --- */
extern void Game_Party_Load(  struct Game *s, if16 *to_load_ids, size_t load_num);
extern void Game_Party_Clear( struct Game *s);
extern void Game_Party_Unload(struct Game *s, if16 *to_unload,   size_t unload_num);

extern tnecs_entity_t Game_Unit_Entity_Create(struct Game *s, if16 in_unit, struct Point in_pos);

/* --- Unitmap --- */
extern void Game_UnitsonMap_Free(struct Game *s);
extern void Game_putPConMap(struct Game *s, if16 *u, struct Point *p, size_t l);

/* --- Utilities --- */
extern void Game_Weapons_Rewrite(struct Game *s);

/* --- Wait/Refresh --- */
extern void Game_Unit_Wait(   struct Game *s, tnecs_entity_t u);
extern void Game_Unit_Refresh(struct Game *s, tnecs_entity_t u);

#endif /* GAME_UNIT_H */
