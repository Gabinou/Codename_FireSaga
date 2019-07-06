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
extern char id; 

struct unit {
    char id;
    char stats[10];
    char stat_bonus[10];
    char growths[10];
    short exp;
    char position[3];
    char equipment[7];
    char equipped;
    char skills[3];
};
