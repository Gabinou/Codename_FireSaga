
#include "unit/unit.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/bonus.h"
#include "unit/flags.h"
#include "unit/status.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
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
#include "globals.h"

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

    .flags.mvt_type     = UNIT_MVT_FOOT_SLOW,
    .id.class           = UNIT_CLASS_VILLAGER,

    .render.rangemap    = RANGEMAP_ATTACKMAP,

    .id.army            =  1,

    .hp.current         = SOTA_MIN_HP,
    .stats.current.hp   = SOTA_MIN_HP,
    .stats.current.move = 1,

    .arms.num           = UNIT_ARMS_NUM,
    .flags.handedness   = UNIT_HAND_RIGHTIE,
    .arms.hands         = {true, true},

    .flags.alive        = true,
};

const struct Unit Nibal_unit = {
    .jsonio_header.json_element = JSON_UNIT,
    /*              hp str mag agi dex fth luck def res con move prof */
    .stats.bases     = {35, 20, 20, 18, 25, 14, 12, 18, 22, 30, 06, 15},

    .level.base_exp = 2500,
    .id.army        = ARMY_HAMILCAR,
    .id.self        = UNIT_ID_NIBAL,

    .hp.current     = 35,
    .flags.alive    = true,
    .flags.sex      = true,  /* 0:F, 1:M. eg. hasPenis. */

    .arms.hands     = {true, true},
};

void Tetrabrachios_default(Unit *unit) {
    *unit = Unit_default;
    unit->arms.num = TETRABRACHIOS_ARMS_NUM;
    Unit_Hand_Set(unit, TETRABRACHIOS_HAND_LEFT,  true);
    Unit_Hand_Set(unit, TETRABRACHIOS_HAND_RIGHT, true);
}

/* -- Hand -- */
/* Other hands than the main two can't be strong/weak hand */
int Unit_Hand_Strong(struct Unit *unit) {
    SDL_assert(unit != NULL);
    return ((Unit_Handedness(unit) == UNIT_HAND_LEFTIE) ? UNIT_HAND_LEFT : UNIT_HAND_RIGHT);
}

int Unit_Hand_Weak(struct Unit *unit) {
    SDL_assert(unit != NULL);
    return ((Unit_Handedness(unit) == UNIT_HAND_LEFTIE) ? UNIT_HAND_RIGHT : UNIT_HAND_LEFT);
}

b32 Unit_hasHand(Unit *unit, i32 hand) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms.num);
    SDL_assert(hand <= UNIT_ARMS_NUM);
    return (unit->arms.hands[hand - UNIT_HAND_LEFT]);
}

void Unit_Hand_Set(Unit *unit, i32 hand, b32 has) {
    SDL_assert(unit != NULL);
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= unit->arms.num);
    SDL_assert(hand <= UNIT_ARMS_NUM);

    unit->arms.hands[hand - UNIT_HAND_LEFT] = has;
}

/* --- Constructors/Destructors --- */
void Tetrabrachios_Init(struct Unit *unit) {
    SDL_assert(unit != NULL);
    Unit_Free(unit);
    Tetrabrachios_default(unit);
    Unit_Members_Alloc(unit);
}

void Unit_Init_tnecs(void *vunit) {
    Unit_Init(vunit);
}

void Unit_Init(Unit *unit) {
    SDL_assert(unit != NULL);
    Unit_Free(unit);
    *unit = Unit_default;
    Unit_Members_Alloc(unit);
}

void Unit_Members_Alloc(struct Unit *unit) {
    if (unit->stats.grown == NULL) {
        unit->stats.grown = DARR_INIT(unit->stats.grown,  struct Unit_stats, SOTA_MAX_LEVEL / 8);
    }

    if (unit->stats.bonus_stack == NULL) {
        unit->stats.bonus_stack = DARR_INIT(unit->stats.bonus_stack,  struct Bonus_Stats, 2);
    }
}

void Unit_Free_tnecs(void *vunit) {
    Unit_Free(vunit);
}

void Unit_Free(struct Unit *unit) {
    SDL_assert(unit != NULL);
    if (unit->stats.bonus_stack != NULL) {
        DARR_FREE(unit->stats.bonus_stack);
        unit->stats.bonus_stack = NULL;
    }

    if (unit->stats.grown != NULL) {
        DARR_FREE(unit->stats.grown);
        unit->stats.grown = NULL;
    }

    if (unit->jsonio_header.json_filename.data != NULL) {
        s8_free(&unit->jsonio_header.json_filename);
        unit->jsonio_header.json_filename.data = NULL;
    }
    Unit_id_set(unit, UNIT_NULL);
}

void Unit_Reinforcement_Load(struct Unit *unit, struct Reinforcement *reinf) {
    Unit_Army_set(unit, reinf->army);
}

void Unit_id_set(struct Unit *unit, i16 id) {
    if (unit == NULL)
        return;
    unit->id.self = id;
}

i16 Unit_id(struct Unit *unit) {
    if (unit == NULL) {
        return (UNIT_ID_NULL);
    }
    return (unit->id.self);
}

