#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H

#include "structs.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct cJSON;

typedef struct Reinforcement {
    struct Point position;
    i32 army;
    i32 boss_icon;
    // Reinforcement use filenames for clarity
    // Use IDs instead? nah. reinf filenames are the exception.
    s8  filename;
    s8  ai_filename;
    i32 turn;
    i32 levelups;
    /* Loaded unit entity */
    tnecs_entity entity;
} Reinforcement;
extern const struct Reinforcement Reinforcement_default;

void Reinforcement_Free(struct Reinforcement  *a);
void Reinforcement_readJSON( struct cJSON *j, struct Reinforcement  *a);
void Reinforcement_writeJSON(struct cJSON *j, struct Reinforcement  *a);

/* -- Reinforcements -- */
void Unit_Reinforcement_Levelups(struct Unit *u, struct Reinforcement *r);

#endif /* Filesystem_H */
