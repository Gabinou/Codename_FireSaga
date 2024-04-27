#ifndef AURA_H
#define AURA_H

#include "enums.h"
#include "cJSON.h"
#include "utilities.h"
#include "jsonio.h"
#include "structs.h"

/* --- I/O --- */
void Aura_readJSON( void *input, cJSON *jaura);
void Aura_writeJSON(void *input, cJSON *jaura);

#endif /* AURA_H */