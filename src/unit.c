
#include "unit.h"

int  class_mvt_types[UNIT_CLASS_END] = {
    /* NULL              */ UNIT_MVT_FOOT_SLOW,
    /* VILLAGER          */ UNIT_MVT_FOOT_SLOW,
    /* LORD              */ UNIT_MVT_FOOT_SLOW,
    /* DUKE              */ UNIT_MVT_FOOT_SLOW,
    /* ARCHER            */ UNIT_MVT_FOOT_SLOW,
    /* MARKSMAN          */ UNIT_MVT_FOOT_SLOW,
    /* SQUIRE            */ UNIT_MVT_ARMORS,
    /* KNIGHT            */ UNIT_MVT_ARMORS,
    /* FENCER            */ UNIT_MVT_FOOT_SLOW,
    /* DUELIST           */ UNIT_MVT_FOOT_SLOW,
    /* PICKPOCKET        */ UNIT_MVT_FOOT_FAST,
    /* THIEF             */ UNIT_MVT_FOOT_FAST,
    /* CORSAIR           */ UNIT_MVT_BANDITS,
    /* VIKING            */ UNIT_MVT_BANDITS,
    /* BANDIT            */ UNIT_MVT_BANDITS,
    /* RAVAGER           */ UNIT_MVT_BANDITS,
    /* PIKEMAN           */ UNIT_MVT_FOOT_SLOW,
    /* HOPLITE           */ UNIT_MVT_FOOT_SLOW,
    /* DANCER            */ UNIT_MVT_FOOT_SLOW,
    /* NULL              */ UNIT_MVT_FOOT_SLOW,
    /* MAGE              */ UNIT_MVT_MAGES,
    /* BATTLEMAGE        */ UNIT_MVT_MAGES,
    /* PRIEST            */ UNIT_MVT_MAGES,
    /* BISHOP            */ UNIT_MVT_MAGES,
    /* CLERIC            */ UNIT_MVT_MAGES,
    /* ORACLE            */ UNIT_MVT_MAGES,
    /* DEMONIC_INCARNATE */ UNIT_MVT_MAGES,
    /* DEMON             */ UNIT_MVT_FOOT_FAST,
    /* ANGELIC_INCARNATE */ UNIT_MVT_MAGES,
    /* ANGEL             */ UNIT_MVT_FLIERS,
    /* KEWAC_RUNNER      */ UNIT_MVT_FOOT_SLOW,
    /* NULL              */ UNIT_MVT_FOOT_SLOW,
    /* KEWAC_HUNTER      */ UNIT_MVT_MAGES,
    /* KEWAC_WARRIOR     */ UNIT_MVT_MAGES,
    /* WOLFSPEAKER       */ UNIT_MVT_MAGES,
    /* KESIRU_RIDER      */ UNIT_MVT_MAGES,
    /* KESIRU_WARRIOR    */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* NULL              */ UNIT_MVT_MAGES,
    /* DEMIGOD           */ UNIT_MVT_MAGES,
    /* GOD               */ UNIT_MVT_MAGES,
    /* GODDESS           */ UNIT_MVT_MAGES,
    /* TWINBORN          */ UNIT_MVT_MAGES,
};

/* --- UNIT --- */
struct Unit_status Unit_status_default = {.status = 0, .turns = 3};

struct Unit_stats Unit_stats_default = {
    .hp =    0,
    .str =   0,
    .mag =   0,
    .dex =   0,
    .agi =   0,
    .fth =   0,
    .luck =  0,
    .def =   0,
    .res =   0,
    .move =  0,
    .con =   0,
    .prof =  0
};

struct Unit Unit_default = {
    .json_element = JSON_UNIT,
    /*                  hp str mag agi dex fth luck def res con move prof */
    .base_stats      = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    .bonus_stats     = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    .caps_stats      = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    .malus_stats     = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    .current_stats   = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    .growths         = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},

    .hit_sequence    = {0, 0},
    .crit_sequence   = {0, 0},

    .agony    = -1,
    .regrets  =  0,
    .rescuee  = UNIT_NULL,

    .hp_sequence     = {0, 0},
    .str_sequence    = {0, 0},
    .mag_sequence    = {0, 0},
    .dex_sequence    = {0, 0},
    .agi_sequence    = {0, 0},
    .fth_sequence    = {0, 0},
    .luck_sequence   = {0, 0},
    .def_sequence    = {0, 0},
    .res_sequence    = {0, 0},
    .con_sequence    = {0, 0},
    .move_sequence   = {0, 0},
    .prof_sequence   = {0, 0},

    .rangemap        = RANGEMAP_ATTACKMAP,
    .user_rangemap   = RANGEMAP_NULL,

    .support_bonuses =  {{{0}}},
    .support_bonus   =   {{0}},

    .dft_pos   =   {-1, -1},

    .handedness      = UNIT_HAND_RIGHTIE,
    .skills          = 0,
    .skill_names     = NULL,
    .exp             = 0,
    .base_exp        = 0,
    .army            = 1,
    ._id             = 0,
    .status_queue    = NULL,
    .grown_stats     = NULL,

    .weapons_dtab    = NULL,

    .current_hp      = 0,
    .is_alive        = true,
    .talkable        = 0,

    .sex             = false, // 0:F, 1:M. eg. hasPenis.
    .literate        = false, // Reading/writing for scribe job.
    .show_danger     = false,

    .hands           = {true, true},
    .equipped        = {false, false},
    .update_stats    = true,
    .computed_stats  = {{0, 0}, {0, 0}, 0, 0, 0, 0, 0, 0, 0, {-1, -1}},

    .temp            = {0},
    .num_equipment   =  0,
    .eq_usable       = {0},
    .num_usable      =  0,
};

