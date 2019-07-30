#ifndef SHARED_H
#define SHARED_H
using namespace std;
#include <stdio.h>
#include <iostream>
#include <string>
#include <ostream>
#include <istream>
#include <ctype.h>
#include <vector>
#include <math.h> 
#include <random>
#include <bits/stdc++.h> 
/*! \file shared.h
* \brief Shared data.
*/
/// \def LEN(arr)
/// \brief Measure length of array. I don't know why this is a macro.
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) 
// extern std::mt19937 mt(1899); 
//Deterministic seed. *DESIGN QUESTION*: What about the RNG? My answer: do like other fire Emblems. Always Same RNG, it is the player actions that change it. Makes debugging repeatable. Nice and convenient.
/// \fn mt
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern std::mt19937 mt; 
/// \fn dist
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern std::uniform_int_distribution<char> dist; //*DESIGN QUESTION* What should be the minimum and maximum probabilities?

/// \fn get_rand
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern int get_rand();

/// \fn single_roll(int)
/// \brief Check if event occurs using single RNG roll: if rand<prob, event occurs. True to probabilities, but humans think it is weird.
extern bool single_roll(int);

/// \fn double_roll(int)
/// \brief Check if event occurs using double RNG roll: if mean of 2 random numbers is lwer than probability of event, it occurs. Skews probabilities, but fits with humans biases.
extern bool double_roll(int);

extern char unit_attributes[][24];

extern char unit_stats[][14];
/// \var extern char unit_stats
/// \brief Global declaration of unit_stats.
extern char weapon_stats[][14];
/// \var extern char weapon_stats
/// \brief Global declaration of weapon_stats.
extern char wpn_types[][12];
/// \var extern char wpn_types
/// \brief Global declaration of wpn_types.
extern char weapons[][14];
/// \var extern char weapons
/// \brief Global declaration of weapons.
extern char statuses[][14];
/// \var extern char statuses
/// \brief Global declaration of statuses.
extern char unit_classes[][24];
/// \var extern char unit_classes
/// \brief Global declaration of unit_classes.
extern char equipment_slots;
/// \var extern char equipment_slots
/// \brief Global declaration of equipment_slots.
extern unsigned char id;
/// \var extern char id
/// \brief Global declaration of id.

/// \class generic
/// \brief Class from which the most objects inherit from.
class generic {
public:
    char name[120];
    /// \var char name
    /// \brief Name of generic object.
    unsigned char id;
    /// \var unsigned char id
    /// \brief All objects get ids.
    char type[24]; 
    /// \var type
    /// \brief For units, contains class string. For weapons, contains weapon type string.
    ~generic();
    generic();
    generic(const generic &obj);
    /// \fn generic(const generic &obj)
    /// \brief Copy constructor for generic object. Useless?
};

