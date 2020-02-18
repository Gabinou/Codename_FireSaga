#ifndef SHARED_HPP
#define SHARED_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <bits/stdc++.h>
#include <algorithm>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "linalg.hpp"

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

namespace DEFAULT {
enum DEFAULTS {
    TILESIZE = 32,
    EQUIPMENT_SIZE = 7,
    WEAPONS_SIZE = 3,
    ITEMS_SIZE = 3,
    CONVOY_SIZE = 200,
    LINE_LENGTH = 255,
}; // replace text IN MY FILES by the values, at compilation.
}

namespace CLASS {
enum UNIT_CLASS {
    LORD,
    DUKE,
    ARCHER,
    ARCHER_RIDER,
    MARKSMAN,
    MARKSMAN_RIDER,
    ANGEL,
    DEMON,
    DEMONIC_INCARNATE,
    ANGELIC_INCARNATE,
    FENCER,
    DUELIST,
    MOUSQUETAIRE,
    PICKPOCKET,
    THIEF,
    ASSASSIN,
    MERCENARY,
    HERO,
    CORSAIR,
    VIKING,
    BANDIT,
    RAVAGER,
    PIKEMAN,
    CAVALIER,
    PALADIN,
    MAGE,
    BATTLEMAGE,
    TROUBADOUR,
    SAGE,
    PRIEST,
    BISHOP,
    ORACLE,
    CLERIC,
    DEMIGOD,
    GOD,
    GODDESS,
    TWINBORN
};
}

namespace TILE {
enum TILES {
    //Basic tile index is 3 digits -> 100
    // two first digits give the tile.
    // third digit gives the asset index.
    // Ex: 324: Peak tile, 4th asset of peak.
    PLAIN = 10,
    BUSH = 11,
    FOREST = 12,
    THICKET = 13,
    SNAG = 14,
    BRIDGE = 15,
    HOLE = 16,

    SEA = 20,
    LAKE = 21,
    RIVER = 22,
    WATERFALL = 23,
    CLIFF = 24,

    HILL = 30,
    MOUNTAIN = 31,
    PEAK = 32,
    CAVE = 33,

    SAND = 40,
    WASTELAND = 41,
    ROCK = 42,
    BONES = 43,

    FLOOR = 50,
    PILLAR = 51,
    WALL = 52,
    DOOR = 53,
    THRONE = 54,

    GATE = 60,
    FENCE = 61,
    SHOP = 62,
    ARMORY = 63,
    VILLAGE = 64,
    FORT = 65,
    CASTLE = 66,

    SNOW = 70,
    GLACIER = 71,
    ICE = 72,
};
}

extern std::mt19937 mt;
extern std::mt19937_64 mt_64;

extern int getRN();
extern int * getGRNs(float avg = 50., float std_dev = 20.);
extern int * boxmuller(int RN_U[2], float avg = 50., float std_dev = 20.);
// boxmuller transforms uniforml RNs into gaussian RNs.

extern bool single_roll(const int RN, const int hit);
extern bool double_roll(const int RN1, const int RN2, const int hit);

extern int geometricslide(int distance, float geo_factor = 2);
extern int vectorslide(int x, int y, bool xfirst);
extern int pingpong(int current, int upper, int lower = 0);
extern bool is_pressed(const Uint8 * state_array, std::vector<SDL_Scancode> to_find);

extern std::vector<int> extract_int_string(std::string);
extern std::vector<std::string> css_from_line(char *);
extern std::vector<std::string> css_from_line(std::string, std::string delimiter = ",");
extern std::vector<std::string> get_words(std::string);
extern std::string words2str(std::vector<std::string>);
extern std::vector<int> csv_from_line(std::string, std::string delimiter = ",");

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
extern std::vector<std::vector<int>> list2matrix(std::vector<std::vector<int>> list);

extern void writeText(int in_fontsize, int in_position[2], float in_sizefactor[2], std::string in_text, SDL_Color in_color, TTF_Font * in_font, SDL_Renderer * in_renderer);

extern SDL_Texture * loadTexture(const char * filename);
extern SDL_Texture * textToTexture(std::string textureText, SDL_Color textColor, TTF_Font * in_font);

struct Tile_stats {
    unsigned int dodge;
    unsigned int Pprot;
    unsigned int Mprot;
    unsigned int heal; // %
};

struct Unit_state {
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
    int x;
    int y;
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
    unsigned int price;
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
    bool trinket = false;
};

struct Inventory_item {
    std::string name = "";
    unsigned int used = 0;
    bool highlighted = true;
    // item images are highlighted by default.
    // Only dark when in unit inventory and unequippable
};

struct Equipped {
// Index of weapon in inventory of character.

    int right;
    int left;
};

struct Map_enemy {
    unsigned char arrival;
    unsigned char levelup;
};

