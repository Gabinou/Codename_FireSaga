
#include "unit/party.h"

void Party_Free(struct Party *party) {
    SDL_assert(party != NULL);

    if (party->unit_names != NULL) {
        DARR_FREE(party->unit_names);
        for (int i = 0; i < DARR_NUM(party->unit_names); i++)
            s8_free(party->unit_names[i]);
        party->unit_names = NULL;
    }

    if (party->filenames != NULL) {
        DARR_FREE(party->filenames);
        for (int i = 0; i < DARR_NUM(party->filenames); i++)
            s8_free(party->filenames[i]);
        party->filenames = NULL;
    }

    if (party->ids != NULL) {
        DARR_FREE(party->ids);
        party->ids = NULL;
    }
}

void Party_readJSON(void *input,  cJSON *jparty) {
    struct Party *party_struct = (struct Party *)input;
    SDL_assert(party_struct != NULL);
    struct Unit *party = party_struct->party;
    SDL_assert(party != NULL);

    Party_Free(party);

    party->ids          = DARR_INIT(party->ids, i16, 8);
    party->filenames    = DARR_INIT(party->filenames, s8, 8);
    party->unit_names   = DARR_INIT(party->unit_names, s8, 8);

    SDL_Log("-- Get json objects --");
    cJSON *jids         = cJSON_GetObjectItem(junit, "ids");
    cJSON *jnames       = cJSON_GetObjectItem(junit, "names");
    cJSON *jfilenames   = cJSON_GetObjectItem(junit, "filenames");

    if (jids != NULL) {
        if (!cJSON_IsArray(jids)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' ids is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jids);
        for (int i = 0; i < num; i++) {
            struct cJSON *jid = cJSON_GetArrayItem(jids, i);
            DARR_PUT(party->ids, cJSON_GetNumberValue(jid));
        }
    }

    if (jnames != NULL) {
        if (!cJSON_IsArray(jnames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' names is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jnames);
        for (int i = 0; i < num; i++) {
            struct cJSON *jname = cJSON_GetArrayItem(jnames, i);
            DARR_PUT(party->unit_names, s8_mut(cJSON_GetStringValue(jname)));
        }
    }
 
    if (jfilenames != NULL) {
        if (!cJSON_IsArray(jfilenames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "'%s' filenames is not an array", json_name);
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jfilenames);
        for (int i = 0; i < num; i++) {
            struct cJSON *jfilename = cJSON_GetArrayItem(jfilenames, i);
            DARR_PUT(party->filenames, s8_mut(cJSON_GetStringValue(jfilename)));
        }
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
