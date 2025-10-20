#ifndef WEAPON_H
#define WEAPON_H
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
**  Weapons are items that:
**      1. When equipped, enable "Attack" option
**          - i.e. initiate combat
**          - Also, boost stats
**      2. Shields ARE weapons.
**          - Combat related.
**          - Initiate combat with shields?
**  Staves are NOT weapons:
**      2. Staves when equipped, enable "Staff" option
**      - Do staves need a separate struct?
*/

#include "SDL.h"
#include "item.h"
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

b32 Staff_ID_isValid(   i32 id);
b32 Weapon_isShield(    i32 id);
b32 Weapon_isTrinket(   i32 id);
b32 Weapon_isOffhand(   i32 id);
b32 Weapon_ID_isValid(  i32 id);

/* --- Getters --- */
Weapon *_Weapon_Get(i32 id);
Weapon *Weapon_Get(Inventory_item *invitem);
i32     Weapon_Handedness(const Weapon * wpn);
void    Weapon_Handedness_Set(Weapon * wpn, i32 set);
i32 * Weapon_Stats_Arr(const Weapon *weapon);
i32 *_Weapon_Stats_Arr(const struct Weapon_stats *wpn_stats);
i32 _Weapon_stats_Indexing(const Weapon_stats *wpn_stats, i32 stat);
i32  Weapon_stats_Indexing(const Weapon *wpn, i32 stat);

/* --- I/O --- */
void Weapon_readJSON( void *input, const cJSON * json);
void Weapon_writeJSON(const void *const input, cJSON * json);

// Note: all input dtab to be more general.
//  - Can load from gl_dtab, or local dtab
void Weapon_Load(i32 id);
void Weapon_Save(i32 id);
void Weapons_All_Load(void);
void Weapons_All_Save(void);
void Weapons_All_Free(void);

void Weapon_Reload(i32 id);
void Weapons_All_Reload(void);

/* --- Stats --- */
typedef struct WeaponStatGet {
    i32 stat;       /* stat to get          */
    i32 distance;   /* distance to target   */
    i32 hand;       /* one or twohanding    */
    Infusion infusion[MAX_ARMS_NUM];
    i32 infuse_num;
} WeaponStatGet;

/* --- Design note: --- */
// Infusions improve the weapon.
// - NO:  infusions input in equations
// - YES: infusions included in wpn stat
struct Weapon_stats Weapon_Stats_Combine_E(
        tnecs_E wpns_E[MAX_ARMS_NUM],
        i32 num,
        WeaponStatGet get);

struct Weapon_stats Weapon_Stats_Combine(
        const Weapon * wpns[MAX_ARMS_NUM],
        i32 num,
        struct WeaponStatGet get);

struct Weapon_stats Weapon_Stats_Infused(const Weapon    * wpn,
                                         struct Infusion * infusion);

i32 Weapon_Stat_Entity(     tnecs_E     inv,
                            WeaponStatGet    get);
i32 Weapon_Stat(            const Weapon    * wpn,
                            WeaponStatGet    get);
b32 _Weapon_inRange(        const Weapon    * wpn,
                            WeaponStatGet    get);
i32 _Weapon_Infusion(       const Weapon    * wpn,
                            WeaponStatGet    get);
i32 _Weapon_Stat_Raw(       const Weapon    * wpn,
                            WeaponStatGet    get);
i32 _Weapon_Stat_Hand(      const Weapon    * wpn,
                            WeaponStatGet    get);

struct Range Weapon_Range(const struct Weapon * const wpn);

i32 Weapon_Uses(const Weapon * wpn,
                const Inventory_item * inv);

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


void Weapon_Repair(struct Weapon * wpn, struct Inventory_item * item, u8 AP);



#endif /* WEAPON_H */
