
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
const Weapon Weapon_default = {
    .jsonio_header.json_element = JSON_WEAPON,
    .flags.canAttack            = 1,
};

/* --- Constructors/Destructors --- */
void Weapon_Init(Weapon *weapon) {
    IES_nullcheck_void(weapon);

    *weapon         = Weapon_default;
    weapon->item    = Item_default;
}

void Weapon_Free(Weapon *weapon) {
    IES_nullcheck_void(weapon);

    s8_free(&weapon->jsonio_header.json_filename);
    Item_Free(&weapon->item);
}

/* --- Weapon_canAttack_<> ---
** WEAPON can be used to attack in isolation:
**  1. Type check:  Weapon is not a shield
**  2. ID check:    Weapon is not broken
*/
b32 Weapon_canAttack(Weapon *weapon) {
    IES_nullcheck_ret(weapon, 0);

    weapon->flags.canAttack  = Weapon_canAttackfromType(weapon);
    weapon->flags.canAttack *= Weapon_canAttackfromID(weapon);
    return (weapon->flags.canAttack);
}

b32 Weapon_canAttackfromType(Weapon *weapon) {
    IES_nullcheck_ret(weapon, 0);
    b32 iscan = flagsum_isIn(   Item_Type(&weapon->item),
                                ITEM_ARCHETYPE_WEAPON);
    return (iscan);
}

b32 Weapon_canAttackfromID(Weapon *weapon) {
    IES_nullcheck_ret(weapon, 0);
    return ((weapon->item.ids.id == ITEM_NULL) ||
            (weapon->item.ids.id == ITEM_ID_BROKEN) ? 0 : 1);
}

/* --- I/O --- */
void Weapon_readJSON(void *input, const cJSON *jwpn) {
    IES_nullcheck_void(input);
    IES_nullcheck_void(jwpn);

    Weapon *weapon = (Weapon *) input;
    SDL_assert(weapon != NULL);

    Weapon_Init(weapon);
    Item_readJSON(&weapon->item, jwpn);
    cJSON *jstats       = cJSON_GetObjectItemCaseSensitive(jwpn, "Stats");
    cJSON *jsubtype     = cJSON_GetObjectItemCaseSensitive(jwpn, "Subtype");
    cJSON *jeffective   = cJSON_GetObjectItemCaseSensitive(jwpn, "Effective");

    Weapon_stats_readJSON(&(weapon->stats), jstats);

    /* - Set item range to weapon - */
    weapon->item.range.min = weapon->stats.range.min;
    weapon->item.range.max = weapon->stats.range.max;
    if (jeffective != NULL) {
        weapon->flags.effective   = cJSON_GetNumberValue(jeffective);
    }

    Weapon_canAttack(weapon);
}

void Weapon_writeJSON(const void *const input, cJSON *jwpn) {
    IES_nullcheck_void(input);
    IES_nullcheck_void(jwpn);

    const Weapon *weapon = (Weapon *) input;
    IES_nullcheck_void(weapon);

    /* -- Writing Item -- */
    Item_writeJSON(&weapon->item, jwpn);
    /* Note: Item write stats. Weapon re-uses it here */
    cJSON *jstats = cJSON_GetObjectItemCaseSensitive(jwpn, "Stats");
    Weapon_stats_writeJSON(&(weapon->stats), jstats);

    cJSON *jeffective   = cJSON_CreateNumber(weapon->flags.effective);
    cJSON_AddItemToObject(jwpn, "Effective",    jeffective);
}

u16 Weapon_TypeExp(const Weapon *weapon) {
    IES_nullcheck_ret(weapon, 0);

    u64 wpntypecode = Item_Type(&weapon->item);

    SDL_assert(wpntypecode > ITEM_NULL);
    /* Double type: SwordOffhand*/

    if ((wpntypecode & ITEM_TYPE_SWORD) &&
        (wpntypecode & ITEM_TYPE_OFFHAND)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDOFFHAND);
    }

    /* Double type: SwordAxe*/
    if ((wpntypecode & ITEM_TYPE_SWORD) &&
        (wpntypecode & ITEM_TYPE_AXE)) {
        return (ITEM_TYPE_EXP_DOUBLE_SWORDAXE);
    }

    /* Double type: LanceAxe*/
    if ((wpntypecode & ITEM_TYPE_LANCE) &&
        (wpntypecode & ITEM_TYPE_AXE)) {
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
    b32 is =    (id > ITEM_ID_OFFHAND_START) &&
                (id < ITEM_ID_OFFHAND_END);
    return (is);
}

b32 Weapon_isShield(i32 id) {
    /* Must be equivalent to using shield item archetype */
    b32 is =    (id > ITEM_ID_SHIELD_START) &&
                (id < ITEM_ID_SHIELD_END);
    return (is);
}

