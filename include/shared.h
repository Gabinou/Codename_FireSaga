#ifndef SHARED_H
#define SHARED_H
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
    char name[14];
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
    unsigned char stats[8];
    /*! \var unsigned char stats.
    * \brief Weapon statistics.
    * Weapon stats descriptions: <br>
    *  - dmg: Damage. +1 dmg -> +1 Attack. Refer to... <br>
    *  - hit: Hit chance. % probability value to hit. Refer to ... <br>
    *  - crit: Crit chance. % probability value to crit. Refer to...  <br>
    *  - wpn_exp: Weapon experience. Base weapon experience for using the weapon. Refer to...  <br>
    *  - uses: Number of uses a weapon has before breaking.  <br>
    *  - range: distance in squares to which a unit can attack with weapon.  <br>
    *  - lvl: Weapon level. Integer representing the weapon experience/level necessary to use the weapon. Refer to ...  <br>
    */
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
        std::vector<int>,  std::vector<char>,
        std::vector<std::string>, std::vector<std::string>
        );
    weapon();
};

/// \struct inventory_item
/// \brief Representation of items in unit inventory to reduce memory.
struct inventory_item {
    char name[20];
    /// \var char name
    /// \brief name of weapon in inventory.
    char used;
    /// \var char used
    /// \brief number of times item was used.
    /// \fn inventory_item(const inventory_item&) 
    /// \brief copy constructor for inventory_item.
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
public:
    char  stats_bonus[10];
    /// \var stats_bonus
    /// \brief Bonuses can be negative -> maluses
    unsigned char current_hp, stats_base[10],
    growths[10], wpn_exp[10], position[3], skills[3],
    love_pts[5], love_growths[5];
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
    std::vector<std::vector<unsigned char>> stats_grown;
    /// \var stats_grown
    /// \brief All stats grown for level ups. Stack of 10-length rows.
    /// \var current_hp
    /// \brief Current hit points.
    /// \var stats_base
    /// \brief Base statistics. Separated from growths to be able to plot growth over time.
    /// stats_grown
    struct inventory_item equipment[7], equipped[1], weapons[4], items[4];
    /// \var equipment
    /// \brief If equipment is not empty, weapons and items are not used. And vice versa.
    /// \var items
    /// \brief Only used if equipment is not. Slots only for items/not weapons.
    /// \var weapons
    /// \brief Only used if equipment is not. Slots only for weapons.
    /// The contents of the equipment array are inventory_item structs.
    /// \var equipped
    /// \brief Equipped weapon inventory_item struct.
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
        std::vector<unsigned int>,
        std::vector<inventory_item>, std::vector<inventory_item>,
        std::vector<inventory_item>, std::vector<inventory_item>, 
        unsigned short, std::vector<std::string>,
        bool, bool, bool, bool
        );
    /// \fn attack(&unit)
    /// \brief Attack damage performed by a unit on another unit.
    unsigned char attack(const unit&);
    ~unit();
    unit();
};

#endif /* SHARED_H */