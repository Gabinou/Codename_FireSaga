#ifndef SKILLS_H
#define SKILLS_H

#include "enums.h"

/* --- FORWARD DECLARATIONS --- */
struct Inventory_Item;
struct Unit;

/* --- TYPEDEFS --- */
typedef void (*Active_Skill)(struct Unit *, void *);

/* --- ANTS --- */
extern Active_Skill global_active_skills[ACTIVE_SKILL_NUM];

/* --- FUNCTIONS --- */
void Make_Active_Skill_Array(void);

#define REGISTER_ENUM(x) void Skill_##x(struct Unit * actor, void * data1);
#include "names/skills_active.h"
#undef REGISTER_ENUM

#endif /* SKILL_H */
