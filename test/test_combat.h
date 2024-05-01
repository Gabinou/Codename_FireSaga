#include "nourstest.h"
#include "platform.h"
#include "combat.h"
#include "game/game.h"
#include "game/combat.h"
#include "game/unit.h"
#include "unit/unit.h"
#include "RNG.h"

void test_combat_stats() {
    SDL_Log("test_combat_stats");
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct Unit *attacker = (struct Unit *)calloc(1, sizeof(struct Unit));
    * attacker = Unit_default;
    struct Unit *defender = (struct Unit *)calloc(1, sizeof(struct Unit));
    * defender = Unit_default;
    Unit_InitWweapons(attacker, weapons_dtab);
    Unit_InitWweapons(defender, weapons_dtab);

    struct Damage dft_damage;
    struct Damage agg_damage;
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

    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    b32 attacker_equip_hand = UNIT_HAND_WEAK;
    b32 defender_equip_hand = UNIT_HAND_WEAK;
    Unit_Item_Drop(defender,           defender_equip_hand);
    Unit_Item_Takeat(defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(attacker,           attacker_equip_hand);
    Unit_Item_Takeat(attacker, in_wpn, attacker_equip_hand);
    defender->equipped[defender_equip_hand] = true;
    attacker->equipped[attacker_equip_hand] = true;
    nourstest_true(Unit_canAttack(defender));
    nourstest_true(Unit_canAttack(attacker));
    Unit_setStats(defender, defender_stats);
    Unit_setStats(attacker, attacker_stats);

    // SOTA_Log_Debug("%d %d %d %d", attacker->current_hp, attacker_stats.hp, defender->current_hp, defender_stats.hp);
    nourstest_true(attacker->current_hp == attacker_stats.hp);
    nourstest_true(defender->current_hp == defender_stats.hp);
    Unit_effectiveStats(attacker);
    Unit_effectiveStats(defender);

    Unit_computedStats(attacker, distance);
    Unit_computedStats(defender, distance);

    agg_stats   = Unit_computedStats(attacker, distance);
    dft_stats   = Unit_computedStats(defender, distance);
    forecast    = Compute_Combat_Forecast(attacker, defender,
                                          (struct Point *)&attacker_pos,
                                          (struct Point *)&defender_pos);
    temp_stats = forecast.stats;
    nourstest_true((temp_stats.agg_rates.hit    == Equation_Combat_Hit(agg_stats.hit,
                    dft_stats.dodge)));
    nourstest_true((temp_stats.agg_rates.hit > 0));
    nourstest_true((temp_stats.agg_rates.crit   == Equation_Combat_Crit(agg_stats.crit,
                    dft_stats.favor)));
    nourstest_true((temp_stats.dft_rates.hit    == Equation_Combat_Hit(dft_stats.hit,
                    agg_stats.dodge)));
    nourstest_true((temp_stats.dft_rates.hit > 0));
    nourstest_true((temp_stats.dft_rates.crit   == Equation_Combat_Crit(dft_stats.crit,
                    agg_stats.favor)));

    agg_damage = Compute_Combat_Damage(attacker, defender);

    struct Weapon *fleuret = DTAB_GET(weapons_dtab, defender->_equipment[defender_equip_hand].id);
    nourstest_true(fleuret->stats.attack[DAMAGE_TYPE_PHYSICAL] > 0);
    dft_damage = Compute_Combat_Damage(defender, attacker);

    nourstest_true((temp_stats.agg_damage.dmg[DMG_TYPE_PHYSICAL] ==
                    agg_damage.dmg[DMG_TYPE_PHYSICAL]));
    nourstest_true((temp_stats.agg_damage.dmg[DMG_TYPE_MAGICAL] ==
                    agg_damage.dmg[DMG_TYPE_MAGICAL]));
    nourstest_true((temp_stats.agg_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    nourstest_true((temp_stats.agg_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] == dft_damage.dmg[DMG_TYPE_PHYSICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] ==
                    dft_damage.dmg[DMG_TYPE_MAGICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    nourstest_true((temp_stats.agg_stats.attack[0] == agg_stats.attack[0]));
    nourstest_true((temp_stats.agg_stats.attack[0] > 0));
    nourstest_true((temp_stats.agg_stats.attack[1] == agg_stats.attack[1]));
    nourstest_true((temp_stats.agg_stats.attack[1] == 0));
    nourstest_true((temp_stats.agg_stats.protection[0] == agg_stats.protection[0]));
    nourstest_true((temp_stats.agg_stats.protection[0] > 0));
    nourstest_true((temp_stats.agg_stats.protection[1] == agg_stats.protection[1]));
    nourstest_true((temp_stats.agg_stats.protection[1] > 0));
    nourstest_true((temp_stats.agg_stats.dodge == agg_stats.dodge));
    nourstest_true((temp_stats.agg_stats.favor == agg_stats.favor));
    nourstest_true((temp_stats.agg_stats.agony == agg_stats.agony));
    nourstest_true((temp_stats.agg_stats.speed == agg_stats.speed));
    nourstest_true((temp_stats.agg_stats.speed == agg_stats.speed));
    nourstest_true((temp_stats.agg_stats.move == attacker_stats.move));

    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] ==
                    dft_damage.dmg[DMG_TYPE_PHYSICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] ==
                    dft_damage.dmg[DMG_TYPE_MAGICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] ==
                    dft_damage.dmg[DMG_TYPE_PHYSICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] ==
                    dft_damage.dmg[DMG_TYPE_MAGICAL]));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_PHYSICAL] > 0));
    nourstest_true((temp_stats.dft_damage.dmg[DMG_TYPE_MAGICAL] == 0));
    nourstest_true((temp_stats.dft_stats.attack[0] == dft_stats.attack[0]));
    nourstest_true((temp_stats.dft_stats.attack[0] > 0));
    nourstest_true((temp_stats.dft_stats.attack[1] == dft_stats.attack[1]));
    nourstest_true((temp_stats.dft_stats.attack[1] == 0));
    nourstest_true((temp_stats.dft_stats.protection[0] == dft_stats.protection[0]));
    nourstest_true((temp_stats.dft_stats.protection[0] > 0));
    nourstest_true((temp_stats.dft_stats.protection[1] == dft_stats.protection[1]));
    nourstest_true((temp_stats.dft_stats.protection[1] > 0));
    nourstest_true((temp_stats.dft_stats.dodge == dft_stats.dodge));
    nourstest_true((temp_stats.dft_stats.favor == dft_stats.favor));
    nourstest_true((temp_stats.dft_stats.agony == dft_stats.agony));
    nourstest_true((temp_stats.dft_stats.speed == dft_stats.speed));
    nourstest_true((temp_stats.dft_stats.speed == dft_stats.speed));
    nourstest_true((temp_stats.dft_stats.move  == defender_stats.move));

    /* --- FREE --- */
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
    Unit_InitWweapons(&attacker, weapons_dtab);
    Unit_InitWweapons(&defender, weapons_dtab);
    struct Damage dft_damage;
    struct Damage agg_damage;
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
    b32 attacker_equip_hand = UNIT_HAND_WEAK;
    b32 defender_equip_hand = UNIT_HAND_WEAK;
    struct Combat_Forecast forecast;
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    defender.equipped[defender_equip_hand] = true;
    attacker.equipped[attacker_equip_hand] = true;
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);

    Unit_effectiveStats(&attacker);
    Unit_effectiveStats(&defender);

    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);

    forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
                                       (struct Point *)&defender_pos);
    temp_stats = forecast.stats;

    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.aggressor_phases == 0);
    nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));

    attacker.current_hp = 1;
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true((attacker.current_hp == 1));
    forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
                                       (struct Point *)&defender_pos);
    temp_stats = forecast.stats;
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == true);
    nourstest_true(temp_stats.dft_damage.dmg[DMG_TYPE_TOTAL] > 0);
    nourstest_true(temp_stats.dft_rates.hit > 0);
    nourstest_true(temp_stats.dft_rates.hit == (defender.computed_stats.hit -
                                                attacker.computed_stats.dodge));
    nourstest_true(temp_flow.aggressor_phases == 0);
    nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));

    attacker.current_hp = 17;
    defender.current_hp = 1;
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true((attacker.current_hp == 17));
    nourstest_true((defender.current_hp == 1));
    forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
                                       (struct Point *)&defender_pos);
    temp_stats = forecast.stats;
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.aggressor_phases == 0);
    nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));

    defender.current_hp = 17;
    attacker.current_hp = 1;
    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 3;
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
                                       (struct Point *)&defender_pos);
    temp_stats = forecast.stats;
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.aggressor_phases == 0);
    nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));

    attacker.current_hp = 1;
    defender.current_hp = 1;
    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true((attacker.current_hp == 1));
    forecast = Compute_Combat_Forecast(&attacker, &defender, (struct Point *)&attacker_pos,
                                       (struct Point *)&defender_pos);
    temp_stats = forecast.stats;
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    temp_death = Compute_Combat_Death(&attacker, &defender, temp_stats, temp_flow);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.aggressor_phases == 0);
    nourstest_true(temp_flow.defendant_phases == 0);
    // nourstest_true((temp_death.aggressor_certain == false));
    // nourstest_true((temp_death.defendant_certain == false));
    // nourstest_true((temp_death.aggressor_possible == false));
    // nourstest_true((temp_death.defendant_possible == false));
}

