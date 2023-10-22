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
    i8 right;
    i8 top;
    i8 left;
    i8 bottom;
};
extern struct MenuElemDirections MenuElemDirections_default;

struct Damage {
    u8 dmg[DAMAGE_TYPES];
    u8 dmg_crit[DAMAGE_TYPES];
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
    i8 dodge;
    i8 Pprot;
    i8 Mprot;
    i8 heal; /* % Negative means damage. */
};
extern struct Tile_stats Tile_stats_default;

/* Struct is better: Can be cast to array */
struct Unit_stats {
    u8 hp;   /* hit points     */
    u8 str;  /* strength       */
    u8 mag;  /* magic          */
    u8 agi;  /* agility        */
    u8 dex;  /* dexterity      */
    u8 fth;  /* faith          */
    u8 luck;
    u8 def;  /* defense        */
    u8 res;  /* resistance     */
    u8 con;  /* constitution   */
    u8 move; /* movement       */
    u8 prof; /* proficiency    */
};
extern struct Unit_stats Unit_stats_default;

/* Struct is better: Can be cast to array*/
struct Range {
    u8 min;
    u8 max;
};

struct Computed_Stats {
    u8 attack[DAMAGE_TYPES];
    u8 protection[DAMAGE_TYPES];
    u8 hit;
    i8 dodge; /* can be negative */
    u8 crit;
    u8 favor;
    u8 move;
    i8 speed; /* relative to agi so +/- */
    u8 agony;
    struct Range range_combined;
    struct Range range_loadout;
};

void Computed_Stats_Print(struct Computed_Stats *stats);
void Computed_Stats_Compare(struct Computed_Stats *stats1, struct Computed_Stats *stats2);

struct Condition {
    // Conversation condition?
    i16 unitid;
    bool dead;
    bool recruited;
};
extern struct Condition Condition_default;

struct Promotion {
    struct Unit_stats bonus;
    u16 skill;
    u16 level;
};
extern struct Promotion Promotion_default;

struct HP {
    u8 max;
    u8 current;
    u8 overheal;
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

struct SquareNeighbours {
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
    u16 price;
    u8 uses;
    u8 AP; // ability power: for heal (%), blowHorn, gainStats, gainSkill, Repair
};
extern struct Item_stats Item_stats_default;

struct Crit_Multiplier {
    u8 num;
    u8 denom;
};
extern struct Crit_Multiplier Crit_Multiplier_default;

struct Weapon_stats {
    u8 attack[ATTACK_TYPES_NO_TOTAL];
    u8 protection[PROTECTION_TYPES_NO_TOTAL];
    struct Range range; /* [0]: min, [1]: max */
    u8 hit;
    i8 dodge;  /* when the Sword is TOO HEAVY TO DODGE */
    u8 crit;
    u8 favor;
    u8 wgt;    /* weight */
    u8 prof;   /* proficiency */
};
extern struct Weapon_stats Weapon_stats_default;

struct Shop {
    i16 shopkeeper;
    i16 *items;
    i8  *qty;  /* < 0 means infinity */
};
extern struct Shop Shop_default;

struct Inventory_item {
    i16 id;
    u8 used;
    bool highlighted;
    /* item images are highlighted by default. */
    /* Only dark when in unit inventory and unequippable */
    i8 infusion;
};
extern struct Inventory_item Inventory_item_default;
extern struct Inventory_item Inventory_item_broken;

struct Reinforcement {
    struct Point position;
    i16 army;
    i16 id;
    u8 turn;
    u8 levelups;
};
extern struct Reinforcement Reinforcement_default;

struct Movement_cost {
    u8 foot_slow;
    u8 foot_fast;
    u8 mages;
    u8 riders_slow;
    u8 riders_fast;
    u8 fliers;
    u8 armors;
    u8 pirates;
    u8 bandits;
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
    u8 onhold;
    u8 move;
};

struct Cursor {
    i16 frames;
    i16 speed;
};

struct Camera {
    struct Point offset; /* pixels */
    float        zoom;
};

struct Map_settings {
    u8         overlay_mode; /* tile paletteswap or opaque overlay */
    u8         stack_mode;
    SDL_Color   color_grid;
    bool        grid_show;
};

struct Settings {
    struct Point res; /* resolution */
    struct Point pos;
    struct Fps   FPS;

    struct Cursor cursor; /* 32 bits */
    u16 tilesize[TWO_D];
    u8 fontsize;

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
