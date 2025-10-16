
#include "log.h"
#include "enums.h"
#include "nmath.h"
#include "names.h"
#include "jsonio.h"
#include "weapon.h"
#include "macros.h"
#include "globals.h"
#include "structs.h"
#include "platform.h"
#include "equations.h"

#include "unit/range.h"

/* Note: magic weapons EXPLODE if infused */
const struct Weapon Weapon_default = {
    .jsonio_header.json_element   = JSON_WEAPON,

    .flags.canAttack    = 1,
};

/* --- Constructors/Destructors --- */
void Weapon_Init(struct Weapon *weapon) {
    SDL_assert(weapon != NULL);
    *weapon         = Weapon_default;
    weapon->item    = Item_default;
}

void Weapon_Free(struct Weapon *weapon) {
    if (weapon == NULL) {
        return;
    }

    s8_free(&weapon->jsonio_header.json_filename);
    Item_Free(&weapon->item);
}

/* --- isCan? --- */
b32 Weapon_canAttack(struct Weapon *weapon) {
    weapon->flags.canAttack  = Weapon_canAttackfromType(weapon);
    weapon->flags.canAttack *= Weapon_canAttackfromID(weapon);
    return (weapon->flags.canAttack);
}

b32 Weapon_canAttackfromType(struct Weapon *weapon) {
    SDL_assert(weapon);
    b32 iscan = flagsum_isIn(Item_Typecode(&weapon->item), ITEM_ARCHETYPE_WEAPON);
    return (iscan);
}

b32 Weapon_canAttackfromID(struct Weapon *weapon) {
    SDL_assert(weapon);
    return ((weapon->item.ids.id == ITEM_NULL) || (weapon->item.ids.id == ITEM_ID_BROKEN) ? 0 : 1);
}

/* --- I/O --- */
void Weapon_readJSON(void *input, const cJSON *jwpn) {
    struct Weapon *weapon = (struct Weapon *) input;
    SDL_assert(weapon != NULL);
    SDL_assert(jwpn != NULL);
    Weapon_Init(weapon);
    Item_readJSON(&weapon->item, jwpn);
    cJSON *jstats           = cJSON_GetObjectItemCaseSensitive(jwpn, "Stats");
    cJSON *jsubtype         = cJSON_GetObjectItemCaseSensitive(jwpn, "Subtype");
    cJSON *jeffective       = cJSON_GetObjectItemCaseSensitive(jwpn, "Effective");

    if (jsubtype != NULL)
        weapon->item.type.sub     = cJSON_GetNumberValue(jsubtype);
    if (jstats != NULL)
        Weapon_stats_readJSON(&(weapon->stats), jstats);

    /* Set item range to weapon */
    weapon->item.range.min = weapon->stats.range.min;
    weapon->item.range.max = weapon->stats.range.max;
    if (jeffective != NULL)
        weapon->flags.effective   = cJSON_GetNumberValue(jeffective);

    weapon->flags.canAttack       = Weapon_canAttack(weapon);
}

void Weapon_writeJSON(const void *const input, cJSON *jwpn) {
    const struct Weapon *weapon = (struct Weapon *) input;
    SDL_assert(jwpn         != NULL);
    SDL_assert(weapon       != NULL);

    Item_writeJSON(&weapon->item, jwpn);
    cJSON *jitemstats   = cJSON_CreateObject();
    Weapon_stats_writeJSON(&(weapon->stats), jitemstats);
    Item_stats_writeJSON(&(weapon->item.stats), jitemstats);
    cJSON *jsubtype     = cJSON_CreateNumber(weapon->item.type.sub);
    cJSON *jeffective   = cJSON_CreateNumber(weapon->flags.effective);
    cJSON_AddItemToObject(jwpn, "Stats",        jitemstats);
    cJSON_AddItemToObject(jwpn, "Subtype",      jsubtype);
    cJSON_AddItemToObject(jwpn, "Effective",    jeffective);
}

