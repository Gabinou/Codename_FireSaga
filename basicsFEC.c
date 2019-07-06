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
char *all_wpns;
char *all_units;
// char *all_characters;
// char *all_weapons;
char id = 0;

char len_char(char *array) {
    size_t length = sizeof(array)/sizeof(array[0]);
    return(length);
}



char get_object(char id, char **list) {
    printf("Wanted id %i \n", id);
    
    return(0);
};

char attack(char unit[][14]){
    char value = unit[1][1];
    // get_object(unit[7][0], all_wpns);
    return(value);
    // return(0);
}



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
int main() {
    printf("TESTING THIS BITCH\n");
    char Marth[][14] = {{id++}, {17, 6, 4, 7, 12, 8, 5, 4, 5, 7},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {40, 50, 5, 80, 60, 50, 30, 20, 0, 0},
                       {0},  {1,1,1}, {0, 0, 0, 0, 0, 0, 0}, {1}};  
   char Sheeda[][14] = {{id++}, {17, 6, 4, 7, 12, 8, 5, 4, 5, 7},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {40, 50, 5, 80, 60, 50, 30, 20, 0, 0},
                       {0},  {1,1,1}, {0, 0, 0, 0, 0, 0, 0}, {1}};  
    struct unit ooMarth = {id++,
                           18, 08, 02, 09, 10, 07, 05, 02, 06, 05,
                           00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
                           40, 50, 05, 80, 60, 50, 30, 20, 00, 00,
                           00,
                           001, 001, 001,
                           00, 00, 00, 00, 00, 00, 00,
                           01,
                           01, 02, 03};
    // ooMarth.id = id++;
    printf("id of Marth %d\n", Marth[0][0]);
    printf("id of ooMarth %d\n", ooMarth.id); 
    printf("id of ooMarth %d\n", ooMarth.stats[0]); 
    printf("id of ooMarth %d\n", ooMarth.stats[8]); 
    printf("id of ooMarth %d\n", ooMarth.stat_bonus[0]); 
    printf("id of ooMarth %d\n", ooMarth.growths[8]); 
    printf("id of ooMarth %d\n", ooMarth.exp); 
    printf("id of ooMarth %d\n", ooMarth.position[1]); 
    printf("id of ooMarth %d\n", ooMarth.skills[2]); 
    char Sheeda[][14] = {{id++}, {17, 6, 4, 7, 12, 8, 5, 4, 5, 7},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {40, 50, 5, 80, 60, 50, 30, 20, 0, 0},
                       {0},  {1,1,1}, {0, 0, 0, 0, 0, 0, 0}, {1}};    
  
    printf("Size of Array Marth %d\n", sizeof(Sheeda));
    printf("Size of struct Marth %d\n", sizeof(ooMarth) );
    
    return(0);

    
    char *all_units[1] = {Marth[0]};
    
    char Rapier[][14] = {{id++}, {5, 90, 10, 6, 45, 45},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    char Iron_swd[][14] = {{id++}, {5, 80, 0, 7, 50, 50},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
                        
    char *all_wpns[2] = {Rapier[0], Iron_swd[0]};    

    printf("This is the allwpns: %p\n ", all_wpns[0]);
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