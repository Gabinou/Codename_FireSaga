#ifndef ENUMS_H
#define ENUMS_H

#include <stdbool.h>
#include <stdint.h>
#include "types.h"
#include "SDL2/SDL.h"

#define SOTA_VER_MAJOR         0
#define SOTA_VER_MINOR         7
#define SOTA_VER_PATCH         7
#define SOTA_VER_STRING        "0.7.7"
#define SOTA_EXECUTABLE_STRING "CodenameFireSage_premake"

enum bOFFSET {
    // bOFFSET: offset in bytes
    // HOWTO: for a WHAT in a struct
    // char * byte_ptr = (char *)struct_ptr
    // WHAT * what = (WHAT *)(byte_ptr + bOFFSET)
    // In words:
    // Cast struct pointer to bytes, bOFFSET it, cast to WHAT is at bOFFSET
    MENU_POS_bOFFSET  = 0, // for struct nmath_point_int32_t in menu structs
    JSON_ELEM_bOFFSET = 0, // for uif8 in json read/writable structs
};

enum SOTA_RANGEMAP {
    RANGEMAP_NULL      = 0,
    RANGEMAP_HEALMAP   = 1, /*includes staves and items (horns, salve, etc.) */
    RANGEMAP_ATTACKMAP = 2,
    RANGEMAP_NUM       = 3,
};

enum SOTA_RANGE {
    RANGE_NULL          = -1,
    RANGE_MIN           =  0,
    RANGE_MAX           =  1,
};

enum PIXEL_ORDER_ARGB8888 {
    LITTLE_ENDIAN_ARGB8888_R = 0,
    LITTLE_ENDIAN_ARGB8888_G = 1,
    LITTLE_ENDIAN_ARGB8888_B = 2,
    LITTLE_ENDIAN_ARGB8888_A = 3
};

#define TWO_HANDING_MIGHT_FACTOR          1.5f
#define TWO_HANDING_WEIGHT_FACTOR         2.00f
#define DUAL_WIELD_NOSKILL_MALUS_FACTOR   2
#define WRONGHAND_MALUS_FACTOR            4
#define RESCUE_MALUS_FACTOR               2
#define ROCK_SHIELD_BOULDER_PROB         90

enum SOTA_TEXT_SCROLL_SPEED {
    SOTA_TEXT_SCROLL_INSTANT = 0,
    SOTA_TEXT_SCROLL_FAST    = 1,
    SOTA_TEXT_SCROLL_MID     = 2,
    SOTA_TEXT_SCROLL_SLOW    = 3,
};

enum SOTA_TEXT {
    SOTA_TEXT_RIGHT_ALIGNED  = 0,
    SOTA_TEXT_LEFT_ALIGNED   = 1,
    SOTA_TEXT_CENTERING      = 2,
    SOTA_TEXT_JUSTIFIED      = 3,
    SOTA_TEXT_WORDLEN_BREAK  = 4,
};

enum AGONY {
    AGONY_FACTOR            = 20,
};

enum SOTA_HIT {
    HIT_DEX_FACTOR          =  2,
    HIT_LUCK_FACTOR         =  2,
};

enum SOTA_CRIT {
    CRIT_DEX_FACTOR         =   3,
    CRIT_LUCK_FACTOR        =   4,
    CRIT_FACTOR             = 150,
    CRIT_FACTOR_PROMOTED    = 200,
    CRIT_FACTOR_ASSASSIN    = 250,

};

enum SOTA_DODGE {
    DODGE_CON_FACTOR        =  2,
    DODGE_AGI_FACTOR        =  1,
    DODGE_LUCK_FACTOR       =  2,
    DODGE_STR_FACTOR        =  4,
    DODGE_FTH_FACTOR        =  2,
    DODGE_WPN_WGT_FACTOR    =  1,
};

enum SOTA_SPEED {
    SPEED_STR_FACTOR        =  4,
    SPEED_CON_FACTOR        =  2,
};

enum SOTA_FAVOR {
    FAVOR_FTH_FACTOR        =  2,
};

enum SOTA_OVERHEAL {
    OVERHEAL_FACTOR         =  4,
};

enum SOTA_REGRETS {
    REGRET_FAITH_FACTOR     =  4,
    REGRET_KILL             =  2,
    REGRET_AGONY            =  1,
    REGRET_LOOT             =  1,
    REGRET_MAX              =  1,
};

enum SOTA {
    SOTA_NULL                               =    0,
    SOTA_EXP_PER_LEVEL                      =  100,
};

enum SOTA_SETTINGS {
    DEFAULT_RESOLUTION_X                    = 1600,
    DEFAULT_RESOLUTION_Y                    =  800,
    DEFAULT_CAMERA_XOFFSET                  =  100,
    DEFAULT_CAMERA_YOFFSET                  =  100,
    CAMERA_SCROLL_SPEED                     =   10,
};

enum SOTA_SDL {
    SDL_INPUT_IGNORE                        =    0,
};

