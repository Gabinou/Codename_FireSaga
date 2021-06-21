#ifndef ENUMS_H
#define ENUMS_H

#include "stdbool.h"
#include "stdint.h"

#define STRINGIFY(x) #x

#define SAVE_FOLDER "saves"
#define GAME_TITLE "Codename: FireSaga"
#define EXE_NAME "CodenameFireSaga"
#define flagsum_isIn(flag, flagsum) ((flag & flagsum) > 0)

enum MATHS {
    SQUARE_NEIGHBOURS = 4,
    HEXAGON_NEIGHBOURS = 6,
};

enum DEFAULTS {
    DEFAULT_TILESIZE = 32,
    DEFAULT_EQUIPMENT_SIZE = 6,
    DEFAULT_WEAPONS_SIZE = 3,
    DEFAULT_ITEMS_SIZE = 3,
    DEFAULT_CONVOY_SIZE = 200,
    DEFAULT_LINE_LENGTH = 255,
    DEFAULT_TILE_DIVISOR = 10,
    DEFAULT_TEXT_LINES = 8,
    DEFAULT_TILE_MAX = 99,
    DEFAULT_MAXPATH = 90,
    DEFAULT_CHAP_NUMBER = 40,
    DEFAULT_TILEMAP_XOFFSET = 16,
    DEFAULT_TILEMAP_YOFFSET = 16,
    DEFAULT_BUFFER_SIZE = 128,
    DEFAULT_SUPPORTS_MAX = 7,
    DEFAULT_BOOKS_NUM = 10,
    DEFAULT_HYAKUPERCENT = 100,
    DEFAULT_DOUBLEHIT_SPEED = 4,
    DEFAULT_MAPPABLE_BUTTONS = 2,
    MAX_PARTY_SIZE = 40,
    MAX_INPUT_ARGS = 15,
    COMBAT_MAX_ATTACKS = 9,
    HYAKU_PERCENT = 100,
    DEFAULT_CURSOR_FRAMES = 10,
    DEFAULT_CURSOR_SPEED = 50,
    CRIT_FACTOR = 150,
    EFFECTIVE_FACTOR = 200,
    NOTEFFECTIVE_FACTOR = 100,
    CRIT_FACTOR_PROMOTED = 200,
    CRIT_FACTOR_ASSASSIN = 250,
    MOVE_WITH_MOUNT = 7,
};

enum FPS_DEFAULTS {
    FPS_DEFAULT_POSX = 850,
    FPS_DEFAULT_POSY = 10,
    FPS_DEFAULT_SHOW = 1,
    FPS_DEFAULT_CAP = 60,
    FPS_DEFAULT_COLORR = 0,
    FPS_DEFAULT_COLORG = 0,
    FPS_DEFAULT_COLORB = 0,
    FPS_DEFAULT_COLORA = 255,
};

enum UNIT_ACTIONS {
    UNIT_ACTION_MOVE = 0,
    UNIT_ACTION_WAIT = 1,
    UNIT_ACTION_ATTACK = 2,
    UNIT_ACTION_SEIZE = 3,
    UNIT_ACTION_OPEN_DOOR = 4,
    UNIT_ACTION_OPEN_CHEST = 5,
    UNIT_ACTION_DANCE = 6,
    UNIT_ACTION_STAFF = 7,
    UNIT_ACTION_USE_ITEM = 8,
    UNIT_ACTION_ESCAPE = 9,
    UNIT_ACTION_RESCUE = 10,
    UNIT_ACTION_STEAL = 11,
    UNIT_ACTION_STAFF_PUSH = 12,
    UNIT_ACTION_STAFF_PULL = 13,
    UNIT_ACTION_STAFF_HEAL = 14,
};

#define KEYBOARD_MINHELD 0.0f
#define CURSOR_FASTTIME 0.35f
#define CURSOR_SLIDEWAIT 0.01f
#define FPS_DEFAULT_SIZEFACTORX 1.0f
#define FPS_DEFAULT_SIZEFACTORY 1.0f
#define DEFAULT_TEXT_UPDATETIME 0.5f
#define DEFAULT_TEXT_RENDERTIME 0.5f

