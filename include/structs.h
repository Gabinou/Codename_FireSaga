#ifndef STRUCTS_H
#define STRUCTS_H

#include "types.h"
#include "debug.h"
#include "enums.h"
#include "nmath.h"
#include "tnecs.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"

struct MenuElemDirections {
    if8 right;
    if8 top;
    if8 left;
    if8 bottom;
};
extern struct MenuElemDirections MenuElemDirections_default;

struct Damage {
    uf8 dmg[DAMAGE_TYPES];
    uf8 dmg_crit[DAMAGE_TYPES];
};
extern struct Damage Damage_default;

typedef struct CombatAnimation {
    int attack_ind;
    int pause_after_ms;
    int pause_before_ms;
    int frame_count; /* highest frame index played in current animation */
} CombatAnimation;
extern struct CombatAnimation CombatAnimation_default;

union Graphics {
    SDL_Texture *texture;
    SDL_Surface *surface;
    i32 index;
};

struct Rendered { /* for SDL_RenderCopyEx */
    union Graphics graphics;
    SDL_RendererFlip flip;
    double angle;
};
extern struct Rendered Rendered_default;

struct Tile_stats {
    if8 dodge;
    if8 Pprot;
    if8 Mprot;
    if8 heal; /* % Negative means damage. */
};
extern struct Tile_stats Tile_stats_default;

/* Struct is better: Can be cast to array */
struct Unit_stats {
    uf8 hp;   /* hit points     */
    uf8 str;  /* strength       */
    uf8 mag;  /* magic          */
    uf8 agi;  /* agility        */
    uf8 dex;  /* dexterity      */
    uf8 fth;  /* faith          */
    uf8 luck;
    uf8 def;  /* defense        */
    uf8 res;  /* resistance     */
    uf8 con;  /* constitution   */
    uf8 move; /* movement       */
    uf8 prof; /* proficiency    */
};
extern struct Unit_stats Unit_stats_default;

/* Struct is better: Can be cast to array*/
struct Range {
    uf8 min;
    uf8 max;
};

struct Computed_Stats {
    uf8 attack[DAMAGE_TYPES];
    uf8 protection[DAMAGE_TYPES];
    uf8 hit;
    if8 dodge; /* can be negative */
    uf8 crit;
    uf8 favor;
    uf8 move;
    if8 speed; /* relative to agi so +/- */
    uf8 agony;
    struct Range range_combined;
    struct Range range_loadout;
};

extern void Computed_Stats_Print(struct Computed_Stats *stats);
extern void Computed_Stats_Compare(struct Computed_Stats *stats1, struct Computed_Stats *stats2);

struct Condition {
    // Conversation condition?
    if16 unitid;
    bool dead;
    bool recruited;
};
extern struct Condition Condition_default;

struct Promotion {
    struct Unit_stats bonus;
    uf16 skill;
    uf16 level;
};
extern struct Promotion Promotion_default;

struct HP {
    uf8 max;
    uf8 current;
    uf8 overheal;
    bool divine; // what?
};
extern struct HP HP_default;

extern struct nmath_hexpoint_int32_t Cube_Direction_xp;
extern struct nmath_hexpoint_int32_t Cube_Direction_xm;
extern struct nmath_hexpoint_int32_t Cube_Direction_yp;
extern struct nmath_hexpoint_int32_t Cube_Direction_ym;
extern struct nmath_hexpoint_int32_t Cube_Direction_zp;
extern struct nmath_hexpoint_int32_t Cube_Direction_zm;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_xp;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_xm;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_yp;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_ym;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_zp;
extern struct nmath_hexpoint_int32_t Cube_Diagonal_zm;

struct Point {
    i32 x;
    i32 y;
};

struct Pointf {
    float x;
    float y;
};

struct SquareNeighbors {
    i32 right;
    i32 top;
    i32 left;
    i32 bottom;
};


struct Node {
    i32 x;
    i32 y;
    i32 distance;
    i32 cost;
};

struct Nodeq {
    i32 x;
    i32 y;
    i32 priority;
    i32 cost;
};

struct Padding {
    i32 right;
    i32 top;
    i32 left;
    i32 bottom;
};
extern struct Padding Padding_default;

struct Item_stats {
    uf16 price;
    uf8 uses;
    uf8 AP; // ability power: for heal (%), blowHorn, gainStats, gainSkill, Repair
};
extern struct Item_stats Item_stats_default;

struct Crit_Multiplier {
    uf8 num;
    uf8 denom;
};
extern struct Crit_Multiplier Crit_Multiplier_default;

struct Weapon_stats {
    uf8 attack[ATTACK_TYPES_NO_TOTAL];
    uf8 protection[PROTECTION_TYPES_NO_TOTAL];
    struct Range range; /* [0]: min, [1]: max */
    uf8 hit;
    if8 dodge;  /* when the Sword is TOO HEAVY TO DODGE */
    uf8 crit;
    uf8 favor;
    uf8 wgt;    /* weight */
    uf8 prof;   /* proficiency */
};
extern struct Weapon_stats Weapon_stats_default;

struct Shop {
    if16 shopkeeper;
    if16 *items;
    if8  *qty;  /* < 0 means infinity */
};
extern struct Shop Shop_default;

struct Inventory_item {
    if16 id;
    uf8 used;
    bool highlighted;
    /* item images are highlighted by default. */
    /* Only dark when in unit inventory and unequippable */
    if8 infusion;
};
extern struct Inventory_item Inventory_item_default;
extern struct Inventory_item Inventory_item_broken;

struct Reinforcement {
    struct Point position;
    if16 army;
    if16 id;
    uf8 turn;
    uf8 levelups;
};
extern struct Reinforcement Reinforcement_default;

struct Movement_cost {
    uf8 foot_slow;
    uf8 foot_fast;
    uf8 mages;
    uf8 riders_slow;
    uf8 riders_fast;
    uf8 fliers;
    uf8 armors;
    uf8 pirates;
    uf8 bandits;
};
extern struct Movement_cost Movement_cost_default;

struct fMovement_cost {
    float foot_slow;
    float foot_fast;
    float mages;
    float riders_slow;
    float riders_fast;
    float fliers;
    float armors;
    float pirates;
    float bandits;
};
extern struct fMovement_cost fMovement_cost_default;

struct Fps {
    struct Point pos;
    SDL_Color textcolor;
    float sizefactor[TWO_D];
    bool show;
    int cap;    /* [Hz] [s^-1]          */
    int ff_cap; /* [%] Above cap FPS    */
};

struct Mouse {
    uf8 onhold;
    uf8 move;
};

struct Cursor {
    if16 frames;
    if16 speed;
};

struct Camera {
    struct Point offset; /* pixels */
    float        zoom;
};

struct Map_settings {
    uf8         overlay_mode; /* tile paletteswap or opaque overlay */
    uf8         stack_mode;
    SDL_Color   color_grid;
    bool        grid_show;
};

struct Settings {
    struct Point res; /* resolution */
    struct Point pos;
    struct Fps   FPS;

    struct Cursor cursor; /* 32 bits */
    uf16 tilesize[TWO_D];
    uf8 fontsize;

    struct Map_settings map_settings;

    struct Mouse mouse; /* 16 bits */
    char title[DEFAULT_BUFFER_SIZE];

    bool fullscreen         : 1;
    bool tophand_stronghand : 1;
};
extern struct Settings Settings_default;

typedef struct Timer {
    i64 time_ns;
    i64 frame_count;
    bool reset;
    bool paused;
} Timer;
extern struct Timer Timer_default;

#endif /* STRUCTS_H */
