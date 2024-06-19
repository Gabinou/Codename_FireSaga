#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "aura.h"
#include "filesystem.h"
#include "SDL2/SDL.h"

/* --- WEAPON/ITEM --- */
struct  Weapon {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    u8 handedness;
    u8 subtype;        /* ex: thrust swords     */
    b32 isMagic   : 1;
    b32 canAttack : 1; /* for special weapons   */
    u16 effective;
    struct Item         *item;
    struct Weapon_stats  stats;
};

typedef struct Weapon Weapon;
extern struct Weapon Weapon_default;

/* --- Constructors/Destructors --- */
void Weapon_Free(struct Weapon *wpn);
void Weapon_Init(struct Weapon *wpn);

/* --- isCan? --- */
u16 Weapon_TypeExp(           struct Weapon *wpn);
b32 Weapon_canInfuse(        struct Weapon *wpn,  struct Inventory_item *item);
b32 Weapon_canAttack(        struct Weapon *wpn);
b32 Weapon_canAttackfromID(  struct Weapon *wpn);
b32 Weapon_canAttackfromType(struct Weapon *wpn);

b32 Weapon_isStaff(   u16 id);
b32 Weapon_isShield(  u16 id);
b32 Weapon_isOffhand( u16 id);
b32 Weapon_ID_isValid(u16 id);

/* --- I/O --- */
void Weapon_readJSON( void *input, cJSON *json);
void Weapon_writeJSON(void *input, cJSON *json);

void Weapon_Load(     struct dtab *weapons_dtab, i16 id);
void Weapon_Save(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Load(struct dtab *weapons_dtab);
void Weapons_All_Save(struct dtab *weapons_dtab);
void Weapons_All_Free(struct dtab *weapons_dtab);

void Weapon_Reload(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Reload(struct dtab *weapons_dtab);

s8 Weapon_Filename(s8 filename, i16 id);

/* --- Stats --- */
int Weapon_Stat(         struct Weapon *wpn, i16 s);
int Weapon_Stat_inRange( struct Weapon *wpn, i16 s, int d);

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
