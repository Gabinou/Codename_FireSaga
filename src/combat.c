
#include "combat.h"
#include "jsonio.h"
#include "RNG.h"
#include "debug.h"
#include "structs.h"
#include "equations.h"
#include "nmath.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/equipment.h"
#include "unit/loadout.h"

/* --- DEFAULT STRUCTS --- */
const struct Combat_Phase Combat_Phase_default = {
    .attacker           = SOTA_AGGRESSOR,
    .attack_num         = 1,
    .skill_multiplier   = SOTA_ASTRA_DEFAULT,
    .skillp_multipliers = {SOTA_ASTRA_DEFAULT},
};

const struct Combat_Attack Combat_Attack_default = {
    .attacker      = SOTA_AGGRESSOR,
};

const struct Combat_Flow Combat_Flow_default = {
    .aggressor_brave        = SOTA_BRAVE_DEFAULT,
    .defendant_brave        = SOTA_BRAVE_DEFAULT,
    .aggressor_phases       = 1,
    .defendant_phases       = 1,
};

const struct Combat_Rates Combat_Rates_default = {0};
const struct Combat_Death Combat_Death_default = {0};

b32 Combat_canDouble(Computed_Stats cs_att, Computed_Stats cs_dfd) {
    i32 diff      = (cs_att.speed - cs_dfd.speed);
    return (diff > SOTA_DOUBLING_SPEED);
}

b32 Combat_canAttack_Equipped(struct Unit *attacker,
                              struct Point *att_pos, struct Point *dfd_pos) {
    SDL_assert(attacker);
    SDL_assert(att_pos);
    SDL_assert(dfd_pos);
    /* Get range of current loadout */
    struct Range att_range = Range_default;
    Unit_Range_Equipped(attacker, ITEM_ARCHETYPE_WEAPON, &att_range);
    /* Is enemy in range? */
    u8 distance  = abs(dfd_pos->x - att_pos->x) +  abs(dfd_pos->y - att_pos->y);
    b32 can     = (distance >= att_range.min) && (distance <= att_range.max);
    return (can);
}

struct Combat_Flow Compute_Combat_Flow(struct Unit *agg, struct Unit *dft,
                                       Computed_Stats cs_agg, Computed_Stats cs_dfd,
                                       struct Point *agg_pos, struct Point *dft_pos) {
    SDL_assert(dft);
    SDL_assert(agg);
    SDL_assert(agg_pos);
    SDL_assert(dft_pos);
    struct Combat_Flow out_flow;
    out_flow.aggressor_phases = 1;
    out_flow.defendant_phases = 0;

    out_flow.defendant_retaliates = Combat_canAttack_Equipped(dft, dft_pos, agg_pos);
    if (out_flow.defendant_retaliates)
        out_flow.defendant_phases = 1;

    // Set number of combat phases for adversary, if doubling
    // TODO: Triple, Quadruple phases with skill.
    if (Combat_canDouble(cs_agg, cs_dfd)) {
        out_flow.aggressor_phases *= 2;
        SDL_assert(!Combat_canDouble(cs_dfd, cs_agg));
    }
    if (Combat_canDouble(cs_dfd, cs_agg)) {
        out_flow.defendant_phases *= 2;
        SDL_assert(!Combat_canDouble(cs_agg, cs_dfd));
    }

    // Check that adversary does not have extra combat phases
    if (out_flow.defendant_phases > 1)
        SDL_assert(out_flow.aggressor_phases <= 1);
    if (out_flow.aggressor_phases > 1)
        SDL_assert(out_flow.defendant_phases <= 1);

    out_flow.aggressor_brave = Unit_Brave(agg);
    out_flow.defendant_brave = Unit_Brave(dft);
    return (out_flow);
}

