#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "classes.h"
using namespace std;

char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight", "cost", "wpn_exp", "uses"};
char wpn_types[][14]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};
unsigned char wpn_exp_lvls[][14] = {25, 60, 100, 150};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
char equipment_slots = 7;
char *all_wpns;
char *all_units;

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
    wpn_exp = std::vector<int>(wpn_types.size(), 0);
    equipment = std::vector<int>(1);
    position = std::vector<int>(1);
    current_hp = stats[0];
}
int character::combat_damage(){
    int damage = stats[1] + stat_bonus[1];
    // cout << weapons[equipped] << "\n";
    return damage;
}
weapon::weapon() {
    stats = std::vector<int>(wpn_stats.size(), 0);
    stat_bonus = std::vector<int>(unit_stats.size(), 0);
    effective = std::vector<std::string>(unit_classes.size(), "");
    name = "Stick";
    wpn_type = 0;
    wpn_count++;
    id = wpn_count;
}

weapon::weapon(int in_wpn_type,
               std::string in_name,
               std::vector<int> in_stats,
               std::vector<int> in_stat_bonus,
               std::vector<std::string> in_effective) {
    wpn_type = in_wpn_type;
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

weapon get_wpn_from_id(int wpn_id, std::vector<weapon *> all_wpns){
    weapon found_wpn;
    for (std::vector<weapon *>::size_type i = 0; i != all_wpns.size(); i++) {      
        weapon current_wpn = *all_wpns[i];
        if (wpn_id == current_wpn.id)  {
            found_wpn = current_wpn;
        } 
    }   
    return(found_wpn);
};

character get_character_from_id(int character_id, std::vector<character *> all_characters){
    character found_wpn;
    for (std::vector<character *>::size_type i = 0; i != all_characters.size(); i++) {      
        character current_wpn = *all_characters[i];
        if (character_id == current_wpn.id)  {
            found_wpn = current_wpn;
        } 
    }   
    return(found_wpn);
};

int weapon::wpn_count = 0;
int character::character_count = 0;
int generic::object_count = 0;
main() {
    std::cout << "TESTING THIS BITCH\n";
    std::cout << "Initializaing a character\n";
    weapon Rapier(0, "Rapier", {5, 90, 10, 7, 30, 2, 600, 2}, std::vector<int>(unit_stats.size(), 0), {"Knight", "Cavalier"});
    weapon Bronze_swd(0, "Bronze Sword", {3, 80, 0, 5, 45, 0, 450, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    weapon Iron_swd(0, "Iron Sword", {5, 80, 0, 7, 45, 0, 450, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    weapon Iron_lnc(1, "Iron Lance", {6, 80, 0, 8, 40, 0, 500, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    weapon Steel_swd(0, "Steel Sword", {8, 70, 0, 9, 35, 1, 500, 1}, std::vector<int>(unit_stats.size(), 0), {""});
    weapon Damascus_swd(0, "Damascus Sword", {12, 65, 0, 8, 25, 3, 1000, 1}, std::vector<int>(unit_stats.size(), 0), {""});

}
