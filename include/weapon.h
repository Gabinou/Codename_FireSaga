#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "filesystem.h"
#include "SDL.h"

/* --- WEAPON/ITEM --- */
typedef struct Weapon {
    u8      json_element;  /* JSON_ELEM_bOFFSET = 0 */
    char   *json_filename; /* JSON_ELEM_bOFFSET = 1 */

    u8 handedness;
    u8 subtype;        /* ex: thrust swords     */
    u16 effective;
    bool isMagic   : 1;
    bool canAttack : 1; /* for special weapons   */
    struct Item         *item;
    struct Weapon_stats  stats;
} Weapon;
extern struct Weapon Weapon_default;

/* --- Union and discriminator for Weapons/Items --- */
struct WpnorItem {
    int dtab;
    struct Weapon *wpn;
    struct Item   *item;
};
extern struct WpnorItem WpnorItem_default;

/* --- Constructors/Destructors --- */
void Weapon_SDL_free(struct Weapon *wpn);
void Weapon_Init(struct Weapon *wpn);

/* --- isCan? --- */
u16 Weapon_TypeExp(          const struct Weapon *wpn);
bool Weapon_canInfuse(        const struct Weapon *wpn, const struct Inventory_item *item);
bool Weapon_canAttack(              struct Weapon *wpn);
bool Weapon_canAttackfromID(  const struct Weapon *wpn);
bool Weapon_canAttackfromType(const struct Weapon *wpn);

bool Weapon_isStaff(   u16 id);
bool Weapon_isShield(  u16 id);
bool Weapon_isOffhand( u16 id);
bool Weapon_ID_isValid(u16 id);

/* --- I/O --- */
void Weapon_readJSON(       void *input, const cJSON *const json);
void Weapon_writeJSON(const void *input,       cJSON       *json);

void Weapon_Load(     struct dtab *weapons_dtab, i16 id);
void Weapon_Save(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Load(struct dtab *weapons_dtab);
void Weapons_All_Save(struct dtab *weapons_dtab);
void Weapons_All_SDL_free(struct dtab *weapons_dtab);

void Weapon_Reload(     struct dtab *weapons_dtab, i16 id);
void Weapons_All_Reload(struct dtab *weapons_dtab);

s8 Weapon_Filename(s8 filename, i16 id);

/* --- Stats --- */
int Weapon_Stat(        const struct Weapon *wpn, i16 s);
int Weapon_Stat_inRange(const struct Weapon *wpn, i16 s, int d);

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