void Unit_setSkills(struct Unit *unit, u64 skills) {
    SDL_assert(unit);
    unit->flags.skills = skills;
}

void Unit_setClassind(struct Unit *unit, i8 class_index) {
    SDL_assert(unit);
    SDL_assert((class_index > 0) && (class_index < UNIT_CLASS_END));
    Unit_Class_set(unit, class_index);
    i32 class = Unit_Class(unit);
    unit->flags.mvt_type    = Unit_mvtType(unit);
    unit->flags.equippable  = class_equippables[class];

    // b32 healclass   = (class == UNIT_CLASS_BISHOP);
    // healclass      |= (class == UNIT_CLASS_CLERIC);

    Unit_Rangemap_default(unit);
}

void Unit_setStats(struct Unit *unit, struct Unit_stats stats) {
    SDL_assert(unit);
    unit->stats.current = stats;
    unit->hp.current    = unit->stats.current.hp;
    Unit_stats effective_stats = Unit_effectiveStats(unit);
    Unit_computedStats(unit, -1, effective_stats);
    unit->hp.current    = effective_stats.hp;
}

void Unit_setBases(struct Unit *unit, struct Unit_stats stats) {
    SDL_assert(unit);
    unit->stats.bases = stats;
    unit->hp.current  = unit->stats.bases.hp;
}

/* --- Second-order info --- */
u8 Unit_mvtType( struct Unit *unit) {
    return (class_mvt_types[Unit_Class(unit)]);
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
    return ((unit->flags.skills & skill) > 0);
}


/* Makes unit take damage.
* NO DAMAGE COMPUTATION HERE! Crit multiplication responsible by caller.
* Input crit b32 just to determine if unit dies instantly or not.
*/
void Unit_takesDamage(struct Unit *unit, u8 damage, b32 crit) {

    /* -- Checks -- */
    SDL_assert(unit);
    SDL_assert(unit->hp.current > 0);

    /* -- Actually take damage -- */
    // SDL_Log("unit->hp.current %d", unit->hp.current);
    unit->hp.current = (damage > unit->hp.current) ? 0 : (unit->hp.current - damage);
    /* -- Check for Death or Agony -- */
    if (unit->hp.current == 0) {
        // TODO: Check for frail skill, other things that kill instantly.
        if (crit)
            Unit_dies(unit);
        else
            Unit_agonizes(unit);
    }
    // SDL_Log("unit->hp.current %d", unit->hp.current);
    // SDL_Log("unit->agonizes %d", unit->agony);
    // SDL_Log("unit->alive %d", unit->alive);
}

void Unit_getsHealed(struct Unit *unit, u8 healing) {
    /* -- Checks -- */
    SDL_assert(unit);

    // TODO: Overheal
    /* -- Actually heal -- */
    int missing_hp   = unit->stats.current.hp - unit->hp.current;
    unit->hp.current = healing > missing_hp ? unit->stats.current.hp : unit->hp.current + healing;
}

void Unit_wait(struct Unit *unit) {
    SDL_assert(unit);
    Unit_Waiting_set(unit,      true);
    Unit_showsDanger_set(unit,  false);
}

void Unit_refresh(struct Unit *unit) {
    SDL_assert(unit);
    Unit_Waiting_set(unit,      false);
    Unit_showsDanger_set(unit,  false);
}

i16 Unit_Level(struct Unit *unit) {
    if (!unit) {
        SDL_assert(false);
        return (0);
    }
    return (ceil(unit->level.exp / SOTA_EXP_PER_LEVEL) + 1);
}
i16 Unit_Experience(const Unit *const unit) {
    if (!unit) {
        SDL_assert(false);
        return (0);
    }
    return (unit->level.exp % SOTA_EXP_PER_LEVEL);
}

void Unit_gainExp(struct Unit *unit, u16 exp) {
    SDL_assert(unit);
    if (((unit->level.exp % SOTA_100PERCENT) + exp) > SOTA_100PERCENT) {
        /* Never should have two level ups at one time.*/
        Unit_lvlUp(unit);
    }
    unit->level.exp += exp;
}