struct Combat_Damage Compute_Combat_Damage(Unit *att, Unit *dfd,
                                           Computed_Stats cs_att,
                                           Computed_Stats cs_dfd) {
    SDL_assert(att && dfd);
    i32 eff;
    Unit_computeEffectivefactor(att, dfd, &eff);
    i32 aap = cs_att.attack.physical;
    i32 aam = cs_att.attack.magical;
    i32 aat = cs_att.attack.True;
    i32 dpp = cs_dfd.protection.physical;
    i32 dpm = cs_dfd.protection.magical;

    // TODO: Sum appropriate damage types according to equipment.
    // Add type damage ONLY if one piece of equipment has that damage type
    struct Combat_Damage damage = {0};

    /* - HIT DAMAGE - */
    damage.dmg.physical = Equation_Combat_Damage(aap, dpp, eff, CRIT_FACTOR, 0);
    damage.dmg.magical  = Equation_Combat_Damage(aam, dpm, eff, CRIT_FACTOR, 0);
    damage.dmg.True     = aat;

    /* - CRIT DAMAGE - */
    damage.dmg_crit.physical = Equation_Combat_Damage(aap, dpp, eff, CRIT_FACTOR, 1);
    damage.dmg_crit.magical  = Equation_Combat_Damage(aam, dpm, eff, CRIT_FACTOR, 1);
    damage.dmg_crit.True     = Equation_Combat_Damage(aat, 0, eff, CRIT_FACTOR, 1);
    Equation_Damage_Total(&damage);
    return (damage);
}

/* Possible Combat death: For AI*/
struct Combat_Death Compute_Combat_Death(struct Unit *aggressor, struct Unit *defendant,
                                         struct Combat_Stats forecast, struct Combat_Flow flow) {
    SDL_assert(aggressor && defendant);

    /* DOES NOT WORK */
    struct Combat_Death out_death = Combat_Death_default;
    // u8 attacker_maxDamage_nocrit = 0, attacker_maxDamage_crit = 0;
    // u8 defender_maxDamage_nocrit = 0, defender_maxDamage_crit = 0;

    /* -- Aggressor -- */
    do { /*Loop never executes, just used for break*/
        if (forecast.agg_rates.hit == 0)
            break;
        // TODO: REMAKE
        // u8 agg_dmg = forecast.agg_damage.dmg.physical +
        //               forecast.agg_damage.dmg.magical;
        // attacker_maxDamage_nocrit = Equation_multiplyDamage(agg_dmg, defendant_possible[SOTA_AGGRESSOR]);
        // attacker_maxDamage_crit   = Equation_multiplyDamage(agg_dmg, defendant_possible[SOTA_AGGRESSOR]);
        // if ((defendant->counters.hp < attacker_maxDamage_nocrit)
        //     && (forecast.agg_rates.hit == SOTA_100PERCENT))
        //     out_death.defendant_certain = true;

        // <if ((defendant->counters.hp < attacker_maxDamage_nocrit) && (forecast.agg_rates.hit < SOTA_100PERCENT))
        //     out_death.defendant_possible = true;
        // if ((forecast.agg_rates.crit > 0) && (defendant->counters.hp < attacker_maxDamage_crit))
        //     out_death.defendant_possible = true;

        // if ((out_death.aggressor_certain) && (out_death.defendant_certain)) {
        //     out_death.aggressor_certain = false;
        //     out_death.defendant_certain = false;
        //     out_death.aggressor_possible = true;
        //     out_death.defendant_possible = true;
        // }
    } while (0);

    /* -- Defendant -- */
    do { /*Loop never executes, just used for break*/
        if (!flow.defendant_retaliates)
            continue;

        if (forecast.dft_rates.hit == 0)
            continue;
        // TODO: REMAKE
        // u8 def_dmg = forecast.dft_damage.dmg.physical +
        //               forecast.dft_damage.dmg.magical;
        // defender_maxDamage_nocrit = Equation_multiplyDamage(def_dmg, defendant_possible[SOTA_DEFENDANT]);
        // defender_maxDamage_crit = Equation_multiplyDamage(def_dmg, defendant_possible[SOTA_DEFENDANT]);
        // if ((aggressor->counters.hp < defender_maxDamage_nocrit)
        //     && (forecast.dft_rates.hit == SOTA_100PERCENT))
        //     out_death.aggressor_certain = true;
        // if ((aggressor->counters.hp < defender_maxDamage_nocrit)
        //     && (forecast.dft_rates.hit < SOTA_100PERCENT))
        //     out_death.aggressor_possible = true;
        // if ((forecast.dft_rates.crit > 0)
        //     && (aggressor->counters.hp < defender_maxDamage_crit))
        //     out_death.aggressor_possible = true;
    } while (0);

    return (out_death);
}

