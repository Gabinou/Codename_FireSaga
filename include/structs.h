#ifndef STRUCTS_H
#define STRUCTS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "nstr.h"
#include "tnecs.h"
#include "SDL.h"
#include "SDL_mixer.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;

/* --- FUNCTIONS --- */
typedef i32(* use_function_t)(struct Item *, struct Unit *, struct Unit *);

/* --- STRUCTS --- */
/* -- Loadout -- */
typedef struct Loadout {
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */
} Loadout;

/* --- JSON HEADER --- */
// All structs that use jsonio need this header as the first element
// TODO: use jsonIO_Header everywhere.
typedef struct jsonIO_Header {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */
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
    // First to be able to cast: Loadout = &canEquip;
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    // Hand to equip to, considering loadout.
    i32 hand;

    // Equipped index to check.
    i32 _eq; /* [ITEM1, SOTA_EQUIPMENT_SIZE] */

    // Why do we need to know archetypes?
    //      - Find all equippable staves ONLY
    //      - Find all equippable weapons (of any type) ONLY
    i64 archetype;

    // Two-handed mode.
    //  - TWO_HAND_EQ_MODE_STRICT
    //      - Can only equip if weapon is currently in other hand
    //  - TWO_HAND_EQ_MODE_LOOSE
    //      - Can equip even in one hand -> step 1 to equip in two hands
    //      - Equivalent to TWO_HAND_EQ_MODE_STRICT with to_eq in other hand
    //      - Needed by LoadoutSelectMenu
    i32 two_hands_mode;

    /* LOADOUT_EQUIPPED, LOADOUT_EQUIPMENT, LOADOUT_INPUT */
    i32 eq_type;

    // For Map_canEquip
    //  - Find canEquip item in range, with or without unit movement
    b32 move;
} canEquip;
extern const canEquip canEquip_default;

struct Cursor {
    i16 frames;
    i16 speed;
};

struct Mouse {
    u8 onhold;
    u8 move;
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
struct Music_settings {
    i32 frequency;          /*  [Hz]    */
    i32 sample_size;        /* [byte]   */
    i32 channels;
    u16 format;             /* AUDIO_*  */
};

struct Map_settings {
    u8          overlay_mode; /* tile paletteswap or opaque overlay */
    u8          stack_mode;
    u8          grid_thickness; /* Number of lines to draw (mirrored) */
    u8          perim_thickness; /* Number of lines to draw (mirrored) */
    SDL_Color   color_grid;
    b32        grid_show;
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
    Point res; /* resolution */
    Point pos;
    struct Fps   FPS;

    struct Cursor cursor; /* 32 bits */
    u16 tilesize[TWO_D];

    struct Map_settings map_settings;
    struct Music_settings music_settings;
    float brightness;
    struct Enemy_Turn_settings enemy_turn_settings;

    struct Mouse mouse; /* 16 bits */
    char title[DEFAULT_BUFFER_SIZE];

    u16 fontsize;
    int music_volume;
    int soundfx_volume;
    u32 window;
    struct Input_Arguments args;
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
    i32 total;
} Damage_Raw;
extern const struct Damage_Raw Damage_Raw_default;

typedef struct Combat_Damage {
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
    union Graphics graphics;
    SDL_RendererFlip flip;
    double angle;
};
extern const struct Rendered Rendered_default;

struct Tile_stats {
    i8 dodge;
    i8 Pprot;
    i8 Mprot;
    i8 heal; /* % Negative means damage. */
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

struct HP {
    b32 divine; /* divine shield */
    u8 max;
    u8 current;
    u8 overheal;
};
extern const struct HP HP_default;

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
    struct Range range; /* of attack [0]: min, [1]: max */
    i32 hit;
    i32 dodge;  /* when the Sword is TOO HEAVY TO DODGE */
    i32 crit;
    i32 favor;
    i32 wgt;    /* weight */
    i32 prof;   /* proficiency */
};
extern const struct Weapon_stats Weapon_stats_default;

struct Shop {
    i16 shopkeeper;
    i16 *items;
    i8  *qty;  /* < 0 means infinity */
};
extern const struct Shop Shop_default;

typedef struct Inventory_item {
    i32 id;
    b32 highlighted;
    u8 used;
    i8 infusion;
    /* item images are highlighted by default. */
    /* Only dark when in unit inventory and unequippable */
}   Inventory_item;
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
    Point offset; /* pixels */
    float        zoom;
};

