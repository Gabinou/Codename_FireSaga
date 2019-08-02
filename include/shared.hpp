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


extern std::unordered_map<string, char> weapon_num;


#endif /* SHARED_HPP */