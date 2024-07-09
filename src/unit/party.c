
#include "unit/party.h"

struct Party Party_default =  {
    .json_element   = JSON_PARTY,
    .size           = 0,
    .json_filename  = {0},
    .folder         = {0},
    .entities       = {0},
    .json_names     = NULL,
    .json_filenames = NULL,
    .json_ids       = NULL,
    .id_stack       = NULL,
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
    s8_free(&party->json_filename);
}
void Party_Init( struct Party *party) {
    SDL_assert(party->json_names        == NULL);
    SDL_assert(party->json_filenames    == NULL);
    SDL_assert(party->json_ids          == NULL);
    SDL_assert(party->id_stack          == NULL);
    party->json_ids         = DARR_INIT(party->json_ids,        i16,    SOTA_MAX_PARTY_SIZE);
    party->id_stack         = DARR_INIT(party->id_stack,        i16,    SOTA_MAX_PARTY_SIZE);
    party->json_names       = DARR_INIT(party->json_names,      s8,     SOTA_MAX_PARTY_SIZE);
    party->json_filenames   = DARR_INIT(party->json_filenames,  s8,     SOTA_MAX_PARTY_SIZE);
}

void Party_Reset(struct Party *party) {
    /* Remove all filenames + all units to 0 */
    if (party->json_filenames != NULL) {
        for (int i = 0; i < DARR_NUM(party->json_filenames); i++)
            s8_free(&party->json_filenames[i]);
        DARR_NUM(party->json_filenames) = 0;
    }
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
    SDL_assert(party->json_filenames);
    SDL_assert(party->folder.data   != NULL);
    SDL_assert(global_unitOrders    != NULL);
    SDL_assert(global_unitNames     != NULL);

    for (int i = 0; i < DARR_NUM(party->json_ids); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        SDL_assert(party->json_ids[i] > UNIT_ID_PC_START);
        SDL_assert(party->json_ids[i] < UNIT_ID_END);
        u64 *order    = DTAB_GET(global_unitOrders, party->json_ids[i]);
        SDL_assert(order != NULL);
        SDL_assert(*order > 0);
        SDL_assert(*order < UNIT_NUM);
        filename        = s8cat(filename, global_unitNames[*order]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->json_filenames, filename);
    }
}

void Party_Names2Filenames(struct Party *party) {
    SDL_assert(party != NULL);
    SDL_assert(party->json_names);
    SDL_assert(party->json_filenames);
    SDL_assert(party->folder.data != NULL);

    for (int i = 0; i < DARR_NUM(party->json_names); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        filename        = s8cat(filename, party->json_names[i]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->json_filenames, filename);
    }
}

/* --- JSONIO --- */
void _Party_Load(tnecs_entity *entities, struct Game *sota, struct dtab *weapons_dtab,
                 struct dtab *items_dtab, s8 *filenames, size_t load_num) {
    struct Unit temp_unit;
    for (size_t i = 0; i < load_num; i++) {
        s8 filename = filenames[i];
        temp_unit               = Unit_default;
        SDL_assert(temp_unit.json_filename.data == NULL);
        temp_unit.items_dtab    = items_dtab;
        temp_unit.weapons_dtab  = weapons_dtab;

        jsonio_readJSON(filename, &temp_unit);
        temp_unit.army = ARMY_FRIENDLY;
        SDL_assert(temp_unit.name.data != NULL);
        SDL_assert(temp_unit.handedness > UNIT_HAND_NULL);
        SDL_assert(temp_unit.handedness < UNIT_HAND_END);
        SDL_assert(temp_unit.mvt_type > UNIT_MVT_START);

        SDL_assert(temp_unit._id > UNIT_ID_PC_START);
        SDL_assert(temp_unit._id < UNIT_ID_PC_END);

        struct Point pos = {0, 0};
        entities[temp_unit._id] = Game_Party_Entity_Create(sota, temp_unit._id, pos, &temp_unit);
    }
}

void Party_Load(struct Party *party, struct Game *sota,
                struct dtab *wdtab, struct dtab *idtab) {
    /* Load units in party)_struct->party, also add dtabs */
    SDL_assert(party != NULL);
    s8 *filenames = party->json_filenames;
    SDL_assert(filenames != NULL);
    SDL_assert(DARR_NUM(filenames) > 0);
    _Party_Load(party->entities, sota, wdtab, idtab, filenames, DARR_NUM(filenames));
}

void Party_readJSON(void *input, cJSON *jparty) {
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

void Party_writeJSON(void *input, cJSON *jparty) {
    /* --- Write file with list of units --- */
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

void Party_Units_writeJSON(void *input, cJSON *jparty) {
    /* --- Each individual unit as a separate json file --- */
    struct Party *party = (struct Party *)input;
    SDL_assert(party != NULL);
}
