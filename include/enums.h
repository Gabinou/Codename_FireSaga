#ifndef ENUMS_H
#define ENUMS_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "types.h"
#include "macros.h"
#include "tnecs.h"

#define LOGFILE          "log.txt"
#define ASSET_LIB        "assets.binou"
#define GAME_TITLE       "Sword of the Anointed"
#define SAVE_FOLDER      "saves"
#define SOTA_EXE_NAME    "CodenameFireSaga"
#define SOTA_BUILD_DIR   "build"
#define GAME_TITLE_ABREV "SotA"

#define SOTA_VER_MAJOR         0
#define SOTA_VER_MINOR         7
#define SOTA_VER_PATCH         7
#define SOTA_VER_STRING        "0.7.7"
#define SOTA_EXECUTABLE_STRING "CodenameFireSage_premake"

#define SOTA_PI (355.0f / 113.0f)

enum PIXELFONT {
    ASCII_CHARSET_ROW_LEN       = 16,
    ASCII_CHARSET_COL_LEN       = 16,
    ASCII_GLYPH_HEIGHT          =  8,
    ASCII_GLYPH_WIDTH           =  8,
    ASCII_CHARSET_NUM           = ASCII_CHARSET_ROW_LEN * ASCII_CHARSET_COL_LEN,
    PIXELFONT_WORD_SPACE        =  2,
    PIXELFONT_GLYPH_SPACE       =  0,
    PIXELFONT_HEIGHT            =  8,
    ASCII_GLYPH_NUM             = 0x0100
};

enum SOTA_SAVES {
    SOTA_SAVE_DEBUG_0       = 0,
    SOTA_SAVE_DEBUG_1       = 1,
    SOTA_SAVE_DEBUG_2       = 2,
    SOTA_SAVE_DEBUG_3       = 3,
    SOTA_SAVE_PLAYER_START  = 4,
};

enum ROWCOL {
    SOTA_COL_INDEX = 0,
    SOTA_ROW_INDEX = 1,
};

enum SOTA_AURA {
    AURA_REMOVE_ON_MOVE         = -1, /* Or any value below */
    AURA_REMOVE_ON_TURN_END     =  0, /* Any value above 0 gets decremented at turn end */
};

enum SOTA_RANGEMAP {
    RANGEMAP_NULL      = 0,
    RANGEMAP_HEALMAP   = 1, /*includes staves and items (horns, salve, etc.) */
    RANGEMAP_ATTACKMAP = 2,
    RANGEMAP_NUM       = 3,
};

enum SOTA_RANGE {
    RANGE_MIN_INDEX     =  0,
    RANGE_MAX_INDEX     =  1,
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
// WRONGHAND_MALUS_FACTOR:
//  - If two hand weapon is one handed
//  - One handed weapons CAN'T be two-handed
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
    AGONY_NULL              = 0,
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
    REGRET_FAITH_FACTOR     =    4,
    REGRET_KILL             =    2,
    REGRET_AGONY            =    1,
    REGRET_LOOT             =    1,
    REGRET_MAX              =  500,
    REGRET_MIN              =    0,
};

enum SOTA_MATH {
    SQUARE_NEIGHBOURS = 4
};

enum SOTA {
    SOTA_NULL                               =    0,
    SOTA_EXP_PER_LEVEL                      =  100,
    SOTA_ITERATIONS_LIMIT                   = 1000,
};

enum UNIT_SEX {
    UNIT_SEX_F   = 0,
    UNIT_SEX_M   = 1,
    UNIT_SEX_NUM = 2
};

enum SOTA_SETTINGS {
    DEFAULT_RESOLUTION_X                    = 1600,
    DEFAULT_RESOLUTION_Y                    =  800,
    DEFAULT_CAMERA_XOFFSET                  =  100,
    DEFAULT_CAMERA_YOFFSET                  =  100,
    CAMERA_SCROLL_SPEED                     =   10, /* [px/frame] */
};

enum SOTA_LIMITS {
    SOTA_MAX_TURNS      =  255,

