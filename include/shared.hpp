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
#include "enums.hpp"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

extern int geometricslide(int distance, float geo_factor = 2);
extern int vectorslide(int x, int y, bool xfirst);
extern int pingpong(int current, int upper, int lower = 0);
// extern bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find);

extern std::vector<std::string> wpntype2str(short unsigned int in_type);

struct Tile_stats {
    char dodge;
    char Pprot;
    char Mprot;
    char heal; // %. Negative means damage.
};

struct Unit_state {
    // Number of turns to be in this status. -1 means forever.
    char poisoned;
    char sleep;
    char stone;
};

struct Unit_stats {
    unsigned char hp; // hit points
    unsigned char str; // strength
    unsigned char mag; // magic
    unsigned char agi; // agility
    unsigned char dex; // dexterity
    unsigned char luck;
    unsigned char def; // defense
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
    // char speed;
};

struct Point {
    short unsigned int x;
    short unsigned int y;
};

struct Weapon_stats {
    unsigned char Pmight; //Physical might Pprot for shields.
    unsigned char Mmight; //Magic might. Mprot for shields.
    Combat_stats combat;
    unsigned char wgt; // weight
    unsigned char uses;
    unsigned char prof;
    unsigned char range[2]; // [min_range, max_range]
    unsigned char hand[2]; //[1], [2] or [1,2]
    bool dmg_type; // 0 is 1 physical. 1 magic.
    unsigned short int price;
};

struct Inventory_item {
    short int id = -1;
    unsigned char used = 0;
    bool highlighted = true;
    // item images are highlighted by default.
    // Only dark when in unit inventory and unequippable
};

struct Equipped {
    // Index of weapon in inventory of character.
    unsigned char right;
    unsigned char left;
};

struct Map_enemy {
    unsigned char arrival;
    unsigned char levelup;
};

struct Movement_cost {
    // i THINK THIS IS USELESS
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

struct Fps {
    Point pos = {750, 0};
    bool show = false;
    float sizefactor[2] = {0.5, 0.5};
    unsigned char cap = 60;
    unsigned char hold = 4;
    unsigned char held = 0;
    unsigned char frame_delay = 1000 / cap;
    SDL_Color textcolor = {0, 0, 0};
    float current = 60.0;
    unsigned short int entity;
};

struct Settings {
    Point res = {1000, 1000};
    unsigned char fontsize = 28;
    Fps FPS;
    unsigned short int tilesize[2] = {DEFAULT::TILESIZE, DEFAULT::TILESIZE};
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

template <typename T> extern std::vector<std::vector<T>> matrix2list(std::vector<std::vector<T>> matrix) {
    std::vector<std::vector<T>> list;

    for (T col = 0; col < matrix.size(); col++) {
        for (T row = 0; row < matrix[0].size(); row++) {
            if (matrix[col][row] > 0) {
                list.push_back({col, row});
            }
        }
    }

    return (list);
}

#endif /* SHARED_HPP */