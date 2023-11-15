#include "combat.h"

/* --- DEFAULT STRUCTS --- */
struct Combat_Phase Combat_Phase_default = {
    .attacker           = SOTA_AGGRESSOR,
    .attack_num         = 1,
    .skill_multiplier   = SOTA_ASTRA_DEFAULT,
    .skillp_multipliers = {SOTA_ASTRA_DEFAULT},
};

struct Combat_Attack Combat_Attack_default = {
    .hit           = false,
    .crit          = false,
    .attacker      = SOTA_AGGRESSOR,
    .total_damage  = 0,
};

struct Combat_Flow Combat_Flow_default = {
    .aggressor_brave        = SOTA_BRAVE_DEFAULT,
    .defendant_brave        = SOTA_BRAVE_DEFAULT,
    .aggressor_phases       = 1,
    .defendant_phases       = 1,
    .defendant_retaliates   = false,
} ;

struct Combat_Rates Combat_Rates_default = {0, 0};

struct Combat_Death Combat_Death_default = {
    .aggressor_possible  = false,
    .aggressor_certain   = false,
    .defendant_possible  = false,
    .defendant_certain   = false,
};

bool Combat_canDouble(const struct Unit *attacker, const struct Unit *defender) {
    SDL_assert(attacker && defender);
    i8 diff        = (attacker->computed_stats.speed - defender->computed_stats.speed);
    bool doubles    = (diff > SOTA_DOUBLING_SPEED);
    return (doubles);
}

bool Combat_canAttack_Equipped(struct Unit *attacker, struct Unit *defender,
                               const struct Point *att_pos,
                               const struct Point *dfd_pos) {
    SDL_assert(attacker && defender);
    SDL_assert(att_pos  && dfd_pos);
    /* Get range of current loadout */
    struct Range *att_range = Unit_Range_Loadout(attacker);
    /* Is enemy in range? */
    u8 distance = abs(dfd_pos->x - att_pos->x) + abs(dfd_pos->y - att_pos->y);
    bool can     = (distance >= att_range->min) && (distance <= att_range->max);
    return (can);
}

struct Combat_Flow Compute_Combat_Flow(struct Unit *agg, struct Unit *dft,
                                       const struct Point *agg_pos,
                                       const struct Point *dft_pos) {
    SDL_assert(agg      && dft);
    SDL_assert(agg_pos  && dft_pos);
    struct Combat_Flow out_flow;
    out_flow.aggressor_phases = 1;
    out_flow.defendant_phases = 0;
    out_flow.defendant_retaliates = Combat_canAttack_Equipped(dft, agg, dft_pos, agg_pos);
    if (out_flow.defendant_retaliates)
        out_flow.defendant_phases = 1;

    // TODO: Triple, Quadruple phases with skill.
    if (Combat_canDouble(agg, dft))
        out_flow.aggressor_phases *= 2;
    if (Combat_canDouble(dft, agg))
        out_flow.defendant_phases *= 2;

    if (out_flow.defendant_phases > 1)
        SDL_assert(out_flow.aggressor_phases <= 1);
    if (out_flow.aggressor_phases > 1)
        SDL_assert(out_flow.defendant_phases <= 1);

    out_flow.aggressor_brave = Unit_Brave(agg);
    out_flow.defendant_brave = Unit_Brave(dft);
    return (out_flow);
}

struct Damage Compute_Combat_Damage(struct Unit *attacker,
                                    struct Unit *defender) {
    SDL_assert(attacker && defender);
    u8 eff = Unit_computeEffectivefactor(attacker, defender);
    u8 aap = attacker->computed_stats.attack[DMG_TYPE_PHYSICAL];
    u8 aam = attacker->computed_stats.attack[DMG_TYPE_MAGICAL];
    u8 aat = attacker->computed_stats.attack[DMG_TYPE_TRUE];
    u8 dpp = defender->computed_stats.protection[DMG_TYPE_PHYSICAL];
    u8 dpm = defender->computed_stats.protection[DMG_TYPE_MAGICAL];

    // TODO: Sum appropriate damage types according to equipment.
    // Add type damage ONLY if one piece of equipment has that damage type

    /* - HIT DAMAGE - */
    attacker->damage.dmg[DMG_TYPE_PHYSICAL] = Equation_Combat_Damage(aap, dpp, eff, CRIT_FACTOR, 0);
    attacker->damage.dmg[DMG_TYPE_MAGICAL]  = Equation_Combat_Damage(aam, dpm, eff, CRIT_FACTOR, 0);
    attacker->damage.dmg[DMG_TYPE_TRUE]     = aat;

    /* - CRIT DAMAGE - */
    attacker->damage.dmg_crit[DMG_TYPE_PHYSICAL] = Equation_Combat_Damage(aap, dpp, eff, CRIT_FACTOR,
                                                   1);
    attacker->damage.dmg_crit[DMG_TYPE_MAGICAL]  = Equation_Combat_Damage(aam, dpm, eff, CRIT_FACTOR,
                                                   1);
    attacker->damage.dmg_crit[DMG_TYPE_TRUE]     = Equation_Combat_Damage(aat, 0, eff, CRIT_FACTOR, 1);
    Equation_Damage_Total(&attacker->damage);
    return (attacker->damage);
}