    /* Unit_stats limits*/
    SOTA_MIN_HP         =    1,
    SOTA_MAX_HP         =   80,
    SOTA_MIN_STR        =    0,
    SOTA_MAX_STR        =   60,
    SOTA_MIN_MAG        =    0,
    SOTA_MAX_MAG        =   60,
    SOTA_MIN_AGI        =    0,
    SOTA_MAX_AGI        =   60,
    SOTA_MIN_DEX        =    0,
    SOTA_MAX_DEX        =   60,
    SOTA_MIN_FTH        =    0,
    SOTA_MAX_FTH        =   80,
    SOTA_MIN_LUCK       =    0,
    SOTA_MAX_LUCK       =   80,
    SOTA_MIN_DEF        =    0,
    SOTA_MAX_DEF        =   60,
    SOTA_MIN_RES        =    0,
    SOTA_MAX_RES        =   60,
    SOTA_MIN_CON        =    0,
    SOTA_MAX_CON        =   60,
    SOTA_MIN_MOVE       =    0,
    SOTA_MAX_MOVE       =   20,
    SOTA_MIN_PROF       =    0,
    SOTA_MAX_PROF       =   60,
    SOTA_MIN_WGT        =    0,
    SOTA_MAX_WGT        =   60,

    SOTA_MIN_HEAL_PERCENT  =    0,
    SOTA_MAX_HEAL_PERCENT  =  200,
    /* Computed_Stats limits*/
    SOTA_MIN_DAMAGE     =    0,
    SOTA_MAX_DAMAGE     = 0xFF,
    SOTA_MIN_ATTACK     =    0,
    SOTA_MAX_ATTACK     = 0xFF,
    SOTA_MIN_PROT       =    0,
    SOTA_MAX_PROT       = 0xFF,
    SOTA_MIN_HEAL       =    0,
    SOTA_MAX_HEAL       = 0xFF,

    SOTA_MIN_HIT        =    0,
    SOTA_MAX_HIT        = 0xFF,
    SOTA_MIN_CRIT       =    0,
    SOTA_MAX_CRIT       = 0xFF,
    SOTA_MIN_FAVOR      =    0,
    SOTA_MAX_FAVOR      = 0xFF,

    SOTA_MIN_SPEED      =  -50,
    SOTA_MAX_SPEED      =   50,
    SOTA_MAX_DODGE      =  100,
    SOTA_MIN_DODGE      = -100, /* negative: Slowed by big weapons */
    SOTA_MIN_AGONY      =    0,
    SOTA_MAX_AGONY      =   10,
    SOTA_MIN_RANGE      =    0,
    SOTA_MAX_RANGE      =   12,

    SOTA_MAX_SUPPORTS       =    7,
    SOTA_SUPPORT_RANGE      =    3,
    SOTA_MAX_MOVEMENT       =   16,
    SOTA_MAX_PARTY_SIZE     =   40,
    SOTA_MAX_TRUE_DAMAGE    =    4,

    SOTA_MIN_STAT           =    0, /* absolute unit stat minimum */
    SOTA_MAX_STAT           =   50, /* absolute unit stat maximum */

    SOTA_MIN_LEVEL          =    0,
    SOTA_MAX_LEVEL          =   40,
};

