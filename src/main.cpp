// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle main.cpp
* \file main.cpp 
* \brief Main game algorithm page.
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h> 
#include "shared.h"
using namespace std;
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) 
/// \fn LEN(arr)
/// \brief That is a macro. What is a macro? Anyway it computes lenght of arrays.


char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
/*! \var char unit_stats
* \brief Unit Stats dmake escription: <br>
*  - HP: Hit Points. Unit health. Unit dies (forever) if it goes down to zero. <br>
*  - Str: Strength. +1 Str -> +1 Attack. Refer to ... <br>
*  - Mag: Magic. +1 Mag -> +1 Attack. Refer to ... <br>
*  - Skill: Higher skill means higher Hit rate and Avoid Rate. Refer to ...  <br>
*  - Speed: Higher speeds lead to double strikes. Refer to ...  <br>
*  - Luck: Increases hit, avoid, decreases enemy crit chance, etc. <br>
*  - Def: Defens.: +1 Def -> -1 Received Damage from physical attacks <br>
*  - Res: Magic Resistance. +1 Red -> -1 Received Damage from magic attacks. <br>
*  - Con: Constitution. Only a unit with a higher constitution can rescue another. Refer to... <br>
*  - Move: Distance, in squares, taht a unit can move on the grid map. Refer to ... <br>
*/
char weapon_stats[][14] = {"dmg", "hit", "crit", "weight", "wpn_exp", "uses", "range", "lvl"}; 
/*! \var char weapon_stats
* \brief Weapon Stats description: <br>
*  - dmg: Damage. +1 dmg -> +1 Attack. Refer to... <br>
*  - hit: Hit chance. % probability value to hit. Refer to ... <br>
*  - crit: Crit chance. % probability value to crit. Refer to...  <br>
*  - wpn_exp: Weapon experience. Base weapon experience for using the weapon. Refer to...  <br>
*  - uses: Number of uses a weapon has before breaking.  <br>
*  - range: distance in squares to which a unit can attack with weapon.  <br>
*  - lvl: Weapon level. Integer representing the weapon experience/level necessary to use the weapon. Refer to ...  <br>
*/
char wpn_types[][12]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};
/// \var char wpn_types
/// \brief Weapon types.
unsigned char wpn_exp_lvls[][14] = {25, 60, 100, 150};
/*! \var char wpn_exp_lvls
* \brief Weapon Experience levels <br>
*  Weapon Experience necessary to go up a weapon level. <br>
*/
char statuses[][14] = {"healthy", "sleep", "poison", "stone", "berserk"};
/*! \var char statuses
*   \brief Statuses. Affects units for 5 turns each, except for stone, which is forever. *DESIGN QUESTION*
*  - Healthy. <br>
*  - Sleep: Unit cannot move, attack, or retaliate. Unit wakes up after being attacked. Refer to ... <br>
*  - Poison: Loses HP every turn. (How much?) <br>
*  - Stone: Unit cannot move, attack or retaliate. Can only be cured by the ... staff. <br>
*  - Berserk. Unit attacks the weakest unit in range. Friend or foe. *DESIGN QUESTION* <br>
*/

char unit_classes[][24] = {"Lord", "Prince", "Princess", "Armor Knight", "Cavalier", "Pegasus Knight", "Wyvern Knight", "Axe Rider", "Lance Rider", "Sword Rider", "Mercernary", "Myrmidon", "Soldier", "Archer", "Mage", "Priest", "Priestess", "King", "Paladin", "Sniper", "General", "Bishop", "Hero", "Pirate", "Fighter", "Thief", "Troubadour", "Cleric", "Monk", "Shaman", "Dragon", "Dancer"};
/// \var char unit_classes
/// \brief Unit classes.
char all_weapon_names[][20] = {"Bronze Sword", "Iron Sword", "Steel Sword", "Rapier"};
/// \var char all_weapon_names
/// \brief All Weapon Names.
char all_unit_names[][14] = {"Marth", "Sheeda"};
/// \var char all_unit_names
/// \brief All Unit Names.
// Other names: Acier de Damas. Damas Sword. Damascus Sword. Damas Sword. Damas steel sword.
    
char equipment_slots = 7;
/// \var char equipment_slots
/// \brief Total number of equipment slots. Only used for an inventory that mixes weapons and items.
char item_slots = 7;
/// \var char item_slots
/// \brief Number of item slots. Separate from weapon slots.
char weapon_slots = 7;
/// \var weapon_slots 
/// \brief Number of weapon slots. Separate from weapon slots.

unsigned char id = 0;
/// Item slots. Separate from weapon slots.

generic::generic() {

}

// generic::generic(const generic &obj) {
   // printf("Copy constructor allocating ptr.\n");
   // ptr = new int;
   // *ptr = *obj.ptr; // copy the value
// }

