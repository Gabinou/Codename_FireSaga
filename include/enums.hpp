#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace DEFAULT {
enum DEFAULTS {
    TILESIZE = 32,
    EQUIPMENT_SIZE = 7,
    WEAPONS_SIZE = 3,
    ITEMS_SIZE = 3,
    CONVOY_SIZE = 200,
    LINE_LENGTH = 255,
    TILE_DIVISOR = 10,
    MAXPATH = 90,
    CHAP_NUMBER = 40,
    BUFFER_SIZE = 128,
    SUPPORTS_MAX = 7,
};
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
    ROCK_PLATE,
    OBSIDIAN_PLATE,
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
    PMIGHT,
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
    // -> Should be 32bit int.
};
}
}

namespace UNIT {
namespace CLASS {
enum CLASSES {
    LORD = 1,
    LORD_RIDER,
    DUKE,
    DUKE_RIDER,
    ARCHER,
    ARCHER_RIDER,
    MARKSMAN,
    MARKSMAN_RIDER,
    ANGEL,
    DEMON,
    PEGASUS_KNIGHT,
    DEMONIC_INCARNATE,
    ANGELIC_INCARNATE,
    FENCER,
    DUELIST,
    MOUSQUETAIRE,
    PICKPOCKET,
    THIEF,
    ASSASSIN,
    MERCENARY,
    HERO,
    CORSAIR,
    VIKING,
    BANDIT,
    RAVAGER,
    PIKEMAN,
    CAVALIER,
    PALADIN,
    MAGE,
    BATTLEMAGE,
    TROUBADOUR,
    SAGE,
    PRIEST,
    BISHOP,
    ORACLE,
    GENERAL,
    CLERIC,
    DEMIGOD,
    GOD,
    KNIGHT,
    GODDESS,
    TWINBORN,
    END
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
namespace NAME {
enum NAMES {
    ERWIN = 1,
    KIARA,
    SILOU,
    SERVIL,
    PERIGNON,
    POET,
    RELIABLE,
    COWARD,
    JAIGEN1H,
    HOTTIE,
    PC_END,

    ZINEDAN,
    ZIDINE, // Zinedine Zidan
    NPC_END,

    BANDIT,
    DUELIST,
    PICKPOCKET,
    THIEF,
    ASSASSIN,
    ARCHER,
    MARKSMAN,
    MERCENARY,
    HERO,
    CORSAIR,
    VIKING,
    RAVAGER,
    CAVALIER,
    PALADIN,
    FENCER,
    MOUSQUETAIRE,
    GENERIC_END,
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
    PINGPONG,
    DIRECT,
    REVERSE
};
}

namespace TILE {
enum TILES {
    // Basic tile index is 3 digits -> 100
    // two first digits give the tile.
    // third digit gives the asset index.
    // Ex: 324: Peak tile, 4th asset of peak.
    // OCTAL NUMBERS BECAUSE WHY NOT?
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

namespace STATE {
enum STATES {
    PAUSE = 0,
    STATS = 1,
    MINIMAP = 2,
    MAP = 3,
    UNITMENU = 4,
    UNITMOVE = 5,
    CUTSCENE = 6,
    OPTIONS = 7,
    STARTMENU = 8,
    EQUIPMENT = 9,
    SOUNDROOM = 10,
    SAVES = 11,
    STORAGE = 12,
    FORGE = 13,
    BONUSEXP = 14,
    PLACEMENT = 15,
    PREPARATION = 16,
    CHOOSEUNITS = 17,
    ITEMS = 18,
    TRADE = 19,
    CONVERSATION = 20,
    BATTLE = 21,
    ATTACK = 22,
    INTROVIDEO = 23,
    END = 24,
};
}
}

#endif /* ENUMS_HPP */