
#include "aura.h"
#include "utilities.h"
#include "cJSON.h"
#include "jsonio.h"
#include "unit/unit.h"
#include "unit/range.h"

void Aura_readJSON(void *input, const cJSON *jaura) {
    struct Aura *aura = input;

    struct cJSON *jUnit_stats       = cJSON_GetObjectItem(jaura, "Unit_stats");
    struct cJSON *jComputed_Stats   = cJSON_GetObjectItem(jaura, "Computed_Stats");
    struct cJSON *jRange            = cJSON_GetObjectItem(jaura, "Range");
    struct cJSON *jturns            = cJSON_GetObjectItem(jaura, "turns");

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
    const struct Aura *aura = input;

    struct cJSON *jturns            = cJSON_CreateNumber(aura->turns);
    struct cJSON *jrange            = cJSON_CreateArray();
    struct cJSON *junit_stats       = cJSON_CreateObject();
    struct cJSON *jcomputed_stats   = cJSON_CreateObject();

    Computed_Stats_writeJSON(&aura->computed_stats, jcomputed_stats);
    Unit_stats_writeJSON(&aura->unit_stats, junit_stats);
    Range_writeJSON(&aura->range, jrange);

    cJSON_AddItemToObject(jaura, "Computed_Stats", jcomputed_stats);
    cJSON_AddItemToObject(jaura, "Unit_stats", junit_stats);
    cJSON_AddItemToObject(jaura, "Range", jrange);
    cJSON_AddItemToObject(jaura, "turns", jturns);
}
