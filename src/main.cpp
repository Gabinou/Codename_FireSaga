// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "shared.h"
using namespace std;


#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) // that is a macro. What is a macro?


char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight", "cost", "wpn_exp", "uses"};
char wpn_types[][12]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};
unsigned char wpn_exp_lvls[][14] = {25, 60, 100, 150};
char weapons[][14]  = {"Iron Sword", "Iron Bow"};
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
char unit_classes[][24] = {"Lord", "Prince", "Princess", "Armor Knight", "Cavalier", "Pegasus Knight", "Wyvern Knight", "Axe Rider", "Lance Rider", "Sword Rider", "Mercernary", "Myrmidon", "Soldier", "Archer", "Mage", "Priest", "Priestess", "King", "Paladin", "Sniper", "General", "Bishop", "Hero", "Pirate", "Fighter", "Thief", "Troubadour", "Cleric", "Monk", "Shaman", "Dragon", "Dancer"};
char equipment_slots = 7;
char *all_wpns;
char *all_units;
unsigned char id = 0;

unit::unit(std::string in_name, std::string in_unit_class, char in_id,
           std::vector<unsigned int> in_stats_base, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_equipment,
           std::vector<unsigned int> in_weapons, std::vector<unsigned int> in_items,
           std::vector<unsigned int> in_equipped, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_love_pts, std::vector<unsigned int> in_love_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           unsigned short in_exp, std::vector<std::string> in_lovers,
           bool in_mounted, bool in_flying, bool in_armored, bool in_promoted) {
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
    strncpy(name, in_name.c_str(), sizeof(in_name));
    strncpy(type, in_unit_class.c_str(), sizeof(in_unit_class));
    for (int i = 0; i < in_stats_base.size(); i++) {
        stats_base[i] = (unsigned int) in_stats_base[i];
        growths[i] = (unsigned int) in_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
        stats_bonus[i] = 0;
        // For some reason, stats_base cannot be printed unless (int) Marth.stats_base[0]. + 0 also works.
    }
    current_hp = (unsigned int) stats_base[0];
    for (int i = 0; i < in_weapons.size(); i++) {
        weapons[i] = (unsigned int) in_weapons[i];
        items[i] = (unsigned int) in_items[i];
    }  
    for (int i = 0; i < in_skills.size(); i++) {
        skills[i] = (unsigned int) in_skills[i];
        position[i] = (unsigned int) position[i];
    }  
    for (int i = 0; i < in_equipment.size(); i++) {
        equipment[i] = (unsigned int) in_equipment[i];
    }      
    for (int i = 0; i < in_lovers.size(); i++) {
        strncpy(lovers[i], in_lovers[i].c_str(), sizeof(in_lovers));
        love_growths[i] = (unsigned int) in_love_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
    }    
    id = in_id;
}
unit::unit(){

}
unit_vec::unit_vec(std::string in_name, std::string in_unit_class, char in_id,
           std::vector<unsigned char> in_stats_base, std::vector<unsigned char> in_growths, 
           std::vector<unsigned char> in_equipment,
           std::vector<unsigned char> in_weapons, std::vector<unsigned char> in_items,
           std::vector<unsigned char> in_equipped, std::vector<unsigned char> in_skills,
           std::vector<unsigned char> in_love_pts, std::vector<unsigned char> in_love_growths,
           std::vector<unsigned char> in_wpn_exp, std::vector<unsigned char> in_position,
           unsigned short in_exp, std::vector<std::string> in_lovers,
           bool in_mounted, bool in_flying, bool in_armored, bool in_promoted) {
    name = in_name; 
    unit_class = in_unit_class; 
    id = in_id; 
    stats_base = in_stats_base; 
    stats_grown = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    stats_bonus = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    growths = in_growths; 
    equipment = in_equipment; 
    weapons = in_weapons; 
    items = in_items; 
    equipped = in_equipped; 
    skills = in_skills; 
    love_pts = in_love_pts; 
    love_growths = in_love_growths; 
    wpn_exp = in_wpn_exp; 
    position = in_position; 
    exp = in_exp; 
    // for (int i = 0; i < in_lovers.size(); i++) {
        // strncpy(lovers[i], in_lovers[i].c_str(), sizeof(in_lovers));
    // }
    lovers = in_lovers;
    flying = in_flying; 
    mounted = in_mounted; 
    armored = in_armored; 
    promoted = in_promoted; 
     
}
unit_vec::unit_vec(){
        // name = "test";
}
unit_vec::~unit_vec(void) {
    printf("Unit_vec %s is being deleted.\n", name.c_str());
}
unit::~unit(void) {
   printf("Unit %s is being deleted.\n", name);
}
weapon::weapon(){
    // a
}
weapon::~weapon(void) {
    printf("Weapon %s is being deleted.\n" , name);
}
weapon::weapon(std::string in_name, std::string in_type, char in_id, unsigned short int in_cost,
       std::vector<int> in_stats, std::vector<char> in_stats_bonus,
       std::vector<std::string> in_owner, std::vector<std::string> in_effective) {
    strncpy(name, in_name.c_str(), sizeof(in_name));
    strncpy(type, in_type.c_str(), sizeof(in_type));
    for (int i = 0; i < in_effective.size(); i++) {
        strncpy(effective[i], in_effective[i].c_str(), sizeof(in_effective));
    }
    for (int i = 0; i < in_owner.size(); i++) {
        strncpy(owner[i], in_owner[i].c_str(), sizeof(in_owner[i]));
    }
    for (int i = 0; i < in_stats_bonus.size(); i++) {
        stats_bonus[i] = (int) in_stats_bonus[i];
    }
    for (int i = 0; i < in_stats.size(); i++) {
        stats[i] = in_stats[i];
    }
    cost = in_cost;
}

