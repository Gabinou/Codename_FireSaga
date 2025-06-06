
#include "unit/party.h"
#include "game/unit.h"
#include "game/game.h"
#include "names.h"
#include "cJSON.h"
#include "log.h"
#include "platform.h"
#include "jsonio.h"

const struct Party Party_default =  {
    .jsonio_header.json_element   = JSON_PARTY,
    .jsonio_header.json_filename  = {0},
    .size           = 0,
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
        // TODO: use link to world?
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
    SDL_assert(global_unitOrders        != NULL);
    SDL_assert(global_unitNames         != NULL);

    // TODO Reset DARR_NUM(party->json_ids) to 0?
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
void _Party_Load(tnecs_entity *entities, struct Game *sota,
                 s8 *filenames, size_t load_num) {
    for (size_t i = 0; i < load_num; i++) {
        /* Creating entity */
        tnecs_world *world = sota->ecs.world;
        tnecs_entity unit_ent = Game_Party_Entity_Create(sota);
        Unit *unit = IES_GET_COMPONENT(world, unit_ent, Unit);
        SDL_assert(unit->jsonio_header.json_filename.data == NULL);
        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);

        /* Reading party unit json */
        s8 filename = filenames[i];
        jsonio_readJSON(filename, unit);
        unit->id.army = ARMY_FRIENDLY;
        i16 id = Unit_id(unit);
        SDL_assert(id > UNIT_ID_PC_START);
        SDL_assert(id < UNIT_ID_PC_END);
        SDL_assert(global_unitNames[Unit_id(unit)].data != NULL);

        SDL_assert(global_unitNames[id].data != NULL);
        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);
        SDL_assert(unit->flags.handedness > UNIT_HAND_NULL);
        SDL_assert(unit->flags.handedness < UNIT_HAND_END);
        SDL_assert(unit->flags.mvt_type > UNIT_MVT_START);

        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);
        /* Putting unit in entities list */
        Game_Party_Entity_Init(sota, unit_ent);
        SDL_assert(sota->party.entities[id] == unit_ent);
    }
}

void Party_Load(struct Party *party, struct Game *sota) {
    SDL_assert(party != NULL);
    s8 *filenames = party->json_filenames;
    SDL_assert(filenames != NULL);
    SDL_assert(DARR_NUM(filenames) > 0);
    _Party_Load(party->entities, sota, filenames, DARR_NUM(filenames));
}

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
