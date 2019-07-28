// Code créé par Gabriel Taillon
// Note on standards. Fuck all that complicated bullcrap. I'm writing for c++11. More convenient and the rest seems like bullshit.
/*! \page handle main.cpp
* \file main.cpp 
* \brief Main game algorithm page.
*/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h> 
#include <random>
#include <bits/stdc++.h> 
#include "shared.h"

// Skill idea: divine shield! Bubble!
std::mt19937 mt(1899); //Deterministic seed. Do like other fire Emblems. Always Same RNG,it is the player actions that change it.
std::uniform_int_distribution<char> dist(0, 100); //*DESIGN QUESTION* What should be the minimum and maximum probabilities?

/// \fn int get_rand
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
int get_rand() { 
    return(dist(mt));
}

/// \fn bool single_roll
/// \brief Check if event occurs using single RNG roll: if rand<prob, event occurs. True to probabilities, but humans think it is weird.
bool single_roll(int in_prob) {
    bool out = (get_rand() < in_prob);
    return(out);
}
/// \fn bool double_roll
/// \brief Check if event occurs using double RNG roll: if mean of 2 random numbers is lwer than probability of event, it occurs. Skews probabilities, but fits with humans biases.
bool double_roll(int in_prob) {
    int rng1 = get_rand();
    int rng2 = get_rand(); 
    printf("%i %i %i\n", rng1, rng2, in_prob);
    bool out = ((rng1 + rng2) < (2 * in_prob));
    return(out);
}

char unit_stats[][14] = {"HP", "Str", "Mag", "Skill", "Speed", "Luck", "Def", "Res", "Con", "Move"};
/*! \var char unit_stats
* \brief Unit Stats dmake escription: <br>
*  - HP: Hit Points. Unit health. Unit dies (forever) if it goes down to zero. <br>
*  - Str: Strength. +1 Str -> +1 attack_damage. Refer to ... <br>
*  - Mag: Magic. +1 Mag -> +1 attack_damage. Refer to ... <br>
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
*  - dmg: Damage. +1 dmg -> +1 attack_damage. Refer to... <br>
*  - hit: Hit chance. % probability value to hit. Refer to ... <br>
*  - crit: Crit chance. % probability value to crit. Refer to...  <br>
*  - wpn_exp: Weapon experience. Base weapon experience for using the weapon. Refer to...  <br>
*  - uses: Number of uses a weapon has before breaking.  <br>
*  - range: distance in squares to which a unit can attack_damage with weapon.  <br>
*  - lvl: Weapon level. Integer representing the weapon experience/level necessary to use the weapon. Refer to ...  <br>
*/
// char wpn_types[][12]  = {"swd", "lance", "axe", "bow", "mgc_wind", "mgc_fire", "mgc_thunder",  "mgc_dark",  "mgc_light", "staff"};
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
*  - Sleep: Unit cannot move, attack_damage, or retaliate. Unit wakes up after being attacked. Refer to ... <br>
*  - Poison: Loses HP every turn. (How much?) <br>
*  - Stone: Unit cannot move, attack_damage or retaliate. Can only be cured by the ... staff. <br>
*  - Berserk. Unit attacks the weakest unit in range. Friend or foe. *DESIGN QUESTION* <br>
*/

char unit_classes[][24] = {"Lord", "Prince", "Princess", "Armor Knight", "Cavalier", "Pegasus Knight", "Wyvern Knight", "Axe Rider", "Lance Rider", "Sword Rider", "Mercernary", "Myrmidon", "Soldier", "Archer", "Mage", "Priest", "Priestess", "King", "Paladin", "Sniper", "General", "Bishop", "Hero", "Pirate", "Fighter", "Thief", "Troubadour", "Cleric", "Monk", "Shaman", "Dragon", "Dancer"};
/// \var char unit_classes
/// \brief Unit classes.
// char all_weapon_names[][20];
/// \var char all_weapon_names
/// \brief All Weapon Names.
char all_unit_names[][14] = {"Marth", "Sheeda"};
/// \var char all_unit_names
/// \brief All Unit Names.
    
char equipment_slots = 7;
/// \var char equipment_slots
/// \brief Total number of equipment slots. Only used for an inventory that mixes weapons and items.
char item_slots = 4;
/// \var char item_slots
/// \brief Number of item slots. Separate from weapon slots.
char weapon_slots = 4;
/// \var weapon_slots 
/// \brief Number of weapon slots. Separate from weapon slots.

generic::generic() {

}

