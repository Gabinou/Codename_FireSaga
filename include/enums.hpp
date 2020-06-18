#ifndef ENUMS_HPP
#define ENUMS_HPP

#define SAVE_FOLDER "saves"

namespace DEFAULT {
enum DEFAULTS {
    TILESIZE = 32,
    EQUIPMENT_SIZE = 7,
    WEAPONS_SIZE = 3,
    ITEMS_SIZE = 3,
    CONVOY_SIZE = 200,
    LINE_LENGTH = 255,
    TILE_DIVISOR = 10,
    TILE_MAX = 99,
    MAXPATH = 90,
    CHAP_NUMBER = 40,
    TILEMAP_XOFFSET = 1,
    TILEMAP_YOFFSET = 1,
    BUFFER_SIZE = 128,
    SUPPORTS_MAX = 7,
    BOOKS_NUM = 10,
};
}

namespace MOUSE {
enum MOUSES {
    ACCEPT = 1,
    NOACCEPT,
    FOLLOW,
    TELEPORT,
};
}

namespace SLIDETYPE {
enum SLIDETYPES {
    GEOMETRIC = 1,
    VECTOR,
};
}

namespace MENU {
enum MENUS {
    MAP = 0,
    UNIT = 1,
    MAPMENU = 2,
};
namespace OPTION {
enum OPTIONS {
    ITEMS = 0,
    TALK = 1,
    RESCUE = 2,
    SEIZE = 3,
    ESCAPE = 4,
    ATTACK = 5,
    SHOP = 6,
    ARMORY = 7,
    VILLAGE = 8,
    CONVOY = 9,
    TRADE = 10,
    STAFF = 11,
    MAP = 12,
    WAIT = 13,
    UNITS = 14,
    ENEMYUNITS = 15,
    OBJECTIVES = 16,
    OPTIONS = 17,
    ENDTURN = 18,
};
}
}
namespace ITEM {
namespace NAME {
enum NAMES {
    WOODEN_SWORD = 1,
    KITCHEN_KNIFE = 2, // Maybe should be Sword AND offhand?
    WRATH_SWORD = 3,
    FLEURET = 4,
    RAPIERE = 5,
    GLAIVE = 6,
    SABER = 7,
    FAUCHON = 8,
    IRON_SWORD = 9,
    STEEL_SWORD = 10,
    DAMAS_SWORD = 11,
    MERCIFUL_BLADE = 12,
    BROADSWORD = 13,
    ESPADON = 14,
    OODACHI = 15,
    UCHIGATANA = 16,
    EXSANGUE = 17,
    HAUTECLAIRE = 18,
    SECUNDUS = 19,
    EXCALIBUR = 20,
    GALATINE = 21,
    RAIJINTOU = 22,
    FUUJINTOU = 23,
    HONJOU_MASAMUNE = 24,
    RAW_IRON_SLAB = 25,
    KIRITO = 26,
    KILLY = 27,
    // War scythe: lance and sword
    // Morning-star: sword and axe

    PITCHFORK = 28,
    FIGHTING_STICK = 29,
    IRON_LANCE = 30,
    WRATH_LANCE = 31,
    STEEL_LANCE = 32,
    DAMAS_LANCE = 33,
    HEAVY_SPEAR = 34, // Square spike lance? Bodkin lance?
    TRIDENT = 35,
    MERCIFUL_LANCE = 36,
    JAVELIN = 37,
    PILUM = 38,
    SPEAR = 39,
    VEL = 40,
    GUNGNIR = 41,
    ACHILLES = 42,
    TONBOKIRI = 43,

    IRON_AXE = 44,
    STEEL_AXE = 45,
    DAMAS_AXE = 46,
    WRATH_AXE = 47,
    THROWING_AXE = 48,
    TOMAHAWK = 49,
    PICK_AXE = 50,
    HOE = 51,
    CLUB = 52,
    HALBERD = 53,
    HAMMER = 54,
    WAR_PICK = 55,
    BATTLEAXE = 56,
    VIDYU = 57,
    HEPHAESTUS = 58,

    ADAPTED_CROSSBOW = 59,
    REPEATABLE_CROSSBOW = 60,
    CROSSBOW = 61,
    SHORT_BOW = 62,
    LONG_BOW = 63,
    COMPOSITE_BOW = 64,
    RECURVE_CROSSBOW = 65,
    RECURVE_BOW = 66,
    APOLLO = 67,
    ARTEMIS = 68,
    PINAKA = 69,

