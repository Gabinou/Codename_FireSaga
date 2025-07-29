/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** struct definitions
** TODO: get rid of it:
**      - put struct definitions in their files
**      - #include in header if definition needed
**        for struct, functions definitions
**      - #include in source otherwise
**
*/
#ifndef STRUCTS_H
#define STRUCTS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "nstr.h"
#include "tnecs.h"
#include "cooldown.h"
#include "SDL.h"
#include "SDL_mixer.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;

/* --- FUNCTIONS --- */
typedef i32(* use_function_t)(const struct Item *const, struct Unit *, struct Unit *);

/* --- STRUCTS --- */

/* --- Alignment --- */
typedef struct Alignment {
    i32 army; /* Narrative only */
} Alignment;

typedef struct Alignment Alignment_Friendly;
typedef struct Alignment Alignment_Neutral;
typedef struct Alignment Alignment_Enemy;

// #define REGISTER_ENUM(x) typedef struct Aligment Aligment_##x;
// #include "names/alignment.h"
// #undef REGISTER_ENUM

/* --- Boss --- */
// For friendlies: Friendly is a main character
//      - Dieing leads to loss
// For enemies: Enemy is a boss
//      - Killing may lead to win
typedef struct Boss {
    SDL_Rect srcrect; /* x,y,w,h */
    SDL_Rect dstrect; /* x,y,w,h */
    SDL_Texture *texture;  /* pixels */
    i32 icon;
} Boss;
extern const struct Boss Boss_default;


/* TODO: Army as component */
// TODO: set item army component SOMEWHERE.
//  when unit takes it?
typedef struct Army {
    i32 id;
} Army;


/* -- Loadout -- */
typedef struct Loadout {
    /* Indices of equipped items in equipment */
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */
} Loadout;

/* --- JSON HEADER --- */
// All structs that use jsonio need this header as the first element
// TODO: use jsonIO_Header everywhere.
typedef struct jsonIO_Header {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    i32  json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */
} jsonIO_Header;

extern const Loadout Loadout_default;

/* --- Map --- */
typedef struct MapFind {

    // DARR: healtolist or attacktolist
    i32             *list;

    // DARR: patients or defendants
    tnecs_entity    *found;

    // Tnecs entity of searching unit
    tnecs_entity     seeker;

    // Quit if ONE unit is found
    b32              fastquit;

    /* LOADOUT_EQUIPMENT, LOADOUT_INPUT (_eq) */
    i32 eq_type;
    i32 _eq; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

} MapFind;
extern const MapFind MapFind_default;

/* -- attackto -- */
typedef struct MapAct {
    // First to be able to cast: Loadout = &canEquip;
    /* only if eq_type == LOADOUT_INPUT */
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    // healtomap: ITEM_ARCHETYPE_STAFF
    // attacktomap: ITEM_ARCHETYPE_WEAPON
    i64 archetype;

    tnecs_entity aggressor; /* or healer    */
    tnecs_entity defendant; /* or patient   */

    // Is movement taken into account?
    b32 move;

    /* ARRAY_MATRIX, ARRAY_LIST */
    i32 output_type;

    /* LOADOUT_EQUIPPED, LOADOUT_EQUIPMENT, LOADOUT_INPUT */
    i32 eq_type;
    i32 _eq; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    i32 mode_movetile;
} MapAct;
extern const MapAct MapAct_default;

/* -- Can Equip -- */
// Input for canEquip function
typedef struct canEquip {
    /* Current loadout.
    **  - Needed because:
    **      - Can't 2H a 1H weapon
    **  - First to be able to cast: Loadout = &canEquip; */
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    /* Hand to equip to, considering loadout. */
    i32 hand;

    /* Equipped index to check. */
    i32 _eq; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    /* Why do we need to know archetypes?
    **  - Find all equippable staves ONLY
    **  - Find all equippable weapons (of any type) ONLY
    */
    i64 archetype;

    /* Two-handed mode.
    **  - TWO_HAND_EQ_MODE_STRICT
    **      - Can only equip if weapon is in other hand
    **  - TWO_HAND_EQ_MODE_LOOSE
    **      - Can equip in one hand -> step 1 to twohanding
    **      - Needed by LoadoutSelectMenu
    */
    i32 two_hands_mode;

    /* LOADOUT_EQUIPPED, LOADOUT_EQUIPMENT, LOADOUT_INPUT */
    i32 eq_type;

    /* For Map_canEquip
    **  - Find canEquip item in range,
    **      with or without unit movement
     */
    b32 move;
} canEquip;
extern const canEquip canEquip_default;

