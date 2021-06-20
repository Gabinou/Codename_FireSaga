#include "unit.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_ds.h"
#include "stb_sprintf.h"
// #endif  STB_SPRINTF_IMPLEMENTATION

struct Mount Mount_default_horse = {
    .type = MOUNT_TYPE_HORSE,
    .price = 2000,
    .ridable_classes = 0,
    .carry_capacity = 10,
};

struct Mount Mount_default_pegasus = {
    .type = MOUNT_TYPE_PEGASUS,
    .price = 0,
    .ridable_classes = (int8_t *)UNIT_CLASS_PIKEMAN,
    .carry_capacity = 7,
};

struct Unit Unit_default = {
    .json_element = JSON_UNIT,
    .base_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .bonus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .caps_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .malus_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .current_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    .growths = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    .skills = 0,
    .exp = 0,
    .base_exp = 0,
    .army = 1,
    ._id = 0,
    .status_turns = {
        .poisoned = 0,
        .sleep = 0,
        .stone = 0,
        .rooted = 0,
        .stunned = 0,
        .silenced = 0,
        .slowed = 0,
    },

    .grown_stats = NULL,
    .weapons = NULL,

    .current_hp = 0,
    .is_alive = true,
    .talkable = 0,

    ._sex = false, // 0:F, 1:M. eg. hasPenis.
    .literate = false, // Reading/writing for scribe job.
    .show_danger = false,

    .weapons = NULL,
    .computed_stats = {0, {0, 0}, {0, 0}, 0, 0, 0, 0, 0, 0, 0},
    ._range = {-1, -1},
    .equipped = {-1, -1},
    .hands = {true, true},

    .mount_assigned = NULL,

    ._equipment = NULL, // 0 for right hand, 1 for left hand
};

void Unit_init(struct Unit * in_unit) {
    SDL_Log("Unit_init");
    SDL_assert(in_unit);
    hmfree(in_unit->weapons);
    hmdefault(*in_unit->weapons, Weapon_default);
}

void Unit_setid(struct Unit * in_unit, const int16_t in_id) {
    SDL_Log("Unit_setid");
    in_unit->_id = in_id;
    SDL_assert(in_unit);
    strcpy(in_unit->name, unitNames[in_unit->_id]);
    SDL_Log("Unit new id: %d, name: %s", in_unit->_id, in_unit->name);
}

void Unit_setSex(struct Unit * in_unit, const bool in_sex) {
    SDL_Log("Unit_setSex");
    SDL_assert(in_unit);
    in_unit->_sex = in_sex;
    in_unit->sex_name = sexNames[in_unit->_sex];
    SDL_Log("Unit sex id: %d, name: %s", in_unit->_sex, in_unit->sex_name);
}

void Unit_setSkills(struct Unit * in_unit, uint64_t in_skills) {
    SDL_Log("Unit_setSkills");
    SDL_assert(in_unit);
    in_unit->skills = in_skills;
    in_unit->skill_names = utilities_skillNames(in_unit->skills);
    SDL_Log("Unit new skills: %lx \n", in_unit->skills);
    for (uint8_t i = 0; arrlen(in_unit->skill_names); i++) {
        SDL_Log("Skill name: %s", in_unit->skill_names[i]);
    }
}

void Unit_setClassind(struct Unit * in_unit, int8_t in_class_index) {
    SDL_Log("Unit_setClassind");
    SDL_assert(in_unit);
    SDL_assert((in_class_index > 0) && (in_class_index < UNIT_CLASS_END));
    in_unit->class_index = in_class_index;
    SDL_Log("in_unit->class_index, utilities_mvtType, %d, %d", in_unit->class_index, utilities_mvtType(in_unit->class_index));
    in_unit->mvt_type = utilities_mvtType(in_unit->class_index);
    in_unit->class_name = classNames[in_unit->class_index];
    in_unit->equippable = equippableClasscodes[in_unit->class_index];
}

void Unit_setStats(struct Unit * in_unit, const struct Unit_stats in_stats) {
    SDL_assert(in_unit);
    in_unit->current_stats = in_stats;
    in_unit->current_hp = in_unit->current_stats.hp;
}

void Unit_setBases(struct Unit * in_unit, const struct Unit_stats in_stats) {
    SDL_assert(in_unit);
    in_unit->base_stats = in_stats;
    in_unit->current_hp = in_unit->base_stats.hp;
}

void Unit_setArmy(struct Unit * in_unit, const uint8_t in_army) {
    SDL_Log("Unit_setArmy");
    SDL_assert(in_unit);
    in_unit->army = in_army;
    in_unit->army_name = armyNames[in_army];
}

void Unit_Item_Add(struct Unit * in_unit, struct Inventory_item in_item) {
    SDL_Log("Unit_Item_Add");
    SDL_assert(in_unit);
    size_t current_len = arrlen(in_unit->_equipment);
    uint8_t equipped_num;
    equipped_num += (in_unit->equipped[UNIT_HAND_RIGHT] > -1);
    equipped_num += (in_unit->equipped[UNIT_HAND_LEFT] > -1);
    SDL_assert(current_len < (DEFAULT_EQUIPMENT_SIZE - equipped_num));
    arrput(in_unit->_equipment, in_item);
}

void Unit_Item_Move(struct Unit * in_unit, const int16_t ind1, const int16_t ind2) {
    SDL_Log("Unit_Item_Move");
    SDL_assert(in_unit);
    struct Inventory_item buffer = in_unit->_equipment[ind1];
    in_unit->_equipment[ind1] = in_unit->_equipment[ind2];
    in_unit->_equipment[ind2] = buffer;
}

void Unit_Item_Remove(struct Unit * in_unit, int8_t in_index) {
    SDL_Log("Unit_Item_Remove");
    SDL_assert(in_unit);
    in_unit->_equipment[in_index] = Inventory_item_default;
}

