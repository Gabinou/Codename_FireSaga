// Class, equippable, Utilities_mvtType, promotion bonus skill
// these are defaults
// Promoted classes are NOT divisible by 2?
REGISTER_ENUM(VILLAGER,     1, FOOT_SLOW,     START,          ITEM_TYPE_LANCE)

REGISTER_ENUM(LORD,         2,  FOOT_SLOW,    SKILLED_RIDER,  ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)
REGISTER_ENUM(DUKE,         3,  FOOT_SLOW,    SKILLED_RIDER,
              ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND)

REGISTER_ENUM(ARCHER,       4,  FOOT_SLOW,    SKILLED_RIDER,  ITEM_TYPE_BOW)
REGISTER_ENUM(MARKSMAN,     5,  FOOT_SLOW,    SKILLED_RIDER,
              ITEM_TYPE_SWORD + ITEM_TYPE_OFFHAND + ITEM_TYPE_BOW)

REGISTER_ENUM(SQUIRE,       6,  ARMORS,       SKILLED_RIDER,  ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)
REGISTER_ENUM(KNIGHT,       7,  ARMORS,       SKILLED_RIDER,  ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)

REGISTER_ENUM(FENCER,       8,  FOOT_SLOW,    TRUEHIT,
              ITEM_TYPE_OFFHAND + ITEM_TYPE_SWORD + ITEM_TYPE_SHIELD)
REGISTER_ENUM(DUELIST,      9,  FOOT_SLOW,    TRUEHIT,
              ITEM_TYPE_OFFHAND + ITEM_TYPE_SWORD + ITEM_TYPE_SHIELD)

REGISTER_ENUM(PICKPOCKET,   10, FOOT_FAST,    START,          ITEM_TYPE_OFFHAND + ITEM_TYPE_SWORD)
REGISTER_ENUM(THIEF,        11, FOOT_FAST,    SKILLED_PICKER, ITEM_TYPE_OFFHAND + ITEM_TYPE_SWORD)

REGISTER_ENUM(CORSAIR,      12, BANDITS,      START,
              ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND + ITEM_TYPE_AXE)
REGISTER_ENUM(VIKING,       13, BANDITS,      WATERWALK,
              ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND + ITEM_TYPE_AXE)

REGISTER_ENUM(BANDIT,       14, BANDITS,      START,          ITEM_TYPE_SHIELD + ITEM_TYPE_AXE)
REGISTER_ENUM(RAVAGER,      15, BANDITS,      TRUEHIT,
              ITEM_TYPE_AXE + ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND + ITEM_TYPE_BOW)

REGISTER_ENUM(PIKEMAN,      16, FOOT_SLOW,    SKILLED_RIDER,  ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)
REGISTER_ENUM(HOPLITE,      17, FOOT_SLOW,    SKILLED_RIDER,  ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)
REGISTER_ENUM(STANDARD_BEARER,  18, FOOT_SLOW,  START,
              ITEM_TYPE_STANDARD + ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND)

REGISTER_ENUM(DANCER,       19, FOOT_SLOW,    DANCE,          ITEM_TYPE_NULL)

REGISTER_ENUM(MAGE,         20, MAGES,        SKILLED_MAGE,
              ITEM_TYPE_ELEMENTAL + ITEM_TYPE_SHIELD + ITEM_TYPE_LANCE)
REGISTER_ENUM(BATTLEMAGE,   21, MAGES,        SKILLED_MAGE,
              ITEM_TYPE_ELEMENTAL + ITEM_TYPE_SHIELD + ITEM_TYPE_SWORD + ITEM_TYPE_TRINKET)

REGISTER_ENUM(PRIEST,       22, MAGES,        SKILLED_CLERIC, ITEM_TYPE_STAFF)
REGISTER_ENUM(BISHOP,       23, MAGES,        SKILLED_CLERIC,
              ITEM_TYPE_STAFF + ITEM_TYPE_ANGELIC + ITEM_TYPE_TRINKET)

REGISTER_ENUM(CLERIC,       24, MAGES,        SKILLED_CLERIC, ITEM_TYPE_STAFF + ITEM_TYPE_OFFHAND)
REGISTER_ENUM(ORACLE,       25, MAGES,        SKILLED_CLERIC, ITEM_TYPE_STAFF)

REGISTER_ENUM(DEMONIC_INCARNATE, 26, MAGES,     ROCK_SHIELD,  ITEM_TYPE_DEMONIC + ITEM_TYPE_TRINKET)
REGISTER_ENUM(DEMON,             27, FOOT_FAST, ROCK_SHIELD,
              ITEM_TYPE_DEMONIC + ITEM_TYPE_CLAW + ITEM_TYPE_AXE + ITEM_TYPE_SHIELD)
REGISTER_ENUM(ANGELIC_INCARNATE, 28, MAGES,     FLIGHT,       ITEM_TYPE_ANGELIC + ITEM_TYPE_TRINKET)
REGISTER_ENUM(ANGEL,             29, FLIERS,    FLIGHT,
              ITEM_TYPE_ANGELIC + ITEM_TYPE_CLAW + ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)

REGISTER_ENUM(KEWAC_RUNNER,     30, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(KEWAC_HUNTER,     32, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(KEWAC_WARRIOR,    33, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(WOLFSPEAKER,      34, MAGES,      START,        ITEM_TYPE_SHIELD)

REGISTER_ENUM(KESIRU_RIDER,     35, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(KESIRU_WARRIOR,   36, MAGES,      START,        ITEM_TYPE_SHIELD)

REGISTER_ENUM(DEMIGOD,          40, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(GOD,              41, MAGES,      START,        ITEM_TYPE_SHIELD)
REGISTER_ENUM(GODDESS,          42, MAGES,      START,        ITEM_TYPE_SWORD)
REGISTER_ENUM(TWINBORN,         43, MAGES,      START,
              ITEM_TYPE_ANGELIC + ITEM_TYPE_DEMONIC + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD)

REGISTER_ENUM(VESTAL,           50, MAGES,      SKILLED_VESTAL, ITEM_TYPE_STAFF + ITEM_TYPE_OFFHAND)
REGISTER_ENUM(WOLF,      51, FOOT_FAST,      START,             ITEM_TYPE_CLAW)