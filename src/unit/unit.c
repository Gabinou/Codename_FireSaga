
#include "unit/unit.h"
#include "unit/loadout.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/equipment.h"
#include "unit/loadout.h"
#include "unit/status.h"
#include "nmath.h"
#include "reinforcement.h"
#include "RNG.h"
#include "log.h"
#include "macros.h"
#include "jsonio.h"
#include "equations.h"
#include "names.h"
#include "item.h"
#include "weapon.h"

// TODO: use `names/classes.h ` to fill
const int class_mvt_types[UNIT_CLASS_END] = {
    /* NULL              */ UNIT_MVT_FOOT_SLOW,
    /* VILLAGER          */ UNIT_MVT_FOOT_SLOW,
    /* LORD              */ UNIT_MVT_FOOT_SLOW,
    /* DUKE              */ UNIT_MVT_FOOT_SLOW,
    /* ARCHER            */ UNIT_MVT_FOOT_SLOW,
    /* MARKSMAN          */ UNIT_MVT_FOOT_SLOW,
    /* SQUIRE            */ UNIT_MVT_ARMORS,
    /* KNIGHT            */ UNIT_MVT_ARMORS,
    /* FENCER            */ UNIT_MVT_FOOT_SLOW,
    /* DUELIST           */ UNIT_MVT_FOOT_SLOW,
    /* PICKPOCKET        */ UNIT_MVT_FOOT_FAST,
    /* THIEF             */ UNIT_MVT_FOOT_FAST,
    /* CORSAIR           */ UNIT_MVT_BANDITS,
    /* VIKING            */ UNIT_MVT_BANDITS,
    /* BANDIT            */ UNIT_MVT_BANDITS,
    /* RAVAGER           */ UNIT_MVT_BANDITS,
    /* PIKEMAN           */ UNIT_MVT_FOOT_SLOW,
    /* HOPLITE           */ UNIT_MVT_FOOT_SLOW,
    /* STANDARD_BEARER   */ UNIT_MVT_FOOT_SLOW,
    /* DANCER            */ UNIT_MVT_FOOT_SLOW,
    /* MAGE              */ UNIT_MVT_MAGES,
    /* BATTLEMAGE        */ UNIT_MVT_MAGES,
    /* PRIEST            */ UNIT_MVT_MAGES,
    /* BISHOP            */ UNIT_MVT_MAGES,
    /* CLERIC            */ UNIT_MVT_MAGES,
    /* ORACLE            */ UNIT_MVT_MAGES,
    /* DEMONIC_INCARNATE */ UNIT_MVT_MAGES,
    /* DEMON             */ UNIT_MVT_FOOT_FAST,
    /* ANGELIC_INCARNATE */ UNIT_MVT_MAGES,
    /* ANGEL             */ UNIT_MVT_FLIERS,
    /* KEWAC_RUNNER      */ UNIT_MVT_FOOT_SLOW,
    /* NULL              */ UNIT_MVT_FOOT_SLOW,
    /* KEWAC_HUNTER      */ UNIT_MVT_MAGES,
    /* KEWAC_WARRIOR     */ UNIT_MVT_MAGES,
    /* WOLFSPEAKER       */ UNIT_MVT_MAGES,
    /* KESIRU_RIDER      */ UNIT_MVT_MAGES,
    /* KESIRU_WARRIOR    */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* DEMIGOD           */ UNIT_MVT_MAGES,
    /* GOD               */ UNIT_MVT_MAGES,
    /* GODDESS           */ UNIT_MVT_MAGES,
    /* TWINBORN          */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* VESTAL            */ UNIT_MVT_MAGES,
    /* WOLF              */ UNIT_MVT_FOOT_FAST,
    /* GLADIATORLEGIONARY*/ UNIT_MVT_FOOT_SLOW,
    /* GLADIATOR         */ UNIT_MVT_FOOT_FAST,
};

const struct Unit Unit_default = {
    .jsonio_header.json_element   = JSON_UNIT,
    .mvt_type       = UNIT_MVT_FOOT_SLOW,
    .class          = UNIT_CLASS_VILLAGER,

    .rangemap       = RANGEMAP_ATTACKMAP,

    .dft_pos        = {-1, -1},

    .army           = 1,

    .current_hp         = SOTA_MIN_HP,
    .current_stats.hp   = SOTA_MIN_HP,

    .alive          = true,

    .arms_num       = UNIT_ARMS_NUM,
    .handedness     = UNIT_HAND_RIGHTIE,
    ._hands          = {true, true},

    .update_stats   = true,
};

const struct Unit Nibal_unit = {
    .jsonio_header.json_element = JSON_UNIT,
    /*              hp str mag agi dex fth luck def res con move prof */
    .base_stats     = {35, 20, 20, 18, 25, 14, 12, 18, 22, 30, 06, 15},

    .base_exp       = 2500,
    .army           = ARMY_HAMILCAR,
    ._id            = UNIT_ID_NIBAL,

    .current_hp     = 35,
    .alive          = true,
    .sex            = true,  /* 0:F, 1:M. eg. hasPenis. */

    ._hands          = {true, true},
};

void Tetrabrachios_default(Unit *unit) {
    *unit = Unit_default;
    unit->arms_num = TETRABRACHIOS_ARMS_NUM;
    Unit_Hand_Set(unit, TETRABRACHIOS_HAND_LEFT,  true);
    Unit_Hand_Set(unit, TETRABRACHIOS_HAND_RIGHT, true);
}

/* -- Hand -- */
/* Other hands than the main two can't be strong/weak hand */
int Unit_Hand_Strong(struct Unit *unit) {
    SDL_assert(unit != NULL);
    return ((unit->handedness == UNIT_HAND_LEFTIE) ? UNIT_HAND_LEFT : UNIT_HAND_RIGHT);
}

int Unit_Hand_Weak(struct Unit *unit) {
    SDL_assert(unit != NULL);
    return ((unit->handedness == UNIT_HAND_LEFTIE) ? UNIT_HAND_RIGHT : UNIT_HAND_LEFT);
}

b32 Unit_hasHand(Unit *unit, i32 hand) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms_num);
    SDL_assert(hand <= UNIT_ARMS_NUM);
    return (unit->_hands[hand - UNIT_HAND_LEFT]);
}

