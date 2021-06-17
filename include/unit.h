#ifndef UNIT_H
#define UNIT_H

#include "filesystem.h"
#include "enums.h"
#include "narrative.h"
#include "weapon.h"
#include "linalg.h"
#include "jsonio.h"
#include "probability.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include <stdio.h>
#include <string.h>

// extern SDL_Renderer * Game_renderer;
struct Item;

//If a cavalier goes into agony, the horse dies? or also agonizes?

struct Mount {
    // Unaligned size 32 bytes
    // Aligned size 24 bytes
    // Horse provides bonus stats?
    // -> up to 7 mov.
    // +1 def

    // Horse provides malus stats?
    // -> -1 Skill?
    // -> -1 luck?
    char * name;
    int16_t * unit_bonds; // get +1hit/+1avoid bonus

    bool sex;
    bool mages_ride; // if no horse is scared of magic/mages.
    bool promoted;
    int16_t owner; // Only one that can ride
    int8_t owner_lvlups;
    int8_t carry_capacity; // con only?

    int8_t * ridable_classes; // 0 means all classes.
    uint16_t price;
    int8_t type;
};

extern struct Mount Mount_default_horse;
extern struct Mount Mount_default_pegasus;
extern struct Mount Mount_default_griffon;

typedef struct Unit {
    // Unaligned size 432 bytes
    // Aligned size 424 bytes
    uint8_t json_element; // has to be first
    int8_t class_index;
    int8_t mvt_type;
    int8_t army;
    uint8_t current_agony;
    uint8_t current_hp;
    int8_t handedness;
    uint16_t talkable;

    int_tile_t _range[2]; // 64 bits

    struct Weapon_stats temp_wpn; // 176 bits
    struct Weapon_stats right_wpn;
    struct Weapon_stats left_wpn;

    struct Unit_status_turns status_turns;

    struct Support supports[DEFAULT_SUPPORTS_MAX];
    struct Damage damage;

    struct Unit_stats base_stats; // 88 bitz
    struct Unit_stats bonus_stats;
    struct Unit_stats caps_stats;
    struct Unit_stats malus_stats;
    struct Unit_stats current_stats;
    struct Unit_stats effective_stats;
    struct Unit_stats growths;
    struct Unit_stats bonus_growths;
    struct Unit_stats effective_growths;
    struct Unit_stats * grown_stats;

    uint64_t skills;

    uint16_t equippable;
    uint16_t exp;
    uint16_t _id;
    uint16_t base_exp;

    bool divine_shield;
    bool is_alive;
    bool agonizes;
    bool _sex; // 0:F, 1:M. eg. hasPenis.
    bool literate; // Reading/writing for scribe job.
    bool courageous; // For reaction to story events
    bool show_danger;
    bool waits;

    bool hands[UNIT_HANDS_NUM]; // 16 bits
    int8_t equipped[UNIT_HANDS_NUM]; // 16 bits


    char * class_name;
    char * army_name;

    char * sex_name;
    char ** skill_names;

    struct Weapons_hash ** weapons;
    struct Inventory_item * _equipment; // 0 for right hand, 1 for left hand

    struct Mount * mount_assigned;

    char name[DEFAULT_BUFFER_SIZE];
    struct Computed_Stats computed_stats;
} Unit;
extern struct Unit Unit_default;

struct Units_hash {
    int16_t key;
    struct Unit value;
};

extern void Unit_init(struct Unit * in_unit);

extern void Unit_setid(struct Unit * in_unit, const int16_t in_id);
extern void Unit_setSex(struct Unit * in_unit, const bool in_sex);
extern void Unit_setClassind(struct Unit * in_unit, int8_t in_class_index);
extern void Unit_checkWeapon(struct Unit * in_unit, int16_t in_id);

extern void Unit_equips(struct Unit * in_unit, const bool hand);
extern void Unit_unequips(struct Unit * in_unit, const bool hand);

extern void Unit_Item_Add(struct Unit * in_unit, struct Inventory_item in_item);
extern void Unit_Item_Move(struct Unit * in_unit, const int16_t ind1, const int16_t ind2);
extern void Unit_Item_Remove(struct Unit * in_unit, int8_t in_index);
extern void Unit_Item_Take(struct Unit * in_unit, struct Inventory_item * out_array, const int16_t in_index, const int16_t out_index);
extern void Unit_Item_Give(struct Unit * in_unit, struct Inventory_item * out_array, const int16_t in_index, const int16_t out_index);
extern void Unit_Item_Drop(struct Unit * in_unit, const int16_t in_index);

extern void Unit_supportUp(struct Unit * in_unit, int16_t in_id);
extern void Unit_lvlUp(struct Unit * in_unit);
extern int16_t Unit_getLvl(struct Unit * in_unit);

extern void Unit_setStats(struct Unit * in_unit, const struct Unit_stats in_stats);
extern struct Unit_stats Unit_getStats(struct Unit * in_unit);
extern void Unit_takesDamage(struct Unit * in_unit, const uint8_t damage);
extern void Unit_getsHealed(struct Unit * in_unit, const uint8_t healing);

extern struct Damage Unit_computeDamage(struct Unit * attacker, struct Unit * defender);
extern int_tile_t * Unit_computeRange(struct Unit * in_unit);
extern uint8_t * Unit_computeAttack(struct Unit * in_unit);
extern uint8_t * Unit_computeDefense(struct Unit * in_unit);
extern uint8_t Unit_computeHit(struct Unit * in_unit);
extern int8_t Unit_computeDodge(struct Unit * in_unit);
extern uint8_t Unit_computeCritical(struct Unit * in_unit);
extern uint8_t Unit_computeFavor(struct Unit * in_unit);
extern uint8_t Unit_computeAgony(struct Unit * in_unit);
extern int8_t Unit_computeSpeed(struct Unit * in_unit);
extern int8_t Unit_computeMove(struct Unit * in_unit);

extern struct Unit_stats Unit_effectiveStats(struct Unit * in_unit);
extern struct Computed_Stats Unit_computedStats(struct Unit * in_unit);

extern void Unit_refresh(struct Unit * in_unit);
extern void Unit_wait(struct Unit * in_unit);
extern void Unit_dies(struct Unit * in_unit);
extern void Unit_agonizes(struct Unit * in_unit);

extern void Unit_readJSON(struct Unit * in_unit, cJSON * in_junit);
extern void Unit_writeJSON(struct Unit * in_unit, cJSON * in_junit);

extern bool Unit_iswpnEquipped(struct Unit * in_unit);
extern bool Unit_canEquip(struct Unit * in_unit, const int16_t in_id);
extern bool Unit_canDouble(struct Unit * attacker, struct Unit * defender);
extern uint8_t Unit_brave(struct Unit * in_unit);
extern bool Unit_canAttack(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos);

extern struct Combat_Forecast_Flow Unit_Combat_Forecast_Flow(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos);
extern struct Combat_Forecast_Rates Unit_Combat_Forecast_Rates(struct Unit * attacker, struct Unit * defender);
extern struct Combat_Forecast_Stats Unit_Combat_Forecast_Stats(struct Unit * attacker, struct Unit * defender);
extern struct Combat_Forecast Unit_Combat_Forecast(struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos);
extern struct Combat_Forecast_Death Unit_Combat_Forecast_Death(struct Combat_Forecast_Stats in_combat_stats, struct Combat_Forecast_Flow in_combat_flow);

extern struct Unit(**baseunitLoaders)();

#endif /* UNIT_H */
