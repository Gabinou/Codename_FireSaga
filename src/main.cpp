#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "shared.h"
using namespace std;

char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight", "cost", "wpn_exp", "uses"};
char wpn_types[][14]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};
unsigned char wpn_exp_lvls[][14] = {25, 60, 100, 150};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Armor Knight", "Cavalier", "Pegasus Knight", "Wyvern Knight", "Axe Rider", "Lance Rider", "Sword Rider", "Mercernary", "Myrmidon", "Soldier", "Archer", "Mage", "Priest", "Priestess", "King", "Paladin", "Sniper", "General", "Bishop", "Hero", "Pirate", "Fighter", "Thief", "Troubadour", "Cleric", "Monk", "Shaman", "Dragon", "Dancer"};
char equipment_slots = 7;
char *all_wpns;
char *all_units;
unsigned char id = 0;
    // struct unit Marth = {"Marth", "Prince", id++, 
                      // // HP  Str Mag Skl Spd Lck Def Res Con Mov
      // /*Stats*/          18,  8,  2,  9, 10,  7,  5,  2,  6,  5,
      // /*Stat bonuses*/    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      // /*Growths*/        40, 50,  5, 80, 60, 50, 30, 20,  0,  0,
      // /*Total exp*/       0,
                      // // swd lnc axe bow wnd tnd fir drk lgt stf
      // /*Weapon exp*/      0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
      // /*Position*/        1,  1,  1,
      // /*Equipment*/       0,  0,  0,  0,  0,  0,  0,
      // /*Weapons*/         0,  0,  0,  0,
      // /*Items*/           0,  0,  0,  0,
      // /*Equipped*/        1,
      // /*Skills*/          1,  2,  3,
      // /*Lovers*/         "Sheeda", "", "", "", "",
      // /*Love Points*/    25,  0,  0,  0,  0,
      // /*Love growths*/    2,  0,  0,  0,  0,
      // /*Mounted*/         0,
      // /*Armored*/         0,     
      // /*Flying*/          0,
      // /*Promoted*/        0
      // };   
unit::unit(std::string in_name, std::string in_classs, char in_id,
           std::vector<unsigned int> in_stats, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_equipment,
           std::vector<unsigned int> in_weapons, std::vector<unsigned int> in_items,
           std::vector<unsigned int> in_equipped, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_love_pts, std::vector<unsigned int> in_love_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           unsigned short in_exp, std::vector<std::string> in_lovers,
           bool in_mounted, bool in_flying, bool in_armored, bool in_promoted) {
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
    strncpy(name, in_name.c_str(), sizeof(in_name));
    strncpy(classs, in_classs.c_str(), sizeof(in_classs));
    for (int i = 0; i < in_stats.size(); i++) {
        stats[i] = (unsigned int) in_stats[i];
        growths[i] = (unsigned int) in_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
        stat_bonus[i] = 0;
        // For some reason, stats cannot be printed unless (int) Marth.stats[0]. + 0 also works.
    } 
    for (int i = 0; i < in_lovers.size(); i++) {
        strncpy(lovers[i], in_lovers[i].c_str(), sizeof(in_lovers));
        love_growths[i] = (unsigned int) in_love_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
    }    
    id = in_id;
}

unit_vec::unit_vec(std::string in_name, std::string in_classs, char in_id, std::vector<unsigned int> in_stats) {
    name = in_name;
    id = in_id;
    classs = in_classs;
    stats = in_stats;
}


// character::character(std::vector<int> in_stats,
                    // std::vector<int> in_stat_bonus,
                    // std::vector<int> in_wpn_exp,
                    // std::vector<int> in_equipment,
                    // std::vector<int> in_position) {
    // stats = in_stats;
    // stat_bonus = in_stat_bonus;
    // wpn_exp = in_wpn_exp;
    // equipment = in_equipment;
    // position = in_position;
    // current_hp = in_stats[0];
// }
// int character::combat_damage(){
    // int damage = stats[1] + stat_bonus[1];
    // // cout << weapons[equipped] << "\n";
    // return damage;
// }
main() {
    std::cout << "TESTING THIS BITCH\n";
    std::cout << "Initializaing a character\n";
    // weapon Rapier(0, "Rapier", {5, 90, 10, 7, 30, 2, 600, 2}, std::vector<int>(unit_stats.size(), 0), {"Knight", "Cavalier"});
    // weapon Bronze_swd(0, "Bronze Sword", {3, 80, 0, 5, 45, 0, 450, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    // weapon Iron_swd(0, "Iron Sword", {5, 80, 0, 7, 45, 0, 450, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    // weapon Iron_lnc(1, "Iron Lance", {6, 80, 0, 8, 40, 0, 500, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    // weapon Steel_swd(0, "Steel Sword", {8, 70, 0, 9, 35, 1, 500, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    // weapon Damascus_swd(0, "Damascus Sword", {12, 65, 0, 8, 25, 3, 1000, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    // unit Marth("Marth2", "Prince", id++, {18,  8,  2,  9, 10,  7,  5,  2,  6,  5});
    // unit_vec Marth_vec("Marth2", "Prince", id++, {18,  8,  2,  9, 10,  7,  5,  2,  6,  5});

    // std:string stringg = "Marth+Sheeda+Marth+Marth+Marth+Marth+Marth";
    // char charr[64] = "MarthMarth+Sheeda+Marth+Marth+Marth+Marth+Marth";
    // std::cout << "Marth2" << endl;
    // std::cout << Marth.name << endl;
    // std::cout << sizeof(stringg) << endl;
    // std::cout << sizeof(charr) << endl;
    // std::cout << Marth.name << endl;
    // // std::cout << (int) Marth.stats << endl;
    // std::cout << Marth.stats[0] + 0 << endl;
    // std::cout << Marth.stats[3] + 2 << endl;
    // std::cout << sizeof(Marth) << endl;
    // std::cout << sizeof(Marth_vec) << endl;
}