void Unit_Item_Take(struct Unit * in_unit, struct Inventory_item * out_array, int16_t in_index, int16_t out_index) {
    SDL_Log("Unit_Item_Take");
    in_unit->_equipment[in_index] = out_array[out_index];
    out_array[out_index] = Inventory_item_default;
}

void Unit_Item_Give(struct Unit * in_unit, struct Inventory_item * out_array, int16_t in_index, int16_t out_index) {
    SDL_Log("Unit_Item_Give");
    SDL_assert(in_unit);
    out_array[out_index] = in_unit->_equipment[in_index];
    in_unit->_equipment[in_index] = Inventory_item_default;
}

void Unit_Item_Drop(struct Unit * in_unit, int16_t in_index) {
    SDL_Log("Unit_Item_Drop");
    SDL_assert(in_unit);
    in_unit->_equipment[in_index] = Inventory_item_default;
}

void Unit_equips(struct Unit * in_unit, const bool hand) {
    SDL_Log("hand, equipment id: %d %d", hand, in_unit->_equipment[hand].id);
    // should not check if equippable. is equippable is known before calling this function,
    // to show to the player and enable/disable the function
    SDL_assert(in_unit);
    SDL_assert(in_unit->_equipment[hand].id > 0);
    Unit_checkWeapon(in_unit, in_unit->_equipment[hand].id);
    if ((Unit_canEquip(in_unit, in_unit->_equipment[hand].id)) && (in_unit->hands[hand])) {
        in_unit->equipped[hand] = true;
    } else {
        in_unit->equipped[hand] = false;
    }
}

void Unit_unequips(struct Unit * in_unit, const bool hand) {
    SDL_Log("Unit_unequips");
    SDL_assert(in_unit);
    in_unit->_range[0] = -1;
    in_unit->_range[1] = -1;
    in_unit->equipped[hand] = -1;
}

void Unit_takesDamage(struct Unit * in_unit, const uint8_t damage) {
    SDL_Log("%s takes %d damage \n", in_unit-> name, damage);
    SDL_assert(in_unit);
    in_unit->current_hp = (damage > in_unit->current_hp) ? 0 : (in_unit->current_hp - damage);
    if (in_unit->current_hp == 0) { Unit_agonizes(in_unit); }
}

void Unit_getsHealed(struct Unit * in_unit, const uint8_t healing) {
    SDL_Log("%s gets healed for %d\n", in_unit->name, healing);
    SDL_assert(in_unit);
    uint8_t effective_healing = (in_unit->current_hp > UINT8_MAX - healing) ? in_unit->current_stats.hp - in_unit->current_hp : healing;
    in_unit->current_hp = ((in_unit->current_hp + effective_healing) > in_unit->current_stats.hp) ? in_unit->current_stats.hp : in_unit->current_hp + effective_healing ;
}

void Unit_wait(struct Unit * in_unit) {
    SDL_assert(in_unit);
    in_unit->waits = true;
}

void Unit_refresh(struct Unit * in_unit) {
    SDL_assert(in_unit);
    in_unit->waits = false;
}

int16_t Unit_getLvl(struct Unit * in_unit) {
    SDL_assert(in_unit);
    return (ceil(in_unit->exp / DEFAULT_HYAKUPERCENT) + 1);
}

void Unit_gainExp(struct Unit * in_unit, const uint16_t in_exp) {
    SDL_assert(in_unit);
    if (((in_unit->exp % DEFAULT_HYAKUPERCENT) + in_exp) > DEFAULT_HYAKUPERCENT) {
        Unit_lvlUp(in_unit);
        // Never should have two level ups at one time.
    }
    in_unit->exp += in_exp;
}

void Unit_supportUp(struct Unit * in_unit, int16_t in_id) {
    SDL_assert(in_unit);
    int i = 0;
    while ((in_unit->supports[i].index = ! in_id) && (i < DEFAULT_SUPPORTS_MAX)) {
        i++;
    }
    if (i == DEFAULT_SUPPORTS_MAX) {
        SDL_Log("Could not find support of index %d", in_id);
    }
    in_unit->supports[i].level += 1;
}

void Unit_lvlUp(struct Unit * in_unit) {
    SDL_Log("Unit_lvlUp");
    SDL_assert(in_unit);
    uint8_t prob;
    uint8_t temp_growth;
    struct Unit_stats temp_stats = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    temp_growth = in_unit->growths.hp;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.hp += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.hp < in_unit->caps_stats.hp)) {
        temp_stats.hp += 1;
    }
    temp_growth = in_unit->growths.str;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.str += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.str < in_unit->caps_stats.str)) {
        temp_stats.str += 1;
    }
    temp_growth = in_unit->growths.mag;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.mag += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.mag < in_unit->caps_stats.mag)) {
        temp_stats.mag += 1;
    }
    temp_growth = in_unit->growths.dex;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.dex += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.dex < in_unit->caps_stats.dex)) {
        temp_stats.dex += 1;
    }
    temp_growth = in_unit->growths.agi;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.agi += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.agi < in_unit->caps_stats.agi)) {
        temp_stats.agi += 1;
    }
    temp_growth = in_unit->growths.luck;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.luck += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.luck < in_unit->caps_stats.luck)) {
        temp_stats.luck += 1;
    }
    temp_growth = in_unit->growths.def;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.def += 1;
        temp_growth;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.def < in_unit->caps_stats.def)) {
        temp_stats.def += 1;
    }
    temp_growth = in_unit->growths.res;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.res += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.res < in_unit->caps_stats.res)) {
        temp_stats.res += 1;
    }
    temp_growth = in_unit->growths.con;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.con += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.con < in_unit->caps_stats.con)) {
        temp_stats.con += 1;
    }
    temp_growth = in_unit->growths.prof;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.prof += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.prof < in_unit->caps_stats.prof)) {
        temp_stats.prof += 1;
    }
    temp_growth = in_unit->growths.move;
    while (temp_growth > DEFAULT_HYAKUPERCENT) {
        temp_stats.move += 1;
        temp_growth -= DEFAULT_HYAKUPERCENT;
    }
    prob = URN_global();
    if ((prob <= temp_growth) && (in_unit->current_stats.move < in_unit->caps_stats.move)) {
        temp_stats.move += 1;
    }
    arrput(in_unit->grown_stats, temp_stats);
    in_unit->current_stats.hp += temp_stats.hp;
    in_unit->current_stats.str += temp_stats.str;
    in_unit->current_stats.mag += temp_stats.mag;
    in_unit->current_stats.dex += temp_stats.dex;
    in_unit->current_stats.agi += temp_stats.agi;
    in_unit->current_stats.luck += temp_stats.luck;
    in_unit->current_stats.def += temp_stats.def;
    in_unit->current_stats.res += temp_stats.res;
    in_unit->current_stats.move += temp_stats.move;
    in_unit->current_stats.con += temp_stats.con;
    in_unit->current_stats.prof += temp_stats.prof;
}