#define REGISTER_ENUM(x) MOUNT_TYPE_##x,
enum MOUNT_TYPES {
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

#define REGISTER_ENUM(x, y) SHOP_CAMP_CHAPTER_##x,
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
#include "names/menu_options.h"
    MENU_OPTION_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) ITEM_NAME_##x = y,
enum ITEM_NAMES {
    ITEM_NAME_START = 0,
#include "names/items.h"
#undef REGISTER_ENUM
    ITEM_NAME_SWORD_START = 0,
    ITEM_NAME_SWORD_END = 29,
    ITEM_NAME_LANCE_START = 99,
    ITEM_NAME_LANCE_END = 116,
    ITEM_NAME_AXE_START = 200,
    ITEM_NAME_AXE_END = 215,
    ITEM_NAME_BOW_START = 300,
    ITEM_NAME_BOW_END = 311,
    ITEM_NAME_SHIELD_START = 400,
    ITEM_NAME_SHIELD_END = 417,
    ITEM_NAME_OFFHAND_START = 500,
    ITEM_NAME_OFFHAND_END = 526,
    ITEM_NAME_ELEMENTAL_START = 600,
    ITEM_NAME_MAGIC_START = 600,
    ITEM_NAME_ELEMENTAL_END = 608,
    ITEM_NAME_MAGIC_END = 608,
    ITEM_NAME_DEMONIC_START = 700,
    ITEM_NAME_DEMONIC_END = 709,
    ITEM_NAME_ANGELIC_START = 800,
    ITEM_NAME_ANGELIC_END = 808,
    ITEM_NAME_STAFF_START = 900,
    ITEM_NAME_STAFF_END = 910,
    ITEM_NAME_CLAW_START = 1000,
    ITEM_NAME_CLAW_END = 1005,
    ITEM_NAME_ITEM_START = 1100,
    ITEM_NAME_ITEM_END = 1107,
    ITEM_NAME_BOOK_START = 5000,
    ITEM_NAME_BOOK_END = 5007,
#define REGISTER_ENUM(x) ITEM_NAME_TALISMAN_##x,
#include "names/units_stats.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_NAME_SCRIPTURE_##x,
#include "names/skills.h"
    ITEM_NAME_BOOKEND,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_TYPE_EXP_##x,
enum ITEM_TYPE_EXP {
    // ITEM_TYPE_EXP_START = 0,
#include "names/items_types.h"
    ITEM_TYPE_EXP_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_TYPE_##x = 1UL << ITEM_TYPE_EXP_##x,
enum ITEM_TYPES {
    ITEM_TYPE_START = 0,
#include "names/items_types.h"
    ITEM_TYPE_END = ITEM_TYPE_EXP_END,
};
#undef REGISTER_ENUM

enum ITEM_DIVISORS {
    ITEM_DIVISOR_SWORD = 1,
    ITEM_DIVISOR_LANCE = 100,
    ITEM_DIVISOR_AXE = 200,
    ITEM_DIVISOR_BOW = 300,
    ITEM_DIVISOR_SHIELD = 400,
    ITEM_DIVISOR_OFFHAND = 500,
    ITEM_DIVISOR_MAGIC = 600,
    ITEM_DIVISOR_ELEMENTAL = 600,
    ITEM_DIVISOR_DEMONIC = 700,
    ITEM_DIVISOR_ANGELIC = 800,
    ITEM_DIVISOR_STAFF = 900,
};

#define REGISTER_ENUM(x) ITEM_STAT_##x,
enum ITEM_STATS {
    ITEM_STAT_START = 0,
#include "names/items_stats.h"
    ITEM_STAT_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t UNIT_PC_ALIVE_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t UNIT_PC_RECRUITED_##x;
#include "names/units_PC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t UNIT_NPC_ALIVE_##x;
#include "names/units_NPC.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) ITEM_EFFECT_EXP_##x,
enum ITEM_EFFECT_EXP {
#include "names/items_effects.h"
    ITEM_EFFECT_EXP_END,
    ITEM_EFFECT_END = ITEM_EFFECT_EXP_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t ITEM_EFFECT_##x;
#include "names/items_effects.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) CAMPJOB_##x,
enum CAMPJOBS {
    CAMPJOB_START = 0,
#include "names/camp_jobs.h"
    CAMPJOB_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y, z, i) UNIT_CLASS_##x,
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

#define REGISTER_ENUM(x) UNIT_NAME_##x,
enum UNIT_NAMES {
    UNIT_NAME_START = 0,
    UNIT_NAME_PC_START = UNIT_NAME_START,
#include "names/units_PC.h"
    UNIT_NAME_PC_END,
    UNIT_NAME_NPC_START = UNIT_NAME_PC_END,
#include "names/units_NPC.h"
#undef REGISTER_ENUM
    UNIT_NAME_NPC_END,
    UNIT_NAME_GENERIC_START = UNIT_NAME_NPC_END,
#define REGISTER_ENUM(x, y, z, i) UNIT_NAME_##x,
#include "names/classes.h"
    UNIT_NAME_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_SKILL_EXP_##x,
enum UNIT_SKILL_EXP {
    UNIT_SKILL_EXP_START = 0,
#include "names/skills.h"
    UNIT_SKILL_EXP_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) extern const uint64_t UNIT_SKILL_##x;
#include "names/skills.h"
enum UNIT_SKILL {
    UNIT_SKILL_START = 0,
    UNIT_SKILL_NUM = 42,
    UNIT_SKILL_END = UNIT_SKILL_NUM,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_STATE_EXP_##x,
enum UNIT_STATE_EXP {
    UNIT_STATE_EXP_START = 0,
#include "names/units_states.h"
    UNIT_STATE_EXP_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) UNIT_STATE_##x = 1UL << UNIT_STATE_EXP_##x,
enum UNIT_STATE {
    UNIT_STATE_START = 0,
#include "names/units_states.h"
    UNIT_STATE_END
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) TILE_##x = y,
enum TILES {
    // Basic tile index is 3 digits -> 100
    // two first digits give the tile.
    // third digit gives the asset index.
    // Ex: 324: Peak tile, 4th asset of peak.
    TILE_START = 0,
#include "names/tiles.h"
    TILE_END = 100,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) GAME_STATE_##x,
enum GAME_STATES {
    GAME_STATE_START = 0,
#include "names/game_states.h"
    GAME_STATE_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) GAME_SUBSTATE_##x,
enum GAME_SUPERSTATES {
    GAME_SUBSTATE_START = 0,
#include "names/game_substates.h"
    GAME_SUBSTATE_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x) JSON_##x,
enum JSON_ELEMENT {
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

#define REGISTER_ENUM(x) GAME_BUTTON_##x,
enum BUTTONS {
    GAME_BUTTON_START = 0,
#include "names/buttons.h"
    GAME_BUTTON_END,
};
#undef REGISTER_ENUM

enum DMG_TYPES {
    DMG_TYPE_PHYSICAL = 0,
    DAMAGE_TYPE_PHYSICAL = 0,
    DMG_TYPE_MAGICAL =  1,
    DAMAGE_TYPE_MAGICAL = 1,
    DMG_TYPES =  2,
    DAMAGE_TYPES =  2
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
    SLIDETYPE_START = 0,
    SLIDETYPE_GEOMETRIC,
    SLIDETYPE_VECTOR,
    SLIDETYPE_END,
};

#define REGISTER_ENUM(x) MENU_##x,
enum MENUS {
    NO_MENU = 0,
    MENU_START = 0,
#include "names/menu.h"
    MENU_END,
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
    NO_INPUTFLAG = 0,
    INPUTFLAG_START = 1,
#include "names/input_flags.h"
    INPUTFLAG_END,
};
#undef REGISTER_ENUM

#define REGISTER_ENUM(x, y) CHAPTER_##x,
enum CHAPTERS {
    NO_CHAPTER = 0,
    CHAPTER_START = 0,
#include "names/chapters.h"
    CHAPTER_END,
};
#undef REGISTER_ENUM

enum SELECTORS {
    SELECTOR_START = 0,
    SELECTOR_MOUSE,
    SELECTOR_CURSOR,
    SELECTOR_END,
};

enum CONTROLLERS {
    CONTROLLER_START = 0,
    CONTROLLER_GAMEPAD,
    CONTROLLER_MOUSE,
    CONTROLLER_KEYBOARD,
    CONTROLLER_TOUCHPAD,
    CONTROLLER_END,
};

enum UNIT_HANDS {
    UNIT_HAND_RIGHT = 0,
    UNIT_HAND_LEFT = 1,
    UNIT_HANDS_NUM = 2,
};

enum COMBAT {
    ATTACKER = 0,
    DEFENDER = 1,
};

enum UNIT_SEX {
    UNIT_SEX_F = 0,
    UNIT_SEX_M = 1,
};

enum SEXES {
    SEX_F = 0,
    SEX_M = 1,
    SEX_NUM = 2,
};

enum OVERLAYS {
    MAP_OVERLAY_HEAL = 1 << 1,
    MAP_OVERLAY_MOVE = 1 << 2,
    MAP_OVERLAY_ATTACK = 1 << 3,
};

enum LOOPINGS {
    LOOPING_PINGPONG = 1,
    LOOPING_DIRECT,
    LOOPING_LINEAR,
    LOOPING_REVERSE,
};

enum POINTS {
    POINTS_MATRIX = 0,
    POINTS_LIST = 1,
};

enum HANDEDNESS {
    RIGHTIE = 1,
    LEFTIE = 2,
    AMBIDEXTROUS = 3,
};

enum RNS {
    GAME_RN_SINGLE = 1,
    GAME_RN_DOUBLE,
    GAME_RN_HYBRID,
    GAME_RN_GAUSSIAN,
};

enum VISIONBLOCKMAP_CODES {
    VISIONBLOCKMAP_NOT_BLOCKED = 0,
    VISIONBLOCKMAP_BLOCKED = 1,
};

enum STARTUP_MODES {
    NO_STARTUP = 0,
    STARTUP_START = 0,
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
    SIGHTMAP_VISIBLE = 1,
    SIGHTMAP_OBSERVER = 2,
    SIGHTMAP_WALL = 3,
    SIGHTMAP_ENEMY = 4,
    SIGHTMAP_FRIENDLY = 5,
};

enum UNIT_ALIGNMENT { // Always from the point of view of the player/Erwin
    NO_ALIGNMENT = 0,
    ALIGNMENT_START = 1,
    ALIGNMENT_FRIENDLY = 2,
    ALIGNMENT_BLUE_UNIT = 2,
    ALIGNMENT_ENEMY = 3,
    ALIGNMENT_RED_UNIT = 3,
    ALIGNMENT_END = 4,
};


enum MOUSE_DEFAULTS { // this extern struct is dumb
    MOUSE_DEFAULT_ONHOLD = MOUSE_ACCEPT,
    MOUSE_DEFAULT_MOVE = MOUSE_FOLLOW,
};

enum MODE_MOVETILE {
    MOVETILE_EXCLUDE = 0,
    MOVETILE_INCLUDE = 1,
};

enum ATTACKMAP {
    ATTACKMAP_BLOCKED = 0,
    ATTACKMAP_MOVEABLEMIN = 1,
};

enum ASSAILABLEMAP {
    ASSAILABLE_BLOCKED = 0,
    ASSAILABLE_MOVEABLEMIN = 1,
};

enum PUSHPULLMAP {
    PUSHPULLMAP_UNIT = 0,
    PUSHPULLMAP_BLOCKED = -1,
    PUSHPULLMAP_MINDIST = 1, // minimal moveable distance

};

enum COSTMAP {
    COSTMAP_BLOCKED = 0,
    COSTMAP_MOVEABLEMIN = 1,
};

enum MOVEMAP {
    MOVEMAP_BLOCKED = 0,
    MOVEMAP_MOVEABLEMIN = 1,
};

enum BLOCKMAP {
    BLOCKMAP_BLOCKED = 0,
    BLOCKMAP_MIN = 1,
};

enum GRADIENTMAP {
    GRADIENTMAP_UNIT = 0,
    GRADIENTMAP_BLOCKED = -1,
    GRADIENTMAP_MINDIST = 1, // minimal moveable distance
};

enum MODE_PATHS {
    PATH_STEP = 0, // i.e. relative path
    PATH_POSITION = 1,  // i.e. absolute path
};


#endif /* ENUMS_H */
