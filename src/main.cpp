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
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) 
#define getrand() dist(mt) 


/// \fn LEN(arr)
/// \brief That is a macro. What is a macro? Anyway it computes lenght of arrays.

// Skill idea: divine shield! Bubble!

// Random number generation: modulo wrecks the average and makes the distribution non uniform if the base interval is not divisible.
// How to get uniform distribution from uniform arbitrary range.

std::mt19937 mt(1899); //Deterministic seed. Do like other fire Emblems. Always Same RNG,it is the player actions that change it.
std::uniform_int_distribution<char> dist(0, 100); //*DESIGN QUESTION* What should be the minimum and maximum probabilities?


bool single_roll(int in_prob) {
    bool out = (getrand() < in_prob);
    return(out);
}

bool double_roll(int in_prob) {
    int rng1 = getrand();
    int rng2 = getrand(); 
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
unsigned char equipped[1];

const unsigned char* unit::get_equipped() const{
    // For some reason passing the pointer works as I intended.
    return(equipped);
}

void unit::equip_weapon(std::vector<unsigned int> in_equipped) {
    if (in_equipped.size() != 0) {
        equipped[0] = (unsigned int) in_equipped[0]; 
    };
    attack_probs[0] = accuracy();
    attack_probs[1] = avoid();
    attack_probs[2] = critical();
    attack_probs[3] = favor();
}

unit::unit(std::string in_name, std::string in_unit_class, char in_id, 
           std::vector<unsigned int> in_stats_base, std::vector<unsigned int> in_growths, std::vector<unsigned int> in_skills,
           std::vector<unsigned int> in_love_pts, std::vector<unsigned int> in_love_growths,
           std::vector<unsigned int> in_wpn_exp, std::vector<unsigned int> in_position,
           std::vector<unsigned int> in_equipped, 
           std::vector<inventory_item> in_equipment,
           std::vector<inventory_item> in_weapons, 
           std::vector<inventory_item> in_items,
           unsigned short in_exp, std::vector<std::string> in_lovers,
           bool in_mounted, bool in_flying, bool in_armored, bool in_promoted) {
    mounted = in_mounted;
    flying = in_flying;
    armored = in_armored;
    promoted = in_promoted;
    for (int i = 0; i < in_equipment.size(); i++) {
        equipment[i] = in_equipment[i];
    }
    strncpy(name, in_name.c_str(), sizeof(in_name));
    strncpy(type, in_unit_class.c_str(), sizeof(in_unit_class));
    for (int i = 0; i < in_stats_base.size(); i++) {
        stats_base[i] = (unsigned int) in_stats_base[i];
        stats[i] = (unsigned int) in_stats_base[i];
        growths[i] = (unsigned int) in_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
        stats_bonus[i] = 0;
        // For some reason, stats_base cannot be printed unless (int) all_units["Marth"].stats_base[0]. + 0 also works.
    }
    
    current_hp = (unsigned int) stats_base[0];

    // printf("Did I equip the weapon successfully %d \n", get_equipped()[0]);
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
        position[i] = (unsigned int) in_position[i];
    }  
    for (int i = 0; i < in_lovers.size(); i++) {
        strncpy(lovers[i], in_lovers[i].c_str(), sizeof(in_lovers[i])); // This line is the problem
        love_growths[i] = (unsigned int) in_love_growths[i];
        wpn_exp[i] = (unsigned int) in_wpn_exp[i];
    }    
    id = in_id;
    equip_weapon(in_equipped);
}
unit::unit(){

}

unsigned char unit::attack_damage() {
    char unit_power = 0;
    char wpn_dmg = all_weapons[equipment[equipped[0]].name].stats[0];
    if (all_weapons[equipment[equipped[0]].name].dmg_type == 0) {
        // Physical attack_damage.
        unit_power = stats[1];
    } else {
        // Magical attack_damage.
        unit_power = stats[2];
    };
    int attack_damage = wpn_dmg + unit_power;
    // printf("wpn_dmg %d\n", wpn_dmg);
    // printf("unit_power %d\n", unit_power);
    return(attack_damage);
}