void Unit_agonizes(struct Unit * in_unit) {
    in_unit->agonizes = true;
    SDL_Log("%s is agonizing. %d turns until death\n", in_unit->name, in_unit->computed_stats.agony);
}

void Unit_dies(struct Unit * in_unit) {
    SDL_assert(in_unit);
    in_unit->is_alive = false;
    SDL_Log("%s is dead.\n", in_unit->name);
}

void Unit_checkWeapon(struct Unit * in_unit, int16_t in_id) {
    SDL_Log("Unit_checkWeapon");
    SDL_assert(in_unit);
    SDL_assert(in_unit->weapons != NULL);
    char * token;
    char buffer[DEFAULT_BUFFER_SIZE];
    if (hmgetp_null(*in_unit->weapons, in_id) == NULL) {
        char filename[DEFAULT_BUFFER_SIZE] = "items"PHYSFS_SEPARATOR;
        strncpy(buffer, hmget(itemNames, in_id), DEFAULT_BUFFER_SIZE);
        token = strtok(buffer, " \t");
        while (token != NULL) {
            strcat(filename, token);
            token = strtok(NULL, " \t");
        }
        strcat(filename, ".json");
        SDL_Log("Loading weapon %d %s", in_id, filename);
        struct Weapon temp_weapon =  Weapon_default;
        readJSON(filename, &temp_weapon);
        SDL_Log("temp_weapon.hit = %d", temp_weapon.stats.combat.hit);
        hmput(*in_unit->weapons, in_id, temp_weapon);
    }
}

bool Unit_canEquip(struct Unit * in_unit, int16_t in_id) {
    SDL_Log("canEquip %d", in_id);
    bool out = false;
    SDL_assert(in_unit->weapons != NULL);
    Unit_checkWeapon(in_unit, in_id);
    struct Weapon weapon = hmget(*in_unit->weapons, in_id);
    uint16_t wpntypecode = weapon.item->type;
    if ((weapon.item->users == NULL) | (weapon.item->users == 0)) {
        out = ((in_unit->equippable & wpntypecode) > 0);
    } else {
        bool found = false;
        for (uint8_t i = 0; i < weapon.item->users_num; i++) {
            found = (weapon.item->users[i] == in_unit->_id);
            if (found) {
                out = ((in_unit->equippable & wpntypecode) > 0);
                break;
            }
        }
    }
    return (out);
}

bool Unit_iswpnEquipped(struct Unit * in_unit) {
    bool out = false;
    if (in_unit->weapons != NULL) {
        bool left;
        bool right;
        if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
            left = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).attackable;
        }
        if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
            right = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).attackable;
        }
        out = left || right;
    } else {
        SDL_Log("Unit_iswpnEquipped: weapons pointer is NULL");
    }
    return (out);
}

uint8_t * Unit_computeDefense(struct Unit * in_unit) {
    SDL_Log("Unit_computeDefense");
    in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL] = 0;
    in_unit->computed_stats.defense[DMG_TYPE_MAGICAL] = 0;
    struct Weapon_stats temp_wstats;
    uint16_t temp_type;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_type = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).item->type;
        if (flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
            in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL] = temp_wstats.Pmight;
            in_unit->computed_stats.defense[DMG_TYPE_MAGICAL] = temp_wstats.Mmight;
        }
    }

    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_type = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).item->type;
        if (flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
            in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL] = equation_defense(in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL], temp_wstats.Pmight);
            in_unit->computed_stats.defense[DMG_TYPE_MAGICAL] = equation_defense(in_unit->computed_stats.defense[DMG_TYPE_MAGICAL], temp_wstats.Mmight);
        }
    }
    in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL] = equation_defense(in_unit->computed_stats.defense[DMG_TYPE_PHYSICAL], in_unit->effective_stats.def);
    in_unit->computed_stats.defense[DMG_TYPE_MAGICAL] = equation_defense(in_unit->computed_stats.defense[DMG_TYPE_MAGICAL], in_unit->effective_stats.res);
    return (in_unit->computed_stats.defense);
}

