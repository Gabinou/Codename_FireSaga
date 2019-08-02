// Code créé par Gabriel Taillon
/*! \page handle weapon.cpp
* \file weapon.cpp 
* \brief weapon class implementation.
*/
using namespace std;
#include "weapon.hpp"
#include "generic.hpp"
#include "shared.hpp"

weapon::weapon(){
    strncpy(name, "", 30); // for some reason in_name is always of size 8. Whatever.
    strncpy(type, "", 30);
    dmg_type = 0;
    for (int i = 0; i < 2; i++) {
        strncpy(effective[i], "", sizeof(""));
    }
    for (int i = 0; i < 10; i++) {
        stats_bonus[i] = 0;
    }
    for (int i = 0; i < 10; i++) {
        stats[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        range[i] = 0;      
    }
    cost = 0;
    exp = 0;
}
weapon::~weapon(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
weapon::weapon(std::string in_name, std::string in_type, char in_id, unsigned short int in_cost,
       std::vector<int> in_stats, std::vector<int> in_range, std::vector<char> in_stats_bonus, 
       std::vector<std::string> in_effective, bool in_dmg_type, int in_exp) {
    strncpy(name, in_name.c_str(), 30); // for some reason in_name is always of size 8. Whatever.
    // weapon_num[name] = 0;
    strncpy(type, in_type.c_str(), 30);
    dmg_type = (bool) in_dmg_type;
    for (int i = 0; i < in_effective.size(); i++) {
        strncpy(effective[i], in_effective[i].c_str(), sizeof(in_effective));
    }
    for (int i = 0; i < in_stats_bonus.size(); i++) {
        stats_bonus[i] = (int) in_stats_bonus[i];
    }
    for (int i = 0; i < in_stats.size(); i++) {
        stats[i] = in_stats[i];
    }
    for (int i = 0; i < in_range.size(); i++) {
        range[i] = in_range[i];      
    }
    cost = in_cost;
    exp = in_exp;
}