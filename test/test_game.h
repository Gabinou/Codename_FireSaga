
#include "nourstest.h"
#include "game/game.h"
#include "game/unit.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/equipment.h"
#include "combat.h"
#include "equations.h"

void test_combat_game() {

    struct Game firesaga = Game_default;
    RNG_Init_xoroshiro256ss();
    firesaga.combat_outcome.attacks = DARR_INIT(firesaga.combat_outcome.attacks, struct Combat_Attack,
                                                SOTA_COMBAT_MAX_ATTACKS);

    // Init test_combat
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
    struct dtab *items_dtab = DTAB_INIT(weapons_dtab, struct Item);
    Unit_InitWweapons(&attacker, weapons_dtab);
    Unit_InitWweapons(&defender, weapons_dtab);
    firesaga.weapons_dtab = weapons_dtab;
    SDL_assert(weapons_dtab != NULL);
    Unit_setClassind(&defender, UNIT_CLASS_FENCER);
    Unit_setClassind(&attacker, UNIT_CLASS_FENCER);
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);

    // Setting up inventory
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    b32 attacker_equip_hand = Unit_Hand_Strong(&attacker);
    b32 defender_equip_hand = Unit_Hand_Strong(&defender);
    attacker.equipment.items_dtab = items_dtab;
    defender.equipment.items_dtab = items_dtab;
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    Unit_Equip(&defender, defender_equip_hand, defender_equip_hand);
    Unit_Equip(&attacker, attacker_equip_hand, attacker_equip_hand);
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    struct Inventory_item *defender_eq  = Unit_Equipment(&defender);
    struct Inventory_item *attacker_eq  = Unit_Equipment(&attacker);
    SDL_assert(defender_eq[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(defender_eq[attacker_equip_hand].id = ITEM_ID_FLEURET);
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    SDL_assert(defender_eq[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(attacker_eq[attacker_equip_hand].id = ITEM_ID_FLEURET);
    nourstest_true(attacker.counters.hp == attacker_stats.hp);
    nourstest_true(defender.counters.hp == defender_stats.hp);
    Unit_stats ES_A = Unit_effectiveStats(&attacker);
    Unit_stats ES_D = Unit_effectiveStats(&defender);
    Computed_Stats cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    Computed_Stats cs_dft = Unit_computedStats(&defender, distance, ES_D);

    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    SDL_assert(defender_eq[defender_equip_hand].id = ITEM_ID_FLEURET);
    SDL_assert(attacker_eq[attacker_equip_hand].id = ITEM_ID_FLEURET);

    struct Weapon *fleuret = ((struct Weapon *)DTAB_GET(weapons_dtab, in_wpn.id));

    // computing attacker effectiveStats
    ES_A = Unit_effectiveStats(&attacker);
    nourstest_true(attacker.stats.current.hp    == ES_A.hp);
    nourstest_true(attacker.stats.current.str   == ES_A.str);
    nourstest_true(attacker.stats.current.mag   == ES_A.mag);
    nourstest_true(attacker.stats.current.agi   == ES_A.agi);
    nourstest_true(attacker.stats.current.dex   == ES_A.dex);
    nourstest_true(attacker.stats.current.luck  == ES_A.luck);
    nourstest_true(attacker.stats.current.def   == ES_A.def);
    nourstest_true(attacker.stats.current.res   == ES_A.res);
    nourstest_true(attacker.stats.current.con   == ES_A.con);
    nourstest_true(attacker.stats.current.move  == ES_A.move);
    nourstest_true(attacker.stats.current.prof  == ES_A.prof);

    // computing defender effectiveStats
    ES_D = Unit_effectiveStats(&defender);
    nourstest_true(defender.stats.current.hp    == ES_D.hp);
    nourstest_true(defender.stats.current.str   == ES_D.str);
    nourstest_true(defender.stats.current.mag   == ES_D.mag);
    nourstest_true(defender.stats.current.agi   == ES_D.agi);
    nourstest_true(defender.stats.current.dex   == ES_D.dex);
    nourstest_true(defender.stats.current.luck  == ES_D.luck);
    nourstest_true(defender.stats.current.def   == ES_D.def);
    nourstest_true(defender.stats.current.res   == ES_D.res);
    nourstest_true(defender.stats.current.con   == ES_D.con);
    nourstest_true(defender.stats.current.move  == ES_D.move);
    nourstest_true(defender.stats.current.prof  == ES_D.prof);

    // computing attacker effectiveStats");
    i32 attacker_hit;
    i32 defender_hit;
    // computing defender effectiveStats");
    Unit_computeHit(&attacker, distance, &attacker_hit);
    Unit_computeHit(&defender, distance, &defender_hit);
    struct Weapon *attacker_weaponp;
    struct Weapon *defender_weaponp;
    uint_fast16_t temp_id;

    // checking defender equipped");
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    if (Unit_isEquipped(&defender, defender_equip_hand)) {
        struct dtab *weapons_dtab = Unit_dtab_Weapons(&defender);
        SDL_assert(weapons_dtab != NULL);
        SDL_assert(defender_eq != NULL);
        temp_id = defender_eq[defender_equip_hand].id;
        SDL_assert(temp_id == in_wpn.id);
        defender_weaponp = ((struct Weapon *)DTAB_GET(weapons_dtab, temp_id));
        SDL_assert(defender_weaponp != NULL);
    }

    // checking attacker equipped");
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    if (Unit_isEquipped(&attacker, attacker_equip_hand)) {
        struct dtab *weapons_dtab = Unit_dtab_Weapons(&attacker);
        SDL_assert(weapons_dtab != NULL);
        temp_id = attacker_eq[attacker_equip_hand].id;
        attacker_weaponp = ((struct Weapon *)DTAB_GET(weapons_dtab, temp_id));
        nourstest_true(attacker_weaponp != NULL);
    }

    // manual Equation_Unit_Hit");
    nourstest_true(attacker_weaponp != NULL);
    nourstest_true(attacker_weaponp->stats.hit);
    nourstest_true(ES_A.dex);
    nourstest_true(ES_A.luck);
    nourstest_true(Equation_Unit_Hit(attacker_weaponp->stats.hit, ES_A.dex,
                                     ES_A.luck, 0) == attacker_hit);
    nourstest_true(Equation_Unit_Hit(defender_weaponp->stats.hit, ES_D.dex,
                                     ES_D.luck, 0) == defender_hit);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;

    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    i32 attacker_speed;
    i32 defender_speed;
    Unit_computeSpeed(&attacker, distance, &attacker_speed);
    nourstest_true(Equation_Unit_Speed(attacker_weaponp->stats.wgt,
                                       ES_A.agi,
                                       ES_A.con,
                                       ES_A.str, 0) == attacker_speed);
    Unit_computeSpeed(&defender, distance, &defender_speed);
    nourstest_true(Equation_Unit_Speed(defender_weaponp->stats.wgt,
                                       ES_D.agi,
                                       ES_D.con,
                                       ES_D.str, 0) == defender_speed);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    &attacker_pos, &defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender, &defender_pos,
                   &attacker_pos));

    struct Combat_Forecast test_Combat_Forecast;

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.attacks[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].attacker ==
                   firesaga.combat_outcome.phases[1].attacker);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 1;
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.stats.current.agi = attacker_stats.agi);
    nourstest_true(ES_A.agi = attacker_stats.agi);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 2;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 3;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 4;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 2);
    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(ES_A.agi == attacker_stats.agi);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 3);
    nourstest_true(firesaga.combat_forecast.attack_num == 3);
    nourstest_true(firesaga.combat_forecast.phase_num == 3);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);
    nourstest_true(firesaga.combat_outcome.phases[2].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[2].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[2].crit);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_retaliates == true);
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.hit == Equation_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.hit == Equation_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(firesaga.combat_forecast.stats.agg_rates.crit == Equation_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(firesaga.combat_forecast.stats.dft_rates.crit == Equation_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(firesaga.combat_outcome.attacks) == 3);
    nourstest_true(firesaga.combat_forecast.attack_num == 3);
    nourstest_true(firesaga.combat_forecast.phase_num == 3);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);
    nourstest_true(firesaga.combat_outcome.phases[2].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[2].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[2].crit);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);
    attacker_stats.agi = attacker_stats.agi;
    defender_stats.agi = attacker_stats.agi;
    Unit_setStats(&defender, defender_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);

    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    attacker.counters.hp = attacker.stats.current.hp;

    nourstest_true(firesaga.combat_forecast.attack_num == 2);
    nourstest_true(firesaga.combat_forecast.phase_num == 2);
    nourstest_true(firesaga.combat_outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(firesaga.combat_outcome.attacks[0].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[0].crit);
    nourstest_true(firesaga.combat_outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(firesaga.combat_outcome.attacks[1].hit);
    nourstest_true(!firesaga.combat_outcome.attacks[1].crit);

    Combat_Resolve(firesaga.combat_outcome.attacks,
                   firesaga.combat_forecast.attack_num,
                   &attacker, &defender);
    nourstest_true(attacker.counters.hp == (attacker.stats.current.hp -
                                            (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.counters.hp == (defender.stats.current.hp -
                                            (cs_agg.attack.physical - ES_D.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    Combat_Resolve(firesaga.combat_outcome.attacks, firesaga.combat_forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 2);
    nourstest_true(attacker.counters.hp == (attacker.stats.current.hp - 2 *
                                            (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.counters.hp == (defender.stats.current.hp -
                                            (cs_agg.attack.physical - ES_D.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    defender_stats.agi = attacker_stats.agi;
    attacker_stats.agi = attacker_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos,
                                                       &defender_pos);
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    Combat_Resolve(firesaga.combat_outcome.attacks, firesaga.combat_forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(attacker.counters.hp == (attacker.stats.current.hp -
                                            (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.counters.hp == (defender.stats.current.hp - 2 *
                                            (cs_agg.attack.physical - ES_D.def)));

    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 2);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    Unit_setStats(&attacker, attacker_stats);
    firesaga.combat_forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                       &attacker_pos, &defender_pos);
    Compute_Combat_Outcome(&firesaga.combat_outcome, &firesaga.combat_forecast,
                           &attacker, &defender);
    firesaga.combat_outcome.attacks[0].crit = true;
    Combat_Resolve(firesaga.combat_outcome.attacks, firesaga.combat_forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(attacker.counters.hp == (ES_A.hp -
                                            (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.counters.hp == (defender.stats.current.hp -
                                            (cs_agg.attack.physical - ES_D.def)));

    nourstest_true(firesaga.combat_forecast.flow.aggressor_phases == 1);
    nourstest_true(firesaga.combat_forecast.flow.defendant_phases == 1);

    /* --- SDL_free --- */
    Unit_Free(&attacker);
    Unit_Free(&defender);
    Game_Weapons_Free(&weapons_dtab);
    DARR_FREE(firesaga.combat_outcome.attacks);
}

void test_game() {

    test_combat_game();
}