uint8_t * Unit_computeAttack(struct Unit * in_unit) {
    SDL_Log("Unit_computeAttack");
    in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = 0;
    in_unit->computed_stats.attack[DMG_TYPE_MAGICAL] = 0;
    struct Weapon_stats temp_wstats;
    uint16_t temp_type;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_type = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).item->type;
        if (!flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
            in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = temp_wstats.Pmight;
            in_unit->computed_stats.attack[DMG_TYPE_MAGICAL] = temp_wstats.Mmight;
        }
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_type = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).item->type;
        if (!flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
            in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = equation_attack(in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL], temp_wstats.Pmight);
            in_unit->computed_stats.attack[DMG_TYPE_MAGICAL] = equation_attack(in_unit->computed_stats.attack[DMG_TYPE_MAGICAL], temp_wstats.Mmight);
        }
    }
    if (in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL] > 0) {
        in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL] = equation_attack(in_unit->computed_stats.attack[DMG_TYPE_PHYSICAL], in_unit->effective_stats.str);
    }
    if (in_unit->computed_stats.attack[DMG_TYPE_MAGICAL] > 0) {
        in_unit->computed_stats.attack[DMG_TYPE_MAGICAL] = equation_attack(in_unit->computed_stats.attack[DMG_TYPE_MAGICAL], in_unit->effective_stats.mag);
    }
    return (in_unit->computed_stats.attack);
}

int_tile_t * Unit_computeRange(struct Unit * in_unit) {
    SDL_Log("Computing unit _range\n");
    struct Weapon temp_weapon;
    int_tile_t * temp_range = NULL;
    arrput(temp_range, 0);
    arrput(temp_range, 0);
    SDL_assert(in_unit->weapons != NULL);
    if (in_unit->equipped[UNIT_HAND_LEFT] >= 0) {
        if (in_unit->_equipment[UNIT_HAND_LEFT].id > 0) {
            temp_weapon = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id);
            if (temp_weapon.attackable) {
                temp_range = temp_weapon.stats.range;
                in_unit->_range[0] = temp_range[0];
                in_unit->_range[1] = temp_range[1];
            }
        }
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] >= 0) {
        if (in_unit->_equipment[UNIT_HAND_RIGHT].id > 0) {
            temp_weapon = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id);
            if (temp_weapon.attackable) {
                temp_range = temp_weapon.stats.range;
                in_unit->_range[0] = ((temp_range[0] < in_unit->_range[0]) & (in_unit->_range[0] > 0)) ? temp_range[0] : in_unit->_range[0];
                in_unit->_range[1] = (temp_range[1] > in_unit->_range[1]) ? temp_range[1] : in_unit->_range[1];
            }
        }
    }
    return (in_unit->_range);
}

struct Computed_Stats Unit_computedStats(struct Unit * in_unit) {
    SDL_Log("Unit_computedStats");
    SDL_assert(in_unit);
    Unit_computeHit(in_unit);
    Unit_computeDodge(in_unit);
    Unit_computeCritical(in_unit);
    Unit_computeFavor(in_unit);
    Unit_computeSpeed(in_unit);
    Unit_computeAttack(in_unit);
    Unit_computeAgony(in_unit);
    Unit_computeDefense(in_unit);
    Unit_computeMove(in_unit);
    in_unit->computed_stats.hp = in_unit->current_hp;
    return (in_unit->computed_stats);
}

uint8_t Unit_computeHit(struct Unit * in_unit) {
    SDL_Log("Unit_computeHit");
    SDL_assert(in_unit);
    uint8_t supports = 0;
    uint8_t wpn_hit = 0;
    struct Weapon_stats temp_wstats_R = Weapon_stats_default;
    struct Weapon_stats temp_wstats_L = Weapon_stats_default;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_wstats_L = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_wstats_R = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
    }
    wpn_hit = equation_wpnHit(temp_wstats_R.combat.hit, temp_wstats_L.combat.hit);
    in_unit->computed_stats.hit = equation_unitHit(wpn_hit, in_unit->current_stats.dex, in_unit->current_stats.luck, supports);
    return (in_unit->computed_stats.hit);
}

int8_t Unit_computeDodge(struct Unit * in_unit) {
    SDL_Log("Unit_computeDodge");
    SDL_assert(in_unit);
    uint8_t support = 0;
    uint8_t tile_dodge = 0;
    uint8_t wpn_dodge = 0;
    uint8_t wpn_wgt = 0;
    struct Weapon_stats temp_wstats_R = Weapon_stats_default;
    struct Weapon_stats temp_wstats_L = Weapon_stats_default;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_wstats_L = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_wstats_R = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
    }
    wpn_dodge = equation_wpnDodge(temp_wstats_R.combat.dodge, temp_wstats_L.combat.dodge);
    wpn_wgt = equation_attack(temp_wstats_R.wgt, temp_wstats_L.wgt);

    in_unit->computed_stats.dodge = equation_unitDodge(wpn_dodge, wpn_wgt, in_unit->effective_stats.luck, in_unit->effective_stats.agi, in_unit->effective_stats.str, in_unit->effective_stats.con, tile_dodge, support);

    return (in_unit->computed_stats.dodge);
}

uint8_t Unit_computeCritical(struct Unit * in_unit) {
    SDL_Log("Unit_computeCritical");
    SDL_assert(in_unit);
    uint8_t supports = 0;
    struct Weapon_stats temp_wstats_R = Weapon_stats_default;
    struct Weapon_stats temp_wstats_L = Weapon_stats_default;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_wstats_L = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_wstats_R = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
    }
    uint8_t wpn_crit = equation_wpnCrit(temp_wstats_R.combat.crit, temp_wstats_L.combat.crit);
    in_unit->computed_stats.crit = equation_unitCrit(wpn_crit, in_unit->current_stats.dex, in_unit->current_stats.luck, supports);
    return (in_unit->computed_stats.crit);
}