void Unit_Hand_Set(Unit *unit, i32 hand, b32 has) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms_num);
    SDL_assert(hand <= UNIT_ARMS_NUM);

    unit->_hands[hand - UNIT_HAND_LEFT] = has;
}

/* --- Constructors/Destructors --- */
void Tetrabrachios_Init(struct Unit *unit) {
    SDL_assert(unit != NULL);
    Unit_Free(unit);
    Tetrabrachios_default(unit);
    Unit_Members_Alloc(unit);
}

void Unit_Init(struct Unit *unit) {
    SDL_assert(unit != NULL);
    Unit_Free(unit);
    *unit = Unit_default;
    Unit_Members_Alloc(unit);
}

void Unit_Members_Alloc(struct Unit *unit) {
    if (unit->grown_stats == NULL) {
        unit->grown_stats   = DARR_INIT(unit->grown_stats,  struct Unit_stats, SOTA_MAX_LEVEL / 8);
    }

    if (unit->status_queue == NULL) {
        unit->status_queue  = DARR_INIT(unit->status_queue, struct Unit_status, 2);
    }

    if (unit->bonus_stack == NULL) {
        unit->bonus_stack   = DARR_INIT(unit->bonus_stack,  struct Bonus_Stats, 2);
    }
    // TODO only allocate IF playable?
    //  - unless enemies have growth haha
    if (unit->grown_stats == NULL) {
        unit->grown_stats = DARR_INIT(unit->grown_stats, struct Unit_stats, SOTA_MAX_LEVEL / 8);
    }
}

void Unit_Free(struct Unit *unit) {
    SDL_assert(unit != NULL);
    if (unit->bonus_stack != NULL) {
        DARR_FREE(unit->bonus_stack);
        unit->bonus_stack = NULL;
    }

    if (unit->grown_stats != NULL) {
        DARR_FREE(unit->grown_stats);
        unit->grown_stats = NULL;
    }

    if (unit->status_queue != NULL) {
        DARR_FREE(unit->status_queue);
        unit->status_queue = NULL;
    }
    if (unit->jsonio_header.json_filename.data != NULL)
        s8_free(&unit->jsonio_header.json_filename);
    s8_free(&unit->name);
    s8_free(&unit->ai_filename);
    unit->_id = 0;
}

void Unit_InitWweapons(struct Unit *unit, struct dtab *weapons_dtab) {
    Unit_Init(unit);
    unit->weapons_dtab = weapons_dtab;
}

void Unit_Reinforcement_Load(struct Unit *unit, struct Reinforcement *reinf) {
    unit->army = reinf->army;
}

i16 Unit_id(struct Unit *unit) {
    if (unit == NULL) {
        return (UNIT_ID_NULL);
    }
    return (unit->_id);
}
void Unit_setid(struct Unit *unit, i16 id) {
    SDL_assert(unit != NULL);
    SDL_assert(Unit_ID_Valid(id));

    unit->_id = id;
    s8_free(&unit->name);
    SDL_assert(unit->name.data == NULL);

    u64 order = *(u64 *)dtab_get(global_unitOrders, id);
    if (unit->name.data != NULL) {
        s8_free(&unit->name);
    }
    unit->name = s8cpy(unit->name, global_unitNames[order]);
}

void Unit_setSkills(struct Unit *unit, u64 skills) {
    SDL_assert(unit);
    unit->skills = skills;
}

void Unit_setClassind(struct Unit *unit, i8 class_index) {
    SDL_assert(unit);
    SDL_assert((class_index > 0) && (class_index < UNIT_CLASS_END));
    unit->class      = class_index;
    unit->mvt_type   = Unit_mvtType(unit);
    unit->equippable = class_equippables[unit->class];

    b32 healclass   = (unit->class == UNIT_CLASS_BISHOP);
    healclass      |= (unit->class == UNIT_CLASS_CLERIC);

    Unit_Rangemap_default(unit);
}

void Unit_setStats(struct Unit *unit, struct Unit_stats stats) {
    SDL_assert(unit);
    unit->current_stats = stats;
    unit->current_hp    = unit->current_stats.hp;
    Unit_effectiveStats(unit);
    Unit_computedStats(unit, -1);
    unit->current_hp = unit->effective_stats.hp;
}

void Unit_setBases(struct Unit *unit, struct Unit_stats stats) {
    SDL_assert(unit);
    unit->base_stats = stats;
    unit->current_hp = unit->base_stats.hp;
}

struct Unit_stats Unit_getStats(struct Unit *unit) {
    struct Unit_stats out_stats = unit->current_stats;
    SDL_assert(unit);
    Unit_effectiveStats(unit);
    // Unit_stats_plus(out_stats,  unit->aura.unit_stats);
    // Unit_stats_minus(out_stats, unit->malus_stats);
    return (out_stats);
}

/* --- Second-order info --- */
u8 Unit_mvtType( struct Unit *unit) {
    return (class_mvt_types[unit->class]);
}

u8 SotA_army2alignment(u8 army) {
    SDL_assert((army > ARMY_START) && (army < ARMY_END));
    return (army_alignment[army]);
}

b32 SotA_isPC(u8 army) {
    SDL_assert((army > ARMY_START) && (army < ARMY_END));
    return (army_isPC[army]);
}

/* --- Skills --- */
b32 Unit_hasSkill( struct Unit *unit, u64 skill) {
    return ((unit->skills & skill) > 0);
}


/* Makes unit take damage.
* NO DAMAGE COMPUTATION HERE! Crit multiplication responsible by caller.
* Input crit b32 just to determine if unit dies instantly or not.
*/
void Unit_takesDamage(struct Unit *unit, u8 damage, b32 crit) {
    // SDL_Log("%s takes %d damage \n", unit->name.data, damage);
    /* -- Checks -- */
    SDL_assert(unit);
    SDL_assert(unit->current_hp > 0);

    /* -- Actually take damage -- */
    // SDL_Log("unit->current_hp %d", unit->current_hp);
    unit->current_hp = (damage > unit->current_hp) ? 0 : (unit->current_hp - damage);
    /* -- Check for Death or Agony -- */
    if (unit->current_hp == 0) {
        // TODO: Check for frail skill, other things that kill instantly.
        if (crit)
            Unit_dies(unit);
        else
            Unit_agonizes(unit);
    }
    // SDL_Log("unit->current_hp %d", unit->current_hp);
    // SDL_Log("unit->agonizes %d", unit->agony);
    // SDL_Log("unit->alive %d", unit->alive);
}

