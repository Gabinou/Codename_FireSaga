
#include "unit/party.h"

/* --- Party --- */
void Party_Free(struct Party *party) {
    SDL_assert(party != NULL);

    if (party->unit_names != NULL) {
        DARR_FREE(party->unit_names);
        for (int i = 0; i < DARR_NUM(party->unit_names); i++)
            s8_free(&party->unit_names[i]);
        party->unit_names = NULL;
    }

    if (party->filenames != NULL) {
        DARR_FREE(party->filenames);
        for (int i = 0; i < DARR_NUM(party->filenames); i++)
            s8_free(&party->filenames[i]);
        party->filenames = NULL;
    }

    if (party->ids != NULL) {
        DARR_FREE(party->ids);
        party->ids = NULL;
    }
}

/* --- Utilities --- */
void Party_Ids2Filenames(struct Party *party_struct) {
    SDL_assert(party_struct != NULL);
    SDL_assert(party_struct->ids);
    SDL_assert(party_struct->filenames);

    for (int i = 0; i < DARR_NUM(party_struct->ids); i++) {
        s8 filename     = s8_mut("units"PHYSFS_SEPARATOR);
        size_t order    = *(u16 *)DTAB_GET(global_unitOrders, party_struct->ids[i]);
        filename        = s8cat(filename, global_unitNames[order]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party_struct->filenames, filename);
    }
}

void Party_Names2Filenames(struct Party *party_struct) {
    SDL_assert(party_struct != NULL);
    SDL_assert(party_struct->unit_names);
    SDL_assert(party_struct->filenames);
    
    for (int i = 0; i < DARR_NUM(party_struct->unit_names); i++) {
        s8 filename     = s8_mut("units"PHYSFS_SEPARATOR);
        filename        = s8cat(filename, party_struct->unit_names[i]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party_struct->filenames, filename);
    }
}

/* --- JSONIO --- */

void Party_Load_Units(struct Party *party_struct, struct Unit *party,
                      struct dtab *wdtab, struct dtab *idtab) {

}

void _Party_Load_Units(struct Party *party_struct, struct Unit *party) {
    /* Read only units with party->filenames*/


}


void Party_readJSON(void *input,  cJSON *jparty) {
    struct Party *party_struct = (struct Party *)input;
    SDL_assert(party_struct != NULL);
    struct Unit *party = party_struct->party;
    SDL_assert(party != NULL);

    Party_Free(party_struct);

    party_struct->ids          = DARR_INIT(party_struct->ids, i16, 8);
    party_struct->filenames    = DARR_INIT(party_struct->filenames, s8, 8);
    party_struct->unit_names   = DARR_INIT(party_struct->unit_names, s8, 8);

    SDL_Log("-- Get json objects --");
    cJSON *jids         = cJSON_GetObjectItem(jparty, "ids");
    cJSON *jnames       = cJSON_GetObjectItem(jparty, "names");
    cJSON *jfilenames   = cJSON_GetObjectItem(jparty, "filenames");

    if (jids != NULL) {
        if (!cJSON_IsArray(jids)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "ids is not an array");
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jids);
        for (int i = 0; i < num; i++) {
            struct cJSON *jid = cJSON_GetArrayItem(jids, i);
            DARR_PUT(party_struct->ids, cJSON_GetNumberValue(jid));
        }
    }

    if (jnames != NULL) {
        if (!cJSON_IsArray(jnames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "names is not an array");
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jnames);
        for (int i = 0; i < num; i++) {
            struct cJSON *jname = cJSON_GetArrayItem(jnames, i);
            DARR_PUT(party_struct->unit_names, s8_mut(cJSON_GetStringValue(jname)));
        }
    }

    if (jfilenames != NULL) {
        if (!cJSON_IsArray(jfilenames)) {
            SDL_LogError(SOTA_LOG_SYSTEM, "filenames is not an array");
            exit(ERROR_JSONParsingFailed);
        }

        i32 num = cJSON_GetArraySize(jfilenames);
        for (int i = 0; i < num; i++) {
            struct cJSON *jfilename = cJSON_GetArrayItem(jfilenames, i);
            DARR_PUT(party_struct->filenames, s8_mut(cJSON_GetStringValue(jfilename)));
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