u16 Weapon_TypeExp(const Weapon * weapon) {
    u64 wpntypecode = Item_Typecode(&weapon->item);

    SDL_assert(wpntypecode > ITEM_NULL);
    /* Double type: SwordOffhand*/

    if ((wpntypecode & ITEM_TYPE_SWORD) && (wpntypecode & ITEM_TYPE_OFFHAND)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDOFFHAND);
    }

    /* Double type: SwordAxe*/
    if ((wpntypecode & ITEM_TYPE_SWORD) && (wpntypecode & ITEM_TYPE_AXE)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDAXE);
    }

    /* Double type: LanceAxe*/
    if ((wpntypecode & ITEM_TYPE_LANCE) && (wpntypecode & ITEM_TYPE_AXE)) {
        return (ITEM_TYPE_EXP_DOUBLE_LANCEAXE);
    }

    /* Single type loop*/
    u16 type_exp = 1;
    while (type_exp < ITEM_TYPE_EXP_END) {
        SDL_assert(type_exp >= 1);
        if (((1UL << (type_exp - 1)) & wpntypecode) > 0)
            break;
        type_exp++;
    }

    return (type_exp);
}

b32 Weapon_isOffhand(i32 id) {
    b32 is = ((id > ITEM_ID_OFFHAND_START) && (id < ITEM_ID_OFFHAND_END));
    return (is);
}

b32 Weapon_isShield(i32 id) {
    b32 is = ((id > ITEM_ID_SHIELD_START) && (id < ITEM_ID_SHIELD_END));
    return (is);
}

b32 Weapon_isTrinket(i32 id) {
    b32 is = ((id > ITEM_ID_SHIELD_START) && (id < ITEM_ID_SHIELD_END));
    return (is);
}

b32 Weapon_canWeakhand(i32 id) {
    /* Weapon can be carried in weakhand without penalty */
    b32 is =    Weapon_isShield(id) ||
                Weapon_isOffhand(id) ||
                Weapon_isTrinket(id);
    return (is);
}

/* Staves are not weapons */
b32 Staff_ID_isValid(i32 id) {
    if ((id <= ITEM_NULL) || (id >= ITEM_ID_END)) {
        return (0);
    }

    return  ((id > ITEM_ID_STAFF_START) &&
             (id < ITEM_ID_STAFF_END));
}

b32 Weapon_ID_isValid(i32 id) {
    if ((id <= ITEM_NULL) || (id >= ITEM_ID_END)) {
        return (0);
    }
    b32 valid = 0;
    // Note: staves aren't weapons i.e. not used in combat
    valid |= ((id > ITEM_ID_SWORD_START)     && (id < ITEM_ID_SWORD_END));
    valid |= ((id > ITEM_ID_LANCE_START)     && (id < ITEM_ID_LANCE_END));
    valid |= ((id > ITEM_ID_AXE_START)       && (id < ITEM_ID_AXE_END));
    valid |= ((id > ITEM_ID_BOW_START)       && (id < ITEM_ID_BOW_END));
    valid |= ((id > ITEM_ID_SHIELD_START)    && (id < ITEM_ID_SHIELD_END));
    valid |= ((id > ITEM_ID_OFFHAND_START)   && (id < ITEM_ID_OFFHAND_END));
    valid |= ((id > ITEM_ID_ELEMENTAL_START) && (id < ITEM_ID_ELEMENTAL_END));
    valid |= ((id > ITEM_ID_ANGELIC_START)   && (id < ITEM_ID_ANGELIC_END));
    valid |= ((id > ITEM_ID_DEMONIC_START)   && (id < ITEM_ID_DEMONIC_END));
    valid |= ((id > ITEM_ID_CLAW_START)      && (id < ITEM_ID_CLAW_END));
    valid |= ((id > ITEM_ID_TRINKET_START)   && (id < ITEM_ID_TRINKET_END));
    valid |= ((id > ITEM_ID_STANDARD_START)  && (id < ITEM_ID_STANDARD_END));
    valid |= ((id > ITEM_ID_SLING_START)     && (id < ITEM_ID_SLING_END));
    valid |= (id == ITEM_ID_GBE);
    return (valid);
}

/* --- Repair --- */
void Weapon_Repair(struct Weapon * wpn, struct Inventory_item * item, u8 AP) {
    /* Repair scaled by item STRENGTH.*/
    /* TODO: hardness equation */
    u8 hardness = Eq_Wpn_Attvar(5,
                                wpn->stats.attack.physical,
                                wpn->stats.attack.magical,
                                wpn->stats.attack.True,
                                wpn->stats.protection.physical,
                                wpn->stats.protection.magical);

    u8 repaired_uses = AP / hardness;
    item->used = repaired_uses > item->used ? 0 : item->used - repaired_uses;
}