void Unit_getsHealed(struct Unit *unit, u8 healing) {
    // SDL_Log("%s gets healed for %d\n", unit->name.data, healing);
    /* -- Checks -- */
    SDL_assert(unit);

    // TODO: Overheal
    /* -- Actually heal -- */
    int missing_hp   = unit->current_stats.hp - unit->current_hp;
    unit->current_hp = healing > missing_hp ? unit->current_stats.hp : unit->current_hp + healing;
}

void Unit_wait(struct Unit *unit) {
    SDL_assert(unit);
    unit->waits         = true;
    unit->show_danger   = false;
}

void Unit_refresh(struct Unit *unit) {
    SDL_assert(unit);
    unit->waits = false;
    unit->show_danger = false;
}

i16 Unit_getLvl(struct Unit *unit) {
    SDL_assert(unit);
    return (ceil(unit->exp / SOTA_100PERCENT) + 1);
}

void Unit_gainExp(struct Unit *unit, u16 exp) {
    SDL_assert(unit);
    if (((unit->exp % SOTA_100PERCENT) + exp) > SOTA_100PERCENT) {
        /* Never should have two level ups at one time.*/
        Unit_lvlUp(unit);
    }
    unit->exp += exp;
}

void Unit_supportUp(struct Unit *unit, i16 id) {
    SDL_assert(unit);
    int i;
    for (i = 0; i < unit->support_num; i++) {
        if (id == unit->supports[i].other_id) {
            unit->supports[i].level += 1;
            break;
        }
    }

    if (id != unit->supports[i].other_id) {
        SDL_Log("Supporting Unit not found");
        exit(ERROR_Generic);
    }

}
struct RNG_Sequence *Unit_sequences_arr(Unit *unit) {
    struct RNG_Sequence *sequences = (struct RNG_Sequence *)&unit->rng_sequence.hp -
                                     (UNIT_STAT_NULL + 1);
    return (sequences);
}

// This function is ugly as sin. TODO: Refactor this. Make more understandable.
void Unit_lvlUp(struct Unit *unit) {
    SDL_assert(unit != NULL);
    SDL_assert(unit->grown_stats != NULL);

    struct Unit_stats grows = Unit_stats_default;
    u8 temp_growth;
    struct Unit_stats temp_stats = {0};
    i32 *growths        = Unit_stats_arr(&unit->growths);
    i32 *grows_arr      = Unit_stats_arr(&grows);
    i32 *stats_arr      = Unit_stats_arr(&temp_stats);
    i32 *caps_stats     = Unit_stats_arr(&unit->caps_stats);
    i32 *current_stats  = Unit_stats_arr(&unit->current_stats);
    struct RNG_Sequence *sequences = Unit_sequences_arr(unit);

    for (int i = UNIT_STAT_NULL + 1; i <= UNIT_STAT_NUM; i++) {
        /* Skip if stat is capped */
        if (current_stats[i] >= caps_stats[i])
            continue;

        /* Add one stat if growths > 100 (Works even above 200 growth) */
        stats_arr[i] += growths[i] / SOTA_100PERCENT;

        /* Roll for remainder growth */
        temp_growth   = growths[i] % SOTA_100PERCENT;
        SDL_assert(temp_growth < SOTA_100PERCENT);

        struct RNG_Sequence *sequence;
        if (rng_sequence_breaker_miss_growth) {
            /* Compute growths rise for sequence of missed growths */
            sequence = (sequences + i);
            temp_growth = sequence->eff_rate = SB_GROWTH_RISE(temp_growth, sequence->len);
        }

        grows_arr[i]  = (temp_growth >= RNG_URN());

        if (rng_sequence_breaker_miss_growth)
            RNG_checkSequence_oneWay(sequence, grows_arr[i]);

        stats_arr[i] += grows_arr[i];
    }

    /* -- Actually adding grown stats to current_stats -- */
    // for (int i = UNIT_STAT_NULL + 1; i <= UNIT_STAT_NUM; i++) {
    //     current_stats[i] += stats_arr[i];
    // }
    unit->current_stats = Unit_stats_plus(unit->current_stats, temp_stats);

    /* -- Adding current lvlup to all grown stats -- */
    DARR_PUT(unit->grown_stats, temp_stats);
}

void Unit_agonizes(struct Unit *unit) {
    unit->agony = 1;
    // SDL_Log("%s is agonizing. %d turns until death\n", unit->name.data, unit->computed_stats.agony);
}

void Unit_dies(struct Unit *unit) {
    SDL_assert(unit);
    unit->alive = false;
    // SDL_Log("%s is dead.\n", unit->name.data);
}


b32 Unit_canCarry(struct Unit *savior, struct Unit *victim) {
    struct Unit_stats victim_stats = Unit_effectiveStats(victim);
    struct Unit_stats savior_stats = Unit_effectiveStats(savior);
    return (Equation_canCarry(savior_stats.con, victim_stats.con));
}

b32 Unit_canDance(struct Unit *unit) {
    b32 out = (unit->class == UNIT_CLASS_DANCER);

    return (out);
}

/* - Unit has any staff in Equipment? - */
int Unit_canStaff_Eq( struct  Unit *unit) {
    for (int i = ITEM1; i < SOTA_EQUIPMENT_SIZE; i++) {
        struct Inventory_item item = unit->_equipment[i];
        if (Weapon_isStaff(item.id)) {
            return (true);
        }
    }
    return (false);
}

/* - Canstaff only if a staff is equipped in strong hand. - */
int Unit_canStaff(struct Unit *unit) {
    i32 stronghand = Unit_Hand_Strong(unit);
    b32 out = false;
    if (Unit_isEquipped(unit, stronghand)) {
        struct Inventory_item item = unit->_equipment[stronghand];
        out = Weapon_isStaff(item.id);
    }

    return (out);
}

/* - Can unit equip a staff with only one hand? - */
b32 Unit_canStaff_oneHand(Unit *unit) {
    return (Unit_hasSkill(unit, PASSIVE_SKILL_STAFF_ONE_HAND));
}

b32 Unit_canMagic_oneHand(Unit *unit) {
    return (Unit_hasSkill(unit, PASSIVE_SKILL_MAGIC_ONE_HAND));
}