void Combat_Death_isPossible(struct Combat_Flow flow, u8 *out) {
    out[SOTA_DEFENDANT] = 0;
    // TODO: compute total damage
}

struct Combat_Death Compute_Combat_Death(struct Unit *aggressor, struct Unit *defendant,
                                         struct Combat_Stats forecast, struct Combat_Flow flow) {
    SDL_assert(aggressor && defendant);

    /* DOES NOT WORK */
    struct Combat_Death out_death = Combat_Death_default;
    u8 defendant_possible[2];
    Combat_Death_isPossible(flow, defendant_possible);
    u8 attacker_maxDamage_nocrit = 0, attacker_maxDamage_crit = 0;
    u8 defender_maxDamage_nocrit = 0, defender_maxDamage_crit = 0;

    /* -- Aggressor -- */
    do { /*Loop never executes, just used for break*/
        if (forecast.agg_rates.hit == 0)
            break;
        // TODO: REMAKE
        // u8 agg_dmg = forecast.agg_damage.dmg[DMG_TYPE_PHYSICAL] +
        //               forecast.agg_damage.dmg[DMG_TYPE_MAGICAL];
        // attacker_maxDamage_nocrit = Equation_multiplyDamage(agg_dmg, defendant_possible[SOTA_AGGRESSOR]);
        // attacker_maxDamage_crit   = Equation_multiplyDamage(agg_dmg, defendant_possible[SOTA_AGGRESSOR]);
        // if ((defendant->current_hp < attacker_maxDamage_nocrit)
        //     && (forecast.agg_rates.hit == SOTA_100PERCENT))
        //     out_death.defendant_certain = true;

        // <if ((defendant->current_hp < attacker_maxDamage_nocrit) && (forecast.agg_rates.hit < SOTA_100PERCENT))
        //     out_death.defendant_possible = true;
        // if ((forecast.agg_rates.crit > 0) && (defendant->current_hp < attacker_maxDamage_crit))
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
        // u8 def_dmg = forecast.dft_damage.dmg[DMG_TYPE_PHYSICAL] +
        //               forecast.dft_damage.dmg[DMG_TYPE_MAGICAL];
        // defender_maxDamage_nocrit = Equation_multiplyDamage(def_dmg, defendant_possible[SOTA_DEFENDANT]);
        // defender_maxDamage_crit = Equation_multiplyDamage(def_dmg, defendant_possible[SOTA_DEFENDANT]);
        // if ((aggressor->current_hp < defender_maxDamage_nocrit)
        //     && (forecast.dft_rates.hit == SOTA_100PERCENT))
        //     out_death.aggressor_certain = true;
        // if ((aggressor->current_hp < defender_maxDamage_nocrit)
        //     && (forecast.dft_rates.hit < SOTA_100PERCENT))
        //     out_death.aggressor_possible = true;
        // if ((forecast.dft_rates.crit > 0)
        //     && (aggressor->current_hp < defender_maxDamage_crit))
        //     out_death.aggressor_possible = true;
    } while (0);

    return (out_death);
}

struct Combat_Rates Compute_Combat_Rates(struct Unit *attacker,
                                         struct Unit *defender,
                                         const struct Point *att_pos,
                                         const struct Point *dfd_pos) {
    SDL_assert(attacker && defender);
    u8 distance = abs(dfd_pos->x - att_pos->x) + abs(dfd_pos->y - att_pos->y);
    struct Combat_Rates out_rates = Combat_Rates_default;
    struct Computed_Stats CS_A    = Unit_computedStats(attacker, distance);
    struct Computed_Stats CS_D    = Unit_computedStats(defender, distance);
    out_rates.hit                 = Equation_Combat_Hit(CS_A.hit,   CS_D.dodge);
    out_rates.crit                = Equation_Combat_Crit(CS_A.crit, CS_D.favor);
    return (out_rates);
}