    ADAPTED_SHIELD = 70,
    POT_LID = 71,
    WOODEN_SHIELD = 72,
    CLOAK = 73,
    LEATHER_SHIELD = 74,
    KITE_SHIELD = 75,
    IRON_BUCKLER = 76,
    STEEL_BUCKLER = 77,
    SPIKED_BUCKLER = 78,
    SCUTUM = 79,
    MASTER_SHIELD = 80,
    GODDESS_HAND = 81,
    ANCILE = 82,
    BOULDER = 83,
    OBSIDIAN_SLAB = 84,
    MARBLE_PLATE_SHIELD = 85,
    DIVINE_PROTECTOR = 86,

    WRISTBLADE = 87, // Why? how to draw?
    SHIELD_HOOK = 88,
    RETRACTABLE_WRISTBLADE = 89, // Why? how to draw?
    MAIN_GAUCHE = 90,
    SWORDBREAKER = 91,
    TRIDENT_DAGGER = 92,
    TANTOU = 93,
    KODACHI = 94, // too similar to tantou and wakizashi
    DAGUE = 95, // too similar to main gauche
    WAKIZASHI = 96,
    BRAQUEMARD = 97,
    MISERICORDE = 98,
    CARNWENNAN = 99,
    FLINTLOCK = 100,
    CIBO = 101, // very tall white dagger
    SHRUNKEN_HEAD = 102, //-> human Skull.
    CRYSTAL = 103,
    GLASS_ORB = 104,
    HUMAN_FEMUR = 105,
    VOID_FRAGMENT = 106,
    DEMON_FANG = 107,
    ANGEL_FEATHER = 108,
    ANGEL_HAIR = 109,
    VIAL_OF_LIGHT = 110,
    MAIDENS_HANDKERCHIEF = 111,
    WAR_HORN = 112,
    // THROWING_DAGGERS

    BALL_LIGHTNING = 113,
    ICICLE = 114,
    VOLCANO = 115,
    WIND_SPEAR = 116,
    EMBER = 117,
    TSUNAMI = 118,
    GOD_FORGE_FIRE = 119,
    FINGOLFIN = 120,
    MITHRANDIR = 121,

    VOIDD = 122,
    NETHER = 123,
    DOWNFALL = 124,
    CONSUME = 125,
    PUTRIFY = 126,
    GLAURUNG = 127,
    MORGOTH = 128,
    GOTHMOG = 129,
    FATALIS = 130,

    HEAL = 131,
    REMEDY = 132,
    RENEW = 133,
    WISH = 134,
    PRAY = 135,

    SALVE = 136, // heals for 10.
    PANACEA = 137,
    HEALING_POTION = 138,
    TELPERION_EXTRACT = 139,
    LAURELIN_EXTRACT = 140,
    CROSS = 141, // Little healing everyturn if in inventory? you or in range?
    BROKEN = 142,

    BOOKBASICS = 500,
    BOOKWEAPONS = 501,
    BOOKELEMENTAL = 502,
    BOOKMATHEMAGICS = 503,
    BOOKLOVE = 504,
    BOOKGROWTHS = 505,
    BOOKPOSSESSION = 506,