/* - Any Weapon to attack with in equipment - */
b32 Unit_canAttack_Eq(struct Unit *unit) {
    SDL_assert(unit != NULL);
    SDL_assert(unit->weapons_dtab != NULL);
    /* - If any item in equipment is a weapon, can attack - */
    for (int i = ITEM1; i < SOTA_EQUIPMENT_SIZE; i++) {
        struct Inventory_item item = unit->_equipment[i];
        if (item.id == ITEM_NULL)
            continue;

        /* Archetype checking*/
        if (!Item_isWeapon(unit->_equipment[i].id))
            continue;

        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, item.id);

        if (!wpn->canAttack)
            continue;

        return (true);
    }

    return (false);
}

/* - Can unit attack with equipped weapons - */
b32 Unit_canAttack(struct Unit *unit) {
    SDL_assert(unit != NULL);
    SDL_assert(unit->weapons_dtab != NULL);

    for (int hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (_Unit_canAttack(unit, hand)) {
            // SDL_Log("CanAttack!");
            return (true);
        }
    }

    return (false);
}

/* - Can unit attack with weapon in hand - */
b32 _Unit_canAttack(struct Unit *unit, i32 hand) {
    if (!Unit_hasHand(unit, hand)) {
        // SDL_Log("No hands");
        return (false);
    }

    if (!Unit_isEquipped(unit, hand)) {
        // SDL_Log("!Unit_isEquipped");
        return (false);
    }

    i32 id = Unit_Id_Equipped(unit,  hand);
    if (!Weapon_ID_isValid(id)) {
        // SDL_Log("!Weapon_ID_isValid");
        return (false);
    }

    Weapon_Load(unit->weapons_dtab, id);

    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, id);
    SDL_assert(wpn != NULL);
    if (!wpn->canAttack) {
        // SDL_Log("!wpn->canAttack");
        return (false);
    }

    return (true);
}

i32 *Unit_Shield_Protection(struct Unit *unit, i32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return (NULL);

    i16 id = Unit_Id_Equipped(unit, hand);
    SDL_assert(Weapon_ID_isValid(id));
    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

    /* should be equivalent to using archetype */
    if (!flagsum_isIn(weapon->item->type, ITEM_TYPE_SHIELD))
        return (NULL);

    return (weapon->stats.protection);
}

i32 *Unit_computeDefense(struct Unit *unit) {
    /* Reset unit protections */
    unit->computed_stats.protection[DMG_TYPE_PHYSICAL] = 0;
    unit->computed_stats.protection[DMG_TYPE_MAGICAL]  = 0;

    /* Shield protection */
    int prot_P = 0, prot_M = 0;

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        i32 *prot;
        if (prot = Unit_Shield_Protection(unit, hand)) {
            prot_P += prot[DMG_TYPE_PHYSICAL];
            prot_M += prot[DMG_TYPE_MAGICAL];
        }
    }

    /* Add all bonuses */
    i32 bonus_P = 0, bonus_M = 0;
    SDL_assert(unit->bonus_stack != NULL);
    for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
        bonus_P += unit->bonus_stack[i].computed_stats.protection[DMG_TYPE_PHYSICAL];
        bonus_M += unit->bonus_stack[i].computed_stats.protection[DMG_TYPE_MAGICAL];
    }

    /* Adding shield protection to effective stats */
    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.protection[DMG_TYPE_PHYSICAL] = Equation_Weapon_Defensevar(3, prot_P,
                                                         effstats.def, bonus_P);
    unit->computed_stats.protection[DMG_TYPE_MAGICAL]  = Equation_Weapon_Defensevar(3, prot_M,
                                                         effstats.res, bonus_M);
    return (unit->computed_stats.protection);
}

i32 *Unit_computeAttack(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    /* Reset unit attacks */
    unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = 0;
    unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = 0;
    unit->computed_stats.attack[DMG_TYPE_TRUE]     = 0;

    /* Weapon attack */
    u8 *attack;
    int attack_P = 0, attack_M = 0, attack_T = 0;

    struct Weapon *weapon;
    /* Get stats of both weapons */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon   = DTAB_GET(unit->weapons_dtab, id);
        attack_P += Weapon_Stat_inRange(weapon, WEAPON_STAT_pATTACK, distance);
        attack_M += Weapon_Stat_inRange(weapon, WEAPON_STAT_mATTACK, distance);
        attack_T += Weapon_Stat_inRange(weapon, WEAPON_STAT_tATTACK, distance);
    }

    /* -- Twohanding -- */
    // DESIGN: weapon stats in both hands added
    // even when teo handing
    //  if (Unit_istwoHanding(unit)) {
    /*                   / 2 -> cause attack value is added twice */
    //   attack_P = (attack_P / 2) * TWO_HANDING_MIGHT_FACTOR;
    //    attack_M = (attack_M / 2) * TWO_HANDING_MIGHT_FACTOR;
    //  }

    /* --- TRUE DAMAGE --- */
    /* -- FENCER SKILLS -- */
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->skills, UNIT_SKILL_PINPRICK))
        attack_T += SOTA_SKILL_PINPRICK;
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->skills, UNIT_SKILL_PIERCE))
        attack_T += SOTA_SKILL_PIERCE;
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->skills, UNIT_SKILL_CRUSH) && Unit_istwoHanding(unit))
        attack_T += SOTA_SKILL_CRUSH;

    /* -- Adding weapon attack to effective stats -- */
    struct Unit_stats effstats = unit->effective_stats;

    /* Add all bonuses */
    i32 bonus_P = 0, bonus_M = 0, bonus_T = 0;
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus_P += unit->bonus_stack[i].computed_stats.attack[0];
            bonus_M += unit->bonus_stack[i].computed_stats.attack[1];
            bonus_T += unit->bonus_stack[i].computed_stats.attack[2];
        }
    }

    /* No attacking with only fists -> 0 attack means don't add str/mag */
    if (attack_P > 0) {
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = Equation_Weapon_Attackvar(3, attack_P,
                                                         effstats.str, bonus_P);
    }

    if (attack_M > 0) {
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = Equation_Weapon_Attackvar(3, attack_M,
                                                         effstats.mag, bonus_M);
    }
    unit->computed_stats.attack[DMG_TYPE_TRUE] = Equation_Weapon_Attackvar(2, attack_T, bonus_T);

    /* -- DUAL WIELDING -- */
    /* Terrible malus if dual wielding without skill */
    b32 candualwield = TNECS_ARCHETYPE_HAS_TYPE(unit->skills, UNIT_SKILL_DUAL_WIELD);
    if (Unit_isdualWielding(unit) && !candualwield) {
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
    }

    i32 *att = unit->computed_stats.attack;
    att[DMG_TYPE_TOTAL] = att[DMG_TYPE_PHYSICAL] + att[DMG_TYPE_MAGICAL] + att[DMG_TYPE_TRUE];

    return (unit->computed_stats.attack);
}

