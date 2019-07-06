#include <stdio.h>
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
    unsigned char id;
    unsigned char stats[10];
    char stat_bonus[10];
    unsigned char growths[10];
    unsigned short exp;
    unsigned char wpn_exp[9];
    unsigned char position[3];
    unsigned char equipment[7];
    unsigned char equipped;
    unsigned char skills[3];
    unsigned char lovers[5][14];
    unsigned char love_pts[5];
    unsigned char love_growths[5];
};

struct weapon {
    char name[14];
    char id;
    char stats[10];
    char stat_bonus[10];
};