uint8_t Unit_computeFavor(struct Unit * in_unit) {
    SDL_Log("Unit_computeFavor");
    SDL_assert(in_unit);
    uint8_t supports = 0 ;
    uint8_t Unit_computeFavor = 0;
    struct Weapon_stats temp_wstats_R = Weapon_stats_default;
    struct Weapon_stats temp_wstats_L = Weapon_stats_default;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_wstats_L = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_wstats_R = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
    }
    uint8_t wpn_favor = equation_wpnCrit(temp_wstats_R.combat.favor, temp_wstats_L.combat.favor);
    in_unit->computed_stats.favor = equation_unitFavor(wpn_favor, in_unit->current_stats.luck, supports);
    return (in_unit->computed_stats.favor);
}

extern uint8_t Unit_computeAgony(struct Unit * in_unit) {
    SDL_assert(in_unit);
    in_unit->computed_stats.agony = equation_agonyTurns(in_unit->effective_stats.str, in_unit->effective_stats.def, in_unit->effective_stats.con);
    return (in_unit->computed_stats.agony);
}

int8_t Unit_computeSpeed(struct Unit * in_unit) {
    SDL_assert(in_unit);
    struct Weapon_stats temp_wstats_R = Weapon_stats_default;
    struct Weapon_stats temp_wstats_L = Weapon_stats_default;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_wstats_L = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).stats;
    }
    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_wstats_R = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).stats;
    }

    uint8_t wpn_wgt = equation_wpnWgt(temp_wstats_R.wgt, temp_wstats_L.wgt);
    in_unit->computed_stats.speed = equation_unitSpeed(wpn_wgt, in_unit->current_stats.agi, in_unit->current_stats.con, in_unit->current_stats.str);
    return (in_unit->computed_stats.speed);
}

int8_t Unit_computeMove(struct Unit * in_unit) {
    SDL_assert(in_unit);
    int8_t move = in_unit->effective_stats.move;
    if (in_unit->mount_assigned != NULL) {
        move = MOVE_WITH_MOUNT;
    }
    in_unit->computed_stats.move = move;
    return (in_unit->computed_stats.move);
}

void Unit_readJSON(struct Unit * in_unit, cJSON * in_junit) {
    SDL_Log("Unit_readJSON");
    SDL_assert(in_unit);
    cJSON * jid = cJSON_GetObjectItemCaseSensitive(in_junit, "id");
    cJSON * jname = cJSON_GetObjectItemCaseSensitive(in_junit, "Name");
    cJSON * jsex = cJSON_GetObjectItemCaseSensitive(in_junit, "Sex");
    cJSON * jbase_exp = cJSON_GetObjectItemCaseSensitive(in_junit, "BaseExp");
    cJSON * jexp = cJSON_GetObjectItemCaseSensitive(in_junit, "Exp");
    cJSON * jcurrent_hp = cJSON_GetObjectItemCaseSensitive(in_junit, "CurrentHP");
    cJSON * jclass_index = cJSON_GetObjectItemCaseSensitive(in_junit, "Class Index");
    cJSON * jcurrent_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Stats");
    cJSON * jbase_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Bases");
    cJSON * jcaps_stats = cJSON_GetObjectItemCaseSensitive(in_junit, "Caps");
    cJSON * jgrowths = cJSON_GetObjectItemCaseSensitive(in_junit, "Growths");
    cJSON * jlevelups = cJSON_GetObjectItemCaseSensitive(in_junit, "Level-ups");
    cJSON * jitems = cJSON_GetObjectItemCaseSensitive(in_junit, "Items");
    in_unit->_id = cJSON_GetNumberValue(jid); //returns 0 if junit is NULL
    strncpy(in_unit->name, cJSON_GetStringValue(jname), strlen(cJSON_GetStringValue(jname)));
    Unit_setSex(in_unit, cJSON_IsTrue(jsex));
    in_unit->base_exp = cJSON_GetNumberValue(jbase_exp);
    in_unit->exp = cJSON_GetNumberValue(jexp);
    in_unit->current_hp = cJSON_GetNumberValue(jcurrent_hp);
    Unit_setClassind(in_unit, cJSON_GetNumberValue(jclass_index));
    readJSON_Unitstats(jcurrent_stats, &in_unit->current_stats);
    readJSON_Unitstats(jcaps_stats, &in_unit->caps_stats);
    readJSON_Unitstats(jbase_stats, &in_unit->base_stats);
    readJSON_Unitstats(jgrowths, &in_unit->growths);
    cJSON * jlevelup = cJSON_GetObjectItemCaseSensitive(jlevelups, "Level-up");
    struct Unit_stats temp_ustats;
    while (jlevelup != NULL) {
        readJSON_Unitstats(jlevelup, &temp_ustats);
        arrput(in_unit->grown_stats, temp_ustats);
        jlevelup = jlevelup->next;
    };
    cJSON * jitem = cJSON_GetObjectItemCaseSensitive(jitems, "Item");
    struct Inventory_item temp_item;
    int i = 0;
    // Unit should also read/write equipped
    while ((jitem != NULL) && (i < DEFAULT_EQUIPMENT_SIZE)) {
        readJSON_item(jitem, &temp_item);
        Unit_Item_Add(in_unit, temp_item);
        jitem = jitem->next;
        i++;
    };
    if (in_unit->_equipment[UNIT_HAND_RIGHT].id > 0) {
        Unit_equips(in_unit, UNIT_HAND_RIGHT);
    }
}

