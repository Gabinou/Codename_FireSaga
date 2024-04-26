#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H

#include "cJSON.h"
#include "unit/unit.h"
#include "position.h"
#include "nstr.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;

struct Reinforcement {
    struct Point position;
    i16 army;
    i8  boss_icon;
    s8  filename;
    s8  ai_filename; /* Overrides ai_filename in unit */
    u8  turn;
    u8  levelups;
};
extern struct Reinforcement Reinforcement_default;

void Reinforcement_Free(struct Reinforcement  *a);
void Reinforcement_readJSON( struct cJSON *j, struct Reinforcement  *a);
void Reinforcement_writeJSON(struct cJSON *j, struct Reinforcement  *a);

/* -- Reinforcements -- */
void Unit_Reinforcement_Levelups(struct Unit *u, struct Reinforcement *r);

#endif /* Filesystem_H */