enum SOTA_ANTS {
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

#define REGISTER_ENUM(x) TNECS_PIPELINE_##x,
enum TNECS_PIPELINES {
    TNECS_PIPELINE_NULL = 0,
#include "names/tnecs_pipelines.h"
    TNECS_PIPELINE_NUM
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) TNECS_RENDER_PHASE_##x,
enum TNECS_RENDER_PHASE {
    TNECS_RENDER_PHASE_NULL = 0,
#include "names/tnecs_render_phases.h"
    TNECS_RENDER_PHASE_NUM
};
#undef REGISTER_ENUM


#define PALETTE_DEFAULT palette_SOTA
#define REGISTER_ENUM(x) PALETTE_##x,
enum PALETTES {
    PALETTE_START                        =  0,
#include "names/palettes.h"
    PALETTE_NUM,
};
#undef REGISTER_ENUM

#define SOTA_COLORKEY_RGB 0x00, 0x00, 0x01 /* RBG */
enum SOTA_COLORS {
    /* --- COLORKEY --- */
    /* Transparent color index for indexed images   */
    /* In sota, it has RGB color 000001             */
    SOTA_COLORKEY           =  0,
    SOTA_RED                =  9,
    SOTA_WHITE              = 40,
    SOTA_BLACK              =  1,
    SOTA_PURPLE             = 13,
    SOTA_GREEN              = 33,
    SOTA_YELLOW             = 22,
    SOTA_DARK_GRAY          =  3,
    SOTA_MENU_BLUE          = 28,
    PALETTE_SOTA_COLOR_NUM  = 41
};

enum FPS_DEFAULTS {
    FPS_DEFAULT_POSX            =  850,
    FPS_DEFAULT_POSY            =   10,
    FPS_DEFAULT_SHOW            =    1,
    FPS_DEFAULT_CAP             =   60,
    FAST_FORWARD_DEFAULT_CAP    = 2000,
    FPS_DEFAULT_COLORR          =    0,
    FPS_DEFAULT_COLORG          =    0,
    FPS_DEFAULT_COLORB          =    0,
    FPS_DEFAULT_COLORA          =  255,
    FPS_SCALE                   =    5
};

enum FRAME_PAUSE_MODE {
    FRAME_PAUSE_FLAT    = 0,
    FRAME_PAUSE_FACTOR  = 1,
    FRAME_PAUSE_SPEED   = 2,
};

enum SOTA_CONVERSIONS {
    SOTA_ns                 = 1000000000UL, /* [ns/s] */
    SOTA_us                 =    1000000, /* [us/s] */
    SOTA_ms                 =       1000, /* [ms/s] */
    IES_ns                  = 1000000000UL, /* [ns/s] */
    IES_us                  =    1000000, /* [us/s] */
    IES_ms                  =       1000, /* [ms/s] */
};

enum SOTA_POPUP_WAIT {
    POPUP_SLIDEWAIT_ns      = 20000000, // -> barely above 1 frame at 60fps
};

enum SOTA_CONTROL {
    SOTA_PLAYER = 0,
    SOTA_AI     = 1,
};

enum SOTA_CONTROLLER_WAIT {
    KEYBOARD_MINHELD_ns     = 0,
    GAMEPAD_MINHELD_ns      = 0,
};

enum SOTA_CURSOR {
    CURSOR_FIRSTMOVE_PAUSE_ms =       250,
    // CURSOR_SLIDEWAIT_ns       =   00000,
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

#define REGISTER_ENUM(x) MOUNT_##x,
enum MOUNT_TYPES {
    MOUNT_NULL       = 0,
#define REGISTER_ENUM(x) MOUNT_##x,
#include "names/mounts.h"
    MOUNT_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x,y,z) COMPONENT_ID(x),
enum COMPONENT {
    COMPONENT_NULL  = TNECS_NULL,
#include "names/components.h"
    COMPONENT_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) AI_##x,
enum AIs {
    AI_NULL     = 0,
#include "names/ai.h"
    AI_NUM,
    AI_DEFAULT = AI_AGGRESSIVE,
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

enum OPTIONS {
    MENU_OPTION_START = 0,
#define REGISTER_ENUM(x, y) MENU_OPTION_##x,
#include "names/menu/options.h"
#undef REGISTER_ENUM
    MENU_OPTION_END,
    MENU_OPTION_NUM = MENU_OPTION_END,
};

enum LOADOUT_TYPE {
    LOADOUT_EQUIPPED    = 0, /* With equipped weapons                   */
    LOADOUT_EQUIPMENT   = 1, /* With weapons in equipment               */
    LOADOUT_LOADOUT     = 2, /* With weapons in loadout struct equipped */
    LOADOUT_EQ          = 3, /* With weapon specified by eq index       */
};

enum ENUM_EQ { /* enum_equipment */
    ITEM_UNEQUIPPED             = 0,
    ITEM_NULL                   = 0,
    ITEM1,
    ITEM2,
    ITEM3,
    ITEM4,
    ITEM5,
    ITEM6,
    SOTA_EQUIPMENT_SIZE         = ITEM6,
};


enum TWO_HAND_EQ_MODE {
    TWO_HAND_EQ_MODE_STRICT   = 0,
    TWO_HAND_EQ_MODE_LOOSE    = 1,
};

/* ITEM_ID is the explicit, human-friendly item index
* EXAMPLE:
*   global_itemNames[ITEM_ORDER...] -> correct item name
*/

enum ITEM {
    ITEM_DESCRIPTION_LEN = DEFAULT_BUFFER_SIZE * 2
};

enum ITEM_ID {
    ITEM_ID_START = 0,
#define REGISTER_ENUM(x, y) ITEM_ID_##x = y,
#include "names/items.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_ID_TALISMAN_##x,
#include "names/units_stats.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_ID_SCRIPTURE_##x,
#include "names/skills_passive.h"
#include "names/skills_active.h"
#undef REGISTER_ENUM

