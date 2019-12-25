#ifndef SHARED_HPP
#define SHARED_HPP
#include <stdio.h>
#include <iostream>
#include <string>
#include <ostream>
#include <istream>
#include <ctype.h>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>
#include <bitset>
#include <array>
#include <memory>
#include <bits/stdc++.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "weapon.hpp"
#include "inventory_item.hpp"
#include "linalg.hpp"

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

/// \fn get_rand
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern int get_rand();

/// \fn single_roll(int)
/// \brief Check if event occurs using single RNG roll: if rand<prob, event occurs. True to probabilities, but humans think it is weird.
extern bool single_roll(int);

/// \fn double_roll(int)
/// \brief Check if event occurs using double RNG roll: if mean of 2 random numbers is lower than probability of event, it occurs. Skews probabilities, but largely fits with humans biases.
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

extern std::vector<int> extract_int_string(std::string);

extern int geometricslide(int distance, float geo_factor = 2);
extern int vectorslide(int x, int y, bool xfirst);
extern bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find);

extern int pingpong(int current, int upper, int lower = 0);

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/*! \fn css_from_line()
*  \brief Get comma separated strings (css) from a line. Lines are gotten from fgets. outputs vector of string for simplicity. Fucking chars man. Sucks ballz.
*/
extern std::vector<std::string> css_from_line(char *);
extern std::vector<std::string> css_from_line(std::string, std::string delimiter = ",");
extern std::vector<std::string> get_words(std::string);
extern std::string words2str(std::vector<std::string>);
extern std::vector<int> csv_from_line(std::string, std::string delimiter = ",");
extern std::unordered_map<std::string, int> wpn_indexes;

//swd lnc axe bow wnd fir tnd drk lgt staff

extern std::string read_line(const char * filename, char skip);
extern void read_all_weapons(const char * filename = "weapons.txt");
extern void write_all_weapons(const char * filename, char const * savestyle = "cpp");
extern void read_all_units(const char * filename);
extern void write_all_units(const char * filename, char const * savestyle = "cpp");
extern void write_all_unit_classes(const char * filename);
extern void read_all_unit_classes(const char * filename = "classes_FE1.txt");

extern void permutations_binary(int len, int num_0, int out[], int i = 0);
extern int h_manhattan(int start[], int end[]);
extern double h_euclidean(int start[], int end[]);


struct Equipped {
    int left;
    int right;
    //Index in big array of all weapons? Or pointer to weapon?
};

struct Weapon_stats {
    unsigned char dmg;
    unsigned char hit;
    unsigned char crit;
    unsigned char dodge;
    unsigned char favor;
    unsigned char wgt;
    unsigned char hands;
    unsigned char range[2];
};

struct Unit_stats {
    unsigned char hp;
    unsigned char str;
    unsigned char mag;
    unsigned char spd;
    unsigned char skl;
    unsigned char luck;
    unsigned char def;
    unsigned char res;
    unsigned char con;
    unsigned char move;
};

struct Weapon {
    unsigned char dmg;
    unsigned char hit;
    unsigned char dodge;
    unsigned char crit;
    unsigned char favor;
    unsigned char wgt;
    unsigned char range[2]; // [min_range, max_range]
    bool hand; //0 is 1 hand. 1 is 2 hands.
    bool dmg_type; // 0 is 1 physical. 1 magic.
};

extern std::vector<std::vector<int>> A_star(std::vector<std::vector<int>> map, int start[], int end[], std::string mode = "matrix");
extern std::vector<std::vector<int>> movemap(std::vector<std::vector<int>> map, int start[], int move, std::string mode = "matrix");
extern std::vector<std::vector<int>> attackmap(std::vector<std::vector<int>> movemap, int start[], int move, int attack, std::string mode = "matrix");

// Texture stuff.
extern SDL_Texture * loadTexture(const char * filename);
extern SDL_Texture * textToTexture(std::string textureText, SDL_Color textColor, TTF_Font * in_font);

#endif /* SHARED_HPP */