struct Combat_Rates Compute_Combat_Rates(Computed_Stats cs_att, Computed_Stats cs_dfd) {
    struct Combat_Rates out_rates   = Combat_Rates_default;
    out_rates.hit                   = Equation_Combat_Hit(  cs_att.hit,  cs_dfd.dodge);
    out_rates.crit                  = Equation_Combat_Crit( cs_att.crit, cs_dfd.favor);
    return (out_rates);
}

struct Combat_Forecast Compute_Combat_Forecast(struct Unit  *agg,
                                               struct Unit  *dft,
                                               struct Point *agg_pos,
                                               struct Point *dft_pos) {
    SDL_assert(agg);
    SDL_assert(dft);
    SDL_assert(agg_pos);
    SDL_assert(dft_pos);
    struct Combat_Forecast out = {0};
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);
    struct Unit_stats eff_agg       = Unit_effectiveStats(agg);
    struct Unit_stats eff_dft       = Unit_effectiveStats(dft);
    struct Computed_Stats cs_agg    = Unit_computedStats(agg, distance, eff_agg);
    struct Computed_Stats cs_dft    = Unit_computedStats(dft, distance, eff_dft);
    out.stats.agg_stats             = cs_agg;
    out.stats.dft_stats             = cs_dft;

    out.flow = Compute_Combat_Flow(agg, dft, cs_agg, cs_dft, agg_pos, dft_pos);
    out.stats.agg_rates         = Compute_Combat_Rates(cs_agg, cs_dft);
    out.stats.agg_damage        = Compute_Combat_Damage(agg, dft, cs_agg, cs_dft);
    if (out.flow.defendant_retaliates) {
        out.stats.dft_rates     = Compute_Combat_Rates(cs_dft, cs_agg);
        out.stats.dft_damage    = Compute_Combat_Damage(dft, agg, cs_dft, cs_agg);
    }
    return (out);
}

void Combat_totalDamage(struct Combat_Attack *attack, struct Combat_Damage *damage) {
    /* - crit hit should be computed before - */
    attack->total_damage = 0;
    if (attack->hit && !attack->crit)
        attack->total_damage = damage->dmg.total;
    else if (attack->hit && attack->crit)
        attack->total_damage = damage->dmg_crit.total;
}

void Compute_Combat_Outcome(struct Combat_Outcome   *outcome,
                            struct Combat_Forecast  *forecast,
                            struct Unit             *aggressor,
                            struct Unit             *defendant) {
    // TODO: tripling with SPEED DEMON skill
    struct Combat_Phase  *phases        = outcome->phases;
    struct Combat_Attack *darr_attacks  = outcome->attacks;

    /* -- Resetting attacks list -- */
    forecast->phase_num     = 0;
    DARR_NUM(darr_attacks)  = 0;

    /* -- Aggressor, Phase 1 -- */
    struct Unit *attacker           = aggressor;
    struct Combat_Phase temp_phase  = Combat_Phase_default;
    struct Combat_Damage damage            = forecast->stats.agg_damage;
    u8 hit     = forecast->stats.agg_rates.hit;
    u8 crit    = forecast->stats.agg_rates.crit;
    u8 brave   = forecast->flow.aggressor_brave;
    temp_phase.attacker = SOTA_AGGRESSOR;

    Compute_Combat_Phase(&temp_phase, darr_attacks, damage, attacker, hit, crit, brave);
    phases[forecast->phase_num++] = temp_phase;

    /* -- Defendant, Retaliation 1 -- */
    if (forecast->flow.defendant_retaliates) {
        hit         = forecast->stats.dft_rates.hit;
        crit        = forecast->stats.dft_rates.crit;
        brave       = forecast->flow.defendant_brave;
        damage      = forecast->stats.dft_damage;
        attacker    = defendant;
        temp_phase.attacker = SOTA_DEFENDANT;

        Compute_Combat_Phase(&temp_phase, darr_attacks, damage, defendant, hit, crit, brave);
        phases[forecast->phase_num++] = temp_phase;
    }
    /* -- Aggressor, Phase 2 -- */
    if (forecast->flow.aggressor_phases > 1) {
        hit         = forecast->stats.agg_rates.hit;
        crit        = forecast->stats.agg_rates.crit;
        brave       = forecast->flow.aggressor_brave;
        damage      = forecast->stats.agg_damage;
        attacker    = aggressor;
        temp_phase.attacker = SOTA_AGGRESSOR;

        Compute_Combat_Phase(&temp_phase, darr_attacks, damage, attacker, hit, crit, brave);
        phases[forecast->phase_num++] = temp_phase;

    }
    /* -- Defendant, Retaliation 2 -- */
    if ((forecast->flow.defendant_retaliates) && (forecast->flow.defendant_phases > 1)) {
        hit         = forecast->stats.dft_rates.hit;
        crit        = forecast->stats.dft_rates.crit;
        brave       = forecast->flow.defendant_brave;
        damage      = forecast->stats.dft_damage;
        attacker    = defendant;
        temp_phase.attacker = SOTA_DEFENDANT;

        Compute_Combat_Phase(&temp_phase, darr_attacks, damage, defendant, hit, crit, brave);
        phases[forecast->phase_num++] = temp_phase;
    }
    forecast->attack_num        = DARR_NUM(darr_attacks);
    outcome->ended              = false;
    outcome->current_attack     = 0;
}