/* --- Stats --- */
/* --- Weapon_Stats_Combine ---
**  - Weapon_stats input for ALL computed stats
**      - Includes everything, each hand.
*/
Weapon_stats Weapon_Stats_Combine_E( tnecs_E          E_L,
                                     tnecs_E          E_R,
                                     WeaponStatGet    get) {
    /* -- Getting weapons -- */
    const Weapon *wpn_L = NULL;
    const Weapon *wpn_R = NULL;
    WeaponStatGet newget = get;
    const Inventory_item *item_L = IES_GET_C(gl_world,
                                             E_L,
                                             Inventory_item);
    if (item_L && Weapon_ID_isValid(item_L->id)) {
        wpn_L = DTAB_GET_CONST(gl_weapons_dtab, item_L->id);
        SDL_assert(wpn_L != NULL);
    }
    const Inventory_item *item_R = IES_GET_C(gl_world,
                                             E_L,
                                             Inventory_item);
    if (item_R && Weapon_ID_isValid(item_R->id)) {
        wpn_R = DTAB_GET_CONST(gl_weapons_dtab, item_R->id);
        SDL_assert(wpn_R != NULL);
    }

    /* -- Getting infusions -- */
    Infusion *inf_L = IES_GET_C(gl_world, item_L->id,
                                Infusion);
    Infusion *inf_R = IES_GET_C(gl_world, item_R->id,
                                Infusion);
    newget.infuse_num   = 2;
    newget.infusion[0]  = *inf_L;
    newget.infusion[1]  = *inf_R;

    return (Weapon_Stats_Combine(wpn_L, wpn_R, newget));
}

Weapon_stats Weapon_Stats_Combine(   const Weapon    *wpn_L,
                                     const Weapon    *wpn_R,
                                     WeaponStatGet    get) {
    /* --- All effective weapon stats for two weapons --- */

    /* -- Skip if no weapons -- */
    if ((wpn_L == NULL) && (wpn_R == NULL)) {
        return (Weapon_stats_default);
    }

    /* -- Are both weapons in range? -- */
    b32 inrange_L = _Weapon_inRange(wpn_L, get);
    b32 inrange_R = _Weapon_inRange(wpn_R, get);
    if (!inrange_L && !inrange_R) {
        /* -- Both weapon out of range -- */
        return (Weapon_stats_default);
    }

    /* -- At least one weapon in range -- */

    /* -- Infusing -- */
    Weapon_stats infused_L = Weapon_Stats_Infused(wpn_L,
                                                  &get.infusion[0]);
    Weapon_stats infused_R = Weapon_Stats_Infused(wpn_R,
                                                  &get.infusion[1]);

    /* -- Effective stats in range -- */
    SDL_assert(inrange_L || inrange_R);
    Weapon_stats stats_L = inrange_L ? infused_L :
                           Weapon_stats_default;
    Weapon_stats stats_R = inrange_R ? infused_R :
                           Weapon_stats_default;

    /* -- Are we two handing? -- */
    if (get.hand == WEAPON_HAND_TWO) {
        SDL_assert(inrange_L && inrange_R);
        /* -- Two handing: stats_L == stats_R -- */
        Weapon_stats out = stats_L;

        /* Two handing wgt bonus */
        out.wgt = Eq_Wpn_Two_Handing_Wgt(out.wgt);
        return (out);
    }

    /* -- One handing: combining stats -- */
    SDL_assert(get.hand == WEAPON_HAND_ONE);

    Weapon_stats out = Weapon_stats_default;

    /* Attack: adding */
    out.attack = Damage_Raw_Add(stats_L.attack,
                                stats_R.attack);

    /* Protection: adding */
    out.protection = Damage_Raw_Add(stats_L.protection, stats_R.protection);

    /* Range: combining */
    out.range   = _Ranges_Combine(stats_L.range, stats_R.range);

    /* Hit: averaging */
    out.hit = Eq_Wpn_Hit(stats_L.hit, stats_R.hit);

    /* Dodge: adding */
    out.dodge = Eq_Wpn_Dodge(stats_L.dodge, stats_R.dodge);

    /* Crit: adding */
    out.crit = Eq_Wpn_Crit(stats_L.crit, stats_R.crit);

    /* Favor: adding */
    out.favor = Eq_Wpn_Favor(stats_L.favor, stats_R.favor);

    /* Weight: adding */
    out.wgt = Eq_Wpn_Wgt(stats_L.wgt, stats_R.wgt);

    /* Attack_Physical_2H: adding */

    out.attack_physical_2H = Eq_Wpn_Attack(
                                     stats_L.attack_physical_2H,
                                     stats_R.attack_physical_2H);

    /* prof:    no, effective prof doesn't make sense */
    /* prof_2H: no, effective prof doesn't make sense */
    /* mastery: no, effective mastery doesn't make sense */
    return (out);
}

