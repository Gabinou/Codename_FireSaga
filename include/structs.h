#ifndef STRUCTS_H
#define STRUCTS_H

#include <math.h>
#include "types.h"
#include "debug.h"
#include "enums.h"
#include "nmath.h"
#include "nstr.h"
#include "tnecs.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"


/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Item;

/* --- FUNCTIONS --- */
typedef i32(* use_function_t)(struct Item *, struct Unit *, struct Unit *);

/* --- STRUCTS --- */
/* -- Loadout -- */
typedef struct Loadout {
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM_UNEQUIPPED, SOTA_EQUIPMENT_SIZE] */
} Loadout;

extern Loadout Loadout_default;
/* -- attackto -- */
typedef struct MapAct {
    // First to be able to cast: Loadout = &canEquip;
    /* only if eq_type == LOADOUT_INPUT */
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM_UNEQUIPPED, SOTA_EQUIPMENT_SIZE] */

    b32 move;

    // healtomap: ITEM_ARCHETYPE_STAFF
    // attacktomap: ITEM_ARCHETYPE_WEAPON
    i64 archetype;

    /* ARRAY_MATRIX, ARRAY_LIST */
    i32 output_type;

    /* LOADOUT_EQUIPPED, LOADOUT_EQUIPMENT, LOADOUT_INPUT */
    i32 eq_type;

    i32 mode_movetile;

    tnecs_entity aggressor; /* or healer    */
    tnecs_entity defendant; /* or patient   */

} MapAct;
extern MapAct MapAct_default;

/* -- Can Equip -- */
// Input for canEquip function
typedef struct canEquip {
    // First to be able to cast: Loadout = &canEquip;
    i32 _loadout[MAX_ARMS_NUM]; /* [ITEM_UNEQUIPPED, SOTA_EQUIPMENT_SIZE] */

    // Hand to equip to, considering loadout.
    i32 hand;

    // Why do we need to know archetypes?
    //      - Find all equippable staves ONLY
    //      - Find all equippable weapons (of any type) ONLY
    i64 archetype;

    // Equipped index to check.
    i32 _eq; /* [ITEM_UNEQUIPPED, SOTA_EQUIPMENT_SIZE] */

    // Two-handed mode.
    //  - TWO_HAND_EQ_MODE_STRICT
    //      - Can only equip if weapon is currently in other hand
    //  - TWO_HAND_EQ_MODE_LOOSE
    //      - Can equip even in one hand -> step 1 to equip in two hands
    //      - Equivalent to TWO_HAND_EQ_MODE_STRICT with to_eq in other hand
    //      - Needed by LoadoutSelectMenu
    i32 two_hands_mode;

    // For Map_canEquip
    //  - Find canEquip item in range, with or without unit movement
    b32 move;
} canEquip;
extern canEquip canEquip_default;

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
    int cap;    /* [Hz] [s^-1] */
    int ff_cap; /* [%] Fast forward cap above FPS */
};


/* --- Settings --- */
struct Music_settings {
    i32 frequency;          /*  [Hz]    */
    i32 sample_size;        /* [byte]   */
    u16 format;             /* AUDIO_*  */
    i32 channels;
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
    u16   cutScene;         /* plays the scene then exits */
    u16   talkScene;
    u8    map_index;
    u8    startup_mode;
    b32   print_help;       /* If help requested anywhere, print help and exit  */
    char *save_filename;    /* debug saves are outside normal save integers     */
} Input_Arguments;
extern struct Input_Arguments Input_Arguments_default;

typedef struct Settings {
    Point res; /* resolution */
    Point pos;
    struct Fps   FPS;

    struct Cursor cursor; /* 32 bits */
    u16 tilesize[TWO_D];
    u8 fontsize;

    struct Map_settings map_settings;
    struct Music_settings music_settings;
    struct Enemy_Turn_settings enemy_turn_settings;

    struct Mouse mouse; /* 16 bits */
    char title[DEFAULT_BUFFER_SIZE];

    int music_volume;
    int soundfx_volume;
    float brightness;

    struct Input_Arguments args;

    u32 window;
    b32 fullscreen          : 1;
} Settings;
extern struct Settings Settings_default;

