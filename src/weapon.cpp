// Code créé par Gabriel Taillon
/*! \page handle weapon.cpp
* \file weapon.cpp
* \brief weapon class implementation.
*/
#include "weapon.hpp"
#include "shared.hpp"

Weapon::Weapon() {

}

Weapon::Weapon(Weapon_stats in_stats){
    stats = in_stats;
}

Weapon::Weapon(std::string in_name, Weapon_stats in_stats){
    stats = in_stats;
    name = in_name;
}

void Weapon::set_malus(Unit_stats in_malus) {
    malus = in_malus;
}

void Weapon::set_bonus(Unit_stats in_bonus) {
    bonus = in_bonus;
}
void Weapon::set_effect(std::string in_effect) {
    effect = in_effect;
}