struct Cursor {
    i32 frames;
    i32 speed;
};

struct Mouse {
    i32 onhold;
    i32 move;
};

typedef struct Point {
    i32 x;
    i32 y;
} Point;

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

struct Fps {
    Point pos;
    SDL_Color textcolor;
    float sizefactor[TWO_D];
    b32 show;
    i32 cap;    /* [Hz] [s^-1] */
    i32 ff_cap; /* [%] Fast forward cap above FPS */
};

/* --- Settings --- */
typedef struct Music_settings {
    i32 frequency;          /*  [Hz]    */
    i32 sample_size;        /* [byte]   */
    i32 channels;
    i32 format;             /* AUDIO_*  */
    i32 music_volume;
    i32 soundfx_volume;
} Music_settings;

struct Map_settings {
    SDL_Color   color_grid;
    i32         overlay_mode; /* tile paletteswap or opaque overlay */
    i32         stack_mode;
    i32         grid_thickness; /* Number of lines to draw (mirrored) */
    i32         perim_thickness; /* Number of lines to draw (mirrored) */
    b32         grid_show;
};
/* grid_thickness 1 ->  |   (center line only)          */
/* grid_thickness 2 -> |||  (center line, +/- 1 lines)  */
/*                  and so on...                        */

struct Enemy_Turn_settings {
    u64 pause_post_reinforcement;
    u64 pause_post_move;
};

typedef struct Input_Arguments {
    char *save_filename;    /* debug saves are outside normal save integers     */
    i32   scene;            /* plays the scene then exits */
    i32   map_index;
    b32   print_help;       /* If help requested anywhere, print help and exit  */
} Input_Arguments;
extern const struct Input_Arguments Input_Arguments_default;

typedef struct Settings {
    Point res;
    Point pos;
    struct Fps   FPS;
    struct Input_Arguments args;

    struct Cursor cursor; /* 32 bits */
    u16 tilesize[TWO_D];  /* [px] */

    struct Map_settings     map_settings;
    struct Music_settings   music_settings;
    struct Enemy_Turn_settings enemy_turn_settings;

    struct Mouse mouse; /* 16 bits */
    char title[DEFAULT_BUFFER_SIZE];

    float brightness;
    i32 fontsize;
    u32 window;
    b32 fullscreen;
} Settings;
extern const struct Settings Settings_default;

typedef struct MenuElemDirections {
    i8 right;
    i8 top;
    i8 left;
    i8 bottom;
} MenuElemDirections;
extern const MenuElemDirections MenuElemDirections_default;

typedef struct Damage_Raw {
    i32 physical;
    i32 magical;
    i32 True;
    i32 dealt;      /* takes into account def, res */
} Damage_Raw;
extern const struct Damage_Raw Damage_Raw_default;

typedef struct Combat_Damage {
    /* Combat damage includes effect of def, res */
    Damage_Raw dmg;
    Damage_Raw dmg_crit;
} Combat_Damage;
extern const struct Combat_Damage Combat_Damage_default;

union Graphics {
    SDL_Texture *texture;
    SDL_Surface *surface;
    i32 index;
};

struct Rendered { /* for SDL_RenderCopyEx */
    union Graphics      graphics;
    double              angle;
    SDL_RendererFlip    flip;
};
extern const struct Rendered Rendered_default;

struct Tile_stats {
    i32 dodge;
    i32 Pprot;
    i32 Mprot;
    i32 heal; /* % Negative means damage. */
};
extern const struct Tile_stats Tile_stats_default;

typedef struct Unit_stats {
    /* Can be cast to array */
    i32 hp;   /* hit points     */
    i32 str;  /* strength       */
    i32 mag;  /* magic          */
    i32 agi;  /* agility        */
    i32 dex;  /* dexterity      */
    i32 fth;  /* faith          */
    i32 luck;
    i32 def;  /* defense        */
    i32 res;  /* resistance     */
    i32 con;  /* constitution   */
    i32 move; /* movement       */
    i32 prof; /* proficiency    */
} Unit_stats;
extern const struct Unit_stats Unit_stats_default;

