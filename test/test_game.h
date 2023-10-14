#include "nourstest.h"

#include "game/game.h"
#include "game/unit.h"
#include "unit.h"
#include "combat.h"
#include "equations.h"

void test_combat_game() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    struct Game firesaga = Game_default;
    // Game_Init(&firesaga);
    RNG_Init_tinymt(&firesaga.tinymt32);
    firesaga.combat_attacks = DARR_INIT(firesaga.combat_attacks, struct Combat_Attack,
                                        SOTA_COMBAT_MAX_ATTACKS);

    SDL_Log("Init test_combat");
    struct Combat_Flow temp_flow;
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    struct Point attacker_pos = {0, 0};
    struct Point defender_pos = {1, 0};
    int distance = 1;
    //                               hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats attacker_stats = {19,  4,  2,  7,  7,  7, 7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {19,  4,  2,  7,  7,  7, 7,  4,  5,  6, 5};
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(&attacker, weapons_dtab);
    Unit_InitWweapons(&defender, weapons_dtab);
    firesaga.weapons_dtab = weapons_dtab;
    SDL_assert(weapons_dtab != NULL);
    Unit_setClassind(&defender, UNIT_CLASS_FENCER);
    Unit_setClassind(&attacker, UNIT_CLASS_FENCER);
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);

    SDL_Log("Setting up inventory");
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    bool attacker_equip_hand = UNIT_HAND_WEAK;
    bool defender_equip_hand = UNIT_HAND_WEAK;
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    defender.equipped[defender_equip_hand] = true;
    attacker.equipped[attacker_equip_hand] = true;
    SDL_assert(defender.equipped[defender_equip_hand]);
    SDL_assert(attacker.equipped[attacker_equip_hand]);
    SDL_assert(defender._equipment[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(attacker._equipment[attacker_equip_hand].id = ITEM_ID_FLEURET);
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    SDL_assert(defender.equipped[defender_equip_hand]);
    SDL_assert(attacker.equipped[attacker_equip_hand]);
    SDL_assert(defender._equipment[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(attacker._equipment[attacker_equip_hand].id = ITEM_ID_FLEURET);
    nourstest_true(attacker.current_hp == attacker_stats.hp);
    nourstest_true(defender.current_hp == defender_stats.hp);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    SDL_assert(defender.equipped[defender_equip_hand]);
    SDL_assert(attacker.equipped[attacker_equip_hand]);
    SDL_assert(defender._equipment[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(attacker._equipment[attacker_equip_hand].id = ITEM_ID_FLEURET);

    struct Weapon *fleuret = ((struct Weapon *)DTAB_GET(weapons_dtab, in_wpn.id));

    SDL_Log("computing attacker effectiveStats");
    Unit_effectiveStats(&attacker);
    nourstest_true(attacker.current_stats.hp    == attacker.effective_stats.hp);
    nourstest_true(attacker.current_stats.str   == attacker.effective_stats.str);
    nourstest_true(attacker.current_stats.mag   == attacker.effective_stats.mag);
    nourstest_true(attacker.current_stats.agi   == attacker.effective_stats.agi);
    nourstest_true(attacker.current_stats.dex   == attacker.effective_stats.dex);
    nourstest_true(attacker.current_stats.luck  == attacker.effective_stats.luck);
    nourstest_true(attacker.current_stats.def   == attacker.effective_stats.def);
    nourstest_true(attacker.current_stats.res   == attacker.effective_stats.res);
    nourstest_true(attacker.current_stats.con   == attacker.effective_stats.con);
    nourstest_true(attacker.current_stats.move  == attacker.effective_stats.move);
    nourstest_true(attacker.current_stats.prof  == attacker.effective_stats.prof);

    SDL_Log("computing defender effectiveStats");
    Unit_effectiveStats(&defender);
    nourstest_true(defender.current_stats.hp    == defender.effective_stats.hp);
    nourstest_true(defender.current_stats.str   == defender.effective_stats.str);
    nourstest_true(defender.current_stats.mag   == defender.effective_stats.mag);
    nourstest_true(defender.current_stats.agi   == defender.effective_stats.agi);
    nourstest_true(defender.current_stats.dex   == defender.effective_stats.dex);
    nourstest_true(defender.current_stats.luck  == defender.effective_stats.luck);
    nourstest_true(defender.current_stats.def   == defender.effective_stats.def);
    nourstest_true(defender.current_stats.res   == defender.effective_stats.res);
    nourstest_true(defender.current_stats.con   == defender.effective_stats.con);
    nourstest_true(defender.current_stats.move  == defender.effective_stats.move);
    nourstest_true(defender.current_stats.prof  == defender.effective_stats.prof);

    SDL_Log("computing attacker effectiveStats");
    Unit_computeHit(&attacker, distance);
    SDL_Log("computing defender effectiveStats");
    Unit_computeHit(&defender, distance);
    uf8 attacker_hit;
    uf8 defender_hit;
    struct Weapon *attacker_weaponp;
    struct Weapon *defender_weaponp;
    uint_fast16_t temp_id;

    SDL_Log("checking defender equipped");
    SDL_assert(defender.equipped[defender_equip_hand]);
    if (defender.equipped[defender_equip_hand]) {
        SDL_assert(defender.weapons_dtab != NULL);
        SDL_assert(defender._equipment != NULL);
        temp_id = defender._equipment[defender_equip_hand].id;
        SDL_assert(temp_id == in_wpn.id);
        defender_weaponp = ((struct Weapon *)DTAB_GET(defender.weapons_dtab, temp_id));
        SDL_assert(defender_weaponp != NULL);
    }

    SDL_Log("checking attacker equipped");
    SDL_assert(attacker.equipped[attacker_equip_hand]);
    if (attacker.equipped[attacker_equip_hand]) {
        SDL_assert(attacker.weapons_dtab != NULL);
        attacker_weaponp = ((struct Weapon *)DTAB_GET(attacker.weapons_dtab,
                                                      attacker._equipment[attacker_equip_hand].id));
        nourstest_true(attacker_weaponp != NULL);
    }

    SDL_Log("manual Equation_Unit_Hit");
    nourstest_true(attacker_weaponp != NULL);
    nourstest_true(attacker_weaponp->stats.hit);
    nourstest_true(attacker.effective_stats.dex);
    nourstest_true(attacker.effective_stats.luck);
    attacker_hit = Equation_Unit_Hit(attacker_weaponp->stats.hit, attacker.effective_stats.dex,
                                     attacker.effective_stats.luck, 0);
    nourstest_true(attacker.computed_stats.hit == attacker_hit);
    defender_hit = Equation_Unit_Hit(defender_weaponp->stats.hit, defender.effective_stats.dex,
                                     defender.effective_stats.luck, 0);
    nourstest_true(defender.computed_stats.hit == defender_hit);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;

    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    int_fast8_t attacker_speed = Equation_Unit_Speed(attacker_weaponp->stats.wgt,
                                                     attacker.effective_stats.agi, attacker.effective_stats.con, attacker.effective_stats.str);
    nourstest_true(Unit_computeSpeed(&attacker, distance) == attacker_speed);
    nourstest_true(attacker.computed_stats.speed == attacker_speed);
    int_fast8_t defender_speed = Equation_Unit_Speed(defender_weaponp->stats.wgt,
                                                     defender.effective_stats.agi, defender.effective_stats.con, defender.effective_stats.str);
    nourstest_true(Unit_computeSpeed(&defender, distance) == defender_speed);
    nourstest_true(defender.computed_stats.speed == defender_speed);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, &attacker_pos,
                                    &defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, &defender_pos, &attacker_pos));

    struct Combat_Forecast test_Combat_Forecast;

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_attacks[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].attacker == firesaga.combat_phases[1].attacker);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 1;
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.current_stats.agi = attacker_stats.agi);
    nourstest_true(attacker.effective_stats.agi = attacker_stats.agi);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 2;
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 3;
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 4;
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.effective_stats.agi == attacker_stats.agi);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 3);
    nourstest_true(firesaga.combat_forecast.attack_num == 3);
    nourstest_true(firesaga.combat_forecast.phase_num == 3);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);
    nourstest_true(firesaga.combat_phases[2].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[2].hit);
    nourstest_true(!firesaga.combat_attacks[2].crit);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           attacker.computed_stats.hit, defender.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           defender.computed_stats.hit, attacker.computed_stats.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           attacker.computed_stats.crit, defender.computed_stats.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           defender.computed_stats.crit, attacker.computed_stats.favor));
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_attacks) == 3);
    nourstest_true(firesaga.combat_forecast.attack_num == 3);
    nourstest_true(firesaga.combat_forecast.phase_num == 3);
    nourstest_true(firesaga.combat_phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_attacks[0].hit);
    nourstest_true(!firesaga.combat_attacks[0].crit);
    nourstest_true(firesaga.combat_phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[1].hit);
    nourstest_true(!firesaga.combat_attacks[1].crit);
    nourstest_true(firesaga.combat_phases[2].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_attacks[2].hit);
    nourstest_true(!firesaga.combat_attacks[2].crit);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);
    attacker_stats.agi = attacker_stats.agi;
    defender_stats.agi = attacker_stats.agi;
    Unit_setStats(&defender, defender_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);

    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);

    Combat_Resolve(firesaga.combat_attacks, firesaga.combat_forecast.attack_num, &attacker, &defender);
    nourstest_true(attacker.current_hp == (attacker.current_stats.hp -
                                           (defender.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - attacker.effective_stats.def)));
    nourstest_true(defender.current_hp == (defender.current_stats.hp -
                                           (attacker.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - defender.effective_stats.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    Combat_Resolve(firesaga.combat_attacks, firesaga.combat_forecast.attack_num, &attacker, &defender);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 2);
    nourstest_true(attacker.current_hp == (attacker.current_stats.hp - 2 *
                                           (defender.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - attacker.effective_stats.def)));
    nourstest_true(defender.current_hp == (defender.current_stats.hp -
                                           (attacker.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - defender.effective_stats.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    defender_stats.agi = attacker_stats.agi;
    attacker_stats.agi = attacker_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    Combat_Resolve(firesaga.combat_attacks, firesaga.combat_forecast.attack_num, &attacker, &defender);
    nourstest_true(attacker.current_hp == (attacker.current_stats.hp -
                                           (defender.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - attacker.effective_stats.def)));
    nourstest_true(defender.current_hp == (defender.current_stats.hp - 2 *
                                           (attacker.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - defender.effective_stats.def)));

    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos, &defender_pos);
    Compute_Combat_Outcome(firesaga.combat_phases, firesaga.combat_attacks, &firesaga.combat_forecast,
                           &firesaga.tinymt32, &attacker, &defender);
    firesaga.combat_attacks[0].crit = true;
    Combat_Resolve(firesaga.combat_attacks, firesaga.combat_forecast.attack_num, &attacker, &defender);
    nourstest_true(attacker.current_hp == (attacker.effective_stats.hp -
                                           (defender.computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - attacker.effective_stats.def)));
    nourstest_true(defender.current_hp == (defender.current_stats.hp -
                                           (attacker
                                            .computed_stats.attack[DAMAGE_TYPE_PHYSICAL] - defender.effective_stats.def)));

    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);

    /* --- FREE --- */
    Unit_Free(&attacker);
    Unit_Free(&defender);
    Game_Weapons_Free(weapons_dtab);
    DARR_FREE(firesaga.combat_attacks);
}

void test_game() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    test_combat_game();
}
