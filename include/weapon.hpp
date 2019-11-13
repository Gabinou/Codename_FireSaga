#ifndef WEAPON_HPP
#define WEAPON_HPP
using namespace std;
#include <istream>
#include <ostream>
#include <bits/stdc++.h>
#include "generic.hpp"
#include "inventory_item.hpp"
#include "shared.hpp"

/*! \file weapon.hpp
* \brief Weapon class. Not the one in the inventory, just the immutable weapon data.
*/
/// \class weapon
/// \brief Weapon class. Implemented to be as light as possible.
/// All unsigned variables cannot be negative.
class weapon: public generic {
    public:
        unsigned char stats[6];
        /*! \var unsigned char stats.
        * \brief Weapon statistics.
        * Weapon stats descriptions: <br>
        *  - dmg: Damage. +1 dmg -> +1 attack_damage. Refer to... <br>
        *  - hit: Hit chance. % probability value to hit. Refer to ... <br>
        *  - critical hit: critical hit chance. % probability value to critical hit. Refer to...  <br>
        *  - wpn_exp: Weapon experience. Base weapon experience for using the weapon. Refer to...  <br>
        *  - uses: Number of uses a weapon has before breaking.  <br>
        *  - range: distance in squares to which a unit can attack_damage with weapon.  <br>
        *  - lvl: Weapon level. Integer representing the weapon experience/level necessary to use the weapon. Refer to ...  <br>
        */
        /// \var unsigned char range
        /// \brief Range of weapon.
        unsigned char range[2]; // 2- maximum,minimuum range.

        char stats_bonus[10], effective[2][20];
        int exp;
        /// \var char effective
        /// \brief List of unit types or unit classes the weapon is effective against.
        /// \var char stats_bonus
        /// \brief Bonuses can be negative -> maluses
        unsigned short int cost;
        std::vector<std::string> owners, exclude;
        /// \var string owners
        /// \brief Only people in owners list can use the weapon, unless empty.
        /// \var string exclude
        /// \brief classes or units that cannot use the weapon.

        /// \var unsigned short int cost
        /// \brief Cost of weapon in Gold.
        bool dmg_type;
        /// \var bool dmg_type
        /// \brief dmg_type: Damage type. 0 for physical, 1 for magical.
        ~weapon();

        void read(const char *, short int);

        void write(string);

        friend std::ostream & operator << (std::ostream & out, const weapon & in_weapon) {
            out << in_weapon.name << "\n";
            out << "Type: \t\t";
            out << in_weapon.type << "\n";
            out << "Wpn_Exp: \t";
            out << in_weapon.exp << "\n";
            out << "Stats: \t\t";

            for (int i = 0; i < sizeof(in_weapon.stats) / sizeof(in_weapon.stats[0]); i++) {
                out << (int) in_weapon.stats[i] << ", ";
            };

            out << "\n" << "Bonus: \t\t";

            for (int i = 0; i < sizeof(in_weapon.stats_bonus) / sizeof(in_weapon.stats_bonus[0]); i++) {
                out << (int) in_weapon.stats_bonus[i] << ", ";
            };

            out << "\n" << "Cost: \t\t" << in_weapon.cost << "\n";

            out << "Dmg_type: \t" << in_weapon.dmg_type << "\n";

            out << "Range: \t\t";

            if (in_weapon.range[1] == 0) {
                out << (int) in_weapon.range[0] << "\n";
            } else {
                out << (int) in_weapon.range[0] << ", " << (int) in_weapon.range[1] << "\n";
            }

            out << "Effective: \t";
            out << in_weapon.effective[0] << ", " << in_weapon.effective[1] << "\n";
            out << "\n";
            return (out);
        }
        weapon(std::string, std::string, char, unsigned short int,
               std::vector<int>, std::vector<int>, std::vector<char>,
               std::vector<std::string>, bool, int
              );
        weapon();
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