#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "SDL.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct dtab;

/* --- WEAPON/ITEM --- */

/* --- Constructors/Destructors --- */
void Weapon_Free(struct Weapon *wpn);
void Weapon_Init(struct Weapon *wpn);

/* --- Handing --- */
b32 Weapon_TwoHand_Only(const Weapon *wpn);
b32 Weapon_OneHand_Only(const Weapon *wpn);

/* --- isCan? --- */
u16 Weapon_TypeExp(          const Weapon *wpn);
b32 Weapon_canInfuse(        struct Weapon *wpn,  struct Inventory_item *item);
b32 Weapon_canAttack(        struct Weapon *wpn);
b32 Weapon_canAttackfromID(  struct Weapon *wpn);
b32 Weapon_canAttackfromType(struct Weapon *wpn);
b32 Weapon_canWeakhand(i32 id);

b32 Weapon_isStaff(   i32 id);
b32 Weapon_isShield(  i32 id);
b32 Weapon_isTrinket( i32 id);
b32 Weapon_isOffhand( i32 id);
b32 Weapon_ID_isValid(i32 id);

/* --- I/O --- */
void Weapon_readJSON( void *input, const cJSON *json);
void Weapon_writeJSON(const void *const input, cJSON *json);

// Note: all input dtab to be more general.
//  - Can load from gl_dtab, or local dtab
void Weapon_Load(     struct dtab *weapons_dtab, i16 id);
void Weapon_Save(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Load(struct dtab *weapons_dtab);
void Weapons_All_Save(struct dtab *weapons_dtab);
void Weapons_All_Free(struct dtab *weapons_dtab);

void Weapon_Reload(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Reload(struct dtab *weapons_dtab);

s8 Weapon_Filename(s8 filename, i16 id);

/* --- Stats --- */
int Weapon_Stat(         const struct Weapon *wpn, i16 s);
int Weapon_Stat_inRange( const struct Weapon *wpn, i16 s, int d);

/* --- Repair --- */
// Making weapons repairable through MAGIC only is baka.
// Stronger the weapon, HARDER it is to repair though.
// Which items are repairable?
// w/Whetstones:      Swords, axes, lances, Offhands
//                              EXCEPT throwing weapons
// w/Repair kit:      Bows? Shields?
// w/Magic Repair kit:      Magic, Staffs, Trinkets ?
// NOPE:   Shields? Items.
//           OI BOSS COULD YOU SHARPEN MY SHIELD?

void Weapon_Repair(struct Weapon *wpn, struct Inventory_item *item, u8 AP);

#endif /* WEAPON_H */
