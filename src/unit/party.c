
#include "unit/party.h"

struct Party Party_default =  {
    .json_element   = JSON_PARTY,
    .json_filename  = {0},
    .folder         = {0},
    .size           = 0,
    .names          = NULL,
    .filenames      = NULL,
    // .ids            = {0},
    .json_units     = {0},
    .entities       = {0},
    .id_stack       = NULL,
};

/* --- Party --- */
void Party_Free(struct Party *party) {
    SDL_assert(party != NULL);

    if (party->names != NULL) {
        for (int i = 0; i < DARR_NUM(party->names); i++)
            s8_free(&party->names[i]);
        DARR_FREE(party->names);
        party->names = NULL;
    }

    if (party->filenames != NULL) {
        for (int i = 0; i < DARR_NUM(party->filenames); i++)
            s8_free(&party->filenames[i]);
        DARR_FREE(party->filenames);
        party->filenames = NULL;
    }

    s8_free(&party->folder);
    s8_free(&party->json_filename);
}

void Party_Reset(struct Party *party) {
    /* Remove all filenames + all units to 0 */
    if (party->filenames != NULL) {
        for (int i = 0; i < DARR_NUM(party->filenames); i++)
            s8_free(&party->filenames[i]);
        DARR_NUM(party->filenames) = 0;
    }
    memset(party->json_units, 0, sizeof(*party->json_units) * SOTA_MAX_PARTY_SIZE);
}

/* --- Utilities --- */
i32 Party_Size(struct Party *ps)  {
    SDL_assert(ps                   != NULL);
    ps->size = _Party_Size(ps->json_units, ps->id_stack);
    return (ps->size);
}

i32 _Party_Size(struct Unit *units, i16 *id_stack)  {
    i32 num = 0;
    for (size_t i = 0; i < SOTA_MAX_PARTY_SIZE; i++) {
        if (units[i]._id > UNIT_ID_PC_START && units[i]._id < UNIT_ID_PC_END) {
            id_stack[num++] = units[i]._id;
        }
    }
    return (num);
}

void Party_Folder(struct Party *party, char *folder) {
    s8_free(&party->folder);
    party->folder = s8_mut(folder);
}

void Party_Ids2Filenames(struct Party *party) {
    SDL_assert(party != NULL);
    SDL_assert(party->filenames);
    SDL_assert(party->folder.data   != NULL);
    SDL_assert(global_unitOrders    != NULL);
    SDL_assert(global_unitNames     != NULL);

    for (int i = 0; i < DARR_NUM(party->ids); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        SDL_assert(party->ids[i] > UNIT_ID_PC_START);
        SDL_assert(party->ids[i] < UNIT_ID_END);
        u64 *order    = DTAB_GET(global_unitOrders, party->ids[i]);
        SDL_assert(order != NULL);
        SDL_assert(*order > 0);
        SDL_assert(*order < UNIT_NUM);
        filename        = s8cat(filename, global_unitNames[*order]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->filenames, filename);
    }
}

void Party_Names2Filenames(struct Party *party) {
    SDL_assert(party != NULL);
    SDL_assert(party->names);
    SDL_assert(party->filenames);
    SDL_assert(party->folder.data != NULL);

    for (int i = 0; i < DARR_NUM(party->names); i++) {
        s8 filename     = s8_mut(party->folder.data);
        filename        = s8cat(filename, s8_literal(PHYSFS_SEPARATOR));
        filename        = s8cat(filename, party->names[i]);
        filename        = s8cat(filename, s8_literal(".json"));
        DARR_PUT(party->filenames, filename);
    }
}

/* --- JSONIO --- */
void _Party_Load(struct Unit *party, struct dtab *weapons_dtab,
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
        party[temp_unit._id] = temp_unit;
    }
}

void Party_Load(struct Party *party,
                struct dtab *wdtab, struct dtab *idtab) {
    /* Load units in party)_struct->party, also add dtabs */
    struct Unit *units = party->json_units;
    SDL_assert(party != NULL);
    s8 *filenames = party->filenames;
    SDL_assert(filenames != NULL);

    _Party_Load(party->json_units, wdtab, idtab, filenames, DARR_NUM(filenames));
}

void Party_readJSON(void *input, cJSON *jparty) {
    struct Party *party = (struct Party *)input;
    SDL_assert(party != NULL);

    s8 folder = {0};
    if (party->folder.data != NULL)
        folder = s8cpy(folder, party->folder);

    Party_Free(party);
    party->folder = folder;

    party->names     = DARR_INIT(party->names,      s8, SOTA_MAX_PARTY_SIZE);
    party->filenames = DARR_INIT(party->filenames,  s8, SOTA_MAX_PARTY_SIZE);

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
            DARR_PUT(party->ids, cJSON_GetNumberValue(jid));
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
            DARR_PUT(party->names, s8_mut(cJSON_GetStringValue(jname)));
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
            DARR_PUT(party->filenames, s8_mut(cJSON_GetStringValue(jfilename)));
        }
    }
}

void Party_writeJSON(void *input, cJSON *jparty) {
    /* --- Write file with list of units --- */
    struct Party *party = (struct Party *)input;
    SDL_assert(party != NULL);
    struct Unit *party = party->party;
    SDL_assert(party != NULL);

    cJSON *jfilenames       = cJSON_CreateArray();

    i32 num = DARR_NUM(party->filenames);
    for (int i = 0; i < num; i++) {
        struct cJSON *jfilename = cJSON_CreateString(party->filenames[i].data);
        cJSON_AddItemToArray(jfilenames, jfilename);
    }

    cJSON_AddItemToObject(jparty, "filenames", jfilenames);
}

void Party_Units_writeJSON(void *input, cJSON *jparty) {
    /* --- Each individual unit as a separate json file --- */
    struct Party *party = (struct Party *)input;
    SDL_assert(party != NULL);
    struct Unit *party = party->party;
    SDL_assert(party != NULL);
}
