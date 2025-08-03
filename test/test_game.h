
#include "nourstest.h"
#include "game/game.h"
#include "game/unit.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/equipment.h"
#include "combat.h"
#include "globals.h"
#include "equations.h"

void test_combat_game() {
    struct Game *IES = Game_New(Settings_default);
    SDL_assert(NULL == IES->ai.npcs);
    tnecs_world *world = gl_world;

    RNG_Init_xoroshiro256ss();
    // IES->combat.outcome.attacks = DARR_INIT(IES->combat.outcome.attacks, struct Combat_Attack,
    // SOTA_COMBAT_MAX_ATTACKS);

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
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, struct Item);
    Unit_Init(&attacker);
    Unit_Init(&defender);
    SDL_assert(gl_weapons_dtab != NULL);
    Unit_setClassind(&defender, UNIT_CLASS_FENCER);
    Unit_setClassind(&attacker, UNIT_CLASS_FENCER);
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);

    // Setting up inventory
    tnecs_entity fleuret  = IES_E_CREATE_wC(world, Inventory_item_ID);
    Inventory_item *inv_fleuret         = IES_GET_C(world, fleuret, Inventory_item);
    inv_fleuret->id = ITEM_ID_FLEURET;

    b32 attacker_equip_hand = Unit_Hand_Strong(&attacker);
    b32 defender_equip_hand = Unit_Hand_Strong(&defender);
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, fleuret, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, fleuret, attacker_equip_hand);
    Unit_Equip(&defender, defender_equip_hand, defender_equip_hand);
    Unit_Equip(&attacker, attacker_equip_hand, attacker_equip_hand);
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    Inventory_item *defender_invitem  = Unit_InvItem(&defender, defender_equip_hand);
    Inventory_item *attacker_invitem  = Unit_InvItem(&defender, attacker_equip_hand);

    SDL_assert(defender_invitem->id = ITEM_ID_FLEURET);
    SDL_assert(attacker_invitem->id = ITEM_ID_FLEURET);
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    SDL_assert(defender_invitem->id = ITEM_ID_FLEURET);
    SDL_assert(attacker_invitem->id = ITEM_ID_FLEURET);
    nourstest_true(attacker.hp.current == attacker_stats.hp);
    nourstest_true(defender.hp.current == defender_stats.hp);
    Unit_stats ES_A = Unit_effectiveStats(&attacker);
    Unit_stats ES_D = Unit_effectiveStats(&defender);
    Computed_Stats cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    Computed_Stats cs_dft = Unit_computedStats(&defender, distance, ES_D);

    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    SDL_assert(defender_invitem->id = ITEM_ID_FLEURET);
    SDL_assert(attacker_invitem->id = ITEM_ID_FLEURET);

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
    struct Weapon *attacker_weaponp = NULL;
    struct Weapon *defender_weaponp = NULL;
    uint_fast16_t temp_id;

    // checking defender equipped");
    SDL_assert(Unit_isEquipped(&defender, defender_equip_hand));
    if (Unit_isEquipped(&defender, defender_equip_hand)) {
        SDL_assert(gl_weapons_dtab != NULL);
        temp_id = defender_invitem->id;
        defender_weaponp = ((struct Weapon *)DTAB_GET(gl_weapons_dtab, temp_id));
        SDL_assert(defender_weaponp != NULL);
    }

    // checking attacker equipped");
    SDL_assert(Unit_isEquipped(&attacker, attacker_equip_hand));
    if (Unit_isEquipped(&attacker, attacker_equip_hand)) {
        SDL_assert(gl_weapons_dtab != NULL);
        temp_id = attacker_invitem->id;
        attacker_weaponp = (struct Weapon *)DTAB_GET(gl_weapons_dtab, temp_id);
        SDL_assert(attacker_weaponp != NULL);
    }

    // manual Eq_Unit_Hit");
    SDL_assert(attacker_weaponp != NULL);
    nourstest_true(attacker_weaponp->stats.hit);
    nourstest_true(ES_A.dex);
    nourstest_true(ES_A.luck);
    nourstest_true(Eq_Unit_Hit(attacker_weaponp->stats.hit, ES_A.dex,
                               ES_A.luck, 0) == attacker_hit);
    nourstest_true(Eq_Unit_Hit(defender_weaponp->stats.hit, ES_D.dex,
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
    nourstest_true(Eq_Unit_Speed(attacker_weaponp->stats.wgt,
                                 ES_A.agi,
                                 ES_A.con,
                                 ES_A.str, 0) == attacker_speed);
    Unit_computeSpeed(&defender, distance, &defender_speed);
    nourstest_true(Eq_Unit_Speed(defender_weaponp->stats.wgt,
                                 ES_D.agi,
                                 ES_D.con,
                                 ES_D.str, 0) == defender_speed);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    &attacker_pos, &defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender, &defender_pos,
                   &attacker_pos));

    // struct Combat_Forecast test_Combat_Forecast;

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 2);
    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.attacks[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].attacker ==
                   IES->combat.outcome.phases[1].attacker);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 1;
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.stats.current.agi = attacker_stats.agi);
    nourstest_true(ES_A.agi = attacker_stats.agi);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 2);
    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 2;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 2);
    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 3;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 2);
    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 4;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 2);
    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);

    attacker_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(ES_A.agi == attacker_stats.agi);
    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 2);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 3);
    nourstest_true(IES->combat.forecast.attack_num == 3);
    nourstest_true(IES->combat.forecast.phase_num == 3);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);
    nourstest_true(IES->combat.outcome.phases[2].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[2].hit);
    nourstest_true(!IES->combat.outcome.attacks[2].crit);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 2);
    nourstest_true(IES->combat.forecast.flow.defendant_retaliates == true);
    nourstest_true(IES->combat.forecast.stats.agg_rates.hit == Eq_Combat_Hit(
                           cs_agg.hit, cs_dft.dodge));
    nourstest_true(IES->combat.forecast.stats.dft_rates.hit == Eq_Combat_Hit(
                           cs_dft.hit, cs_agg.dodge));
    nourstest_true(IES->combat.forecast.stats.agg_rates.crit == Eq_Combat_Crit(
                           cs_agg.crit, cs_dft.favor));
    nourstest_true(IES->combat.forecast.stats.dft_rates.crit == Eq_Combat_Crit(
                           cs_dft.crit, cs_agg.favor));
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    nourstest_true(DARR_NUM(IES->combat.outcome.attacks) == 3);
    nourstest_true(IES->combat.forecast.attack_num == 3);
    nourstest_true(IES->combat.forecast.phase_num == 3);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);
    nourstest_true(IES->combat.outcome.phases[2].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[2].hit);
    nourstest_true(!IES->combat.outcome.attacks[2].crit);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);
    attacker_stats.agi = attacker_stats.agi;
    defender_stats.agi = attacker_stats.agi;
    Unit_setStats(&defender, defender_stats);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    SDL_assert(Unit_canAttack(&attacker));
    SDL_assert(Unit_canAttack(&defender));
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    SDL_assert(ES_A.str > 0);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    SDL_assert(cs_agg.attack.physical > 0);

    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);

    SDL_assert(cs_agg.attack.physical > 0);
    SDL_assert(IES->combat.forecast.stats.agg_stats.attack.physical > 0);
    SDL_assert(IES->combat.forecast.stats.agg_stats.attack.physical == cs_agg.attack.physical);

    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    attacker.hp.current = attacker.stats.current.hp;

    nourstest_true(IES->combat.forecast.attack_num == 2);
    nourstest_true(IES->combat.forecast.phase_num == 2);
    nourstest_true(IES->combat.outcome.phases[0].attacker == SOTA_AGGRESSOR);
    nourstest_true(IES->combat.outcome.attacks[0].hit);
    nourstest_true(!IES->combat.outcome.attacks[0].crit);
    nourstest_true(IES->combat.outcome.phases[1].attacker == SOTA_DEFENDANT);
    nourstest_true(IES->combat.outcome.attacks[1].hit);
    nourstest_true(!IES->combat.outcome.attacks[1].crit);
    Combat_Resolve(IES->combat.outcome.attacks,
                   IES->combat.forecast.attack_num,
                   &attacker, &defender);
    nourstest_true(attacker.hp.current == (attacker.stats.current.hp -
                                           (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.hp.current == (defender.stats.current.hp -
                                           (cs_agg.attack.physical - ES_D.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    defender_stats.agi = defender_stats.agi + 5;
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    Combat_Resolve(IES->combat.outcome.attacks, IES->combat.forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 2);
    nourstest_true(attacker.hp.current == (attacker.stats.current.hp - 2 *
                                           (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.hp.current == (defender.stats.current.hp -
                                           (cs_agg.attack.physical - ES_D.def)));

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    defender_stats.agi = attacker_stats.agi;
    attacker_stats.agi = attacker_stats.agi + 5;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos,
                                                   &defender_pos);
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    Combat_Resolve(IES->combat.outcome.attacks, IES->combat.forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(attacker.hp.current == (attacker.stats.current.hp -
                                           (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.hp.current == (defender.stats.current.hp - 2 *
                                           (cs_agg.attack.physical - ES_D.def)));

    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 2);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = defender_stats.agi;
    Unit_setStats(&attacker, attacker_stats);
    IES->combat.forecast = Compute_Combat_Forecast(&attacker, &defender,
                                                   &attacker_pos, &defender_pos);
    Compute_Combat_Outcome(&IES->combat.outcome, &IES->combat.forecast,
                           &attacker, &defender);
    IES->combat.outcome.attacks[0].crit = true;
    Combat_Resolve(IES->combat.outcome.attacks, IES->combat.forecast.attack_num, &attacker,
                   &defender);
    nourstest_true(attacker.hp.current == (ES_A.hp -
                                           (cs_dft.attack.physical - ES_A.def)));
    nourstest_true(defender.hp.current == (defender.stats.current.hp -
                                           (cs_agg.attack.physical - ES_D.def)));

    nourstest_true(IES->combat.forecast.flow.aggressor_phases == 1);
    nourstest_true(IES->combat.forecast.flow.defendant_phases == 1);

    /* --- SDL_free --- */
    Unit_Free(&attacker);
    Unit_Free(&defender);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    Game_Free(IES);
}

void test_game() {
    test_combat_game();
}
