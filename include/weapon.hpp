#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <istream>
#include <ostream>
#include <bits/stdc++.h>
#include "inventory_item.hpp"
#include "shared.hpp"

/*! \file weapon.hpp
* \brief Weapon class. Not the one in the inventory, just the immutable weapon data.
*/
/// \class weapon
/// \brief Weapon class. Implemented to be as light as possible.
/// All unsigned variables cannot be negative.
class Weapon {
    private:
    	std::string name;
        Weapon_stats stats;
        Unit_stats bonus;
        Unit_stats malus;
        std::string effect;
        
    public:

        Weapon(Weapon_stats in_stats);
        Weapon(std::string in_name, Weapon_stats in_stats);

        void set_stats(Weapon_stats in_stats);
        void set_bonus(Unit_stats in_bonus);
        void set_malus(Unit_stats in_malus);
        void set_effect(std::string in_effect);
};

/*! \var all_weapons
* \brief Contains all instances of weapons.
*  Uses the weapon's name as std::unordered_map's key.
*  Are immutable. Should never be changed in game.
*  Instead, the inventory_item instances conatains the
*  number of times the item was used. If it is equal to
*  the item's use, the item is destroyed.
*/
extern std::unordered_map<string, weapon> all_weapons;

#endif /* WEAPON_HPP */