    ITEM_ID_BOOKEND,
    ITEM_ID_END,
};

// ITEM_ORDER is the implicit, compiler-friendly item position in array
// global_itemNames[ITEM_ORDER...] -> correct item name
enum ITEM_ORDER {
    ITEM_ORDER_START = 0,
#define REGISTER_ENUM(x, y) ITEM_ORDER_##x,
#include "names/items.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_ORDER_TALISMAN_##x,
#include "names/units_stats.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_ORDER_SCRIPTURE_##x,
#include "names/skills_passive.h"
#include "names/skills_active.h"
#undef REGISTER_ENUM

    ITEM_NUM,
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
enum ITEM_TYPE {
    ITEM_TYPE_NULL  = 0,
    ITEM_TYPE_START = 0,
#include "names/items_types.h"
    ITEM_TYPE_END = 1UL << ITEM_TYPE_EXP_END,
};
#undef REGISTER_ENUM

/* --- ITEM HIERARCHY --- */
/* Example: Fleuret                 */
/*        archetype -> Weapon         */
/*        Type.top  -> Sword          */
/*        Type.sub  -> Thrust Sword   */
// Archetype is weapon type with possibly multiple bits enabled
enum ITEM_ARCHETYPE {
    ITEM_ARCHETYPE_NULL     =   ITEM_TYPE_NULL,
    ITEM_ARCHETYPE_ITEM     =   ITEM_TYPE_ITEM,
    ITEM_ARCHETYPE_STAFF    =   ITEM_TYPE_STAFF,
    ITEM_ARCHETYPE_SHIELD   =   ITEM_TYPE_SHIELD,
    ITEM_ARCHETYPE_WEAPON   =   ITEM_TYPE_SWORD     | ITEM_TYPE_LANCE       |
                                ITEM_TYPE_AXE       | ITEM_TYPE_ELEMENTAL   |
                                ITEM_TYPE_DEMONIC   | ITEM_TYPE_ANGELIC     |
                                ITEM_TYPE_CLAW      | ITEM_TYPE_STANDARD    |
                                ITEM_TYPE_BOW       | ITEM_TYPE_OFFHAND,
    ITEM_ARCHETYPE_WEAKHAND =   ITEM_TYPE_OFFHAND   | ITEM_TYPE_SHIELD      |
                                ITEM_TYPE_ITEM      | ITEM_ARCHETYPE_WEAPON,
    /* All weapon types that can be equipped for Unit action: Attack */
    ITEM_ARCHETYPE_STRONGHAND_ATTACK    = ITEM_ARCHETYPE_WEAPON,
    /* All weapon types that can be equipped for Unit action: Staff */
    ITEM_ARCHETYPE_STRONGHAND_STAFF     = ITEM_ARCHETYPE_STAFF,
    ITEM_ARCHETYPE_NUM = 7,
};

enum TEXT_COLOR {
    SOTA_MALUS_RED   = 0xB2,
    SOTA_MALUS_GREEN = 0x10,
    SOTA_MALUS_BLUE  = 0x30,

