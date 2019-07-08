// Flame Ensign maker, abbreviated as FEmaker
// Code créé par Gabriel Taillon en Juin 2019.
#include "shared.h" //Include note: "" means search in current folder, then in pre-designated source dirs
#include <stdarg.h>
#include <stdio.h> //Include note: <> means search in pre-designated compiler source dirs for standard libs
#include <windows.h>

// use char for everything cause it reduces memory, and I like the design: small
// numbers for damage, armor, etc.
char unit_stats[][14] = {"HP",   "Str", "Mag", "Skill", "Speed",
                         "Luck", "Def", "Res", "Con",   "Move"};
char weapon_stats[][14] = {"dmg",  "hit",     "crit", "weight",
                           "cost", "wpn_exp", "uses"};
char wpn_types[][14] = {"swd",       "lance",    "axe",         "bow",
                        "mgc_wind",  "mgc_fire", "mgc_thunder", "mgc_dark",
                        "mgc_light", "staff"};
unsigned char wpn_exp_lvls[][14] = {25, 60, 100, 150};
char weapons[][14] = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord",         "Prince",   "Princess",
                           "Knight",       "Cavalier", "Pegasus Knight",
                           "Wyvern knight"};
char equipment_slots = 7;
char *all_wpns;
char *all_units;
// char *all_characters;
// char *all_weapons;
unsigned char id = 0;

char len_char(char *array) {
  size_t length = sizeof(array) / sizeof(array[0]);
  return (length);
}

char get_object(char id, char **list) {
  printf("Wanted id %i \n", id);

  return (0);
};

char attack(char unit[][14]) {
  char value = unit[1][1];
  // get_object(unit[7][0], all_wpns);
  return (value);
  // return(0);
}

int main() {
  printf("TESTING THIS BITCH\n");
  typedef struct Rapiers {
    Weapon weapon;
    // name = "Rapier";
    // Rapier.name = "Rapier";
    // struct weapon {"Rapier", id++,
    // //   Dmg Hit Crt Wgt Gld Exp Use
    // /*Weapon Stats*/    5, 80, 10,  5, 45, 10, 45,
    // // HP  Str Mag Skl Spd Lck Def Res Con Mov
    // /*Stat bonuses*/    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,};
    // struct weapon Iron_lance = {"Iron Lance", id++,
    // //   Dmg Hit Crt Wgt Gld Exp Use
    // /*Weapon Stats*/    7, 75, 0,  6, 50, 10, 50,
    // // HP  Str Mag Skl Spd Lck Def Res Con Mov
    // /*Stat bonuses*/    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,};
  } Rapier;
  // Rapier Rapier1;

  // struct weapon Rapier1 = Rapier; //enough to make deepcopies
  // strcpy(Rapier1.name, "Marthss");
  // printf("Rapier name %s\n", Rapier.name);
  // printf("Rapier id %d\n", Rapier.id);
  // printf("Rapier1 name %s\n", Rapier1.weapon.name);
  printf("Rapier1 name \n");
  // printf("Rapier1 id %d\n", Rapier1.id);
  // printf("Iron_Lance id %d\n", Iron_lance.id);

  struct unit Sheeda = {"Sheeda", "Pegasus Knight", id++,
                        // HP  Str Mag Skl Spd Lck Def Res Con Mov
                        /*Stats*/ 17, 7, 3, 8, 12, 9, 4, 4, 5, 7,
                        /*Stat bonuses*/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        /*Growths*/ 30, 40, 5, 60, 70, 50, 20, 30, 0, 0,
                        /*Total exp*/ 0,
                        // swd lnc axe bow wnd tnd fir drk lgt stf
                        /*Weapon exp*/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        /*Position*/ 1, 1, 1,
                        /*Equipment*/ 0, 0, 0, 0, 0, 0, 0,
                        /*Weapons*/ 0, 0, 0, 0,
                        /*Items*/ 0, 0, 0, 0,
                        /*Equipped*/ 1,
                        /*Skills*/ 1, 2, 3,
                        /*Lovers*/ "Marth", "", "", "", "",
                        /*Love Points*/ 30, 0, 0, 0, 0,
                        /*Love growths*/ 2, 0, 0, 0, 0,
                        /*Mounted*/ 0,
                        /*Armored*/ 0,
                        /*Flying*/ 0,
                        /*Promoted*/ 0};

  struct unit Marth = {"Marth", "Prince", id++,
                       // HP  Str Mag Skl Spd Lck Def Res Con Mov
                       /*Stats*/ 18, 8, 2, 9, 10, 7, 5, 2, 6, 5,
                       /*Stat bonuses*/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       /*Growths*/ 40, 50, 5, 80, 60, 50, 30, 20, 0, 0,
                       /*Total exp*/ 0,
                       // swd lnc axe bow wnd tnd fir drk lgt stf
                       /*Weapon exp*/ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       /*Position*/ 1, 1, 1,
                       /*Equipment*/ 0, 0, 0, 0, 0, 0, 0,
                       /*Weapons*/ 0, 0, 0, 0,
                       /*Items*/ 0, 0, 0, 0,
                       /*Equipped*/ 1,
                       /*Skills*/ 1, 2, 3,
                       /*Lovers*/ "Sheeda", "", "", "", "",
                       /*Love Points*/ 25, 0, 0, 0, 0,
                       /*Love growths*/ 2, 0, 0, 0, 0,
                       /*Mounted*/ 0,
                       /*Armored*/ 0,
                       /*Flying*/ 0,
                       /*Promoted*/ 0};
  if (Marth.promoted) {
    printf("He is promoted.\n");
  };
  printf("id of Marth %d\n", Marth.id);
  printf("id of Marth %d\n", Marth.stats[0]);
  printf("id of Marth %d\n", Marth.stats[8]);
  printf("id of Marth %d\n", Marth.stat_bonus[0]);
  printf("id of Marth %d\n", Marth.growths[8]);
  printf("id of Marth %d\n", Marth.exp);
  printf("id of Marth %d\n", Marth.position[1]);
  printf("id of Marth %d\n", Marth.skills[2]);
  printf("id of Marth %s\n", Marth.lovers[0]);

  // printf("This is the allwpns: %p\n ", all_wpns[0]);
  // printf("This is the computed attack value: %d\n ", attack(Marth));

  return (0);
}
