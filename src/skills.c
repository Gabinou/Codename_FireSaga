#include "skills.h"

void Skill_SPRINT(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_LUNGE(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_CHARGE(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_SCOUTING(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_DIVINE_SHIELD(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_SWITCH(struct Unit *actor, void *patient) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_DANCE(struct Unit *actor, void *patient) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_ROCK_SHIELD(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Inventory_item rock_shield = Inventory_item_default;
    if (RNG_URN(tinyMT_global) < ROCK_SHIELD_BOULDER_PROB)
        rock_shield.id = ITEM_ID_OBSIDIAN_SLAB;
    else
        rock_shield.id = ITEM_ID_BOULDER;
    Unit_Item_Take(actor, rock_shield);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_CLEAVE(struct Unit *aggressor, void *in_defendant) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *defendant = (struct Unit *)in_defendant;
    /* TODO: apply fear */
    Unit_dies(defendant);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_DISMEMBER(struct Unit *aggressor, void *in_defendant) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *defendant = in_defendant;
    defendant->hands[UNIT_HAND_STRONG] = false;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_TUNNELING(struct Unit *actor, void *pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_ASSASSINATE(struct Unit *killer, void *victim) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *defendant = (struct Unit *)victim;
    Unit_dies(defendant);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* Infuse: give non-magic weapons additional magic damage */
void Skill_INFUSE(struct Unit *actor, void *in_item) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Inventory_item *item = (struct Inventory_item *)in_item;
    SDL_assert(item->infusion <= SOTA_INFUSEABLE);
    struct Unit_stats stats = Unit_effectiveStats(actor);
    item->infusion = stats.mag / SOTA_INFUSE_FACTOR;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_VENOMOUS_SPIT(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_STEAL(struct Unit *actor, void *data1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Skill_PICK(struct Unit *actor, void *tile_data) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Tile *tile = (struct Tile *) tile_data;
    /* TODO: Check pick in hand */
    /* TODO: make it use one time. */
    // actor->
    // free(tile->door);
    // tile->door = NULL;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

Active_Skill global_active_skills[ACTIVE_SKILL_NUM] = {0};
void Make_Active_Skill_Array() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
#define REGISTER_ENUM(x) global_active_skills[ACTIVE_SKILL_##x] = Skill_##x;
#include "names/skills_active.h"
#undef REGISTER_ENUM
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
