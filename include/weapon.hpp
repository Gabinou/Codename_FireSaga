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
    /// \var unsigned short int cost
    /// \brief Cost of weapon in Gold.
    bool dmg_type; 
    /// \var bool dmg_type
    /// \brief dmg_type: Damage type. 0 for physical, 1 for magical.
    ~weapon();
    
    void read(const char *filename, char skip) {
        // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 8.
        FILE *f = fopen(filename, "r");
        char line[500];
        char line2[500];
        for (int i = 0; i < skip; i++) {
            fgets(line, sizeof(line), f); // skips skip lines.
        }
        fgets(line, sizeof(line), f);
        line[strlen(line)-1] = 0;  //fgets also includes the \n in the line. This removes it.
        strncpy(name, line, sizeof(name));        
        fscanf(f, "%*s %s", type);
        fgets(line, sizeof(line), f);
        fgets(line, sizeof(line), f);
        std::vector<int> temp = extractIntegerWords(line);
        fgets(line, sizeof(line), f);
        exp = temp[0];
        temp = extractIntegerWords(line);
        for (int i = 0; i < temp.size(); i++) {
            stats[i] = temp[i];
        }
        fgets(line, sizeof(line), f);
        temp = extractIntegerWords(line);
        for (int i = 0; i < temp.size(); i++) {
            stats_bonus[i] = temp[i];
        }     
        fgets(line, sizeof(line), f);      
        cost = extractIntegerWords(line)[0];          
        fgets(line, sizeof(line), f);
        dmg_type = extractIntegerWords(line)[0];
        fgets(line, sizeof(line), f);
        temp = extractIntegerWords(line);
        range[0] = temp[0];
        range[1] = temp[1];
        fseek(f, 10, SEEK_CUR);
        fgets(line, sizeof(line), f);
        char * pch;
        char eff = 0, i = 0;
        while (pch != NULL) {
            if (i == 0) {
                pch = strtok(line, ":,");
            } else {
                pch = strtok(NULL, ":,");       
            }
            // printf("%s \n", pch);
            // printf("%d \n", strstr(pch.c_str(), "Knight"));
            if (pch!=NULL) {
                for (int j = 0; j < 33; j++) {
                    // printf("%s %s \n", pch, unit_classes[j]);
                    if (strstr(pch, ::unit_classes[j]) != 0) {
                        strcpy(effective[eff], ::unit_classes[j]);
                        printf("%s \n", effective[eff]);
                        eff++;
                    }
                }
                for (int j = 0; j < 4; j++) {
                    if (strstr(pch, ::unit_attributes[j]) != 0) {
                        strcpy(effective[eff], ::unit_attributes[j]);
                        printf("%s \n", effective[eff]);
                        eff++;
                    }
                }
            }
            i++;
            
        }
        fclose(f);
    }; 
    friend std::ostream & operator << (std::ostream &out, const weapon &in_weapon) {
        out << in_weapon.name << "\n";
        // out << "Stats: \t\t" // must stay like this cause of busted stats variable that needs more space to not crash.
        // << (int) in_weapon.stats[0] << ", " << (int) in_weapon.stats[1] << ", " 
        // << (int) in_weapon.stats[2] << ", " << (int) in_weapon.stats[3] << ", " 
        // << (int) in_weapon.stats[4] << ", " << (int) in_weapon.stats[5] << "\n";
        out << "Type: \t\t";
        out << in_weapon.type << "\n";
        out << "Wpn_Exp: \t";
        printf("%d,\n", in_weapon.exp);
        out << in_weapon.exp << "\n";
        out << "Stats: \t\t";
        for (int i = 0; i < sizeof(in_weapon.stats)/sizeof(in_weapon.stats[0]); i++) {
            out << (int) in_weapon.stats[i] << ", ";
        };
        out << "\n" << "Bonus: \t\t";
        for (int i = 0; i < sizeof(in_weapon.stats_bonus)/sizeof(in_weapon.stats_bonus[0]); i++) {
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
        return(out);
    }
    /// \fn weapon(std::string, std::string, char, unsigned short int, std::vector<int>, std::vector<int>, std::vector<char>, std::vector<std::string>, std::vector<std::string>, bool)
    /// \brief constructor for weapon.
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