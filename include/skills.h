#ifndef SKILLS_H
#define SKILLS_H

#include "structs.h"
#include "nmath.h"
#include "unit.h"
#include "RNG.h"
#include "item.h"

/* --- FORWARD DECLARATIONS --- */
struct Inventory_Item;

/* --- TYPEDEFS --- */
typedef void (*Active_Skill)(struct Unit *, void *);

/* --- CONSTANTS --- */
extern Active_Skill global_active_skills[ACTIVE_SKILL_NUM];

/* --- FUNCTIONS --- */
extern void Make_Active_Skill_Array();

#define REGISTER_ENUM(x) extern void Skill_##x(struct Unit * actor, void * data1);
#include "names/skills_active.h"
#undef REGISTER_ENUM

#endif /* SKILL_H */
