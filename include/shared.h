#ifndef SHARED_H
#define SHARED_H

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

class generic {
public:
    char name[14];
    unsigned char id;
    char type[24]; //for units, contains class string. for weapons, contains weapon type string.
    // generic();
};

class unit: public generic {
    // Lightweight implementation.
public:
    // all unsigned variables cannot be negative.
    char  stat_bonus[10];
    // Bonuses can be negative -> maluses
    unsigned char current_hp, stats[10], growths[10], wpn_exp[10], position[3],
        equipment[7], weapons[4], items[4], equipped[1], skills[3],
        love_pts[5], love_growths[5];
    unsigned short exp;
    // If equipment is not empty, weapons and items are not used. and vice versa.
    // Equipped is the index of the weapon in unit.equipment.
    char lovers[5][14]; // names of possible lovers.
    bool mounted, flying, armored, promoted;
    unit(std::string, std::string, char,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, std::vector<unsigned int>,
        std::vector<unsigned int>, unsigned short, std::vector<std::string>,
        bool, bool, bool, bool
        );
    ~unit();
    unit();
};

//Unit implementation notes:
    // Equal init time.
    // unit_vec size \= 2 * unit size
    // unit_vec size \= 2 * unit size

class unit_vec {
    // Flexible implementation.
public:
    public:
        std::string name, unit_class;
        std::vector<std::string> lovers;
        unsigned char id, current_hp;
        unsigned short exp;
        std::vector<unsigned char> stats, growths, wpn_exp, position,
                                   equipment, weapons, items, equipped, skills,
                                   love_pts, love_growths; 
        bool mounted, flying, armored, promoted;
    unit_vec(std::string, std::string, char,
            std::vector<unsigned char>, std::vector<unsigned char>,
            std::vector<unsigned char>, std::vector<unsigned char>,
            std::vector<unsigned char>, std::vector<unsigned char>,
            std::vector<unsigned char>, std::vector<unsigned char>,
            std::vector<unsigned char>, std::vector<unsigned char>,
            std::vector<unsigned char>, unsigned short, std::vector<std::string>,
            bool, bool, bool, bool
            );
    unit_vec();
    ~unit_vec();
};

class weapon: public generic {
public:
    unsigned char stats[7];
    char stat_bonus[10], effective[2][14], reserved[5][14];

    ~weapon();
    weapon(std::string, std::string, char,
        std::vector<unsigned char>,  std::vector<char>,
        std::vector<std::string>, std::vector<std::string>
        );
    weapon();
};

// class weapon_vec : public generic {
// public:
// static int wpn_count;
// int id;
// int wpn_type;
// std::vector<int> stats, stat_bonus, growths, range;
// std::vector<std::string> effective;
// weapon_vec(int in_wpn_type,
// std::string in_name,
// std::vector<int> in_stats,
// std::vector<int> in_stat_bonus,
// std::vector<std::string> in_effective);
// // Can either be a unit class, type or any other characteritic. Make a
// // functions that recognizes the correct characteristic.
// weapon_vec();
// };

#endif /* SHARED_H */