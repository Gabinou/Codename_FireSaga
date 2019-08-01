#ifndef SHARED_HPP
#define SHARED_HPP
using namespace std;
#include <stdio.h>
#include <iostream>
#include <string>
#include <ostream>
#include <istream>
#include <ctype.h>
#include <vector>
#include <math.h> 
#include <random>
#include <bits/stdc++.h> 
#include "unit.hpp"
#include "weapon.hpp"
/*! \file shared.hpp
* \brief Shared data.
*/
/// \def LEN(arr)
/// \brief Measure length of array. I don't know why this is a macro.
#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) 
// extern std::mt19937 mt(1899); 
//Deterministic seed. *DESIGN QUESTION*: What about the RNG? My answer: do like other fire Emblems. Always Same RNG, it is the player actions that change it. Makes debugging repeatable. Nice and convenient.
/// \fn mt
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern std::mt19937 mt; 
/// \fn dist
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern std::uniform_int_distribution<char> dist; //*DESIGN QUESTION* What should be the minimum and maximum probabilities?

/// \fn get_rand
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern int get_rand();

/// \fn single_roll(int)
/// \brief Check if event occurs using single RNG roll: if rand<prob, event occurs. True to probabilities, but humans think it is weird.
extern bool single_roll(int);

/// \fn double_roll(int)
/// \brief Check if event occurs using double RNG roll: if mean of 2 random numbers is lwer than probability of event, it occurs. Skews probabilities, but fits with humans biases.
extern bool double_roll(int);

extern char unit_attributes[][24];

extern char unit_stats[][14];
/// \var extern char unit_stats
/// \brief Global declaration of unit_stats.
extern char weapon_stats[][14];
/// \var extern char weapon_stats
/// \brief Global declaration of weapon_stats.
extern char wpn_types[][12];
/// \var extern char wpn_types
/// \brief Global declaration of wpn_types.
extern char weapons[][14];
/// \var extern char weapons
/// \brief Global declaration of weapons.
extern char statuses[][14];
/// \var extern char statuses
/// \brief Global declaration of statuses.
extern char unit_classes[][24];
/// \var extern char unit_classes
/// \brief Global declaration of unit_classes.
extern char equipment_slots;
/// \var extern char equipment_slots
/// \brief Global declaration of equipment_slots.
extern unsigned char id;
/// \var extern char id
/// \brief Global declaration of id.

extern std::vector<int> extractIntegerWords(string);
/// \struct inventory_item
/// \brief Representation of items in unit inventory to reduce memory.
struct inventory_item {
    /// \var char name
    /// \brief name of weapon in inventory.
    char name[30];   
    /// \var used
    /// \brief number of times item was used.
    char used;
    /// \fn inventory_item(const inventory_item&) 
    /// \brief constructor of inventory_item.
    inventory_item(const inventory_item&); 
    /// \fn inventory_item(std::string, char) 
    /// \brief constructor of inventory_item.
    inventory_item(std::string, char); 
    /// \fn inventory_item() 
    /// \brief constructor of inventory_item.
    inventory_item(); 
    ~inventory_item(); 

};

/*! \var all_weapons
* \brief Contains all instances of weapons.
*  Uses the weapon's name as std::unordered_map's key.
*  Are immutable. Should never be changed in game.
*  Instead, the inventory_item instances conatains the
*  number of times the item was used. If it is equal to 
*  the item's use, the item is destroyed.
*/
extern std::unordered_map<string, weapon> all_weapons;
/*! \var inventory_items
* \brief Contains all instances of inventory_items
*  inventory items are the represntation of the weapon
*  in the equipment of units. Made to be lighter than 
*  actually copying the weapon. 
*/
// I think a unordered map inventory_items should exist for every save.
extern std::unordered_map<string, struct inventory_item> inventory_items;
/*! \var all_units
* \brief Contains all instances of units.
*  Uses the unit's name as std::unordered_map's key.
*/
// I think a unordered map all_units should exist for every save.
extern std::unordered_map<string, unit> all_units;

#endif /* SHARED_HPP */