typedef struct Range {
    /* No gaps in the range */
    i32 min;
    i32 max;
} Range;
extern const struct Range Range_default;

typedef struct Computed_Stats {
    Damage_Raw attack;
    Damage_Raw protection;
    i32 hit;
    i32 dodge; /* can be negative */
    i32 crit;
    i32 favor;
    i32 move;
    i32 speed; /* relative to agi so +/- */
    i32 agony; /* turns left before death (-1 not agonizing) */
    struct Range range_equipment;   /* Range of all equipment */
    struct Range range_loadout;     /* Range of equipped weapons */
    // issue:       What about staff AND weapon equipped?
    // Solution:
    //          - Only one is shown at a time.
    //          - Show two at a time w/ Blue+Red+Purple filter
} Computed_Stats;
extern const struct Computed_Stats Computed_Stats_default;

void Computed_Stats_Print(  struct Computed_Stats *stats);
void Computed_Stats_Compare(struct Computed_Stats *stats1,
                            struct Computed_Stats *stats2);

/* Bonus stats added to unit, including decay/removal conditions */
// When are bonus_stats checked for removal?
//  - Unit movement     (auras)
//  - End of turn       (auras)
//  - Equipping item    (bonus stat for equipping)
//  - Trading item      (bonus stat from item in equipment)
//  - Using item        (get a new skill)
typedef struct Bonus_Stats {
    /* Stat bonuses */
    struct Unit_stats       unit_stats;
    struct Computed_Stats   computed_stats;
    /* Conditions */
    // If any condition is not met, bonus stat is deactivated.
    struct Range range;
    i32 turns;
    u16 source_item; /* Should be equipped by unit_ent */
    u16 source_skill;
    b32 active;
    tnecs_entity source_unit;
} Bonus_Stats;
extern const struct Bonus_Stats Bonus_Stats_default;

struct Promotion {
    struct Unit_stats bonus;
    u16 skill;
    u16 level;
};
extern const struct Promotion Promotion_default;

extern const struct nmath_hexpoint_int32_t Cube_Direction_xp;
extern const struct nmath_hexpoint_int32_t Cube_Direction_xm;
extern const struct nmath_hexpoint_int32_t Cube_Direction_yp;
extern const struct nmath_hexpoint_int32_t Cube_Direction_ym;
extern const struct nmath_hexpoint_int32_t Cube_Direction_zp;
extern const struct nmath_hexpoint_int32_t Cube_Direction_zm;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_xp;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_xm;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_yp;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_ym;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_zp;
extern const struct nmath_hexpoint_int32_t Cube_Diagonal_zm;

enum SOTA_PADDING_DIRECTION {
    SOTA_PADDING_RIGHT  = 0,
    SOTA_PADDING_TOP    = 1,
    SOTA_PADDING_LEFT   = 2,
    SOTA_PADDING_BOTTOM = 3
};

struct Padding {
    i32 right;
    i32 top;
    i32 left;
    i32 bottom;
};
extern const struct Padding Padding_default;

struct Item_stats {
    i32 price;
    i32 uses;
    i32 AP; /* ability power: for heal (%), blowHorn, gainStats, gainSkill, Repair */
};
extern const struct Item_stats Item_stats_default;

struct Crit_Multiplier {
    u8 num;
    u8 denom;
};
extern const struct Crit_Multiplier Crit_Multiplier_default;

typedef struct Aura {
    struct Range            range; /* [0]: min, [1]: max */
    struct Unit_stats       unit_stats;
    struct Computed_Stats   computed_stats;
    i32 turns;
} Aura;
extern const struct Aura Aura_default;

struct Weapon_stats {
    Damage_Raw attack;
    Damage_Raw protection;
    struct Range range;
    /* Note: all stats can be negative,
    ** e.g. when the Sword is TOO HEAVY TO DODGE */
    i32 hit;
    i32 dodge;
    i32 crit;
    i32 favor;
    i32 wgt;    /* weight */
    i32 prof;   /* proficiency, to wield */

    /* Design:
    **  Stats for twohanding.
    **  Better than doubling stats for ALL weapons.
    **  Not many stats... */
    i32 prof_2H;
    i32 attack_physical_2H;
};
extern const struct Weapon_stats Weapon_stats_default;

