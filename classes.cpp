#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "classes.h"
using namespace std;

std::vector<std::string> unit_stats = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Mov"};
std::vector<std::string> wpn_stats = {"dmg", "hit", "crit", "weight"};
std::vector<std::string> wpn_types = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
std::vector<std::string> weapons = {"Iron Sword", "Iron Bow", "Rapier",};
std::vector<std::string> items = {"Vulnerary"};
std::vector<std::string> statuses = {"healthy", "sleep", "poison", "stone", "berserk"};
std::vector<std::string> unit_classes = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;

character::character(std::vector<int> in_stats,
                    std::vector<int> in_stat_bonus,
                    std::vector<int> in_wpn_exp,
                    std::vector<int> in_equipment,
                    std::vector<int> in_position) {
    stats = in_stats;
    stat_bonus = in_stat_bonus;
    wpn_exp = in_wpn_exp;
    equipment = in_equipment;
    position = in_position;
    current_hp = in_stats[0];
}

character::character() {
    stats = std::vector<int>(unit_stats.size(), 0);
    stat_bonus = std::vector<int>(unit_stats.size(), 0);
    wpn_exp = std::vector<int>(1);
    equipment = std::vector<int>();
    position = std::vector<int>(1);
    current_hp = stats[0];
}
int character::combat_damage(){
    int damage = stats[1];
    // cout << weapons[equipped] << "\n";
    return damage;
}
weapon::weapon() {
    stats = std::vector<int>(wpn_stats.size(), 0);
    stat_bonus = std::vector<int>(unit_stats.size(), 0);
    effective = std::vector<std::string>(unit_classes.size(), "");
    name = "Stick";
    wpn_count++;
    id = wpn_count;
}

weapon::weapon(std::string in_name,
               std::vector<int> in_stats,
               std::vector<int> in_stat_bonus,
               std::vector<std::string> in_effective) {
    stats = in_stats;
    stat_bonus = in_stat_bonus;
    effective = in_effective;
    name = in_name;
    wpn_count++;
    id = wpn_count;
}

generic::generic() {
    object_count++;
}

int weapon::wpn_count = 0;
int character::character_count = 0;
int generic::object_count = 0;
main() {

    std::cout << "TESTING THIS BITCH\n";
    std::cout << "Initializaing a character\n";
    weapon Rapier("Rapier", {5, 90, 10, 7}, std::vector<int>(unit_stats.size(), 0), std::vector<std::string>(unit_classes.size(), ""));
    weapon Iron_swd("Iron Sword", {5, 80, 0, 8}, std::vector<int>(unit_stats.size(), 0), std::vector<std::string>(unit_classes.size(), ""));
    weapon *rapier = &Rapier;
    weapon *iron_swd = &Iron_swd;
    character Marth;
    character *marth = &Marth;
    std::vector<character *> all_characters;
    std::vector<weapon *> all_wpns;
    all_wpns = {rapier, iron_swd};
    all_characters = {marth};
    Marth.name = "Marth";
    Marth.stats = {18,8,2,10,11,7,5,2,5,5};
    Marth.wpn_exp = std::vector<int>(wpn_types.size(), 0);
    Marth.equipment.push_back(Rapier.id);
            // std::vector<int> equipment = std::vector<int>(equipment_slots);
            // std::vector<int> position = std::vector<int>(3);
            // std::string name;
    for (std::vector<weapon *>::size_type i = 0; i != all_wpns.size(); i++) {      
        weapon current_wpn = *all_wpns[i] ;
        if (Marth.equipment[0] == current_wpn.id)  {
            std::cout << "Marth is carrying a " << current_wpn.name << std::endl;
          // std::cout << i << "\n";
        } 
    }
    std::cout << Marth.stats[0] << "\n";
    std::cout << Rapier.id << "\n";
    std::cout << Marth.stat_bonus[0] << "\n";
    
    
    std::cout << Marth.equipment[0] << "\n";
    std::cout << Marth.wpn_exp[0] << "\n";
    std::cout << Marth.current_hp << "\n";
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