#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H

#include "cJSON.h"
#include "position.h"
#include "nstr.h"

struct Reinforcement {
    struct Point position;
    i16 army;
    s8 filename;
    s8 ai_filename; /* Overrides ai_filename in unit */
    u8 turn;
    u8 levelups;
    // TODO: 1 or 2 
    // 1- Use unit filename instead of id
    //      - Gets rid of name from file vs name from id conflict
    //      - Gets rid of name id in file vs id in reinforcement issue
    // 2- Make ai id and use it instead of ai_filename
    //      - Have to free strings
};
extern struct Reinforcement Reinforcement_default;

void Reinforcement_Free(struct Reinforcement  *a);
void jsonio_Read_Reinforcement( struct cJSON *j, struct Reinforcement  *a);
void jsonio_Write_Reinforcement(struct cJSON *j, struct Reinforcement  *a);

#endif /* Filesystem_H */