    END,
};
}
namespace TYPE {
enum TYPES {
    // uint16_t!
    // uint16_t equippable = sword + shield;
    // if (type & equippeable) > 0, weapon is equippable.
    SWORD = 0x0001,
    LANCE = 0x0002,
    AXE = 0x0004,
    BOW = 0x0008,
    TRINKET = 0x0010,
    OFFHAND = 0x0020,
    ELEMENTAL = 0x0040,
    DEMONIC = 0x0080,
    ANGELIC = 0x0100,
    SHIELD = 0x0200,
    STAFF = 0x0400,
    CLAW = 0x0800,
    ITEM = 0x1000,
    BOOK = 0x2000,
    END = 0x4000,

};
}
namespace DMG_TYPE {
enum DMG_TYPES {
    PHYSICAL = false,
    MAGICAL =  true,
};
}
namespace STAT {
enum STATS {
    PMIGHT = 1,
    MMIGHT,
    HIT,
    DODGE,
    CRIT,
    FAVOR,
    WGT,
    USES,
    USED,
    USES_LEFT,
    PROF,
    RANGEMIN,
    RANGEMAX,
    HANDLEFT,
    HANDRIGHT,
    PRICE,
    HEAL,
    ID,
    END,
};
}
namespace EFFECT {
enum EFFECTS {
    // uint64_t
    KILL1P = 0x0001,
    BRAVE2X = 0x0002,
    BRAVE3X = 0x0004,
    BRAVE4X = 0x0008,
    NOCOUNTER = 0x0010,
    IGNORE_DEF = 0x0020,
    IGNORE_RES = 0x0040,
    IGNORE_SHIELD = 0x0080,
    USE_HEAL = 0x0100,
    USE_BUFF = 0x0200,
    USE_DIVINE = 0x0400,
    NO_CRIT = 0x0800,
    NO_ATTACK = 0x1000,
    CHARM = 0x2000,
    IMMUNE_MAGIC = 0x2000,
    IMMUNE_PHYS = 0x4000,
    COUP_DE_GRACE = 0x8000,
    BREAK_WEAPON = 0x00010000,
    EXP2X = 0x00020000,
    LIFESTEAL = 0x00040000,
    POISON = 0x00080000,
    CURSED = 0x00100000, // weapon has a chance to deal damage to user.
    MASOCHISM = 0x00200000,
    STUN = 0x00400000,
    PASS = 0x00800000,
    JOUST = 0x01000000,
    SPARE = 0x02000000,
    CAPTURE = 0x04000000,
    HALF_DMG = 0x08000000,
    BREAK_SHIELD = 0x10000000,
};
}
}
namespace CAMPJOB {
enum CAMPJOBS {
    LIBRARIAN = 0,
    COOK = 1,
    GUARD = 2,
    SCRIBE = 3,
    STABLEHAND = 4,
    ASSISTANT = 5,
    CLERGYMAN = 6,
    STORAGEMASTER = 7,
};
}
namespace UNIT {
namespace CLASS {
enum CLASSES {
    LORD = 1,
    LORD_RIDER = 2,
    DUKE = 3,
    DUKE_RIDER = 4,
    ARCHER = 5,
    ARCHER_RIDER = 6,
    MARKSMAN = 7,
    MARKSMAN_RIDER = 8,
    ANGEL = 9,
    DEMON = 10,
    PEGASUS_KNIGHT = 11,
    ANGELIC_INCARNATE = 12,
    DEMONIC_INCARNATE = 13,
    FENCER = 14,
    DUELIST = 15,
    MOUSQUETAIRE = 16,
    PICKPOCKET = 17,
    THIEF = 18,
    ASSASSIN = 19,
    MERCENARY = 20,
    HERO = 21,
    CORSAIR = 22,
    VIKING = 23,
    BANDIT = 24,
    RAVAGER = 25,
    PIKEMAN = 26,
    CAVALIER = 27,
    PALADIN = 28,
    MAGE = 29,
    BATTLEMAGE = 30,
    TROUBADOUR = 31,
    SAGE = 32,
    PRIEST = 33,
    BISHOP = 34,
    ORACLE = 35,
    GENERAL = 36,
    CLERIC = 37,
    DEMIGOD = 38,
    GOD = 39,
    KNIGHT = 40,
    GODDESS = 41,
    TWINBORN = 42,
    END = 43,
};
}
namespace TYPE {
enum TYPES {
    HUMAN = 0x0001,
    MOUNTED = 0x0002,
    FLYING = 0x0004,
    ARMOR = 0x0008,
    DEMON = 0x0010,
    ANGEL = 0x0020,
};
}
namespace ARMY {
enum ARMIES {
    FRIENDLY = 1,
    ERWIN = 2,
    FREE_MILITIA = 3,
    NEUTRAL = 4,
    IMPERIAL = 5,
    THEOCRACY = 6,
    VOLDAN = 7,
    KEWAC = 8,
    ENEMY = 9,
    BANDITS = 10,
};
}
namespace NAME {
enum NAMES {
    ERWIN = 1,
    KIARA = 2,
    SILOU = 3,
    PIROU = 4,
    SERVIL = 5,
    PERIGNON = 6,
    LAUZON = 7,
    NICOLE = 8,
    OTTO = 9,
    SEBASTIAN = 10,
    MICHAEL = 11,
    CHASSE = 12,
    PIERRE = 13,
    MELLY = 14,
    SIMON = 15,
    RAYAN = 16,
    TEHARON = 17,
    TEKAK = 18,
    EUGENE = 19,
    LUCRECE = 20,
    PC_END = 21,