void Unit_supportUp(struct Unit *unit, i16 id) {
    SDL_assert(unit);
    int i;
    for (i = 0; i < unit->support.num; i++) {
        if (id == unit->support.arr[i].other_id) {
            unit->support.arr[i].level += 1;
            break;
        }
    }

    if (id != unit->support.arr[i].other_id) {
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
    struct Unit_stats *grown = Unit_Stats_Grown(unit);
    SDL_assert(grown != NULL);

    struct Unit_stats grows = Unit_stats_default;
    u8 temp_growth;
    struct Unit_stats temp_stats = {0};
    i32 *growths        = Unit_stats_arr(Unit_Stats_Growths(unit));
    i32 *grows_arr      = Unit_stats_arr(&grows);
    i32 *stats_arr      = Unit_stats_arr(&temp_stats);
    i32 *caps_stats     = Unit_stats_arr(&unit->stats.caps);
    i32 *current_stats  = Unit_stats_arr(&unit->stats.current);
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
    unit->stats.current = Unit_stats_plus(unit->stats.current, temp_stats);

    /* -- Adding current lvlup to all grown stats -- */
    DARR_PUT(grown, temp_stats);
}

void Unit_agonizes(struct Unit *unit) {
    Unit_computeAgony(unit, &unit->counters.agony);
}

void Unit_dies(struct Unit *unit) {
    SDL_assert(unit);
    unit->flags.alive = false;
}

b32 Unit_canCarry(struct Unit *savior, struct Unit *victim) {
    struct Unit_stats victim_stats = Unit_effectiveStats(victim);
    struct Unit_stats savior_stats = Unit_effectiveStats(savior);
    return (Equation_canCarry(savior_stats.con, victim_stats.con));
}

b32 Unit_canDance(struct Unit *unit) {
    b32 out = (Unit_Class(unit) == UNIT_CLASS_DANCER);
    return (out);
}

/* - Unit has any staff in Equipment? - */
int Unit_canStaff_Eq( struct  Unit *unit) {
    for (int i = ITEM1; i < SOTA_EQUIPMENT_SIZE; i++) {
        Inventory_item *item = Unit_InvItem(unit, i);
        if (Weapon_isStaff(item->id)) {
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
        Inventory_item *item = Unit_InvItem(unit, stronghand);
        out = Weapon_isStaff(item->id);
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
    SDL_assert(gl_weapons_dtab != NULL);
    /* - If any item in equipment is a weapon, can attack - */
    for (int i = ITEM1; i < SOTA_EQUIPMENT_SIZE; i++) {
        Inventory_item *item = Unit_InvItem(unit, i);

        if (item == NULL)
            continue;

        if (item->id == ITEM_NULL)
            continue;

        /* Archetype checking*/
        if (!Item_isWeapon(item->id))
            continue;

        const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, item->id);

        if (!wpn->flags.canAttack)
            continue;

        return (true);
    }

    return (false);
}

/* - Can unit attack with equipped weapons - */
b32 Unit_canAttack(struct Unit *unit) {
    SDL_assert(unit != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    for (int hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
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
    Weapon_Load(gl_weapons_dtab, id);

    const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, id);
    SDL_assert(wpn != NULL);
    if (!wpn->flags.canAttack) {
        // SDL_Log("!wpn->canAttack");
        return (false);
    }

    return (true);
}

Damage_Raw Unit_Shield_Protection(struct Unit *unit, i32 hand) {
    if (!Unit_isEquipped(unit, hand))
        return (Damage_Raw_default);

    i16 id = Unit_Id_Equipped(unit, hand);
    SDL_assert(Weapon_ID_isValid(id));
    Weapon_Load(gl_weapons_dtab, id);
    const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);

    /* should be equivalent to using archetype */
    if (!flagsum_isIn(weapon->item.type.top, ITEM_TYPE_SHIELD))
        return (Damage_Raw_default);

    return (weapon->stats.protection);
}

void Unit_computeDefense(struct Unit *unit, i32* def) {
    /* Reset unit protections */
    Damage_Raw protection   = {0};
    Damage_Raw bonus        = {0};

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        Damage_Raw prot = Unit_Shield_Protection(unit, hand);
        protection.physical   += prot.physical;
        protection.magical    += prot.magical;
    }

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus.physical  = total.computed_stats.protection.physical;
    bonus.magical   = total.computed_stats.protection.magical;

    /* Adding shield protection to effective stats */
    struct Unit_stats effstats = Unit_effectiveStats(unit);
    def[DMG_PHYSICAL] = Equation_Weapon_Defensevar(3,
                                                   protection.physical,
                                                   effstats.def,
                                                   bonus.physical);
    def[DMG_MAGICAL]  = Equation_Weapon_Defensevar(3,
                                                   protection.magical,
                                                   effstats.res,
                                                   bonus.magical);
}

void Unit_computeAttack(struct Unit *unit, int distance, i32* attack) {
    SDL_assert(unit             != NULL);
    SDL_assert(gl_world         != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);
    /* Reset unit attacks */
    Damage_Raw bonus        = {0};
    Damage_Raw wpn_attack   = {0};

    /* Get stats of both weapons */
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        /* Weapon stat */
        tnecs_entity entity = Unit_InvItem_Entity(unit, hand);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        // TODO: two handing.
        //  dodge should not stack!
        get.stat = WEAPON_STAT_pATTACK;
        wpn_attack.physical += Weapon_Stat_Entity(entity, get);
        get.stat = WEAPON_STAT_mATTACK;
        wpn_attack.magical  += Weapon_Stat_Entity(entity, get);
        get.stat = WEAPON_STAT_tATTACK;
        wpn_attack.True     += Weapon_Stat_Entity(entity, get);
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
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->flags.skills, UNIT_SKILL_PINPRICK))
        wpn_attack.True += SOTA_SKILL_PINPRICK;
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->flags.skills, UNIT_SKILL_PIERCE))
        wpn_attack.True += SOTA_SKILL_PIERCE;
    if (TNECS_ARCHETYPE_HAS_TYPE(unit->flags.skills, UNIT_SKILL_CRUSH) && Unit_istwoHanding(unit))
        wpn_attack.True += SOTA_SKILL_CRUSH;

    /* -- Adding weapon attack to effective stats -- */
    struct Unit_stats effstats = Unit_effectiveStats(unit);

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus.physical  = total.computed_stats.attack.physical;
    bonus.magical   = total.computed_stats.attack.magical;
    bonus.True      = total.computed_stats.attack.True;

    /* No attacking with only fists -> 0 attack means don't add str/mag */
    if (wpn_attack.physical > 0) {
        attack[DMG_PHYSICAL] = Equation_Weapon_Attackvar(3, wpn_attack.physical,
                                                         effstats.str, bonus.physical);
        attack[DMG_TRUE] = Equation_Weapon_Attackvar(2, wpn_attack.True, bonus.True);
    }

    if (wpn_attack.magical > 0) {
        attack[DMG_MAGICAL] = Equation_Weapon_Attackvar(3, wpn_attack.magical,
                                                        effstats.mag, bonus.magical);
        attack[DMG_TRUE] = Equation_Weapon_Attackvar(2, wpn_attack.True, bonus.True);
    }

    /* -- DUAL WIELDING -- */
    /* Terrible malus if dual wielding without skill */
    b32 candualwield = TNECS_ARCHETYPE_HAS_TYPE(unit->flags.skills, UNIT_SKILL_DUAL_WIELD);
    if (Unit_isdualWielding(unit) && !candualwield) {
        attack[DMG_PHYSICAL] /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
        attack[DMG_MAGICAL]  /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
    }

    attack[DMG_TOTAL] = attack[DMG_PHYSICAL] + attack[DMG_MAGICAL] + attack[DMG_TRUE];

}

