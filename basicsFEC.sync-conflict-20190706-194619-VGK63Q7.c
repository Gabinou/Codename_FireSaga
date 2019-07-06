#include <stdio.h>
#include <stdarg.h>
#include "shared.h"
char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight", "uses", "cost"};
char wpn_types[][14]  = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
int equipment_slots = 7;

int len_char(char *array) {
    size_t n = sizeof(array)/sizeof(array[0]);
    return(n);
}

int len_int(int *array) {
    size_t n = sizeof(array)/sizeof(array[0]);
    return(n);
}

int main() {
    int Marth[][14] = {{1}, {18, 8, 2, 10, 11, 7, 5, 2, 6, 5},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {40, 50, 5, 80, 60, 50, 30, 20, 0, 0},
                       {0},  {1,1,1}, {0, 0, 0, 0, 0, 0, 0}, {}};
    int Rapier[][14] = {{4}, {5, 90, 10, 6, 45, 450},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    // Everything is arrays.
        // Character array structures:
        // Marth[0] = id
        // Marth[1] = stats
        // Marth[2] = stat_bonus
        // Marth[3] = growths
        // Marth[4] = EXP
        // Marth[5] = position
        // Marth[6] = equipment
        // Marth[7] = equipped
        // Marth[8] = skills
        // status
        // supports
        //Weapon array structures: 
        // Rapier[0] = id
        // Rapier[1] = stats
        // Rapier[2] = stat_bonus
        


    printf("TESTING THIS BITCH\n");

    // printf("This statt %i \n", equipment_slots);
    printf("This statt %s \n", unit_stats[1]);
    printf("Length of unit stats array %i \n", len_char(*unit_stats));
    size_t n = sizeof(Marth[2])/sizeof(Marth[2]);
    printf("Length of unit stats array %i \n", n);
    
    return(0);
}