enum SOTA_LIMITS {
    SOTA_MAX_DAMAGE                         = 0xFF,
    SOTA_MAX_HP                             =   80,
    SOTA_MAX_SUPPORTS                       =    7,
    SOTA_MAX_MOVEMENT                       =   16,
    SOTA_MAX_PARTY_SIZE                     =   40,
    SOTA_MAX_TRUE_DAMAGE                    =    4,
    SOTA_MIN_STAT                           =    0, /* absolute unit stat minimum */
    SOTA_MAX_STAT                           =   50, /* absolute unit stat maximum */
    SOTA_MAX_RANGE                          =   12,
    SOTA_MAX_LEVEL                          =   40,
};

enum SOTA_CONSTANTS {
    SOTA_100PERCENT                         =  100,
    INTERVAL_BOUNDS_NUM                     =    2,
    SOTA_COL_LEN                            =    8,
    SOTA_WPN_ID_FACTOR                      =  100, /* id / SOTA_WPN_ID_FACTOR = type_exp */
};

enum SOTA_TILE {
    TILE_DIVISOR                            = 1000,
    TILE_ID_MAX                             =   99,
    SOTA_TILESIZE                           =   16,
    TILESET_COL_LEN                         =    8, /* transposed from Export Tilesets. */
};

enum SOTA_BOOKS {
    SOTA_BOOKS_NUM                          =   10,
};

enum SOTA_DEFAULTS {
    DEFAULT_EQUIPMENT_SIZE                   =    6,
    DEFAULT_BUFFER_SIZE                      =  128,
};

enum SOTA_EFFECTIVE {
    EFFECTIVE_FACTOR                         =  300,
    NOTEFFECTIVE_FACTOR                      =  100,
};

enum SOTA_DIMENSIONS {
    ONE_D           = 1,
    TWO_D           = 2,
    THREE_D         = 3,
    DIMENSION_X     = 0,
    DIMENSION_Y     = 1,
    DIMENSION_Z     = 2,
};

#define PALETTE_DEFAULT palette_NES
enum PALETTES {
    PALETTE_NES_COLOR_NUM               = 56,
    PALETTE_BLACK                       =  0,
    PALETTE_NULL                        =  0,
    PALETTE_START                       =  0,
    PALETTE_NES                         =  1,
    PALETTE_NES_FILTER_RED              =  2,
    PALETTE_NES_RED                     =  2,
    PALETTE_NES_FILTER_GREEN            =  3,
    PALETTE_NES_GREEN                   =  3,
    PALETTE_NES_FILTER_BLUE             =  4,
    PALETTE_NES_BLUE                    =  4,
    PALETTE_NES_FILTER_DARKRED          =  5,
    PALETTE_NES_DARKRED                 =  5,
    PALETTE_NES_FILTER_SHADOW           =  6,
    PALETTE_NES_SHADOW                  =  6,
    PALETTE_NES_FILTER_NIGHT            =  7,
    PALETTE_NES_NIGHT                   =  7,
    PALETTE_NES_DARKEN                  =  8,
    PALETTE_NES_LIGHTEN                 =  9,
    PALETTE_NES_LIGHTENMORE             = 10,
    PALETTE_NES_DARKREDwSHADOW          = 11,
    PALETTE_NES_FILTER_DARKREDwSHADOW   = 11,
    PALETTE_NES_FILTER_PURPLE           = 12,
    PALETTE_NES_PURPLE                  = 12,
    PALETTE_NES_ENEMY                   = 13,
    PALETTE_NUM                         = 14,
    PALETTE_NES_COLOR_GROUPSIZE         =  4,
    /* Color RGB 000001 is transparent */
    PALETTE_COLORKEY                    =  0, /* indexed image, transparent color index*/
};
#define SOTA_COLORKEY 0x00, 0x00, 0x01

enum FPS_DEFAULTS {
    FPS_DEFAULT_POSX    =  850,
    FPS_DEFAULT_POSY    =   10,
    FPS_DEFAULT_SHOW    =    1,
    FPS_DEFAULT_CAP     =   60,
    FF_DEFAULT_CAP      = 2000,
    FPS_DEFAULT_COLORR  =    0,
    FPS_DEFAULT_COLORG  =    0,
    FPS_DEFAULT_COLORB  =    0,
    FPS_DEFAULT_COLORA  =  255,
    FPS_SCALE           =    5,
};

enum NES_COLORS {
    NES_COLORKEY        = 0,
    NES_TRANSPARENT     = 0,
    NES_BLACK           = 1,
    NES_WHITE           = PALETTE_NES_COLOR_NUM - 1,
};

enum FRAME_PAUSE_MODE {
    FRAME_PAUSE_FLAT    = 0,
    FRAME_PAUSE_FACTOR  = 1,
    FRAME_PAUSE_SPEED   = 2,
};

enum SOTA_CONVERSIONS {
    SOTA_ns                 = 1000000000, /* [ns/s] */
    SOTA_us                 =    1000000, /* [us/s] */
    SOTA_ms                 =       1000, /* [ms/s] */
};

enum SOTA_POPUP_WAIT {
    POPUP_SLIDEWAIT_ns      = 20000000,
    POPUP_TILE_SLIDEWAIT_ns = 10000000,
    POPUP_UNIT_SLIDEWAIT_ns = 10000000,
};