    SOTA_BONUS_RED   = 0x71,
    SOTA_BONUS_GREEN = 0xF3,
    SOTA_BONUS_BLUE  = 0x41,
};

enum HPBAR {
    HPBAR_HEIGHT                =  5,
    HPBAR_LEN                   = 35,
    HPBAR_PATTERN_MULTIPLIER    =  4,
    HPBAR_PATTERN_START         =  6,
    HPBAR_HPPERROW              = 30, /* 92 pixels, about 6 tiles */
    HPBAR_TILESIZE_X            =  4, /* [pixels] */
    HPBAR_TILESIZE_Y            = 10, /* [pixels] */
    HPBAR_TEXTURE_NONE          =  0,
    HPBAR_TEXTURE_FILLED        =  1,
    HPBAR_TEXTURE_EMPTY         =  2,
    HPBAR_TEXTURE_PDAMAGE       =  3,
    HPBAR_TEXTURE_MDAMAGE       =  4,
    HPBAR_TEXTURE_TDAMAGE       =  5,
    HPBAR_TEXTURE_OVERHEAL      =  6, /* SAME AS SHIELD */
};
#define HPBAR_UPDATEWAIT 0.05f /* [s] */

enum ITEM_DIVISORS {
    ITEM_DIVISOR            = 100,
    ITEM_DIVISOR_SWORD      =   1,
    ITEM_DIVISOR_LANCE      = 100,
    ITEM_DIVISOR_AXE        = 200,
    ITEM_DIVISOR_BOW        = 300,
    ITEM_DIVISOR_SHIELD     = 400,
    ITEM_DIVISOR_OFFHAND    = 500,
    ITEM_DIVISOR_MAGIC      = 600,
    ITEM_DIVISOR_ELEMENTAL  = 600,
    ITEM_DIVISOR_DEMONIC    = 700,
    ITEM_DIVISOR_ANGELIC    = 800,
    ITEM_DIVISOR_STAFF      = 900,
};

#define REGISTER_ENUM(x) UNIT_TITLE_##x,
enum UNIT_TITLES {
    UNIT_TITLE_NULL = 0,
#include "names/titles.h"
    UNIT_TITLE_NUM,
};
#undef REGISTER_ENUM

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

#define REGISTER_ENUM(x, y) extern const  u64 UNIT_PC_ALIVE_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const  u64 UNIT_PC_RECRUITED_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) extern const  u64 UNIT_NPC_ALIVE_##x;
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

#define REGISTER_ENUM(x, y) extern const u64 ITEM_EFFECT_##x;
#include "names/items_effects.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) CAMPJOB_##x,
enum CAMPJOBS {
    CAMPJOB_START = 0,
#include "names/camp_jobs.h"
    // #undef REGISTER_ENUM
    CAMPJOB_END,
    // #define REGISTER_ENUM(x) CAMPJOB_INDEX_##x = 1 << (CAMPJOB_##x - 1),
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(name, id, x, y, z) UNIT_CLASS_##name = id,
enum UNIT_CLASSES {
    UNIT_CLASS_START = 0,
#include "names/classes.h"
    UNIT_CLASS_END,
    UNIT_CLASS_NUM = UNIT_CLASS_END,
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

#define REGISTER_ENUM(x, y) ARMY_##x,
enum UNIT_ARMIES {
    ARMY_NULL   = 0,
    ARMY_START  = 0,
#include "names/armies.h"
    ARMY_END,
    ARMY_MALLOC = ARMY_END,
    ARMY_NUM    = ARMY_END - 1,
};
#undef REGISTER_ENUM

extern const u8 army_control[ARMY_MALLOC];

/* UNIT_ID is the explicit, human-friendly unit index
* EXAMPLE:
*   global_unitNames[UNIT_ORDER...] -> correct item name
*/

#define REGISTER_ENUM(x, y) UNIT_ID_##x = y,
enum UNIT_IDS {
    UNIT_NULL           = 0,
    UNIT_ID_NULL        = 0,
    UNIT_ID_START       = 0,
    UNIT_ID_PC_START    = 0,
#include "names/units_PC.h"
#include "names/units_NPC.h"
    UNIT_ID_END,
    UNIT_ID_NUM = UNIT_ID_END,
};
#undef REGISTER_ENUM

// UNIT_ORDER is the implicit, compiler-friendly item position in array
// global_unitNames[ITEM_ORDER...] -> correct item name
// name should be all caps
#define UNIT_ORDER(name) UNIT_ORDER_##name
#define REGISTER_ENUM(x, y) UNIT_ORDER_##x,
enum UNIT_ORDER {
    UNIT_ORDER_START = 0,
#include "names/units_PC.h"
#include "names/units_NPC.h"
    UNIT_NUM,
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

enum INFUSE {
    SOTA_NON_INFUSEABLE = -1, /* for magic weapons */
    SOTA_INFUSEABLE     =  0,
    SOTA_INFUSE_FACTOR  =  3,
};

/* TODO: Combine all skills into one list */
#define REGISTER_ENUM(x) PASSIVE_SKILL_##x,
enum PASSIVE_SKILLS {
    SKILL_START         = 0,
    PASSIVE_SKILL_START = 0,
#include "names/skills_passive.h"
    PASSIVE_SKILL_END,
    PASSIVE_SKILL_NUM = PASSIVE_SKILL_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ACTIVE_SKILL_##x,
enum ACTIVE_SKILLS {
    ACTIVE_SKILL_START = 0,
#include "names/skills_active.h"
    ACTIVE_SKILL_END,
    ACTIVE_SKILL_NUM = ACTIVE_SKILL_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const u64 UNIT_SKILL_##x;
#include "names/skills_passive.h"
#include "names/skills_active.h"
enum UNIT_SKILL {
    UNIT_SKILL_START =  0,
    UNIT_SKILL_NUM   = 49,
    UNIT_SKILL_END   = UNIT_SKILL_NUM,
};
#undef REGISTER_ENUM

enum STAT_ID {
    STAT_ID_NULL = 0,
};

#define REGISTER_ENUM(x) UNIT_STAT_##x,
enum UNIT_STATS {
    UNIT_STAT_NULL      = 0,
#include "names/units_stats.h"
    UNIT_STAT_MALLOC,
    UNIT_STAT_NUM       = UNIT_STAT_MALLOC - 1

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
    TILE_ICON_UNKNOWN   = 4,
    TILE_ICON_REINF     = 5,
    TILE_ICON_DANGERW   = 6, /* whiter danger */
    TILE_ICON_DANGERB   = 7, /* darker danger */
    TILE_ICON_LOOT1     = 8,
    TILE_ICON_LOOT2     = 9,
};

enum GAME_STATES {
    GAME_STATE_START = 0,
#define REGISTER_ENUM(x, y) GAME_STATE_##x,
#include "names/game_states.h"
    GAME_STATE_END,
    GAME_STATE_NUM = GAME_STATE_END,
#undef REGISTER_ENUM
};

#define REGISTER_ENUM(x, y) GAME_SUBSTATE_##x,
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
#define REGISTER_ENUM(x, y) SOTA_INPUT_##x,
enum SOTA_INPUTS {
    SOTA_INPUT_DPAD_END         =  4,
    SOTA_INPUT_START            = -1,
#include "names/input.h"
    SOTA_INPUT_END,
};
#undef REGISTER_ENUM

enum SOTA_BUTTONS {
    SOTA_MAPPABLE_BUTTONS_NUM   =  2,
    SOTA_BUTTON_DPAD_END        =  4,
    SOTA_BUTTON_DPAD_RIGHT      =  0,
    SOTA_BUTTON_DPAD_UP,
    SOTA_BUTTON_DPAD_DOWN,
    SOTA_BUTTON_DPAD_LEFT,
    SOTA_BUTTON_A,
    SOTA_BUTTON_B,
    SOTA_BUTTON_X,
    SOTA_BUTTON_Y,
    SOTA_BUTTON_START,
    SOTA_BUTTON_SHOULDER_LEFT,
    SOTA_BUTTON_SHOULDER_RIGHT,
    SOTA_BUTTON_TRIGGER_LEFT,
    SOTA_BUTTON_TRIGGER_RIGHT,
    SOTA_BUTTON_KEYBOARD_M,     /* Debug: toggle */
    SOTA_BUTTON_KEYBOARD_SPACE, /* Debug: reload */
    SOTA_BUTTON_END,
};
extern const char sdl_buttons[SOTA_BUTTON_END];

enum SKILL_VALUES {
    SOTA_SKILL_PINPRICK         = 1,
    SOTA_SKILL_PIERCE           = 2,
    SOTA_SKILL_CRUSH            = 4,
};

/* --- DAMAGE TYPES --- */
enum DMG_TYPES {
    DMG_PHYSICAL    = 0,
    DMG_MAGICAL     = 1,
    DMG_TRUE        = 2,
    DMG_TOTAL       = 3,
    DMG_TYPES       = 4,
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
    /* Fast - Slow */
    SLIDETYPE_GEOMETRIC,
    /* Slow - Fast - Slow */
    SLIDETYPE_EASYINEASYOUT,
    SLIDETYPE_VELOCITY,
    SLIDETYPE_VECTOR,
    SLIDETYPE_END,
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

#define REGISTER_ENUM(x, y) MENU_TYPE_##x,
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

#define REGISTER_ENUM(x, y, z) CHAPTER_##x,
enum CHAPTERS {
    NO_CHAPTER      = 0,
    CHAPTER_NULL    = 0,
    CHAPTER_START   = 0,
#include "names/chapters.h"
    CHAPTER_FOLDER_TEST,
    CHAPTER_MALLOC,
    CHAPTER_NUM     = CHAPTER_MALLOC - 1
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) SCENE_TIME_##x,
enum SCENE_TIME {
    SCENE_TIME_NULL = 0,
#include "names/scene_time.h"
    SCENE_TIME_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) SCENE_##x,
enum SCENE {
    SCENE_NULL = 0,
#include "names/scene.h"
    SCENE_NUM,
    SCENE_MAX_ACTORS    = 8,
    SCENE_DIVISOR       = 100,
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

enum SOTA_OCTANT_DIRECTIONS { /* ternary -> octant */
    /* Intuitive screen direction */
    /* Unbalanced ternary: */
    /* For direction [x,y] with x,y in [-1,0,1], */
    /* SOTA_DIRECTION_... == 3^x + 3^y */
    /* Area split into octants around 2D area */
    // NOTE: Order in enum is array index
    SOTA_DIRECTION_NULL         =  0,
    SOTA_DIRECTION_INSIDE       =  0,
    SOTA_DIRECTION_RIGHT        =  2,
    SOTA_DIRECTION_TOP          =  6, /* Intuitive -> top of screen */
    SOTA_DIRECTION_LEFT         =  1,
    SOTA_DIRECTION_BOTTOM       =  3,
    SOTA_DIRECTION_TOPRIGHT     =  8,
    SOTA_DIRECTION_TOPLEFT      =  7,
    SOTA_DIRECTION_BOTRIGHT     =  5,
    SOTA_DIRECTION_BOTLEFT      =  4,
    SOTA_DIRECTIONS_MAIN_NUM    =  4,
    SOTA_DIRECTIONS_NUM         =  8, /* Including diagonals */
};

enum SOTA_TERNARY {
    /* Unbalanced ternary */
    SOTA_TERNARY_EQUAL  =   0,
    SOTA_TERNARY_BELOW  =   1,
    SOTA_TERNARY_ABOVE  =   2,
};


/* Direction array index from ternary index */
extern const int direction_arr_i[SOTA_DIRECTIONS_NUM + 1];

enum SOTA_COMBAT {
    /* glossary */
    SOTA_DEFENDANT                  = 0,
    SOTA_AGGRESSOR                  = 1,