main() {
    printf("TESTING THIS BITCH\n");
    printf("Initializaing a character\n");
    
    unordered_map<string, weapon> all_weapons;
    
    
    // weapon Rapier("Rapier", "swd", id++, 600,
                  // { 5, 90, 10,  7, 30,  2,  2},
                  // std::vector<char>(LEN(unit_stats), 0), {"Marth"}, {"Knight", "Cavalier"});
    all_weapons["Rapier"] = weapon("Rapier", "swd", id++, 600,
                  { 5, 90, 10,  7, 30,  2,  2},
                  std::vector<char>(LEN(unit_stats), 0), {"Marth"}, {"Knight", "Cavalier"});
    printf("Does unordered_map allocation work? %s\n", all_weapons["Rapier"].name);
    printf("Does unordered_map allocation work? %d\n", all_weapons["Rapier"].stats[0]);
    return(0);    
    weapon Bronze_swd("Bronze Sword", "swd", id++, 450,
                  {3, 80,  0,  5, 45,  0,  1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    weapon Iron_swd("Iron Sword", "swd", id++, 450,
                  {5, 80,  0,  7, 45,  0,  1}, 
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    weapon Iron_lnc("Iron Lance", "lance", id++, 450,
                  {6, 80,  0,  8, 40,  0,  1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    weapon Steel_swd("Steel Sword", "swd", id++, 500,
                  {8, 70,  0,  9,  35, 1,  1}, 
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    weapon Damascus_swd("Damascus Sword", "swd", id++, 1000,
                  {12, 65,  0,  8, 25,  3,  1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    /* CLONING: 
    *   WEAPONS:
    * idea for lightness and foregoing cloning weapons: In equipment,
    * just make a list {"weapon_name", uses_left}. Then refer to weapons
    * by name for dealing with combat and stuff. No need for cloning weapons,
    * just refer to the eternal and unchanging weapons. Use the constant
    * weapon stats_bonus and add to the character stats_bonus, which does change.
    * So essentially make items immutable. 
    *    CHARACTERS
    What about characters? I think characters need one object per filesave. then modify this object as the game evolves.


    */

    unit Marth("Marth", "Prince", id++, 
        //HP  Str Mag Skl Spd Lck Def Res Con Mov
        /*stats_base*/           {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Growths*/         {90,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Equipment*/       {18,  8,  2,  9, 10,  7,  5,},
        /*Weapons*/         {18,  8,  2,  9,},
        /*Items*/           {18,  8,  2,  9,},
        /*Equipped*/        {18},
        /*Position*/        {18,1,1},
        /*Love_pts*/        {18,1,1 ,1,1},
        /*Love_growths*/    {18,1,1 ,1,1},
        /*Weapon_exp*/      {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Skills*/          {18,  8,  2},
        /*Exp*/             0, 
        /*Lovers*/          {"Sheeda", "" ,"","",""},
        /*Mounted*/         0,
        /*Flying*/          0,
        /*Armored*/         0,
        /*MPromoted*/       0
    );
    
    unit Marths[10];
    std::vector<unit> Marths_vec;
    printf(" Size of arrays of Marths %d\n", sizeof(Marths));
    printf(" Size of arrays of Marths_vec %d\n", sizeof(Marths_vec));
    return(0);
    cout << sizeof(Marth) << "bytes" << endl; 
    unit_vec Marth_vec("Marth2", "Prince", id++, 
    {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
    {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
    {18,  8,  2,  9, 10,  7,  5,},
    {18,  8,  2,  9,},
    {18,  8,  2,  9,},
    {18},
    {18,1,1},
    {18,1,1 ,1,1},
    {18,1,1 ,1,1},
    {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
    {18,  8,  2},
    0, 
    {"Sheeda", "" ,"","",""},
    0, 0, 0, 0
    );

    cout << sizeof(Marth_vec) << "bytes" << endl; 

}