typedef struct Timer {
    u64 time_ns;
    u64 frame_count;
    u64 limit_ns;
    b32 reset; /* reset if above time. */
    b32 paused;
} Timer;
extern const struct Timer Timer_default;

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
struct AI_State {
    tnecs_entity *npcs; /* DARR, list of npcs to control */
    int npc_i;          /* index of latest entity */
    b32 init;           /* Did AI init? */
    b32 decided;        /* Did AI decide for latest entity */
    b32 move_anim;      /* Was move animation done for latest entity */
    b32 act_anim;       /* Was act  animation done for latest entity */
    b32 turn_over;      /* Is turn over? */
    struct AI_Action action;
};
extern const struct AI_State AI_State_default;

typedef struct Convoy {
    struct jsonIO_Header jsonio_header;

    struct Inventory_item books[SOTA_BOOKS_NUM];
    struct Inventory_item items[SOTA_CONVOY_SIZE_MAX];
    u8 cumnum[ITEM_TYPE_NUM + 1]; // items are [cumnum1, cumnum2)

    u8 books_num;
    u8 items_num;
    u8 size;
    i16 bank; // [gold]
    b32 sort_direction;
} Convoy;

extern const struct Convoy Convoy_default;

/* -- Combat_Phase -- */
// Total attack num in phase = for i < brave_factor -> SUM(skillp_multipliers[i]) * skill_multiplier
// skill_multiplier and skillp_multipliers stack, BUT -> no skills should use both.
struct Combat_Phase {
    b32    attacker;
    // skillp_multipliers: Different multiplier for every brave applies to every attack in phase
    u8     skillp_multipliers[SOTA_BRAVE_MAX];
    // skill_multiplier: Applies to every higher priority attack in phase
    u8     skill_multiplier;
    u8     attack_num;
};
extern const struct Combat_Phase Combat_Phase_default;

/* -- Combat_Attack -- */
struct Combat_Attack {
    b32    hit;
    b32    crit;
    b32    attacker;
    u8     total_damage; // total damage taken, depending on hit/crit
};
extern const struct Combat_Attack Combat_Attack_default;

/* -- Combat_Flow -- */
// Number of combat phases initiated by each combatants
// (aggressor_phases > 1) -> (defendant_phases <= 1) and vice versa
// Brave: attack multiplier combat for all phases
struct Combat_Flow {
    b32    defendant_retaliates;
    u8     defendant_phases;
    u8     aggressor_phases;
    u8     aggressor_brave;
    u8     defendant_brave;
};
extern const struct Combat_Flow Combat_Flow_default;

/* -- Combat_Death -- */
// Can combatants die?
// WILL combatants die?
struct Combat_Death {
    b32 aggressor_certain;
    b32 defendant_certain;
    b32 aggressor_possible;
    b32 defendant_possible;
};
extern const struct Combat_Death Combat_Death_default;

/* -- Combat_Rates -- */
typedef struct Combat_Rates {
    u8 hit;
    u8 crit;
} Combat_Rates;
extern const struct Combat_Rates Combat_Rates_default;

/* -- Combat_Stats -- */
// All combatant stats related to combats
struct Combat_Stats {
    struct Combat_Rates     agg_rates;
    struct Combat_Rates     dft_rates;
    struct Combat_Damage    agg_damage;
    struct Combat_Damage    dft_damage;
    struct Computed_Stats   agg_stats;
    struct Computed_Stats   dft_stats;
    i8                      agg_equipment[UNIT_ARMS_NUM];
    i8                      dft_equipment[UNIT_ARMS_NUM];
};
extern const struct Combat_Stats Combat_Stats_default;