    SOTA_BRAVE_DEFAULT              = 1,
    SOTA_BRAVE_MAX                  = 7, /* For Fingolfin */

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

enum MAP {
    MAP_MAX_COLS = 50,
    MAP_MAX_ROWS = 50
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
    MAP_CONDITION_NONE              = 0,
    MAP_CONDITION_WIN_BOSS          = 1,
    MAP_CONDITION_LOSS_MAIN_CHAR    = 2,
    MAP_CONDITION_LOSS_DEBUG_MAP    = 3,
    MAP_CONDITION_ROUT,
};

enum OVERLAYS {
    MAP_OVERLAY_NULL            = 0,
    MAP_OVERLAY_HEAL            = 1 << 0,
    MAP_OVERLAY_MOVE            = 1 << 1,
    MAP_OVERLAY_ATTACK          = 1 << 2,
    MAP_OVERLAY_DANGER          = 1 << 3,
    MAP_OVERLAY_GLOBAL_DANGER   = 1 << 4,
    MAP_OVERLAY_START_POS       = 1 << 5,
};

enum LOOPINGS {
    LOOPING_INVALID          = 0,
    LOOPING_PINGPONG         = 1,
    LOOPING_FORWARD,
    LOOPING_REVERSE,
    LOOPING_NUM,
};

enum ARRAY_TYPE {
    ARRAY_MATRIX            = 0,
    ARRAY_LIST              = 1,
};

enum WEAPON_SUBTYPE {
    WEAPON_SUBTYPE_NONE      = 0,
    WEAPON_SUBTYPE_THRUST    = 1,
};

enum UNIT_HAND {
    /* Only first two hands can be strong/weak hands */
    UNIT_HAND_LEFT              = ITEM1,
    UNIT_HAND_RIGHT             = ITEM2,
    UNIT_ARMS_NUM               = 2,
    TETRABRACHIOS_HAND_LEFT     = ITEM3, /* TOP/BOTTOM LEFT     */
    TETRABRACHIOS_HAND_RIGHT    = ITEM4, /* TOP/BOTTOM RIGHT    */
    TETRABRACHIOS_ARMS_NUM      = 4,
    MAX_ARMS_NUM                = 4,
};
#define UNIT_OTHER_HAND(hand) 2 - (hand - 1) /* Only for L/R hands*/

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
    SOTA_RN_SINGLE = 1,
    SOTA_RN_DOUBLE,
    SOTA_RN_HYBRID,
    SOTA_RN_GAUSSIAN,
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

enum MOUSE_DEFAULTS {
    MOUSE_DEFAULT_ONHOLD    = MOUSE_ACCEPT,
    MOUSE_DEFAULT_MOVE      = MOUSE_FOLLOW,
    MOUSE_DEFAULT_SRCRECT_H = 32,
    MOUSE_DEFAULT_SRCRECT_W = 32,
    MOUSE_DEFAULT_DSTRECT_H = 80,
    MOUSE_DEFAULT_DSTRECT_W = 80,
};

enum BOSS_ICONS {
    BOSS_ICON_WIDTH     =  8,
    BOSS_ICON_HEIGHT    = 16,
    BOSS_ICON_OFFSET_X  =  9,
    BOSS_ICON_OFFSET_Y  =  6,
    BOSS_ICON_NULL      =  0,
    BOSS_ICON_STGEORGE  =  1,
    BOSS_ICON_ORIFLAMME =  2, /* Frankish battle standard */
    BOSS_ICON_KEWAC     =  3,
    BOSS_ICON_KESIRU    =  4,
    BOSS_ICON_VOLDAN    =  5,
    BOSS_ICON_HAMILCAR  =  5,
    BOSS_ICON_NUM       =  6,
};

enum SOTA_CONVOY {
    SOTA_CONVOY_SIZE_MAX = 200,
    SOTA_CONVOY_NULL = 0,
    SOTA_CONVOY_SIZE_START = 50,
    SOTA_CONVOY_SORT_HIGH2LOW = 0,
    SOTA_CONVOY_SORT_LOW2HIGH = 1,
};

/* --- Settings --- */
#define SOTA_BRIGHTNESS_MAX 1.0f
#define SOTA_BRIGHTNESS_MIN 0.0f

enum PLAYER_SETTINGS {
    SOTA_SETTINGS_NULL              = -1,
    SOTA_SETTINGS_ANIMATION         =  0,
    SOTA_SETTINGS_TEXT_SPEED        =  1,
    SOTA_SETTINGS_AUTOCURSOR        =  2,
    SOTA_SETTINGS_AUTOEND_TURNTURNS =  3,
    SOTA_SETTINGS_VOLUME_MUSIC      =  4,
    SOTA_SETTINGS_VOLUME_SOUNDFX    =  5,
    SOTA_SETTINGS_BRIGHTNESS        =  6,
    SOTA_SETTINGS_NUM,
};

enum ANIMATION_SETTINGS {
    ANIMATION_SETTINGS_NULL         = -1,
    ANIMATION_SETTINGS_ON           =  0,
    ANIMATION_SETTINGS_FRIENDLY     =  1,
    ANIMATION_SETTINGS_ENEMY        =  2,
    ANIMATION_SETTINGS_OFF          =  3,
    ANIMATION_SETTINGS_NUM,
};

enum TEXT_SPEED_SETTINGS {
    TEXT_SPEED_SETTINGS_NULL        = -1,
    TEXT_SPEED_SETTINGS_SLOW        =  0,
    TEXT_SPEED_SETTINGS_MID         =  1,
    TEXT_SPEED_SETTINGS_FAST        =  2,
    TEXT_SPEED_SETTINGS_MAX         =  3,
    TEXT_SPEED_SETTINGS_NUM,
};

enum AUTOCURSOR_SETTINGS {
    AUTOCURSOR_SETTINGS_NULL        = -1,
    AUTOCURSOR_SETTINGS_OFF         =  0,
    AUTOCURSOR_SETTINGS_ON          =  1,
};

enum AUTOEND_TURNS_SETTINGS {
    AUTOEND_TURNS_SETTINGS_NULL     = -1,
    AUTOEND_TURNS_SETTINGS_OFF      =  0,
    AUTOEND_TURNS_SETTINGS_ON       =  1,
};

#endif /* ENUMS_H */
