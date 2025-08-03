
#include "skills.h"
#include "RNG.h"
#include "unit/unit.h"
#include "unit/equipment.h"
#include "unit/loadout.h"

void Skill_SPRINT(struct Unit *actor, void *data1) {
}

void Skill_LUNGE(struct Unit *actor, void *data1) {
}

void Skill_CHARGE(struct Unit *actor, void *data1) {
}

void Skill_SCOUTING(struct Unit *actor, void *data1) {
}

void Skill_DIVINE_SHIELD(struct Unit *actor, void *data1) {
}

void Skill_SWITCH(struct Unit *actor, void *patient) {
}

void Skill_DANCE(struct Unit *actor, void *patient) {
}

void Skill_ROCK_SHIELD(struct Unit *actor, void *data1) {
    // TODO create entity
    // tnecs_entity entity  = IES_E_CREATE_wC(gl_world, Timer_ID);

    // struct Inventory_item rock_shield = Inventory_item_default;
    // if (RNG_URN() < ROCK_SHIELD_BOULDER_PROB)
    //     rock_shield.id = ITEM_ID_OBSIDIAN_SLAB;
    // else
    //     rock_shield.id = ITEM_ID_BOULDER;
    // Unit_Item_Take(actor, rock_shield);
}

void Skill_CLEAVE(struct Unit *aggressor, void *in_defendant) {
    struct Unit *defendant = (struct Unit *)in_defendant;
    /* TODO: apply fear */
    Unit_dies(defendant);
}

void Skill_DISMEMBER(struct Unit *aggressor, void *in_defendant) {
    struct Unit *defendant = in_defendant;
    Unit_Hand_Set(defendant, Unit_Hand_Strong(defendant), false);
}

void Skill_TUNNELING(struct Unit *actor, void *pos) {

}

void Skill_ASSASSINATE(struct Unit *killer, void *victim) {
    struct Unit *defendant = (struct Unit *)victim;
    Unit_dies(defendant);
}

/* Infuse: give non-magic weapons additional magic damage */
void Skill_INFUSE(struct Unit *actor, void *in_item) {
    // TODO: implement for Infusion component

    // struct Inventory_item *item = (struct Inventory_item *)in_item;
    // SDL_assert(item->infusion <= SOTA_INFUSEABLE);
    // struct Unit_stats stats = Unit_effectiveStats(actor);
    // item->infusion = stats.mag / SOTA_INFUSE_FACTOR;
}

void Skill_VENOMOUS_SPIT(struct Unit *actor, void *data1) {
}

void Skill_STEAL(struct Unit *actor, void *data1) {
}

void Skill_PICK(struct Unit *actor, void *tile_data) {
    // struct Tile *tile = (struct Tile *) tile_data;
    /* TODO: Check pick in hand */
    /* TODO: make it use one time. */
    // actor->
    // SDL_free(tile->door);
    // tile->door = NULL;
}

Active_Skill global_active_skills[ACTIVE_SKILL_NUM] = {0};
void Make_Active_Skill_Array(void) {
#define REGISTER_ENUM(x) global_active_skills[ACTIVE_SKILL_##x] = Skill_##x;
#include "names/skills_active.h"
#undef REGISTER_ENUM
}