b32 Weapon_isTrinket(i32 id) {
    b32 is =    (id > ITEM_ID_SHIELD_START) &&
                (id < ITEM_ID_SHIELD_END);
    return (is);
}

b32 Weapon_canWeakhand(i32 id) {
    /* Weapon can be carried in weakhand without penalty */
    b32 is =    Weapon_isShield(id) || Weapon_isOffhand(id) ||
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
    valid |=    (id > ITEM_ID_SWORD_START)     &&
                (id < ITEM_ID_SWORD_END);
    valid |=    (id > ITEM_ID_LANCE_START)     &&
                (id < ITEM_ID_LANCE_END);
    valid |=    (id > ITEM_ID_AXE_START)       &&
                (id < ITEM_ID_AXE_END);
    valid |=    (id > ITEM_ID_BOW_START)       &&
                (id < ITEM_ID_BOW_END);
    valid |=    (id > ITEM_ID_SHIELD_START)    &&
                (id < ITEM_ID_SHIELD_END);
    valid |=    (id > ITEM_ID_OFFHAND_START)   &&
                (id < ITEM_ID_OFFHAND_END);
    valid |=    (id > ITEM_ID_ELEMENTAL_START) &&
                (id < ITEM_ID_ELEMENTAL_END);
    valid |=    (id > ITEM_ID_ANGELIC_START)   &&
                (id < ITEM_ID_ANGELIC_END);
    valid |=    (id > ITEM_ID_DEMONIC_START)   &&
                (id < ITEM_ID_DEMONIC_END);
    valid |=    (id > ITEM_ID_CLAW_START)      &&
                (id < ITEM_ID_CLAW_END);
    valid |=    (id > ITEM_ID_TRINKET_START)   &&
                (id < ITEM_ID_TRINKET_END);
    valid |=    (id > ITEM_ID_STANDARD_START)  &&
                (id < ITEM_ID_STANDARD_END);
    valid |=    (id > ITEM_ID_SLING_START)     &&
                (id < ITEM_ID_SLING_END);
    valid |=    (id == ITEM_ID_GBE);
    return (valid);
}

/* --- Repair --- */
void Weapon_Repair(const Weapon *wpn, InvItem *item, i32 AP) {
    IES_nullcheck_void(wpn);
    IES_nullcheck_void(item);

    i32 hardness    = Eq_Wpn_Repair_Hard(wpn->stats);
    i32 repair      = Eq_Wpn_Repair(hardness, AP);
    InvItem_Repair(item, repair);
}

/* --- Stats --- */
/* --- Weapons_Stats_Eff ---
** Effective stats
**  - Weapon_stats input for ALL computed stats
**      - Includes everything, each hand.
*/
Weapon_stats Weapons_Stats_Eff_E(   tnecs_E wpns_E[MAX_ARMS_NUM],
                                    i32 num, WeaponStatGet get) {
    /* - Skip if input bad - */
    if ((wpns_E == NULL ) || (num == 0)) {
        return (Weapon_stats_default);
    }

    /* -- Getting weapons -- */
    const Weapon *wpns[MAX_ARMS_NUM]  = {0};
    WeaponStatGet newget    = get;
    newget.infuse_num       = num;

    InvItem     *item       = NULL;
    Infusion    *infusion   = NULL;
    for (int i = 0; i < num; i++) {
        item = IES_GET_C(   gl_world, wpns_E[i],
                            InvItem);
        if (!item) {
            continue;
        }

        if (!Weapon_ID_isValid(item->id)) {
            continue;
        }

        Weapon *wpn = DTAB_GET(gl_weapons_dtab, item->id);
        SDL_assert(wpn != NULL);
        wpns[i] = wpn;

        infusion = IES_GET_C(gl_world, wpns_E[i], Infusion);
        if (infusion != NULL) {
            newget.infusion[i]  = *infusion;
        }
    }

    return (Weapons_Stats_Eff(wpns, num, newget));
}

