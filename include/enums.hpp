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
    SECONDCURSOR,
    REPLACESCURSOR,
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
    KITCHEN_KNIFE,
    WRATH_SWORD,
    FLEURET,
    RAPIERE,
    GLAIVE,
    SABER,
    FAUCHON,
    IRON_SWORD,
    STEEL_SWORD,
    DAMAS_SWORD,
    MERCIFUL_BLADE,
    BROADSWORD,
    ESPADON,
    OODACHI,
    UCHIGATANA,
    EXSANGUE,
    HAUTECLAIRE,
    SECUNDUS,
    EXCALIBUR,
    GALATINE,
    RAIJINTOU,
    FUUJINTOU,
    HONJOU_MASAMUNE,
    RAW_IRON_SLAB,
    KIRITO,
    KILLY,

    PITCHFORK,
    FIGHTING_STICK,
    IRON_LANCE,
    WRATH_LANCE,
    STEEL_LANCE,
    DAMAS_LANCE,
    HEAVY_SPEAR,
    TRIDENT,
    MERCIFUL_LANCE,
    JAVELIN,
    PILUM,
    SPEAR,
    VEL,
    GUNGNIR,
    ACHILLES,
    TONBOKIRI,

    IRON_AXE,
    STEEL_AXE,
    DAMAS_AXE,
    WRATH_AXE,
    THROWING_AXE,
    TOMAHAWK,
    PICK_AXE,
    HOE,
    CLUB,
    HALBERD,
    HAMMER,
    WAR_PICK,
    BATTLEAXE,
    VIDYU,
    HEPHAESTUS,

    ADAPTED_CROSSBOW,
    REPEATABLE_CROSSBOW,
    CROSSBOW,
    SHORT_BOW,
    LONG_BOW,
    COMPOSITE_BOW,
    RECURVE_CROSSBOW,
    RECURVE_BOW,
    APOLLO,
    ARTEMIS,
    PINAKA,

    ADAPTED_SHIELD,
    POT_LID,
    WOODEN_SHIELD,
    CLOAK,
    LEATHER_SHIELD,
    KITE_SHIELD,
    IRON_BUCKLER,
    STEEL_BUCKLER,
    SPIKED_BUCKLER,
    SCUTUM,
    MASTER_SHIELD,
    GODDESS_HAND,
    ANCILE,
    BOULDER,
    OBSIDIAN_SLAB,
    MARBLE_PLATE_SHIELD,
    DIVINE_PROTECTOR,

    WRISTBLADE,
    SHIELD_HOOK,
    RETRACTABLE_WRISTBLADE,
    MAIN_GAUCHE,
    SWORDBREAKER,
    TRIDENT_DAGGER,
    TANTOU,
    KODACHI,
    DAGUE,
    WAKIZASHI,
    BRAQUEMARD,
    MISERICORDE,
    CARNWENNAN,
    FLINTLOCK,
    CIBO,

    SHRUNKEN_HEAD,
    CRYSTAL,
    GLASS_ORB,
    HUMAN_FEMUR,
    VOID_FRAGMENT,
    DEMON_FANG,
    ANGEL_FEATHER,
    ANGEL_HAIR,
    VIAL_OF_LIGHT,
    MAIDENS_HANDKERCHIEF,
    WAR_HORN,

    BALL_LIGHTNING,
    ICICLE,
    VOLCANO,
    WIND_SPEAR,
    EMBER,
    TSUNAMI,
    GOD_FORGE_FIRE,
    FINGOLFIN,
    MITHRANDIR,

    VOIDD,
    NETHER,
    DOWNFALL,
    CONSUME,
    PUTRIFY,
    GLAURUNG,
    MORGOTH,
    GOTHMOG,
    FATALIS,

    HEAL,
    REMEDY,
    RENEW,
    WISH,
    PRAY,

    SALVE, // heals for 10.
    PANACEA,
    HEALING_POTION,
    TELPERION_EXTRACT,
    LAURELIN_EXTRACT,
    CROSS, // Little healing everyturn if in inventory? you or in range?

    BOOKBASICS,
    BOOKWEAPONS,
    BOOKELEMENTAL,
    BOOKMATHEMAGICS,
    BOOKLOVE,
    BOOKGROWTHS,
    BOOKPOSSESSION,

    END,
};
}
namespace TYPE {
enum TYPES {
    // unsigned!!! short int equippable = sword + shield;
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
    // short unsigned int 16 bits->65536
    // unsigned int 16?32 bits->4,294,967,295
    // long unsigned int 64 bits
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
    // short unsigned int 16 bits->65536
    // unsigned int 16?32 bits->4,294,967,295
    // long unsigned int 64 bits
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
    CHAPTER1INTRO = 1;
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