    ZINEDAN = 22,
    ZIDINE = 23, // Zinedine Zidan
    SAWISK = 24,
    BANDIT = 25,
    DUELIST = 26,
    PICKPOCKET = 27,
    THIEF = 28,
    ASSASSIN = 29,
    ARCHER = 30,
    MARKSMAN = 31,
    MERCENARY = 32,
    HERO = 33,
    CORSAIR = 34,
    VIKING = 35,
    RAVAGER = 36,
    CAVALIER = 37,
    PALADIN = 38,
    FENCER = 39,
    MOUSQUETAIRE = 40,
    NPC_END = 41,
};
}
namespace SKILL {
enum SKILLS : unsigned int {
    // uint64_t
    CANTO = 0x0001,
    SKILLED_RIDER = 0x0002,
    SPRINT = 0x0004,
    SWITCH = 0x0008,
    MOUNTAINWALK = 0x0010,
    WATERWALK = 0x0020,
    CRIT_KILLS = 0x0040,
    DISMEMBER = 0x0020,
    ATK_RANGE_P1 = 0x0100,
    DIVINE_SHIELD = 0x0100,
    NO_CRIT = 0x0400,
    NO_COUNTER = 0x0800,
    MAX_DESPAIR = 0x1000,
    TUNNELING = 0x2000,
    SCOUTING = 0x4000,
    ASSASSINATE = 0x8000,
    LOCKPICK = 0x00010000,
    NO_LOCKPICK = 0x00020000, //Unlocks withouth lockpicks.
    IMMUNE_MAGIC = 0x00040000,
    IMMUNE_ELEMENTAL = 0x00080000,
    IMMUNE_DEMONIC = 0x00100000,
    IMMUNE_ANGELIC = 0x00200000,
    LIFESTEAL_RN = 0x00400000, //sol
    INFUSE = 0x00800000,
    DOUBLE_EXP = 0x01000000,
    AMBIDEXTRY = 0x02000000, // *DESIGN QUESTION* should this be a skill?
    TWO_HAND_STYLE = 0x04000000, //Weight of weapons are halved if two hands.
    PIERCE_RN = 0x08000000, //luna
    // Boring skills
    COUNTER = 0x10000000,
    THRUST_SWORD_BONUS = 0x20000000,
    SHIELD_BONUS = 0x40000000,
    OFFHAND_BONUS = 0x80000000,
    // -> Should be long 32bit int.
};
const unsigned long long int BOW_BONUS = 0x100000000;
}
namespace MVT {
enum MVT {
    FOOT_SLOW = 1,
    FOOT_FAST,
    MAGES,
    RIDERS_SLOW,
    RIDERS_FAST,
    FLIERS,
    ARMORS,
    PIRATES,
    BANDITS,
    END,
};
}
namespace SEX {
enum SEXES {
    F = false,
    M = true,
};
}
}

namespace MAP {
namespace OVERLAY {
enum OVERLAYS {
    HEAL = 0x0001,
    MOVE = 0x0002,
    ATTACK = 0x0004,
};
}
namespace DANGER {
enum DANGERS {

};
}
}

namespace LOOPING {
enum LOOPINGS {
    PINGPONG = 1,
    DIRECT,
    LINEAR,
    REVERSE,
};
}

namespace TILE {
enum TILES {
    // Basic tile index is 3 digits -> 100
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

namespace GAME {
namespace BUTTON {
enum BUTTONS {
    RIGHT = 1,
    LEFT,
    UP,
    DOWN,
    ACCEPT,
    CANCEL,
    MAP,
    PAUSE,
    STATS,
    OPTIONS,
    ACCELERATE,
};
}

namespace RN {
enum RNS {
    SINGLE = 1,
    DOUBLE,
    HYBRID,
    GAUSSIAN,
};
}

namespace SCENE {
enum SCENES {
    CHAPTER1INTRO = 1,
    SUPPORT_ERWINKIARA_C,
    SUPPORT_ERWINKIARA_B,
    SUPPORT_ERWINKIARA_A,
};
}

namespace STATE {
enum STATES {
    PAUSE = 1,
    STATS,
    MINIMAP,
    MAP,
    MAPMENU,
    UNITMENU,
    UNITMOVE,
    CUTSCENE,
    NEXTTURN,
    OPTIONS,
    STARTMENU,
    EQUIPMENT,
    SOUNDROOM,
    SAVES,
    STORAGE,
    FORGE,
    BONUSEXP,
    PLACEMENT,
    PREPARATION,
    CHOOSEUNITS,
    ITEMS,
    TRADE,
    CONVERSATION,
    BATTLE,
    ATTACK,
    UNITHOVER,
    TURNEND,
    TURNBEGIN,
    NPCTURN,
    INTROVIDEO,
    END,
};
}
}
namespace CHAPTER {
enum CHAPTERS {
    TEST = 0,
    CHAP1,
    END,
};
}

#endif /* ENUMS_HPP */