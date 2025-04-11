
#include "nourstest.h"
#include "platform.h"
#include "combat.h"
#include "game/game.h"
#include "game/combat.h"
#include "game/unit.h"
#include "unit/unit.h"
#include "RNG.h"
#include "pathfinding.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "equations.h"

void test_combat_stats() {
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct Unit *attacker = (struct Unit *)SDL_calloc(1, sizeof(struct Unit));
    * attacker = Unit_default;
    struct Unit *defender = (struct Unit *)SDL_calloc(1, sizeof(struct Unit));
    * defender = Unit_default;
    Unit_Init(attacker);
    Unit_Init(defender);

    struct Combat_Damage dft_damage;
    struct Combat_Damage agg_damage;
    struct Combat_Stats temp_stats;
    struct Combat_Forecast forecast;
    struct Computed_Stats agg_stats;
    struct Computed_Stats dft_stats;
    // struct Unit defender = Unit_default;
    struct Point attacker_pos = {1, 1};
    struct Point defender_pos = {1, 2};
    int distance = Pathfinding_Manhattan(attacker_pos, defender_pos);
    //                                 hp,str,mag,agi,fth,dex,luck,def,res,con,move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,  7,  7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {18,  7,  3,  8,  8,  7,  8,  5,  6,  7, 6};

    Unit_setClassind(defender, UNIT_CLASS_FENCER);
    Unit_setClassind(attacker, UNIT_CLASS_FENCER);

    // struct Inventory_item in_wpn = Inventory_item_default;
    // in_wpn.id = ITEM_ID_FLEURET;
    // b32 attacker_equip_hand = Unit_Hand_Strong(&attacker);
    // b32 defender_equip_hand = Unit_Hand_Strong(&defender);
    // Unit_Item_Drop(defender,           defender_equip_hand);
    // Unit_Item_Takeat(defender, in_wpn, defender_equip_hand);
    // Unit_Item_Drop(attacker,           attacker_equip_hand);
    // Unit_Item_Takeat(attacker, in_wpn, attacker_equip_hand);
    // nourstest_true(Unit_canAttack(defender));
    // nourstest_true(Unit_canAttack(attacker));
    // Unit_setStats(defender, defender_stats);
    // Unit_setStats(attacker, attacker_stats);

    // // SOTA_Log_Debug("%d %d %d %d", attacker->counters.hp, attacker_stats.hp, defender->counters.hp, defender_stats.hp);
    // nourstest_true(attacker->counters.hp == attacker_stats.hp);
    // nourstest_true(defender->counters.hp == defender_stats.hp);
    // Unit_effectiveStats(attacker);
    // Unit_effectiveStats(defender);

    // Unit_computedStats(attacker, distance);
    // Unit_computedStats(defender, distance);

    // agg_stats   = Unit_computedStats(attacker, distance);
    // dft_stats   = Unit_computedStats(defender, distance);
    // forecast    = Compute_Combat_Forecast(attacker, defender,
    //                                       (struct Point *)&attacker_pos,
    //                                       (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;
    // nourstest_true((temp_stats.agg_rates.hit    == Equation_Combat_Hit(agg_stats.hit,
    //                 dft_stats.dodge)));
    // nourstest_true((temp_stats.agg_rates.hit > 0));
    // nourstest_true((temp_stats.agg_rates.crit   == Equation_Combat_Crit(agg_stats.crit,
    //                 dft_stats.favor)));
    // nourstest_true((temp_stats.dft_rates.hit    == Equation_Combat_Hit(dft_stats.hit,
    //                 agg_stats.dodge)));
    // nourstest_true((temp_stats.dft_rates.hit > 0));
    // nourstest_true((temp_stats.dft_rates.crit   == Equation_Combat_Crit(dft_stats.crit,
    //                 agg_stats.favor)));

    // agg_damage = Compute_Combat_Damage(attacker, defender);

    // struct Weapon *fleuret = DTAB_GET(weapons_dtab, defender->_equipment[defender_equip_hand].id);
    // nourstest_true(fleuret->stats.attack.physical > 0);
    // dft_damage = Compute_Combat_Damage(defender, attacker);

    // nourstest_true((temp_stats.agg_damage.dmg.physical ==
    //                 agg_damage.dmg.physical));
    // nourstest_true((temp_stats.agg_damage.dmg.magical ==
    //                 agg_damage.dmg.magical));
    // nourstest_true((temp_stats.agg_damage.dmg.physical > 0));
    // nourstest_true((temp_stats.agg_damage.dmg.magical == 0));
    // nourstest_true((temp_stats.dft_damage.dmg.physical == dft_damage.dmg.physical));
    // nourstest_true((temp_stats.dft_damage.dmg.magical ==
    //                 dft_damage.dmg.magical));
    // nourstest_true((temp_stats.dft_damage.dmg.physical > 0));
    // nourstest_true((temp_stats.dft_damage.dmg.magical == 0));
    // nourstest_true((temp_stats.agg_stats.attack[0] == agg_stats.attack[0]));
    // nourstest_true((temp_stats.agg_stats.attack[0] > 0));
    // nourstest_true((temp_stats.agg_stats.attack[1] == agg_stats.attack[1]));
    // nourstest_true((temp_stats.agg_stats.attack[1] == 0));
    // nourstest_true((temp_stats.agg_stats.protection[0] == agg_stats.protection[0]));
    // nourstest_true((temp_stats.agg_stats.protection[0] > 0));
    // nourstest_true((temp_stats.agg_stats.protection[1] == agg_stats.protection[1]));
    // nourstest_true((temp_stats.agg_stats.protection[1] > 0));
    // nourstest_true((temp_stats.agg_stats.dodge == agg_stats.dodge));
    // nourstest_true((temp_stats.agg_stats.favor == agg_stats.favor));
    // nourstest_true((temp_stats.agg_stats.agony == agg_stats.agony));
    // nourstest_true((temp_stats.agg_stats.speed == agg_stats.speed));
    // nourstest_true((temp_stats.agg_stats.speed == agg_stats.speed));
    // nourstest_true((temp_stats.agg_stats.move == attacker_stats.move));

    // nourstest_true((temp_stats.dft_damage.dmg.physical ==
    //                 dft_damage.dmg.physical));
    // nourstest_true((temp_stats.dft_damage.dmg.magical ==
    //                 dft_damage.dmg.magical));
    // nourstest_true((temp_stats.dft_damage.dmg.physical > 0));
    // nourstest_true((temp_stats.dft_damage.dmg.magical == 0));
    // nourstest_true((temp_stats.dft_damage.dmg.physical ==
    //                 dft_damage.dmg.physical));
    // nourstest_true((temp_stats.dft_damage.dmg.magical ==
    //                 dft_damage.dmg.magical));
    // nourstest_true((temp_stats.dft_damage.dmg.physical > 0));
    // nourstest_true((temp_stats.dft_damage.dmg.magical == 0));
    // nourstest_true((temp_stats.dft_stats.attack[0] == dft_stats.attack[0]));
    // nourstest_true((temp_stats.dft_stats.attack[0] > 0));
    // nourstest_true((temp_stats.dft_stats.attack[1] == dft_stats.attack[1]));
    // nourstest_true((temp_stats.dft_stats.attack[1] == 0));
    // nourstest_true((temp_stats.dft_stats.protection[0] == dft_stats.protection[0]));
    // nourstest_true((temp_stats.dft_stats.protection[0] > 0));
    // nourstest_true((temp_stats.dft_stats.protection[1] == dft_stats.protection[1]));
    // nourstest_true((temp_stats.dft_stats.protection[1] > 0));
    // nourstest_true((temp_stats.dft_stats.dodge == dft_stats.dodge));
    // nourstest_true((temp_stats.dft_stats.favor == dft_stats.favor));
    // nourstest_true((temp_stats.dft_stats.agony == dft_stats.agony));
    // nourstest_true((temp_stats.dft_stats.speed == dft_stats.speed));
    // nourstest_true((temp_stats.dft_stats.speed == dft_stats.speed));
    // nourstest_true((temp_stats.dft_stats.move  == defender_stats.move));

    /* --- SDL_free --- */
    Unit_Free(attacker);
    SDL_free(attacker);
    Unit_Free(defender);
    SDL_free(defender);
    Game_Weapons_Free(&weapons_dtab);
}