Weapon_stats Weapons_Stats_Eff( const Weapon* wpns[MAX_ARMS_NUM],
                                i32 num, WeaponStatGet get) {
    /* --- All effective weapon stats for multiple weapons --- */

    /* -- Skip if no weapons -- */
    if ((wpns == NULL) || (num == 0) ||
        (num >= MAX_ARMS_NUM)) {
        return (Weapon_stats_default);
    }

    /* -- Are both weapons in range? -- */
    b32 inrange[MAX_ARMS_NUM];
    b32 anyinrange = 0;
    for (int i = 0; i < num; i++) {
        inrange[i] = _Weapon_inRange(wpns[i], get);
        anyinrange |= inrange[i];
    }
    if (!anyinrange) {
        /* -- All weapons out of range -- */
        return (Weapon_stats_default);
    }

    /* -- At least one weapon in range -- */

    /* -- Infusing -- */
    Weapon_stats infused[MAX_ARMS_NUM]  = {0};
    Weapon_stats stats[MAX_ARMS_NUM]    = {0};
    for (int i = 0; i < num; i++) {
        infused[i]  = Weapon_Stats_Infused(wpns[i],
                                           &get.infusion[i]);
        /* -- Effective stats in range -- */
        stats[i]    =   inrange[i] ? infused[i] :
                        Weapon_stats_default;
    }

    /* -- Are we two handing? -- */
    if (get.hand == WEAPON_HAND_TWO) {
        // TODO, twohanding for tetrabrachios?
        SDL_assert(inrange[0]);
        /* -- Two handing: stats_L == stats_R -- */
        Weapon_stats out = stats[0];

        /* Two handing wgt bonus */
        out.wgt = Eq_Wpn_Two_Handing_Wgt(out.wgt);
        return (out);
    }

    /* -- One handing: combining stats -- */
    SDL_assert( (get.hand == WEAPON_HAND_ONE) ||
                (get.hand == WEAPON_HAND_ANY));

    /* - Building stat array - */
    Damage_Raw  attack[MAX_ARMS_NUM]                = {0};
    Damage_Raw  protection[MAX_ARMS_NUM]            = {0};
    Range       range[MAX_ARMS_NUM]                 = {0};
    i32         hit[MAX_ARMS_NUM]                   = {0};
    i32         dodge[MAX_ARMS_NUM]                 = {0};
    i32         crit[MAX_ARMS_NUM]                  = {0};
    i32         favor[MAX_ARMS_NUM]                 = {0};
    i32         wgt[MAX_ARMS_NUM]                   = {0};
    i32         attack_physical_2H[MAX_ARMS_NUM]    = {0};
    i32         prof[MAX_ARMS_NUM]                  = {0};
    i32         prof_2H[MAX_ARMS_NUM]               = {0};
    i32         mastery[MAX_ARMS_NUM]               = {0};

    for (int i = 0; i < num; i++) {
        attack[i]               = stats[i].attack;
        protection[i]           = stats[i].protection;
        range[i]                = stats[i].range;
        hit[i]                  = stats[i].hit;
        dodge[i]                = stats[i].dodge;
        crit[i]                 = stats[i].crit;
        favor[i]                = stats[i].favor;
        wgt[i]                  = stats[i].wgt;
        attack_physical_2H[i]   = stats[i].attack_physical_2H;
        prof_2H[i]              = stats[i].prof_2H;
        prof[i]                 = stats[i].prof;
        mastery[i]              = stats[i].mastery;
    }

    Weapon_stats out = Weapon_stats_default;

    /* Attack: adding */
    out.attack = Damage_Raw_Addarr(attack, num);

    /* Protection: adding */
    out.protection = Damage_Raw_Addarr(protection, num);

    /* Range: combining */
    out.range = _Ranges_Combinearr(range, num);

    /* Hit: averaging */
    out.hit = Eq_Wpn_Hitarr(hit, num);

    /* Dodge: adding */
    out.dodge = Eq_Wpn_Dodgearr(dodge, num);

    /* Crit: adding */
    out.crit = Eq_Wpn_Critarr(crit, num);

    /* Favor: adding */
    out.favor = Eq_Wpn_Favorarr(favor, num);

    /* Weight: adding */
    out.wgt = Eq_Wpn_Wgtarr(wgt, num);

    /* Attack_Physical_2H: adding */
    out.attack_physical_2H = Eq_Wpn_Attackarr(
                                     attack_physical_2H,
                                     num);

    /* prof_2H: adding */
    out.prof_2H = Eq_Wpn_Profarr(prof_2H, num);

    /* eff_prof: Adding */
    out.prof = Eq_Wpn_Profarr(prof, num);

    // Note: not used for Unit_canEquip
    //  - More AS malus from Prof, up to mastery cap
    //  - Harder to get AS bonus when dual wielding

    /* mastery: Adding */
    out.mastery = Eq_Wpn_Masteryarr(mastery, num);

    return (out);
}

i32 Weapon_Stat_Eff_E(     tnecs_E     inv_item,
                           WeaponStatGet    get) {
    /* -- Single weapon effective stat -- */
    if (inv_item == TNECS_NULL) {
        return (0);
    }
    WeaponStatGet newget    = get;
    const InvItem    *item   = IES_GET_C(gl_world, inv_item, InvItem);
    SDL_assert(item);
    SDL_assert(Weapon_ID_isValid(item->id));
    const Weapon            *wpn    = DTAB_GET_CONST(gl_weapons_dtab, item->id);
    SDL_assert(wpn != NULL);
    Infusion *infusion = IES_GET_C( gl_world, inv_item,
                                    Infusion);
    if (infusion) {
        newget.infusion[0]  = *infusion;
        newget.infuse_num   = 1;
    }
    return (Weapon_Stat_Eff(wpn, newget));
}

