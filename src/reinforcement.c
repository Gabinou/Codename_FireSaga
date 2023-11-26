
#include "reinforcement.h"

struct Reinforcement Reinforcement_default = {
    .ai_filename    =  {0},
    .filename      =  {0},
    .turn           =  0,
    .levelups       =  0,
    .army           = -1,
    .position       = {0, 0},
};

void Reinforcement_Free(struct Reinforcement  *reinf) {
    s8_free(&reinf->filename);
    s8_free(&reinf->ai_filename);
}

void jsonio_Read_Reinforcement(struct cJSON         *_jreinf,
                               struct Reinforcement *reinf) {
    SDL_assert(reinf  != NULL);
    SDL_assert(_jreinf != NULL);
    struct cJSON *jai       = cJSON_GetObjectItem(_jreinf,      "AI");
    struct cJSON *jarmy     = cJSON_GetObjectItem(_jreinf,      "army");
    struct cJSON *jturn     = cJSON_GetObjectItem(_jreinf,      "turn");
    struct cJSON *jlevelups = cJSON_GetObjectItem(_jreinf,      "levelups");
    struct cJSON *jposition = cJSON_GetObjectItem(_jreinf,      "Position");
    struct cJSON *jfilename = cJSON_GetObjectItem(_jreinf,      "filename");

    struct cJSON *jrow      = cJSON_GetObjectItem(jposition,    "row");
    struct cJSON *jcol      = cJSON_GetObjectItem(jposition,    "col");
    
    reinf->filename     = s8_mut(cJSON_GetStringValue(jfilename));
    reinf->ai_filename  = s8_mut(cJSON_GetStringValue(jai));
    reinf->army         = cJSON_GetNumberValue(jarmy);
    reinf->turn         = cJSON_GetNumberValue(jturn);
    reinf->levelups     = cJSON_GetNumberValue(jlevelups);
    reinf->position.x   = cJSON_GetNumberValue(jrow);
    reinf->position.y   = cJSON_GetNumberValue(jcol);
}

void jsonio_Write_Reinforcement(      struct cJSON                  *jreinf,
                                struct Reinforcement *reinf) {
    SDL_assert(jreinf != NULL);
    struct cJSON *jai       = cJSON_CreateString(reinf->ai_filename.data);
    struct cJSON *jrow      = cJSON_CreateNumber(reinf->position.x);
    struct cJSON *jcol      = cJSON_CreateNumber(reinf->position.y);
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