b32 Unit_Equipment_Full( struct Unit *unit) {
    return (unit->num_equipment == SOTA_EQUIPMENT_SIZE);
}

void Unit_Equipment_Print( struct Unit *unit) {
    SDL_assert(unit != NULL);
    for (int eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        if (unit->_equipment[eq].id == ITEM_NULL) {
            SDL_Log("%d ITEM_NULL", eq);
            continue;
        }

        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[eq].id);
        if (wpn == NULL) {
            SDL_Log("%d Unloaded", eq);
            continue;
        }

        SDL_Log("%d %s", eq, wpn->item->name);
    }
}

struct Computed_Stats Unit_computedStats_wLoadout(Unit *unit, Loadout *loadout, int dist) {
    /* Save starting equipment */
    i32 start_equipped[UNIT_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    /* Compute stats with input loadout */
    Unit_Loadout_Import(unit, loadout);
    Unit_computedStats(unit, dist);

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);

    return (unit->computed_stats);
}

/* Computed stats at distance (-1 is always in range) */
// Implicitely for weapons. Staves only care about range -> compute directly.
struct Computed_Stats Unit_computedStats(struct Unit *unit, int distance) {
    SDL_assert(unit);
    if (!unit->update_stats) {
        return (unit->computed_stats);
    }

    /* Weapon-dependent stats */
    if (Unit_canAttack(unit)) {
        Unit_computeHit(     unit,  distance);
        Unit_computeAttack(  unit,  distance);
        Unit_computeCritical(unit,  distance);
        Unit_Range_Equipped(unit, ITEM_ARCHETYPE_WEAPON);
    } else {
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = 0;
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = 0;
        unit->computed_stats.attack[DMG_TYPE_TRUE]     = 0;
        unit->computed_stats.attack[DMG_TYPE_TOTAL]    = 0;
        unit->computed_stats.hit                       = 0;
        unit->computed_stats.crit                      = 0;
        unit->computed_stats.range_equipment.min       = 0;
        unit->computed_stats.range_equipment.max       = 0;
        unit->computed_stats.range_loadout.min         = 0;
        unit->computed_stats.range_loadout.max         = 0;
    }

    /* Distance-dependent stats */
    Unit_computeSpeed(unit,   distance);
    Unit_computeDodge(unit,   distance);
    Unit_computeFavor(unit,   distance);

    /* Distance-independent stats */
    Unit_computeMove(unit);
    Unit_computeAgony(unit);
    Unit_computeDefense(unit);
    Unit_computeRegrets(unit);

    return (unit->computed_stats);
}

/* Add regrets to computed stats. */
i32 Unit_computeRegrets(struct Unit *unit) {
    SDL_assert(unit);
    /* Pre-computation */
    i8 malus = Equation_Regrets(unit->regrets, unit->effective_stats.fth);
    struct Computed_Stats stats = unit->computed_stats;

    /* Apply regrets malus to computed stats */
    unit->computed_stats.hit   = stats.hit   > malus ? stats.hit               - malus : 0;
    unit->computed_stats.dodge = stats.dodge > malus + INT8_MIN  ? stats.dodge - malus : INT8_MIN;
    unit->computed_stats.crit  = stats.crit  > malus ? stats.crit              - malus : 0;
    unit->computed_stats.favor = stats.favor > malus ? stats.favor             - malus : 0;
    return (malus);
}

i32 Unit_computeHit(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    i32 hits[MAX_ARMS_NUM] = {0};
    struct Weapon *weapon;

    /* Get stats of both weapons */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id      = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon      = DTAB_GET(unit->weapons_dtab, id);
        /* Combine hit of both weapons */
        hits[hand]  = Weapon_Stat_inRange(weapon, WEAPON_STAT_HIT, distance);
    }

    i32 wpn_hit = Equation_Weapon_Dodgearr(hits, MAX_ARMS_NUM);

    /* Add all bonuses */
    SDL_assert(unit->bonus_stack != NULL);
    i32 bonus   = 0;
    for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
        bonus += unit->bonus_stack[i].computed_stats.hit;
    }

    /* Compute hit */
    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.hit   = Equation_Unit_Hit(wpn_hit, effstats.dex, effstats.luck, bonus);

    return (unit->computed_stats.hit);
}

i32 Unit_computeDodge(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    i32 bonus       = 0, tile_dodge = 0;
    i32 wgts[MAX_ARMS_NUM]      = {0};
    i32 dodges[MAX_ARMS_NUM]    = {0};
    struct Weapon *weapon;

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon          = DTAB_GET(unit->weapons_dtab, id);
        dodges[hand]    = Weapon_Stat_inRange(weapon, WEAPON_STAT_DODGE, distance);
        wgts[hand]      = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }

    i32 wpn_dodge   = Equation_Weapon_Dodgearr(dodges, MAX_ARMS_NUM);
    i32 wpn_wgt     = Equation_Weapon_Wgtarr(wgts, MAX_ARMS_NUM);

    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus += unit->bonus_stack[i].computed_stats.dodge;
        }
    }

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.dodge = Equation_Unit_Dodge(wpn_wgt, wpn_dodge, effstats.luck,
                                                     effstats.fth, effstats.agi, effstats.str,
                                                     effstats.con, tile_dodge, bonus);
    return (unit->computed_stats.dodge);
}

i32 Unit_computeCritical(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    // TODO: get support bonus
    i32 bonus = 0;
    i32 crits[MAX_ARMS_NUM] = {0};
    struct Weapon *weapon;

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon          = DTAB_GET(unit->weapons_dtab, id);
        crits[hand]     = Weapon_Stat_inRange(weapon, WEAPON_STAT_CRIT, distance);
    }

    u8 wpn_crit = Equation_Weapon_Critarr(crits, MAX_ARMS_NUM);

    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus += unit->bonus_stack[i].computed_stats.crit;
        }
    }

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.crit = Equation_Unit_Crit(wpn_crit, effstats.dex, effstats.luck, bonus);
    return (unit->computed_stats.crit);
}

