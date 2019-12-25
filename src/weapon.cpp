// Code créé par Gabriel Taillon
/*! \page handle weapon.cpp
* \file weapon.cpp
* \brief weapon class implementation.
*/
#include "shared.hpp"
#include "weapon.hpp"


Weapon::Weapon(Weapon_stats in_stats){
    stats = in_stats;
}

Weapon::Weapon(std::string in_name, Weapon_stats in_stats){
    stats = in_stats;
    name = in_name;
}
