#include <stdio.h>
#include <stdarg.h>
#include "shared.h"
char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight"};
char wpn_types[][14]  = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;

int length(char array) {
    size_t n = sizeof(array)/sizeof(array[0]);
    return(n)
}


int main() {
    int Marth[][14] = {{1}, {18, 8, 2, 10, 11, 7, 5, 2, 6, 5}, {1,2}};
    // Everything is arrays.
        // Character array structures:
        // Marth[0] = level
        // Marth[1] = stats
    printf("TESTING THIS BITCH\n");

    // printf("This statt %i \n", equipment_slots);
    printf("This statt %s \n", unit_stats[1]);
    printf("Length of unit stats array %i \n", length(unit_stats));
    
    return(0);
}