void Unit_writeJSON(struct Unit * in_unit, cJSON * in_junit) {
    SDL_assert(in_unit);
    cJSON * jid = cJSON_CreateNumber(in_unit->_id);
    cJSON * jexp = cJSON_CreateNumber(in_unit->base_exp);
    cJSON * jbase_exp = cJSON_CreateNumber(in_unit->exp);
    cJSON * jcurrent_hp = cJSON_CreateNumber(in_unit->current_hp);
    cJSON * jsex = cJSON_CreateBool(in_unit->_sex);
    cJSON * jname = cJSON_CreateString(in_unit->name);
    cJSON * jclass = cJSON_CreateString(in_unit->class_name);
    cJSON * jclass_index = cJSON_CreateNumber(in_unit->class_index);
    cJSON * jcurrent_stats = cJSON_CreateObject();
    writeJSON_Unitstats(jcurrent_stats, &in_unit->current_stats);
    cJSON * jcaps_stats = cJSON_CreateObject();
    writeJSON_Unitstats(jcaps_stats, &in_unit->caps_stats);
    cJSON * jbase_stats = cJSON_CreateObject();
    writeJSON_Unitstats(jbase_stats, &in_unit->base_stats);
    cJSON * jgrowths = cJSON_CreateObject();
    writeJSON_Unitstats(jgrowths, &in_unit->growths);
    cJSON * jgrown = cJSON_CreateObject();
    cJSON * jlevel = NULL;
    cJSON * jlevelup = NULL;
    cJSON_AddItemToObject(in_junit, "level", jlevel);
    cJSON_AddItemToObject(in_junit, "id", jid);
    cJSON_AddItemToObject(in_junit, "Name", jname);
    cJSON_AddItemToObject(in_junit, "Sex", jsex);
    cJSON_AddItemToObject(in_junit, "BaseExp", jbase_exp);
    cJSON_AddItemToObject(in_junit, "Exp", jexp);
    cJSON_AddItemToObject(in_junit, "CurrentHP", jcurrent_hp);
    cJSON_AddItemToObject(in_junit, "Class", jclass);
    cJSON_AddItemToObject(in_junit, "Class Index", jclass_index);
    cJSON_AddItemToObject(in_junit, "Stats", jcurrent_stats);
    cJSON_AddItemToObject(in_junit, "Caps", jcaps_stats);
    cJSON_AddItemToObject(in_junit, "Bases", jbase_stats);
    cJSON_AddItemToObject(in_junit, "Growths", jgrowths);
    cJSON_AddItemToObject(in_junit, "Level-ups", jgrown);
    for (uint8_t i = 0; i < arrlen(in_unit->grown_stats); i++) {
        jlevelup = cJSON_CreateObject();
        jlevel = cJSON_CreateNumber(i - in_unit->base_exp / DEFAULT_HYAKUPERCENT + 2);
        cJSON_AddItemToObject(jlevelup, "level", jlevel);
        writeJSON_Unitstats(jlevelup, &in_unit->grown_stats[i]);
        cJSON_AddItemToObject(jgrown, "Level-up", jlevelup);
        // +2 -> +1 start at lvl1, +1 cause you level to level 2
    }
    cJSON * jitems = cJSON_CreateObject();
    for (uint8_t item_num = 0; item_num < DEFAULT_EQUIPMENT_SIZE; item_num ++) {
        writeJSON_item(jitems, &in_unit->_equipment[item_num]);
    }
    cJSON_AddItemToObject(in_junit, "Items", jitems);
}

bool Unit_canDouble(struct Unit * attacker, struct Unit * defender) {
    SDL_assert(attacker);
    SDL_assert(defender);
    bool doubles = false;
    if (attacker->computed_stats.speed > defender->computed_stats.speed) {
        doubles = ((attacker->computed_stats.speed - defender->computed_stats.speed) > DEFAULT_DOUBLEHIT_SPEED);
    }
    return (doubles);
}

bool Unit_canAttack(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos) {
    SDL_assert(attacker);
    SDL_assert(defender);
    SDL_assert(attacker_pos);
    SDL_assert(defender_pos);
    bool can = false;
    uint8_t distance = abs(defender_pos->x - attacker_pos->x) + abs(defender_pos->y - attacker_pos->y);

    int_tile_t * attacker_range = Unit_computeRange(attacker);

    if ((distance >= attacker_range[0]) && (distance <= attacker_range[1])) {
        can = true;
    }

    return (can);
}

uint8_t Unit_computeEffectivefactor(struct Unit * attacker, struct Unit * defender) {
    SDL_assert(attacker);
    SDL_assert(defender);
    uint8_t effective = NOTEFFECTIVE_FACTOR;
    return (effective);
}

struct Damage Unit_computeDamage(struct Unit * attacker, struct Unit * defender) {
    SDL_Log("Unit_computeDamage");
    SDL_assert(attacker);
    SDL_assert(defender);
    uint8_t attack[DMG_TYPES] = {0, 0};
    uint8_t wpn_might[DMG_TYPES] = {0, 0};

