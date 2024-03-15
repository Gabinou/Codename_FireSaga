#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include "enums.h"
#include "bitfields.h"
#include "weapon.h"
#include "unit/unit.h"
#include "position.h"
#include "sprite.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- Constructors/Destructors --- */
void Game_Items_Free(  struct dtab **items_dtab);
void Game_Weapons_Free(struct dtab **weapons_dtab);

/* --- Party utilities --- */
void Party_Load(struct Unit *party, struct dtab *weapons_dtab,
                struct dtab *items_dtab, i16 *unit_ids, size_t load_num);

void Game_Party_Load(  struct Game *s, i16 *to_load_ids, size_t load_num);
void Game_Party_Clear( struct Game *s);
void Game_Party_Unload(struct Game *s, i16 *to_unload,   size_t unload_num);

tnecs_entity Game_Party_Entity_Create(struct Game *s, i16 in_unit, struct Point in_pos);

/* --- Unitmap --- */
void Game_putPConMap(struct Game *s, i16 *u, struct Point *p, size_t l);

/* --- Utilities --- */
void Game_Weapons_Rewrite(struct Game *s);

/* --- Wait/Refresh --- */
void Game_Unit_Wait(   struct Game *s, tnecs_entity u);
void Game_Unit_Refresh(struct Game *s, tnecs_entity u);

#endif /* GAME_UNIT_H */
