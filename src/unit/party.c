
#include "unit/party.h"

void Party_readJSON(void *input,  cJSON *jparty) {
    struct Party *party_struct = (struct Party *)input;
    SDL_assert(party_struct != NULL);
    struct Unit *party = party_struct->party;
    SDL_assert(party != NULL);

    SDL_Log("-- Get json objects --");
    cJSON *jids         = cJSON_GetObjectItem(junit, "ids");
    cJSON *jnames       = cJSON_GetObjectItem(junit, "names");
    cJSON *jfilenames   = cJSON_GetObjectItem(junit, "filenames");

    if (jids != NULL) {
        if (!cJSON_IsArray(jids)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' ids is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num_ids = cJSON_GetArraySize(jids);
    }

    if (jnames != NULL) {
        if (!cJSON_IsArray(jnames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' names is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num_ids = cJSON_GetArraySize(jnames);
    }
 
    if (jfilenames != NULL) {
        if (!cJSON_IsArray(jfilenames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' filenames is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num_ids = cJSON_GetArraySize(jfilenames);
    }
    
}

void Party_writeJSON(void *input, cJSON *jparty) {
    /* --- Write file with list of units --- */
    struct Party *party_struct = (struct Party *)input;
    SDL_assert(party_struct != NULL);
    struct Unit *party = party_struct->party;
    SDL_assert(party != NULL);

}

void Party_Units_writeJSON(void *input, cJSON *jparty) {
    /* --- Each individual unit as a separate json file --- */
    struct Party *party_struct = (struct Party *)input;
    SDL_assert(party_struct != NULL);
    struct Unit *party = party_struct->party;
    SDL_assert(party != NULL);

}