i32 Unit_computeFavor(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    i32 bonus = 0 ;
    i32 favors[MAX_ARMS_NUM] = {0};
    struct Weapon *weapon;

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon          = DTAB_GET(unit->weapons_dtab, id);
        favors[hand]    = Weapon_Stat_inRange(weapon, WEAPON_STAT_FAVOR, distance);
    }

    i32 wpn_favor = Equation_Weapon_Favorarr(favors, MAX_ARMS_NUM);

    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus += unit->bonus_stack[i].computed_stats.favor;
        }
    }

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.favor = Equation_Unit_Favor(wpn_favor, effstats.fth, bonus);
    return (unit->computed_stats.favor);
}

i32 Unit_computeAgony(struct Unit *unit) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);

    i32 bonus = 0;
    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus += unit->bonus_stack[i].computed_stats.agony;
        }
    }

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.agony = Equation_Agony_Turns(effstats.str, effstats.def, effstats.con, bonus);
    return (unit->computed_stats.agony);
}

i32 Unit_computeSpeed(struct Unit *unit, int distance) {
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    i32 bonus = 0;
    i32 wgts[MAX_ARMS_NUM]      = {0};
    struct Weapon *weapon;

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        weapon          = DTAB_GET(unit->weapons_dtab, id);
        wgts[hand]      = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }

    /* item weight in both hands is always added */
    i32 wpn_wgt     = Equation_Weapon_Wgtarr(wgts, MAX_ARMS_NUM);
    if (Unit_istwoHanding(unit))
        wpn_wgt /= TWO_HANDING_WEIGHT_FACTOR;

    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            bonus += unit->bonus_stack[i].computed_stats.speed;
        }
    }

    // if (TNECS_ARCHETYPE_HAS_TYPE(unit->skills, UNIT_SKILL_)) {
    // TODO: compute effective_weight
    struct Unit_stats fstats = unit->effective_stats;
    unit->computed_stats.speed = Equation_Unit_Speed(wpn_wgt, fstats.agi,
                                                     fstats.con, fstats.str,
                                                     bonus);
    return (unit->computed_stats.speed);
}

i32 Unit_computeMove(struct Unit *unit) {
    SDL_assert(unit);
    i8 move = unit->effective_stats.move;
    if (unit->mount != NULL)
        move = MOVE_WITH_MOUNT;
    unit->computed_stats.move = move;
    return (unit->computed_stats.move);
}

