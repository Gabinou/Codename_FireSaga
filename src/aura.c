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

#include "aura.h"
#include "cJSON.h"
#include "jsonio.h"
#include "utilities.h"

#include "unit/unit.h"
#include "unit/range.h"

const struct Aura Aura_default = {0};

void Aura_readJSON(void *input, const cJSON *jaura) {
    Aura *aura = input;

    cJSON *jUnit_stats       = cJSON_GetObjectItem(jaura, "Unit_stats");
    cJSON *jComputed_Stats   = cJSON_GetObjectItem(jaura, "Computed_Stats");
    cJSON *jRange            = cJSON_GetObjectItem(jaura, "Range");
    cJSON *jturns            = cJSON_GetObjectItem(jaura, "turns");

    if (jUnit_stats != NULL)
        Unit_stats_readJSON(&aura->unit_stats, jUnit_stats);

    if (jComputed_Stats != NULL)
        Computed_Stats_readJSON(&aura->computed_stats, jComputed_Stats);

    if (jRange != NULL)
        Range_readJSON(&aura->range, jRange);

    if (jturns != NULL)
        aura->turns = cJSON_GetNumberValue(jturns);

}

void Aura_writeJSON(const void *input, cJSON *jaura) {
    const Aura *aura = input;

    cJSON *jturns            = cJSON_CreateNumber(aura->turns);
    cJSON *jrange            = cJSON_CreateArray();
    cJSON *junit_stats       = cJSON_CreateObject();
    cJSON *jcomputed_stats   = cJSON_CreateObject();

    Computed_Stats_writeJSON(&aura->computed_stats, jcomputed_stats);
    Unit_stats_writeJSON(&aura->unit_stats, junit_stats);
    Range_writeJSON(&aura->range, jrange);

    cJSON_AddItemToObject(jaura, "Computed_Stats", jcomputed_stats);
    cJSON_AddItemToObject(jaura, "Unit_stats", junit_stats);
    cJSON_AddItemToObject(jaura, "Range", jrange);
    cJSON_AddItemToObject(jaura, "turns", jturns);
}