unsigned char unit::combat_damage(const unit& enemy, bool critical) {
    char terrain_def = 0;
    char enemy_def = 0 ;
    char unit_power = 0;
    char wpn_dmg = all_weapons[equipment[equipped[0]].name].stats[0];
    if (all_weapons[equipment[equipped[0]].name].dmg_type == 0) {
        // Physical attack_damage.
        unit_power = stats[1];
        enemy_def = enemy.stats[6];
    } else {
        // Magical attack_damage.
        unit_power = stats[2];
        enemy_def = enemy.stats[7];
    };
    // printf("unit_power %d\n", unit_power);
    // printf("enemy_def  %d\n", enemy_def );
    unsigned char crit_factor = 1;
    if (critical) {crit_factor=3;};
    int attack_damage = crit_factor*(std::max(wpn_dmg + unit_power - enemy_def - terrain_def, 0)); // Modern FE style. for crit_factor = 3
    // int attack_damage = crit_factor*(wpn_dmg + unit_power) - enemy_def - terrain_def);  // FE4-FE5 style. for crit_factor = 2
    if (attack_damage <= 0) {attack_damage = 0;};
    return(attack_damage);
}
unsigned char unit::avoid(){
    char supports = 0;
    char terrain_avoid = 0;
    unsigned char unit_avoid = stats[4]*2 + stats[5];
    unsigned char avoid = terrain_avoid + unit_avoid + supports ;
    return(avoid);
}
unsigned char unit::critical(){
    char supports = 0 ;
    char unit_skill = 0;
    unsigned char wpn_crit = all_weapons[equipment[equipped[0]].name].stats[2];
    unsigned char critical = wpn_crit + unit_skill + supports;
    return(critical);
}
bool unit::retaliation(const unit& enemy) const{
    unsigned char distance = abs(enemy.position[0] - position[0]) + abs(enemy.position[1] - position[1]);
    printf("Distance %d \n", distance);
    bool out = 0;
    for (int i = 0; i < 3; i++) {
        if (distance == all_weapons[enemy.equipment[enemy.get_equipped()[0]].name].range[i]){
            out = 1;
        }
    }
    return(out);
}

bool unit::combat_double(const unit& enemy) const{
    unsigned char unit_speed = stats[4];
    unsigned char enemy_speed = enemy.stats[4];
    bool out = ((unit_speed - wpn_weighed_down() - enemy_speed) > 4);
    return(out);
}

unsigned char unit::wpn_weighed_down() const{
    //*DESIGN QUESTION* What should be the influence of weapons?
    unsigned char wpn_wght = all_weapons[equipment[equipped[0]].name].stats[3];
    unsigned char unit_con = stats[8];
    return(std::max(wpn_wght - unit_con, 0));
}

unsigned char unit::combat_critical(const unit& enemy){
    char supports = 0;
    char unit_skill = 0;
    unsigned char wpn_crit = all_weapons[equipment[equipped[0]].name].stats[2];
    unsigned char critical = wpn_crit + unit_skill + supports - enemy.attack_probs[3];
    return(critical);
}
unsigned char unit::favor(){
    char supports = 0 ;
    char unit_favor = (ceil(stats[5]/2.)); // By default, integer division floors
    // For design simplicity, I think it is good to have percent values only change by increments of 1. Simple.
    unsigned char favor = unit_favor + supports;
    return(favor);
}
unsigned char unit::accuracy(){
    char supports = 0;
    unsigned char wpn_hit = all_weapons[equipment[equipped[0]].name].stats[1];
    unsigned char unit_acc = stats[3]*2 + stats[5];
    unsigned char accuracy = wpn_hit + unit_acc + supports;
    return(accuracy);
}
unsigned char unit::combat_hit(const unit& enemy){
    char supports = 0;
    unsigned char wpn_hit = all_weapons[equipment[equipped[0]].name].stats[1];
    unsigned char unit_acc = stats[3]*2 + stats[5];
    unsigned char accuracy = wpn_hit + unit_acc + supports - enemy.attack_probs[1];
    return(accuracy);
}
unsigned char unit::attack(unit& enemy){

    bool unit_hits = (getrand() < combat_hit(enemy));
    bool unit_crits = (getrand() < combat_critical(enemy));
    /* *DESIGN QUESTION* Should a random number always be rolled for crits, even if the hit doesn't connect?
    * I think so. Always same number of rand rolled. 
    * But what about crit animations? Should crit animations be shown to miss? Fire Emblem thinks not.
    */
    // unit.current_hp -= combat_damage(enemy, unit_crits);
    enemy.current_hp -= combat_damage(enemy, unit_crits);
    return(combat_damage(enemy, unit_crits));
}

void unit::combat(unit& enemy){
    bool unit_doubles = combat_double(enemy);
    // enemy.combat_double(unit);
    // const unit &temp_unit = static_cast<const unit&>(*this);
    bool enemy_retaliates = enemy.retaliation(static_cast<const unit&>(*this));
    bool enemy_doubles = 0;
    
    // unit.attack()
    
    if (enemy_retaliates) {
        // enemy.attack()
        enemy_doubles = enemy.combat_double(static_cast<const unit&>(*this));
    }
    // if (unit_doubles) {unit.attack()}
    // if (enemy_doubles) {enemy.attack()}
    printf("unit name %s\n", name);
    printf("enemy name %s\n", enemy.name);
    printf("unit double %d\n", unit_doubles);
    printf("enemy retaliates %d\n", enemy_retaliates);
    printf("enemy double %d\n", enemy_doubles);
    // retaliation();

}