/* --- I/O --- */
void Unit_readJSON(void *input,  cJSON *junit) {
    struct Unit *unit = (struct Unit *)input;
    SDL_assert(unit->grown_stats != NULL);
    SDL_assert(unit);
    // SDL_Log("-- Get json objects --");
    cJSON *jid              = cJSON_GetObjectItem(junit, "id");
    cJSON *jai              = cJSON_GetObjectItem(junit, "AI");
    cJSON *jarmy            = cJSON_GetObjectItem(junit, "army");
    cJSON *jsex             = cJSON_GetObjectItem(junit, "Sex");
    cJSON *jexp             = cJSON_GetObjectItem(junit, "Exp");
    cJSON *jname            = cJSON_GetObjectItem(junit, "Name");
    cJSON *jitems           = cJSON_GetObjectItem(junit, "Items");
    cJSON *jgrowths         = cJSON_GetObjectItem(junit, "Growths");
    cJSON *jequipped        = cJSON_GetObjectItem(junit, "Equipped");
    cJSON *jlevelups        = cJSON_GetObjectItem(junit, "Level-ups");
    cJSON *jsupports        = cJSON_GetObjectItem(junit, "Supports");
    cJSON *jbase_exp        = cJSON_GetObjectItem(junit, "BaseExp");
    cJSON *jcurrent_hp      = cJSON_GetObjectItem(junit, "CurrentHP");
    cJSON *jbase_stats      = cJSON_GetObjectItem(junit, "Bases");
    cJSON *jcaps_stats      = cJSON_GetObjectItem(junit, "Caps");
    cJSON *jclass_index     = cJSON_GetObjectItem(junit, "Class Index");
    cJSON *jsupport_type    = cJSON_GetObjectItem(junit, "Support Type");
    cJSON *jcurrent_stats   = cJSON_GetObjectItem(junit, "Stats");

    cJSON *jhandedness      = cJSON_GetObjectItem(junit, "Handedness");
    cJSON *jhands           = cJSON_GetObjectItem(junit, "Hands");

    /* --- Hands --- */
    if (jhandedness != NULL) {
        unit->handedness = cJSON_GetNumberValue(jhandedness);
    }

    if (jhands != NULL) {
        if (!cJSON_IsArray(jhands)) {
            SDL_Log("Unit \"Hands\" should be a JSON array.");
            exit(1);
        }

        unit->arms_num = cJSON_GetArraySize(jhands);

        for (int hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
            cJSON *jhand    = cJSON_GetArrayItem(jhands, hand - UNIT_HAND_LEFT);
            Unit_Hand_Set(unit, hand, cJSON_GetNumberValue(jhand));
        }
    }

    /* --- Equipped --- */
    if (jequipped != NULL) {
        if (cJSON_GetArraySize(jequipped) != unit->arms_num) {
            SDL_Log("Unit \"Equipped\" array should have same size as \"Hands Num\".");
            exit(1);
        }
        for (int i = 0; i < unit->arms_num; i++) {
            cJSON *jequippedi  = cJSON_GetArrayItem(jequipped, i);
            unit->_equipped[i] = cJSON_GetNumberValue(jequippedi);
        }
    }

    // SDL_Log("-- setting name from ID --");
    SDL_assert(jid);
    Unit_setid(unit, cJSON_GetNumberValue(jid));
    SDL_assert(unit->name.data != NULL);
    char *json_name     = cJSON_GetStringValue(jname);
    char *ai_filename   = cJSON_GetStringValue(jai);
    if (ai_filename != NULL) {
        // SDL_Log("ai_filename '%s'", ai_filename);
        unit->ai_filename   = s8_mut(ai_filename);
    }
    u64 order = *(u64 *)DTAB_GET(global_unitOrders, unit->_id);
    s8 idname = global_unitNames[order];

    if (!s8equal(unit->name, s8_var(json_name))) {
        SDL_LogError(SOTA_LOG_SYSTEM,
                     "Name in unit filename '%s' does not match id name %d->'%s'",
                     json_name, unit->_id, idname.data);
        exit(ERROR_JSONParsingFailed);
    }

    // SDL_Log("-- startup misc --");
    unit->sex               = cJSON_IsTrue(jsex);
    unit->army              = cJSON_GetNumberValue(jarmy);
    unit->exp               = cJSON_GetNumberValue(jexp);
    unit->base_exp          = cJSON_GetNumberValue(jbase_exp);
    Unit_setClassind(unit, cJSON_GetNumberValue(jclass_index));
    SDL_assert(jcurrent_stats);

    // SDL_Log("-- Supports --");
    unit->support_type = cJSON_GetNumberValue(jsupport_type);
    if (jsupports != NULL) {
        if (!cJSON_IsArray(jsupports)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' supports is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        unit->support_num = cJSON_GetArraySize(jsupports);
        for (int i = 0; i < unit->support_num; ++i) {
            struct cJSON *jsupport = cJSON_GetArrayItem(jsupports, i);
            char *name = cJSON_GetStringValue(jsupport);
            // unit->supports[i].other_id = Hashes_supportName2ID(name);
            unit->supports[i].level         = 0;
            unit->supports[i].other_type    = 0;
        }
    }

    // SDL_Log("--set stats --");
    Unit_stats_readJSON(&unit->current_stats, jcurrent_stats);
    SDL_assert(jcaps_stats);
    Unit_stats_readJSON(&unit->caps_stats, jcaps_stats);
    SDL_assert(jbase_stats);
    Unit_stats_readJSON(&unit->base_stats, jbase_stats);
    SDL_assert(jgrowths);
    Unit_stats_readJSON(&unit->growths, jgrowths);
    // DESIGN QUESTION: Check that current stats fit with bases + levelups?
    //  - No levelups mean NO GRAPHS
    //  => Check if it fits

    if (jcurrent_hp != NULL) {
        unit->current_hp        = cJSON_GetNumberValue(jcurrent_hp);
    }

    cJSON *jlevelup = cJSON_GetObjectItem(jlevelups, "Level-up");
    struct Unit_stats temp_ustats;

    DARR_NUM(unit->grown_stats) = 0;
    while (jlevelup != NULL) {
        Unit_stats_readJSON(&temp_ustats, jlevelup);
        DARR_PUT(unit->grown_stats, temp_ustats);
        jlevelup = jlevelup->next;
    };

    /* -- Unit should also read/write equipped -- */
    // SDL_Log("-- items --");

    /* -- Drop everything, unequip all -- */
    Unit_Equipment_Drop(unit);
    Unit_Unequip_All(unit);

    /* -- Load equipment -- */
    cJSON *jitem;
    cJSON_ArrayForEach(jitem, jitems) {
        struct Inventory_item temp_item;
        Inventory_item_readJSON(&temp_item, jitem);
        if (temp_item.id > ITEM_NULL) {
            Unit_Item_Take(unit, temp_item);
        }
    }

    /* -- Load equipped -- */

}

void Unit_writeJSON(void *input, cJSON *junit) {
    struct Unit *unit = (struct Unit *)input;
    SDL_assert(unit);
    SDL_assert(junit);
    /* --- Hands --- */
    cJSON *jhandedness  = cJSON_CreateNumber(unit->handedness);
    cJSON *jhands       = cJSON_CreateArray();
    cJSON *jequipped    = cJSON_CreateArray();
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms_num; hand++) {
        cJSON *jhand        = cJSON_CreateNumber(Unit_hasHand(unit, hand));
        cJSON *jequippedi   = cJSON_CreateNumber(Unit_Eq_Equipped(unit, hand));
        cJSON_AddItemToArray(jhands,    jhand);
        cJSON_AddItemToArray(jequipped, jequippedi);
    }
    cJSON_AddItemToObject(junit, "Hands",       jhands);
    cJSON_AddItemToObject(junit, "Equipped",    jequipped);
    cJSON_AddItemToObject(junit, "Handedness",  jhandedness);

    cJSON *jid            = cJSON_CreateNumber(unit->_id);
    cJSON *jexp           = cJSON_CreateNumber(unit->base_exp);
    cJSON *jsex           = cJSON_CreateBool(unit->sex);
    cJSON *jname          = cJSON_CreateString(unit->name.data);
    cJSON *jai            = cJSON_CreateString(unit->ai_filename.data);

    cJSON *jclass         = cJSON_CreateString(classNames[unit->class].data);
    cJSON *jbase_exp      = cJSON_CreateNumber(unit->exp);
    cJSON *jcurrent_hp    = cJSON_CreateNumber(unit->current_hp);
    cJSON *jclass_index   = cJSON_CreateNumber(unit->class);
    cJSON *jcurrent_stats = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->current_stats, jcurrent_stats);
    cJSON *jcaps_stats    = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->caps_stats, jcaps_stats);
    cJSON *jbase_stats    = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->base_stats, jbase_stats);
    cJSON *jgrowths       = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->growths, jgrowths);
    cJSON *jgrown         = cJSON_CreateObject();
    cJSON *jlevel         = NULL;
    cJSON *jlevelup       = NULL;
    cJSON_AddItemToObject(junit, "level",       jlevel);
    cJSON_AddItemToObject(junit, "id",          jid);
    cJSON_AddItemToObject(junit, "Name",        jname);
    cJSON_AddItemToObject(junit, "AI",          jai);

    cJSON_AddItemToObject(junit, "Sex",         jsex);
    cJSON_AddItemToObject(junit, "BaseExp",     jbase_exp);
    cJSON_AddItemToObject(junit, "Exp",         jexp);
    cJSON_AddItemToObject(junit, "CurrentHP",   jcurrent_hp);
    cJSON_AddItemToObject(junit, "Class",       jclass);
    cJSON_AddItemToObject(junit, "Class Index", jclass_index);
    cJSON_AddItemToObject(junit, "Stats",       jcurrent_stats);
    cJSON_AddItemToObject(junit, "Caps",        jcaps_stats);
    cJSON_AddItemToObject(junit, "Bases",       jbase_stats);
    cJSON_AddItemToObject(junit, "Growths",     jgrowths);
    cJSON_AddItemToObject(junit, "Level-ups",   jgrown);
    for (u8 i = 0; i < DARR_NUM(unit->grown_stats); i++) {
        jlevelup = cJSON_CreateObject();
        jlevel = cJSON_CreateNumber(i - unit->base_exp / SOTA_100PERCENT + 2);
        cJSON_AddItemToObject(jlevelup, "level", jlevel);
        Unit_stats_writeJSON(&unit->grown_stats[i], jlevelup);
        cJSON_AddItemToObject(jgrown, "Level-up", jlevelup);
        // +2 -> +1 start at lvl1, +1 cause you level to level 2
    }
    cJSON *jitems = cJSON_CreateArray();
    for (u8 item_num = 0; item_num < SOTA_EQUIPMENT_SIZE; item_num ++) {
        cJSON *jitem = cJSON_CreateObject();
        Inventory_item_writeJSON(&unit->_equipment[item_num], jitem);
        cJSON_AddItemToArray(jitems, jitem);
    }
    cJSON_AddItemToObject(junit, "Items", jitems);
}

