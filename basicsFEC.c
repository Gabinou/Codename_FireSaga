#include <stdio.h>
#include <stdarg.h>
#include<windows.h>
#include "shared.h"

// use char for everything cause it reduces memory, and I like the design: small numbers for damage, armor, etc.
char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight"};
char wpn_types[][14]  = {"swd", "lance", "axe","bow", "mgc_wind", "mgc_dark", "mgc_fire", "mgc_thunder", "staff"};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Knight", "Cavalier", "Pegasus Knight", "Wyvern knight"};
char equipment_slots = 7;
// char *all_characters;
// char *all_weapons;
char id = 0;

char len_char(char *array) {
    size_t length = sizeof(array)/sizeof(array[0]);
    return(length);
}

char attack(char unit[][14]){
    char value = unit[1][1];
    
    return(value);
    // return(0);
}

char get_object(char id) {
    all_weapons[0][1];
    return(0);
};




int main() {
    char Marth[][14] = {{id++}, {18, 8, 2, 10, 11, 7, 5, 2, 6, 5},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {40, 50, 5, 80, 60, 50, 30, 20, 0, 0},
                       {0},  {1,1,1}, {0, 0, 0, 0, 0, 0, 0}, {}};
    
    char *all_characters[] = {*Marth};
    
    char Rapier[][14] = {{id++}, {5, 90, 10, 6, 45, 45},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}};
    char *all_weapons[] = {*Rapier};                  
                        
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
    char *p = Marth[1];
    printf("This is the computed attack value: %d\n ", attack(Marth));
    
    // printf("This statt %i \n", equipment_slots);
    printf("This statt %s \n", unit_stats[1]);
    printf("Length of unit stats array %i \n", len_char(*unit_stats));
    size_t n = sizeof(Marth[2])/sizeof(Marth[2]);
    printf("Length of unit stats array %i \n", n);
    printf("Length of unit stats array %i \n", n+1);
    printf("Equipment slots %i \n", equipment_slots);
    printf("Equipment slots %i \n", equipment_slots+1);
    printf("Current Id %i \n", id);
    // can do math on a char!
    
    return(0);
}