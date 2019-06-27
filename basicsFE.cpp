#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
using namespace std;

std::vector<int> ids_list() {
    // Want to make an lgorithm that prints all numbers whose sum of digits is moduloable by 7
    int digit;
    int ids;
    std::vector<int> out;
    for (int id = 200; id<210 ; id++) {
        // std::cout << id << "\n";
        int value = id;
        int digit_sum=0;
        int digits = log10((float)value) + 1; //this determines the number of digits
        // std::cout << id << "\n";
        // std::cout << digits << "\n";
        for (int i = digits - 1; i >= 0; i--) {
            int divisor = pow((float)10, i);
            int digit = value / divisor;
            digit_sum += digit;
            // std::cout << digit << "\n";
            value -= digit * divisor;

        }
    // std::cout << digit_sum << "\n";
    if ( digit_sum % 7 == 0 ) {
        out.push_back(id);
    }
    }
   return out;
};

class character {
    public:
        std::vector<int> stats = std::vector<int>(7);
        std::vector<int> stat_bonus = std::vector<int>(7);
        std::vector<int> wpn_lvls = std::vector<int>(8);
        std::vector<int> equipment = std::vector<int>(7);
        std::string name;
        std::string unit_class;
        bool mounted;
        bool flying;
        int wpn;
        int id;
        
        // if ever write .dll or .so, use char in dll/so exposed functions.
        // char name = 
        /* Stats order:
            
        */    
};

class weapon {
    public:
        std::vector<int> stats = std::vector<int>(4);
        std::vector<int> stat_bonus = std::vector<int>(7);
        std::string name;
        std::vector<std::string> effective;
        int id;
};

main() {
    std::vector<int> ids;
    ids = ids_list();
std:cout << ids[0];
    return 0;
    std::vector<std::string> unit_stats = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res"};
    std::vector<std::string> weapon_stats = {"dmg", "hit", "crit", "weight"};
    std::vector<std::string> wpn_types = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
    std::vector<std::string> statuses = {"healthy", "sleep", "poison", "stone", "berserk"};
    std::cout << "TESTING THIS BITCH\n";
    character Marth;
    Marth.stats = {1, 2};
    for (std::string x : unit_stats)
        std::cout << x << " ";    
    std::cout << "Marth's " << unit_stats[0] << ":" << Marth.stats[0] << "\n";
    std::cout << "TA MERE\n";
    // printf("Marth's HUGE FACTOR:  (%d)\n", sizeof(Marth));
}