i32 Weapon_Stat_Entity(     tnecs_E     inv_item,
                            WeaponStatGet    get) {
    /* Read weapon stat, w/bonuses, from entity */
    if (inv_item == TNECS_NULL) {
        return (0);
    }
    WeaponStatGet newget    = get;
    const Inventory_item    *item   = IES_GET_C(gl_world, inv_item, Inventory_item);
    SDL_assert(item);
    SDL_assert(Weapon_ID_isValid(item->id));
    const Weapon            *wpn    = DTAB_GET_CONST(gl_weapons_dtab, item->id);
    SDL_assert(wpn != NULL);
    Infusion *infusion = IES_GET_C( gl_world, inv_item,
                                    Infusion);

    newget.infusion[0] = *infusion;
    return (Weapon_Stat(wpn, newget));
}

i32 Weapon_Stat(const struct Weapon *wpn,
                WeaponStatGet        get) {
    if (wpn == NULL) {
        return (0);
    }
    /* -- Is weapon in range? -- */
    if (!_Weapon_inRange(wpn, get)) {
        /* -- Weapon out of range -- */
        return (0);
    }

    /* -- Weapon in range -- */
    /* Read weapon stat, w/infusion */
    i32 stat_hand = _Weapon_Stat_Hand(wpn, get);
    if (stat_hand == 0) {
        /* stat is 0, don't add infusion */
        return (0);
    }

    /* -- Is weapon infused? -- */
    i32 infusion = _Weapon_Infusion( wpn, get);
    if (infusion == 0) {
        /* -- Weapon not infused -- */
        return (stat_hand);
    }

    /* -- Weapon is infused -- */
    /* - Preventing Double dipping - */
    //  - i.e. increasing both attack & protection
    b32 isshield = Weapon_isShield(wpn->item.ids.id);
    b32 attack_stat =
            (get.stat == WEAPON_STAT_pATTACK) ||
            (get.stat == WEAPON_STAT_mATTACK);
    b32 prot_stat =
            (get.stat == WEAPON_STAT_pPROTECTION) ||
            (get.stat == WEAPON_STAT_mPROTECTION);

    if (isshield && attack_stat) {
        // Shields: infusion does not affect:
        //  - pAttack
        //  - mAttack
        return (stat_hand);
    }
    if (!isshield && prot_stat) {
        // Weapons: infusion does not affect:
        //  - pProtection
        //  - mProtection
        return (stat_hand);
    }

    /* -- Weapon infusion applied to correct stat -- */
    i32 infused     =  Eq_Wpn_Infuse(stat_hand, infusion);
    return (infused);
}

Weapon_stats Weapon_Stats_Infused(const Weapon  *wpn,
                                  Infusion      *infusion) {
    /* --- Get stats for weapon, including infusions ---
    ** Note: Just adds infusions to >zero stats */
    Weapon_stats out = wpn->stats;
    if (infusion == NULL) {
        return (out);
    }

    b32 isshield = Weapon_isShield(wpn->item.ids.id);
    if (isshield) {
        /* -- Weapon is a shield, infusing prot -- */
        if (out.protection.physical > 0) {
            out.protection.physical = Eq_Wpn_Infuse(
                                              out.protection.physical,
                                              infusion->physical);
        }
        if (out.protection.magical > 0) {
            out.protection.magical  = Eq_Wpn_Infuse(
                                              out.protection.magical,
                                              infusion->magical);
        }
        return (out);
    }

    /* -- Not a shield, infusing attack -- */
    if (out.attack_physical_2H > 0) {
        out.attack_physical_2H = Eq_Wpn_Infuse(
                                         out.attack_physical_2H,
                                         infusion->physical);
    }
    if (out.attack.physical > 0) {
        out.attack.physical = Eq_Wpn_Infuse(
                                      out.attack.physical,
                                      infusion->physical);
    }
    if (out.attack.magical > 0) {
        out.attack.magical  = Eq_Wpn_Infuse(
                                      out.attack.magical,
                                      infusion->magical);
    }

    return (out);
}