struct Shop {
    i16 shopkeeper;
    i16 *items;
    i8  *qty;  /* < 0 means infinity */
};
extern const struct Shop Shop_default;

typedef struct Infusion {
    /* Magic power added to weapon */
    // DESIGN QUESTION:
    //  - Can there ever be... physical infusion?
    //      - Yes. Should be rare, granted by buff dude
    i32 physical;
    i32 magical;
} Infusion;

typedef struct Inventory_item {
    i32 id;
    b32 highlighted;
    i32 used;
    /* item images are highlighted by default. */
    /* Only dark when in unit inventory and unequippable */
} Inventory_item;
extern const struct Inventory_item Inventory_item_default;
extern const struct Inventory_item Inventory_item_broken;

struct Movement_cost {
    i32 foot_slow;
    i32 foot_fast;
    i32 mages;
    i32 riders_slow;
    i32 riders_fast;
    i32 fliers;
    i32 armors;
    i32 pirates;
    i32 bandits;
};

extern const struct Movement_cost Movement_cost_default;

struct Camera {
    /* To zoom in and out on the map */
    Point offset; /* pixels */
    float zoom;
};
extern const struct Camera Camera_default;

typedef struct Timer {
    u64 time_ns;
    u64 frame_count;
    u64 limit_ns;
    b32 reset; /* reset if above time. */
    b32 paused;
} Timer;
extern const struct Timer Timer_default;

void Timer_Init_tnecs(void *voidtimer);
void Timer_Init(struct Timer *timer);

/* Action to be taken by AI */
// By principle AI_Decide_Action should take intermediary action if ultimate
// objective is out of range.
// Ex: attacking unit on the way to seizing a tile
struct AI_Action {
    Point target_move;   /* {-1, -1} if none */
    Point target_action; /* {-1, -1} if none */
    tnecs_entity patient;       /* as in opposite of agent */
    int action;
};

/* AI internal state for game SOTA */
struct Game_AI {
    tnecs_entity *npcs; /* DARR, list of npcs to control */
    /* IES is in control if exists */
    tnecs_entity control;
    int npc_i;  /* index of latest entity */
    b32 init;   /* Did AI init? */
    /* Did AI decide for latest entity */
    b32 decided;
    /* Was move animation done for latest entity */
    b32 move_anim;
    /* Was act  animation done for latest entity */
    b32 act_anim;
    b32 turn_over; /* Is turn over? */
    struct AI_Action action;
};
extern const struct Game_AI Game_AI_default;

/* -- Combat_Phase -- */
// Total attack num in phase = for i < brave_factor -> SUM(skillp_multipliers[i]) * skill_multiplier
// skill_multiplier and skillp_multipliers stack, BUT -> no skills should use both.
typedef struct Combat_Phase {
    b32    attacker;
    // skillp_multipliers: Different multiplier for every brave applies to every attack in phase
    u8     skillp_multipliers[SOTA_BRAVE_MAX];
    // skill_multiplier: Applies to every higher priority attack in phase
    u8     skill_multiplier;
    u8     attack_num;
} Combat_Phase;
extern const struct Combat_Phase Combat_Phase_default;

/* -- Combat_Attack -- */
typedef struct Combat_Attack {
    i32  hit;
    i32  crit;
    i32  attacker;
    i32  total_damage;
} Combat_Attack;
extern const struct Combat_Attack Combat_Attack_default;

/* -- Combat_Flow -- */
// Number of combat phases initiated by each combatants
// (aggressor_phases > 1) -> (defendant_phases <= 1) and vice versa
// Brave: attack multiplier combat for all phases
typedef struct Combat_Flow {
    b32    defendant_retaliates;
    u8     defendant_phases;
    u8     aggressor_phases;
    u8     aggressor_brave;
    u8     defendant_brave;
} Combat_Flow;
extern const struct Combat_Flow Combat_Flow_default;

/* -- Combat_Death -- */
// Can combatants die?
// WILL combatants die?
typedef struct Combat_Death {
    b32 aggressor_certain;
    b32 defendant_certain;
    b32 aggressor_possible;
    b32 defendant_possible;
} Combat_Death;
extern const struct Combat_Death Combat_Death_default;

