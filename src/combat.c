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

bool Combat_canDouble(const struct Unit *restrict attacker, const struct Unit *restrict defender) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attacker && defender);
    if8 diff        = (attacker->computed_stats.speed - defender->computed_stats.speed);
    bool doubles    = (diff > SOTA_DOUBLING_SPEED);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (doubles);
}

bool Combat_canAttack_Equipped(struct Unit *restrict attacker, struct Unit *restrict defender,
                               const struct Point *restrict att_pos,
                               const struct Point *restrict dfd_pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attacker && defender);
    SDL_assert(att_pos  && dfd_pos);
    /* Get range of current loadout */
    struct Range *att_range = Unit_Range_Loadout(attacker);
    /* Is enemy in range? */
    uf8 distance = abs(dfd_pos->x - att_pos->x) + abs(dfd_pos->y - att_pos->y);
    bool can     = (distance >= att_range->min) && (distance <= att_range->max);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (can);
}

struct Combat_Flow Compute_Combat_Flow(struct Unit *restrict agg, struct Unit *restrict dft,
                                       const struct Point *restrict agg_pos,
                                       const struct Point *restrict dft_pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_flow);
}

struct Damage Compute_Combat_Damage(struct Unit *restrict attacker,
                                    struct Unit *restrict defender) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attacker && defender);
    uf8 eff = Unit_computeEffectivefactor(attacker, defender);
    uf8 aap = attacker->computed_stats.attack[DMG_TYPE_PHYSICAL];
    uf8 aam = attacker->computed_stats.attack[DMG_TYPE_MAGICAL];
    uf8 aat = attacker->computed_stats.attack[DMG_TYPE_TRUE];
    uf8 dpp = defender->computed_stats.protection[DMG_TYPE_PHYSICAL];
    uf8 dpm = defender->computed_stats.protection[DMG_TYPE_MAGICAL];

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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (attacker->damage);
}

void Combat_Death_isPossible(struct Combat_Flow flow, uf8 *restrict out) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    out[SOTA_DEFENDANT] = 0;
    // TODO: compute total damage
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Combat_Death Compute_Combat_Death(struct Unit *aggressor, struct Unit *defendant,
                                         struct Combat_Stats forecast, struct Combat_Flow flow) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(aggressor && defendant);

    /* DOES NOT WORK */
    struct Combat_Death out_death = Combat_Death_default;
    uf8 defendant_possible[2];
    Combat_Death_isPossible(flow, defendant_possible);
    uf8 attacker_maxDamage_nocrit = 0, attacker_maxDamage_crit = 0;
    uf8 defender_maxDamage_nocrit = 0, defender_maxDamage_crit = 0;

    /* -- Aggressor -- */
    do { /*Loop never executes, just used for break*/
        if (forecast.agg_rates.hit == 0)
            break;
        // TODO: REMAKE
        // uf8 agg_dmg = forecast.agg_damage.dmg[DMG_TYPE_PHYSICAL] +
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
        // uf8 def_dmg = forecast.dft_damage.dmg[DMG_TYPE_PHYSICAL] +
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

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_death);
}

struct Combat_Rates Compute_Combat_Rates(struct Unit *restrict attacker,
                                         struct Unit *restrict defender,
                                         const struct Point *restrict att_pos,
                                         const struct Point *restrict dfd_pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attacker && defender);
    uf8 distance = abs(dfd_pos->x - att_pos->x) + abs(dfd_pos->y - att_pos->y);
    struct Combat_Rates out_rates = Combat_Rates_default;
    struct Computed_Stats CS_A    = Unit_computedStats(attacker, distance);
    struct Computed_Stats CS_D    = Unit_computedStats(defender, distance);
    out_rates.hit                 = Equation_Combat_Hit(CS_A.hit,   CS_D.dodge);
    out_rates.crit                = Equation_Combat_Crit(CS_A.crit, CS_D.favor);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_rates);
}

