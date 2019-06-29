#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "classes.h"
using namespace std;

std::vector<std::string> unit_stats = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con"};
std::vector<std::string> weapon_stats = {"dmg", "hit", "crit", "weight"};
std::vector<std::string> wpn_types = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
std::vector<std::string> weapons = {"Iron Sword", "Iron Bow"};
std::vector<std::string> statuses = {"healthy", "sleep", "poison", "stone", "berserk"};
std::vector<std::string> unit_classes = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;

character::character(std::vector<int> in_stats = std::vector<int>(1),
                    std::vector<int> in_stat_bonus = std::vector<int>(1),
                    std::vector<int> in_wpn_exp = std::vector<int>(1),
                    std::vector<int> in_equipment = std::vector<int>(1),
                    std::vector<int> in_position = std::vector<int>(1))
{
    stats = in_stats;
    stat_bonus = in_stat_bonus;
    wpn_exp = in_wpn_exp;
    equipment = in_equipment;
    position = in_position;
}
   
int character::combat_damage(){
    int damage = stats[1];
    // cout << weapons[equipped] << "\n";
    return damage;
}
main() {
    std::cout << "TESTING THIS BITCH\n";
    character Marth;
    Marth.stats = {18,1,1};
            std::vector<int> stat_bonus = std::vector<int>(unit_stats.size());
            std::vector<int> wpn_exp = std::vector<int>(wpn_types.size());
            std::vector<int> equipment = std::vector<int>(equipment_slots);
            std::vector<int> position = std::vector<int>(3);
            std::string name;
    // Marth.stats = {18, 2};
    std::cout << Marth.stats[0] << "\n";
    // std::cout << Marth.stat_bonus[0] << "\n";
    return 0;
}

// class weapon {
    // public:
        // std::vector<int> stats = std::vector<int>(equipment_slots);
        // std::vector<int> stat_bonus = std::vector<int>(unit_stats.size());
        // std::string name;
        // std::vector<std::string> effective;
        // int id;
// };

// class map {
    // public:
        // std::vector<int> map_size = {256, 256};
        // // std::vector<int> tiles(map_size[0], map_size[1]);
        

// };