extern std::vector<int> extractIntegerWords(string);

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
    
    friend std::istream & operator >> (std::istream &in, weapon &out_weapon) {
        std::string buf;
        in >> std::skipws;
        // std::getline(in, buf);
        in >> buf;
        // std::getline(in, buf);
        // std::getline(in, buf[2]);
        // std::getline(in, buf[3]);
        // std::getline(in, buf[4]);
        // std::getline(in, buf[5]);
        // std::getline(in, buf[6]);
        // std::getline(in, buf[7]);

        strncpy(out_weapon.name,  buf.c_str(), sizeof(buf));
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        in >> buf;
        // in >> buf;
        // std::getline(in, buf);
        // std::vector<int> temp = extractIntegerWords(buf);
        // for (int i = 0; i < temp.size(); i++) {
            // out_weapon.stats[i] = temp[i];
        // }
        // in >> buf;  
        // std::getline(in, buf);
        // temp = extractIntegerWords(buf);
        // for (int i = 0; i < temp.size(); i++) {
            // out_weapon.stats_bonus[i] = temp[i];
        // }
        // in >> buf;
        // std::getline(in, buf);
        // temp = extractIntegerWords(buf);
        // out_weapon.cost = temp[0];
        // in >> buf;
        // std::getline(in, buf);
        // temp = extractIntegerWords(buf);
        // out_weapon.dmg_type = temp[0];
        // in >> buf;
        // std::getline(in, buf);
        // temp = extractIntegerWords(buf);
        // for (int i = 0; i < temp.size(); i++) {
            // out_weapon.range[i] = temp[i];
        // }
        // in >> buf;
        // in >> buf;
        // strncpy(out_weapon.effective[0],  buf.c_str(), sizeof(buf)); 
        // in >> buf;
        // strncpy(out_weapon.effective[1],  buf.c_str(), sizeof(buf)); 
        return(in);
    }
    void read(const char *filename, char skip) {
        // 2019/07/30: skip should be a multiple of *number of lines written to weapon.txt* which is 8.
        FILE *f = fopen(filename, "r");
        char line[500];
        for (int i = 0; i < skip; i++) {
            fgets(line, sizeof(line), f); // skips n lines.
        }
        fgets(line, sizeof(line), f);
        strncpy(name, line, sizeof(name));
        fgets(line, sizeof(line), f);
        std::vector<int> temp = extractIntegerWords(line);
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
        for (int i = 0; i < temp.size(); i++) {
            range[i] = temp[i];
        }
        // fgets(line, sizeof(line), f);
        fseek(f, 10, SEEK_CUR);
        fgets(line, sizeof(line), f);
        char * pch;
        std:;string pch2;
        int eff = 0;
        int val = 0 ;
        char ind;
        for (int i = 0; i < sizeof(effective)/sizeof(effective[0]); i++) {
            if (i == 0) {
                pch = strtok(line, ":,");
            } else {
                pch = strtok(NULL, ":,");       
            }      
            for (int j = 0; j < 33; j++) {
                printf ("%d\n", strstr(pch, unit_classes[j]));  
                printf ("%s %s\n", pch, unit_classes[j]);  
                if (strstr(pch, unit_classes[j]) != 0) {
                    strcpy(effective[eff], unit_classes[j]);
                    eff++;
                }
            }
            // if (ind != -1) {
                // effective[eff] = unit_classes[ind]
            // }
            ind = -1;
            for (int j = 0; j < 4; j++) {
                printf ("%d\n", strstr(pch, unit_attributes[j]));  
                printf ("%s %s\n", pch, unit_attributes[j]); 
                if (strstr(pch, unit_attributes[j]) != 0) {
                    strcpy(effective[eff], unit_attributes[j]);
                    eff++;
                }
            }
            // if (ind != -1) {
                // strcpy(effective[eff], unit_attributes[ind]);
            // }
        }
        printf ("%d\n", eff); 
        

        
          // while (pch != NULL)
          // {
            // printf ("%s\n", pch);
            // pch = strtok (NULL, ":,");
            // strstr(pch, unit_attributes[0]);
            // // for (int i = 0; i < 4; i++) {
                // // if (strstr(pch, unit_attributes[i]) == 0) {
                    // // strcpy(effective[eff], unit_attributes[i]);
                    // // eff++;
                // // }
            // // }
            // // for (int i = 0; i < 4; i++) {
                // // if (strstr(pch, unit_classes[i]) == 0) {
                    // // strcpy(effective[eff], unit_classes[i]);
                    // // eff++;
                // // }
            // // }
          // }
        // sscanf(line, "%s %500", word1, word2, word3, word4);
        // // word1[strlen(word1)-1] = 0;
        // printf("%s\n", line);
        // printf("%s\n", word1);
        // printf("%s\n", word2);
        // for (int i = 0; i < 4; i++) {
            // if (strstr(word1, unit_attributes[i]) == 0) {
                // strcpy(effective[0], unit_attributes[i]);
            // }
        // }
        // for (int i = 0; i < 33; i++) {
            // if (strstr(word1, unit_classes[i]) == 0) {
                // strcpy(effective[0], unit_classes[i]);
            // }
        // }
        // for (int i = 0; i < 4; i++) {
            // if (strstr(word2, unit_attributes[i]) == 0) {
                // strcpy(effective[1], unit_attributes[i]);
            // }
        // }
        // for (int i = 0; i < 33; i++) {
            // if (strstr(word2, unit_classes[i]) == 0) {
                // strcpy(effective[1], unit_classes[i]);
            // }
        // }
        // fscanf(f, " %[^, \n],", word);
        // printf("%s\n", word);
        printf("THIS\n");
        // fscanf(f, "%s", line);
        // fscanf(f, "%s", line);
        //     
        // strcpy(effective[0], line);
        // fscanf(f, "%s", effective[1]);
        fclose(f);
    }; 
    friend std::ostream & operator << (std::ostream &out, const weapon &in_weapon) {
        out << in_weapon.name << "\n";
        // out << "Stats: \t\t" // must stay like this cause of busted stats variable that needs more space to not crash.
        // << (int) in_weapon.stats[0] << ", " << (int) in_weapon.stats[1] << ", " 
        // << (int) in_weapon.stats[2] << ", " << (int) in_weapon.stats[3] << ", " 
        // << (int) in_weapon.stats[4] << ", " << (int) in_weapon.stats[5] << "\n";
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
        if (in_weapon.range[0] == in_weapon.range[1]) {
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
        std::vector<std::string>, bool
        );
    weapon();
};