/* -- Combat_Forecast -- */
// All stats required to predict how combat will go,
// before actually doing the RNG check.
struct Combat_Forecast {
    struct Combat_Flow      flow;
    struct Combat_Death     death;
    struct Combat_Stats     stats;
    u8                      phase_num;
    u8                      attack_num;
};
extern const struct Combat_Forecast Combat_Forecast_default;

/* -- Combat_Outcome -- */
// Actual phases and attacks that happen during combat
/* RNG CHECK HAPPENS HERE. */
struct Combat_Outcome {
    struct Combat_Phase   phases[SOTA_COMBAT_MAX_PHASES];
    struct Combat_Attack *attacks;
    int current_attack;
    b32 ended; /* death before all attacks */
};
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
    b32 mounted;
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
    struct Inventory_item arr[SOTA_EQUIPMENT_SIZE + 1];
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
    i32 num;
    b32 hands[UNIT_ARMS_NUM]; /* Does unit have hands? */
};

struct Unit_Stats_Bundle {
    struct Unit_stats  caps;
    struct Unit_stats  bases;
    struct Unit_stats  current; /* Only changes on levelup */
    struct Unit_stats  growths;
    struct Unit_stats *grown;

    /* Design exception:
        - Bonuses track if their conditions are met themselves.
        - TODO: rename. Not a stack, nor queue.
    */
    struct Bonus_Stats *bonus_stack;
};

struct Unit_Counters {
    i32 hp;
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

struct Unit_Statuses {
    // TODO: remove. Statuses should be components.
    struct Unit_status *queue;
};

typedef struct Unit {
    /* ---------------------- Unit --------------------- *
    *  Represents characters occupying tiles on a map.
    *  Units, move, equip weapons, get afflicted with
    *  statuses, fight, ride mounts, agonize, die...
    *
    *  # Design
    *  ## Members are *constants*, NO dependency on game state
    *  - Game state EXCEPT self: map, other units, etc...
    *  - base_stats, current_stats do not depend on unit
    *    equipment, neighboring units, auras... -> unit member
    *  - Always compute variable stats by inputing game state:
    *      - effective_stats, computed_stats
    *  ## Record indices, not pointers
    *  - Centralize data into external arrays
    *      - Ex: all possible names in *global_unitNames*
    *  - "Out of band": smaller struct
    *  - Fewer dynamic allocs, fewer frees
    *  - Less fragmentation. Faster?

    *  # Terminology
    *  - base_stats
    *  - current_stats     = base_stats + grown_stats
    *  - effective_stats   = current_stats + bonuses
    *  - computed_stats    = func(effective_stats, supports, bonuses)
    */

    struct jsonIO_Header jsonio_header;

    struct Unit_IDs             id;
    struct Unit_Sequence        rng_sequence;
    struct Unit_Flags           flags;
    struct Unit_Rescue          rescue;
    struct Unit_Render          render;
    struct Unit_Support         support;
    struct Unit_Level           level;
    struct Unit_Equipment       equipment;
    struct Unit_canEquip        can_equip;
    struct Unit_Arms            arms;
    struct Unit_Stats_Bundle    stats;
    struct Unit_Counters        counters;
    struct Unit_Statuses        statuses; // TODO: rm
} Unit;
extern const struct Unit Unit_default;

struct GraphStat {
    i16 level;
    i16 base_level;
    i8 cumul_stat[SOTA_MAX_LEVEL];
    i8 stat_id;
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
    u8 y_lenperpixel;

    b32 x_ticks : 1;
    b32 y_ticks : 1;
};
extern const struct Graph Graph_default;

/* --- Bars --- */
struct CircleBar {
    int fill;
    SDL_RendererFlip flip;
    Point pos;
};
extern const struct CircleBar CircleBar_default;

struct SimpleBar {
    float fill;
    size_t len; /* [pixels] as overfilled */
    size_t height; /* [pixels] */
    Point pos;
    Point scale;
    SDL_Color BG_dark;
    SDL_Color BG_light;
    SDL_Color FG_dark;
    SDL_Color FG_light;
    SDL_RendererFlip flip;
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

