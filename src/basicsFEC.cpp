#include <stdio.h>
char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con"};
char unit_stats[][14] weapon_stats = {"dmg", "hit", "crit", "weight"};
char unit_stats[][14] wpn_types = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
char unit_stats[][14] weapons = {"Iron Sword", "Iron Bow"};
char unit_stats[][14] statuses = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_stats[][14] unit_classes = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;


main() {
    printf("TESTING THIS BITCH\n");

    printf("This statt %s \n", unit_stats[1]);
    
}