void unit::enemy_select(const unit& enemy) {
    combat_probs[0] = combat_hit(enemy);
    combat_probs[1] = combat_critical(enemy);
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
       std::vector<int> in_stats, std::vector<int> in_range, std::vector<char> in_stats_bonus,
       std::vector<std::string> in_owner, std::vector<std::string> in_effective, bool in_dmg_type) {
    strncpy(name, in_name.c_str(), 30); // for some reason in_name is always of size 8. Whatever.
    strncpy(type, in_type.c_str(), 30);
    dmg_type = (bool) in_dmg_type;
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
    for (int i = 0; i < in_range.size(); i++) {
        range[i] = in_range[i];      
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
std::unordered_map<string, weapon> all_weapons;
std::unordered_map<string, struct inventory_item> inventory_items;
std::unordered_map<string, unit> all_units;

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
            // dmg hit  crt wght uses  exp
              {15,  65,  0,   8,  25,   1},
              {1}, // range
              std::vector<char>(LEN(unit_stats), 0), {}, {}, 0);
   
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
        /*Weapon_exp*/      { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
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


    all_units["Sheeda"] = unit("Sheeda", "Prince", id++, 
                            /*HP Str Mag Skl Spd Lck Def Res Con Mov
        stats_base*/        {18,  7,  2,  7, 18,  8,  4,  4,  5,  7},
        /*Growths*/         {40, 25, 20, 40, 70, 80, 20, 20,  0,  0},
        /*Skills*/          {18,  8,  2},
        /*Love_pts*/        { 0,  0,  0,  0,  0},
        /*Love_growths*/    { 0,  0,  0,  0,  0},
        /*Weapon_exp*/      { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
        /*Position*/        { 1,  2,  1},
        /*Equipped*/        {0},
        /*Equipment*/       {inventory_items["Iron Lance_0001"]},
        /*Weapons*/         {},
        /*Items*/           {},
        /*Exp*/             0, 
        /*Lovers*/          {"Marth", "", "", "", ""},
        /*Mounted*/         0,
        /*Flying*/          0,
        /*Armored*/         0,
        /*Promoted*/        0);
           
    
    printf("Marth's weapon. %s\n", inventory_items["Rapier_0001"].name);
    printf("Marth's weapon. %s\n", all_units["Marth"].equipment[all_units["Marth"].get_equipped()[0]].name);
    // return(0);
    // printf("Marth's weapon. %s\n", all_units["Marth"].get_equipped[0].name);
    printf("Marth's dmg_type. %d\n", all_weapons[all_units["Marth"].equipment[0].name].dmg_type);
    printf("Marth's attack_damage value. %d\n", all_units["Marth"].attack_damage());
    printf("Marth's combat_damage value against Sheeda. %d\n", all_units["Marth"].combat_damage(all_units["Sheeda"],0));
    printf("Marth's combat_damage (crit) value against Sheeda. %d\n", all_units["Marth"].combat_damage(all_units["Sheeda"],1));
    printf("Marth's combat_hit probability against Sheeda. %d\n", all_units["Marth"].combat_hit(all_units["Sheeda"]));
    printf("Marth's combat_critical probability against Sheeda. %d\n", all_units["Marth"].combat_critical(all_units["Sheeda"]));
    printf("Marth's accuracy. %d\n", all_units["Marth"].accuracy());
    printf("Marth's avoid. %d\n", all_units["Marth"].avoid());
    printf("Marth's crit. %d\n", all_units["Marth"].critical());
    printf("Marth's favor. %d\n", all_units["Marth"].favor());
    printf("Sheeda's accuracy. %d\n", all_units["Sheeda"].accuracy());
    printf("Sheeda's avoid. %d\n", all_units["Sheeda"].avoid());
    printf("Sheeda's crit. %d\n", all_units["Sheeda"].critical());
    printf("Sheeda's favor. %d\n", all_units["Sheeda"].favor());
    printf("Does Sheeda double Marth?. %d\n", all_units["Sheeda"].combat_double(all_units["Marth"]));
    printf("Does Marth double Sheeda?. %d\n", all_units["Marth"].combat_double(all_units["Sheeda"]));
    printf("Does Marth retaliate?. %d\n", all_units["Sheeda"].retaliation(all_units["Marth"]));
    printf("Does Sheeda retaliate?. %d\n", all_units["Marth"].retaliation(all_units["Sheeda"]));
    all_units["Marth"].enemy_select(all_units["Marth"]);
    all_units["Marth"].attack(all_units["Sheeda"]);
    printf("Sheeda took damage?. %d\n", all_units["Sheeda"].current_hp);
    all_units["Marth"].combat(all_units["Sheeda"]);
    int i;
    // std::cout << "Please enter an integer value: ";
    // std::cin >> i;
    // return(0);
}
