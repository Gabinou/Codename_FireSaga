#ifndef AURA_H
#define AURA_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
typedef struct cJSON cJSON;

/* --- I/O --- */
void Aura_readJSON( void *input, cJSON *jaura);
void Aura_writeJSON(void *input, cJSON *jaura);

#endif /* AURA_H */