#ifndef AURA_H
#define AURA_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Aura: boost unit stats when in range
**
*/

#include "enums.h"
#include "cJSON.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;

typedef struct Aura {
    struct Range            range; /* [0]: min, [1]: max */
    struct Unit_stats       unit_stats;
    struct Computed_Stats   computed_stats;
    i32 turns;
} Aura;
extern const struct Aura Aura_default;

/* --- I/O --- */
void Aura_readJSON( void *input, const cJSON *jaura);
void Aura_writeJSON(const void *input, cJSON *jaura);

#endif /* AURA_H */