unit::unit(std::string in_name, std::string in_unit_class, char in_id,
           std::vector<unsigned int> in_stats_base, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_love_pts, std::vector<unsigned int> in_love_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           std::vector<inventory_item> in_equipped, 
           std::vector<inventory_item> in_equipment,
           std::vector<inventory_item> in_weapons, 
           std::vector<inventory_item> in_items,
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
        if (in_weapons.size() != 0) {
            weapons[i] = in_weapons[i];
        };
        if (in_items.size() != 0) {
            items[i] = in_items[i];
        };
    }  
    for (int i = 0; i < in_skills.size(); i++) {
        skills[i] = (unsigned int) in_skills[i];
        position[i] = (unsigned int) position[i];
    }  
    for (int i = 0; i < in_equipment.size(); i++) {
        equipment[i] = in_equipment[i];
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
unsigned char unit::attack(const unit& enemy) {
    return(0);
    // return(enemy.stats[6]);
}
generic::~generic(void) {
    // printf("Generic object is being deleted\n");
}
unit::~unit(void) {
   // printf("Unit %s is being deleted.\n", name.c_str());
}
weapon::weapon(){
    // a
}
weapon::~weapon(void) {
    // printf("Weapon %s is being deleted.\n" , name);
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

inventory_item::inventory_item() {

}

inventory_item::inventory_item(std::string in_name, char in_used){
    strcpy(name, in_name.c_str());
    used = in_used;
}


inventory_item::~inventory_item(void) {
    // printf("Weapon %s is being deleted.\n" , name);
}
inventory_item::inventory_item(const inventory_item& source) {
    strcpy(name, source.name);
    used = source.used;
}
/// \fn main 
/// \brief Main FEmaker algorithm.
main() {
    printf("TESTING THIS BITCH\n");
    printf("Initializaing a character\n");
    
    unordered_map<string, weapon> all_weapons;
    unordered_map<string, struct inventory_item> inventory_items;
    unordered_map<string, unit> all_units;
    
    // Unordered map convention: "name" is the immutable original object.
    // Copies have "name_id"
    
    /* CLONING: 
    *   WEAPONS:
    * idea for lightness and foregoing cloning weapons: In equipment,
    * just make a list {"weapon_name", uses_left}. Then refer to weapons
    * by name for dealing with combat and stuff. No need for cloning weapons,
    * just refer to the eternal and unchanging weapons. Use the constant
    * weapon stats_bonus and add to the character stats_bonus, which does change.
    * So essentially make items immutable. 
    *  Maybe call this variable: weapons_reference.
    *    CHARACTERS
    What about characters? I think characters need one object per filesave. then modify this object as the game evolves.


    */
    all_weapons["Rapier"] = weapon("Rapier", "swd", id++, 600,
                // dmg  hit  crt wght uses  exp range
                  {5,  90,  10,   7,  30,   2,  1},
                  std::vector<char>(LEN(unit_stats), 0), {"Marth"}, {"Knight", "Cavalier"}); 
    all_weapons["Bronze Sword"] = weapon("Bronze Sword", "swd", id++, 450,
               // dmg  hit  crt wght uses  exp range
                  {3,  80,   0,   5,  45,   1,   1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    all_weapons["Iron Sword"] =  weapon("Iron Sword", "swd", id++, 450,
               // dmg hit  crt wght uses  exp range
                  {5,  80,    0,   7,  45,   1,   1}, 
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    all_weapons["Iron Lance"] = weapon("Iron Lance", "lance", id++, 450,
               // dmg hit  crt wght uses  exp range
                  {6,  80,   0,   8,   40,  1,   1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    all_weapons["Steel Sword"] = weapon("Steel Sword", "swd", id++, 500,
               // dmg hit  crt wght uses  exp range
                  {8,  70,   0,   9,   35,  1,  1}, 
                  std::vector<char>(LEN(unit_stats), 0), {}, {});
    all_weapons["Lame de Damas"] = weapon("Lame de Damas", "swd", id++, 1000,
                // dmg hit  crt wght uses  exp range
                  {12,  65,  0,   8,  25,   1,  1},
                  std::vector<char>(LEN(unit_stats), 0), {}, {});

    for (int i = 0; i < sizeof all_weapon_names / sizeof all_weapon_names[0]; i++) {
        inventory_items[all_weapon_names[i]] =  inventory_item(all_weapon_names[i], 10);
    }
    inventory_items["Empty"] = inventory_item("Empty", 0);
    
    printf("TAGUEULE CONNASSE %s\n", all_weapon_names[0]);
    printf("TAGUEULE CONNASSE %s\n", inventory_items["Empty"].name);
    printf("TAGUEULE CONNASSE %s\n", inventory_items["Bronze Sword"].name);
    printf("TAGUEULE CONNASSE %s\n", inventory_items["Iron Sword"].name);
    printf("TAGUEULE CONNASSE %s\n", inventory_items["Steel Sword"].name);
    printf("TAGUEULE CONNASSE %d\n", inventory_items["Iron Sword"].used); 
    // inventory_items["Rapier"] = inventory_items["Rapier"];
    // inventory_items["Rapier1"] = inventory_items["Rapier"];
    
    // std::cout << inventory_items["Iron Sword"] << endl; 
    unit Marth("Marth", "Prince", id++, 
                            //HP Str Mag Skl Spd Lck Def Res Con Mov
        /*stats_base*/      {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Growths*/         {90,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Skills*/          {18,  8,  2},
        /*Love_pts*/        {18,1,1 ,1,1},
        /*Love_growths*/    {18,1,1 ,1,1},
        /*Weapon_exp*/      {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Position*/        {18,1,1},
        /*Equipped*/        {inventory_items["Rapier1"]},
        /*Equipment*/       {inventory_items["Rapier1"]},
        /*Weapons*/         {},
        /*Items*/           {},
        /*Exp*/             0, 
        /*Lovers*/          {"Sheeda", "" ,"","",""},
        /*Mounted*/         0,
        /*Flying*/          0,
        /*Armored*/         0,
        /*Promoted*/        0
    );
    
    // unit Marths[10];
    // std::vector<unit> Marths_vec;
    // printf(" Size of arrays of Marths %d\n", sizeof(Marths));
    // printf(" Size of arrays of Marths_vec %d\n", sizeof(Marths_vec));
    // return(0);
    // cout << sizeof(Marth) << "bytes" << endl; 

    // cout << sizeof(Marth_vec) << "bytes" << endl; 

}