b32 Unit_Equipment_Full( struct Unit *unit) {
    return (unit->equipment.num == SOTA_EQUIPMENT_SIZE);
}

void Unit_Equipment_Print( struct Unit *unit) {
    SDL_assert(unit != NULL);
    for (int eq = ITEM1; eq < SOTA_EQUIPMENT_SIZE; eq++) {
        Inventory_item *item = Unit_InvItem(unit, eq);

        if (item->id == ITEM_NULL) {
            SDL_Log("%d ITEM_NULL", eq);
            continue;
        }

        const struct Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, item->id);
        if (wpn == NULL) {
            SDL_Log("%d Unloaded", eq);
            continue;
        }
        s8 name = Item_Name(wpn->item.ids.id);
        SDL_Log("%d %s", eq, name.data);
    }
}

struct Computed_Stats Unit_computedStats_wLoadout(Unit *unit, Loadout *loadout, int dist) {
    /* Save starting equipment */
    i32 start_equipped[UNIT_ARMS_NUM];
    Unit_Equipped_Export(unit, start_equipped);

    /* Compute stats with input loadout */
    Unit_Loadout_Import(unit, loadout);
    Unit_stats eff_s = Unit_effectiveStats(unit);
    struct Computed_Stats computed_stats = Unit_computedStats(unit, dist, eff_s);

    /* Restore starting equipment */
    Unit_Equipped_Import(unit, start_equipped);

    return (computed_stats);
}

/* Computed stats at distance (-1 is always in range) */
// Implicitly for weapons. Staves only care about range -> compute directly.
struct Computed_Stats Unit_computedStats(struct Unit *unit, int distance, Unit_stats eff_s) {
    SDL_assert(unit);
    struct Computed_Stats computed_stats = {0};

    /* Weapon-dependent stats */
    if (Unit_canAttack(unit)) {
        Unit_computeHit(     unit,  distance, &computed_stats.hit);
        Unit_computeAttack(  unit,  distance, (i32*)&computed_stats.attack);
        Unit_computeCritical(unit,  distance, &computed_stats.crit);
        Unit_Range_Equipped(unit, ITEM_ARCHETYPE_WEAPON, &computed_stats.range_loadout);
    }

    /* Distance-dependent stats */
    Unit_computeSpeed(unit, distance, &computed_stats.speed);
    Unit_computeDodge(unit, distance, &computed_stats.dodge);
    Unit_computeFavor(unit, distance, &computed_stats.favor);

    /* Distance-independent stats */
    Unit_computeMove(unit,      &computed_stats.move);
    Unit_computeAgony(unit,     &computed_stats.agony);
    Unit_computeDefense(unit,   (i32 *)&computed_stats.protection);
    Unit_computeRegrets(unit, &computed_stats, &unit->counters.regrets);

    return (computed_stats);
}

/* Add regrets to computed stats. */
void Unit_computeRegrets(struct Unit *unit, struct Computed_Stats *stats, i32 *regrets) {
    SDL_assert(unit);
    /* Pre-computation */
    i8 malus = Equation_Regrets(unit->counters.regrets, Unit_effectiveStats(unit).fth);

    /* Apply regrets malus to computed stats */
    // TODO: add regrets to equation hit, crit, dodge, favor
    stats->hit   = stats->hit   > malus ? stats->hit - malus : 0;
    stats->dodge = stats->dodge > malus + INT8_MIN  ? stats->dodge - malus : INT8_MIN;
    stats->crit  = stats->crit  > malus ? stats->crit - malus : 0;
    stats->favor = stats->favor > malus ? stats->favor - malus : 0;
    unit->counters.regrets += malus;
}

