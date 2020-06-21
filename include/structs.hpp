#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <vector>
#include "SDL2/SDL.h"
#include <entityx/entityx.h>
#include "enums.hpp"

struct Tile_stats {
    int8_t dodge;
    int8_t Pprot;
    int8_t Mprot;
    int8_t heal; // %. Negative means damage.
};

struct Unit_state {
    // Number of turns to be in this status. <0 means forever.
    int8_t poisoned;
    int8_t sleep;
    int8_t stone;
};

struct Map_condition {
    int16_t army = -1;
    int16_t unit = -1;
    int16_t colmin = -1;
    int16_t colmax = -1;
    int16_t rowmin = -1;
    int16_t rowmax = -1;
    //list of points instead? or both? 
};

struct Quantity {
    uint8_t swords = 0;
    uint8_t lances = 0;
    uint8_t axes = 0;
    uint8_t bows = 0;
    uint8_t trinkets = 0;
    uint8_t offhands = 0;
    uint8_t elemental = 0;
    uint8_t demonic = 0;
    uint8_t angelic = 0;
    uint8_t shields = 0;
    uint8_t staffs = 0;
    uint8_t claws = 0;
    uint8_t items = 0;
    uint8_t books = 0;
};

struct Condition {
    int16_t unitid;
    bool dead;
    bool recruited;
};

struct Narrative {
    bool death[UNIT::NAME::NPC_END - UNIT::NAME::ERWIN] = {0};
    bool recruited[UNIT::NAME::PC_END - UNIT::NAME::ERWIN] = {0};
    int8_t chapter = 0;
};

struct Unit_stats {
    uint8_t hp; // hit points
    uint8_t str; // strength
    uint8_t mag; // magic
    uint8_t agi; // agility
    uint8_t dex; // dexterity
    uint8_t luck;
    uint8_t def; // defense
    uint8_t res; // resistance
    uint8_t con; // constitution
    uint8_t move; // movement
    uint8_t prof; // proficiency
};

struct Dialog_line {
    std::vector<Condition> conditions;
    std::vector<int8_t> chapters;
    int16_t speaker;
    std::string line;
};

struct Page {
    std::vector<std::string> paragraphs;
    std::string title = "";
    // limit to how much text?
    std::string image;

};

struct Combat_stats {
    uint8_t hit;
    uint8_t dodge;
    uint8_t crit;
    uint8_t favor;
    // int8_t speed;
};

struct Infusion {
    int8_t power = -1; // >0 means weapon was already infused/is a magic weapon.
    uint16_t type = 0;
};

struct Point {
    int32_t x;
    int32_t y;
};

struct Weapon_stats {
    uint8_t Pmight; //Physical might Pprot for shields.
    uint8_t Mmight; //Magic might. Mprot for shields.
    Combat_stats combat;
    uint8_t wgt; // weight
    uint8_t uses;
    uint8_t prof; // proficiency
    uint8_t range[2]; // [min_range, max_range]
    uint8_t hand[2]; //[1,0], [2,0] or [1,2]
    bool dmg_type; // 0 is physical. 1 magic.
    uint16_t price;
    uint8_t heal;
};

struct Inventory_item {
    int16_t id = -1;
    uint8_t used = 0;
    bool highlighted = true;
    int8_t infused = -1;
    // item images are highlighted by default.
    // Only dark when in unit inventory and unequippable
};

struct Hands {
    bool right;
    bool left;
};

struct Equipped {
    Inventory_item right;
    Inventory_item left;
};

struct Map_arrival {
    uint8_t turn;
    uint8_t levelups;
    int16_t army;
    int16_t id;
    Point position;
};

struct Support {
    // std::vector<int16_t> inds;
    int16_t index = -1;
    int8_t level = -1;
};

struct Movement_cost {
    // i THINK THIS IS USELESS
    uint8_t foot_slow; // fencer, mousquetaire, duelist, lord, duke, archer, marksman, mercenary, hero, trooper
    uint8_t foot_fast; // pickpocket thief assassin, demon
    uint8_t mages; // mage, battlemage, sage, oracle, priest, cleric, bishop, incarnate, possessed
    uint8_t riders_slow; // cavalier, archer and marksman rider, lord and duke rider.
    uint8_t riders_fast; // paladin, troubadour
    uint8_t fliers; // pegasus knight, angel
    uint8_t armors; // knight, general
    uint8_t pirates; // Corsair, viking
    uint8_t bandits; // bandit, ravager
};

struct Fps {
    Point pos = {750, 0};
    bool show = true;
    float sizefactor[2] = {0.5, 0.5};
    uint8_t cap = 60;
    SDL_Color textcolor = {0, 0, 0};
};

struct Mouse {
    uint8_t onhold = MOUSE::ACCEPT;
    uint8_t move = MOUSE::FOLLOW;
};

struct Settings {
    char title[DEFAULT::BUFFER_SIZE] = "Codename:Firesaga";
    Point res = {1000, 1000}; //resolution
    Point pos = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED};
    uint8_t fontsize = 28;
    bool fullscreen = false;
    Fps FPS;
    Mouse mouse;
    uint16_t tilesize[2] = {DEFAULT::TILESIZE, DEFAULT::TILESIZE};
};


struct MouseInputMap {
    std::vector<uint8_t> accept{SDL_BUTTON_LEFT};
    std::vector<uint8_t> cancel{SDL_BUTTON_RIGHT};
    std::vector<uint8_t> pause{SDL_BUTTON_MIDDLE};
    std::vector<uint8_t> stats{};
    std::vector<uint8_t> menu_right{};
    std::vector<uint8_t> menu_left{};
    std::vector<uint8_t> minimap{};
    std::vector<uint8_t> faster{};

};

struct KeyboardInputMap {
    std::vector<SDL_Scancode> moveright{SDL_SCANCODE_RIGHT, SDL_SCANCODE_D, SDL_SCANCODE_L};
    std::vector<SDL_Scancode> moveup{SDL_SCANCODE_W, SDL_SCANCODE_UP, SDL_SCANCODE_I};
    std::vector<SDL_Scancode> movedown{SDL_SCANCODE_S, SDL_SCANCODE_DOWN, SDL_SCANCODE_K};
    std::vector<SDL_Scancode> moveleft{SDL_SCANCODE_A, SDL_SCANCODE_LEFT, SDL_SCANCODE_J};

    std::vector<SDL_Scancode> accept{SDL_SCANCODE_SPACE};
    SDL_Scancode new_accept = SDL_SCANCODE_SPACE;
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