enum SOTA_CONTROLLER_WAIT {
    KEYBOARD_MINHELD_ns     = 0,
    GAMEPAD_MINHELD_ns      = 0,
};

enum SOTA_CURSOR {
    CURSOR_FIRSTMOVE_PAUSE_ms =       250,
    CURSOR_SLIDEWAIT_ns       =   1000000,
    CURSOR_FIRSTMENU_PAUSE_ns = 100000000,
    DEFAULT_CURSOR_FRAMES     =        10,
    DEFAULT_CURSOR_SPEED      =        30,
    DEFAULT_CURSOR_SRC_W      =        64,
    DEFAULT_CURSOR_SRC_H      =        64,
    DEFAULT_CURSOR_FACTOR     =         2, /* Cursor/Tile size factor */
};

enum SOTA_FPS {
    SOTA_FPS_UPDATE_ns        = 1000000000, /* 1 second */
};

#define FPS_DEFAULT_SIZEFACTORX  1.0f
#define FPS_DEFAULT_SIZEFACTORY  1.0f
#define DEFAULT_CAMERA_ZOOM      4.0f
#define CAMERA_INCREMENT         1.0f
#define MAX_CAMERA_ZOOM          9.0f
#define MIN_CAMERA_ZOOM          2.0f
#define CAMERA_BOUNDS            0.95f
#define CAMERA_BOUNDS_SCALE      0.1f
#define CAMERA_SCROLL_ZOOMFACTOR 2.75f

enum SOTA_MOUNT {
    MOVE_WITH_MOUNT  = 7,
};

#define REGISTER_ENUM(x) MOUNT_TYPE_##x,
enum MOUNT_TYPES {
    MOUNT_TYPE_NULL  = 0,
    MOUNT_TYPE_START = 0,
#include "names/mounts_types.h"
    MOUNT_TYPE_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) MOUNT_NAME_##x,
enum MOUNT_NAMES {
    MOUNT_NAME_START = 0,
#include "names/mounts.h"
    MOUNT_NAME_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z) SHOP_CAMP_CHAPTER_##x,
enum SHOPS {
    SHOP_START = 0,
#include "names/chapters.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x) SHOP_##x,
#include "names/shops.h"
    SHOP_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z) MENU_OPTION_##x,
enum OPTIONS {
    MENU_OPTION_START = 0,
#include "names/menu/options.h"
    MENU_OPTION_END,
    MENU_OPTION_NUM = MENU_OPTION_END,
};
#undef REGISTER_ENUM

/* ITEM_ID is the explicit, human-friendly item index
* EXAMPLE:
*   global_itemNames[ITEM_ORDER...] -> correct item name
*/

#define REGISTER_ENUM(x, y) ITEM_ID_##x = y,
enum ITEM_IDs {
    ITEM_NULL     = 0,
    ITEM_ID_START = 0,
#include "names/items.h"
#undef REGISTER_ENUM
};

// ITEM_ORDER is the implicit, compiler-friendly item position in array
// global_itemNames[ITEM_ORDER...] -> correct item name
#define REGISTER_ENUM(x, y) ITEM_ORDER_##x,
enum ITEM_ORDER {
    ITEM_ORDER_START = 0,
#include "names/items.h"
#undef REGISTER_ENUM

    // name should be all caps
#define ITEM_ORDER(name) ITEM_ORDER_##name

#define REGISTER_ENUM(x) ITEM_ID_TALISMAN_##x,
#include "names/units_stats.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_ID_SCRIPTURE_##x,
#include "names/skills_passive.h"
#include "names/skills_active.h"
    ITEM_ID_BOOKEND,
};
#undef REGISTER_ENUM

/* --- ITEM HIERARCHY --- */
/* Example: Fleuret                 */
/*      Archetype -> Weapon         */
/*           Type -> Sword          */
/*        SubType -> Thrust Sword   */
enum ITEM_ARCHETYPE {
    ITEM_ARCHETYPE_NULL     = -1,
    ITEM_ARCHETYPE_ITEM     =  0,
    ITEM_ARCHETYPE_STAFF    =  1, /* Staves use Weapon struct  -> weapons_dtab */
    ITEM_ARCHETYPE_WEAPON   =  2,
    ITEM_ARCHETYPE_SHIELD   =  3, /* Shields use Weapon struct -> weapons_dtab */
    ITEM_ARCHETYPE_NUM,
};
extern int dtabs_id[ITEM_ARCHETYPE_NUM];

/* Note: Only ITEM_ARCHETYPE_ITEM use Item struct -> items_dtab */
enum DTAB {
    SOTA_DTAB_NULL       = -1,
    SOTA_DTAB_WEAPONS    =  0,
    SOTA_DTAB_ITEMS      =  1,
};