typedef struct MenuElemDirections {
    i8 right;
    i8 top;
    i8 left;
    i8 bottom;
} MenuElemDirections;
extern MenuElemDirections MenuElemDirections_default;

struct Damage {
    i32 dmg[DAMAGE_TYPES];
    i32 dmg_crit[DAMAGE_TYPES];
};
extern struct Damage Damage_default;

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
typedef struct Unit_stats {
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
extern struct Unit_stats Unit_stats_default;

/* Struct is better: Can be cast to array */
typedef struct Range {
    i32 min;
    i32 max;
} Range;
extern struct Range Range_default;

/* Imagine I would implement a range with gaps */
struct RangeGaps {
    i32 canatt[SOTA_MAX_RANGE];
};
extern struct Range Range_default;

typedef struct Computed_Stats {
    i32 attack[DAMAGE_TYPES];
    i32 protection[DAMAGE_TYPES];
    i32 hit;
    i32 dodge; /* can be negative */
    i32 crit;
    i32 favor;
    i32 move;
    i32 speed; /* relative to agi so +/- */
    i32 agony;
    struct Range range_equipment;   /* Range of all equipment */
    struct Range range_loadout;     /* Range of equipped weapons */
    // issue:       What about staff AND weapon equipped?
    // Solution:
    //          - Only one is shown at a time: Show only one at a time
    //          - Show two at atime with Blue+Red+Purple filter
} Computed_Stats;
extern struct Computed_Stats Computed_Stats_default;

void Computed_Stats_Print(  struct Computed_Stats *stats);
void Computed_Stats_Compare(struct Computed_Stats *stats1,
                            struct Computed_Stats *stats2);

/* Bonus stats added to unit, including decay/removal conditions */
typedef struct Bonus_Stats {
    /* Stat bonuses */
    struct Unit_stats       unit_stats;
    struct Computed_Stats   computed_stats;
    /* Conditions */
    struct Range range;
    tnecs_entity source_unit;
    u16 source_item;              /* Should be equipped by unit_ent */
    u16 source_skill;
    b32 active;
    i32 turns;
} Bonus_Stats;
extern struct Bonus_Stats Bonus_Stats_default;

struct Condition {
    /* Conversation condition? */
    i16 unitid;
    b32 dead;
    b32 recruited;
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
    b32 divine; /* divine shield */
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
extern struct Padding Padding_default;

struct Item_stats {
    i32 price;
    i32 uses;
    i32 AP; /* ability power: for heal (%), blowHorn, gainStats, gainSkill, Repair */
};
extern struct Item_stats Item_stats_default;

struct Crit_Multiplier {
    u8 num;
    u8 denom;
};
extern struct Crit_Multiplier Crit_Multiplier_default;

typedef struct Aura {
    struct Range            range; /* [0]: min, [1]: max */
    struct Unit_stats       unit_stats;
    struct Computed_Stats   computed_stats;
    i32 turns;
} Aura;
extern struct Aura Aura_default;

struct Weapon_stats {
    i32 attack[ATTACK_TYPES_NO_TOTAL];
    i32 protection[PROTECTION_TYPES_NO_TOTAL];
    struct Range range; /* of attack [0]: min, [1]: max */
    i32 hit;
    i32 dodge;  /* when the Sword is TOO HEAVY TO DODGE */
    i32 crit;
    i32 favor;
    i32 wgt;    /* weight */
    i32 prof;   /* proficiency */
};
extern struct Weapon_stats Weapon_stats_default;

struct Shop {
    i16 shopkeeper;
    i16 *items;
    i8  *qty;  /* < 0 means infinity */
};
extern struct Shop Shop_default;

typedef struct Inventory_item {
    i32 id;
    u8 used;
    b32 highlighted;
    /* item images are highlighted by default. */
    /* Only dark when in unit inventory and unequippable */
    i8 infusion;
}   Inventory_item;
extern struct Inventory_item Inventory_item_default;
extern struct Inventory_item Inventory_item_broken;

// struct Reinforcement {
//     Point position;
//     i16 army;
//     // i16 id;
//     s8 filename;
//     // TODO: 1 or 2
//     // 1- Use unit filename instead of id
//     //      - Gets rid of name from file vs name from id conflict
//     //      - Gets rid of name id in file vs id in reinforcement issue
//     // 2- Make ai id and use it instead of ai_filename
//     //      - Have to free strings
//     //      - char array?
//     s8 ai_filename; /* Overrides ai_filename in unit */
//     u8 turn;
//     u8 levelups;
// };
// extern struct Reinforcement Reinforcement_default;

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

struct Camera {
    Point offset; /* pixels */
    float        zoom;
};

typedef struct Timer {
    u64 time_ns;
    u64 frame_count;
    b32 reset; /* reset if above time. */
    b32 paused;
    u64 limit_ns;
} Timer;
extern struct Timer Timer_default;

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
extern struct AI_State AI_State_default;

typedef struct Convoy {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    struct dtab *weapons_dtab;

    struct Inventory_item books[SOTA_BOOKS_NUM];
    struct Inventory_item items[SOTA_CONVOY_SIZE_MAX];
    u8 cumnum[ITEM_TYPE_NUM + 1]; // items are [cumnum1, cumnum2)

    i16 bank; // [gold]
    u8 books_num;
    u8 items_num;
    u8 size;
    b32 sort_direction;
} Convoy;

extern struct Convoy Convoy_default;

/* -- Combat_Phase -- */
// Total attack num in phase = for i < brave_factor -> SUM(skillp_multipliers[i]) * skill_multiplier
// skill_multiplier and skillp_multipliers stack, BUT -> no skills should use both.
struct Combat_Phase {
    // skillp_multipliers: Different multiplier for every brave applies to every attack in phase
    u8     skillp_multipliers[SOTA_BRAVE_MAX];
    // skill_multiplier: Applies to every higher priority attack in phase
    u8     skill_multiplier;
    u8     attack_num;
    b32    attacker;
};
extern struct Combat_Phase Combat_Phase_default;

/* -- Combat_Attack -- */
struct Combat_Attack {
    u8     total_damage; // total damage taken, depending on hit/crit
    b32    hit;
    b32    crit;
    b32    attacker;
};
extern struct Combat_Attack Combat_Attack_default;

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
extern struct Combat_Flow Combat_Flow_default;

/* -- Combat_Death -- */
// Can combatants die?
// WILL combatants die?
struct Combat_Death {
    b32 aggressor_certain;
    b32 defendant_certain;
    b32 aggressor_possible;
    b32 defendant_possible;
};
extern struct Combat_Death Combat_Death_default;

/* -- Combat_Rates -- */
struct Combat_Rates {
    u8 hit;
    u8 crit;
} ;
extern struct Combat_Rates Combat_Rates_default;

/* -- Combat_Stats -- */
// All combatant stats related to combats
struct Combat_Stats {
    struct Combat_Rates     agg_rates;
    struct Combat_Rates     dft_rates;
    struct Damage           agg_damage;
    struct Damage           dft_damage;
    struct Computed_Stats   agg_stats;
    struct Computed_Stats   dft_stats;
    i8                      agg_equipment[UNIT_ARMS_NUM];
    i8                      dft_equipment[UNIT_ARMS_NUM];
};
extern struct Combat_Stats Combat_Stats_default;

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
extern struct Combat_Forecast Combat_Forecast_default;

/* -- Combat_Outcome -- */
// Actual phases and attacks that happen during combat
/* RNG CHECK HAPPENS HERE. */
struct Combat_Outcome {
    struct Combat_Phase   phases[SOTA_COMBAT_MAX_PHASES];
    struct Combat_Attack *attacks;
    int current_attack;
    b32                   ended; /* death before all attacks */
};
extern struct Combat_Outcome Combat_Outcome_default;

/* --- RNG SEQUENCE BREAKER (SB) --- */
struct RNG_Sequence { /* Sequence of hits/misses in a row */
    i8 len;
    i8 eff_rate;
    b32 hit; /* 0 if sequence of misses, 1 of hits */
};


/* --- UNIT --- */
struct Support {
    u16 other_id;
    u16 other_type;
    i8 level;
};

typedef struct Unit {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    i16 class;
    i8  mvt_type;
    i8  army;
    u8  current_hp;
    i8  handedness;
    u16 talkable;
    u8  current_agony;
    i8  agony; /* turns left before death (-1 not agonizing) */
    u8  regrets;

    // Status with least remaining turns on top.
    struct Unit_status *status_queue;

    // TODO: change into ID for AI?
    s8 ai_filename; /* Default AI for unit */

    struct Support supports[SOTA_MAX_SUPPORTS];
    u16 support_type;
    u16 support_num;
    struct Damage damage;

    /* Stats */
    struct Unit_stats base_stats;

    struct Bonus_Stats *bonus_stack;
    struct Unit_stats bonus_stats; // TODO remove for new Bonus_Stat Struct
    struct Unit_stats malus_stats; // TODO remove for new Bonus_Stat Struct
    struct Unit_stats caps_stats;
    struct Unit_stats current_stats;    /* base_stats + all growths */
    struct Unit_stats effective_stats;  /* current_stats + bonuses/maluses */

    /* Growths */
    struct Unit_stats growths;
    struct Unit_stats bonus_growths;
    struct Unit_stats effective_growths;
    struct Unit_stats *grown_stats;

    u64 skills;

    struct RNG_Sequence hit_sequence;
    struct RNG_Sequence crit_sequence;

    struct RNG_Sequence hp_sequence;
    struct RNG_Sequence str_sequence;
    struct RNG_Sequence mag_sequence;
    struct RNG_Sequence dex_sequence;
    struct RNG_Sequence agi_sequence;
    struct RNG_Sequence fth_sequence;
    struct RNG_Sequence luck_sequence;
    struct RNG_Sequence def_sequence;
    struct RNG_Sequence res_sequence;
    struct RNG_Sequence con_sequence;
    struct RNG_Sequence move_sequence;
    struct RNG_Sequence prof_sequence;

    u16 equippable;
    u16 base_exp;
    u16 exp;
    u16 _id;
    u16 rescuee;

    i8 rangemap;
    i8 user_rangemap; /* reset to NULL when equipment changes */

    bitflag16_t job_talent;

    /* Defendant position (self is Aggressor.) */
    Point dft_pos; /* Used to compute stats in case of dual wielding */

    i32     arms_num;
    b32     hands[MAX_ARMS_NUM]; /* Does unit have hands?             */
    i32 _equipped[MAX_ARMS_NUM]; /* [ITEM_UNEQUIPPED, SOTA_EQUIPMENT_SIZE] */

    s8 *skill_names;

    struct dtab *weapons_dtab;
    struct dtab *items_dtab;

    struct Inventory_item _equipment[SOTA_EQUIPMENT_SIZE];

    /* For twohanding when computing computedstats */
    struct Inventory_item temp;

    i32 eq_canEquip[SOTA_EQUIPMENT_SIZE];
    i32 num_equipment;
    i32 num_canEquip;

    struct Mount *mount;
    b32 mounted;

    s8 name;        /* TODO: get rid of it. Use id for global_unitNames */
    s8 title;       /* TODO: get rid of it. Use ir for global_unitTitles */

    struct Computed_Stats computed_stats;   /* Computed from Unit_Stats */

    // TODO: get rid of support_bonuses
    // struct Computed_Stats support_bonuses[SOTA_MAX_SUPPORTS];
    // struct Computed_Stats support_bonus;

    b32 sex;            /* 0:F, 1:M. eg. hasPenis. */
    b32 waits;
    b32 alive;
    b32 literate;       /* Reading/writing for scribe job. */
    b32 courageous;     /* For reaction to story events    */
    b32 show_danger;
    b32 update_stats;
    b32 divine_shield;
    b32 isDualWielding;
} Unit;
extern struct Unit Unit_default;

struct GraphStat {
    i16 level;
    i16 base_level;
    i8 cumul_stat[SOTA_MAX_LEVEL];
    i8 stat_id;
};
extern struct GraphStat GraphStat_default;

struct Graph {
    SDL_Rect rect; // x,y,w,h
    SDL_Texture *texture;

    struct GraphStat graph_stats[UNIT_STAT_MALLOC];

    Point plot_min; /* [XY units] */
    Point plot_max; /* [XY units] */

    i32 header; /* [pixels] */
    i32 footer; /* [pixels] */
    i32 margin_left;  /* [pixels] */
    i32 margin_right; /* [pixels] */

    /* length until writing another pixel, including pixel */
    /* ant for x */
    u8 y_lenperpixel;

    u8 stat_num;
    u8 linestyle;
    b32 x_ticks : 1;
    b32 y_ticks : 1;
};
extern struct Graph Graph_default;


/* --- Bars --- */
struct CircleBar {
    int fill;
    SDL_RendererFlip flip;
    Point pos;
};
extern struct CircleBar CircleBar_default;


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
extern struct SimpleBar SimpleBar_default;

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
    SDL_Scancode    y[SOTA_MAPPABLE_BUTTONS_NUM];
    SDL_Scancode    x[SOTA_MAPPABLE_BUTTONS_NUM];
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
extern struct KeyboardInputMap KeyboardInputMap_default;


/* --- Party --- */
/* Just for JSON loading */
struct Party {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

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
extern struct Party Party_default;


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

    struct dtab *items_dtab;
    struct dtab *weapons_dtab;
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
extern struct Game Game_default;

typedef struct Item {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    u8 target;  /* units by which item is usable. */
    struct Range range;
    struct Aura aura; /* Aura with range [0, 0] to for wielder-only bonus */

    struct Item_stats stats;
    u16  id;           /* 0 is NULL */
    u64  type;         /* and not type_exp */
    u16 *users;        /* item only usable by users.   NULL -> everyone */
    u16 *classes;      /* item only usable by classes. NULL -> everyone */

    /* -- Item Effects -- */
    u64 passive;

    /* Use function is used for Staves effects too. */
    use_function_t active; /* NULL if not usable */

    // TODO: Change to s8
    s8 name;
    char description[DEFAULT_BUFFER_SIZE * 2];

    b32 canSell     : 1;
    b32 write_stats : 1;
    b32 canUse      : 1;
    b32 canRepair   : 1; /* TODO: Move to weapon? */
} Item;
extern struct Item Item_default;

typedef struct Weapon {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    u8 handedness;
    u8 subtype;        /* ex: thrust swords     */
    b32 isMagic   : 1;
    b32 canAttack : 1; /* for special weapons   */
    u16 effective;
    Item         *item;
    struct Weapon_stats  stats;
} Weapon;
extern struct Weapon Weapon_default;

typedef struct Arrow {
    i32 *costmap;
    i32 *movemap;
    i32 move;
    i32 col_len;
    i32 row_len;
    i32 *pathlist;  /* from initial unit position to current cursor position */
    i32 tilesize[TWO_D];
    i32 map_tilesize[TWO_D];
    struct Point start;
    struct SDL_Texture *textures;  /* [patch_id] */
    struct Rendered rendereds[SOTA_MAX_MOVEMENT]; /* [patch_id] */
    b32 show;
} Arrow;
extern struct Arrow Arrow_default;


#endif /* STRUCTS_H */
