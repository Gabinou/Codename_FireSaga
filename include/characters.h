#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
using namespace std;

std::vector<std::string> unit_stats = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con"};
std::vector<std::string> weapon_stats = {"dmg", "hit", "crit", "weight"};
std::vector<std::string> wpn_types = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
std::vector<std::string> weapons = {"Iron Sword", "Iron Bow"};
std::vector<std::string> statuses = {"healthy", "sleep", "poison", "stone", "berserk"};
std::vector<std::string> unit_classes = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;

class character {
    public:
        std::vector<int> stats, stat_bonus, wpn_lvls, equipment, position;
        bool mounted, flying;
        int equipped, id, unit_class;
        std::vector<std::string> weapons;
        
        character(int test=0) {
            std::vector<int> stats = std::vector<int>(unit_stats.size());
            std::vector<int> stat_bonus = std::vector<int>(unit_stats.size());
            std::vector<int> wpn_exp = std::vector<int>(wpn_types.size());
            std::vector<int> equipment = std::vector<int>(equipment_slots);
            std::vector<int> position = std::vector<int>(3);
            std::string name;

        };        
        int combat_damage(void){
            int damage = stats[1] + weapons[equipped];
            // cout << stats[0] + weapons[equipped] << "\n";
            return damage;
        }
        // int combat_avoid();
        // int combat_crit(); 
        
        // if ever write .dll or .so, use char in dll/so exposed functions.
        // char name = 
        /* Stats order:
            
        */    
};

class weapon {
    public:
        std::vector<int> stats = std::vector<int>(equipment_slots);
        std::vector<int> stat_bonus = std::vector<int>(unit_stats.size());
        std::string name;
        std::vector<std::string> effective;
        int id;
};

class map {
    public:
        std::vector<int> map_size = {256, 256};
        // std::vector<int> tiles(map_size[0], map_size[1]);
        

};

main() {
    std::cout << "TESTING THIS BITCH\n";
    character Marth;
    Marth.stats = {18, 2};
    for (std::string x : unit_stats)
        std::cout << x << " ";    
    std::cout << "Marth's " << unit_stats[0] << ":" << Marth.stats[0] << "\n";
    std::cout << "Marth's damage:" << Marth.combat_damage() << "\n";
    std::cout << "TA MERE\n";
    // printf("Marth's HUGE FACTOR:  (%d)\n", sizeof(Marth));
}