    s8   folder;

    /* Json read values in order of read */
    s8              *json_filenames;
    s8              *json_names;
    i16             *json_ids;

    /* Entities created from json_units */
    /* Always in same order -> UNIT_ID_... */
    tnecs_entity    entities        [SOTA_MAX_PARTY_SIZE]; /* [unit_id] -> entity */

    /* Id stack for units currently in party*/
    i16            *id_stack;

    i32 size;
};
extern const struct Party Party_default;


/* --- Game Object --- */
typedef struct Game {
    SDL_Renderer    *renderer;
    SDL_Texture     *render_target;
#ifdef SOTA_OPENGL
    GLuint         gl_programid;
    SDL_GLContext  gl_context;
    SDL_Window     *gl_window;
#endif /* SOTA_OPENGL */
    // world is mostly world_render
    // only entity in world_control is cursor
    tnecs_world  *world;
    tnecs_world  *world_render;
    tnecs_world  *world_control;

    tnecs_component timer_typeflag;
    Point  cursor_lastpos;

    struct dtab *menu_options_dtab;
    struct dtab *defaultstates_dtab;

    struct dtab *tiles_loaded_dtab;
    struct dtab *units_loaded_dtab;

    tnecs_entity ai_timer;
    tnecs_entity reinf_timer;

    struct KeyboardInputMap  keyboardInputMap;
    struct GamepadInputMap   gamepadInputMap;
    // struct MouseInputMap  mouseInputMap;

    struct Camera camera;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_tight;
    struct PixelFont *pixelnours_big;
    struct PixelFont *menu_pixelfont;
    struct Map *map;

    /* gameplay state bitfields, narrative conditions */
    struct Conditions *conditions;

    tnecs_entity *menu_stack;
    tnecs_entity player_select_menus[MENU_PLAYER_SELECT_NUM]; /* [PLAYER_SELECT_MENU_...] */
    tnecs_entity popups[POPUP_TYPE_NUM]; /* [POPUP_TYPE_...] */
    tnecs_entity item_select_menu;
    tnecs_entity trade_menu;
    tnecs_entity staff_select_menu;
    tnecs_entity weapon_select_menu;
    tnecs_entity stats_menu;
    tnecs_entity scene;
    tnecs_entity cutscene;
    tnecs_entity pre_combat_popup;
    tnecs_entity first_menu;
    tnecs_entity title;
    tnecs_entity growths_menu;
    tnecs_entity deployment_menu;

    s8 filename_menu;
    char reason[DEFAULT_BUFFER_SIZE];

    tnecs_entity entity_cursor;
    tnecs_entity entity_mouse;
    tnecs_entity entity_transition;
    tnecs_entity entity_highlighted;
    tnecs_entity entity_shadowed;
    tnecs_entity entity_fps;
    tnecs_entity selected_unit_entity;
    tnecs_entity hovered_unit_entity;

    i8 moved_direction;
    i8 selected_menu_option;

    Point selected_unit_initial_position;
    Point selected_unit_moved_position;

    tnecs_entity *map_enemies;
    struct Party party;

    tnecs_entity *ent_unit_loaded;

    i32  cursor_moved_time_ms;

    struct Combat_Outcome    combat_outcome;
    struct Combat_Forecast   combat_forecast;
    struct Combat_Flow       combat_flow;
    struct Combat_Forecast  *AI_forecasts;

    i8 animation_attack;
    i8 chapter;
    i8 state;
    i8 substate;
    i8 state_previous;
    i8 substate_previous;

    SDL_Window *window;

    struct Settings    settings;
    struct Convoy      convoy;
    u64 s_xoshiro256ss[4]; /* Only used to read s from RNG file */

    Point cursor_move;
    b32 cursor_frame_moved;
    b32 cursor_diagonal;

    /* --- FPS --- */
    float instant_fps; /* frames/time after fps_text->update_time_ns */
    float rolling_fps; /* rolling average of fps */

    /* --- COMBAT --- */
    int candidate;          // potential candidate
    int previous_candidate; // previously selected candidate
    /* -- Chosen by player -- */
    // Also use for non-combat: staff, item use...
    tnecs_entity aggressor; // combat -> player unit
    tnecs_entity defendant; // combat -> player chose

    /* -- Choices list for player -- */

    /* --- UNIT ACTION CANDIDATES --- */
    // copy of one other psm list, used by choosecandidates
    tnecs_entity *candidates;
    /* on attackmap */
    tnecs_entity *defendants;     // combat
    tnecs_entity *patients;       // staff
    /* on neighbouring tiles */
    tnecs_entity *victims;        // rescue
    tnecs_entity *spectators;     // dance
    tnecs_entity *auditors;       // talk
    tnecs_entity *passives;       // trade
    tnecs_entity *openables;      // doors and chests
    tnecs_entity *deployed;       // deployment unit placement

    struct AI_State ai_state;

    i32   controller_code;
    /* Button interpreted as which input?  */
    u32 inputs[SOTA_BUTTON_END];

    /* The music that will be played. */
    Mix_Music *music;
    Mix_Chunk *soundfx_cursor;
    Mix_Chunk *soundfx_next_turn;

    u64    runtime_ns; // -> millions of years
    b32  *shadow_area;  /* pixels */
    b32   ismouse          : 1;
    b32   iscursor         : 1;
    b32   isrunning        : 1;
    b32   isShadow         : 1;
    b32   fast_forward     : 1;
} Game;
extern const struct Game Game_default;

typedef struct Item {
    struct jsonIO_Header jsonio_header;

    struct Range range;
    struct Aura aura; /* Aura with range [0, 0] to for wielder-only bonus */

    struct Item_stats stats;
    u16  id;           /* 0 is NULL */
    u8 target;          /* units on which item is usable. */
    b32 canSell     : 1;
    b32 write_stats : 1;
    b32 canUse      : 1;
    b32 canRepair   : 1; /* TODO: Move to weapon? */
    u64  type;         /* and not type_exp */
    u16 *users;        /* item only usable by users.   NULL -> everyone */
    u16 *classes;      /* item only usable by classes. NULL -> everyone */

    /* -- Item Effects -- */
    u64 passive;

    /* Use function is used for Staves effects too. */
    use_function_t active; /* NULL if not usable */

    s8 name;
    char description[DEFAULT_BUFFER_SIZE * 2];

} Item;
extern const struct Item Item_default;

typedef struct Weapon {
    struct jsonIO_Header jsonio_header;
    Item         *item;
    struct Weapon_stats  stats;

    u8 handedness;
    u8 subtype;        /* ex: thrust swords     */
    u16 effective;
    b32 isMagic;
    b32 canAttack; /* for special weapons   */
} Weapon;
extern const struct Weapon Weapon_default;

typedef struct Arrow {
    i32 *costmap;
    i32 *movemap;
    i32 move;
    i32 col_len;
    i32 row_len;
    b32 show;
    i32 *pathlist;  /* from initial unit position to current cursor position */
    i32 tilesize[TWO_D];
    i32 map_tilesize[TWO_D];
    struct Point start;
    struct SDL_Texture *textures;  /* [patch_id] */
    struct Rendered rendereds[SOTA_MAX_MOVEMENT]; /* [patch_id] */
} Arrow;
extern const struct Arrow Arrow_default;

/* --- Pathfinding --- */
typedef struct PathfindingAct {
    i32             *movemap;
    i32             *acttomap;
    tnecs_entity    *occupymap;

    i32              col_len;
    i32              row_len;
    i32              mode_movetile;
    tnecs_entity     self;

    Range            range;
    Point            point;
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