#define REGISTER_ENUM(x) ITEM_TYPE_EXP_##x,
enum ITEM_TYPE_EXP {
    ITEM_TYPE_EXP_NULL = 0,
#include "names/items_types.h"
    ITEM_TYPE_EXP_END,
    ITEM_TYPE_NUM = ITEM_TYPE_EXP_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_TYPE_##x = 1UL << (ITEM_TYPE_EXP_##x - 1),
enum ITEM_TYPES {
    ITEM_TYPE_NULL  = 0,
    ITEM_TYPE_START = 0,
#include "names/items_types.h"
    ITEM_TYPE_END = 1UL << ITEM_TYPE_EXP_END,
    ITEM_TYPE_canATTACK = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_AXE + ITEM_TYPE_ELEMENTAL + ITEM_TYPE_DEMONIC + ITEM_TYPE_ANGELIC + ITEM_TYPE_BOW + ITEM_TYPE_OFFHAND,
};
#undef REGISTER_ENUM

enum TEXT_COLOR {
    SOTA_MALUS_RED   = 0xB2,
    SOTA_MALUS_GREEN = 0x10,
    SOTA_MALUS_BLUE  = 0x30,

    SOTA_BONUS_RED   = 0x71,
    SOTA_BONUS_GREEN = 0xF3,
    SOTA_BONUS_BLUE  = 0x41,
};

enum OVERLAY_COLOR {
    OVERLAY_ATTACK_COLOR_NES =         32,
    OVERLAY_ATTACK_COLOR_RED =       0xB2,
    OVERLAY_ATTACK_COLOR_GREEN =     0x10,
    OVERLAY_ATTACK_COLOR_BLUE =      0x30,

    OVERLAY_DANGER_COLOR_NES =         32,
    OVERLAY_GLOBALRANGE_COLOR_NES =    30,

    OVERLAY_MOVE_COLOR_NES =           45,
    OVERLAY_MOVE_COLOR_RED =         0x41,
    OVERLAY_MOVE_COLOR_GREEN =       0x61,
    OVERLAY_MOVE_COLOR_BLUE =        0xFB,

