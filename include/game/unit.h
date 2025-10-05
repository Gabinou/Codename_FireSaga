#ifndef GAME_UNIT_H
#define GAME_UNIT_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Top level unit coordination
**  - Ex: Putting PC unit Es on map
*/

#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;
struct dtab;

/* --- Constructors/Destructors --- */
void Game_Items_Free(  struct dtab **items_dtab);
void Game_Weapons_Free(struct dtab **weapons_dtab);

/* --- Party utilities --- */
void Game_Party_Free(  struct Game *s);
void Game_Party_Load(  struct Game *s);
void Game_Party_Clear( struct Game *s);
void Game_Party_Unload(struct Game *s, i16 *to_unload,   size_t unload_num);
tnecs_E Game_Party_Entity_Create(struct Game *s);
void         Game_Party_Entity_Init(struct Game *s,
                                    tnecs_E ent,
                                    s8 f);

/* -- Loaded units utilities -- */
void Game_Loaded_Units_Free(struct Game *sota);

/* --- Unitmap --- */
void Game_putPConMap(struct Game *s, i16 *u, struct Point *p, size_t l);

/* --- Item --- */
b32 Game_Unit_canUse_Item(          struct Game *s,
                                    const   struct Item *item,
                                    const   struct Unit *user);

/* --- Utilities --- */
void Game_Weapons_Rewrite(  struct Game     *s);
void Game_Map_Unit_Load(    struct Game     *s,
                            tnecs_E     ent,
                            s8               filename);


/* --- Wait/Refresh --- */
void Game_Unit_Wait(   struct Game *s, tnecs_E u);
void Game_Unit_Refresh(struct Game *s, tnecs_E u);

#endif /* GAME_UNIT_H */
