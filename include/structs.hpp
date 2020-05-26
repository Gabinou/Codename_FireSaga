#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>
#include "SDL2/SDL.h"
#include <entityx/entityx.h>
#include "enums.hpp"

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

struct Map_condition {
    short int army = -1;
    short int unit = -1;
    short int colmin = -1;
    short int colmax = -1;
    short int rowmin = -1;
    short int rowmax = -1;
};

struct Quantity {
    unsigned char swords = 0;
    unsigned char lances = 0;
    unsigned char axes = 0;
    unsigned char bows = 0;
    unsigned char trinkets = 0;
    unsigned char offhands = 0;
    unsigned char elemental = 0;
    unsigned char demonic = 0;
    unsigned char angelic = 0;
    unsigned char shields = 0;
    unsigned char staffs = 0;
    unsigned char claws = 0;
    unsigned char items = 0;
    unsigned char books = 0;
};

struct Narrative {
    bool death[UNIT::NAME::NPC_END - UNIT::NAME::ERWIN] = {0};
    bool recruited[UNIT::NAME::PC_END - UNIT::NAME::ERWIN] = {0};
    char chapter = 0;
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

struct Dialog_line {
    // How to make the line dynamic?
    unsigned short int id;
    unsigned char speaker;
    std::string line;
};

struct Page {
    std::vector<std::string> paragraphs;
    std::string title = "";
    // limit to how much text?
    std::string image;

};

struct Combat_stats {
    unsigned char hit;
    unsigned char dodge;
    unsigned char crit;
    unsigned char favor;
    // char speed;
};

struct Infusion {
    char power = -1; // >0 means weapon was already infused/is a magic weapon.
    short unsigned int type = 0;
};

struct Point {
    short int x;
    short int y;
};

struct Point_int {
    int x;
    int y;
};

struct Weapon_stats {
    unsigned char Pmight; //Physical might Pprot for shields.
    unsigned char Mmight; //Magic might. Mprot for shields.
    Combat_stats combat;
    unsigned char wgt; // weight
    unsigned char uses;
    unsigned char prof; // proficiency
    unsigned char range[2]; // [min_range, max_range]
    unsigned char hand[2]; //[1,0], [2,0] or [1,2]
    bool dmg_type; // 0 is physical. 1 magic.
    unsigned short int price;
    unsigned char heal;
};

struct Inventory_item {
    short int id = -1;
    unsigned char used = 0;
    bool highlighted = true;
    char infused = -1;
    // item images are highlighted by default.
    // Only dark when in unit inventory and unequippable
};

struct Equipped {
    // Index of weapon in inventory of character.
    char right;
    char left;
};

struct Map_arrival {
    unsigned char turn;
    unsigned char levelups;
    short int army;
    short int id;
    Point position;
};

struct Support {
    // std::vector<short int> inds;
    short int index = -1;
    char level = -1;
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
    bool show = true;
    float sizefactor[2] = {0.5, 0.5};
    unsigned char cap = 60;
    SDL_Color textcolor = {0, 0, 0};
};

struct Mouse {
    unsigned char onhold = MOUSE::ACCEPT;
    unsigned char move = MOUSE::FOLLOW;
};

struct Settings {
    char title[DEFAULT::BUFFER_SIZE] = "Codename:Firesaga";
    Point res = {1000, 1000}; //resolution
    Point_int pos = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
    unsigned char fontsize = 28;
    bool fullscreen = false;
    Fps FPS;
    Mouse mouse;
    unsigned short int tilesize[2] = {DEFAULT::TILESIZE, DEFAULT::TILESIZE};
};


struct MouseInputMap {
    std::vector<unsigned char> accept{SDL_BUTTON_LEFT};
    std::vector<unsigned char> cancel{SDL_BUTTON_RIGHT};
    std::vector<unsigned char> pause{SDL_BUTTON_MIDDLE};
    std::vector<unsigned char> stats{};
    std::vector<unsigned char> menu_right{};
    std::vector<unsigned char> menu_left{};
    std::vector<unsigned char> minimap{};
    std::vector<unsigned char> faster{};

};

struct KeyboardInputMap {
    std::vector<SDL_Scancode> moveright{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, SDL_SCANCODE_L};
    std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP, SDL_SCANCODE_I};
    std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN, SDL_SCANCODE_K};
    std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT, SDL_SCANCODE_J};

    std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
    std::vector<SDL_Scancode> cancel{SDL_SCANCODE_BACKSPACE};
    std::vector<SDL_Scancode> stats{SDL_SCANCODE_Q};
    std::vector<SDL_Scancode> menu_right{SDL_SCANCODE_Q}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> menu_left{SDL_SCANCODE_E}; // Switches between units of same affilition, when in 'map' states.
    std::vector<SDL_Scancode> minimap{SDL_SCANCODE_R}; // Same controls as when in map state.
    std::vector<SDL_Scancode> faster{SDL_SCANCODE_TAB}; // Toggle or Hold?
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


#endif /* STRUCTS_HPP */