    OVERLAY_HEAL_COLOR_NES =           13,
    OVERLAY_HEAL_COLOR_RED =         0x49,
    OVERLAY_HEAL_COLOR_GREEN =       0xAA,
    OVERLAY_HEAL_COLOR_BLUE =        0x10,
};

enum HPBAR {
    HPBAR_HEIGHT =                    5,
    HPBAR_LEN =                      35,
    HPBAR_PATTERN_MULTIPLIER =        4,
    HPBAR_PATTERN_START =             6,
    HPBAR_HPPERROW =                 30, /* 92 pixels, about 6 tiles */
    HPBAR_TILESIZE_X =                4, /* [pixels] */
    HPBAR_TILESIZE_Y =               10, /* [pixels] */
    HPBAR_TEXTURE_NONE =              0,
    HPBAR_TEXTURE_FILLED =            1,
    HPBAR_TEXTURE_EMPTY =             2,
    HPBAR_TEXTURE_PDAMAGE =           3,
    HPBAR_TEXTURE_MDAMAGE =           4,
    HPBAR_TEXTURE_TDAMAGE =           5,
    HPBAR_TEXTURE_OVERHEAL =          6, /* SAME AS SHIELD */
};
#define HPBAR_UPDATEWAIT 0.05f /* [s] */

enum ITEM_DIVISORS {
    ITEM_DIVISOR =                 100,
    ITEM_DIVISOR_SWORD =             1,
    ITEM_DIVISOR_LANCE =           100,
    ITEM_DIVISOR_AXE =             200,
    ITEM_DIVISOR_BOW =             300,
    ITEM_DIVISOR_SHIELD =          400,
    ITEM_DIVISOR_OFFHAND =         500,
    ITEM_DIVISOR_MAGIC =           600,
    ITEM_DIVISOR_ELEMENTAL =       600,
    ITEM_DIVISOR_DEMONIC =         700,
    ITEM_DIVISOR_ANGELIC =         800,
    ITEM_DIVISOR_STAFF =           900,
};

#define REGISTER_ENUM(x) ITEM_STAT_##x,
enum ITEM_STATS {
    ITEM_STAT_START = 0,
#include "names/items_stats.h"
    ITEM_STAT_END,
#undef REGISTER_ENUM
#define REGISTER_ENUM(x) WEAPON_STAT_##x,
    WEAPON_STAT_START = ITEM_STAT_END,
#include "names/weapon_stats.h"
    WEAPON_STAT_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const uint64_t UNIT_PC_ALIVE_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const uint64_t UNIT_PC_RECRUITED_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const uint64_t UNIT_NPC_ALIVE_##x;
#include "names/units_NPC.h"
#undef REGISTER_ENUM

// NOTE: Item effects
// Killy_Cibo_Pair: Give Cibo prot +2/+2 and Killy atk +4/0/0
#define REGISTER_ENUM(x, y) ITEM_EFFECT_EXP_##x = y,
enum ITEM_EFFECT_EXP {
    ITEM_EFFECT_EXP_NULL = 0,
#include "names/items_effects.h"
    ITEM_EFFECT_EXP_END,
    ITEM_EFFECT_END = ITEM_EFFECT_EXP_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const uint64_t ITEM_EFFECT_##x;
#include "names/items_effects.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) CAMPJOB_##x,
enum CAMPJOBS {
    CAMPJOB_START = 0,
#include "names/camp_jobs.h"
#undef REGISTER_ENUM
    CAMPJOB_END,
#define REGISTER_ENUM(x) CAMPJOB_INDEX_##x = 1 << (CAMPJOB_##x - 1),
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(name, id, x, y, z) UNIT_CLASS_##name = id,
enum UNIT_CLASSES {
    UNIT_CLASS_START = 0,
#include "names/classes.h"
    UNIT_CLASS_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_TYPE_EXP_##x,
enum UNIT_TYPE_EXP {
    UNIT_TYPE_EXP_START = 0,
#include "names/units_types.h"
    UNIT_TYPE_EXP_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_TYPE_##x = 1UL << UNIT_TYPE_EXP_##x,
enum UNIT_TYPES {
    UNIT_TYPE_START = 0,
#include "names/units_types.h"
    UNIT_TYPE_NUM = UNIT_TYPE_EXP_END,
    UNIT_TYPE_END = UNIT_TYPE_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ARMY_##x,
enum UNIT_ARMIES {
    ARMY_START = 0,
#include "names/armies.h"
    ARMY_END,
};
#undef REGISTER_ENUM

/* UNIT_ID is the explicit, human-friendly unit index
* EXAMPLE:
*   global_unitNames[UNIT_ORDER...] -> correct item name
*/

#define REGISTER_ENUM(x, y) UNIT_ID_##x = y,
enum UNIT_IDS {
    UNIT_NULL = 0,
    UNIT_ID_START = 0,
#include "names/units_PC.h"
#include "names/units_NPC.h"
#undef REGISTER_ENUM
    UNIT_ID_END,
};
#undef REGISTER_ENUM

// UNIT_ORDER is the implicit, compiler-friendly item position in array
// global_unitNames[ITEM_ORDER...] -> correct item name
// name should be all caps
#define UNIT_ORDER(name) UNIT_ORDER_##name
#define REGISTER_ENUM(x, y) UNIT_ORDER##x,
enum UNIT_ORDER {
    UNIT_ORDER_START = 0,
#include "names/units_PC.h"
#include "names/units_NPC.h"
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_SKILL_EXP_##x,
enum UNIT_SKILL_EXP {
    UNIT_SKILL_EXP_START =  0,
#include "names/skills_passive.h"
#include "names/skills_active.h"
    UNIT_SKILL_EXP_END,
    UNIT_SKILL_MAX_NUM =    4,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) PASSIVE_SKILL_##x,
enum PASSIVE_SKILLS {
    PASSIVE_SKILL_START = 0,
#include "names/skills_passive.h"
    PASSIVE_SKILL_END,
    PASSIVE_SKILL_NUM = PASSIVE_SKILL_END,
};
#undef REGISTER_ENUM

enum INFUSE {
    SOTA_INFUSEABLE     = -1,
    SOTA_NON_INFUSEABLE =  0, /* for magic weapons */
    SOTA_INFUSE_FACTOR  =  3,
};

#define REGISTER_ENUM(x) ACTIVE_SKILL_##x,
enum ACTIVE_SKILLS {
    ACTIVE_SKILL_START = 0,
#include "names/skills_active.h"
    ACTIVE_SKILL_END,
    ACTIVE_SKILL_NUM = ACTIVE_SKILL_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t UNIT_SKILL_##x;
#include "names/skills_passive.h"
#include "names/skills_active.h"
enum UNIT_SKILL {
    UNIT_SKILL_START =  0,
    UNIT_SKILL_NUM   = 49,
    UNIT_SKILL_END   = UNIT_SKILL_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_STAT_##x,
enum UNIT_STATS {
    UNIT_STAT_START = -1,
#include "names/units_stats.h"
    UNIT_STAT_NUM
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_STATUS_EXP_##x,
enum UNIT_STATUS_EXP {
    UNIT_STATUS_NULL      = 0,
    UNIT_STATUS_EXP_START = 0,
#include "names/units_statuses.h"
    UNIT_STATUS_EXP_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_STATUS_##x = 1UL << UNIT_STATUS_EXP_##x,
enum UNIT_STATUS {
    UNIT_STATUS_START = 0,
#include "names/units_statuses.h"
    UNIT_STATUS_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) TILE_##x,
enum TILES {
    // Basic tile index is 4 digits -> 1000
    // two first digits give the tile.
    // third digit gives the asset index.
    // Ex: 324: Peak tile, 4th asset of peak.
    TILENAME_MAX_LEN = 8,
    TILE_START = 0,
#include "names/tiles.h"
    TILE_END,
};
#undef REGISTER_ENUM

enum TILE_ICON_INDICES {
    TILE_ICON_NULL      = 0,
    TILE_ICON_ATTACKTO  = 1,
    TILE_ICON_MOVETO    = 2,
    TILE_ICON_DANGER    = 3,
};

enum GAME_STATES {
    GAME_STATE_START = 0,
#define REGISTER_ENUM(x) GAME_STATE_##x,
#include "names/game_states.h"
    GAME_STATE_END,
    GAME_STATE_NUM = GAME_STATE_END,
#undef REGISTER_ENUM
};

#define REGISTER_ENUM(x) GAME_SUBSTATE_##x,
enum GAME_substate {
    GAME_SUBSTATE_START = 0,
#include "names/game_substates.h"
    GAME_SUBSTATE_END,
    GAME_SUBSTATE_NUM = GAME_SUBSTATE_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) JSON_##x,
enum JSON_ELEMENT {
    JSON_NULL  = 0,
    JSON_START = 0,
#include "names/json_elements.h"
    JSON_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_MVT_##x,
enum UNIT_MVT {
    UNIT_MVT_START = 0,
#include "names/mvt_types.h"
    UNIT_MVT_END,
};
#undef REGISTER_ENUM

/* --- Inputs vs buttons --- */
/* Button: Physical button on a controller e.g. A, B, X, Y
*  Input:  Context dependent interpretation of user button press
*       e,g. Menuright, Accept, Open Minimap, Pause...
*/
#define REGISTER_ENUM(x) SOTA_INPUT_##x,
enum SOTA_INPUTS {
    SOTA_INPUT_DPAD_END         =  4,
    SOTA_INPUT_NULL             = -1,
    SOTA_INPUT_START            = -1,
#include "names/input.h"
    SOTA_INPUT_END,
};
#undef REGISTER_ENUM

enum SOTA_BUTTONS {
    SOTA_MAPPABLE_BUTTONS_NUM   =  2,
    SOTA_BUTTON_DPAD_END        =  4,
    SOTA_BUTTON_NULL            = -1,
    SOTA_BUTTON_DPAD_RIGHT      =  0,
    SOTA_BUTTON_DPAD_UP,
    SOTA_BUTTON_DPAD_DOWN,
    SOTA_BUTTON_DPAD_LEFT,
    SOTA_BUTTON_A,
    SOTA_BUTTON_B,
    SOTA_BUTTON_Y,
    SOTA_BUTTON_X,
    SOTA_BUTTON_START,
    SOTA_BUTTON_SHOULDER_LEFT,
    SOTA_BUTTON_SHOULDER_RIGHT,
    SOTA_BUTTON_TRIGGER_LEFT,
    SOTA_BUTTON_TRIGGER_RIGHT,
    SOTA_BUTTON_END,
};
extern char sota_buttons[SOTA_BUTTON_END];

enum SKILL_VALUES {
    SOTA_SKILL_PINPRICK         = 1,
    SOTA_SKILL_PIERCE           = 2,
    SOTA_SKILL_CRUSH            = 4,
};

/* --- DAMAGE TYPES --- */
enum DAMAGE_TYPES {
    DMG_TYPE_PHYSICAL           = 0,
    DAMAGE_TYPE_PHYSICAL        = 0,
    DMG_TYPE_MAGICAL            = 1,
    DAMAGE_TYPE_MAGICAL         = 1,
    DMG_TYPE_TRUE               = 2,
    DAMAGE_TYPE_TRUE            = 2,
    DMG_TYPE_TOTAL              = 3,
    DAMAGE_TYPE_TOTAL           = 3,
    PROTECTION_TYPES            = 3,
    PROTECTION_TYPES_NO_TOTAL   = 2,
    ATTACK_TYPES                = 4,
    ATTACK_TYPES_NO_TOTAL       = 3,
    DAMAGE_TYPES                = 4,
};

enum MOUSES {
    MOUSE_START = 0,
    MOUSE_ACCEPT,
    MOUSE_NOACCEPT,
    MOUSE_FOLLOW,
    MOUSE_TELEPORT,
    MOUSE_END,
};

enum SLIDETYPES {
    SLIDETYPE_NONE              = 0,
    SLIDETYPE_START             = 0,
    SLIDETYPE_GEOMETRIC,
    SLIDETYPE_LINEAR,
    SLIDETYPE_LINEARXY,
    SLIDETYPE_LINEARYX,
    SLIDETYPE_EASYINEASYOUT,
    SLIDETYPE_VECTOR,
    SLIDETYPE_END,
    SLIDETYPE_EASYINEASYOUT_FACTOR_X =   8, /* TODO: proportional to screen x resolution */
    SLIDETYPE_EASYINEASYOUT_FACTOR_Y =   4, /* TODO: proportional to screen y resolution */
};

#define REGISTER_ENUM(x) POPUP_TYPE_##x,
enum POPUP_TYPE {
    POPUP_TYPE_NULL  = 0,
    POPUP_TYPE_START = 0,
#include "names/popup/types.h"
    POPUP_TYPE_END,
    POPUP_TYPE_NUM = POPUP_TYPE_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) MENU_TYPE_##x,
enum MENU_TYPE {
    MENU_TYPE_START = 0,
#include "names/menu/types.h"
    MENU_TYPE_END,
    STATS_MENU_CYCLE_NUM = 2,
};
#undef REGISTER_ENUM

enum SOTA_POPUP {
    POPUP_NULL             = 0,
    POPUP_ITEM_DESCRIPTION = 0,
};

/* change to player_select menu */
#define REGISTER_ENUM(x) MENU_PLAYER_SELECT_##x,
enum MENU_PLAYER_SELECT {
    NO_MENU                  = 0,
    MENU_PLAYER_SELECT_START = 0,
#include "names/menu/player_select.h"
    MENU_PLAYER_SELECT_END,
    MENU_PLAYER_SELECT_NUM = MENU_PLAYER_SELECT_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ERROR_##x,
enum ERRORS {
    #ifndef NO_ERROR
    NO_ERROR = 0,
    #endif
    ERROR_START = 0,
#include "names/errors.h"
    ERROR_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) INPUTFLAG_##x,
enum INPUTFLAGS {
    NO_INPUTFLAG    = 0,
    INPUTFLAG_START = 1,
#include "names/input_flags.h"
    INPUTFLAG_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z) CHAPTER_##x,
enum CHAPTERS {
    NO_CHAPTER    = -1,
    CHAPTER_START = -1,
#include "names/chapters.h"
    CHAPTER_FOLDER_TEST,
    CHAPTER_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) SCENE_TIME_##x,
enum SCENE_TIME {
    SCENE_TIME_NULL = 0,
#include "names/scene_time.h"
    SCENE_TIME_NUM,
};
#undef REGISTER_ENUM

enum SELECTORS {
    SELECTOR_START = 0,
    SELECTOR_MOUSE,
    SELECTOR_CURSOR,
    SELECTOR_END,
};

// Should the controller type be the entity?
enum CONTROLLERS {
    CONTROLLER_START = 0,
    CONTROLLER_GAMEPAD,
    CONTROLLER_MOUSE,
    CONTROLLER_KEYBOARD,
    CONTROLLER_TOUCHPAD,
    CONTROLLER_END,
};

enum CONTROLLER_DIRECTIONS { /* ternary -> octant */
    /* Intuitive screen direction */
    /* Unbalanced ternary, [x,y] -> [3^0,3^1] */
    /* Area split into octants around 2D area */
    // NOTE: Order in enum is array index
    SOTA_DIRECTION_NULL         =  0,
    SOTA_DIRECTION_INSIDE       =  0,
    SOTA_DIRECTION_RIGHT        =  2,
    SOTA_DIRECTION_TOP          =  6, /* Intuitive -> top of screen */
    SOTA_DIRECTION_LEFT         =  1,
    SOTA_DIRECTION_BOTTOM       =  3,
    SOTA_DIRECTION_DIAGONAL_TR  =  8,
    SOTA_DIRECTION_DIAGONAL_TL  =  7,
    SOTA_DIRECTION_DIAGONAL_BR  =  5,
    SOTA_DIRECTION_DIAGONAL_BL  =  4,
    SOTA_DIRECTIONS_MAIN_NUM    =  4,
    SOTA_DIRECTIONS_NUM         =  8, /* Including diagonals */
};

/* Direction array index from ternary index */
extern int direction_arr_i[SOTA_DIRECTIONS_NUM + 1];

enum SOTA_COMBAT {
    /* glossary */
    SOTA_DEFENDANT                  = 0,
    SOTA_AGGRESSOR                  = 1,