/* -- Combat_Rates -- */
typedef struct Combat_Rates {
    u8 hit;
    u8 crit;
} Combat_Rates;
extern const struct Combat_Rates Combat_Rates_default;

/* -- Combat_Stats -- */
// All combatant stats related to combats
typedef struct Combat_Stats {
    struct Combat_Rates     agg_rates;
    struct Combat_Rates     dft_rates;
    struct Combat_Damage    agg_damage;
    struct Combat_Damage    dft_damage;
    struct Computed_Stats   agg_stats;
    struct Computed_Stats   dft_stats;
    i8                      agg_equipment[UNIT_ARMS_NUM];
    i8                      dft_equipment[UNIT_ARMS_NUM];
} Combat_Stats;
extern const struct Combat_Stats Combat_Stats_default;

/* -- Combat_Forecast -- */
// All stats required to predict how combat will go,
// before actually doing the RNG check.
typedef struct Combat_Forecast {
    struct Combat_Flow      flow;
    struct Combat_Death     death;
    struct Combat_Stats     stats;
    u8                      phase_num;
    u8                      attack_num;
} Combat_Forecast;
extern const struct Combat_Forecast Combat_Forecast_default;

/* -- Combat_Outcome -- */
// Actual phases and attacks that happen during combat
/* RNG CHECK HAPPENS HERE. */
typedef struct Combat_Outcome {
    struct Combat_Phase   phases[SOTA_COMBAT_MAX_PHASES];
    struct Combat_Attack *attacks;
    i32 current_attack;
    b32 ended; /* death before all attacks */
} Combat_Outcome;
extern const struct Combat_Outcome Combat_Outcome_default;

/* --- RNG SEQUENCE BREAKER (SB) --- */
struct RNG_Sequence { /* Sequence of hits/misses in a row */
    b32 hit;        /* 0 if sequence of misses, 1 of hits */
    i8 len;
    i8 eff_rate;    /* TODO RM */
};

/* --- UNIT --- */
struct Unit_Sequence {
    struct RNG_Sequence hit;
    struct RNG_Sequence crit;

    struct RNG_Sequence hp;
    struct RNG_Sequence str;
    struct RNG_Sequence mag;
    struct RNG_Sequence dex;
    struct RNG_Sequence agi;
    struct RNG_Sequence fth;
    struct RNG_Sequence luck;
    struct RNG_Sequence def;
    struct RNG_Sequence res;
    struct RNG_Sequence con;
    struct RNG_Sequence move;
    struct RNG_Sequence prof;
};

struct Unit_Flags {
    b32 sex;            /* 0:F, 1:M. eg. hasPenis. */
    b32 waits;
    b32 alive;
    b32 literate;       /* Scribe job.  */
    b32 courageous;     /* Story events */
    b32 divine_shield;
    u64 skills;
    bitflag16_t job_talent;
    u16 equippable;
    u16 talkable;
    i8  handedness;
    i8  mvt_type;
};

struct Unit_Rescue {
    u16 id;
};

struct Unit_Render {
    /* Map: which tiles get rendered for unit */
    //  e.g. heal tiles for healers
    i8 rangemap;
    i8 user_rangemap; /* reset to NULL when equipment changes */
    b32 show_danger;
};

struct Support {
    u16 other_id;
    u16 other_type;
    i8  level;
};

struct Unit_Support {
    struct Support arr[SOTA_MAX_SUPPORTS];
    u16 type;
    u16 num;
};

struct Unit_Equipment {
    tnecs_entity           _arr[SOTA_EQUIPMENT_SIZE + 1];
    // struct Inventory_item   arr[SOTA_EQUIPMENT_SIZE + 1];
    i32 num;

    struct Loadout _equipped; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */
};

struct Unit_canEquip {
    /* Design exception:
        - can_equip depends on game state,
        - but, can_equip needs to be hosted somewhere.
    */

    i32 arr[SOTA_EQUIPMENT_SIZE + 1];
    i32 num;

    // TODO: recompute API
    //      - Unit_canEquip(gamestate)
    // When should recompute be set to true?
    //  - Unit movement     (new enemies in range)
    //      - All enemies, self recompute
    //  - Turn end
    //      - All friendlies recompute
    //  - Trading item      (new item to check)
    //      - self recompute
    //  - Using item        (get a new skill)
    //      - Skills can give range
    //      - self recompute
    b32 recompute;
};