    struct Weapon_stats temp_wstats;
    uint16_t temp_type;
    if (attacker->equipped[UNIT_HAND_LEFT] > 0) {
        temp_type = hmget(*attacker->weapons, attacker->_equipment[UNIT_HAND_LEFT].id).item->type;
        if (!flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*attacker->weapons, attacker->_equipment[UNIT_HAND_RIGHT].id).stats;
            wpn_might[DMG_TYPE_PHYSICAL] = temp_wstats.Pmight;
            wpn_might[DMG_TYPE_MAGICAL] = temp_wstats.Mmight;
        }
    }
    if (attacker->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_type = hmget(*attacker->weapons, attacker->_equipment[UNIT_HAND_RIGHT].id).item->type;
        if (!flagsum_isIn(temp_type, ITEM_TYPE_SHIELD)) {
            temp_wstats = hmget(*attacker->weapons, attacker->_equipment[UNIT_HAND_RIGHT].id).stats;
            wpn_might[DMG_TYPE_PHYSICAL] = equation_attack(wpn_might[DMG_TYPE_PHYSICAL], temp_wstats.Pmight);
            wpn_might[DMG_TYPE_MAGICAL] = equation_attack(wpn_might[DMG_TYPE_MAGICAL], temp_wstats.Mmight);
        }
    }
    attack[DMG_TYPE_PHYSICAL] = equation_attack(wpn_might[DMG_TYPE_PHYSICAL], attacker->effective_stats.str);
    attack[DMG_TYPE_MAGICAL] = equation_attack(wpn_might[DMG_TYPE_MAGICAL], attacker->effective_stats.mag);

    uint8_t effective = Unit_computeEffectivefactor(attacker, defender);

    attacker->damage.dmg[DMG_TYPE_PHYSICAL] = equation_combatDamage(attack[DMG_TYPE_PHYSICAL], defender->computed_stats.defense[DMG_TYPE_PHYSICAL], effective, CRIT_FACTOR, 0);
    attacker->damage.dmg[DMG_TYPE_MAGICAL] = equation_combatDamage(attack[DMG_TYPE_MAGICAL], defender->computed_stats.defense[DMG_TYPE_MAGICAL], effective, CRIT_FACTOR, 0);
    attacker->damage.dmg_crit[DMG_TYPE_PHYSICAL] = equation_combatDamage(attack[DMG_TYPE_PHYSICAL], defender->computed_stats.defense[DMG_TYPE_PHYSICAL], effective, CRIT_FACTOR, 1);
    attacker->damage.dmg_crit[DMG_TYPE_MAGICAL] = equation_combatDamage(attack[DMG_TYPE_MAGICAL], defender->computed_stats.defense[DMG_TYPE_MAGICAL], effective, CRIT_FACTOR, 1);

    return (attacker->damage);
}

struct Combat_Forecast_Rates Unit_Combat_Forecast_Rates(struct Unit * attacker, struct Unit * defender) {
    SDL_assert(attacker);
    SDL_assert(defender);
    struct Combat_Forecast_Rates out_rates = Combat_Forecast_Rates_default;
    Unit_computedStats(attacker);
    Unit_computedStats(defender);
    out_rates.hit = equation_rateHit(attacker->computed_stats.hit, defender->computed_stats.dodge);
    out_rates.crit = equation_rateCrit(attacker->computed_stats.crit, defender->computed_stats.favor);
    return (out_rates);
}

struct Combat_Forecast_Stats Unit_Combat_Forecast_Stats(struct Unit * attacker, struct Unit * defender) {
    struct Combat_Forecast_Stats out_stats;
    out_stats.attacker_rates = Unit_Combat_Forecast_Rates(attacker, defender);
    out_stats.defender_rates = Unit_Combat_Forecast_Rates(defender, attacker);
    out_stats.attacker_damage = Unit_computeDamage(attacker, defender);
    out_stats.defender_damage = Unit_computeDamage(defender, attacker);
    out_stats.defender_computed =  Unit_computedStats(defender);
    out_stats.attacker_computed =  Unit_computedStats(attacker);
    out_stats.attacker_equipment[0] = attacker->equipped[0];
    out_stats.attacker_equipment[1] = attacker->equipped[1];
    out_stats.defender_equipment[0] = defender->equipped[0];
    out_stats.defender_equipment[1] = defender->equipped[1];
    return (out_stats);
}

uint8_t * attacksNumfromCombatFlow(struct Combat_Forecast_Flow in_forecast_flow) {
    uint8_t * out = NULL;
    out = (uint8_t *) malloc(2 * sizeof(*out));
    if (in_forecast_flow.defender_retaliates) {
        out[DEFENDER] = (1 + in_forecast_flow.defender_doubles) * in_forecast_flow.defender_brave;
    } else {
        out[DEFENDER] = 0;
    }
    out[ATTACKER] = (1 + in_forecast_flow.attacker_doubles) * in_forecast_flow.attacker_brave;
    return (out);
}

struct Combat_Forecast_Death Unit_Combat_Forecast_Death(struct Combat_Forecast_Stats in_forecast_stats, struct Combat_Forecast_Flow in_forecast_flow) {
    struct Combat_Forecast_Death out_death = Combat_Forecast_Death_default;
    uint8_t * attacksnum = attacksNumfromCombatFlow(in_forecast_flow);
    uint8_t attacker_maxDamage_nocrit = 0;
    uint8_t attacker_maxDamage_crit = 0;
    uint8_t defender_maxDamage_nocrit = 0;
    uint8_t defender_maxDamage_crit = 0;
    if (in_forecast_flow.defender_retaliates) {
        if (in_forecast_stats.defender_rates.hit > 0) {
            defender_maxDamage_nocrit = equation_multiplyDamage((in_forecast_stats.defender_damage.dmg[DMG_TYPE_PHYSICAL] + in_forecast_stats.defender_damage.dmg[DMG_TYPE_MAGICAL]), attacksnum[DEFENDER]);
            defender_maxDamage_crit = equation_multiplyDamage((in_forecast_stats.defender_damage.dmg_crit[DMG_TYPE_PHYSICAL] + in_forecast_stats.defender_damage.dmg_crit[DMG_TYPE_MAGICAL]), attacksnum[DEFENDER]);
            if ((in_forecast_stats.attacker_computed.hp < defender_maxDamage_nocrit) && (in_forecast_stats.defender_rates.hit > 99)) {
                out_death.attacker_certain = true;
            }
            if ((in_forecast_stats.attacker_computed.hp < defender_maxDamage_nocrit) && (in_forecast_stats.defender_rates.hit < 99)) {
                out_death.attacker_possible = true;
            }
            if ((in_forecast_stats.defender_rates.crit > 0)
                    && (in_forecast_stats.attacker_computed.hp < defender_maxDamage_crit)) {
                out_death.attacker_possible = true;
            }
        }
        if (in_forecast_stats.attacker_rates.hit > 0) {
            attacker_maxDamage_nocrit = equation_multiplyDamage((in_forecast_stats.attacker_damage.dmg[DMG_TYPE_PHYSICAL] + in_forecast_stats.attacker_damage.dmg[DMG_TYPE_MAGICAL]), attacksnum[ATTACKER]);
            attacker_maxDamage_crit = equation_multiplyDamage((in_forecast_stats.attacker_damage.dmg_crit[DMG_TYPE_PHYSICAL] + in_forecast_stats.attacker_damage.dmg_crit[DMG_TYPE_MAGICAL]), attacksnum[ATTACKER]);
            if ((in_forecast_stats.defender_computed.hp < attacker_maxDamage_nocrit)  && (in_forecast_stats.attacker_rates.hit > 99)) {
                out_death.defender_certain = true;
            }
            if ((in_forecast_stats.defender_computed.hp < attacker_maxDamage_nocrit)  && (in_forecast_stats.attacker_rates.hit < 99)) {
                out_death.defender_possible = true;
            }
            if ((in_forecast_stats.attacker_rates.crit > 0)
                    && (in_forecast_stats.defender_computed.hp < attacker_maxDamage_crit)) {
                out_death.defender_possible = true;
            }
            if ((out_death.attacker_certain) & (out_death.defender_certain)) {
                out_death.attacker_certain = false;
                out_death.defender_certain = false;
                out_death.attacker_possible = true;
                out_death.defender_possible = true;
            }
        }
    }
    return (out_death);
}


