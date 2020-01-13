#ifndef SHARED_HPP
#define SHARED_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <bits/stdc++.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
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
extern std::mt19937_64 mt_64;

/// \fn getRN
/// \brief gets the next random number, using pre-defined Mersenne-Twister object applied to pre-defined uniform distribution.
extern int getRN();

/// \fn single_roll(int)
/// \brief Check if event occurs using single RNG roll: if rand<prob, event occurs. True to probabilities, but humans think it is weird.
extern bool single_roll(const int RN, const int hit);

/// \fn double_roll(int)
/// \brief Check if event occurs using double RNG roll: if mean of 2 random numbers is lower than probability of event, it occurs. Skews probabilities, but largely fits with humans biases.
extern bool double_roll(const int RN1, const int RN2, const int hit);

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

extern std::vector<std::vector<int>> A_star(std::vector<std::vector<int>> map, int start[], int end[], std::string mode = "matrix");
extern std::vector<std::vector<int>> movemap(std::vector<std::vector<int>> map, int start[], int move, std::string mode = "matrix");
extern std::vector<std::vector<int>> attackmap(std::vector<std::vector<int>> movemap, int start[], int move, int attack, std::string mode = "matrix");

extern void writeText(int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font, SDL_Renderer * in_renderer);

// Texture stuff.
extern SDL_Texture * loadTexture(const char * filename);
extern SDL_Texture * textToTexture(std::string textureText, SDL_Color textColor, TTF_Font * in_font);

struct Unit_state {
    bool flight;
    bool mounted;
    bool armored;
    std::string hand;
    char poisoned;
    char sleep;
    char stone;
};

struct Unit_stats {
    unsigned char hp; // hit points
    unsigned char str; // strength
    unsigned char mag; // magic power
    unsigned char agi; // agility
    unsigned char dex; // dexterity
    unsigned char luck;
    unsigned char def; // defense power
    unsigned char res; // resistance
    unsigned char con; // constitution
    unsigned char move; // movement
    unsigned char prof; // proficiency
};

struct Combat_stats {
    unsigned char hit;
    unsigned char dodge;
    unsigned char crit;
    unsigned char favor;
};

struct Weapon_stats {
    unsigned char might;
    Combat_stats combat;
    unsigned char wgt; // weight
    unsigned char uses;
    unsigned char wpnlvl;
    unsigned char range[2]; // [min_range, max_range]
    unsigned char hand[2]; //[1], [2] or [1,2]
    bool dmg_type; // 0 is 1 physical. 1 magic.
};

struct Weapon_type {
    bool lance = false;
    bool axe = false;
    bool sword = false;
    bool staff = false;
    bool demonic = false;
    bool angelic = false;
    bool elemental = false;
    bool offhand = false;
    bool shield = false;
};

struct Inventory_item {
    std::string name;
    unsigned int used = 0;
};

struct Equipped {
    Inventory_item left;
    Inventory_item right;
    //Index in big array of all weapons? Or pointer to weapon?
};

struct Movement_cost {
    unsigned char foot_slow; // fencer, mousquetaire, duelist, lord, duke, archer, marksman, mercenary, hero, trooper
    unsigned char foot_fast; // pickpocket thief assassin, demon
    unsigned char mages; // mage, battlemage, sage, oracle, priest, cleric, bishop, incarnate, possessed
    unsigned char riders_slow; // cavalier, archer and marksman rider, lord and duke rider.
    unsigned char riders_fast; // paladin, troubadour
    unsigned char fliers; // pegasus knight, angel
    unsigned char armors; // knight, general
    unsigned char pirates; // Corsair, viking
    unsigned char bandits; // bandit, ravager
};


struct Point {
    int x;
    int y;
};

struct Fps {
    Point pos = {750, 0};
    bool show = false;
    float sizefactor[2] = {0.5, 0.5};
    char cap = 60;
    char hold = 4;
    char held = 0;
    char frame_delay = 1000 / cap;
    SDL_Color color = {0, 0, 0};
    float current = 60.0;
    int ind;
};

struct Settings {
    Point res = {1000, 1000};
    char fontsize = 28;
    Fps FPS;
};

struct KeyboardInputMap {
    std::vector<SDL_Scancode> moveright{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, SDL_SCANCODE_L};
    std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP, SDL_SCANCODE_I};
    std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN, SDL_SCANCODE_K};
    std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT, SDL_SCANCODE_J};

    std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
    std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
    std::vector<SDL_Scancode> stats{};
    std::vector<SDL_Scancode> menu_right{}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> menu_left{}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> minimap{}; // Same controls as when in map state.
    std::vector<SDL_Scancode> faster{}; // Toggle or Hold?
    std::vector<SDL_Scancode> pause{SDL_SCANCODE_RETURN};
};

struct GamepadInputMap {
    std::vector<SDL_GameControllerAxis> mainxaxis{SDL_CONTROLLER_AXIS_LEFTX};
    std::vector<SDL_GameControllerAxis> mainyaxis{SDL_CONTROLLER_AXIS_LEFTY};
    std::vector<SDL_GameControllerAxis> secondxaxis{SDL_CONTROLLER_AXIS_RIGHTX};
    std::vector<SDL_GameControllerAxis> secondyaxis{SDL_CONTROLLER_AXIS_RIGHTY};
    std::vector<SDL_GameControllerAxis> triggerleft{SDL_CONTROLLER_AXIS_TRIGGERLEFT};
    std::vector<SDL_GameControllerAxis> triggerright{SDL_CONTROLLER_AXIS_TRIGGERRIGHT};

    std::vector<SDL_GameControllerButton> moveright{SDL_CONTROLLER_BUTTON_DPAD_RIGHT};
    std::vector<SDL_GameControllerButton> moveup{SDL_CONTROLLER_BUTTON_DPAD_UP};
    std::vector<SDL_GameControllerButton> movedown{SDL_CONTROLLER_BUTTON_DPAD_DOWN};
    std::vector<SDL_GameControllerButton> moveleft{SDL_CONTROLLER_BUTTON_DPAD_LEFT};

    std::vector<SDL_GameControllerButton> accept{SDL_CONTROLLER_BUTTON_A};
    std::vector<SDL_GameControllerButton> cancel{SDL_CONTROLLER_BUTTON_B};
    std::vector<SDL_GameControllerButton> stats{SDL_CONTROLLER_BUTTON_X};
    std::vector<SDL_GameControllerButton> menu_right{SDL_CONTROLLER_BUTTON_RIGHTSHOULDER};
    std::vector<SDL_GameControllerButton> menu_left{SDL_CONTROLLER_BUTTON_LEFTSHOULDER};
    std::vector<SDL_GameControllerButton> minimap{SDL_CONTROLLER_BUTTON_Y};
    std::vector<SDL_GameControllerButton> faster{SDL_CONTROLLER_BUTTON_START};
    std::vector<SDL_GameControllerButton> pause{SDL_CONTROLLER_BUTTON_START};
};

#endif /* SHARED_HPP */