struct Unit Unit_Nibal_make() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit Nibal_unit = {
        .json_element = JSON_UNIT,
        /*                  hp str mag agi dex fth luck def res con move prof */
        .base_stats       = {35, 20, 20, 18, 25, 14, 12, 18, 22, 30, 06, 15},
        .bonus_stats      = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        .caps_stats       = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        .malus_stats      = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        .current_stats    = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        .growths          = {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        .agony            = -1,

        .skills         = UNIT_SKILL_VENOMOUS_SPIT,
        .exp            = 0,
        .base_exp       = 2500,
        .army           = ARMY_HAMILCAR,
        ._id            = UNIT_ID_NIBAL,
        .status_queue   = NULL,

        .grown_stats    = NULL,
        .weapons_dtab   = NULL,

        .current_hp     = 35,
        .is_alive       = true,
        .talkable       = 0,

        .sex            = true,  /* 0:F, 1:M. eg. hasPenis. */
        .literate       = false, /* Reading/writing for scribe job. */
        .show_danger    = false,

        .computed_stats = {{0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, 0, 0, 0, {-1, -1}},
        .equipped       = {-1, -1},
        .hands          = {true, true},

        .mount          = NULL,

        .num_equipment  = 0,
    };
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (Nibal_unit);
}

/* --- Constructors/Destructors --- */
void Unit_Init(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    Unit_Free(unit);
    unit->grown_stats  = DARR_INIT(unit->grown_stats,  struct Unit_stats, SOTA_MAX_LEVEL / 8);
    unit->status_queue = DARR_INIT(unit->status_queue, struct Unit_status, 2);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Free(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    if (unit->grown_stats != NULL) {
        DARR_FREE(unit->grown_stats);
        unit->grown_stats = NULL;
    }
    if (unit->skill_names != NULL) {
        DARR_FREE(unit->skill_names);
        unit->skill_names = NULL;
    }
    if (unit->status_queue != NULL) {
        DARR_FREE(unit->status_queue);
        unit->status_queue = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_InitWweapons(struct Unit *unit, struct dtab *weapons_dtab) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Unit_Init(unit);
    unit->weapons_dtab = weapons_dtab;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Setters/Getters --- */
struct WpnorItem Unit_WpnorItem(struct Unit *unit, int i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit->weapons_dtab != NULL);
    SDL_assert(unit->items_dtab   != NULL);

    struct Inventory_item *inv_item = Unit_Item_Side(unit, i);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (Utilities_WpnorItem(inv_item->id, unit->weapons_dtab, unit->items_dtab));
}


/* Unit_Item_Strong works both ways:
    - Get id in strong space from side space [0, eq_size]
    - Get weapon in correct hand side from stronghand id
*/
int Unit_Id_Strong(struct Unit *unit, int side_i) {
    return (side_i < UNIT_HANDS_NUM ? Unit_Hand_Strong2Side(unit, side_i) : side_i);
}

/* Unit_Item_Strong only gives the weapon in input hand.*/
struct Inventory_item *Unit_Item_Side(struct Unit *unit, int side_i) {
    return (&unit->_equipment[side_i]);
}

/* Unit_Item_Strong works both ways:
    - Get weapon in stronghand from indices [0, eq_size]
    - Get weapon in correct hand side from stronghand
*/
struct Inventory_item *Unit_Item_Strong(struct Unit *unit, int strong_i) {
    return (&unit->_equipment[Unit_Id_Strong(unit, strong_i)]);
}

int Unit_Hand_Strong2Side(const struct Unit *unit, int strong_i) {
    /*  strong in out   strong    in    out
    *      0   0   0     left   strong  left    --> XOR
    *      0   1   1     left    weak   right   --> XOR
    *      1   0   1     right  strong  right   --> XOR
    *      1   1   0     right   weak   left    --> XOR
    */
    return (Unit_Hand_Strong(unit) ^ strong_i);
}

int Unit_Hand_Side2Strong(const struct Unit *unit, int side_i) {
    /*  strong in out   strong    in    out
    *      0   0   0     left    left  strong   --> XOR
    *      0   1   1     left    right  weak    --> XOR
    *      1   0   1     right   left   weak    --> XOR
    *      1   1   0     right   right strong   --> XOR
    */
    return (Unit_Hand_Strong(unit) ^ side_i);
}

/* Note: weakhand = 1 - stronghand */
int Unit_Hand_Strong(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    SDL_assert(unit != NULL);
    return (SotA_Hand_Strong(unit->handedness));
}

int SotA_Hand_Strong(if8 handedness) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if ((handedness <= UNIT_HAND_NULL) || (handedness >= UNIT_HAND_END)) {
        SOTA_Log_Debug("Wrong handedness");
        exit(ERROR_Generic);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    /* Stronghand is left hand only for left-handed.
    *  Stronghand is right hand for for right-handed AND ambidextrous. */
    return ( (handedness == UNIT_HAND_LEFTIE) ? UNIT_HAND_LEFT : UNIT_HAND_RIGHT);
}


void Unit_setid(struct Unit *unit, if16 id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    unit->_id = id;
    strcpy(unit->name, global_unitNames[unit->_id]);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_setSkills(struct Unit *unit, u64 skills) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->skills = skills;
    if (unit->skill_names != NULL)
        DARR_FREE(unit->skill_names);
    unit->skill_names = Names_skillNames(unit->skills);
    SOTA_Log_Debug("Unit new skills: %lx \n", unit->skills);
    for (u8 i = 0; DARR_LEN(unit->skill_names); i++)
        SOTA_Log_Debug("Skill name: %s", unit->skill_names[i]);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_setClassind(struct Unit *unit, if8 class_index) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert((class_index > 0) && (class_index < UNIT_CLASS_END));
    unit->class      = class_index;
    unit->mvt_type   = Unit_mvtType(unit);
    unit->equippable = class_equippables[unit->class];

    bool healclass  = (unit->class == UNIT_CLASS_BISHOP);
    healclass      |= (unit->class == UNIT_CLASS_CLERIC);

    Unit_Rangemap_Default(unit);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_setStats(struct Unit *unit, struct Unit_stats stats) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->current_hp    = unit->current_stats.hp;
    unit->current_stats = stats;
    Unit_effectiveStats(unit);
    Unit_computedStats(unit, -1);
    unit->current_hp = unit->effective_stats.hp;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_setBases(struct Unit *unit, struct Unit_stats stats) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->base_stats = stats;
    unit->current_hp = unit->base_stats.hp;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Unit_stats Unit_getStats(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = unit->current_stats;
    SDL_assert(unit);
    Unit_effectiveStats(unit);
    Unit_stats_plus(out_stats,  unit->bonus_stats);
    Unit_stats_minus(out_stats, unit->malus_stats);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}

/* --- Second-order info --- */
u8 Unit_mvtType(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (class_mvt_types[unit->class]);
}

u8 SotA_army2alignment(u8 army) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if ((army <= ARMY_START) || (army >= ARMY_END)) {
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
        SOTA_Log_Debug("Army out of bounds");
        exit(ERROR_OutofBounds);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (army_alignment[army]);
}

bool SotA_isPC(u8 army) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if ((army <= ARMY_START) || (army >= ARMY_END)) {
        SOTA_Log_Debug("Army out of bounds");
        exit(ERROR_OutofBounds);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (army_isPC[army]);
}

/* --- Usability --- */
void Unit_Find_Usable(struct Unit *unit, int archetype) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Find usable weapons in eq_space --  */
    unit->num_usable = 0;
    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (Unit_Eq_Usable(unit, archetype, i))
            unit->eq_usable[unit->num_usable++] = i;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Unit_All_Usable(struct Unit *unit) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Set all weapons to be usable --  */
    /* Use case: Dropping item  */
    unit->num_usable = 0;
    for (int i = 0; i < unit->num_equipment; i++) {
        unit->eq_usable[unit->num_usable++] = i;
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


bool Unit_Eq_Usable(const struct Unit *unit, int archetype, int i) {
    SDL_assert(i >= 0);
    SDL_assert(i < DEFAULT_EQUIPMENT_SIZE);
    return (Unit_Item_Usable(unit, archetype, unit->_equipment[i].id));
}

bool Unit_Item_Usable(const struct Unit *unit, int archetype, int id) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool usable = false;
    do {
        /* -- If item select, everything is usable --  */
        if (archetype == ITEM_ARCHETYPE_ITEM) {
            usable = true;
            break;
        }

        bool iswpn    = Item_isWeapon(id);
        bool isstaff  = Item_isStaff(id);
        bool isshield = Item_isShield(id);
        bool canequip = Unit_canEquip(unit, id);
        SDL_assert(!(isstaff && iswpn));

        if (archetype == ITEM_ARCHETYPE_WEAPON) {
            usable = (iswpn && !isstaff && canequip);
        } else if (archetype == ITEM_ARCHETYPE_STAFF) {
            usable = isstaff;
        } else if (archetype == ITEM_ARCHETYPE_SHIELD) {
            usable = isshield;
        }

    } while (false);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (usable);
}

/* --- Skills --- */
bool Unit_hasSkill(const struct Unit *unit, u64 skill) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return ((unit->skills & skill) > 0);
}

/* --- Statuses --- */
void Unit_Status_Add(struct Unit *unit, struct Unit_status status) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if16 i = Unit_Status_Find(unit, status.status);
    if (unit->status_queue == NULL)
        unit->status_queue = DARR_INIT(unit->status_queue, struct Unit_status, 2);

    if (i < 0)
        DARR_PUT(unit->status_queue, status);
    else {
        unit->status_queue[i].turns = STATUS_DEFAULT_TURNS;
        i = Unit_Status_Find_Turns(unit, unit->status_queue[i].turns);
        DARR_INSERT(unit->status_queue, status, i);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Status_Decrement(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (unit->status_queue == NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        unit->status_queue[i].turns--;
        if (unit->status_queue[i].turns <= 0)
            DARR_DEL(unit->status_queue, i);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

if16 Unit_Status_Find(struct Unit *unit, if16 status) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit->status_queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        if (unit->status_queue[i].status == status) {
            SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
            return (i);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (-1);
}

// What is that for?
if16 Unit_Status_Find_Turns(struct Unit *unit, if16 turns) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if16 out = -1;
    SDL_assert(unit->status_queue != NULL);
    for (size_t i = 0; i < DARR_NUM(unit->status_queue); i++) {
        if (unit->status_queue[i].turns == turns) {
            out = i;
            break;
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* --- Items --- */
/* Private item atker at specific spot. Does no checks
 */
void _Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, size_t i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    unit->_equipment[i] = item;
}

/* Take item at specific spot
    - During gameplay, errors if taken at non-empty location
 */
void Unit_Item_Takeat(struct Unit *unit, struct Inventory_item item, size_t i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab != NULL);
    if (item.id == ITEM_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    Weapon_Load(unit->weapons_dtab, item.id);

    if ((i < 0) || (i >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Item i out of bounds");
        exit(ERROR_OutofBounds);
    }
    if (unit->_equipment[i].id != ITEM_NULL) {
        SOTA_Log_Debug("Item taken at non-empty spot");
        exit(ERROR_OutofBounds);
    }
    _Unit_Item_Takeat(unit, item, i);
    unit->num_equipment++;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Item_Take(struct Unit *unit, struct Inventory_item item) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    if (unit->num_equipment >= DEFAULT_EQUIPMENT_SIZE) {
        SOTA_Log_Debug("Unit Inventory full, should not be able to take item");
        exit(ERROR_OutofBounds);
    }

    if (item.id == ITEM_NULL) {
        SOTA_Log_Debug("Unit should not be able to take NULL item");
        exit(ERROR_OutofBounds);
    }

    for (size_t i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (unit->_equipment[i].id == ITEM_NULL) {
            Unit_Item_Takeat(unit, item, i);
            break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Inventory_item Unit_Item_Drop(struct Unit *unit, if16 i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if ((i < 0) || (i >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item out = unit->_equipment[i];
    unit->_equipment[i]        = Inventory_item_default;
    unit->num_equipment--;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void Unit_Item_Swap(struct Unit *unit, if16 i1, if16 i2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    if ((i1 < 0) || (i1 >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Item index1 out of bounds");
        exit(ERROR_OutofBounds);
    }
    if ((i2 < 0) || (i2 >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Item index2 out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer = unit->_equipment[i1];
    unit->_equipment[i1]          = unit->_equipment[i2];
    unit->_equipment[i2]          = buffer;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Item_Trade(struct Unit *giver, struct Unit *taker, if16 ig, if16 it) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(giver);
    SDL_assert(taker);
    if ((it < 0) || (it >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Taker Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    if ((ig < 0) || (ig >= DEFAULT_EQUIPMENT_SIZE)) {
        SOTA_Log_Debug("Giver Item index out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer_giver = Unit_Item_Drop(giver, ig);
    struct Inventory_item buffer_taker = Unit_Item_Drop(taker, it);
    Unit_Item_Takeat(taker, buffer_giver, it);
    Unit_Item_Takeat(giver, buffer_taker, ig);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Inventory_item Unit_Equip_TwoHanding(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    /* Weapon is in stronghand, equipping to weakhand */
    int stronghand = Unit_Hand_Strong(unit);
    bool weakhand  = 1 - stronghand;
    if (!unit->hands[weakhand] || !unit->hands[stronghand]) {
        SOTA_Log_Debug("Can't equip a weapon in two hands without two hands");
        exit(ERROR_Generic);
    }

    /* -- Checks -- */
    SDL_assert(unit->_equipment[stronghand].id > ITEM_NULL);
    Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);

    /* -- Copy item in hand to other hand -- */
    struct Inventory_item out  = unit->_equipment[weakhand];
    unit->_equipment[weakhand] = unit->_equipment[stronghand];

    /* -- Set flags -- */
    unit->equipped[weakhand]   = true;
    unit->isTwoHanding         = true;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void _Unit_Check_Equipped(struct Unit *unit, bool hand) {
    if (!unit->equipped[hand])
        return;

    if (unit->_equipment[hand].id <= ITEM_NULL)
        return;

    if (unit->_equipment[hand].id != ITEM_ID_BROKEN)
        return;

    /* Broken item in hand, unequipping */
    Unit_Unequip(unit, hand);
}

void Unit_Check_Equipped(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Checks if equipped weapon is BORKED, de-equips if so */
    _Unit_Check_Equipped(unit, UNIT_HAND_LEFT);
    _Unit_Check_Equipped(unit, UNIT_HAND_RIGHT);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Unit_Equip(struct Unit *unit, bool hand, int i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Unit_Item_Swap(unit, hand, i);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (Unit_Equip_inHand(unit, hand));
}

bool Unit_Equip_inHand(struct Unit *unit, bool hand) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("hand %d, equipment id %ld", hand, unit->_equipment[hand].id);
    SDL_assert(unit != NULL);

    /* -- Error if try to equip NULL item -- */
    if (unit->_equipment[hand].id <= ITEM_NULL) {
        SOTA_Log_Debug("No item in hand. Cannot equip.");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (unit->equipped[hand] = false);
    }
    Weapon_Load(unit->weapons_dtab, unit->_equipment[hand].id);

    /* -- Error checking -- */
    if (!Unit_canEquip(unit, unit->_equipment[hand].id)) {
        SOTA_Log_Debug("Cannot equip item.");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (unit->equipped[hand] = false);
    }

    if (!unit->hands[hand]) {
        SOTA_Log_Debug("No hand to equip with.");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (unit->equipped[hand] = false);
    }

    unit->equipped[hand] = true;
    Unit_isdualWielding(unit);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->equipped[hand]);
}

void Unit_Unequip(struct Unit *unit, bool hand) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);

    /* -- Unequip -- */
    unit->equipped[hand] = false;

    /* -- If twohanding, not anymore! -- */
    if (unit->isTwoHanding) {
        unit->_equipment[hand] = Inventory_item_default;
        unit->isTwoHanding    = false;
    }
    /* -- If dual wielding, not anymore! -- */
    unit->isDualWielding = false;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* Makes unit take damage.
* NO DAMAGE COMPUTATION HERE! Crit multiplication responsible by caller.
* Input crit bool just to determine if unit dies instantly or not.
*/
void Unit_takesDamage(struct Unit *unit, u8 damage, bool crit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%s takes %d damage \n", unit->name, damage);
    /* -- Checks -- */
    SDL_assert(unit);
    if (unit->current_hp == 0) {
        SOTA_Log_Debug("Error: Unit HP was already 0 before taking damage.");
        exit(ERROR_Generic);
    }

    /* -- Actually take damage -- */
    unit->current_hp = (damage > unit->current_hp) ? 0 : (unit->current_hp - damage);

    /* -- Check for Death or Agony -- */
    if (unit->current_hp == 0) {
        // TODO: Check for frail skill, other things that kill instantly.
        if (crit)
            Unit_dies(unit);
        else
            Unit_agonizes(unit);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_getsHealed(struct Unit *unit, u8 healing) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%s gets healed for %d\n", unit->name, healing);
    /* -- Checks -- */
    SDL_assert(unit);

    // TODO: Overheal
    /* -- Actually heal -- */
    int missing_hp   = unit->current_stats.hp - unit->current_hp;
    unit->current_hp = healing > missing_hp ? unit->current_stats.hp : unit->current_hp + healing;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_wait(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->waits = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_refresh(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->waits = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

if16 Unit_getLvl(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (ceil(unit->exp / SOTA_100PERCENT) + 1);
}

void Unit_gainExp(struct Unit *unit, uf16 exp) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    if (((unit->exp % SOTA_100PERCENT) + exp) > SOTA_100PERCENT) {
        /* Never should have two level ups at one time.*/
        Unit_lvlUp(unit);
    }
    unit->exp += exp;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_supportUp(struct Unit *unit, if16 id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    int i;
    for (i = 0; i < unit->support_num; i++) {
        if (id == unit->supports[i].other_id) {
            unit->supports[i].level += 1;
            break;
        }
    }

    if (id != unit->supports[i].other_id) {
        SOTA_Log_Debug("Supporting Unit not found");
        exit(ERROR_Generic);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// This function is ugly as sin. TODO: Refactor this. Make more understandable.
void Unit_lvlUp(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    struct Unit_stats grows = Unit_stats_default;
    u8 temp_growth;
    struct Unit_stats temp_stats = {0};
    u8 *growths       = (u8 *)&unit->growths;
    u8 *grows_arr     = (u8 *)&grows;
    u8 *stats_arr     = (u8 *)&temp_stats;
    u8 *caps_stats    = (u8 *)&unit->caps_stats;
    u8 *current_stats = (u8 *)&unit->current_stats;
    struct RNG_Sequence *sequences = (struct RNG_Sequence *)&unit->hp_sequence;

    for (int i = 0; i < UNIT_STAT_NUM; i++) {
        /* Skip if stat is capped */
        if (current_stats[i] >= caps_stats[i])
            continue;

        /* Add one stat if growths > 100 (Works even above 200 growth) */
        stats_arr[i] += growths[i] / SOTA_100PERCENT;

        /* Roll for remainder growth */
        temp_growth   = growths[i] % SOTA_100PERCENT;
        SDL_assert(temp_growth < SOTA_100PERCENT);

        struct RNG_Sequence *sequence;
        if (rng_sequence_breaker_miss_growth) {
            /* Compute growths rise for sequence of missed growths */
            sequence = (sequences + i);
            temp_growth = sequence->eff_rate = SB_GROWTH_RISE(temp_growth, sequence->len);
        }

        grows_arr[i]  = (temp_growth >= global_RNG_URN(tinyMT_global));

        if (rng_sequence_breaker_miss_growth)
            RNG_checkSequence_oneWay(sequence, grows_arr[i]);

        stats_arr[i] += grows_arr[i];
    }

    /* -- Actually adding grown stats to current_stats -- */
    for (int i = 0; i < UNIT_STAT_NUM; i++)
        current_stats[i] += stats_arr[i];

    /* -- Adding current lvlup to all grown stats -- */
    DARR_PUT(unit->grown_stats, temp_stats);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_agonizes(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    unit->agonizes = true;
    SOTA_Log_Debug("%s is agonizing. %d turns until death\n", unit->name, unit->computed_stats.agony);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_dies(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->is_alive = false;
    SOTA_Log_Debug("%s is dead.\n", unit->name);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* Can unit equip weapon input item? */
bool Unit_canEquip(const struct Unit *unit, if16 id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (id <= ITEM_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (false);
    }

    bool type    = Unit_canEquip_Type(unit, id);
    bool left    = Unit_canEquip_Hand(unit, id, UNIT_HAND_LEFT);
    bool right   = Unit_canEquip_Hand(unit, id, UNIT_HAND_RIGHT);
    // SOTA_Log_Debug("Unit_canEquip_Hand %d %d %d", type, right, left);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (type && (left || right));
}

/* Can unit equip weapon currently in hand? */
bool Unit_canEquip_inHand(const struct Unit *unit, bool hand) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit->weapons_dtab != NULL);
    if16 hand_id = unit->_equipment[hand].id;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (Unit_canEquip_Hand(unit, hand_id, hand));
}

/* Can unit equip arbitrary weapon in its hand? */
bool Unit_canEquip_Hand(const struct Unit *unit, if16 id, bool hand) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (id <= ITEM_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (false);
    }
    if (!Weapon_ID_isValid(id)) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (false);
    }

    SDL_assert(unit->weapons_dtab != NULL);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);

    bool eq_any = (weapon->handedness == WEAPON_HAND_ANY);
    // 2O: two-hand weapon in other hand
    bool eq_2O  = (weapon->handedness == WEAPON_HAND_TWO);
    // 1A: one-hand weapon in any hand
    bool eq_1A  = (weapon->handedness == WEAPON_HAND_ONE);
    // TODO: Right hand or left hand

    if (eq_2O || eq_any  || eq_1A) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (true);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (false);
}

/* Can unit equip arbitrary weapon with a certain type? */
bool Unit_canEquip_Type(const struct Unit *unit, if16 id) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Unequippable if ITEM_NULL */
    if (id <= ITEM_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (false);
    }

    if (!Weapon_ID_isValid(id)) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (false);
    }

    SDL_assert(unit->weapons_dtab != NULL);
    Weapon_Load(unit->weapons_dtab, id);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
    uf16 wpntypecode      = weapon->item->type;
    SDL_assert(wpntypecode);

    /* Is unit among weapon's users? */
    if ((weapon->item->users == NULL) || (DARR_NUM(weapon->item->users) == 0)) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return ((unit->equippable & wpntypecode) > 0);
    }

    /* Is weapon's type equippable by unit? */
    bool found = false;
    for (u8 i = 0; i < DARR_NUM(weapon->item->users); i++) {
        found = (weapon->item->users[i] == unit->_id);
        if (found) {
            SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
            return ((unit->equippable & wpntypecode) > 0);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (false);
}

/* Is a rightie using a weapon in its left hand? */
bool Unit_iswrongHanding(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool out = false;
    do {
        struct Inventory_item *item = Unit_Item_Strong(unit, UNIT_HAND_WEAK);
        if ((item->id == ITEM_NULL) || (item->id == ITEM_ID_BROKEN))
            break;

        int archetype = Item_Archetype(item->id);
        /* Offhands count as weapon archetype, so need to check like this for wronghanding*/
        if ((archetype == ITEM_ARCHETYPE_SHIELD) || Item_isOffhand(item->id))
            break;
        out = true;
    } while (false);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* Is a unit wielding a weapon in its hand? Note: Units can equip staves.
    -> Equipped + a weapon (not a staff, or offhand, or trinket...)
 */
bool Unit_isWielding(struct Unit *unit, bool hand) {
    if (!unit->equipped[hand])
        return (false);

    uf16 id = unit->_equipment[hand].id;
    if (id <= ITEM_NULL)
        return (false);

    int archetype = Item_Archetype(id);
    if (archetype != ITEM_ARCHETYPE_WEAPON)
        return (false);

    return (true);
}

/* If a unit dual wielding? i.e. wielding a weapon in both hands */
bool Unit_isdualWielding(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);

    // Unit_Equipment_Print(unit);

    bool left   = Unit_isWielding(unit, UNIT_HAND_LEFT);
    bool right  = Unit_isWielding(unit, UNIT_HAND_RIGHT);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->isDualWielding = left && right && !unit->isTwoHanding);
}

bool Unit_canCarry(struct Unit *savior, struct Unit *victim) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats victim_stats = Unit_effectiveStats(victim);
    struct Unit_stats savior_stats = Unit_effectiveStats(savior);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (Equation_canCarry(savior_stats.con, victim_stats.con));
}

bool Unit_canDance(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool out = (unit->class == UNIT_CLASS_DANCER);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* - Unit has any usable staff in Equipment? - */
int Unit_canStaff_Eq(const struct  Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        struct Inventory_item item = unit->_equipment[i];
        if (Weapon_isStaff(item.id)) {
            SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
            return (true);
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (false);
}

/* - Can unit equip a staff in strong hand? - */
int Unit_canStaff(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    bool out = false, stronghand = Unit_Hand_Strong(unit);

    if (unit->equipped[stronghand]) {
        struct Inventory_item item = unit->_equipment[stronghand];
        out = Weapon_isStaff(item.id);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* - Can unit equip a staff with only one hand? - */
int Unit_canStaff_oneHand(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (Unit_hasSkill(unit, PASSIVE_SKILL_STAFF_ONE_HAND));
}


/* - Any Weapon to attack with in equipment - */
bool Unit_canAttack_Eq(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    SDL_assert(unit->weapons_dtab != NULL);
    /* - If any item in equipment is a weapon, can attack - */
    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        struct Inventory_item item = unit->_equipment[i];
        if (item.id == ITEM_NULL)
            continue;

        /* Archetype checking*/
        if (!Item_isWeapon(unit->_equipment[i].id))
            continue;

        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, item.id);

        if (!wpn->canAttack)
            continue;

        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (true);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (false);
}

/* - Can unit attack with equipped weapons - */
bool Unit_canAttack(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    SDL_assert(unit->weapons_dtab != NULL);

    bool left   = _Unit_canAttack(unit, UNIT_HAND_LEFT);
    bool right  = _Unit_canAttack(unit, UNIT_HAND_RIGHT);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (left || right);
}

/* - Can unit attack with weapon in hand - */
bool _Unit_canAttack(struct Unit *unit, bool hand) {
    if (!unit->equipped[hand])
        return (false);

    struct Inventory_item item = unit->_equipment[hand];
    if (item.id <= ITEM_NULL)
        return (false);

    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, item.id);
    return (wpn->canAttack);
}



struct Weapon *Unit_Get_Equipped_Weapon(const struct Unit *unit, bool hand) {
    if (!unit->equipped[hand])
        return (NULL);

    int id = unit->_equipment[hand].id;
    if (id == ITEM_NULL)
        return (NULL);

    struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[hand].id);
    return (wpn);
}

/* Add-up all malus stat sources */
if8 Unit_computeMalus(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if8 out = 0;
    unit->malus_stats = Unit_stats_default;
    struct Unit_stats temp;
    /* TODO: Skills Malus */

    /* Rescue Malus */
    if (unit->rescuee >= UNIT_ID_START) {
        temp = Unit_stats_div_cst(unit->current_stats, RESCUE_MALUS_FACTOR);
        unit->malus_stats = Unit_stats_plus(unit->malus_stats, temp);
    }

    /* Equipment Malus */
    struct Weapon *weapon;
    weapon = Unit_Get_Equipped_Weapon(unit, UNIT_HAND_LEFT);
    if (weapon != NULL)
        unit->malus_stats = Unit_stats_plus(unit->malus_stats, weapon->item->malus_stats);

    weapon = Unit_Get_Equipped_Weapon(unit, UNIT_HAND_RIGHT);
    if (weapon != NULL)
        unit->malus_stats = Unit_stats_plus(unit->malus_stats, weapon->item->malus_stats);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* Add-up all bonus stat sources */
if8 Unit_computeBonus(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if8 out = 0;
    unit->bonus_stats = Unit_stats_default;

    /* TODO: Skills Bonus */
    /* Equipment Bonus */
    struct Weapon *weapon;
    weapon = Unit_Get_Equipped_Weapon(unit, UNIT_HAND_LEFT);
    if (weapon != NULL)
        unit->bonus_stats = Unit_stats_plus(unit->bonus_stats, weapon->item->bonus_stats);

    weapon = Unit_Get_Equipped_Weapon(unit, UNIT_HAND_RIGHT);
    if (weapon != NULL)
        unit->bonus_stats = Unit_stats_plus(unit->bonus_stats, weapon->item->bonus_stats);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

u8 *Unit_Shield_Protection(struct Unit *unit, bool hand) {
    if (!unit->equipped[hand])
        return (NULL);

    Weapon_Load(unit->weapons_dtab, unit->equipped[hand]);
    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[hand].id);

    /* should be equivalent to using archetype */
    if (!flagsum_isIn(weapon->item->type, ITEM_TYPE_SHIELD))
        return (NULL);

    return (weapon->stats.protection);
}

u8 *Unit_computeDefense(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Reset unit protections */
    unit->computed_stats.protection[DMG_TYPE_PHYSICAL] = 0;
    unit->computed_stats.protection[DMG_TYPE_MAGICAL]  = 0;

    /* Shield protection */
    int prot_P = 0, prot_M = 0;
    u8 *prot;
    if (prot = Unit_Shield_Protection(unit, UNIT_HAND_LEFT)) {
        prot_P += prot[DMG_TYPE_PHYSICAL];
        prot_M += prot[DMG_TYPE_MAGICAL];
    }

    if (prot = Unit_Shield_Protection(unit, UNIT_HAND_RIGHT)) {
        prot_P += prot[DMG_TYPE_PHYSICAL];
        prot_M += prot[DMG_TYPE_MAGICAL];
    }

    /* Adding shield protection to effective stats */
    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.protection[DMG_TYPE_PHYSICAL] = Equation_Weapon_defense(prot_P, effstats.def);
    unit->computed_stats.protection[DMG_TYPE_MAGICAL]  = Equation_Weapon_defense(prot_M, effstats.res);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.protection);
}

u8 *Unit_computeAttack(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    /* Reset unit attacks */
    unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = 0;
    unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = 0;
    unit->computed_stats.attack[DMG_TYPE_TRUE]     = 0;

    /* Weapon attack */
    u8 *attack;
    int attack_P = 0, attack_M = 0, attack_T = 0;

    struct Weapon *weapon;
    /* Get stats of both weapons */
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        attack_P += Weapon_Stat_inRange(weapon, WEAPON_STAT_pATTACK, distance);
        attack_M += Weapon_Stat_inRange(weapon, WEAPON_STAT_mATTACK, distance);
        attack_T += Weapon_Stat_inRange(weapon, WEAPON_STAT_tATTACK, distance);

    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        attack_P += Weapon_Stat_inRange(weapon, WEAPON_STAT_pATTACK, distance);
        attack_M += Weapon_Stat_inRange(weapon, WEAPON_STAT_mATTACK, distance);
        attack_T += Weapon_Stat_inRange(weapon, WEAPON_STAT_tATTACK, distance);

    }

    /* -- Twohanding -- */
    // DESIGN: weapon stats in both hands added
    // even when teo handing
    //  if (unit->isTwoHanding) {
    /*                   / 2 -> cause attack value is added twice */
    //   attack_P = (attack_P / 2) * TWO_HANDING_MIGHT_FACTOR;
    //    attack_M = (attack_M / 2) * TWO_HANDING_MIGHT_FACTOR;
    //  }

    /* --- TRUE DAMAGE --- */
    /* -- FENCER SKILLS -- */
    if (TNECS_TYPEFLAG_HAS_TYPE(unit->skills, UNIT_SKILL_PINPRICK))
        attack_T += SOTA_SKILL_PINPRICK;
    if (TNECS_TYPEFLAG_HAS_TYPE(unit->skills, UNIT_SKILL_PIERCE))
        attack_T += SOTA_SKILL_PIERCE;
    if (TNECS_TYPEFLAG_HAS_TYPE(unit->skills, UNIT_SKILL_CRUSH) && unit->isTwoHanding)
        attack_T += SOTA_SKILL_CRUSH;

    /* -- Adding weapon attack to effective stats -- */
    struct Unit_stats effstats = unit->effective_stats;

    /* No attacking with only fists -> 0 attack means don't add str/mag */
    if (attack_P > 0)
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = Equation_Weapon_Attack(attack_P, effstats.str);
    if (attack_M > 0)
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = Equation_Weapon_Attack(attack_M, effstats.mag);
    unit->computed_stats.attack[DMG_TYPE_TRUE] = attack_T;

    /* -- DUAL WIELDING -- */
    /* Terrible malus if dual wielding without skill */
    bool candualwield = TNECS_TYPEFLAG_HAS_TYPE(unit->skills, UNIT_SKILL_DUAL_WIELD);
    if (Unit_isdualWielding(unit) && !candualwield) {
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  /= DUAL_WIELD_NOSKILL_MALUS_FACTOR;
    }

    u8 *att = unit->computed_stats.attack;
    att[DMG_TYPE_TOTAL] = att[DMG_TYPE_PHYSICAL] + att[DMG_TYPE_MAGICAL] + att[DMG_TYPE_TRUE];

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.attack);
}
/* -- Loadout Range -- */
/* Compute range of loadout:
*  - Range of weapon in dominant hand
*   - UNLESS dual-wielding. Ranges combine, BUT only add stats if in range.
*/
struct Range *Unit_Range_Loadout(struct Unit   *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    struct Range *range = &unit->computed_stats.range_loadout;
    range->min = UINT8_MAX;
    range->max = 0;
    Unit_isdualWielding(unit);

    bool stronghand = Unit_Hand_Strong(unit);
    do {
        /* If dual wielding, range_loadout is combined. */
        if (unit->isDualWielding) {
            SOTA_Log_Debug("IS dual wielding,");
            _Unit_Range_Combine(unit, range, true, ITEM_ARCHETYPE_WEAPON);
            break;
        }

        /* If not dual wielding, compute range of weapon in stronghand. */
        if (!unit->equipped[stronghand]) {
            SOTA_Log_Debug("If not dual wielding,");
            break;
        }

        if (!Unit_Eq_Usable(unit, ITEM_ARCHETYPE_WEAPON, stronghand)) {
            SOTA_Log_Debug("If not usable,");
            break;
        }

        Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[stronghand].id);
        if (wpn == NULL) {
            SOTA_Log_Debug("wpn null,");
            break;
        }
        Ranges_Combine(range, wpn->stats.range);
    } while (false);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (range);
}

struct Range *Unit_Range_Item(struct Unit   *unit, int i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;

    do {
        /* If dual wielding, range_loadout is combined. */
        Weapon_Load(unit->weapons_dtab, unit->_equipment[i].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
        if (wpn == NULL)
            break;

        Ranges_Combine(range, wpn->stats.range);
    } while (false);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


/* -- Range Combiners -- */
/* Compute range of equipment or equipped by combining ranges
*   Usage:
*       - Dangermap
*       - Menu Option checking
*/

struct Range *_Unit_Range_Combine(const struct Unit   *unit, struct Range *range,
                                  bool equipped, int archetype) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Finds range of ANYTHING - */
    int num = equipped ? UNIT_HANDS_NUM : DEFAULT_EQUIPMENT_SIZE;
    bool stronghand = Unit_Hand_Strong(unit);
    for (int i = 0; i < num; i++) {
        /* Skip if no item */
        if (unit->_equipment[i].id == ITEM_NULL)
            continue;

        /* Equipped only: Skip if unequipped */
        if (equipped && !unit->equipped[i])
            continue;

        /* Skip if unusable */
        if (!Unit_Eq_Usable(unit, archetype, i))
            continue;

        /* Skip if wrong archetype*/
        if ((archetype == ITEM_ARCHETYPE_STAFF)  && !Item_isStaff(unit->_equipment[i].id))
            continue;

        /* Skip if weakhand for staff */
        if ((archetype == ITEM_ARCHETYPE_STAFF)  && (i != stronghand) && (equipped))
            continue;

        if ((archetype == ITEM_ARCHETYPE_WEAPON) && !Item_isWeapon(unit->_equipment[i].id))
            continue;

        /* Combine ranges */
        Weapon_Load(unit->weapons_dtab, unit->_equipment[i].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
        Ranges_Combine(range, wpn->stats.range);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (range);
}

struct Range *Unit_Range_Combine_Equipment(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    /* Compute range of equipment, using unit rangemap */
    /* Used to find if any */
    return (Unit_Range_Combine(unit, false));
}

struct Range *Unit_Range_Combine(struct Unit *unit, bool equipped) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Compute range using unit rangemap */
    int archetype = ITEM_ARCHETYPE_WEAPON;
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;

    if (rangemap == RANGEMAP_HEALMAP) {
        archetype = ITEM_ARCHETYPE_STAFF;
    } else if (rangemap == RANGEMAP_ATTACKMAP) {
        archetype = ITEM_ARCHETYPE_WEAPON;
    }

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (_Unit_Range_Combine(unit, range, equipped, archetype));
}

struct Range *Unit_Range_Combine_Staves(struct Unit *unit, bool equipped) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_STAFF);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (range);
}


struct Range *Unit_Range_Combine_Weapons(struct Unit *unit, bool equipped) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - Finds range only for same weapon type as DECIDED BY INPUT - */

    struct Range *range = &unit->computed_stats.range_combined;
    range->min = UINT8_MAX;
    range->max = 0;
    _Unit_Range_Combine(unit, range, equipped, ITEM_ARCHETYPE_WEAPON);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (range);
}

bool Range_Valid(struct Range range) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return ((range.max > 0) && (range.min > 0) && (range.min <= SOTA_MAX_RANGE)
            && (range.max <= SOTA_MAX_RANGE));
}

struct Range Ranges_Combine(struct Range *r1, struct Range r2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Gap example:    1  2  3  4  5
    // r1: [1,1]      |-|             (gap with    r2)
    // r1: [1,2]      |----|          (no gap with r2)
    // r2: [3,5]            |-------|
    bool r1_valid = Range_Valid(*r1);
    bool r2_valid = Range_Valid(r2);

    bool gap  = (r1->max < (r2.min  - 1)) || (r1->min > (r2.max  + 1));

    if (gap && r1_valid && r2_valid) {
        SOTA_Log_Debug("Gap in combined ranges. Should never happen.");
        exit(ERROR_OutofBounds);
    }

    r1->max = r1->max > r2.max ? r1->max : r2.max; /* Best max range is biggest  */
    r1->min = r1->min < r2.min ? r1->min : r2.min; /* Best min range is smallest */
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

bool Unit_Equipment_Full(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (unit->num_equipment == DEFAULT_EQUIPMENT_SIZE);
}


void Unit_Equipment_Print(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit != NULL);
    for (u8 i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        if (unit->_equipment[i].id == ITEM_NULL) {
            SOTA_Log_Debug("%d ITEM_NULL", i);
            continue;
        }

        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
        if (wpn == NULL) {
            SOTA_Log_Debug("%d Unloaded", i);
            continue;
        }

        SOTA_Log_Debug("%d %s", i, wpn->item->name);
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


struct Computed_Stats Unit_supportBonus(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    return (unit->support_bonus);
}

/* --- Loadout Manipulation --- */
/* - Does that loadout wields a weapon with two hands? - */
bool Unit_Loadout_twoHanding(int lh, int rh) {
    bool lh_valid = (lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE);
    bool rh_valid = (rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE);
    return ((lh_valid && rh_valid && (lh == rh)));
}

/* - Public: Chooses between tohanding and not - */
void Unit_Loadout_Swap(struct Unit *unit, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool lh_valid = (lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE);
    bool rh_valid = (rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE);
    bool twohands = Unit_Loadout_twoHanding(lh, rh);

    if (twohands) {
        _Unit_Loadout_Swap_Twohanding(unit, lh);
    } else if (lh_valid || rh_valid) {
        _Unit_Loadout_Swap(unit, lh, rh);
    } else {
        SOTA_Log_Debug("Invalid input. Not swapping items %d %d", lh, rh);
    }
    // Unit_Equipment_Print(unit);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Loadout_Swap_Reverse(struct Unit *unit, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool lh_valid = (lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE);
    bool rh_valid = (rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE);
    bool twohands = Unit_Loadout_twoHanding(lh, rh);

    if (twohands) {
        _Unit_Loadout_Swap_Reverse_Twohanding(unit);
    } else if (lh_valid || rh_valid) {
        _Unit_Loadout_Swap_Reverse(unit, lh, rh);
    } else {
        SOTA_Log_Debug("Invalid input. Not swapping items %d %d", lh, rh);
    }
    // Unit_Equipment_Print(unit);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Unit_Loadout_Swap_Twohanding(struct Unit *unit, int i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int stronghand  = Unit_Hand_Strong(unit);
    if (i != stronghand)
        Unit_Item_Swap(unit, stronghand, i);
    unit->temp = Unit_Equip_TwoHanding(unit);
    // TODO: Place in an empty inventory spot.
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Unit_Loadout_Swap_Reverse_Twohanding(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int stronghand  = Unit_Hand_Strong(unit);
    int weakhand    = 1 - stronghand;
    // TODO: Only takeat if item was not previously dropped.
    _Unit_Item_Takeat(unit, unit->temp, weakhand);
    // Unit_Equipment_Print(unit);
    unit->temp = Inventory_item_default;
    unit->isTwoHanding = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Unit_Loadout_Swap(struct Unit *unit, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(lh != rh); /* no twohanding here */
    bool lh_valid = ((lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE));
    bool rh_valid = ((rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE));

    /* Swapping */
    if (lh_valid && (lh != UNIT_HAND_LEFT)) {
        Unit_Item_Swap(unit, UNIT_HAND_LEFT, lh);
        /* item rh wanted switched UNIT_HAND_LEFT -> lh */
        if (rh == UNIT_HAND_LEFT)
            rh = lh;
    }

    if (rh_valid && (rh != UNIT_HAND_RIGHT))
        Unit_Item_Swap(unit, UNIT_HAND_RIGHT, rh);

    /* Equipping */
    if (lh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
    else
        Unit_Unequip(unit, UNIT_HAND_LEFT);

    if (rh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
    else
        Unit_Unequip(unit, UNIT_HAND_RIGHT);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* If input hand indices (lh, rh) are unvalid, unequip hand */
void _Unit_Loadout_Swap_Reverse(struct Unit *unit, int lh, int rh) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(lh != rh); /* no twohanding here */
    bool lh_valid = ((lh > -1) && (lh < DEFAULT_EQUIPMENT_SIZE));
    bool rh_valid = ((rh > -1) && (rh < DEFAULT_EQUIPMENT_SIZE));

    /* Swapping */
    if (lh_valid && (lh != UNIT_HAND_LEFT)) {
        /* item rh wanted switched UNIT_HAND_LEFT -> lh */
        if (rh == UNIT_HAND_LEFT)
            rh = lh;
    }

    if (rh_valid && (rh != UNIT_HAND_RIGHT))
        Unit_Item_Swap(unit, UNIT_HAND_RIGHT, rh);
    if (lh_valid && (lh != UNIT_HAND_LEFT))
        Unit_Item_Swap(unit, UNIT_HAND_LEFT,  lh);

    /* Equipping */
    if (lh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
    else
        Unit_Unequip(unit, UNIT_HAND_LEFT);

    if (rh_valid)
        Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
    else
        Unit_Unequip(unit, UNIT_HAND_RIGHT);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Computed_Stats Unit_computedStats_wLoadout(struct Unit *unit, int lh, int rh, int dist) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // TODO: what if unit is already twohanding?
    Unit_Loadout_Swap(unit, lh, rh);
    Unit_computedStats(unit, dist);
    Unit_Loadout_Swap_Reverse(unit, lh, rh);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats);
}

/* Computed stats at distance (-1 is always in range) */
struct Computed_Stats Unit_computedStats(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    if (!unit->update_stats) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (unit->computed_stats);
    }

    /* check if no weapons in hand*/
    if (unit->equipped[UNIT_HAND_LEFT] || unit->equipped[UNIT_HAND_RIGHT]) {
        Unit_computeHit(unit,       distance);
        Unit_computeAttack(unit,    distance);
        Unit_computeCritical(unit,  distance);
    } else {
        unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = 0;
        unit->computed_stats.attack[DMG_TYPE_MAGICAL]  = 0;
        unit->computed_stats.attack[DMG_TYPE_TRUE]     = 0;
        unit->computed_stats.attack[DMG_TYPE_TOTAL]    = 0;
        unit->computed_stats.hit                       = 0;
        unit->computed_stats.crit                      = 0;
        unit->computed_stats.range_combined.min        = 0;
        unit->computed_stats.range_combined.max        = 0;
        unit->computed_stats.range_loadout.min         = 0;
        unit->computed_stats.range_loadout.max         = 0;
    }
    /* Distance-dependent stats */
    Unit_computeSpeed(unit,   distance);
    Unit_computeDodge(unit,   distance);
    Unit_computeFavor(unit,   distance);

    /* Distance-independent stats */
    Unit_computeMove(unit);
    Unit_computeAgony(unit);
    Unit_computeDefense(unit);
    Unit_computeRegrets(unit);
    Unit_Range_Loadout(unit);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats);
}

/* Add regrets to computed stats. */
if8 Unit_computeRegrets(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    /* Pre-computation */
    if8 malus = Equation_Regrets(unit->regrets, unit->effective_stats.fth);
    struct Computed_Stats stats = unit->computed_stats;

    /* Apply regrets malus to computed stats */
    unit->computed_stats.hit   = stats.hit   > malus ? stats.hit               - malus : 0;
    unit->computed_stats.dodge = stats.dodge > malus + INT8_MIN  ? stats.dodge - malus : INT8_MIN;
    unit->computed_stats.crit  = stats.crit  > malus ? stats.crit              - malus : 0;
    unit->computed_stats.favor = stats.favor > malus ? stats.favor             - malus : 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (malus);
}

u8 Unit_computeHit(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    u8 supports = 0, wpn_hit = 0;
    int hit_L    = 0, hit_R   = 0;
    struct Weapon *weapon;
    /* Get stats of both weapons */
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        hit_L   = Weapon_Stat_inRange(weapon, WEAPON_STAT_HIT, distance);
    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        hit_R    = Weapon_Stat_inRange(weapon, WEAPON_STAT_HIT, distance);
    }
    wpn_hit = Equation_Weapon_Hit(hit_L, hit_R);

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.hit   = Equation_Unit_Hit(wpn_hit, effstats.dex, effstats.luck, supports);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.hit);
}

if8 Unit_computeDodge(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    u8 support   = 0, tile_dodge = 0;
    int wgt_L     = 0, wgt_R      = 0;
    int dodge_L   = 0, dodge_R    = 0;
    u8 wpn_dodge = 0, wpn_wgt    = 0;
    struct Weapon *weapon;
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        dodge_L = Weapon_Stat_inRange(weapon, WEAPON_STAT_DODGE, distance);
        wgt_L = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        dodge_R  = Weapon_Stat_inRange(weapon, WEAPON_STAT_DODGE, distance);
        wgt_R  = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }

    wpn_dodge = Equation_Weapon_Dodge(dodge_L, dodge_R);
    /* weapons weight in both hands always added */
    wpn_wgt   = Equation_Weapon_Wgt( wgt_L,   wgt_R);

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.dodge = Equation_Unit_Dodge(wpn_wgt, wpn_dodge, effstats.luck,
                                                     effstats.fth, effstats.agi, effstats.str,
                                                     effstats.con, tile_dodge, support);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.dodge);
}

u8 Unit_computeCritical(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    // TODO: get support bonus
    u8 supports = 0;
    int crit_L = 0, crit_R = 0;
    struct Weapon *weapon;
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        crit_L = Weapon_Stat_inRange(weapon, WEAPON_STAT_CRIT, distance);
    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        crit_R  = Weapon_Stat_inRange(weapon, WEAPON_STAT_CRIT, distance);
    }

    u8 wpn_crit = Equation_Weapon_Crit(crit_L, crit_R);

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.crit = Equation_Unit_Crit(wpn_crit, effstats.dex, effstats.luck, supports);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.crit);
}

u8 Unit_computeFavor(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    u8 supports = 0 ;
    int favor_L = 0, favor_R = 0;
    struct Weapon *weapon;
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        favor_L = Weapon_Stat_inRange(weapon, WEAPON_STAT_FAVOR, distance);
    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        favor_R  = Weapon_Stat_inRange(weapon, WEAPON_STAT_FAVOR, distance);
    }

    u8 wpn_favor = Equation_Weapon_Crit(favor_L, favor_R);

    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.favor = Equation_Unit_Favor(wpn_favor, effstats.fth, supports);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.favor);
}

u8 Unit_computeAgony(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    struct Unit_stats effstats = unit->effective_stats;
    unit->computed_stats.agony = Equation_Agony_Turns(effstats.str, effstats.def, effstats.con);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.agony);
}

if8 Unit_computeSpeed(struct Unit *unit, int distance) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    SDL_assert(unit->weapons_dtab);
    int weight_L = 0, weight_R = 0;
    struct Weapon *weapon;
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        weight_L  = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id > ITEM_NULL);
        weapon   = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        weight_R  = Weapon_Stat(weapon, WEAPON_STAT_WGT);
    }
    /* item weight in both hands is always added */
    u8 wpn_wgt = Equation_Weapon_Wgt(weight_L, weight_R);
    if (unit->isTwoHanding)
        wpn_wgt /= TWO_HANDING_WEIGHT_FACTOR;

    // if (TNECS_TYPEFLAG_HAS_TYPE(unit->skills, UNIT_SKILL_)) {
    // TODO: compute effective_weight
    struct Unit_stats fstats = unit->effective_stats;
    unit->computed_stats.speed = Equation_Unit_Speed(wpn_wgt, fstats.agi, fstats.con, fstats.str);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.speed);
}

if8 Unit_computeMove(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    if8 move = unit->effective_stats.move;
    if (unit->mount != NULL)
        move = MOVE_WITH_MOUNT;
    unit->computed_stats.move = move;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->computed_stats.move);
}

/* -- Deplete: decrease durability -- */
void _Unit_Item_Deplete(struct Unit *unit, int i, int archetype) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Only unit function that calls Inventory_item_Deplete */

    /* Skip if NULL. Not an error, unit can have empty hand. */
    if (unit->_equipment[i].id == ITEM_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* Skip if item's archetype to deplete does not match input. */
    if (!(Item_Archetype(unit->_equipment[i].id) != archetype)) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[i].id);
    struct Item   *item   = weapon->item;
    Inventory_item_Deplete(&unit->_equipment[UNIT_HAND_LEFT], item->stats.uses);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Unit_Equipped_Deplete(struct Unit *unit, bool hand, int archetype) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (!unit->equipped[hand]) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    _Unit_Item_Deplete(unit, hand, archetype);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Item_Deplete(struct Unit *unit, int i) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth, __func__);
    /* Upon use, decrease item durability */
    _Unit_Item_Deplete(unit, i, ITEM_ARCHETYPE_NULL);
}

void Unit_Equipped_Staff_Deplete(struct Unit *unit, bool hand) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Upon healing, decrease staff durability */
    _Unit_Equipped_Deplete(unit, hand, ITEM_ARCHETYPE_STAFF);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Equipped_Weapons_Deplete(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Upon getting hit, decrease shields durability */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_WEAPON);
    if (!unit->isTwoHanding)
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_WEAPON);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Equipped_Shields_Deplete(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Upon getting hit, use shields */
    _Unit_Equipped_Deplete(unit, UNIT_HAND_LEFT, ITEM_ARCHETYPE_SHIELD);
    if (!unit->isTwoHanding)
        _Unit_Equipped_Deplete(unit, UNIT_HAND_RIGHT, ITEM_ARCHETYPE_SHIELD);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- I/O --- */
void Unit_readJSON(void *input, const cJSON *const junit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *unit = (struct Unit *)input;
    SDL_assert(unit);
    SOTA_Log_Debug("-- Get json objects --");
    cJSON *jid              = cJSON_GetObjectItem(junit, "id");
    cJSON *jsex             = cJSON_GetObjectItem(junit, "Sex");
    cJSON *jexp             = cJSON_GetObjectItem(junit, "Exp");
    cJSON *jname            = cJSON_GetObjectItem(junit, "Name");
    cJSON *jitems           = cJSON_GetObjectItem(junit, "Items");
    cJSON *jgrowths         = cJSON_GetObjectItem(junit, "Growths");
    cJSON *jlevelups        = cJSON_GetObjectItem(junit, "Level-ups");
    cJSON *jsupports        = cJSON_GetObjectItem(junit, "Supports");
    cJSON *jbase_exp        = cJSON_GetObjectItem(junit, "BaseExp");
    cJSON *jcurrent_hp      = cJSON_GetObjectItem(junit, "CurrentHP");
    cJSON *jbase_stats      = cJSON_GetObjectItem(junit, "Bases");
    cJSON *jcaps_stats      = cJSON_GetObjectItem(junit, "Caps");
    cJSON *jclass_index     = cJSON_GetObjectItem(junit, "Class Index");
    cJSON *jsupport_type    = cJSON_GetObjectItem(junit, "Support Type");
    cJSON *jcurrent_stats   = cJSON_GetObjectItem(junit, "Stats");
    unit->_id = cJSON_GetNumberValue(jid); //returns 0 if junit is NULL

    SOTA_Log_Debug("-- startup misc --");
    SDL_assert(unit->name != NULL);
    strncpy(unit->name, cJSON_GetStringValue(jname), strlen(cJSON_GetStringValue(jname)));
    unit->sex               = cJSON_IsTrue(jsex);
    unit->exp               = cJSON_GetNumberValue(jexp);
    unit->base_exp          = cJSON_GetNumberValue(jbase_exp);
    unit->current_hp        = cJSON_GetNumberValue(jcurrent_hp);
    Unit_setClassind(unit, cJSON_GetNumberValue(jclass_index));
    SDL_assert(jcurrent_stats);

    SOTA_Log_Debug("-- Supports --");
    unit->support_type = cJSON_GetNumberValue(jsupport_type);
    if (cJSON_IsArray(jsupports)) {
        unit->support_num = cJSON_GetArraySize(jsupports);
        for (int i = 0; i < unit->support_num; ++i) {
            struct cJSON *jsupport = cJSON_GetArrayItem(jsupports, i);
            char *name = cJSON_GetStringValue(jsupport);
            unit->supports[i].other_id = Hashes_supportName2ID(name);
            unit->supports[i].level = 0;
            unit->supports[i].other_type = 0;
        }
    } else {
        SOTA_Log_Debug("Supports is not an array");
    }

    SOTA_Log_Debug("--set stats --");
    Filesystem_readJSON_Unitstats(jcurrent_stats, &unit->current_stats);
    SDL_assert(jcaps_stats);
    Filesystem_readJSON_Unitstats(jcaps_stats, &unit->caps_stats);
    SDL_assert(jbase_stats);
    Filesystem_readJSON_Unitstats(jbase_stats, &unit->base_stats);
    SDL_assert(jgrowths);
    Filesystem_readJSON_Unitstats(jgrowths, &unit->growths);
    cJSON *jlevelup = cJSON_GetObjectItem(jlevelups, "Level-up");
    struct Unit_stats temp_ustats;
    if (unit->grown_stats != NULL) {
        DARR_FREE(unit->grown_stats);
        unit->grown_stats = NULL;
    }

    unit->grown_stats = DARR_INIT(unit->grown_stats, struct Unit_stats, SOTA_MAX_LEVEL / 8);
    while (jlevelup != NULL) {
        Filesystem_readJSON_Unitstats(jlevelup, &temp_ustats);
        DARR_PUT(unit->grown_stats, temp_ustats);
        jlevelup = jlevelup->next;
    };
    SOTA_Log_Debug("-- items --");
    /* -- Unit should also read/write equipped -- */
    cJSON *jitem;
    cJSON_ArrayForEach(jitem, jitems) {
        struct Inventory_item temp_item;
        Filesystem_readJSON_Item(jitem, &temp_item);
        if (temp_item.id > ITEM_NULL)
            Unit_Item_Take(unit, temp_item);
    }
    if (unit->_equipment[UNIT_HAND_RIGHT].id > 0)
        Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
    if (unit->_equipment[UNIT_HAND_LEFT].id > 0)
        Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void Unit_writeJSON(const void *input, cJSON *junit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *unit = (struct Unit *)input;
    SDL_assert(unit);
    SDL_assert(junit);
    cJSON *jid            = cJSON_CreateNumber(unit->_id);
    cJSON *jexp           = cJSON_CreateNumber(unit->base_exp);
    cJSON *jsex           = cJSON_CreateBool(unit->sex);
    cJSON *jname          = cJSON_CreateString(unit->name);
    cJSON *jclass         = cJSON_CreateString(classNames[unit->class]);
    cJSON *jbase_exp      = cJSON_CreateNumber(unit->exp);
    cJSON *jcurrent_hp    = cJSON_CreateNumber(unit->current_hp);
    cJSON *jclass_index   = cJSON_CreateNumber(unit->class);
    cJSON *jcurrent_stats = cJSON_CreateObject();
    Filesystem_writeJSON_Unitstats(jcurrent_stats, &unit->current_stats);
    cJSON *jcaps_stats    = cJSON_CreateObject();
    Filesystem_writeJSON_Unitstats(jcaps_stats, &unit->caps_stats);
    cJSON *jbase_stats    = cJSON_CreateObject();
    Filesystem_writeJSON_Unitstats(jbase_stats, &unit->base_stats);
    cJSON *jgrowths       = cJSON_CreateObject();
    Filesystem_writeJSON_Unitstats(jgrowths, &unit->growths);
    cJSON *jgrown         = cJSON_CreateObject();
    cJSON *jlevel         = NULL;
    cJSON *jlevelup       = NULL;
    cJSON_AddItemToObject(junit, "level",       jlevel);
    cJSON_AddItemToObject(junit, "id",          jid);
    cJSON_AddItemToObject(junit, "Name",        jname);
    cJSON_AddItemToObject(junit, "Sex",         jsex);
    cJSON_AddItemToObject(junit, "BaseExp",     jbase_exp);
    cJSON_AddItemToObject(junit, "Exp",         jexp);
    cJSON_AddItemToObject(junit, "CurrentHP",   jcurrent_hp);
    cJSON_AddItemToObject(junit, "Class",       jclass);
    cJSON_AddItemToObject(junit, "Class Index", jclass_index);
    cJSON_AddItemToObject(junit, "Stats",       jcurrent_stats);
    cJSON_AddItemToObject(junit, "Caps",        jcaps_stats);
    cJSON_AddItemToObject(junit, "Bases",       jbase_stats);
    cJSON_AddItemToObject(junit, "Growths",     jgrowths);
    cJSON_AddItemToObject(junit, "Level-ups",   jgrown);
    for (u8 i = 0; i < DARR_NUM(unit->grown_stats); i++) {
        jlevelup = cJSON_CreateObject();
        jlevel = cJSON_CreateNumber(i - unit->base_exp / SOTA_100PERCENT + 2);
        cJSON_AddItemToObject(jlevelup, "level", jlevel);
        Filesystem_writeJSON_Unitstats(jlevelup, &unit->grown_stats[i]);
        cJSON_AddItemToObject(jgrown, "Level-up", jlevelup);
        // +2 -> +1 start at lvl1, +1 cause you level to level 2
    }
    cJSON *jitems = cJSON_CreateArray();
    for (u8 item_num = 0; item_num < DEFAULT_EQUIPMENT_SIZE; item_num ++) {
        cJSON *jitem = cJSON_CreateObject();
        Filesystem_writeJSON_item(jitem, &unit->_equipment[item_num]);
        cJSON_AddItemToArray(jitems, jitem);
    }
    cJSON_AddItemToObject(junit, "Items", jitems);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

u8 Unit_computeEffectivefactor(struct Unit *attacker, struct Unit *defender) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(attacker);
    SDL_assert(defender);
    u8 effective = NOTEFFECTIVE_FACTOR;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (effective);
}

/* Compute Brave factor, c-a-d combat attack multiplier in all combat phases
    Function computes the highest brave factor among equipped weapons.
    This means that -> Brave factor DOES NOT STACK <-
*/
u8 Unit_Brave(const struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SDL_assert(unit);
    u8 out_brave   = 1;
    u64 temp_effect = 0;
    struct Weapon *weapon;
    // TODO: use AP to compute brave factor
    if (unit->equipped[UNIT_HAND_LEFT]) {
        weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        temp_effect = weapon->item->passive;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X))
            out_brave = 2;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X))
            out_brave = 3;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X))
            out_brave = 4;
    }