struct Unit_Level {
    u16 base_exp;
    u16 exp;
};

struct Unit_Arms {
    i32 num;                    /* of arms */
    i32 hands[UNIT_ARMS_NUM];   /* Which hands? */
};

struct Unit_Stats_Bundle {
    struct Unit_stats  caps;
    struct Unit_stats  bases;
    struct Unit_stats  current; /* Only changes on levelup */
    // Note: unit.stats.current NOT used to track current HP!
    //  - Use unit.HP.current for that!
    struct Unit_stats  growths;
    struct Unit_stats *grown;

    /* Design exception:
        - Bonuses track if their conditions are met themselves.
        - TODO: rename. Not a stack, nor queue.
    */
    struct Bonus_Stats *bonus_stack;
};

struct Unit_Counters {
    i32 agony;
    i32 regrets;
};

struct Unit_IDs {
    i32 self;
    i32 title;
    i32 class;
    i32 army;
    i32 ai;
    i32 mount;
};

struct Unit_Status {
    // TODO: remove. Statuses should be components.
    struct Unit_status *queue;
};

struct HP {
    u8 current;
    u8 overheal;    /* given by heal staves + skill */
    u8 shield;      /* given by shield staff */
};
extern const struct HP HP_default;


typedef struct Unit {
    /* ---------------------- Unit --------------------- *
    **   Represents characters occupying tiles on a map.
    **   Units, move, equip weapons, get afflicted with
    **   statuses, fight, ride mounts, agonize, die...
    **
    **   # Design
    **   ## Members are *constants*, NO dependency on game state
    **      i.e. all EXCEPT self: map, other units, etc...
    **   - base_stats, current_stats do not depend on unit
    **     equipment, neighboring units, auras... -> unit member
    **   - Always compute variable stats by inputing game state:
    **       - effective_stats, computed_stats
    **   ## Record indices, not pointers
    **   - Centralize data into external arrays
    **   - "Out of band": smaller struct
    **   - Fewer dynamic allocs, fewer frees
    **   - Less fragmentation. Faster?

    *  # Terminology
    *  - base_stats
    *  - current_stats     = base_stats + grown_stats
    *  - effective_stats   = current_stats + bonuses
    *  - computed_stats    = func(effective_stats, supports, bonuses)
    */

    struct jsonIO_Header jsonio_header;
    struct HP                   hp;
    struct Unit_IDs             id;
    struct Unit_Arms            arms;
    struct Unit_Flags           flags;
    struct Unit_Level           level;
    struct Unit_Rescue          rescue;
    struct Unit_Render          render;
    struct Unit_Support         support;
    struct Unit_Counters        counters;
    struct Unit_Sequence        rng_sequence;
    struct Unit_canEquip        can_equip;
    struct Unit_Equipment       equipment;
    struct Unit_Stats_Bundle    stats;
} Unit;
extern const struct Unit Unit_default;

struct GraphStat {
    i8 cumul_stat[SOTA_MAX_LEVEL];
    i8 stat_id;
    i16 level;
    i16 base_level;
};
extern const struct GraphStat GraphStat_default;

struct Graph {
    SDL_Rect rect; // x,y,w,h
    SDL_Texture *texture;

    struct GraphStat graph_stats[UNIT_STAT_MALLOC];
    u8 stat_num;
    u8 linestyle;

    Point plot_min; /* [XY units] */
    Point plot_max; /* [XY units] */

    i32 header; /* [pixels] */
    i32 footer; /* [pixels] */
    i32 margin_left;  /* [pixels] */
    i32 margin_right; /* [pixels] */

    /* length until writing another pixel, including pixel */
    /* ant for x */
    i32 y_lenperpixel;

    b32 x_ticks;
    b32 y_ticks;
};
extern const struct Graph Graph_default;

/* --- Bars --- */
struct CircleBar {
    i32 fill;
    SDL_RendererFlip flip;
    Point pos;
};
extern const struct CircleBar CircleBar_default;

struct SimpleBar {
    size_t len; /* [pixels] as overfilled */
    size_t height; /* [pixels] */
    Point pos;
    Point scale;
    SDL_Color BG_dark;
    SDL_Color BG_light;
    SDL_Color FG_dark;
    SDL_Color FG_light;
    SDL_RendererFlip flip;
    float fill;
};
extern const struct SimpleBar SimpleBar_default;