/// \struct inventory_item
/// \brief Representation of items in unit inventory to reduce memory.
struct inventory_item {
    /// \var char name
    /// \brief name of weapon in inventory.
    char name[30];   
    /// \var used
    /// \brief number of times item was used.
    char used;
    /// \fn inventory_item(const inventory_item&) 
    /// \brief constructor of inventory_item.
    inventory_item(const inventory_item&); 
    /// \fn inventory_item(std::string, char) 
    /// \brief constructor of inventory_item.
    inventory_item(std::string, char); 
    /// \fn inventory_item() 
    /// \brief constructor of inventory_item.
    inventory_item(); 
    ~inventory_item(); 

};
/// \class unit
/// \brief Unit class. Implemented to be as light as possible.
/// All unsigned variables cannot be negative.
class unit: public generic {
    // Lightweight implementation.
private: 
    unsigned char equipped[1]; //needs setters for stats update. 
    // DOUBLE DUTY: is also the number of the last element in equipment. Maybe in weapons alsl.
    char current_hp; //needs setters for death check on update
    
public:
    char stats_bonus[10];
    /// \var stats_bonus
    /// \brief Bonuses can be negative -> maluses
    /// \var stats
    /// \brief current stats. Computed every level up using stats_grown.
    // for seom reason, stats array of len<19 cause cygwin_exception
    unsigned char stats[19], stats_base[10],
    growths[10], wpn_exp[10], position[3], skills[3],
    love_pts[5], love_growths[5],
    attack_probs[4], combat_probs[2];
    /// \var unsigned char attack_probs[4]
    /// \brief Probabilities associated with attacking, computed when alone.
    /// Accuracy, Avoid, Crit and Favor.
    /// \var unsigned char combat_probs[4]
    /// \brief probabilities associated with combat with an enemy unit.
    /// Accuracy, Avoid, Crit and Favor.
    /// \var unsigned char love_growths
    /// \brief Number of points units that have love potential get for *DESIGN QUESTION*.
    /// \var unsigned char growths
    /// \brief Unit stats growth: percent probability that upon level up, unit stats grows by one. Growth>100% are interpreted as meaning a unit can grow +1 or +2.
    /// \var unsigned char position
    /// \brief Position on the map. 3D because of the possibility of fliers to fly higher. And underground paths.
    /// \var unsigned char skills
    /// \brief Skill names that unit possesses.    
    /// \var unsigned char wpn_exp
    /// \brief Wpn experience possessed by the unit. If a wpn_exp is equal to 0, unit cannot equip associated weapon type.    
    /// \var unsigned char love_pts
    /// \brief current love points. Refer to ...
    /// \var equipped
    /// \brief Equipped weapon. Index of of weapon in equipment vector or weapon vector. Is a vector in case not only weapon can be equipped. In such case equipped[0] would be the weapon, and equipped[1] an item, etc.
    std::vector<std::vector<unsigned char>> stats_grown;
    /// \var stats_grown
    /// \brief All stats grown for level ups. Stack of 10-length rows.
    /// \var current_hp
    /// \brief Current hit points.
    /// \var stats_base
    /// \brief Base statistics. Separated from growths to be able to plot growth over time.
    /// stats_grown
    struct inventory_item equipment[7], weapons[4], items[4];
    /// \var equipment
    /// \brief If equipment is not empty, weapons and items are not used. And vice versa.
    /// \var items
    /// \brief Only used if equipment is not. Slots only for items/not weapons.
    /// \var weapons
    /// \brief Only used if equipment is not. Slots only for weapons.
    /// The contents of the equipment array are inventory_item structs.
    unsigned short int exp;
    /*! \var unsigend short int exp
    * \brief Total Unit Experience points.
    * Not unit level in data. The experience points left most digits are the level. Example: 423exp means unit level 5 with 23 exp.
    */
    char lovers[5][14]; 
    /// \var char lovers
    /// \brief Names of possible lovers.
    bool mounted, flying, armored, promoted;
    /// \var bool mounted
    /// \brief. 1/0 mounted. For effective weapons
    /// \var bool flying
    /// \brief. 1/0 flying. For effective weapons and movement    
    /// \var bool armored
    /// \brief. 1/0 unit amored. For effective weapons
    /// \var bool promoted
    /// \brief. 1/0 unit promoted. For exp on enemy kill.
    /// \fn w  unit(std::string, std::string, char,         std::vector<unsigned int>, std::vector<unsigned int>,       std::vector<unsigned int>, std::vector<unsigned int>, std::vector<unsigned int>, std::vector<unsigned int>,      std::vector<unsigned int>, std::vector<unsigned int>, std::vector<inventory_item>, std::vector<inventory_item>, std::vector<inventory_item>, unsigned short, std::vector<std::string>, bool, bool, bool, bool)
    /// \brief constructor for unit.
    unit(std::string, std::string, char,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<inventory_item>,
        std::vector<inventory_item>, std::vector<inventory_item>, 
        unsigned short, std::vector<std::string>,
        bool, bool, bool, bool
        );
    /*! \fn unsigned char combat_damage(onst unit& enemy, bool critical)
    *  \brief Computes unit's damage during combat for a single attack, taking the critical into account.
    */
    unsigned char combat_damage(const unit&, bool critical);
    /*! \fn attack_damage()
    *  \brief Standalone unit's theoretical max damage for a single attack. Essentially, Str/Mag + weapon damage.
    */
    unsigned char attack_damage();
    /*! \fn accuracy()
    *  \brief Percent "probability" of standalone unit to hit enemy unit.
    *  Becomes a real probablity when substracted to enemy avoid.
    */
    unsigned char accuracy();
    /*! \fn combat_hit()
    *  \brief Percent probability of unit to hit enemy in combat.
    */
    unsigned char combat_hit(const unit&);
    /*! \fn avoid()
    *  \brief Percent "probability" of standalone unit to avoid incoming attack.
    *  Becomes a real probablity when substracted to enemy accuracy.
    */
    unsigned char avoid();
    /*! \fn critical()
    *  \brief Percent "probability" of standalone unit to perform critical hit on enemies.
    *  Becomes a real probablity when substracted to enemy avoid.
    */
    unsigned char critical();
    /*! \fn combat_critical(const unit& enemy)
    *  \brief Percent probability to perform a critical hit on your enemy in combat.
    */
    unsigned char combat_critical(const unit&);
    /*! \fn char favor()
    *  \brief Percent "probability" by which attacking enemy unit's critical chance get reduced. Essentially: crit avoid.
    *   I struggled to name this one. Alternatives include: blessings, blessed, divine, etc. All names though point to RNJesus: reducing the likelihood of getting *   your face critted off is divine after all. So yeah, I think of this value as RNJesus's divine favor.
    */
    unsigned char favor();
    /*! \fn take_damage()
    *  \brief Unit takes damage, decreases current_hp. Decided to keep take_damage and heal functions separate. Why? Dunno.
    *   Also checks for death.
    */  
    void take_damage(unsigned char);
    /*! \fn heal()
    *  \brief Heal unit, increases current_hp. Decided to keep take_damage and heal functions separate. Why? Dunno. Does not check for death.
    */
    void heal(unsigned char);
    /// \fn get_hp()    
    /// \brief getter for current_hp   
    unsigned char get_hp() const;
    /// \fn equip_weapon(std::vector<unsigned int>)
    /// \brief Setter for private equipped.
    void equip_weapon(std::vector<unsigned int>);
    /// \fn double_attack
    /// \brief Bool that returns if unit double attacks.
    bool double_attack(const unit&);
    /// \fn enemy_select
    /// \brief On enemy selection, compute combat probabilities, values, statistics, etc.
    void enemy_select(const unit&);
    /// \fn get_equipped
    /// \brief Getter for private equipped.
    const unsigned char* get_equipped() const;
    /*! \fn retaliation(const unit& enemy)
    *  \brief When unit gets attacked in combat, does he attacks back? 1/0. Mainly a check for range.
    */  
    bool retaliation(const unit& enemy) const;
    /*! \fn bool combat_double(const unit& enemy)
    *  \brief Does unit perform a double hit on enemy? 1/0. Normally called in combat.
    */
    bool combat_double(const unit& enemy) const;
    /// \fn combat(unit& enemy)
    /// \brief Makes a single combat phase. Does the enemy retaliate? Does unit or enemy double? Makes all checks, then perform all attacks in order.
    void combat(unit& enemy);
    /*! \fn attack(unit enemy)
    *  \brief Perform a single attack on the enemy.
    *   An attack checks if hit connects, if it crits and then computes the damage, for a single attack.
    *   Doubling, brave effects and other skills detemine the number of attacks in function combat.
    */
    unsigned char attack(unit& enemy);
    /*! \fn unsigned char wpn_weighed_down()
    *  \brief Amount substracted to speed in combat because of weapon weight.
    */
    // I think this should be shown in the UI. It should show the max value and current value as a function of equipped weapon, with something saying 'WEIGHED DOWN' or something.
    unsigned char wpn_weighed_down() const;
    /// \fn void write
    /// \brief write the object to file. Keeps the exact same formatting as the friend << function.
    void write(const char *filename) {
        FILE *f = fopen(filename, "a");
        fprintf(f, "%s \n", name);
        fprintf(f, "Class: \t\t %s \n", type);
        fprintf(f, "Class: \t\t %s \n", type);
        fclose(f);
    }; 
    
