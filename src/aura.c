
#include "aura.h"

void Aura_readJSON( void *input, cJSON *jaura) {
    struct Aura *aura = input;

    struct cJSON *jUnit_stats       = cJSON_GetObjectItem(jaura, "Unit_stats");
    struct cJSON *jComputed_Stats   = cJSON_GetObjectItem(jaura, "Computed_Stats");
    struct cJSON *jRange            = cJSON_GetObjectItem(jaura, "Range");

    if (jUnit_stats != NULL)
        Unit_stats_readJSON(&aura->unit_stats, jUnit_stats);

    if (jComputed_Stats != NULL)
        Computed_Stats_readJSON(&aura->computed_stats, jComputed_Stats);

    if (jRange != NULL)
        Range_readJSON(&aura->computed_stats, jRange);
}

void Aura_writeJSON(void *input, cJSON *jaura) {

}
