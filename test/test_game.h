#include "minctest.h"
#include "game.h"
#include "unit.h"
#include "equations.h"

void test_combat() {
    SDL_Log("test_combat");
    struct Game firesaga = Game_default;
    // Game_init(&firesaga);
    init_tinyMT(&firesaga.tinymt32);

    struct Combat_Forecast_Flow temp_flow;
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    struct Point attacker_pos = Point_default;
    struct Point defender_pos = Point_default;
    //hp, str, mag, agi, dex, luck, def, res, con, move
    struct Unit_stats attacker_stats = {23,  4,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {23,  4,  2,  7,  7,   7,  4,  5,  6, 5};
    struct Weapons_hash * weapons1 = NULL;
    hmdefault(weapons1, Weapon_default);
    Unit_setClassind(&defender, UNIT_CLASS_MERCENARY);
    Unit_setClassind(&attacker, UNIT_CLASS_MERCENARY);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_NAME_FLEURET;
    Unit_Item_Add(&defender, in_wpn);
    Unit_Item_Add(&attacker, in_wpn);
    attacker.weapons = &weapons1;
    defender.weapons = &weapons1;
    bool attacker_equip_hand = UNIT_HAND_RIGHT;
    bool defender_equip_hand = UNIT_HAND_RIGHT;
    Unit_equips(&defender, attacker_equip_hand);
    Unit_equips(&attacker, defender_equip_hand);
    lok(Unit_iswpnEquipped(&defender));
    lok(Unit_iswpnEquipped(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    lok(attacker.current_hp == attacker_stats.hp);
    lok(defender.current_hp == defender_stats.hp);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);

    Unit_effectiveStats(&attacker);
    lok(attacker.current_stats.hp == attacker.effective_stats.hp);
    lok(attacker.current_stats.str == attacker.effective_stats.str);
    lok(attacker.current_stats.mag == attacker.effective_stats.mag);
    lok(attacker.current_stats.agi == attacker.effective_stats.agi);
    lok(attacker.current_stats.dex == attacker.effective_stats.dex);
    lok(attacker.current_stats.luck == attacker.effective_stats.luck);
    lok(attacker.current_stats.def == attacker.effective_stats.def);
    lok(attacker.current_stats.res == attacker.effective_stats.res);
    lok(attacker.current_stats.con == attacker.effective_stats.con);
    lok(attacker.current_stats.move == attacker.effective_stats.move);
    lok(attacker.current_stats.prof == attacker.effective_stats.prof);

    Unit_effectiveStats(&defender);
    lok(defender.current_stats.hp == defender.effective_stats.hp);
    lok(defender.current_stats.str == defender.effective_stats.str);
    lok(defender.current_stats.mag == defender.effective_stats.mag);
    lok(defender.current_stats.agi == defender.effective_stats.agi);
    lok(defender.current_stats.dex == defender.effective_stats.dex);
    lok(defender.current_stats.luck == defender.effective_stats.luck);
    lok(defender.current_stats.def == defender.effective_stats.def);
    lok(defender.current_stats.res == defender.effective_stats.res);
    lok(defender.current_stats.con == defender.effective_stats.con);
    lok(defender.current_stats.move == defender.effective_stats.move);
    lok(defender.current_stats.prof == defender.effective_stats.prof);

    Unit_computeHit(&attacker);
    Unit_computeHit(&defender);
    uint8_t attacker_hit;
    uint8_t defender_hit;
    struct Weapon attacker_weapon = Weapon_default;
    struct Weapon defender_weapon = Weapon_default;


    if (defender.equipped[defender_equip_hand]) {
        defender_weapon = hmget(weapons1, defender._equipment[defender_equip_hand].id);
    }
    if (attacker.equipped[attacker_equip_hand]) {
        attacker_weapon = hmget(weapons1, attacker._equipment[attacker_equip_hand].id);
    }

    attacker_hit = equation_unitHit(attacker_weapon.stats.combat.hit, attacker.effective_stats.dex, attacker.effective_stats.luck, 0);
    lok(attacker.computed_stats.hit == attacker_hit);
    defender_hit = equation_unitHit(defender_weapon.stats.combat.hit, defender.effective_stats.dex, defender.effective_stats.luck, 0);
    lok(defender.computed_stats.hit == defender_hit);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;

    Unit_computedStats(&attacker);
    Unit_computedStats(&attacker);
    Unit_computedStats(&defender);
    int8_t attacker_speed = equation_unitSpeed(attacker_weapon.stats.wgt, attacker.effective_stats.agi, attacker.effective_stats.con, attacker.effective_stats.str);
    lok(Unit_computeSpeed(&attacker) == attacker_speed);
    lok(attacker.computed_stats.speed == attacker_speed);
    lok(attacker.computed_stats.speed == 7);
    int8_t defender_speed = equation_unitSpeed(defender_weapon.stats.wgt, defender.effective_stats.agi, defender.effective_stats.con, defender.effective_stats.str);
    lok(Unit_computeSpeed(&defender) == defender_speed);
    lok(defender.computed_stats.speed == defender_speed);
    lok(defender.computed_stats.speed == 7);
    temp_flow = Unit_Combat_Forecast_Flow(&attacker, &attacker_pos, &defender, &defender_pos);
    lok(temp_flow.defender_retaliates == Unit_canAttack(&defender, &defender_pos, &attacker, &attacker_pos));
    lok(temp_flow.attacker_doubles == 0);
    lok(temp_flow.defender_doubles == 0);


    struct Combat_Forecast test_Combat_Forecast;

    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 2);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);

    attacker_stats.agi = 8;
    Unit_setStats(&attacker, attacker_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 2);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);

    attacker_stats.agi = 9;
    Unit_setStats(&attacker, attacker_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 2);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);

    attacker_stats.agi = 10;
    Unit_setStats(&attacker, attacker_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 2);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);

    attacker_stats.agi = 11;
    Unit_setStats(&attacker, attacker_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 2);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);

    attacker_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == true);
    lok(firesaga.combat_forecast.flow.defender_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 3);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);
    lok(firesaga.outcome_combat[2].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[2].hit);
    lok(!firesaga.outcome_combat[2].crit);


    attacker_stats.agi = 7;
    defender_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    test_Combat_Forecast = Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    lok(firesaga.combat_forecast.flow.attacker_doubles == false);
    lok(firesaga.combat_forecast.flow.defender_doubles == true);
    lok(firesaga.combat_forecast.flow.defender_retaliates == true);
    lok(firesaga.combat_forecast.stats.attacker_rates.hit == equation_rateHit(attacker.computed_stats.hit, defender.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.defender_rates.hit == equation_rateHit(defender.computed_stats.hit, attacker.computed_stats.dodge));
    lok(firesaga.combat_forecast.stats.attacker_rates.crit == equation_rateCrit(attacker.computed_stats.crit, defender.computed_stats.favor));
    lok(firesaga.combat_forecast.stats.defender_rates.crit == equation_rateCrit(defender.computed_stats.crit, attacker.computed_stats.favor));
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    lok(firesaga.Combat_Outcome_Attacks_num == 3);
    lok(firesaga.outcome_combat[0].aggressor == ATTACKER);
    lok(firesaga.outcome_combat[0].hit);
    lok(!firesaga.outcome_combat[0].crit);
    lok(firesaga.outcome_combat[1].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[1].hit);
    lok(!firesaga.outcome_combat[1].crit);
    lok(firesaga.outcome_combat[2].aggressor == DEFENDER);
    lok(firesaga.outcome_combat[2].hit);
    lok(!firesaga.outcome_combat[2].crit);

    defender_stats.agi = 7;
    Unit_setStats(&defender, defender_stats);
    Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    Game_Combat_Resolves(&firesaga, &attacker, &defender);
    lok(attacker.current_hp == 15);
    lok(defender.current_hp == 15);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = 7;
    defender_stats.agi = 12;
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    Game_Combat_Resolves(&firesaga, &attacker, &defender);
    lok(attacker.current_hp == 7);
    lok(defender.current_hp == 15);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    defender_stats.agi = 7;
    attacker_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    Unit_setStats(&defender, defender_stats);
    Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    Game_Combat_Resolves(&firesaga, &attacker, &defender);
    lok(attacker.current_hp == 15);
    lok(defender.current_hp == 7);
    // printf("%d \n", attacker.current_hp);
    // printf("%d \n", defender.current_hp);

    Unit_getsHealed(&attacker, 100);
    Unit_getsHealed(&defender, 100);

    attacker_stats.agi = 7;
    Unit_setStats(&attacker, attacker_stats);
    Game_Combat_Forecast(&firesaga, &attacker, &attacker_pos, &defender, &defender_pos);
    Game_Combat_Outcome(&firesaga, &attacker, &defender);
    firesaga.outcome_combat[0].crit = true;
    Game_Combat_Resolves(&firesaga, &attacker, &defender);
    lok(attacker.current_hp == 15);
    lok(defender.current_hp == 7);
    // printf("%d \n", attacker.current_hp);
    // printf("%d \n", defender.current_hp);
}

void test_game() {
    SDL_Log("test_game");
    test_combat();
}