void test_combat_death() {

    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    Unit_Init(&attacker);
    Unit_Init(&defender);
    struct Combat_Damage dft_damage;
    struct Combat_Damage agg_damage;
    struct Combat_Stats temp_stats;
    struct Combat_Flow temp_flow;
    struct Combat_Death temp_death;
    struct Computed_Stats agg_stats;
    struct Computed_Stats dft_stats;
    // struct Unit defender = Unit_default;
    struct Point attacker_pos = {1, 2};
    struct Point defender_pos = {2, 2};
    int distance = Pathfinding_Manhattan(attacker_pos, defender_pos);

    //                                  hp,str,mag,agi,dex,fth,luck,def,res,con,move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,   7,  1,  2,  6, 5};
    struct Unit_stats defender_stats = {17,  6,  2,  7,  7,   7,  1,  2,  6, 5};

    Unit_setClassind(&defender, UNIT_CLASS_FENCER);
    Unit_setClassind(&attacker, UNIT_CLASS_FENCER);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;

    b32 attacker_equip_hand = Unit_Hand_Strong(&attacker);
    b32 defender_equip_hand = Unit_Hand_Strong(&defender);
    struct Combat_Forecast forecast;
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    // Unit_Equip(defender, defender_equip_hand); = true;
    // attacker.equipped[attacker_equip_hand] = true;
    // nourstest_true(Unit_canAttack(&defender));
    // nourstest_true(Unit_canAttack(&attacker));
    // Unit_setStats(&defender, defender_stats);
    // Unit_setStats(&attacker, attacker_stats);

    // Unit_effectiveStats(&attacker);
    // Unit_effectiveStats(&defender);

    // Unit_computedStats(&attacker, distance);
    // Unit_computedStats(&defender, distance);

    // forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                    (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;

    // temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                 (struct Point *)&defender_pos);
    // temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    // nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    // nourstest_true(temp_flow.aggressor_phases == 0);
    // nourstest_true(temp_flow.defendant_phases == 0);
    // // nourstest_true((temp_death.aggressor_certain == false));
    // // nourstest_true((temp_death.defendant_certain == false));
    // // nourstest_true((temp_death.aggressor_possible == false));
    // // nourstest_true((temp_death.defendant_possible == false));

    // attacker.counters.hp = 1;
    // Unit_computedStats(&attacker, distance);
    // Unit_computedStats(&defender, distance);
    // nourstest_true((attacker.counters.hp == 1));
    // forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                    (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;
    // temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                 (struct Point *)&defender_pos);
    // temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    // nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
    //                &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    // nourstest_true(temp_flow.defendant_retaliates == true);
    // nourstest_true(temp_stats.dft_damage.dmg[DMG_TOTAL] > 0);
    // nourstest_true(temp_stats.dft_rates.hit > 0);
    // nourstest_true(temp_stats.dft_rates.hit == (defender.computed_stats.hit -
    //                                             attacker.computed_stats.dodge));
    // nourstest_true(temp_flow.aggressor_phases == 0);
    // nourstest_true(temp_flow.defendant_phases == 0);
    // // nourstest_true((temp_death.aggressor_certain == false));
    // // nourstest_true((temp_death.defendant_certain == false));
    // // nourstest_true((temp_death.aggressor_possible == false));
    // // nourstest_true((temp_death.defendant_possible == false));

    // attacker.counters.hp = 17;
    // defender.counters.hp = 1;
    // Unit_computedStats(&attacker, distance);
    // Unit_computedStats(&defender, distance);
    // nourstest_true((attacker.counters.hp == 17));
    // nourstest_true((defender.counters.hp == 1));
    // forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                    (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;
    // temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                 (struct Point *)&defender_pos);
    // temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    // nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
    //                &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    // nourstest_true(temp_flow.aggressor_phases == 0);
    // nourstest_true(temp_flow.defendant_phases == 0);
    // // nourstest_true((temp_death.aggressor_certain == false));
    // // nourstest_true((temp_death.defendant_certain == false));
    // // nourstest_true((temp_death.aggressor_possible == false));
    // // nourstest_true((temp_death.defendant_possible == false));

    // defender.counters.hp = 17;
    // attacker.counters.hp = 1;
    // attacker_pos.x = 1;
    // attacker_pos.y = 2;
    // defender_pos.x = 2;
    // defender_pos.y = 3;
    // Unit_computedStats(&attacker, distance);
    // Unit_computedStats(&defender, distance);
    // forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                    (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;
    // temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                 (struct Point *)&defender_pos);
    // temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    // nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
    //                &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    // nourstest_true(temp_flow.aggressor_phases == 0);
    // nourstest_true(temp_flow.defendant_phases == 0);
    // // nourstest_true((temp_death.aggressor_certain == false));
    // // nourstest_true((temp_death.defendant_certain == false));
    // // nourstest_true((temp_death.aggressor_possible == false));
    // // nourstest_true((temp_death.defendant_possible == false));

    // attacker.counters.hp = 1;
    // defender.counters.hp = 1;
    // attacker_pos.x = 1;
    // attacker_pos.y = 2;
    // defender_pos.x = 2;
    // defender_pos.y = 2;
    // Unit_computedStats(&attacker, distance);
    // Unit_computedStats(&defender, distance);
    // nourstest_true((attacker.counters.hp == 1));
    // forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                    (struct Point *)&defender_pos);
    // temp_stats = forecast.stats;
    // temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
    //                                 (struct Point *)&defender_pos);
    // temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    // nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
    //                &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    // nourstest_true(temp_flow.aggressor_phases == 0);
    // nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));
}