i32 _Weapon_Infusion(   const Weapon    *wpn,
                        WeaponStatGet    get) {
    /* Get infusion bonus for input weapon stat */
    if (get.infusion == NULL) {
        return (0);
    }
    /* Infusion for attacking weapons i.e. non-shields */
    if (get.stat == WEAPON_STAT_pATTACK) {
        return (get.infusion->physical);
    }
    if (get.stat == WEAPON_STAT_mATTACK) {
        return (get.infusion->magical);
    }

    b32 isshield = Weapon_isShield(wpn->item.ids.id);

    if (!isshield) {
        return (0);
    }

    /* Weapon is a shield */
    if (get.stat == WEAPON_STAT_pPROTECTION) {
        return (get.infusion->physical);
    }
    if (get.stat == WEAPON_STAT_mPROTECTION) {
        return (get.infusion->magical);
    }

    return (0);
}

i32 _Weapon_Stat_Raw(const Weapon * weapon,
                     WeaponStatGet    get) {
    /* Read weapon.stat directly */
    SDL_assert((get.stat > ITEM_STAT_START) && (get.stat < WEAPON_STAT_END));

    if ((get.stat > ITEM_STAT_START) && (get.stat < ITEM_STAT_END)) {
        return (Item_Stat(&weapon->item, get.stat));
    }

    i32 *wpn_stats_arr = (i32 *)&weapon->stats.attack;
    int stat = wpn_stats_arr[get.stat - WEAPON_STAT_START - 1];
    return (stat);
}

i32 _Weapon_Stat_Hand(  const Weapon    * wpn,
                        WeaponStatGet    get) {
    /* Gives weapon stat for proper hand */
    // Weapons can only ever be used in
    // one or two hands

    /* Alternative stats for two hands */
    //  1. prof: twohanding is easier/harder
    //  2. pattack: stronger with two hands
    // Magic weapons: no benefits
    // Shields: can't two hand

    /* Update get.stat for two handing stats */
    /* TODO: Implement 2H stat reading. */
    if (get.hand == WEAPON_HAND_TWO) {
        if (get.stat == WEAPON_STAT_PROF) {
            return (wpn->stats.prof_2H);
        } else if (get.stat == WEAPON_STAT_pATTACK) {
            return (wpn->stats.attack_physical_2H);
        }
    }

    return (_Weapon_Stat_Raw(wpn, get));
}

b32 _Weapon_inRange(const Weapon    *weapon,
                    WeaponStatGet    get) {
    /* Gives raw weapon stat if distance is in range.
    *  Shields and offhands are always in range.
    *    DEBUG: input -1 to always be in_range
    */
    if (get.distance < 0) {
        // for debug, negative always in range
        return (1);
    }

    b32 isshield  = Weapon_isShield(weapon->item.ids.id);
    if (isshield) {
        return (1);
    }

    b32 isoffhand = Weapon_isOffhand(weapon->item.ids.id);
    if (isoffhand) {
        return (1);
    }

    struct Range range = weapon->stats.range;

    /* b32 in_range = ((range.min <= get.distance) && (get.distance <= range.max)); */
    if (inRange_Dist(range, get.distance)) {
        return (1);
    }

    return (0);
}

/* --- Handing --- */
/* Can weapon be onehanded? */
b32 Weapon_TwoHand_Only(const Weapon * wpn) {
    return (Item_TwoHand_Only(&wpn->item));
}

/* Can weapon be twohanded? */
b32 Weapon_OneHand_Only(const Weapon * wpn) {
    return (Item_OneHand_Only(&wpn->item));
}

i32 Weapon_Handedness(const Weapon * wpn) {
    SDL_assert(wpn != NULL);
    return (Item_Handedness(&wpn->item));
}

void Weapon_Handedness_Set(Weapon * wpn, i32 set) {
    return (Item_Handedness_Set(&wpn->item, set));
}

// Range of weapon, for attacking
struct Range Weapon_Range(const Weapon * const wpn) {
    return (wpn->stats.range);
}

i32 Weapon_Uses(const Weapon            *wpn,
                const Inventory_item    *invitem) {
    return (Pure_Item_Uses(&wpn->item, invitem));
}

/* --- Getter --- */
Weapon *Weapon_Get(Inventory_item *invitem) {
    if (invitem == NULL) {
        return (NULL);
    }
    return (_Weapon_Get(invitem->id));
}

Weapon *_Weapon_Get(i32 id) {
    if (!Weapon_ID_isValid(id)) {
        return (NULL);
    }

    return (DTAB_GET(gl_weapons_dtab, id));
}

void Weapons_All_Free(void) {

}