void Unit_computeHit(struct Unit *unit, int distance, i32 *hit) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);
    i32 hits[MAX_ARMS_NUM] = {0};

    /* Get stats of both weapons */
    for (i32 h = UNIT_HAND_LEFT; h <= unit->arms.num; h++) {
        if (!Unit_isEquipped(unit, h)) {
            continue;
        }

        int id      = Unit_Id_Equipped(unit, h);
        if (!Weapon_ID_isValid(id)) {
            /* items can be equipped, but do not
            ** contribute to computed stats directly */
            continue;
        }

        /* Getting hit stat at distance
        ** Note: hit is averaged, so WEAPON_HAND_ONE
        **       for each hand can always be used. */
        tnecs_entity entity = Unit_InvItem_Entity(unit, h);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        get.stat = WEAPON_STAT_HIT;

        hits[h]  = Weapon_Stat_Entity(entity, get);
    }

    i32 wpn_hit = Equation_Weapon_Hitarr(hits, MAX_ARMS_NUM);

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    i32 bonus = total.computed_stats.hit;

    /* Compute hit */
    struct Unit_stats effstats = Unit_effectiveStats(unit);
    *hit   = Equation_Unit_Hit(wpn_hit, effstats.dex, effstats.luck, bonus);
}

void Unit_computeDodge(struct Unit *unit, int distance, i32 *dodge) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);
    i32 bonus       = 0, tile_dodge = 0;
    i32 wgts[MAX_ARMS_NUM]      = {0};
    i32 dodges[MAX_ARMS_NUM]    = {0};

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        tnecs_entity entity = Unit_InvItem_Entity(unit, hand);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        // TODO: two handing.
        //  dodge should not stack!
        get.stat = WEAPON_STAT_DODGE;
        dodges[hand]    = Weapon_Stat_Entity(entity, get);
        get.stat = WEAPON_STAT_WGT;
        wgts[hand]      = Weapon_Stat_Entity(entity, get);
    }

    i32 wpn_dodge   = Equation_Weapon_Dodgearr(dodges, MAX_ARMS_NUM);
    i32 wpn_wgt     = Equation_Weapon_Wgtarr(wgts, MAX_ARMS_NUM);

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus = total.computed_stats.dodge;

    struct Unit_stats effstats = Unit_effectiveStats(unit);
    *dodge = Equation_Unit_Dodge(wpn_wgt,
                                 wpn_dodge,
                                 effstats.luck,
                                 effstats.fth,
                                 effstats.agi,
                                 effstats.str,
                                 effstats.con,
                                 tile_dodge,
                                 bonus);
}

void Unit_computeCritical(struct Unit *unit, int distance, i32 *crit) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);
    // TODO: get support bonus
    i32 bonus = 0;
    i32 crits[MAX_ARMS_NUM] = {0};

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        tnecs_entity entity = Unit_InvItem_Entity(unit, hand);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        // TODO: two handing.
        //  dodge should not stack!
        get.stat = WEAPON_STAT_CRIT;
        crits[hand] = Weapon_Stat_Entity(entity, get);
    }

    u8 wpn_crit = Equation_Weapon_Critarr(crits, MAX_ARMS_NUM);

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus = total.computed_stats.crit;

    struct Unit_stats effstats = Unit_effectiveStats(unit);
    *crit = Equation_Unit_Crit(wpn_crit, effstats.dex, effstats.luck, bonus);
}

void Unit_computeFavor(struct Unit *unit, int distance, i32 *favor) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);
    i32 bonus = 0 ;
    i32 favors[MAX_ARMS_NUM] = {0};

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        tnecs_entity entity = Unit_InvItem_Entity(unit, hand);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        // TODO: two handing.
        //  dodge should not stack!
        get.stat = WEAPON_STAT_FAVOR;
        favors[hand]    = Weapon_Stat_Entity(entity, get);
    }

    i32 wpn_favor = Equation_Weapon_Favorarr(favors, MAX_ARMS_NUM);

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus = total.computed_stats.favor;

    struct Unit_stats effstats = Unit_effectiveStats(unit);
    *favor = Equation_Unit_Favor(wpn_favor, effstats.fth, bonus);
}

void Unit_computeAgony(struct Unit *unit, i32 *agony) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);

    i32 bonus = 0;
    /* Add all bonuses */
    if (unit->stats.bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->stats.bonus_stack); i++) {
            bonus += unit->stats.bonus_stack[i].computed_stats.agony;
        }
    }

    struct Unit_stats effstats = Unit_effectiveStats(unit);
    *agony = Equation_Agony_Turns(effstats.str, effstats.def, effstats.con, bonus);
}