    SOTA_BRAVE_DEFAULT              = 1,
    SOTA_BRAVE_MAX                  = 4,

    SOTA_ASTRA_DEFAULT              = 1,
    SOTA_ASTRA                      = 5,

    SOTA_DOUBLING_SPEED             = 4, /* Speed difference > to double */

    /* Combat limit for combatant */
    SOTA_UNIT_MAX_COMBAT_PHASES     = 3,

    /* Combat limits */
    SOTA_COMBAT_MAX_PHASES          = SOTA_UNIT_MAX_COMBAT_PHASES + 1,
    SOTA_COMBAT_PHASE_MAX_ATTACKS   = SOTA_ASTRA * SOTA_BRAVE_MAX,
    SOTA_COMBAT_MAX_ATTACKS         = SOTA_COMBAT_PHASE_MAX_ATTACKS * SOTA_COMBAT_MAX_PHASES,
};

/* GLOSSARY */
enum TALKING {
    /* I talk to him, so the other unit is the auditor. */
    SPEAKER = 0,
    AUDITOR = 1,
};

enum TRADING {
    TRADER_NULL     = -1,
    TRADER_PASSIVE  =  0,
    TRADER_ACTIVE   =  1,
};

enum RESCUE {
    RESCUE_NULL     = -1,
    RESCUE_VICTIM   =  0,
    RESCUE_SAVIOR   =  1,
};

enum STAFF {
    STAFF_NULL      = -1,
    STAFF_HEALER    =  0,
    STAFF_PATIENT   =  1,
};

enum DANCING {
    DANCING_NULL        = -1,
    DANCING_DANCER      =  0,
    DANCING_SPECTATOR   =  1, /* or Pervert? lol*/
};

enum SHOPPING {
    SHOPPING_NULL =      -1,
    SHOPPING_BUYER =      0,
    SHOPPING_SELLER =     1,
};

enum MAP_DANGERMAP {
    DANGERMAP_UNIT_DIVISOR           = 1000,
};

enum MAP_SETTINGS_OVERLAY_MODE {
    MAP_OVERLAY_MODE_NULL        = 0,
    MAP_OVERLAY_MODE_PALETTESWAP = 1 << 0,
    MAP_OVERLAY_MODE_OPAQUE      = 1 << 1,
    MAP_OVERLAY_MODE_FLASH       = 1 << 2,
};

enum MAP_SETTINGS_STACK {
    MAP_SETTING_DANGERMAP_NULL  = 0,
    MAP_SETTING_DANGERMAP_HIDE  = 1,
    MAP_SETTING_UNITMAP_HIDE    = 2,
    MAP_SETTING_STACK_DANGERMAP = 3, /* top Dangermap opaque or flash */
    MAP_SETTING_STACK_MOVEMAP   = 4, /* opaque or flash */
};

enum MAP_CONDITIONS {
    MAP_CONDITION_NONE        = 0,
    MAP_CONDITION_SEIZE       = 1,
    MAP_CONDITION_STANDTHERE,
    MAP_CONDITION_ROUT,
};

enum OVERLAYS {
    MAP_OVERLAY_NULL            = 0,
    MAP_OVERLAY_HEAL            = 1 << 0,
    MAP_OVERLAY_MOVE            = 1 << 1,
    MAP_OVERLAY_ATTACK          = 1 << 2,
    MAP_OVERLAY_DANGER          = 1 << 3,
    MAP_OVERLAY_GLOBAL_DANGER   = 1 << 4,
};

enum LOOPINGS {
    LOOPING_INVALID          = 0,
    LOOPING_PINGPONG         = 1,
    LOOPING_FORWARD,
    LOOPING_REVERSE,
    LOOPING_NUM,
};

enum POINTS {
    POINTS_MATRIX            = 0,
    POINTS_LIST              = 1,
};

enum WEAPON_SUBTYPE {
    WEAPON_SUBTYPE_NONE      = 0,
    WEAPON_SUBTYPE_THRUST    = 1,
};

enum UNIT_HANDS {
    UNIT_HAND_LEFT           = 0,
    UNIT_HAND_RIGHT          = 1,
    UNIT_HAND_STRONG         = 0,
    UNIT_HAND_WEAK           = 1,
    UNIT_HANDS_NUM           = 2,
};

enum WEAPON_HANDEDNESS {
    WEAPON_HAND_NULL         = 0,
    WEAPON_HAND_ANY          = 0, // one OR two hands
    WEAPON_HAND_ONE          = 1, // only one hand
    WEAPON_HAND_TWO          = 2, // only two hands
    WEAPON_HAND_LEFT         = 3, // only left hand
    WEAPON_HAND_RIGHT        = 4, // only right hand
};

enum UNIT_HANDEDNESS {
    UNIT_HAND_NULL           = 0b00,
    UNIT_HAND_LEFTIE         = 0b01, // 1
    UNIT_HAND_RIGHTIE        = 0b10, // 2
    UNIT_HAND_AMBIDEXTROUS   = 0b11, // 3
    UNIT_HAND_END
};

enum RNGs {
    GAME_RN_SINGLE = 1,
    GAME_RN_DOUBLE,
    GAME_RN_HYBRID,
    GAME_RN_GAUSSIAN,
};

enum VISION_BLOCKMAP_CODES {
    VISION_BLOCKMAP_NOT_BLOCKED = 0,
    VISION_BLOCKMAP_BLOCKED     = 1,
};

enum STARTUP_MODES {
    NO_STARTUP              = 0,
    STARTUP_START           = 0,
    STARTUP_TITLESCREEN,
    STARTUP_TESTING,
    STARTUP_SOUND_ROOM,
    STARTUP_SCENE_ROOM,
    STARTUP_SUPPORT_ROOM,
    STARTUP_BATTLECONVOS_ROOM,
    STARTUP_OTHERCONVOS_ROOM,
    STARTUP_END,
};

enum SIGHTMAP_CODES {
    SIGHTMAP_NOT_VISIBLE = 0,
    SIGHTMAP_VISIBLE     = 1,
    SIGHTMAP_OBSERVER    = 2,
    SIGHTMAP_WALL        = 3,
    SIGHTMAP_ENEMY       = 4,
    SIGHTMAP_FRIENDLY    = 5,
};

/* Always from the point of view of the player/Erwin */
enum ALIGNMENT {
    NO_ALIGNMENT         = 0,
    ALIGNMENT_NULL       = 0,
    ALIGNMENT_START      = 0,
    ALIGNMENT_FRIENDLY   = 1,
    ALIGNMENT_BLUE_UNIT  = 1,
    ALIGNMENT_ENEMY      = 2,
    ALIGNMENT_RED_UNIT   = 2,
    ALIGNMENT_NUM,
};

enum MOUSE_DEFAULTS { // this extern struct is dumb
    MOUSE_DEFAULT_ONHOLD    = MOUSE_ACCEPT,
    MOUSE_DEFAULT_MOVE      = MOUSE_FOLLOW,
    MOUSE_DEFAULT_SRCRECT_H = 32,
    MOUSE_DEFAULT_SRCRECT_W = 32,
    MOUSE_DEFAULT_DSTRECT_H = 80,
    MOUSE_DEFAULT_DSTRECT_W = 80,
};

#endif /* ENUMS_H */
