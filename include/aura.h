#ifndef AURA_H
#define AURA_H

#include "enums.h"
#include "cJSON.h"
#include "utilities.h"
#include "structs.h"

/* --- I/O --- */
void Aura_readJSON( struct Aura *aura, cJSON *jaura);
void Aura_writeJSON(struct Aura *aura, cJSON *jaura);

#endif /* AURA_H */