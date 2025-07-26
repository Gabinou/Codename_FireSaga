#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "SDL.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct dtab;

/* --- WEAPON/ITEM --- */
typedef struct Weapon_Flags {
    i32 effective;  /* Bonus damage vs unit types*/
    // DESIGN QUESTION:
    //  - instead of making magic weapon IMPOSSIBLE to infuse
    //  - Make infused weapon lose durability propto magic power?
    b32 isMagic;
    // Note:
    // - Some weapons need story reason to be usable
    b32 canAttack;
    b32 recharge_every_chapter;
} Weapon_Flags;

typedef struct Weapon {
    struct jsonIO_Header jsonio_header;

    struct Item         item;
    struct Weapon_stats stats;
    struct Weapon_Flags flags;
} Weapon;
extern const struct Weapon Weapon_default;

/* --- Constructors/Destructors --- */
void Weapon_Free(struct Weapon *wpn);
void Weapon_Init(struct Weapon *wpn);

/* --- HandHandHanding --- */
b32 Weapon_TwoHand_Only(const Weapon *wpn);
b32 Weapon_OneHand_Only(const Weapon *wpn);

/* --- isCan? --- */
u16 Weapon_TypeExp(          const Weapon *wpn);
b32 Weapon_canAttack(        struct Weapon *wpn);
b32 Weapon_canAttackfromID(  struct Weapon *wpn);
b32 Weapon_canAttackfromType(struct Weapon *wpn);
b32 Weapon_canWeakhand(i32 id);

b32 Weapon_isStaff(   i32 id);
b32 Weapon_isShield(  i32 id);
b32 Weapon_isTrinket( i32 id);
b32 Weapon_isOffhand( i32 id);
b32 Weapon_ID_isValid(i32 id);

/* --- Getters --- */
i32     Weapon_Handedness(const Weapon *wpn);
void    Weapon_Handedness_Set(Weapon *wpn, i32 set);

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
typedef struct WeaponStatGet {
    i32 stat;       /* stat to get          */
    i32 distance;   /* distance to target   */
    i32 hand;       /* one or twohanding    */
    b32 infuse;     /* add infusion to stat */
    const Infusion  *infusion;
} WeaponStatGet;

/* --- Design note: --- */
// Infusions improve the weapon.
// - NO:  infusions input in equations
// - YES: infusions included in wpn stat
i32 Weapon_Stat_Entity(     tnecs_entity     inv,
                            WeaponStatGet    get);
i32 Weapon_Stat(            const Weapon    *wpn,
                            WeaponStatGet    get);
b32 _Weapon_inRange(        const Weapon    *wpn,
                            WeaponStatGet    get);
i32 _Weapon_Infusion(       const Weapon    *wpn,
                            WeaponStatGet    get);
i32 _Weapon_Stat_Raw(       const Weapon    *wpn,
                            WeaponStatGet    get);
i32 _Weapon_Stat_Hand(      const Weapon    *wpn,
                            WeaponStatGet    get);

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