// *INDENT-OFF*
struct Combat_Forecast Compute_Combat_Forecast(struct Unit * agg, 
                                               struct Unit * dft,
                                               const struct Point * agg_pos,
                                               const struct Point * dft_pos) {
// *INDENT-ON*
    SDL_assert(agg        &&dft);
    SDL_assert(agg_pos    &&dft_pos);
    struct Combat_Forecast out = {0};
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);
    Unit_effectiveStats(agg);
    Unit_effectiveStats(dft);
    out.stats.agg_stats         = Unit_computedStats(agg, distance);
    out.stats.dft_stats         = Unit_computedStats(dft, distance);
    out.flow = Compute_Combat_Flow(agg, dft, agg_pos, dft_pos);
    out.stats.agg_rates         = Compute_Combat_Rates(agg,  dft, agg_pos, dft_pos);
    out.stats.agg_damage        = Compute_Combat_Damage(agg, dft);
    if (out.flow.defendant_retaliates) {
        out.stats.dft_rates     = Compute_Combat_Rates(dft,  agg, dft_pos, agg_pos);
        out.stats.dft_damage    = Compute_Combat_Damage(dft, agg);
    }
    return (out);
}

void Combat_totalDamage(struct Combat_Attack *attack, struct Damage *damage) {
    /* - crit hit should be computed before - */
    attack->total_damage = 0;
    if (attack->hit && !attack->crit)
        attack->total_damage = damage->dmg[DMG_TYPE_TOTAL];
    else if (attack->hit && attack->crit)
        attack->total_damage = damage->dmg_crit[DMG_TYPE_TOTAL];
}