i32 Unit_computeEffectivefactor(struct Unit *attacker, struct Unit *defender) {
    SDL_assert(attacker);
    SDL_assert(defender);
    u8 effective = NOTEFFECTIVE_FACTOR;
    return (effective);
}

/* Compute Brave factor, c-a-d combat attack multiplier in all combat phases
    Function computes the highest brave factor among equipped weapons.
    This means that -> Brave factor DOES NOT STACK. <-
*/
u8 Unit_Brave(struct Unit *unit) {
    SDL_assert(unit);
    u8 out_brave   = 1;
    u64 temp_effect = 0;
    struct Weapon *weapon;
    // TODO: use AP to compute brave factor
    // TODO: Brave for all hands
    if (Unit_isEquipped(unit, UNIT_HAND_LEFT)) {
        int id = Unit_Id_Equipped(unit, UNIT_HAND_LEFT);
        weapon = DTAB_GET(unit->weapons_dtab, id);
        temp_effect = weapon->item->passive;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X))
            out_brave = 2;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X))
            out_brave = 3;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X))
            out_brave = 4;
    }

    if (Unit_isEquipped(unit, UNIT_HAND_RIGHT)) {
        int id = Unit_Id_Equipped(unit, UNIT_HAND_RIGHT);
        weapon = DTAB_GET(unit->weapons_dtab, id);
        temp_effect = weapon->item->passive;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X))
            out_brave = (out_brave >  2) ? out_brave : 2;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X))
            out_brave = (out_brave >  3) ? out_brave : 3;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X))
            out_brave = 4;
    }
    return (out_brave);
}

void Unit_Cap_Stats(struct Unit *unit) {
    unit->current_stats.hp   = unit->caps_stats.hp;
    unit->current_stats.str  = unit->caps_stats.str;
    unit->current_stats.mag  = unit->caps_stats.mag;
    unit->current_stats.agi  = unit->caps_stats.agi;
    unit->current_stats.dex  = unit->caps_stats.dex;
    unit->current_stats.fth  = unit->caps_stats.fth;
    unit->current_stats.luck = unit->caps_stats.luck;
    unit->current_stats.def  = unit->caps_stats.def;
    unit->current_stats.res  = unit->caps_stats.res;
    unit->current_stats.con  = unit->caps_stats.con;
    unit->current_stats.prof = unit->caps_stats.prof;
}

/* What is this for?
caps before promotion? */
void Unit_HalfCap_Stats(struct Unit *unit) {
    unit->current_stats.hp   = unit->caps_stats.hp   / 2;
    unit->current_stats.str  = unit->caps_stats.str  / 2;
    unit->current_stats.mag  = unit->caps_stats.mag  / 2;
    unit->current_stats.agi  = unit->caps_stats.agi  / 2;
    unit->current_stats.dex  = unit->caps_stats.dex  / 2;
    unit->current_stats.fth  = unit->caps_stats.fth  / 2;
    unit->current_stats.luck = unit->caps_stats.luck / 2;
    unit->current_stats.def  = unit->caps_stats.def  / 2;
    unit->current_stats.res  = unit->caps_stats.res  / 2;
    unit->current_stats.con  = unit->caps_stats.con  / 2;
    unit->current_stats.prof = unit->caps_stats.prof / 2;
}

struct Unit_stats Unit_effectiveGrowths(struct Unit *unit) {
    SDL_assert(unit);
    unit->effective_growths = unit->growths;
    Unit_stats_plus(unit->bonus_growths, unit->effective_growths);
    return (unit->effective_growths);
}

struct Unit_stats Unit_effectiveStats(struct Unit *unit) {
    /* current_stats + all bonuses */
    SDL_assert(unit);

    /* Preparation */
    unit->effective_stats = unit->current_stats;

    /* Add all bonuses */
    if (unit->bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
            unit->effective_stats = Unit_stats_plus(unit->effective_stats, unit->bonus_stack[i].unit_stats);
        }
    }

    /* Add Mount move */
    if (unit->mount != NULL)
        unit->effective_stats.move = unit->mount->move;

    return (unit->effective_stats);
}

void Unit_Promote(struct Unit *unit, i8 new_class_index) {
    SDL_assert(unit);
    // struct Unit_stats promotion_bonus = DTAB_GET(promotion_bonus_stats, new_class_index);
    // DARR_PUT(unit->grown_stats, promotion_bonus);
    // Unit_stats_plus(unit->current_stats, promotion_bonus);
    // unit->skills += DTAB_GET(promotion_bonus_skills, new_class_index);
    // unit->class = new_class_index;
}

/* -- Unit_id -- */
b32 Unit_ID_Valid(u16 id) {
    b32 out =  (id > UNIT_ID_PC_START)      && (id < UNIT_ID_PC_END);
    out |=     (id > UNIT_ID_NPC_START)     && (id < UNIT_ID_NPC_END);
    out |=     (id > UNIT_ID_GENERIC_START) && (id < UNIT_ID_GENERIC_END);
    return (out);
}

b32 Unit_HP_isFull(struct Unit *unit) {
    Unit_effectiveStats(unit);
    return (unit->current_hp >= unit->effective_stats.hp);
}