struct Unit_type {
    bool human;
    bool mounted;
    bool flying;
    bool armored;
    bool demon;
    bool angel;
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

struct Fps {
    Point pos = {750, 0};
    bool show = false;
    float sizefactor[2] = {0.5, 0.5};
    char cap = 60;
    char hold = 4;
    char held = 0;
    char frame_delay = 1000 / cap;
    SDL_Color textcolor = {0, 0, 0};
    float current = 60.0;
    int entity;
};

struct Settings {
    Point res = {1000, 1000};
    char fontsize = 28;
    Fps FPS;
    int tilesize[2] = {32, 32};
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



template <typename T> extern std::vector<std::vector<T>> attackmap(std::vector<std::vector<T>> movemap, short unsigned int start[], short unsigned int move, unsigned char range[2], std::string mode = "matrix") {
    // Using the movemap to computes all attackable tiles.
    // EXCLUDING moveable tiles.
    std::vector<std::vector<T>> attackmap;
    std::vector<std::vector<T>> movelist;
    std::vector<T> temp_point = {0, 0};
    T tempx, tempy;

    movelist = matrix2list(movemap);

    if (mode == "matrix") {
        attackmap = movemap;

        for (int i = 0; i < attackmap.size(); i++) {
            std::fill(attackmap[i].begin(), attackmap[i].end(), 0);
        }
    }

    for (int i = 0; i < movelist.size(); i++) {
        for (int rangex = 0; rangex <= range[1]; rangex++) {
            int miny = std::max(0, range[0] - rangex);
            int maxy = std::max(range[1] - rangex, 0);

            for (int rangey = miny; rangey <= maxy; rangey++) {
                tempx = std::min(movelist[i][1] + rangex, (int)(movemap[0].size() - 1));
                tempy = std::min(movelist[i][0] + rangey, (int)(movemap.size() - 1));

                if ((tempx < (int)movemap[0].size()) && (tempy < (int)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::max(movelist[i][1] - rangex, 0);
                tempy = std::min(movelist[i][0] + rangey, (int)(movemap.size() - 1));

                if ((tempx >= 0) && (tempy < (int)movemap.size())) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::min(movelist[i][1] + rangex, (int)(movemap[0].size() - 1));
                tempy = std::max(movelist[i][0] - rangey, 0);

                if ((tempx < (int)movemap[0].size()) && (tempy >= 0)) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }

                tempx = std::max(movelist[i][1] - rangex, 0);
                tempy = std::max(movelist[i][0] - rangey, 0);

                if ((tempx >= 0) && (tempy >= 0)) {
                    if (movemap[tempy][tempx] == 0) {
                        if (mode == "list") {
                            temp_point = {tempx, tempy};
                            attackmap.push_back(temp_point);
                        }

                        if (mode == "matrix") {
                            attackmap[tempy][tempx] = 1;
                        }
                    }
                }
            }
        }
    }

    if (mode == "list") {
        // make_uniques
    }

    return (attackmap);
}


template <typename T> extern std::vector<std::vector<T>> movemap(std::vector<std::vector<T>> map, short unsigned int start[], short unsigned int move, std::string mode = "matrix") {
    // Using the map, computes all moveable tiles.
    // outputs either a list of points, or a map of 1 and zeros.
    // Both outputs are 2D vectors.
    struct node {
        T x;
        T y;
        T distance;
    };
    std::vector<std::vector<T>> movemap;

    if (mode == "matrix") {
        movemap = map;

        for (int i = 0; i < movemap.size(); i++) {
            std::fill(movemap[i].begin(), movemap[i].end(), 0);
        }
    }

    std::vector<node> open, closed;
    node current, neighbor;
    current.x = start[0];
    current.y = start[1];
    current.distance = 0;
    open.push_back(current);
    int index[2] = {-1, 1};

    while (!open.empty()) {
        current = open.back();
        open.pop_back();
        closed.push_back(current);

        if (mode == "matrix") {
            movemap[current.y][current.x] = 1;
        }

        if (mode == "list") {
            movemap.push_back({current.x, current.y});
        }

        // Two next whiles check the 4 neighbors.
        // (i-j)/2 == 1 when (i+j)/2 == 0 and vice versa.
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                neighbor.x = std::min(std::max(current.x + ((index[i] + index[j]) / 2), 0), T(map[0].size()) - 1);
                neighbor.y = std::min(std::max(current.y + ((index[i] - index[j]) / 2), 0), T(map.size()) - 1);
                neighbor.distance = current.distance + map[neighbor.y][neighbor.x];

                if ((neighbor.distance <= move) && (map[neighbor.y][neighbor.x] > 0)) {
                    bool inclosed = false;

                    for (int k = 0; k < closed.size(); k++) {
                        if ((neighbor.x == closed[k].x) && (neighbor.y == closed[k].y)) {
                            inclosed = true;

                            if (neighbor.distance < closed[k].distance) {
                                inclosed = false;
                                closed.erase(closed.begin() + k);
                            }

                            break;
                        }
                    }

                    if (!inclosed) {
                        open.push_back(neighbor);
                    }
                }
            }
        }
    }

    return (movemap);
}


#endif /* SHARED_HPP */