void Unit_computeSpeed(struct Unit *unit, int distance, i32 *speed) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);
    i32 bonus = 0;
    i32 wgts[MAX_ARMS_NUM]      = {0};

    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        if (!Unit_isEquipped(unit, hand))
            continue;

        int id          = Unit_Id_Equipped(unit, hand);
        SDL_assert(Weapon_ID_isValid(id));
        tnecs_entity entity = Unit_InvItem_Entity(unit, hand);
        WeaponStatGet get = {
            .distance   = distance,
            .hand       = WEAPON_HAND_ONE,
            .infuse     = 1
        };
        // TODO: two handing.
        //  dodge should not stack!
        get.stat = WEAPON_STAT_WGT;
        wgts[hand]      = Weapon_Stat_Entity(entity, get);
    }

    /* item weight in both hands is always added */
    i32 wpn_wgt     = Equation_Weapon_Wgtarr(wgts, MAX_ARMS_NUM);
    if (Unit_istwoHanding(unit)) {
        wpn_wgt /= TWO_HANDING_WEIGHT_FACTOR;
    }

    /* Bonuses total */
    Bonus_Stats total = Unit_Bonus_Total(unit);
    bonus = total.computed_stats.speed;

    // if (TNECS_ARCHETYPE_HAS_TYPE(unit->flags.skills, UNIT_SKILL_)) {
    // TODO: compute effective_weight
    struct Unit_stats fstats = Unit_effectiveStats(unit);
    *speed = Equation_Unit_Speed(wpn_wgt, 
                                 fstats.agi,
                                 fstats.con,
                                 fstats.str,
                                 bonus);
}

void Unit_computeMove(struct Unit *unit, i32 *move) {
    SDL_assert(unit);
    *move = Unit_effectiveStats(unit).move;
    // TODO: compute move with mounts
    // if (unit->mount.ptr != NULL)
    // *move = MOVE_WITH_MOUNT;
}

/* --- I/O --- */
void Unit_readJSON(void *input, const cJSON *junit) {
    struct Unit *unit = (struct Unit *)input;
    struct Unit_stats *grown = Unit_Stats_Grown(unit);
    SDL_assert(grown        != NULL);
    SDL_assert(unit         != NULL);

    // SDL_Log("-- Get json objects --");
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
        unit->flags.handedness = cJSON_GetNumberValue(jhandedness);
    }

    if (jhands != NULL) {
        if (!cJSON_IsArray(jhands)) {
            SDL_Log("Unit \"Hands\" should be a JSON array.");
            exit(1);
        }

        unit->arms.num = cJSON_GetArraySize(jhands);

        for (int hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
            cJSON *jhand    = cJSON_GetArrayItem(jhands, hand - UNIT_HAND_LEFT);
            Unit_Hand_Set(unit, hand, cJSON_GetNumberValue(jhand));
        }
    }

    /* --- Equipped --- */
    if (jequipped != NULL) {
        if (cJSON_GetArraySize(jequipped) != unit->arms.num) {
            SDL_Log("Unit \"Equipped\" array should have same size as \"Hands Num\".");
            exit(1);
        }
        i32 *equipped = Unit_Equipped_Array(unit);
        for (int i = 0; i < unit->arms.num; i++) {
            cJSON *jequippedi  = cJSON_GetArrayItem(jequipped, i);
            equipped[i] = cJSON_GetNumberValue(jequippedi);
        }
    }

    // SDL_Log("-- setting name from ID --");
    char *json_name     = cJSON_GetStringValue(jname);
    Unit_id_set(unit, Unit_Name2ID(s8_var(json_name)));
    char *ai_filename   = cJSON_GetStringValue(jai);
    if (ai_filename != NULL) {
        s8 s8_ai_filename  = s8_var(ai_filename);
        Unit_AI_set(unit, AI_Name2ID(s8_ai_filename));
    }
    u64 order = *(u64 *)DTAB_GET(global_unitOrders, Unit_id(unit));
    s8 idname = global_unitNames[order];

    if (!s8equal(global_unitNames[order], s8_var(json_name))) {
        SDL_LogError(SOTA_LOG_SYSTEM,
                     "Name in unit filename '%s' does not match id name %d->'%s'",
                     json_name, Unit_id(unit), idname.data);
        SDL_assert(false);
        exit(ERROR_JSONParsingFailed);
    }

    // SDL_Log("-- startup misc --");
    unit->flags.sex         = cJSON_IsTrue(jsex);
    Unit_Army_set(unit, cJSON_GetNumberValue(jarmy));
    unit->level.exp         = cJSON_GetNumberValue(jexp);
    unit->level.base_exp    = cJSON_GetNumberValue(jbase_exp);
    Unit_setClassind(unit, cJSON_GetNumberValue(jclass_index));
    SDL_assert(jcurrent_stats);

    // SDL_Log("-- Supports --");
    unit->support.type = cJSON_GetNumberValue(jsupport_type);
    if (jsupports != NULL) {
        if (!cJSON_IsArray(jsupports)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' supports is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        unit->support.num = cJSON_GetArraySize(jsupports);
        for (int i = 0; i < unit->support.num; ++i) {
            // struct cJSON *jsupport = cJSON_GetArrayItem(jsupports, i);
            // char *name = cJSON_GetStringValue(jsupport);
            // unit->supports[i].other_id = Hashes_supportName2ID(name);
            unit->support.arr[i].level         = 0;
            unit->support.arr[i].other_type    = 0;
        }
    }

    // SDL_Log("--set stats --");
    Unit_stats_readJSON(&unit->stats.current, jcurrent_stats);
    SDL_assert(jcaps_stats);
    Unit_stats_readJSON(&unit->stats.caps, jcaps_stats);
    SDL_assert(jbase_stats);
    Unit_stats_readJSON(&unit->stats.bases, jbase_stats);
    SDL_assert(jgrowths);
    Unit_stats_readJSON(Unit_Stats_Growths(unit), jgrowths);
    // DESIGN QUESTION: Check that current stats fit with bases + levelups?
    //  - No levelups mean NO GRAPHS
    //  => Check if it fits

    if (jcurrent_hp != NULL) {
        unit->hp.current        = cJSON_GetNumberValue(jcurrent_hp);
    }

    cJSON *jlevelup = cJSON_GetObjectItem(jlevelups, "Level-up");
    struct Unit_stats temp_ustats;

    DARR_NUM(grown) = 0;
    while (jlevelup != NULL) {
        Unit_stats_readJSON(&temp_ustats, jlevelup);
        DARR_PUT(grown, temp_ustats);
        jlevelup = jlevelup->next;
    };

    /* -- Unit should also read/write equipped -- */
    // SDL_Log("-- items --");

    /* -- Drop everything, unequip all -- */
    Unit_Equipment_Drop(unit);
    Unit_Unequip_All(unit);

    /* -- Load equipment -- */
    cJSON *jitem;
    tnecs_entity entity = TNECS_NULL;
    cJSON_ArrayForEach(jitem, jitems) {
        if (entity == TNECS_NULL) {
            entity = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Inventory_item_ID);
        }
        Inventory_item *item = IES_GET_COMPONENT(gl_world, entity, Inventory_item);
        Inventory_item_readJSON(item, jitem);

        if (item->id > ITEM_NULL) {
            Unit_Item_Take(unit, entity);
            entity = TNECS_NULL;
        }
    }
    if (entity != TNECS_NULL) {
        tnecs_entity_destroy(gl_world, entity);
    }
}