uint8_t Unit_brave(struct Unit * in_unit) {
    SDL_assert(in_unit);
    uint8_t out_brave = 1;
    uint64_t temp_effect = 0;
    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_effect = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).item->passive_effect;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X)) {
            out_brave = 2;
        } else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X)) {
            out_brave = 3;
        } else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X)) {
            out_brave = 4;
        }
    }

    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_effect = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).item->passive_effect;
        if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE2X)) {
            out_brave = (out_brave >  2) ? out_brave : 2;
        } else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE3X)) {
            out_brave = (out_brave >  3) ? out_brave : 3;
        } else if (flagsum_isIn(temp_effect, ITEM_EFFECT_BRAVE4X)) {
            out_brave = 4;
        }
    }
    return (out_brave);
}

struct Combat_Forecast Unit_Combat_Forecast(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos) {
    SDL_assert(attacker);
    SDL_assert(attacker_pos);
    SDL_assert(defender);
    SDL_assert(defender_pos);
    struct Combat_Forecast out_forecast;
    out_forecast.flow = Unit_Combat_Forecast_Flow(attacker, attacker_pos, defender, defender_pos);
    out_forecast.stats = Unit_Combat_Forecast_Stats(attacker, defender);
    return (out_forecast);
}

struct Combat_Forecast_Flow Unit_Combat_Forecast_Flow(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos) {
    SDL_assert(attacker);
    SDL_assert(attacker_pos);
    SDL_assert(defender);
    SDL_assert(defender_pos);
    struct Combat_Forecast_Flow out_flow;
    out_flow.defender_doubles = Unit_canDouble(defender, attacker);
    out_flow.attacker_doubles = Unit_canDouble(attacker, defender);
    out_flow.attacker_brave = Unit_brave(attacker);
    out_flow.defender_brave = Unit_brave(defender);
    out_flow.defender_retaliates = Unit_canAttack(attacker, attacker_pos, defender, defender_pos);
    return (out_flow);
}

struct Unit_stats Unit_getStats(struct Unit * in_unit) {
    struct Unit_stats out_stats = in_unit->current_stats;
    SDL_assert(in_unit);
    Unit_effectiveStats(in_unit);
    Unit_stats_plus(out_stats, in_unit->bonus_stats);
    Unit_stats_minus(out_stats, in_unit->malus_stats);
    return (out_stats);
}

struct Unit_stats Unit_effectiveStats(struct Unit * in_unit) {
    SDL_assert(in_unit);
    struct Unit_stats temp_ustats = Unit_stats_default;
    struct Weapon_stats temp_wstats = Weapon_stats_default;
    uint16_t temp_type;
    in_unit->bonus_stats = Unit_stats_default;
    in_unit->malus_stats = Unit_stats_default;
    in_unit->effective_stats = in_unit->current_stats;

    if (in_unit->equipped[UNIT_HAND_LEFT] > 0) {
        temp_ustats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).item->bonus_stats;
        Unit_stats_plus(in_unit->bonus_stats, temp_ustats);
        temp_ustats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_LEFT].id).item->malus_stats;
        Unit_stats_plus(in_unit->malus_stats, temp_ustats);
    }

    if (in_unit->equipped[UNIT_HAND_RIGHT] > 0) {
        temp_ustats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).item->bonus_stats;
        Unit_stats_plus(in_unit->bonus_stats, temp_ustats);
        temp_ustats = hmget(*in_unit->weapons, in_unit->_equipment[UNIT_HAND_RIGHT].id).item->malus_stats;
        Unit_stats_plus(in_unit->malus_stats, temp_ustats);
    }

    if (in_unit->mount_assigned != NULL) {
        in_unit->effective_stats.move = MOVE_WITH_MOUNT;
    }

    Unit_stats_plus(in_unit->effective_stats, in_unit->bonus_stats);
    Unit_stats_minus(in_unit->effective_stats, in_unit->malus_stats);
    SDL_Log("effective move of %s: %d", in_unit->name, in_unit->effective_stats.move);
    return (in_unit->effective_stats);
}

void Unit_promote(struct Unit * in_unit, int8_t new_class_index) {
    SDL_assert(in_unit);
    // struct Unit_stats promotion_bonus = hmget(promotion_bonus_stats, new_class_index);
    // arrput(in_unit->grown_stats, promotion_bonus);
    // Unit_stats_plus(in_unit->current_stats, promotion_bonus);

    // in_unit->skills += hmget(promotion_bonus_skills, new_class_index);

    // in_unit->class_index = new_class_index;
}