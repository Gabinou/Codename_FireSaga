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
** Party (array of player controlled units) handling
*/

#include "unit/party.h"
#include "game/unit.h"
#include "game/game.h"
#include "names.h"
#include "cJSON.h"
#include "sprite.h"
#include "log.h"
#include "platform.h"
#include "globals.h"
#include "jsonio.h"

const struct Party Party_default =  {
    .jsonio_header.json_element   = JSON_PARTY,
};

/* --- Party --- */
void Party_Free(struct Party *party) {
    SDL_assert(party != NULL);

    if (party->json_names != NULL) {
        for (int i = 0; i < DARR_NUM(party->json_names); i++)
            s8_free(&party->json_names[i]);
        DARR_FREE(party->json_names);
        party->json_names = NULL;
    }

    if (party->json_filenames != NULL) {
        for (int i = 0; i < DARR_NUM(party->json_filenames); i++)
            s8_free(&party->json_filenames[i]);
        DARR_FREE(party->json_filenames);
        party->json_filenames = NULL;
    }

    if (party->json_ids != NULL) {
        DARR_FREE(party->json_ids);
        party->json_ids = NULL;
    }

    if (party->id_stack != NULL) {
        DARR_FREE(party->id_stack);
        party->id_stack = NULL;
    }

    s8_free(&party->folder);
    s8_free(&party->load_filename);
    s8_free(&party->save_filename);
    s8_free(&party->jsonio_header.json_filename);
}

void Party_Init( struct Party *party) {
    SDL_assert(party != NULL);
    Party_Free(party);
    *party = Party_default;
    Party_Members_Alloc( party);
}

void Party_Members_Alloc( struct Party *party) {
    SDL_assert(party != NULL);
    if (party->json_names        == NULL) {
        party->json_names       = DARR_INIT(party->json_names,      s8,     SOTA_MAX_PARTY_SIZE);
    }

    if (party->json_filenames    == NULL) {
        party->json_filenames   = DARR_INIT(party->json_filenames,  s8,     SOTA_MAX_PARTY_SIZE);
    }

    if (party->json_ids          == NULL) {
        party->json_ids         = DARR_INIT(party->json_ids,        i16,    SOTA_MAX_PARTY_SIZE);
    }

    if (party->id_stack          == NULL) {
        party->id_stack         = DARR_INIT(party->id_stack,        i16,    SOTA_MAX_PARTY_SIZE);
    }
}

void Party_Reset(struct Party *party) {
    if (party->json_filenames == NULL)
        return;

    for (int i = 0; i < DARR_NUM(party->json_filenames); i++)
        s8_free(&party->json_filenames[i]);
    DARR_NUM(party->json_filenames) = 0;
    DARR_NUM(party->id_stack)       = 0;
}

/* --- Utilities --- */
i32 Party_Size(struct Party *ps)  {
    SDL_assert(ps != NULL);
    ps->size = 0;
    for (size_t i = UNIT_ID_START + 1; i < SOTA_MAX_PARTY_SIZE; i++) {
        if (ps->entities[i] > TNECS_NULL) {
            ps->size++;
        }
    }
    return (ps->size);
}

void Party_ID_Stack(struct Party *party)  {
    SDL_assert(party->id_stack != NULL);
    /* Stack IDs in order of entities */
    /* TODO use order of usage */
    DARR_NUM(party->id_stack) = 0;
    for (size_t i = UNIT_ID_START + 1; i < SOTA_MAX_PARTY_SIZE; i++) {
        if (party->entities[i] > TNECS_NULL) {
            DARR_PUT(party->id_stack, i);
        }
    }
}

void Party_Folder(struct Party *party, char *folder) {
    s8_free(&party->folder);
    party->folder = s8_mut(folder);
}

void Party_Ids2Filenames(struct Party *party) {
    SDL_assert(party != NULL);
    SDL_assert(party->json_filenames    != NULL);
    SDL_assert(party->folder.data       != NULL);

    // TODO Reset DARR_NUM(party->json_ids) to 0?
    for (int i = 0; i < DARR_NUM(party->json_ids); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        i32 id = party->json_ids[i];
        SDL_assert(id > UNIT_ID_PC_START);
        SDL_assert(id < UNIT_ID_END);
        filename        = s8cat(filename, _Unit_Name_id(id));
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->json_filenames, filename);
    }
}

void Party_Names2Filenames(struct Party *party) {
    SDL_assert(party != NULL);
    SDL_assert(party->json_names);
    SDL_assert(party->json_filenames);
    SDL_assert(party->folder.data != NULL);

    // TODO Reset DARR_NUM(party->json_names) to 0?
    for (int i = 0; i < DARR_NUM(party->json_names); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        filename        = s8cat(filename, party->json_names[i]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->json_filenames, filename);
    }
}

/* --- JSONIO --- */
void Party_readJSON(void *input, const cJSON *jparty) {
    struct Party *party = (struct Party *)input;
    SDL_assert(party                    != NULL);
    SDL_assert(party->json_ids          != NULL);
    SDL_assert(party->json_names        != NULL);
    SDL_assert(party->json_filenames    != NULL);

    // SDL_Log("-- Get json objects --");
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
            DARR_PUT(party->json_ids, cJSON_GetNumberValue(jid));
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
            DARR_PUT(party->json_names, s8_mut(cJSON_GetStringValue(jname)));
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
            DARR_PUT(party->json_filenames, s8_mut(cJSON_GetStringValue(jfilename)));
        }
    }
}

void Party_writeJSON(const void *input, cJSON *jparty) {
    /* --- Write list of units to jparty --- */
    struct Party *party = (struct Party *)input;
    SDL_assert(party != NULL);

    cJSON *jfilenames       = cJSON_CreateArray();

    i32 num = DARR_NUM(party->json_filenames);
    for (int i = 0; i < num; i++) {
        struct cJSON *jfilename = cJSON_CreateString(party->json_filenames[i].data);
        cJSON_AddItemToArray(jfilenames, jfilename);
    }

    cJSON_AddItemToObject(jparty, "filenames", jfilenames);
}
