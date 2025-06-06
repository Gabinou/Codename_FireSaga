#ifndef AURA_H
#define AURA_H

#include "enums.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;

/* --- I/O --- */
void Aura_readJSON( void *input, const cJSON *jaura);
void Aura_writeJSON(const void *input, cJSON *jaura);

#endif /* AURA_H */