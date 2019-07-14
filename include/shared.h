#ifndef SHARED_H
#define SHARED_H
/*! \file shared.h
* \brief Shared data.
*/
extern char unit_stats[][14];
extern char unit_stats[][14];
extern char weapon_stats[][14];
extern char wpn_types[][12];
extern char weapons[][14];
extern char statuses[][14];
extern char unit_classes[][24];
extern char equipment_slots;
extern char* all_units;
extern char* all_wpns;
extern unsigned char id;


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
    char type[24]; //for units, contains class string. for weapons, contains weapon type string.
    ~generic();
    generic();
    generic(const generic &obj);
};

//Unit implementation notes:
    // unit_vec size \= 2 * unit size

/// \class weapon
class weapon: public generic {
public:
    unsigned char stats[6];
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
    weapon(std::string, std::string, char, unsigned short int,
        std::vector<int>,  std::vector<char>,
        std::vector<std::string>, std::vector<std::string>
        );
    weapon();
};

/// \var inventory_item
/// \brief Representation of items in unit inventory to reduce memory.
struct inventory_item {
    char name[14];
    char used;
    inventory_item(const inventory_item&); 
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
    std::vector<std::vector<unsigned char>> stats_grown;
    
    /// \var current_hp
    /// \brief Current hit points.
    /// \var stats_base
    /// \brief Base statistics. Separated from growths to be able to plot growth over time.
    /// stats_grown
    struct inventory_item equipment[7], equipped[1], weapons[4], items[4];
    /// \var equipment
    /// If equipment is not empty, weapons and items are not used. And vice versa.
    /// The contents of the equipment array are inventory_item structs.
    /// \var equipped
    /// \brief Equipped weapon inventory_item struct.
    unsigned short exp;

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
    unit(std::string, std::string, char,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<inventory_item>, std::vector<inventory_item>,
        std::vector<inventory_item>,
        unsigned short, std::vector<std::string>,
        bool, bool, bool, bool
        );
    char attack(char, unsigned char, unsigned char);
    ~unit();
    unit();
};

#endif /* SHARED_H */