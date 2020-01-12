#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "shared.hpp"
#include "ECS.hpp"
#include "game.hpp"
#include "shared.hpp"
#include "SDL2/SDL.h"
#include <stdio.h>

/*! \file weapon.hpp
* \brief Weapon class. Not the one in the inventory, just the immutable weapon data.
*/
/// \class weapon
/// \brief Weapon class. Implemented to be as light as possible.
/// All unsigned variables cannot be negative.
class Weapon {
    private:
        Weapon_stats stats;
        //hp,str,mag,skl,spd,luck,def,res,con,move
        Unit_stats bonus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        Unit_stats malus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        std::string effect = "None";
        std::string type;
        std::string name;
        std::string effective;
        std::string description;
    public:

        Weapon();
        Weapon(Weapon_stats in_stats);
        Weapon(std::string in_name, Weapon_stats in_stats);
        Weapon(std::string in_name, std::string in_type, Weapon_stats in_stats);

        void setStats(Weapon_stats in_stats);
        Weapon_stats getStats();
        void setBonus(Unit_stats in_bonus);
        Unit_stats getBonus();
        void setMalus(Unit_stats in_malus);
        Unit_stats getMalus();
        void setEffect(std::string in_effect);
        std::string getEffect();
        void setEffective(std::string in_effective);
        std::string getEffective();
        void setDescription(std::string in_description);
        std::string getDescription();
};

/*! \var all_weapons
* \brief Contains all instances of weapons.
*  Uses the weapon's name as std::unordered_map's key.
*  Are immutable. Should never be changed in game.
*  Instead, the inventory_item instances conatains the
*  number of times the item was used. If it is equal to
*  the item's use, the item is destroyed.
*/
extern std::unordered_map<std::string, Weapon> all_weapons;

#endif /* WEAPON_HPP */