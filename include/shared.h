#ifndef SHARED_H
#define SHARED_H

extern char unit_stats[][14];
extern char unit_stats[][14];
extern char weapon_stats[][14];
extern char wpn_types[][14];
extern char weapons[][14];
extern char statuses[][14];
extern char unit_classes[][24];
extern char equipment_slots;
extern char *all_units;
extern char *all_wpns;
extern unsigned char id; 

struct unit {
    char name[14];
    char classs[14];
    // all unsigned variables cannot be negative.
    unsigned char id; 
    unsigned char stats[10];
    char stat_bonus[10]; //Bonuses can be negative -> maluses
    unsigned char growths[10];
    unsigned short exp;
    unsigned char wpn_exp[10];
    unsigned char position[3];
    unsigned char equipment[7]; // If equipment is not empty, weapons and items are not used.
    unsigned char weapons[4]; // If weapons or items are not empty, equipment is not used.
    unsigned char items[4];
    // Equipped is the index of the weapon in unit.equipment.
    unsigned char equipped;
    unsigned char skills[3]; // skills id.
    char lovers[5][14]; // names of possible lovers. 
    unsigned char love_pts[5];
    unsigned char love_growths[5];
    bool mounted;
    bool flying;
    bool armored;
    bool promoted;
};

struct weapon {
    char name[14];
    unsigned char id;
    unsigned char stats[7];
    char stat_bonus[10];
    char effective[][14];
};

#endif /* SHARED_H */