void Unit_writeJSON(const void *input, cJSON *junit) {
    struct Unit *unit = (struct Unit *)input;
    SDL_assert(unit);
    SDL_assert(junit);
    /* --- Hands --- */
    cJSON *jhandedness  = cJSON_CreateNumber(unit->flags.handedness);
    cJSON *jhands       = cJSON_CreateArray();
    cJSON *jequipped    = cJSON_CreateArray();
    for (i32 hand = UNIT_HAND_LEFT; hand <= unit->arms.num; hand++) {
        cJSON *jhand        = cJSON_CreateNumber(Unit_hasHand(unit, hand));
        cJSON *jequippedi   = cJSON_CreateNumber(Unit_Eq_Equipped(unit, hand));
        cJSON_AddItemToArray(jhands,    jhand);
        cJSON_AddItemToArray(jequipped, jequippedi);
    }
    cJSON_AddItemToObject(junit, "Hands",       jhands);
    cJSON_AddItemToObject(junit, "Equipped",    jequipped);
    cJSON_AddItemToObject(junit, "Handedness",  jhandedness);

    cJSON *jid            = cJSON_CreateNumber(Unit_id(unit));
    cJSON *jexp           = cJSON_CreateNumber(unit->level.base_exp);
    cJSON *jsex           = cJSON_CreateBool(Unit_Sex(unit));
    cJSON *jname          = cJSON_CreateString(global_unitNames[Unit_id(unit)].data);
    i32 ai = Unit_AI_Type(unit);
    SDL_assert(ai >= 0);
    s8 ai_filename        = ai_names[ai];
    cJSON *jai            = cJSON_CreateString(ai_filename.data);
    cJSON *jclass         = cJSON_CreateString(classNames[Unit_Class(unit)].data);
    cJSON *jbase_exp      = cJSON_CreateNumber(unit->level.exp);
    cJSON *jcurrent_hp    = cJSON_CreateNumber(unit->hp.current);
    cJSON *jclass_index   = cJSON_CreateNumber(Unit_Class(unit));
    cJSON *jcurrent_stats = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->stats.current, jcurrent_stats);
    cJSON *jcaps_stats    = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->stats.caps, jcaps_stats);
    cJSON *jbase_stats    = cJSON_CreateObject();
    Unit_stats_writeJSON(&unit->stats.bases, jbase_stats);
    cJSON *jgrowths       = cJSON_CreateObject();
    Unit_stats_writeJSON(Unit_Stats_Growths(unit), jgrowths);
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
    struct Unit_stats *grown = Unit_Stats_Grown(unit);
    for (u8 i = 0; i < DARR_NUM(grown); i++) {
        jlevelup = cJSON_CreateObject();
        jlevel = cJSON_CreateNumber(i - unit->level.base_exp / SOTA_100PERCENT + 2);
        cJSON_AddItemToObject(jlevelup, "level", jlevel);
        Unit_stats_writeJSON(&grown[i], jlevelup);
        cJSON_AddItemToObject(jgrown, "Level-up", jlevelup);
        // +2 -> +1 start at lvl1, +1 cause you level to level 2
    }
    cJSON *jitems = cJSON_CreateArray();
    for (u8 item_num = 0; item_num < SOTA_EQUIPMENT_SIZE; item_num ++) {
        cJSON *jitem = cJSON_CreateObject();
        const Inventory_item *item = Unit_InvItem(unit, item_num + ITEM1);
        if (NULL == item) {
            item = &Inventory_item_default;
        }
        Inventory_item_writeJSON(item, jitem);
        cJSON_AddItemToArray(jitems, jitem);
    }
    cJSON_AddItemToObject(junit, "Items", jitems);
}