// *INDENT-OFF*
struct Combat_Forecast Compute_Combat_Forecast(struct Unit *restrict agg, 
                                               struct Unit *restrict dft,
                                               const struct Point *restrict agg_pos,
                                               const struct Point *restrict dft_pos) {
// *INDENT-ON*
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(agg        &&dft);
    SDL_assert(agg_pos    &&dft_pos);
    struct Combat_Forecast out = {0};
    uf8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void Combat_totalDamage(struct Combat_Attack *restrict attack, struct Damage *restrict damage) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - crit hit should be computed before - */
    attack->total_damage = 0;
    if (attack->hit && !attack->crit)
        attack->total_damage = damage->dmg[DMG_TYPE_TOTAL];
    else if (attack->hit && attack->crit)
        attack->total_damage = damage->dmg_crit[DMG_TYPE_TOTAL];
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Compute_Combat_Outcome(struct Combat_Phase    *restrict phases,
                            struct Combat_Attack   *restrict darr_attacks,
                            struct Combat_Forecast *restrict forecast,
                            struct TINYMT32_T      *restrict tinymt32,
                            struct Unit         *aggressor,
                            struct Unit         *defendant) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // TODO: tripling with SPEED DEMON skill

    /* -- Resetting attacks list -- */
    forecast->phase_num     = 0;
    DARR_NUM(darr_attacks)  = 0;

    /* -- Aggressor, Phase 1 -- */
    struct Unit *attacker           = aggressor;
    struct Combat_Phase temp_phase  = Combat_Phase_default;
    struct Damage damage            = forecast->stats.agg_damage;
    uf8 hit     = forecast->stats.agg_rates.hit;
    uf8 crit    = forecast->stats.agg_rates.crit;
    uf8 brave   = forecast->flow.aggressor_brave;
    temp_phase.attacker = SOTA_AGGRESSOR;

    Compute_Combat_Phase(tinymt32, &temp_phase, darr_attacks, damage, attacker, hit, crit, brave);
    phases[forecast->phase_num++] = temp_phase;

    /* -- Defendant, Retaliation 1 -- */
    if (forecast->flow.defendant_retaliates) {
        hit         = forecast->stats.dft_rates.hit;
        crit        = forecast->stats.dft_rates.crit;
        brave       = forecast->flow.defendant_brave;
        damage      = forecast->stats.dft_damage;
        attacker    = defendant;
        temp_phase.attacker = SOTA_DEFENDANT;

        Compute_Combat_Phase(tinymt32, &temp_phase, darr_attacks, damage, defendant, hit, crit, brave);
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

        Compute_Combat_Phase(tinymt32, &temp_phase, darr_attacks, damage, attacker, hit, crit, brave);
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

        Compute_Combat_Phase(tinymt32, &temp_phase, darr_attacks, damage, defendant, hit, crit, brave);
        phases[forecast->phase_num++] = temp_phase;
    }
    forecast->attack_num = DARR_NUM(darr_attacks);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Combat Attacks -- */
int Combat_Phase_Attack_Num(struct Combat_Phase *restrict phase, int brave_factor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(brave_factor >= 1);
    int attacks = 0;
    for (int i = 0; i < brave_factor; ++i) {
        SDL_assert(phase->skillp_multipliers[i] >= 1);
        // NOTE: += cause the multiplier is ADDED for EVERY BRAVE.
        //          -> that is the DEFINITION of a multiplication
        attacks += phase->skillp_multipliers[i] * phase->skill_multiplier;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (attacks);
}

int Combat_TotalAttack_Num(struct Combat_Phase *restrict phases, int brave_factor, int phase_num) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int total_attacks = 0;
    for (int i = 0; i < phase_num; ++i)
        total_attacks += Combat_Phase_Attack_Num(&phases[i], brave_factor);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (total_attacks);
}

void Compute_Combat_Phase(struct TINYMT32_T     *restrict tinymt32,
                          struct Combat_Phase *restrict phase,
                          struct Combat_Attack  *restrict darr_attacks, struct Damage        damage,
                          struct Unit *restrict attacker, uf8 hit_rate, uf8 crit_rate, uf8 brave_factor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    phase->attack_num = Combat_Phase_Attack_Num(phase, brave_factor);
    for (int i = 0; i < phase->attack_num; i++)
        Compute_Combat_Attack(tinymt32, phase, darr_attacks, damage, attacker, hit_rate, crit_rate);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Compute_Combat_Attack(struct TINYMT32_T    *restrict tinymt32,
                           struct Combat_Phase  *restrict phase,
                           struct Combat_Attack *restrict darr_attacks, struct Damage damage,
                           struct Unit *restrict attacker, uf8 hit_rate, uf8 crit_rate) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    temp_attack.hit = RNG_checkRate(tinymt32, attacker->hit_sequence.eff_rate, SOTA_RNG);
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
    temp_attack.crit = RNG_checkRate(tinymt32, crit_rate, SOTA_RNG);
    /* -- checking sequence -- */
    #if defined(RNG_SEQUENCE_BREAKER_CRIT) || defined(RNG_SEQUENCE_BREAKER_NOCRIT)
    RNG_checkSequence_twoWay(&attacker->crit_sequence, temp_attack.crit);
    #endif /* RNG_SEQUENCE_BREAKER_CRIT */

    /* --- COMPUTING TOTAL ATTACK DAMAGE --- */
    Combat_totalDamage(&temp_attack, &damage);

    temp_attack.attacker = phase->attacker;
    DARR_PUT(darr_attacks, temp_attack); // growing here breaks, address changes
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

uf8 Combat_Next_HP(struct Combat_Attack attack, uf8 hp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    uf8 next_hp = hp < attack.total_damage ? hp : hp - attack.total_damage;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (next_hp);
}

void Combat_Resolve(struct Combat_Attack *combat_attacks, uf8 attack_num,
                    struct Unit *aggressor, struct Unit *defendant) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attack_num > 0);
    SDL_assert(attack_num <= SOTA_COMBAT_MAX_PHASES);
    struct Unit *attacker, *defender;

    for (uf8 i = 0; i < attack_num; i++) {
        attacker = combat_attacks[i].attacker ? aggressor : defendant;
        defender = combat_attacks[i].attacker ? defendant : aggressor;

        if (Unit_canAttack(attacker))
            Resolve_Attack(combat_attacks[i], attacker, defender);

        if (!aggressor->is_alive | !defendant->is_alive)
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Resolve_Attack(struct Combat_Attack attack,
                    struct Unit *restrict attacker,
                    struct Unit *restrict defender) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (!attack.hit) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    if (defender->divine_shield) {
        defender->divine_shield = false;
    } else {
        Unit_takesDamage(defender, attack.total_damage, attack.crit);
        Unit_Equipped_Shields_Deplete(defender);
        Unit_Check_Equipped(defender);
    }
    Unit_Equipped_Weapons_Deplete(attacker);
    Unit_Check_Equipped(attacker);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