void test_combat_flow() {

    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct Combat_Flow temp_flow;
    struct Unit attacker = Unit_default;
    struct Unit defender = Unit_default;
    Unit_InitWweapons(&attacker, weapons_dtab);
    Unit_InitWweapons(&defender, weapons_dtab);
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
    b32 attacker_equip_hand = UNIT_HAND_WEAK;
    b32 defender_equip_hand = UNIT_HAND_WEAK;
    Unit_Item_Drop(&defender,           defender_equip_hand);
    Unit_Item_Takeat(&defender, in_wpn, defender_equip_hand);
    Unit_Item_Drop(&attacker,           attacker_equip_hand);
    Unit_Item_Takeat(&attacker, in_wpn, attacker_equip_hand);
    defender.equipped[defender_equip_hand] = true;
    attacker.equipped[attacker_equip_hand] = true;
    nourstest_true(Unit_canAttack(&defender));
    nourstest_true(Unit_canAttack(&attacker));
    Unit_setStats(&defender, defender_stats);
    Unit_setStats(&attacker, attacker_stats);
    nourstest_true(attacker.current_hp == attacker_stats.hp);
    nourstest_true(defender.current_hp == defender_stats.hp);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
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

    Unit_computeHit(&attacker, distance);
    Unit_computeHit(&defender, distance);
    i32 attacker_hit;
    i32 defender_hit;
    struct Weapon attacker_weapon = Weapon_default;
    struct Weapon defender_weapon = Weapon_default;

    if (defender.equipped[defender_equip_hand]) {
        defender_weapon = *((struct Weapon *)DTAB_GET(weapons_dtab,
                                                      defender._equipment[defender_equip_hand].id));
    }
    if (attacker.equipped[attacker_equip_hand]) {
        attacker_weapon = *((struct Weapon *)DTAB_GET(weapons_dtab,
                                                      attacker._equipment[attacker_equip_hand].id));
    }

    attacker_hit = Equation_Unit_Hit(attacker_weapon.stats.hit, attacker.effective_stats.dex,
                                     attacker.effective_stats.luck, 0);
    nourstest_true(attacker.computed_stats.hit == attacker_hit);
    defender_hit = Equation_Unit_Hit(defender_weapon.stats.hit, defender.effective_stats.dex,
                                     defender.effective_stats.luck, 0);
    nourstest_true(defender.computed_stats.hit == defender_hit);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    i32 attacker_speed = Equation_Unit_Speed(attacker_weapon.stats.wgt,
                                             attacker.effective_stats.agi,
                                             attacker.effective_stats.con,
                                             attacker.effective_stats.str, 0);
    nourstest_true(Unit_computeSpeed(&attacker, distance) == attacker_speed);
    nourstest_true(attacker.computed_stats.speed == attacker_speed);
    nourstest_true(attacker.computed_stats.speed == 7);
    i32 defender_speed = Equation_Unit_Speed(defender_weapon.stats.wgt,
                                             defender.effective_stats.agi,
                                             defender.effective_stats.con,
                                             defender.effective_stats.str, 0);
    nourstest_true(Unit_computeSpeed(&defender, distance) == defender_speed);
    nourstest_true(defender.computed_stats.speed == defender_speed);
    nourstest_true(defender.computed_stats.speed == 7);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == true);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 3;

    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    attacker_speed = Equation_Unit_Speed(attacker_weapon.stats.wgt,
                                         attacker.effective_stats.agi,
                                         attacker.effective_stats.con,
                                         attacker.effective_stats.str, 0);
    nourstest_true(Unit_computeSpeed(&attacker, distance) == attacker_speed);
    nourstest_true(attacker.computed_stats.speed == attacker_speed);
    defender_speed = Equation_Unit_Speed(defender_weapon.stats.wgt,
                                         defender.effective_stats.agi,
                                         defender.effective_stats.con,
                                         defender.effective_stats.str, 0);
    nourstest_true(Unit_computeSpeed(&defender, distance) == defender_speed);
    nourstest_true(defender.computed_stats.speed == defender_speed);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == false);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 0);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 8;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true(attacker.computed_stats.speed == 8);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x      = 1;
    attacker_pos.y      = 2;
    defender_pos.x      = 2;
    defender_pos.y      = 2;
    attacker_stats.agi  = 9;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true(attacker.computed_stats.speed == 9);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 10;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true(attacker.computed_stats.speed == 10);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 11;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true(attacker.computed_stats.speed == 11);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(temp_flow.defendant_phases == 1);

    attacker_pos.x = 1;
    attacker_pos.y = 2;
    defender_pos.x = 2;
    defender_pos.y = 2;
    attacker_stats.agi = 12;
    Unit_setStats(&attacker, attacker_stats);
    Unit_computedStats(&attacker, distance);
    Unit_computedStats(&defender, distance);
    nourstest_true(attacker.computed_stats.speed == 12);
    temp_flow = Compute_Combat_Flow(&attacker, &defender, (struct Point *)&attacker_pos,
                                    (struct Point *)&defender_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&defender,
                   &attacker, (struct Point *)&defender_pos, (struct Point *)&attacker_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 2);
    nourstest_true(Combat_canDouble(&attacker, &defender));
    nourstest_true(temp_flow.defendant_phases == 1);

    temp_flow = Compute_Combat_Flow(&defender, &attacker, (struct Point *)&defender_pos,
                                    (struct Point *)&attacker_pos);
    nourstest_true(temp_flow.defendant_retaliates == Combat_canAttack_Equipped(&attacker,
                   &defender, (struct Point *)&attacker_pos, (struct Point *)&defender_pos));
    nourstest_true(temp_flow.defendant_retaliates == 1);
    nourstest_true(temp_flow.aggressor_phases == 1);
    nourstest_true(Combat_canDouble(&attacker, &defender));
    nourstest_true(temp_flow.defendant_phases == 2);

    /* --- FREE --- */
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

    nourstest_true(army_isPC[Silou.army]);
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

    struct Damage damage = {0};
    damage.dmg[DMG_TYPE_PHYSICAL] = 8;
    damage.dmg_crit[DMG_TYPE_PHYSICAL] = 24;

    URN_debug = 11; /* HIT/NOCRIT */
    SDL_assert(URN_debug == RNG_URN());
    SDL_assert(RNG_double_roll(URN_debug, URN_debug, hit_rate) == true);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit  == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 1);
    nourstest_true(Silou.hit_sequence.eff_rate == hit_rate);
    nourstest_true(Silou.crit_sequence.hit == false);
    nourstest_true(Silou.crit_sequence.len == 1);
    nourstest_true(Silou.crit_sequence.eff_rate == crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 1);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit   == true);
    nourstest_true(attack.crit  == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 2);
    nourstest_true(Silou.hit_sequence.eff_rate == hit_rate);
    nourstest_true(Silou.crit_sequence.hit == false);
    nourstest_true(Silou.crit_sequence.len == 2);
    nourstest_true(Silou.crit_sequence.eff_rate == crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 2);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit   == true);
    nourstest_true(attack.crit  == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 3);
    nourstest_true(Silou.hit_sequence.eff_rate == hit_rate);
    nourstest_true(Silou.crit_sequence.hit == false);
    nourstest_true(Silou.crit_sequence.len == 3);
    nourstest_true(Silou.crit_sequence.eff_rate == SB_RATE_RISE(crit_rate,
                                                                Silou.crit_sequence.len - 1));
    nourstest_true(DARR_NUM(darr_attacks) == 3);

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    nourstest_true(DARR_NUM(darr_attacks) == 4);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit  == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 4);
    nourstest_true(Silou.hit_sequence.eff_rate == hit_rate);
    nourstest_true(Silou.crit_sequence.hit == false);
    nourstest_true(Silou.crit_sequence.len == 4);
    nourstest_true(Silou.crit_sequence.eff_rate == SB_RATE_RISE(crit_rate,
                                                                Silou.crit_sequence.len - 1));

    URN_debug = 49; /* HIT/NOCRIT*/
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 5);
    nourstest_true(Silou.hit_sequence.eff_rate == hit_rate);
    nourstest_true(Silou.crit_sequence.hit == false);
    nourstest_true(Silou.crit_sequence.len == 5);
    nourstest_true(Silou.crit_sequence.eff_rate == SB_RATE_RISE(crit_rate,
                                                                Silou.crit_sequence.len - 1));

    URN_debug = 61; /* NOHIT/NOCRIT*/
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == false);
    nourstest_true(Silou.hit_sequence.len == 1);
    nourstest_true(Silou.hit_sequence.eff_rate == SB_RATE_RISE(hit_rate, Silou.hit_sequence.len));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == false);
    nourstest_true(Silou.hit_sequence.len == 2);
    nourstest_true(Silou.hit_sequence.eff_rate == SB_RATE_RISE(hit_rate, Silou.hit_sequence.len - 1));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == false);
    nourstest_true(Silou.hit_sequence.len == 3);
    nourstest_true(Silou.hit_sequence.eff_rate == SB_RATE_RISE(hit_rate, Silou.hit_sequence.len - 1));

    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == false);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == false);
    nourstest_true(Silou.hit_sequence.len == 4);
    nourstest_true(Silou.hit_sequence.eff_rate == SB_RATE_RISE(hit_rate, Silou.hit_sequence.len - 1));
    nourstest_true(Silou.hit_sequence.eff_rate < URN_debug);

    // Hit now cause of sequence breaker increase hit
    Compute_Combat_Attack(&phase, darr_attacks, damage, &Silou, hit_rate, crit_rate);
    attack = darr_attacks[DARR_NUM(darr_attacks) - 1];
    nourstest_true(attack.hit == true);
    nourstest_true(attack.crit == false);
    nourstest_true(Silou.hit_sequence.hit == true);
    nourstest_true(Silou.hit_sequence.len == 1);

    /* Free everything */
    DARR_FREE(darr_attacks);
    URN_debug = -1;
}

void test_combat() {
    SDL_Log("test_combat");
    test_combat_stats();
    // test_combat_death();
    test_combat_flow();
    test_combat_sequence();
}