struct GamepadInputMap {
    /* Physical joysticks -> no user change */
    /* Joysticks: [INT_FAST16_MIN, INT_FAST16_MAX] -> [-32768, 32767] */
    SDL_GameControllerAxis    axis_left_x;
    SDL_GameControllerAxis    axis_left_y;
    SDL_GameControllerAxis    axis_right_x;
    SDL_GameControllerAxis    axis_right_y;

    /* Physical dpad -> no user change */
    SDL_GameControllerButton  dpad_right;
    SDL_GameControllerButton  dpad_up;
    SDL_GameControllerButton  dpad_left;
    SDL_GameControllerButton  dpad_down;

    /* Physical buttons -> user can change */
    SDL_GameControllerButton  a;
    SDL_GameControllerButton  b;
    SDL_GameControllerButton  x;
    SDL_GameControllerButton  y;
    SDL_GameControllerButton  start;
    SDL_GameControllerButton  shoulder_left;
    SDL_GameControllerButton  shoulder_right;

    /* Physical triggers -> user can change */
    /* Triggers  [0, INT_FAST16_MAX] -> [0, 32767] */
    SDL_GameControllerButton  trigger_left;
    SDL_GameControllerButton  trigger_right;
};

struct KeyboardInputMap {
    /* Physical dpad -> Keyboard equivalent */
    SDL_Scancode    dpad_right[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_up[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    dpad_down[SOTA_MAPPABLE_BUTTONS_NUM];

    /* Physical buttons -> Keyboard equivalent */
    SDL_Scancode    a[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    b[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    x[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    y[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    start[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    shoulder_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    shoulder_right[SOTA_MAPPABLE_BUTTONS_NUM];

    /* Physical triggers -> Keyboard equivalent */
    /* Triggers  [0, INT_FAST16_MAX] -> [0, 32767] */
    SDL_Scancode    trigger_left[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    trigger_right[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    m[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    space[SOTA_MAPPABLE_BUTTONS_NUM];

    u8  dpad_right_len;
    u8  dpad_up_len;
    u8  dpad_left_len;
    u8  dpad_down_len;
    u8  a_len;
    u8  b_len;
    u8  x_len;
    u8  y_len;
    u8  start_len;
    u8  shoulder_left_len;
    u8  shoulder_right_len;
    u8  trigger_right_len;
    u8  trigger_left_len;
    u8  m_len;
    u8  space_len;
};
extern const struct KeyboardInputMap KeyboardInputMap_default;


/* --- Party --- */
/* Just for JSON loading */
struct Party {
    struct jsonIO_Header jsonio_header;

    s8  folder;
    s8  save_filename;
    s8  load_filename;

    /* Json read values in order of read */
    s8  *json_filenames;
    s8  *json_names;
    i16 *json_ids;

    /* Entities created from json_units */
    /* Always in same order -> UNIT_ID_... */
    tnecs_entity    entities        [SOTA_MAX_PARTY_SIZE]; /* [unit_id] -> entity */

    /* Id stack for units currently in party*/
    i16            *id_stack;

    i32 size;
};
extern const struct Party Party_default;

typedef struct Game_State_Times {
    i8 current;
    i8 previous;
} Game_State_Times;

typedef struct Game_State {
    struct Game_State_Times top; /* aka state */
    struct Game_State_Times sub;
    i8 animation_attack;
    i8 chapter;
} Game_State;

typedef struct Game_Render {
    SDL_Renderer    *er;
    SDL_Texture     *target;
    SDL_Window      *window;
#ifdef SOTA_OPENGL
    GLuint           gl_programid;
    SDL_GLContext    gl_context;
#endif /* SOTA_OPENGL */
} Game_Render;

typedef struct Game_ECS {
    tnecs_component timer_typeflag;
} Game_ECS;

typedef struct Game_Timers {
    tnecs_entity ai;
    tnecs_entity reinf;
    u64    runtime_ns; /* -> millions of years */
} Game_Timers;

typedef struct Game_Flags {
    b32   ismouse;
    b32   iscursor;
    b32   isrunning;
    b32   isShadow;
    b32   fast_forward;
} Game_Flags;

typedef struct Combat {
    struct Combat_Outcome    outcome;
    struct Combat_Forecast   forecast;
    struct Combat_Flow       flow;
    struct Combat_Forecast  *AI_forecasts;

    /* Also use for non-combat: staff, item use... */
    tnecs_entity aggressor;
    tnecs_entity defendant;
} Combat;

typedef struct Game_Fonts {
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_tight;
    struct PixelFont *pixelnours_big;
    struct PixelFont *menu;
} Game_Fonts;

typedef struct Game_FPS {
    /* frames/time after fps_text->update_time_ns */
    f32 instant;
    /* rolling average of fps */
    // float rolling;
    tnecs_entity entity;
} Game_FPS;

typedef struct Game_Audio {
    /* The music that will be played. */
    Mix_Music *music;
    Mix_Chunk *soundfx_cursor;
    Mix_Chunk *soundfx_next_turn;
} Game_Audio;

typedef struct Game_RNG {
    u64 s_xoshiro256ss[4];
} Game_RNG;

typedef struct Game_Inputs {
    struct KeyboardInputMap  keyboardInputMap;
    struct GamepadInputMap   gamepadInputMap;
    // struct MouseInputMap  mouseInputMap;
    i32   controller_type;
    /* Button interpreted as which input */
    u32 arr[SOTA_BUTTON_END];
} Game_Inputs;

typedef struct Game_Debug {
    char reason[DEFAULT_BUFFER_SIZE];
} Game_Debug;

typedef struct Game_Narrative {
    /* gameplay state bitfields, narrative conditions */
    struct Conditions *conditions;
    tnecs_entity cutscene;
    tnecs_entity scene;
} Game_Narrative;

typedef struct Game_Cursor {
    tnecs_entity    entity;
    Point           move;
    Point           lastpos;
    b32             frame_moved;
    b32             diagonal;
    i32             moved_time_ms;
    i8              moved_direction;
} Game_Cursor;

typedef struct Game_Mouse {
    tnecs_entity entity;
} Game_Mouse;

typedef struct Game_Selected {
    tnecs_entity unit_entity;
    Point        unit_initial_position;
    Point        unit_moved_position;
    i8           menu_option;
} Game_Selected;

typedef struct Game_Popups {
    tnecs_entity arr[POPUP_TYPE_NUM];
    tnecs_entity pre_combat;
} Game_Popups;

typedef struct Game_Hovered {
    tnecs_entity unit_entity;
} Game_Hovered;

typedef struct Game_Menus {
    tnecs_entity *stack;
    tnecs_entity player_select[MENU_PLAYER_SELECT_NUM];
    tnecs_entity item_select;
    tnecs_entity trade;
    tnecs_entity staff_select;
    tnecs_entity weapon_select;
    tnecs_entity stats;
    tnecs_entity growths;
    tnecs_entity deployment;
    s8 filename;
} Game_Menus;

typedef struct Game_Title_Screen {
    tnecs_entity menu; /* i.e. first_menu */
    tnecs_entity title;
} Game_Title_Screen;

typedef struct Game_Targets {
    /* -- Chosen by player -- */
    /* Order of target in any candidates array */
    int order;
    int previous_order;

    /* ptr to other target list, used by choosecandidates */
    tnecs_entity *candidates;       /* [order] */
    /* --- on attackmap --- */
    tnecs_entity *defendants;       /* combat */
    tnecs_entity *patients;         /* staff */
    /* --- on neighbouring tiles --- */
    tnecs_entity *victims;          /* rescue */
    tnecs_entity *spectators;       /* dance */
    tnecs_entity *auditors;         /* talk */
    tnecs_entity *passives;         /* trade */
    tnecs_entity *openables;        /* doors and chests */
    tnecs_entity *deployed;         /* deployment positions */
} Game_Targets;

/* --- Pathfinding --- */
typedef struct PathfindingAct {
    i32             *movemap;
    i32             *acttomap;
    tnecs_entity    *occupymap;
    Range            range;
    Point            point;
    tnecs_entity     self;
    i32              col_len;
    i32              row_len;
    i32              mode_movetile;
} PathfindingAct;
extern const PathfindingAct PathfindingAct_default;

struct TextLines {
    char **lines;
    int   *lines_len;
    int    line_num;
    int    line_len;
};
extern const struct TextLines TextLines_default;

#endif /* STRUCTS_H */