    /// \brief friend << (it overloads it) for unit to write the unit to text file.
    friend std::ostream & operator << (std::ostream &out, const unit &in_unit) {
        out << in_unit.name << "\n"
        << "Class: \t\t" << in_unit.type << "\n"
        << "Bases: \t\t";
        for (int i = 0; i < sizeof(in_unit.stats_base)/sizeof(in_unit.stats_base[0]); i++) {
            if (i == (sizeof(in_unit.stats_base)/sizeof(in_unit.stats_base[0])) - 1) {
                out << (int) in_unit.stats_base[i] << "\n";
            } else {
                out << (int) in_unit.stats_base[i] << ", ";
            }
        };
        out << "Stats: \t\t" // must stay like this cause of busted stats variable that needs more space to not crash.
        << (int) in_unit.stats[0] << ", " << (int) in_unit.stats[1] << ", " 
        << (int) in_unit.stats[2] << ", " << (int) in_unit.stats[3] << ", " 
        << (int) in_unit.stats[4] << ", " << (int) in_unit.stats[5] << ", " 
        << (int) in_unit.stats[6] << ", " << (int) in_unit.stats[7] << ", " 
        << (int) in_unit.stats[8] << ", " << (int) in_unit.stats[9] << "\n"
        << "Growths: \t";
        for (int i = 0; i < sizeof(in_unit.growths)/sizeof(in_unit.growths[0]); i++) {
            if (i == (sizeof(in_unit.growths)/sizeof(in_unit.growths[0])) - 1) {
                out << (int) in_unit.growths[i] << "\n";
            } else {
                out << (int) in_unit.growths[i] << ", ";
            }
        };
        out << "Equipped: \t" << in_unit.equipment[in_unit.get_equipped()[0]].name << "\n"
        << "Equipment: \t";
        for (int i = 0; i < sizeof(in_unit.equipment)/sizeof(in_unit.equipment[0]); i++) {
            out << in_unit.equipment[i].name << ", ";
        };
        out << "\n" << "Weapons: \t";
        for (int i = 0; i < sizeof(in_unit.weapons)/sizeof(in_unit.weapons[0]); i++) {
            out << in_unit.weapons[i].name << ", ";
        };
        out << "\n" << "Items: \t\t"; 
        for (int i = 0; i < sizeof(in_unit.items)/sizeof(in_unit.items[0]); i++) {
            out << in_unit.items[i].name << ", ";
        };
        out << "\n" << "Lovers: \t";
        for (int i = 0; i < sizeof(in_unit.lovers)/sizeof(in_unit.lovers[0]); i++) {
            if (i == (sizeof(in_unit.lovers)/sizeof(in_unit.lovers[0])) - 1) {
                out << in_unit.lovers[i] << "\n";
            } else {
                out << in_unit.lovers[i] << ", ";
            }
        };
        out << "love_pts: \t";
        for (int i = 0; i < sizeof(in_unit.love_pts)/sizeof(in_unit.love_pts[0]); i++) {
            if (i == (sizeof(in_unit.love_pts)/sizeof(in_unit.love_pts[0])) - 1) {
                out << (int) in_unit.love_pts[i] << "\n";
            } else {
                out << (int) in_unit.love_pts[i] << ", ";
            }
        };
        out << "love_grt: \t";
        for (int i = 0; i < sizeof(in_unit.love_growths)/sizeof(in_unit.love_growths[0]); i++) {
            if (i == (sizeof(in_unit.love_growths)/sizeof(in_unit.love_growths[0])) - 1) {
                out << (int) in_unit.love_growths[i] << "\n";
            } else {
                out << (int) in_unit.love_growths[i] << ", ";
            }
        };
        out << "wpn_exp: \t";
        for (int i = 0; i < sizeof(in_unit.wpn_exp)/sizeof(in_unit.wpn_exp[0]); i++) {
            if (i == (sizeof(in_unit.wpn_exp)/sizeof(in_unit.wpn_exp[0])) - 1) {
                out << (int) in_unit.wpn_exp[i] << "\n";
            } else {
                out << (int) in_unit.wpn_exp[i] << ", ";
            }
        };
        out << "Exp: \t\t" << in_unit.exp << "\n"
        << "Mounted: \t" << in_unit.mounted << "\n"
        << "Flying: \t"  << in_unit.flying << "\n"
        << "Armored: \t" << in_unit.armored << "\n"
        << "Promoted: \t" << in_unit.promoted << "\n"
        << std::endl;
        return(out);
    }
     
    /*! \fn death()
    *  \brief What happens when character dies.
    */
    void death();
    ~unit();
    unit();
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
/*! \var inventory_items
* \brief Contains all instances of inventory_items
*  inventory items are the represntation of the weapon
*  in the equipment of units. Made to be lighter than 
*  actually copying the weapon. 
*/
// I think a unordered map inventory_items should exist for every save.
extern std::unordered_map<string, struct inventory_item> inventory_items;
/*! \var all_units
* \brief Contains all instances of units.
*  Uses the unit's name as std::unordered_map's key.
*/
// I think a unordered map all_units should exist for every save.
extern std::unordered_map<string, unit> all_units;

#endif /* SHARED_H */