/* -- Combat Attacks -- */
int Combat_Phase_Attack_Num(struct Combat_Phase *phase, int brave_factor) {
    SDL_assert(brave_factor >= 1);
    int attacks = 0;
    for (int i = 0; i < brave_factor; ++i) {
        SDL_assert(phase->skillp_multipliers[i] >= 1);
        // NOTE: += cause the multiplier is ADDED for EVERY BRAVE.
        //          -> that is the DEFINITION of a multiplication
        attacks += phase->skillp_multipliers[i] * phase->skill_multiplier;
    }
    return (attacks);
}

int Combat_Attack_Total_Num(struct Combat_Phase *phases, int brave_factor, int phase_num) {
    int total_attacks = 0;
    for (int i = 0; i < phase_num; ++i)
        total_attacks += Combat_Phase_Attack_Num(&phases[i], brave_factor);

    return (total_attacks);
}

void Compute_Combat_Phase(struct Combat_Phase   *phase,
                          struct Combat_Attack  *darr_attacks,
                          struct Combat_Damage          damage,
                          struct Unit           *attacker,
                          u8 hit_rate, u8 crit_rate, u8 brave_factor) {
    phase->attack_num = Combat_Phase_Attack_Num(phase, brave_factor);
    for (int i = 0; i < phase->attack_num; i++)
        Compute_Combat_Attack(phase, darr_attacks, damage, attacker, hit_rate, crit_rate);
}

