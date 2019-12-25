// Code créé par Gabriel Taillon
/*! \page handle weapon.cpp
* \file weapon.cpp
* \brief weapon class implementation.
*/
#include "shared.hpp"
#include "weapon.hpp"

Weapon::Weapon() {

}

Weapon::Weapon(Weapon_stats in_stats){
    stats = in_stats;
}

Weapon::Weapon(std::string in_name, Weapon_stats in_stats){
    stats = in_stats;
    name = in_name;
}

Weapon::set_malus(Unit_stats in_malus) {
    malus = in_malus;
}

Weapon::set_bonus(Unit_stats in_bonus) {
    bonus = in_bonus;
}
Weapon::set_effect(std::string in_effect) {
    effect = in_effect;
}