    if (unit->equipped[UNIT_HAND_RIGHT]) {
        weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        temp_effect = weapon->item->passive;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X))
            out_brave = (out_brave >  2) ? out_brave : 2;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X))
            out_brave = (out_brave >  3) ? out_brave : 3;
        else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X))
            out_brave = 4;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_brave);
}

void Unit_Cap_Stats(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    unit->current_stats.hp   = unit->caps_stats.hp;
    unit->current_stats.str  = unit->caps_stats.str;
    unit->current_stats.mag  = unit->caps_stats.mag;
    unit->current_stats.agi  = unit->caps_stats.agi;
    unit->current_stats.dex  = unit->caps_stats.dex;
    unit->current_stats.fth  = unit->caps_stats.fth;
    unit->current_stats.luck = unit->caps_stats.luck;
    unit->current_stats.def  = unit->caps_stats.def;
    unit->current_stats.res  = unit->caps_stats.res;
    unit->current_stats.con  = unit->caps_stats.con;
    unit->current_stats.prof = unit->caps_stats.prof;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* What is this for?
caps before promotion? */
void Unit_HalfCap_Stats(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    unit->current_stats.hp   = unit->caps_stats.hp   / 2;
    unit->current_stats.str  = unit->caps_stats.str  / 2;
    unit->current_stats.mag  = unit->caps_stats.mag  / 2;
    unit->current_stats.agi  = unit->caps_stats.agi  / 2;
    unit->current_stats.dex  = unit->caps_stats.dex  / 2;
    unit->current_stats.fth  = unit->caps_stats.fth  / 2;
    unit->current_stats.luck = unit->caps_stats.luck / 2;
    unit->current_stats.def  = unit->caps_stats.def  / 2;
    unit->current_stats.res  = unit->caps_stats.res  / 2;
    unit->current_stats.con  = unit->caps_stats.con  / 2;
    unit->current_stats.prof = unit->caps_stats.prof / 2;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

struct Unit_stats Unit_effectiveGrowths(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    unit->effective_growths = unit->growths;
    Unit_stats_plus(unit->bonus_growths, unit->effective_growths);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->effective_growths);
}

struct Unit_stats Unit_effectiveStats(struct Unit *unit) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* bases + grown + bonus - malus */
    SDL_assert(unit);
    /* Skip if not update_stats? */
    if (!unit->update_stats) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (unit->effective_stats);
    }

    /* Preparation */
    struct Unit_stats temp_ustats = Unit_stats_default;
    unit->bonus_stats             = Unit_stats_default;
    unit->malus_stats             = Unit_stats_default;
    unit->effective_stats         = unit->current_stats;
    struct Weapon *weapon;

    /* Computing effective stats for strong hand weapon */
    /* just wielding the weapon increases stats */
    if (unit->equipped[UNIT_HAND_LEFT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_LEFT].id != ITEM_NULL);
        weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_LEFT].id);
        Unit_stats_plus(unit->bonus_stats, weapon->item->bonus_stats);
        Unit_stats_plus(unit->malus_stats, weapon->item->malus_stats);
    }
    /* Computing effective stats for right hand weapon */
    if (unit->equipped[UNIT_HAND_RIGHT]) {
        SDL_assert(unit->_equipment[UNIT_HAND_RIGHT].id != ITEM_NULL);
        weapon = DTAB_GET(unit->weapons_dtab, unit->_equipment[UNIT_HAND_RIGHT].id);
        Unit_stats_plus(unit->bonus_stats, weapon->item->bonus_stats);
        Unit_stats_plus(unit->malus_stats, weapon->item->malus_stats);
    }

    /* If unit is mounted, unit movement is mount movement */
    if (unit->mount != NULL)
        unit->effective_stats.move = MOVE_WITH_MOUNT;

    Unit_stats_plus(unit->effective_stats,  unit->bonus_stats);
    Unit_stats_minus(unit->effective_stats, unit->malus_stats);

    /* effective HP is total hp plus bonuses */
    unit->effective_stats.hp = unit->current_stats.hp;
    //TODO: hp bonuses

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (unit->effective_stats);
}

