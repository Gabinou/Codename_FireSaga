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

Weapon::Weapon(std::string in_name, std::string in_type, Weapon_stats in_stats){
    stats = in_stats;
    name = in_name;
    name = in_type;
}

Weapon_stats Weapon::getStats() {
    return(stats);
}

void Weapon::setMalus(Unit_stats in_malus) {
    malus = in_malus;
}

Unit_stats Weapon::getMalus() {
    return(malus);
}

void Weapon::setStats(Weapon_stats in_stats) {
    stats = in_stats;
}

void Weapon::setBonus(Unit_stats in_bonus) {
    bonus = in_bonus;
}
Unit_stats Weapon::getBonus() {
    return(bonus);
}

void Weapon::setEffect(std::string in_effect) {
    effect = in_effect;
}
std::string Weapon::getEffect() {
    return(effect);
}
void Weapon::setEffective(std::string in_effective) {
	effective = in_effective;
}
std::string Weapon::getEffective() {
    return(effective);
}
void Weapon::setDescription(std::string in_description) {
	description = in_description;
}
std::string Weapon::getDescription() {
    return(description);
}
