
#include "reinforcement.h"
#include "unit/boss.h"
#include "nmath.h"
#include "names.h"
#include "unit/unit.h"
#include "unit/stats.h"
#include "unit/flags.h"
#include "cJSON.h"

const Reinforcement Reinforcement_default = {
    .boss_icon      = -1,
};

void Reinforcement_Free(Reinforcement *reinf) {
    s8_free(&reinf->filename);
}

void Reinforcement_readJSON(cJSON      *_jreinf,
                            Reinforcement *reinf) {
    SDL_assert(reinf  != NULL);
    SDL_assert(_jreinf != NULL);
    struct cJSON *jai       = cJSON_GetObjectItem(_jreinf,      "AI");
    struct cJSON *jboss     = cJSON_GetObjectItem(_jreinf,      "Boss");
    struct cJSON *jarmy     = cJSON_GetObjectItem(_jreinf,      "army");
    struct cJSON *jturn     = cJSON_GetObjectItem(_jreinf,      "turn");
    struct cJSON *jlevelups = cJSON_GetObjectItem(_jreinf,      "levelups");
    struct cJSON *jposition = cJSON_GetObjectItem(_jreinf,      "Position");
    struct cJSON *jfilename = cJSON_GetObjectItem(_jreinf,      "filename");

    SDL_assert(jposition != NULL);
    struct cJSON *jrow      = cJSON_GetObjectItem(jposition,    "row");
    struct cJSON *jcol      = cJSON_GetObjectItem(jposition,    "col");

    reinf->filename     = s8_mut(cJSON_GetStringValue(jfilename));
    s8 ai_filename      = s8_mut(cJSON_GetStringValue(jai));
    reinf->ai_id        = AI_Name2ID(ai_filename);
    s8_free(&ai_filename);
    reinf->army         = cJSON_GetNumberValue(jarmy);

    SDL_assert(reinf->army > ARMY_START);
    SDL_assert(reinf->army < ARMY_END);
    reinf->turn         = cJSON_GetNumberValue(jturn);
    reinf->levelups     = cJSON_GetNumberValue(jlevelups);
    reinf->position.x   = cJSON_GetNumberValue(jcol);
    reinf->position.y   = cJSON_GetNumberValue(jrow);
    reinf->position.y   = cJSON_GetNumberValue(jrow);

    /* Is reinf a boss? */
    if (jboss == NULL)
        return;

    /* Read boss icon from army */
    reinf->boss_icon = army_icons[reinf->army];

    struct cJSON *jicon = cJSON_GetObjectItem(jboss, "icon");
    if (jicon == NULL)
        return;

    /* Override army icon with icon tag in boss object */
    reinf->boss_icon = cJSON_GetNumberValue(jicon);
}

void Reinforcement_writeJSON(struct cJSON         *jreinf,
                             struct Reinforcement *reinf) {
    SDL_assert(jreinf != NULL);
    s8 ai_filename          = ai_names[reinf->ai_id];
    struct cJSON *jai       = cJSON_CreateString(ai_filename.data);
    struct cJSON *jcol      = cJSON_CreateNumber(reinf->position.x);
    struct cJSON *jrow      = cJSON_CreateNumber(reinf->position.y);
    struct cJSON *jturn     = cJSON_CreateNumber(reinf->turn);
    struct cJSON *jarmy     = cJSON_CreateNumber(reinf->army);
    struct cJSON *jfilename = cJSON_CreateString(reinf->filename.data);
    struct cJSON *jlevelups = cJSON_CreateNumber(reinf->levelups);
    struct cJSON *jposition = cJSON_CreateObject();
    cJSON_AddItemToObject(jposition,    "row",      jrow);
    cJSON_AddItemToObject(jposition,    "col",      jcol);
    cJSON_AddItemToObject(jreinf,       "filename", jfilename);
    cJSON_AddItemToObject(jreinf,       "AI",       jai);
    cJSON_AddItemToObject(jreinf,       "army",     jarmy);
    cJSON_AddItemToObject(jreinf,       "levelups", jlevelups);
    cJSON_AddItemToObject(jreinf,       "turn",     jturn);
    cJSON_AddItemToObject(jreinf,       "Position", jposition);
}

/* AI/NPC units only */
void Unit_Reinforcement_Levelups(struct Unit *unit, struct Reinforcement *reinf) {
    struct Unit_stats *grown = Unit_Stats_Grown(unit);
    SDL_assert(grown != NULL);

    /* Skip if unit was already leveled */
    if (DARR_NUM(grown) == reinf->levelups)
        return;

    for (int i = 0; i < reinf->levelups; i++) {
        unit->level.exp += SOTA_EXP_PER_LEVEL;
        Unit_lvlUp(unit);
    }
}