void Unit_Promote(struct Unit *unit, if8 new_class_index) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit);
    // struct Unit_stats promotion_bonus = DTAB_GET(promotion_bonus_stats, new_class_index);
    // DARR_PUT(unit->grown_stats, promotion_bonus);
    // Unit_stats_plus(unit->current_stats, promotion_bonus);
    // unit->skills += DTAB_GET(promotion_bonus_skills, new_class_index);
    // unit->class = new_class_index;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Rangemap --- */
int Unit_Rangemap_Get(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (rangemap);
}

void Unit_Rangemap_Toggle(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert((unit->rangemap > RANGEMAP_NULL) && (unit->rangemap < RANGEMAP_NUM));

    /* Set user_rangemap to default */
    if (unit->user_rangemap == RANGEMAP_NULL)
        unit->user_rangemap = unit->rangemap;

    /* Toggle only if hasStaff or canAttack with equipment*/
    bool toggle = false;
    toggle |= Unit_canAttack_Eq(unit) && (unit->user_rangemap == RANGEMAP_HEALMAP);
    toggle |= Unit_canStaff_Eq(unit)  && (unit->user_rangemap == RANGEMAP_ATTACKMAP);

    /* user_rangemap not set previously, reverse rangemap */
    // RANGEMAP_NUM - RANGEMAP_ATTACKMAP == RANGEMAP_HEALMAP and vice versa!
    //      3       -         2          ==        1
    //      3       -         1          ==        2
    if (toggle)
        unit->user_rangemap = RANGEMAP_NUM - unit->user_rangemap;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Unit_Rangemap_Equipment(struct Unit *unit) {
    /* 1- Weapon equipped in strong hand */
    bool stronghand = Unit_Hand_Strong(unit);
    if (unit->equipped[stronghand]) {
        Weapon_Load(unit->weapons_dtab, unit->_equipment[stronghand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[stronghand].id);
        uf16 type = wpn->item->type;

        if (flagsum_isIn(type, ITEM_TYPE_STAFF) || flagsum_isIn(type, ITEM_TYPE_ITEM)) {
            unit->rangemap = RANGEMAP_HEALMAP;
        } else {
            unit->rangemap = RANGEMAP_ATTACKMAP;
        }
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* 2- Weapon equipped in weak hand */
    bool weakhand = 1 - stronghand;
    if (unit->equipped[weakhand] && unit->hands[weakhand]) {
        Weapon_Load(unit->weapons_dtab, unit->_equipment[weakhand].id);
        struct Weapon *wpn = DTAB_GET(unit->weapons_dtab, unit->_equipment[weakhand].id);
        uf16 type = wpn->item->type;

        if (flagsum_isIn(type, ITEM_TYPE_STAFF) || flagsum_isIn(type, ITEM_TYPE_ITEM)) {
            unit->rangemap = RANGEMAP_HEALMAP;
        } else {
            unit->rangemap = RANGEMAP_ATTACKMAP;
        }

        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
}

void Unit_Rangemap_Default(struct Unit *unit) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    int rangemap = unit->user_rangemap > RANGEMAP_NULL ? unit->user_rangemap : unit->rangemap;
    // Compute default rangemap priority

    /* Sota default for class (healer staff) */
    if ((unit->class == UNIT_CLASS_PRIEST) || (unit->class == UNIT_CLASS_BISHOP) ||
        (unit->class == UNIT_CLASS_CLERIC) || (unit->class == UNIT_CLASS_ORACLE)) {
        unit->rangemap = RANGEMAP_HEALMAP;
    } else {
        unit->rangemap = RANGEMAP_ATTACKMAP;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Unit stats --- */
struct Unit_stats Unit_stats_plus_cst(struct Unit_stats in_stats1, int cst) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(in_stats1.hp,   cst, UINT8_MAX);
    out_stats.str   = nmath_bplus(in_stats1.str,  cst, UINT8_MAX);
    out_stats.mag   = nmath_bplus(in_stats1.mag,  cst, UINT8_MAX);
    out_stats.agi   = nmath_bplus(in_stats1.agi,  cst, UINT8_MAX);
    out_stats.dex   = nmath_bplus(in_stats1.dex,  cst, UINT8_MAX);
    out_stats.luck  = nmath_bplus(in_stats1.luck, cst, UINT8_MAX);
    out_stats.def   = nmath_bplus(in_stats1.def,  cst, UINT8_MAX);
    out_stats.res   = nmath_bplus(in_stats1.res,  cst, UINT8_MAX);
    out_stats.con   = nmath_bplus(in_stats1.con,  cst, UINT8_MAX);
    out_stats.move  = nmath_bplus(in_stats1.move, cst, UINT8_MAX);
    out_stats.prof  = nmath_bplus(in_stats1.prof, cst, UINT8_MAX);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}

struct Unit_stats Unit_stats_plus(struct Unit_stats in_stats1,
                                  struct Unit_stats in_stats2) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bplus(in_stats1.hp,    in_stats2.hp,   UINT8_MAX);
    out_stats.str   = nmath_bplus(in_stats1.str,   in_stats2.str,  UINT8_MAX);
    out_stats.mag   = nmath_bplus(in_stats1.mag,   in_stats2.mag,  UINT8_MAX);
    out_stats.agi   = nmath_bplus(in_stats1.agi,   in_stats2.agi,  UINT8_MAX);
    out_stats.dex   = nmath_bplus(in_stats1.dex,   in_stats2.dex,  UINT8_MAX);
    out_stats.luck  = nmath_bplus(in_stats1.luck,  in_stats2.luck, UINT8_MAX);
    out_stats.def   = nmath_bplus(in_stats1.def,   in_stats2.def,  UINT8_MAX);
    out_stats.res   = nmath_bplus(in_stats1.res,   in_stats2.res,  UINT8_MAX);
    out_stats.con   = nmath_bplus(in_stats1.con,   in_stats2.con,  UINT8_MAX);
    out_stats.move  = nmath_bplus(in_stats1.move,  in_stats2.move, UINT8_MAX);
    out_stats.prof  = nmath_bplus(in_stats1.prof,  in_stats2.prof, UINT8_MAX);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}

struct Unit_stats Unit_stats_minus_cst(struct Unit_stats in_stats1, int cst) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(in_stats1.hp,   cst, 0);
    out_stats.str   = nmath_bminus(in_stats1.str,  cst, 0);
    out_stats.mag   = nmath_bminus(in_stats1.mag,  cst, 0);
    out_stats.agi   = nmath_bminus(in_stats1.agi,  cst, 0);
    out_stats.dex   = nmath_bminus(in_stats1.dex,  cst, 0);
    out_stats.luck  = nmath_bminus(in_stats1.luck, cst, 0);
    out_stats.def   = nmath_bminus(in_stats1.def,  cst, 0);
    out_stats.res   = nmath_bminus(in_stats1.res,  cst, 0);
    out_stats.con   = nmath_bminus(in_stats1.con,  cst, 0);
    out_stats.move  = nmath_bminus(in_stats1.move, cst, 0);
    out_stats.prof  = nmath_bminus(in_stats1.prof, cst, 0);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}

struct Unit_stats Unit_stats_div_cst(struct Unit_stats in_stats1, int cst) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = in_stats1.hp    / cst;
    out_stats.str   = in_stats1.str   / cst;
    out_stats.mag   = in_stats1.mag   / cst;
    out_stats.agi   = in_stats1.agi   / cst;
    out_stats.dex   = in_stats1.dex   / cst;
    out_stats.luck  = in_stats1.luck  / cst;
    out_stats.def   = in_stats1.def   / cst;
    out_stats.res   = in_stats1.res   / cst;
    out_stats.con   = in_stats1.con   / cst;
    out_stats.move  = in_stats1.move  / cst;
    out_stats.prof  = in_stats1.prof  / cst;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}

struct Unit_stats Unit_stats_minus(struct Unit_stats in_stats1, struct Unit_stats in_stats2) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp    = nmath_bminus(in_stats1.hp,    in_stats2.hp,   0);
    out_stats.str   = nmath_bminus(in_stats1.str,   in_stats2.str,  0);
    out_stats.mag   = nmath_bminus(in_stats1.mag,   in_stats2.mag,  0);
    out_stats.agi   = nmath_bminus(in_stats1.agi,   in_stats2.agi,  0);
    out_stats.dex   = nmath_bminus(in_stats1.dex,   in_stats2.dex,  0);
    out_stats.luck  = nmath_bminus(in_stats1.luck,  in_stats2.luck, 0);
    out_stats.def   = nmath_bminus(in_stats1.def,   in_stats2.def,  0);
    out_stats.res   = nmath_bminus(in_stats1.res,   in_stats2.res,  0);
    out_stats.con   = nmath_bminus(in_stats1.con,   in_stats2.con,  0);
    out_stats.move  = nmath_bminus(in_stats1.move,  in_stats2.move, 0);
    out_stats.prof  = nmath_bminus(in_stats1.prof,  in_stats2.prof, 0);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out_stats);
}


bool Unit_ID_Valid(uf16 id) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Unit ID valid: %d", id);
    bool out = (id > UNIT_ID_PC_START)      && (id < UNIT_ID_PC_END);
    out |=     (id > UNIT_ID_NPC_START)     && (id < UNIT_ID_NPC_END);
    out |=     (id > UNIT_ID_GENERIC_START) && (id < UNIT_ID_GENERIC_END);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