// TODO: get rid of weapon_stat. Use Weapons_Stats_Eff
i32 Weapon_Stat_Eff(const Weapon *wpn,
                    WeaponStatGet get) {
    IES_nullcheck_ret(wpn, 0);

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
            (get.stat == WEAPON_STAT_pATTACK)   ||
            (get.stat == WEAPON_STAT_mATTACK)   ||
            (get.stat == WEAPON_STAT_pATTACK_2H);
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
    IES_nullcheck_ret(wpn,      Weapon_stats_default);
    IES_nullcheck_ret(infusion, Weapon_stats_default);
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
    IES_nullcheck_ret(wpn, 0);

    /* Get infusion bonus for input weapon stat */
    if (get.infusion == NULL) {
        return (0);
    }
    /* Infusion for attacking weapons i.e. non-shields */
    if ((get.stat == WEAPON_STAT_pATTACK) ||
        (get.stat == WEAPON_STAT_pATTACK_2H)) {
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

i32 *Weapon_Stats_Arr(const Weapon *weapon) {
    IES_nullcheck_ret(weapon, NULL);

    return (_Weapon_Stats_Arr(&weapon->stats));
}

i32 *_Weapon_Stats_Arr(const Weapon_stats *wpn_stats) {
    IES_nullcheck_ret(wpn_stats, NULL);

    return ((i32 *)&wpn_stats->attack);
}

i32 _Weapon_stats_Indexing( const Weapon_stats *wpn_stats,
                            i32 stat) {
    IES_nullcheck_ret(wpn_stats, 0);

    i32 *wpn_stats_arr = _Weapon_Stats_Arr(wpn_stats);
    return (wpn_stats_arr[stat - WEAPON_STAT_START - 1]);
}

i32 Weapon_stats_Indexing(const Weapon *wpn, i32 stat) {
    IES_nullcheck_ret(wpn, 0);

    return (_Weapon_stats_Indexing(&wpn->stats, stat));
}

i32 _Weapon_Stat_Raw(const Weapon *weapon,
                     WeaponStatGet    get) {
    IES_nullcheck_ret(weapon, 0);

    /* Read weapon.stat directly */
    SDL_assert( (get.stat > ITEM_STAT_START) &&
                (get.stat < WEAPON_STAT_END));

    if ((get.stat > ITEM_STAT_START) &&
        (get.stat < ITEM_STAT_END)) {
        return (Item_Stat(&weapon->item, get.stat));
    }

    return (Weapon_stats_Indexing(weapon, get.stat));
}

i32 _Weapon_Stat_Hand(  const Weapon    *wpn,
                        WeaponStatGet    get) {
    IES_nullcheck_ret(wpn, 0);
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
    IES_nullcheck_ret(weapon, 0);
    /* Gives raw weapon stat if distance is in range.
    *  Shields and offhands are always in range.
    *    DEBUG: input -1 to always be in_range
    */
    if (get.distance <= DISTANCE_INVALID) {
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
b32 Weapon_TwoHand_Only(const Weapon *wpn) {
    IES_nullcheck_ret(wpn, 0);

    return (Item_TwoHand_Only(&wpn->item));
}

/* Can weapon be twohanded? */
b32 Weapon_OneHand_Only(const Weapon * wpn) {
    IES_nullcheck_ret(wpn, 0);

    return (Item_OneHand_Only(&wpn->item));
}

i32 Weapon_Handedness(const Weapon * wpn) {
    IES_nullcheck_ret(wpn, 0);

    return (Item_Handedness(&wpn->item));
}

void Weapon_Handedness_Set(Weapon *wpn, i32 set) {
    IES_nullcheck_void(wpn);
    return (Item_Handedness_Set(&wpn->item, set));
}

// Range of weapon, for attacking
struct Range Weapon_Range(const Weapon * const wpn) {
    IES_nullcheck_ret(wpn, Range_default);
    return (wpn->stats.range);
}

i32 Weapon_remUses(const Weapon     *wpn,
                   const InvItem    *invitem) {
    IES_nullcheck_ret(wpn, 0);
    IES_nullcheck_ret(invitem, 0);

    return (Pure_Item_remUses(&wpn->item, invitem));
}

/* --- Getter --- */
Weapon *Weapon_Get(InvItem *invitem) {
    IES_nullcheck_ret(invitem, NULL);
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