void Compute_Combat_Outcome(struct Combat_Phase     *phases,
                            struct Combat_Attack    *darr_attacks,
                            struct Combat_Forecast *forecast,
                            struct Unit         *aggressor,
                            struct Unit         *defendant) {
    // TODO: tripling with SPEED DEMON skill

    /* -- Resetting attacks list -- */
    forecast->phase_num     = 0;
    DARR_NUM(darr_attacks)  = 0;

    /* -- Aggressor, Phase 1 -- */
    struct Unit *attacker           = aggressor;
    struct Combat_Phase temp_phase  = Combat_Phase_default;
    struct Damage damage            = forecast->stats.agg_damage;
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
    forecast->attack_num = DARR_NUM(darr_attacks);
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

int Combat_TotalAttack_Num(struct Combat_Phase *phases, int brave_factor, int phase_num) {
    int total_attacks = 0;
    for (int i = 0; i < phase_num; ++i)
        total_attacks += Combat_Phase_Attack_Num(&phases[i], brave_factor);

    return (total_attacks);
}

void Compute_Combat_Phase(struct Combat_Phase *phase,
                          struct Combat_Attack   *darr_attacks, struct Damage        damage,
                          struct Unit *attacker, u8 hit_rate, u8 crit_rate, u8 brave_factor) {
    phase->attack_num = Combat_Phase_Attack_Num(phase, brave_factor);
    for (int i = 0; i < phase->attack_num; i++)
        Compute_Combat_Attack(phase, darr_attacks, damage, attacker, hit_rate, crit_rate);

}

void Compute_Combat_Attack(struct Combat_Phase   *phase,
                           struct Combat_Attack *darr_attacks, struct Damage damage,
                           struct Unit *attacker, u8 hit_rate, u8 crit_rate) {
    struct Combat_Attack temp_attack;

    attacker->hit_sequence.eff_rate = hit_rate;
    /* --- CHECKING HIT --- */
    /* -- computing effective rate -- */
    #ifdef RNG_SEQUENCE_BREAKER_HIT
    if (!SotA_isPC(attacker->army)) { /* rate drops if hit sequence for NPC ONLY */
        if ((attacker->hit_sequence.hit) && (attacker->hit_sequence.len > 0)) {
            attacker->hit_sequence.eff_rate = SB_RATE_DROP(hit_rate, attacker->hit_sequence.len);
        }
    }
    #endif /* RNG_SEQUENCE_BREAKER_HIT */
    #ifdef RNG_SEQUENCE_BREAKER_MISS
    if (SotA_isPC(attacker->army)) { /* rate rises if miss sequence for PC ONLY */
        if ((!attacker->hit_sequence.hit) && (attacker->hit_sequence.len > 0)) {
            attacker->hit_sequence.eff_rate = SB_RATE_RISE(hit_rate, attacker->hit_sequence.len);
        }
    }
    #endif /* RNG_SEQUENCE_BREAKER_MISS */

    /* -- checking hit or miss -- */
    temp_attack.hit = RNG_checkRate(attacker->hit_sequence.eff_rate, SOTA_RNG);
    /* -- checking sequence -- */
    #if defined(RNG_SEQUENCE_BREAKER_HIT) || defined(RNG_SEQUENCE_BREAKER_MISS)
    RNG_checkSequence_twoWay(&attacker->hit_sequence, temp_attack.hit);
    #endif /* RNG_SEQUENCE_BREAKER_HIT */

    /* --- CHECKING CRIT --- */
    /* -- computing effective rate -- */
    attacker->crit_sequence.eff_rate = crit_rate;
    #ifdef RNG_SEQUENCE_BREAKER_CRIT
    if (!SotA_isPC(attacker->army)) { /* rate drops if crit sequence for PC ONLY */
        if ((attacker->crit_sequence.hit) && (attacker->crit_sequence.len > 0))
            attacker->crit_sequence.eff_rate = SB_RATE_DROP(crit_rate, attacker->crit_sequence.len);
    }
    #endif /* RNG_SEQUENCE_BREAKER_HIT */
    #ifdef RNG_SEQUENCE_BREAKER_NOCRIT
    if (SotA_isPC(attacker->army)) { /* rate rises if nocrit sequence for PC ONLY */
        if ((!attacker->crit_sequence.hit) && (attacker->crit_sequence.len > 0))
            attacker->crit_sequence.eff_rate = SB_RATE_RISE(crit_rate, attacker->crit_sequence.len);
    }
    #endif /* RNG_SEQUENCE_BREAKER_MISS */
    /* -- checking crit or nocrit -- */
    temp_attack.crit = RNG_checkRate(crit_rate, SOTA_RNG);
    /* -- checking sequence -- */
    #if defined(RNG_SEQUENCE_BREAKER_CRIT) || defined(RNG_SEQUENCE_BREAKER_NOCRIT)
    RNG_checkSequence_twoWay(&attacker->crit_sequence, temp_attack.crit);
    #endif /* RNG_SEQUENCE_BREAKER_CRIT */

    /* --- COMPUTING TOTAL ATTACK DAMAGE --- */
    Combat_totalDamage(&temp_attack, &damage);

    temp_attack.attacker = phase->attacker;
    DARR_PUT(darr_attacks, temp_attack); // growing here breaks, address changes
}

u8 Combat_Next_HP(struct Combat_Attack attack, u8 hp) {
    u8 next_hp = hp < attack.total_damage ? hp : hp - attack.total_damage;
    return (next_hp);
}

void Combat_Resolve(struct Combat_Attack *combat_attacks, u8 attack_num,
                    struct Unit *aggressor, struct Unit *defendant) {
    SDL_assert(attack_num > 0);
    SDL_assert(attack_num <= SOTA_COMBAT_MAX_PHASES);
    struct Unit *attacker, *defender;

    for (u8 i = 0; i < attack_num; i++) {
        attacker = combat_attacks[i].attacker ? aggressor : defendant;
        defender = combat_attacks[i].attacker ? defendant : aggressor;

        if (Unit_canAttack(attacker))
            Resolve_Attack(combat_attacks[i], attacker, defender);

        b32 agg_death = (!aggressor->alive) || (aggressor->agony >= 0);
        b32 dft_death = (!defendant->alive) || (defendant->agony >= 0);

        if (agg_death || dft_death)
            break;
    }
}

void Resolve_Attack(struct Combat_Attack attack, struct Unit *attacker,
                    struct Unit *defender) {
    /* - Skip if attack doesn't hit - */
    if (!attack.hit)
        /* - Deplete ranged weapons here? - */
        return;

    /* - Pop divine shield - */
    if (defender->divine_shield) {
        defender->divine_shield = false;
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