void Unit_computeEffectivefactor(struct Unit *attacker, struct Unit *defender, i32 *factor) {
    SDL_assert(attacker);
    SDL_assert(defender);
    // TODO implement
    *factor = NOTEFFECTIVE_FACTOR;
}

/* Compute Brave factor, c-a-d combat attack multiplier in all combat phases
    Function computes the highest brave factor among equipped weapons.
    This means that -> Brave factor DOES NOT STACK. <-
*/
u8 Unit_Brave(struct Unit *unit) {
    SDL_assert(unit);
    SDL_assert(gl_weapons_dtab);

    u8 out_brave   = 1;
    u64 temp_effect = 0;
    // TODO: use AP to compute brave factor
    // TODO: Brave for all hands
    if (Unit_isEquipped(unit, UNIT_HAND_LEFT)) {
        int id = Unit_Id_Equipped(unit, UNIT_HAND_LEFT);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        temp_effect = weapon->item.effect.passive;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X))
            out_brave = 2;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X))
            out_brave = 3;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X))
            out_brave = 4;
    }

    if (Unit_isEquipped(unit, UNIT_HAND_RIGHT)) {
        int id = Unit_Id_Equipped(unit, UNIT_HAND_RIGHT);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        temp_effect = weapon->item.effect.passive;
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
    unit->stats.current.hp   = unit->stats.caps.hp;
    unit->stats.current.str  = unit->stats.caps.str;
    unit->stats.current.mag  = unit->stats.caps.mag;
    unit->stats.current.agi  = unit->stats.caps.agi;
    unit->stats.current.dex  = unit->stats.caps.dex;
    unit->stats.current.fth  = unit->stats.caps.fth;
    unit->stats.current.luck = unit->stats.caps.luck;
    unit->stats.current.def  = unit->stats.caps.def;
    unit->stats.current.res  = unit->stats.caps.res;
    unit->stats.current.con  = unit->stats.caps.con;
    unit->stats.current.prof = unit->stats.caps.prof;
}

/* What is this for?
caps before promotion? */
void Unit_HalfCap_Stats(struct Unit *unit) {
    unit->stats.current.hp   = unit->stats.caps.hp   / 2;
    unit->stats.current.str  = unit->stats.caps.str  / 2;
    unit->stats.current.mag  = unit->stats.caps.mag  / 2;
    unit->stats.current.agi  = unit->stats.caps.agi  / 2;
    unit->stats.current.dex  = unit->stats.caps.dex  / 2;
    unit->stats.current.fth  = unit->stats.caps.fth  / 2;
    unit->stats.current.luck = unit->stats.caps.luck / 2;
    unit->stats.current.def  = unit->stats.caps.def  / 2;
    unit->stats.current.res  = unit->stats.caps.res  / 2;
    unit->stats.current.con  = unit->stats.caps.con  / 2;
    unit->stats.current.prof = unit->stats.caps.prof / 2;
}

struct Unit_stats Unit_effectiveGrowths(struct Unit *unit) {
    // TODO:
    //  1. Compute bonuses dynamically from bonus_stack
    //  2. Add them to a new struct Unit_stats
    SDL_assert(unit);
    return (unit->stats.growths);
}

struct Unit_stats Unit_effectiveStats(struct Unit *unit) {
    // TODO: input map to computed bonuses
    // TODO:
    //  - Check all auras for bonuses/maluses
    //  - Check all skills for bonuses/maluses

    /* current_stats + all bonuses */
    SDL_assert(unit);

    // TODO: compute bonuses dynamically
    /* Reset stats to current */
    Unit_stats effective = unit->stats.current;

    /* Add all bonuses */
    if (unit->stats.bonus_stack != NULL) {
        for (int i = 0; i < DARR_NUM(unit->stats.bonus_stack); i++) {
            effective = Unit_stats_plus(effective, unit->stats.bonus_stack[i].unit_stats);
        }
    }

    /* TODO: Add Mount move */
    // if (unit->mount.ptr != NULL)
    //     effective.move = unit->mount.ptr->move;

    return (effective);
}

void Unit_Promote(struct Unit *unit, i8 new_class_index) {
    SDL_assert(unit);
    // struct Unit_stats promotion_bonus = DTAB_GET(promotion_bonus_stats, new_class_index);
    // DARR_PUT(unit->grown_stats, promotion_bonus);
    // Unit_stats_plus(unit->stats.current, promotion_bonus);
    // unit->flags.skills += DTAB_GET(promotion_bonus_skills, new_class_index);
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
    return (unit->hp.current >= Unit_effectiveStats(unit).hp);
}
