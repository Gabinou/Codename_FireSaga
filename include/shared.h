#ifndef SHARED_H
#define SHARED_H

using namespace std;

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) 
// extern std::mt19937 mt(1899); //Deterministic seed. Do like other fire Emblems. Always Same RNG,it is the player actions that change it.
extern std::mt19937 mt; //Deterministic seed. Do like other fire Emblems. Always Same RNG,it is the player actions that change it.
extern std::uniform_int_distribution<char> dist; //*DESIGN QUESTION* What should be the minimum and maximum probabilities?
// extern std::uniform_int_distribution<char> dist(0, 100); //*DESIGN QUESTION* What should be the minimum and maximum probabilities?
// #define getrand() dist(mt) 
inline int getrand() { 
    return(dist(mt));
}
/*! \file shared.h
* \brief Shared data.
*/

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

/// \class weapon
/// \brief Unit class. Implemented to be as light as possible.
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
    unsigned char range[3];
  
    char stats_bonus[10], effective[2][14], owner[5][14];
    /// \var char effective
    /// \brief List of unit types or unit classes the weapon is effective against.
    /// \var char stats_bonus
    /// \brief Bonuses can be negative -> maluses
    /// \var char owner
    /// \brief Array of character names that can equip/use weapon.
    unsigned short int cost;
    /// \var unsigned short int cost
    /// \brief Cost of weapon in Gold.
    bool dmg_type; 
    /// \var bool dmg_type
    /// \brief dmg_type: Damage type. 0 for physical, 1 for magical.
    ~weapon();
    
    /// \fn weapon(std::string, std::string, char, unsigned short int, std::vector<int>,  std::vector<char>, std::vector<std::string>, std::vector<std::string>)
    /// \brief constructor for weapon.
    weapon(std::string, std::string, char, unsigned short int,
        std::vector<int>, std::vector<int>, std::vector<char>,
        std::vector<std::string>, std::vector<std::string>, bool
        );
    weapon();
};

/// \struct inventory_item
/// \brief Representation of items in unit inventory to reduce memory.
struct inventory_item {
    // char *name2;
    char name[30];
    /// \var char name
    /// \brief name of weapon in inventory.
    char used;
    // weapon &wpn_ptr;
    /// \var char used
    /// \brief number of times item was used.
    /// \fn inventory_item(const inventory_item&) 
    /// \brief copy constructor for inventory_item.
    // int &wpn_pnt; 
    /// \var wpn_pnt
    /// \brief Pointer to weapon.
    inventory_item(const inventory_item&); 
    /// \fn inventory_item(std::string, char) 
    /// \brief constructor of inventory_item.
    inventory_item(std::string, char); 
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
    // std::vector<char> stats[10];
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
    /// \fn combat_damage(&unit)
    /// \brief combat_damage performed by a unit on another unit.
    unsigned char combat_damage(const unit&, bool critical);
    /// \fn attack_damage(&unit)
    /// \brief Maximal possible combat damage.
    unsigned char attack_damage();
    /// \fn accuracy(&unit)
    /// \brief Percent accuracy. This minus enemy avoid gives percent chance to hit.
    unsigned char accuracy();
    /// \fn combat_accuracy(&unit)
    /// \brief Percent accuracy in combat. Takes into account the enemy weapon.
    unsigned char combat_hit(const unit&);
    /// \fn avoid(&unit)
    /// \brief Percent avoid. Enemy avoid minus avoid gives percent chance to get hit.
    unsigned char avoid();
     /// \fn combat_avoid(&unit)
    /// \brief Percent avoid. Enemy avoid minus avoid gives percent chance to get hit.  Takes into account the enemy weapon.
    unsigned char critical();
    /// \fn combat_crit(&unit)
    /// \brief Percent critical hit chance. This minus critical hit avoid gives change to hit a critical hit.
    unsigned char combat_critical(const unit&);
    /// \fn favor(&unit)
    /// \brief Percent critical hit evade chance. Enemy critical hit chance minus favor gives chance to get hit by a critical hit.   
    unsigned char favor();
    /// \fn combat_favor(&unit)
    /// \brief Percent critical hit evade chance. Enemy critical hit chance minus favor gives chance to get hit by a critical hit. 
    
    /// \fn take_damage(unsigned char)
    /// \brief set current_hp value. Also check for death.   
    void take_damage(unsigned char);
    /// \fn get_hp()
    /// \brief getter for current_hp   
    void heal(unsigned char);
    /// \fn get_hp()    
    /// \brief getter for current_hp   
    unsigned char get_hp() const;
    /// \fn bool double_attack
    /// \brief Bool that returns if unit double attacks.
    void equip_weapon(std::vector<unsigned int>);
    /// \fn bool double_attack
    /// \brief Bool that returns if unit double attacks.
    bool double_attack(const unit&);
    /// \fn void enemy_select
    /// \brief On enemy selection, compute combat probabilities, values, statistics, etc.
    void enemy_select(const unit&);
    /// \fn void get_equipped
    /// \brief Getter for private equipped.
    const unsigned char* get_equipped() const;
    /// \fn bool combat_retaliation
    /// \brief Does enemy retaliate during combat phase? yes/no.   
    bool retaliation(const unit& enemy) const;
    /// \fn bool combat_double
    /// \brief Does unit double when fighting enemy? yes/no.   
    bool combat_double(const unit& enemy) const;
    /// \fn void combat_
    /// \brief makes the combat phases. Does one retaliate? Does one double? then the attack order is established.
    void combat(unit& enemy);
    /// \fn void attack
    /// \brief Makes a single attack in the combat phase. Also checks for brave effect/vantage skill and doubles if it exists
    unsigned char attack(unit& enemy);
    /// \fn unsigned char wpn_weighed_down
    /// \brief By how much is unit weighed down by its weapon. Gets substracted to speed for combat_double.     
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
        << (int) in_unit.stats[6] << ", " << (int) in_unit.stats[5] << ", " 
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
     
    /// \fn void death
    /// \brief Death function. For now only says that people died. Other functions come later. 
    void death();
    ~unit();
    unit();
};

extern std::unordered_map<string, weapon> all_weapons;
extern std::unordered_map<string, struct inventory_item> inventory_items;
extern std::unordered_map<string, unit> all_units;

#endif /* SHARED_H */