void test_combat_flow() {

    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct dtab *items_dtab = DTAB_INIT(items_dtab, struct Item);
    struct Combat_Flow temp_flow;
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    Unit_Init(&attacker);
    Unit_Init(&defender);
    struct Point attacker_pos = {1, 2};
    struct Point defender_pos = {2, 2};
    int distance = Pathfinding_Manhattan(attacker_pos, defender_pos);
    //hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats attacker_stats = {17,  6,  2,  7,  7,  7,   7,  4,  5,  6, 5};
    struct Unit_stats defender_stats = {17,  6,  2,  7,  7,  7,   7,  4,  5,  6, 5};

    Unit_setClassind(&defender, UNIT_CLASS_FENCER);
    Unit_setClassind(&attacker, UNIT_CLASS_FENCER);

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    b32 attacker_equip_hand = Unit_Hand_Strong(&attacker);
    b32 defender_equip_hand = Unit_Hand_Strong(&defender);
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    Unit_Equip(&attacker, attacker_equip_hand, attacker_equip_hand);
    Unit_Equip(&defender, defender_equip_hand, defender_equip_hand);
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.counters.hp == attacker_stats.hp);
    nourstest_true(defender.counters.hp == defender_stats.hp);
    struct Unit_stats ES_A = Unit_effectiveStats(&attacker);
    struct Unit_stats ES_D = Unit_effectiveStats(&defender);
    Computed_Stats cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    Computed_Stats cs_dft = Unit_computedStats(&defender, distance, ES_D);

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

    Unit_effectiveStats(&defender);
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

    i32 attacker_hit;
    i32 defender_hit;
    Unit_computeHit(&attacker, distance, &attacker_hit);
    Unit_computeHit(&defender, distance, &defender_hit);
    struct Weapon attacker_weapon = Weapon_default;
    struct Weapon defender_weapon = Weapon_default;

    if (Unit_isEquipped(&defender, defender_equip_hand)) {
        defender_weapon = *((struct Weapon *)DTAB_GET(weapons_dtab, Unit_Id_Equipment(&defender,
                                                      defender_equip_hand)));
    }
    if (Unit_isEquipped(&attacker, attacker_equip_hand)) {
        attacker_weapon = *((struct Weapon *)DTAB_GET(weapons_dtab, Unit_Id_Equipment(&defender,
                                                      attacker_equip_hand)));
    }

    nourstest_true(Equation_Unit_Hit(attacker_weapon.stats.hit, ES_A.dex, ES_A.luck,
                                     0) == attacker_hit);
    nourstest_true(Equation_Unit_Hit(defender_weapon.stats.hit, ES_D.dex, ES_D.luck,
                                     0) == defender_hit);
    ES_A = Unit_effectiveStats(&attacker);
    ES_D = Unit_effectiveStats(&defender);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);

    i32 attacker_speed;
    Unit_computeSpeed(&attacker, distance, &attacker_speed);
    nourstest_true(Equation_Unit_Speed(attacker_weapon.stats.wgt,
                                       ES_A.agi, ES_A.con, ES_A.str,
                                       0) == attacker_speed);
    nourstest_true(attacker_speed == 7);
    i32 defender_speed;
    Unit_computeSpeed(&defender, distance, &defender_speed);
    nourstest_true(Equation_Unit_Speed(defender_weapon.stats.wgt,
                                       ES_D.agi, ES_D.con, ES_D.str, 0) == defender_speed);
    nourstest_true(defender_speed == 7);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == true);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 3;

    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    Unit_computeSpeed(&defender, distance, &attacker_speed);
    nourstest_true(Equation_Unit_Speed(attacker_weapon.stats.wgt,
                                       ES_A.agi,
                                       ES_A.con,
                                       ES_A.str, 0) == attacker_speed);
    Unit_computeSpeed(&defender, distance, &defender_speed);
    nourstest_true(Equation_Unit_Speed(defender_weapon.stats.wgt,
                                       ES_D.agi,
                                       ES_D.con,
                                       ES_D.str, 0) == defender_speed);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == false);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 8;
    Unit_setStats(&attacker, attacker_stats);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(cs_agg.speed == 8);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x      = 1;
    attacker_pos.y      = 2;
    defender_pos.x      = 2;
    defender_pos.y      = 2;
    attacker_stats.agi  = 9;
    Unit_setStats(&attacker, attacker_stats);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(cs_agg.speed == 9);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 10;
    Unit_setStats(&attacker, attacker_stats);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(cs_agg.speed == 10);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 11;
    Unit_setStats(&attacker, attacker_stats);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(cs_agg.speed == 11);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,

                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    cs_agg = Unit_computedStats(&attacker, distance, ES_A);
    cs_dft = Unit_computedStats(&defender, distance, ES_D);
    nourstest_true(cs_agg.speed == 12);
    temp_flow = Compute_Combat_Flow(&attacker, &defender,
                                    cs_agg, cs_dft,
                                    (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 2);
    nourstest_true(Combat_canDouble(cs_agg, cs_dft));
    nourstest_true(temp_flow.defendant_phases == 1);

    temp_flow = Compute_Combat_Flow(&defender, &attacker,
                                    cs_agg, cs_dft,
                                    (struct Point *)&defender_pos,
                                    (struct Point *)&attacker_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&attacker,
                   (struct Point *)&attacker_pos, (struct Point *)&defender_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 2);
    nourstest_true(Combat_canDouble(cs_agg, cs_dft));
    nourstest_true(temp_flow.defendant_phases == 1);

    /* --- SDL_free --- */
    Unit_Free(&attacker);
    Unit_Free(&defender);
    Game_Weapons_Free(&weapons_dtab);
}