generic::~generic(void) {
    // printf("Generic object is being deleted\n");
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
std::unordered_map<string, weapon> all_weapons;
std::unordered_map<string, struct inventory_item> inventory_items;
std::unordered_map<string, unit> all_units;



/// \fn void write_all_units
/// \brief Write all_units stats to file.
void write_all_units(const char *filename, char const *savestyle = "cpp" ) {
    if (savestyle == "cpp") {
        std::ofstream out(filename);
        for (int i=0; i < sizeof(all_unit_names)/sizeof(all_unit_names[0]); i++){
            out << all_units[all_unit_names[i]];
        }
        out.close();
    } else if (savestyle == "c") {
        remove(filename);
        for (int i=0; i < sizeof(all_unit_names)/sizeof(all_unit_names[0]); i++){
            all_units[all_unit_names[i]].write(filename);
        }
    }
}

main() {
    printf("TESTING THIS BITCH\n");
    printf("TESTING THIS BITCH\n");
    printf("Initializaing a character\n");

    all_weapons["Rapier"] = weapon("Rapier", "swd", id++, 600,
            // dmg  hit  crt wght uses  exp
              {5,  90,  10,   7,  30,   2},
              {1}, // range
              std::vector<char>(LEN(unit_stats), 0), {"Marth"}, {"Knight", "Cavalier"}, 0); 
    all_weapons["Bronze Sword"] = weapon("Bronze Sword", "swd", id++, 450,
           // dmg  hit  crt wght uses  exp
              {3,  80,   0,   5,  45,   1},
              {1}, // range
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
    all_weapons["Iron Sword"] =  weapon("Iron Sword", "swd", id++, 450,
           // dmg hit  crt wght uses  exp
              {5,  80,    0,   7,  45,  1},
              {1}, // range              
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
    all_weapons["Iron Lance"] = weapon("Iron Lance", "lance", id++, 450,
           // dmg hit  crt wght uses  exp
              {6,  80,   0,   8,   40,  1},
              {1}, // range
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
    all_weapons["Steel Sword"] = weapon("Steel Sword", "swd", id++, 500,
           // dmg hit  crt wght uses  exp
              {8,  70,   0,   9,   35,  1},
              {1}, // range              
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
    all_weapons["Lame de Damas"] = weapon("Lame de Damas", "swd", id++, 1000,
            // Other names: Acier de Damas. Damas Sword. Damascus Sword. Damas Sword. Damas steel sword.
            // dmg hit  crt wght uses  exp
              {15,  65,  0,   8,  25,   1},
              {1}, // range
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
    // Unordered map convention: "name" is the immutable original object.
    // Copies have "name_id"
    
    std::unordered_map<std::string, weapon>::iterator it = all_weapons.begin();
    while(it != all_weapons.end()) {
        char key[(it->first).size() + 1];
        strcpy(key, (it->first).c_str());
        inventory_items[strcat(key,"_0001")] =  inventory_item(key, 10);
        it++;
    }
    
    // unit Marth("Marth", "Prince", id++, 
                            // //HP Str Mag Skl Spd Lck Def Res Con Mov
        // /*stats_base*/      {18,  1,  2,  9, 10,  7,  5,  2,  6,  5},
        // /*Growths*/         {90,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        // /*Skills*/          {18,  8,  2},
        // /*Love_pts*/        {18,1,1 ,1,1},
        // /*Love_growths*/    {18,1,1 ,1,1},
        // /*Weapon_exp*/      {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        // /*Position*/        {18,1,1},
        // /*Equipped*/        {0},
        // /*Equipment*/       {inventory_items["Rapier_0001"]},
        // /*Weapons*/         {},
        // /*Items*/           {},
        // /*Exp*/             0, 
        // /*Lovers*/          {"Sheeda", "" ,"","",""},
        // /*Mounted*/         0,
        // /*Flying*/          0,
        // /*Armored*/         0,
        // /*Promoted*/        0
    // );    
    
    all_units["Marth"] = unit("Marth", "Prince", id++, 
                            /*HP Str Mag Skl Spd Lck Def Res Con Mov
        stats_base*/        {18,  8,  2,  9, 10,  7,  5,  2,  6,  5},
        /*Growths*/         {60, 50, 20, 70, 60, 40, 30, 20,  0,  0},
        /*Skills*/          {18,  8,  2},
        /*Love_pts*/        { 0,  0,  0,  0,  0},
        /*Love_growths*/    { 0,  0,  0,  0,  0},
                           //swd lnc axe bow wnd firetnd drk lgt staff
        /*Weapon_exp*/      { 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /*Position*/        { 1,  1,  1},
        /*Equipped*/        {0},
        /*Equipment*/       {inventory_items["Rapier_0001"]},
        /*Weapons*/         {},
        /*Items*/           {},
        /*Exp*/             0, 
        /*Lovers*/          {"Sheeda", "", "", "", ""},
        /*Mounted*/         0,
        /*Flying*/          0,
        /*Armored*/         0,
        /*Promoted*/        0);


    all_units["Sheeda"] = unit("Sheeda", "Pegasus Knight", id++, 
                            /*HP Str Mag Skl Spd Lck Def Res Con Mov
        stats_base*/        {18,  7,  2,  7, 18,  8,  4,  4,  5,  7},
        /*Growths*/         {40, 25, 20, 40, 70, 80, 20, 20,  0,  0},
        /*Skills*/          {18,  8,  2},
        /*Love_pts*/        { 0,  0,  0,  0,  0},
        /*Love_growths*/    { 0,  0,  0,  0,  0},
                           //swd lnc axe bow wnd fir tnd drk lgt staff
        /*Weapon_exp*/      { 0,  1,  0,  0,  0,  0,  0,  0,  0,  0},
        /*Position*/        { 1,  2,  1},
        /*Equipped*/        {0},
        /*Equipment*/       {inventory_items["Iron Lance_0001"]},
        /*Weapons*/         {},
        /*Items*/           {},
        /*Exp*/             0, 
        /*Lovers*/          {"Marth", "", "", "", ""},
        /*Mounted*/         1,
        /*Flying*/          1,
        /*Armored*/         0,
        /*Promoted*/        0);
           
    // std::ofstream out("units.txt");
    // out << all_units["Marth"]; // This is the c++ way of printing an object to file.
    // out << all_units["Sheeda"];
    // out.close();
    // remove("units.txt");
    // all_units["Marth"].write("units.txt");
    
    write_all_units("units.txt", "cpp");

    // printf("Marth's weapon. %s\n", inventory_items["Rapier_0001"].name);
    // printf("Marth's weapon. %s\n", all_units["Marth"].equipment[all_units["Marth"].get_equipped()[0]].name);
    // // return(0);
    // // printf("Marth's weapon. %s\n", all_units["Marth"].get_equipped[0].name);
    // // printf("Marth's dmg_type. %d\n", all_weapons[all_units["Marth"].equipment[0].name].dmg_type);
    // printf("Marth's attack_damage value. %d\n", all_units["Marth"].attack_damage());
    // printf("Marth's combat_damage value against Sheeda. %d\n", all_units["Marth"].combat_damage(all_units["Sheeda"],0));
    // printf("Marth's combat_damage (crit) value against Sheeda. %d\n", all_units["Marth"].combat_damage(all_units["Sheeda"],1));
    // printf("Marth's combat_hit probability against Sheeda. %d\n", all_units["Marth"].combat_hit(all_units["Sheeda"]));
    // printf("Marth's combat_critical probability against Sheeda. %d\n", all_units["Marth"].combat_critical(all_units["Sheeda"]));
    // printf("Marth's accuracy. %d\n", all_units["Marth"].accuracy());
    // printf("Marth's avoid. %d\n", all_units["Marth"].avoid());
    // printf("Marth's crit. %d\n", all_units["Marth"].critical());
    // printf("Marth's favor. %d\n", all_units["Marth"].favor());
    // printf("Sheeda's accuracy. %d\n", all_units["Sheeda"].accuracy());
    // printf("Sheeda's avoid. %d\n", all_units["Sheeda"].avoid());
    // printf("Sheeda's crit. %d\n", all_units["Sheeda"].critical());
    // printf("Sheeda's favor. %d\n", all_units["Sheeda"].favor());
    // printf("Does Sheeda double Marth?. %d\n", all_units["Sheeda"].combat_double(all_units["Marth"]));
    // printf("Does Marth double Sheeda?. %d\n", all_units["Marth"].combat_double(all_units["Sheeda"]));
    // printf("Does Marth retaliate?. %d\n", all_units["Sheeda"].retaliation(all_units["Marth"]));
    // printf("Does Sheeda retaliate?. %d\n", all_units["Marth"].retaliation(all_units["Sheeda"]));
    // all_units["Marth"].enemy_select(all_units["Marth"]);
    // all_units["Marth"].combat(all_units["Sheeda"]);
    // printf("Current Sheeda HP %d\n", all_units["Sheeda"].get_hp());
    // printf("Current Marth HP %d\n", all_units["Marth"].get_hp());
    // all_units["Marth"].combat(all_units["Sheeda"]);
    // printf("Current Marth HP %d\n", all_units["Marth"].get_hp());
    // all_units["Marth"].combat(all_units["Sheeda"]);
    // printf("Current Marth HP %d\n", all_units["Marth"].get_hp());

    // printf("Sheeda took damage?. %d\n", all_units["Sheeda"].get_hp());
    // all_units["Marth"].attack(all_units["Sheeda"]);
    // printf("Sheeda took damage?. %d\n", all_units["Sheeda"].get_hp());
    // all_units["Marth"].combat(all_units["Sheeda"]);
    // std::cout << "Please enter an integer value: ";
    // std::cin >> i;
    // return(0);
}