void Compute_Combat_Attack(struct Combat_Phase  *phase,
                           struct Combat_Attack *darr_attacks,
                           struct Combat_Damage         damage,
                           struct Unit          *attacker,
                           u8 hit_rate, u8 crit_rate) {
    struct Combat_Attack temp_attack;

    attacker->rng_sequence.hit.eff_rate = hit_rate;
    /* --- CHECKING HIT --- */
    /* -- computing effective rate -- */
#ifdef RNG_SEQUENCE_BREAKER_HIT
    if (!SotA_isPC(Unit_Army(attacker))) { /* rate drops if hit sequence for NPC ONLY */
        if ((attacker->rng_sequence.hit.hit) && (attacker->rng_sequence.hit.len > 0)) {
            attacker->rng_sequence.hit.eff_rate = SB_RATE_DROP(hit_rate, attacker->rng_sequence.hit.len);
        }
    }
#endif /* RNG_SEQUENCE_BREAKER_HIT */
#ifdef RNG_SEQUENCE_BREAKER_MISS
    if (SotA_isPC(Unit_Army(attacker))) { /* rate rises if miss sequence for PC ONLY */
        if ((!attacker->rng_sequence.hit.hit) && (attacker->rng_sequence.hit.len > 0)) {
            attacker->rng_sequence.hit.eff_rate = SB_RATE_RISE(hit_rate, attacker->rng_sequence.hit.len);
        }
    }
#endif /* RNG_SEQUENCE_BREAKER_MISS */

    /* -- checking hit or miss -- */
    temp_attack.hit = RNG_checkRate(attacker->rng_sequence.hit.eff_rate, SOTA_RN_DOUBLE);
    /* -- checking sequence -- */
#if defined(RNG_SEQUENCE_BREAKER_HIT) || defined(RNG_SEQUENCE_BREAKER_MISS)
    RNG_checkSequence_twoWay(&attacker->rng_sequence.hit, temp_attack.hit);
#endif /* RNG_SEQUENCE_BREAKER_HIT */

    /* --- CHECKING CRIT --- */
    /* -- computing effective rate -- */
    attacker->rng_sequence.crit.eff_rate = crit_rate;
#ifdef RNG_SEQUENCE_BREAKER_CRIT
    if (!SotA_isPC(Unit_Army(attacker))) { /* rate drops if crit sequence for PC ONLY */
        if ((attacker->rng_sequence.crit.hit) && (attacker->rng_sequence.crit.len > 0))
            attacker->rng_sequence.crit.eff_rate = SB_RATE_DROP(crit_rate, attacker->rng_sequence.crit.len);
    }
#endif /* RNG_SEQUENCE_BREAKER_HIT */
#ifdef RNG_SEQUENCE_BREAKER_NOCRIT
    if (SotA_isPC(Unit_Army(attacker))) { /* rate rises if nocrit sequence for PC ONLY */
        if ((!attacker->rng_sequence.crit.hit) && (attacker->rng_sequence.crit.len > 0))
            attacker->rng_sequence.crit.eff_rate = SB_RATE_RISE(crit_rate, attacker->rng_sequence.crit.len);
    }
#endif /* RNG_SEQUENCE_BREAKER_MISS */
    /* -- checking crit or nocrit -- */
    temp_attack.crit = RNG_checkRate(crit_rate, SOTA_RNG);
    /* -- checking sequence -- */
#if defined(RNG_SEQUENCE_BREAKER_CRIT) || defined(RNG_SEQUENCE_BREAKER_NOCRIT)
    RNG_checkSequence_twoWay(&attacker->rng_sequence.crit, temp_attack.crit);
#endif /* RNG_SEQUENCE_BREAKER_CRIT */

    /* --- COMPUTING TOTAL ATTACK DAMAGE --- */
    Combat_totalDamage(&temp_attack, &damage);

    temp_attack.attacker = phase->attacker;
    DARR_PUT(darr_attacks, temp_attack); // growing here breaks, address changes
}

void Combat_Resolve(struct Combat_Attack *combat_attacks, u8 attack_num,
                    struct Unit *aggressor, struct Unit *defendant) {
    SDL_assert(attack_num > 0);
    SDL_assert(attack_num <= SOTA_COMBAT_MAX_PHASES);
    struct Unit *attacker, *defender;

    for (int i = 0; i < attack_num; i++) {
        attacker = combat_attacks[i].attacker ? aggressor : defendant;
        defender = combat_attacks[i].attacker ? defendant : aggressor;

        if (Unit_canAttack(attacker))
            Combat_Resolve_Attack(combat_attacks[i], attacker, defender);

        b32 agg_death = (!Unit_isAlive(aggressor)) || (Unit_Current_Agony(aggressor) > AGONY_NULL);
        b32 dft_death = (!Unit_isAlive(defendant)) || (Unit_Current_Agony(defendant) > AGONY_NULL);

        if (agg_death || dft_death)
            break;
    }
}

void Combat_Resolve_Attack(struct Combat_Attack attack, struct Unit *attacker,
                           struct Unit *defender) {
    /* - Skip if attack doesn't hit - */
    SDL_assert(defender != NULL);
    if (!attack.hit)
        /* - Deplete ranged weapons here? - */
        return;

    /* - Pop divine shield - */
    if (Unit_isDivineShield(defender)) {
        Unit_DivineShield_set(defender, false);
        return;
    }

    /* - Unit takes damage - */
    Unit_takesDamage(defender, attack.total_damage, attack.crit);

    /* - Deplete defender shields - */
    Unit_Equipped_Shields_Deplete(defender);
    Unit_Check_Equipped(defender);

    /* - Deplete attacker weapons - */
    Unit_Equipped_Weapons_Deplete(attacker);
    Unit_Check_Equipped(attacker);
}