#ifndef RNG_SEQUENCE_BREAKER_HIT
    #define RNG_SEQUENCE_BREAKER_HIT
#endif /* RNG_SEQUENCE_BREAKER_HIT */
#ifndef RNG_SEQUENCE_BREAKER_MISS
    #define RNG_SEQUENCE_BREAKER_MISS
#endif /* RNG_SEQUENCE_BREAKER_MISS */
#ifndef RNG_SEQUENCE_BREAKER_CRIT
    #define RNG_SEQUENCE_BREAKER_CRIT
#endif /* RNG_SEQUENCE_BREAKER_CRIT */
#ifndef RNG_SEQUENCE_BREAKER_NOCRIT
    #define RNG_SEQUENCE_BREAKER_NOCRIT
#endif /* RNG_SEQUENCE_BREAKER_NOCRIT */

void test_combat_sequence() {

    struct Unit Silou = Unit_default;

    nourstest_true(army_isPC[Unit_Army(&Silou)]);
    struct Combat_Attack *darr_attacks;
    struct Combat_Attack temp_attack = {0};

    darr_attacks = DARR_INIT(darr_attacks, struct Combat_Attack, 20);

    struct Combat_Phase phase = {0};
    phase.skill_multiplier = 1;
    phase.attacker = SOTA_AGGRESSOR;

    struct Combat_Attack attack = {0};
    attack.hit = true;
    i32 hit_rate     = 50;
    i32 crit_rate    = 10;

    struct Combat_Damage damage = {0};
    damage.dmg.physical = 8;
    damage.dmg_crit.physical = 24;

    URN_debug = 11; /* HIT/NOCRIT */
    SDL_assert(URN_debug == RNG_URN());
    SDL_assert(RNG_double_roll(URN_debug, URN_debug, hit_rate) == true);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit  == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 1);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == hit_rate);
    nourstest_true(Silou.rng_sequence.crit.hit == false);
    nourstest_true(Silou.rng_sequence.crit.len == 1);
    nourstest_true(Silou.rng_sequence.crit.eff_rate == crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 1);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit   == true);
    nourstest_true(attack.crit  == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 2);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == hit_rate);
    nourstest_true(Silou.rng_sequence.crit.hit == false);
    nourstest_true(Silou.rng_sequence.crit.len == 2);
    nourstest_true(Silou.rng_sequence.crit.eff_rate == crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 2);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit   == true);
    nourstest_true(attack.crit  == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 3);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == hit_rate);
    nourstest_true(Silou.rng_sequence.crit.hit == false);
    nourstest_true(Silou.rng_sequence.crit.len == 3);
    nourstest_true(Silou.rng_sequence.crit.eff_rate == SB_RATE_RISE(crit_rate,
                   Silou.rng_sequence.crit.len - 1));
    nourstest_true(DARR_NUM(darr_attacks) == 3);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 4);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit  == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 4);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == hit_rate);
    nourstest_true(Silou.rng_sequence.crit.hit == false);
    nourstest_true(Silou.rng_sequence.crit.len == 4);
    nourstest_true(Silou.rng_sequence.crit.eff_rate == SB_RATE_RISE(crit_rate,
                   Silou.rng_sequence.crit.len - 1));

    URN_debug = 49; /* HIT/NOCRIT*/
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 5);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == hit_rate);
    nourstest_true(Silou.rng_sequence.crit.hit == false);
    nourstest_true(Silou.rng_sequence.crit.len == 5);
    nourstest_true(Silou.rng_sequence.crit.eff_rate == SB_RATE_RISE(crit_rate,
                   Silou.rng_sequence.crit.len - 1));

    URN_debug = 61; /* NOHIT/NOCRIT*/
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == false);
    nourstest_true(Silou.rng_sequence.hit.len == 1);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == SB_RATE_RISE(hit_rate,
                   Silou.rng_sequence.hit.len));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == false);
    nourstest_true(Silou.rng_sequence.hit.len == 2);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == SB_RATE_RISE(hit_rate,
                   Silou.rng_sequence.hit.len - 1));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == false);
    nourstest_true(Silou.rng_sequence.hit.len == 3);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == SB_RATE_RISE(hit_rate,
                   Silou.rng_sequence.hit.len - 1));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == false);
    nourstest_true(Silou.rng_sequence.hit.len == 4);
    nourstest_true(Silou.rng_sequence.hit.eff_rate == SB_RATE_RISE(hit_rate,
                   Silou.rng_sequence.hit.len - 1));
    nourstest_true(Silou.rng_sequence.hit.eff_rate < URN_debug);

    // Hit now cause of sequence breaker increase hit
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.rng_sequence.hit.hit == true);
    nourstest_true(Silou.rng_sequence.hit.len == 1);

    /* SDL_free everything */
    DARR_FREE(darr_attacks);
    URN_debug = -1;
}

void test_combat() {
    test_combat_stats();
    // test_combat_death();
    test_combat_flow();
    test_combat_sequence();
}
