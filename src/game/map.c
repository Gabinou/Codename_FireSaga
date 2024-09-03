#include "game/map.h"

#define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
void Game_Map_Load(struct Game *sota, i16 in_map_index) {
    SDL_assert((in_map_index > CHAPTER_START) && (in_map_index < CHAPTER_END));
    SDL_LogDebug(SOTA_LOG_SYSTEM, "%ld \n", in_map_index);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Associated map filename     %s \n", mapFilenames[in_map_index].data);

    /* --- PRELIMINARIES --- */
    Game_Map_Free(sota);

    /* --- Allocating map --- */
    sota->map = Map_Init(sota->map, sota->settings.tilesize[0], sota->settings.tilesize[1]);
    SDL_assert(sota->world != NULL);
    sota->map->world        = sota->world;
    sota->map->stack_mode   = sota->settings.map_settings.stack_mode;
    Map_Renderer_Set(sota->map, sota->renderer);

    /* --- Reading map from json files --- */
    // Map_dArrays_Init(sota->map, &sota->settings);
    jsonio_readJSON(mapFilenames[in_map_index], sota->map);
    // read_json sets the row_len and col_len necessary for dArrays_Init

    /* Set initial camera position */
    sota->camera.offset.x   = sota->map->camera.offset.x;
    sota->camera.offset.y   = sota->map->camera.offset.y;
    sota->camera.zoom       = sota->map->camera.zoom;

    sota->map->update = true;
}

void Game_Map_Free(struct Game *sota) {
    if (sota->map != NULL) {
        Map_Units_Hide(sota->map);
        Map_Free(sota->map);
        SDL_free(sota->map);
        sota->map = NULL;
    }
}

void Game_debugMap_Free(struct Game *sota) {

}

/* Game_Gameplay_Start */
// Pre-requisites:
//  - Map loaded
//  - Party loaded
//  - Convoy loaded
// TODO: Check pre-requisites
void Game_Gameplay_Start(struct Game *sota, i32 state, i32 substate) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Starting Gameplay\n");
    /* -- Preliminaries -- */
    /* - Updating game states - */
    if (state == GAME_STATE_Preparation)
        strncpy(sota->reason, "Debug map preparation", sizeof(sota->reason));
    if (sota->state != state)
        Game_State_Set(sota, state, sota->reason);
    if (sota->substate != substate)
        Game_State_Set(sota, substate, sota->reason);

    if (sota->state == GAME_STATE_Preparation) {
        /* -- Deployment Menu -- */
        // TODO: move to start deployment event or something
        Game_DeploymentMenu_Enable(sota);
        struct Menu *mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
        struct DeploymentMenu *dm = mc->data;
        SDL_assert(dm->_party_size > 0);
        DeploymentMenu_Party_Set(dm, &sota->party);

        Game_cursorFocus_onMenu(sota);

        /* -- Show starting positions -> for deployment -- */
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_START_POS);
    } else if (sota->state == GAME_STATE_Gameplay_Map) {
        Game_cursorFocus_onMap(sota);
    }

    // TODO: move music start to when game inits.
    // SDL_Log("Loading Music\n");
    Map_Music_Load(sota->map);

    /* -- Load reinforcements -- */
    Game_Map_Reinforcements_Load(sota);
}

void Game_debugMap_Load(struct Game *sota) {
    /* -- Preliminaries -- */
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test Map\n");
    /* - Updating game states - */

    // TODO read party from save json, not map
    // Game_Map_Load(sota, CHAPTER_TEST_V8);
    // SDL_assert(DARR_NUM(sota->map->death_enemy) == 1);
    // SDL_assert(DARR_NUM(sota->map->death_friendly) == 2);
    Game_Map_Load(sota, CHAPTER_TEST_NES1);

    // TODO: load from json file
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test party\n");

    /* -- Loading Party -- */
    sota->party = Party_default;
    Party_Init(&sota->party);
    SDL_assert(sota->party.json_names       != NULL);
    SDL_assert(sota->party.json_filenames   != NULL);
    SDL_assert(sota->party.json_ids         != NULL);

    /* - Reading party json - */
    SDL_assert(sota->map->party_filename.data != NULL);
    // SDL_Log("sota->map->party_filename %s", sota->map->party_filename.data);

    Party_Folder(&sota->party, "units/debug_map");
    jsonio_readJSON(sota->map->party_filename, &sota->party);
    SDL_assert(sota->party.json_filenames != NULL);

    /* - Loading party units json - */
    Party_Load(&sota->party, sota, sota->weapons_dtab, sota->items_dtab);
    Party_Size(&sota->party);

    SDL_assert(sota->party.size > 0);
}

/* --- Reinforcements --- */
void Game_Map_Reinforcements_Free(struct Game *sota) {

    SDL_assert(sota != NULL);
    if (sota->map_enemies == NULL) {
        SDL_Log("Map enemies uninitialized");
        return;
    }
    if (DARR_NUM(sota->map_enemies) == 0) {
        SDL_Log("No map enemies");
        return;
    }

    char filename[DEFAULT_BUFFER_SIZE];
    while (DARR_NUM(sota->map_enemies) > 0) {
        tnecs_entity temp_unit_ent =  DARR_POP(sota->map_enemies);
        if (temp_unit_ent == TNECS_NULL)
            continue;
        if (sota->world->entities[temp_unit_ent] == TNECS_NULL)
            continue;

        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);

        if (unit) {
            b32 isPC = (unit->_id > UNIT_ID_PC_START) && (unit->_id < UNIT_ID_PC_END);

            if (!isPC) {
                Unit_Free(unit);
                SDL_assert(unit->grown_stats == NULL);
            }
        }

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        if (sprite) {
            Sprite_Free(sprite);
        }

        tnecs_entity_destroy(sota->world, temp_unit_ent);
    }

    if (sota->map_enemies != NULL) {
        DARR_FREE(sota->map_enemies);
        sota->map_enemies = NULL;
    }
}

void Game_Map_Reinforcements_Load(struct Game *sota) {
    SDL_assert(sota->map != NULL);
    char filename[DEFAULT_BUFFER_SIZE];
    for (i16 i = 0; i < DARR_NUM(sota->map->reinforcements); i++) {
        struct Reinforcement *reinf = &(sota->map->reinforcements[i]);
        // Skip if reinforcement is not for THIS turn
        if ((reinf->turn != sota->map->turn))
            continue;

        // SDL_Log("-- Reinforcement turn: %d %d --", reinf->turn, sota->map->turn);

        // TODO: Skip if something blocks tile reinforcements come to
        // SDL_Log("-- loading reinforcements %ld --", i);
        // SDL_Log("-- create entity --");
        tnecs_entity temp_unit_ent;
        temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position,
                                                        Sprite, Timer, MapHPBar, AI);
        DARR_PUT(sota->map_enemies, temp_unit_ent);

        // SDL_Log("-- checks --");
        tnecs_component typeflag;
        typeflag = TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Unit, Position,
                                                  Sprite, Timer, MapHPBar, AI);

        size_t typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        size_t typeflag_id2 = tnecs_typeflagid(sota->world, sota->world->entity_typeflags[temp_unit_ent]);

        int num_typeflag1 = sota->world->num_entities_bytype[typeflag_id1];
        int num_typeflag2 = sota->world->num_entities_bytype[typeflag_id2];

        // SDL_Log("- current -");
        size_t current_num = sota->world->num_entities_bytype[typeflag_id1];

        tnecs_entity **entities_bytype = sota->world->entities_bytype;
        SDL_assert(entities_bytype != NULL);

        // SDL_Log("-- loading position --");
        struct Position *position = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Position);
        SDL_assert(position != NULL);
        *position = Position_default;
        // SDL_memcpy(position, &Position_default, sizeof(Position));
        position->onTilemap = true;
        Position_Bounds_Set(position, 0, sota->map->col_len, 0, sota->map->row_len);
        position->scale[0]      = (float)sota->map->tilesize[0];
        position->scale[1]      = (float)sota->map->tilesize[0];
        position->tilemap_pos.x = reinf->position.x;
        position->tilemap_pos.y = reinf->position.y;
        position->pixel_pos.x   = (i32)lround(position->tilemap_pos.x * position->scale[0]);
        position->pixel_pos.y   = (i32)lround(position->tilemap_pos.y * position->scale[1]);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        int index = position->tilemap_pos.y * sota->map->col_len + position->tilemap_pos.x;

        // check if another unit already on the map.
        if (sota->map->unitmap[index] != TNECS_NULL) {
            // DESIGN QUESTION. If another entity is already on the map.
            //  -> FE does not even load unit
            tnecs_entity_destroy(sota->world, DARR_POP(sota->map_enemies));
            continue;
        }

        if ((reinf->boss_icon > BOSS_ICON_NULL) && (reinf->boss_icon < BOSS_ICON_NUM)) {
            // SDL_Log("-- loading unit Boss --");
            // SDL_Log("%d %d", position->tilemap_pos.x, position->tilemap_pos.y);
            tnecs_entity entity = TNECS_ADD_COMPONENT(sota->world, temp_unit_ent, Boss);
            position = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Position);

            // SDL_Log("%d %d", position->tilemap_pos.x, position->tilemap_pos.y);
            SDL_assert(temp_unit_ent == entity);
            struct Boss *boss = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Boss);
            SDL_assert(boss != NULL);
            *boss = Boss_default;
            boss->icon = reinf->boss_icon;
            Boss_Icon_Load(boss, sota->renderer);
            typeflag += TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Boss);
            typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        }

        // TODO: Walking around on the map
        // SDL_Log("-- loading slider --");
        // struct Slider * slider = TNECS_GET_COMPONENT(sota->world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        // SDL_Log("-- loading unit --");
        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        *unit = Unit_default;
        Unit_Allocs(unit);
        SDL_assert(unit != NULL);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        /* DESIGN: Reinforcements wait! */
        unit->waits = true;
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->weapons_dtab  = sota->weapons_dtab;
        unit->items_dtab    = sota->items_dtab;
        s8 unit_path  = s8_mut("units"PHYSFS_SEPARATOR);
        unit_path     = s8cat(unit_path, reinf->filename);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        jsonio_readJSON(unit_path, unit);

        Unit_Reinforcement_Load(unit, reinf);
        s8_free(&unit_path);
        SDL_assert(unit->name.data != NULL);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        /* Make AI reinforcements levelup */
        Unit_Reinforcement_Levelups(unit, reinf);

        // SDL_Log("-- loading unit equipment --");
        for (int j = 0; j < DARR_NUM(sota->map->reinf_equipments[i]); j++) {
            unit->_equipment[j] = sota->map->reinf_equipments[i][j];
        }

        if (Unit_canEquip_inHand(unit, UNIT_HAND_LEFT)) {
            Unit_Equip(unit, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
        }
        if (Unit_canEquip_inHand(unit, UNIT_HAND_RIGHT)) {
            Unit_Equip(unit, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
        }

        Unit_effectiveStats(unit);

        /* Check that reinforcement has something equipped at least in strong hand */
        // Ensures enemy can attack
        b32 stronghand = Unit_Hand_Strong(unit);
        SDL_assert(Unit_isEquipped(unit, stronghand));

        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        SDL_assert(unit->status_queue != NULL);

        s8_free(&unit_path);

        // SDL_Log("-- loading unit AI --");
        struct AI *ai = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, AI);
        *ai = AI_default;
        s8 ai_path  = s8_mut("ai"PHYSFS_SEPARATOR);
        /* - Default ai - */
        if (unit->ai_filename.data == NULL)
            unit->ai_filename = s8_mut("aggressive.json");

        if (reinf->ai_filename.data != NULL)
            ai_path = s8cat(ai_path, reinf->ai_filename);
        else
            ai_path = s8cat(ai_path, unit->ai_filename);

        jsonio_readJSON(ai_path, ai);

        s8_free(&ai_path);
        // TODO: AI_Free
        s8_free(&ai->json_filename);

        // SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent    = temp_unit_ent;
        map_hp_bar->len         = sota->settings.tilesize[0];
        map_hp_bar->update      = true;
        map_hp_bar->visible     = true;

        // SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        *timer = Timer_default;

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        Sprite_Palette_Swap(sprite, sota_palettes[sota->map->ipalette_enemy], sota->renderer);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        // SDL_Log("-- put on map --");
        SDL_assert(sota->world->entity_typeflags[temp_unit_ent] == typeflag);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Map_Unit_Put(sota->map, position->tilemap_pos.x, position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        SDL_assert(unit->army == reinf->army);
        SDL_assert(unit->name.data != NULL);
    }
    sota->map->reinf_loaded = sota->map->turn;
    SDL_assert(DARR_NUM(sota->map_enemies) <= DARR_NUM(sota->map->reinforcements));

}

/* --- Tiles & tilesets  --- */
void Game_Tilesets_Dump(struct Game *sota) {
    i32 tile_ind;
    for (size_t i = 0; i < DARR_NUM(sota->map->tiles); i++) {
        s8 dumpname = s8_mut("Tileset_");
        if (sota->map->tilesindex[i] > TILE_ID_MAX)
            tile_ind = sota->map->tilesindex[i] / TILE_DIVISOR;
        else
            tile_ind = sota->map->tilesindex[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(sota->map, tile_ind);
        struct Tile *temp_tile = sota->map->tiles + tile_order;
        dumpname = s8cat(dumpname, s8_var(temp_tile->name));
        dumpname = s8cat(dumpname, s8_literal(".png"));
        SDL_Log("%s", dumpname);
        // SDL_Texture *temptexture = DTAB_GET(sota->map->textures, (sota->map->tilesindex[i]));
        // Filesystem_Texture_Dump(dumpname, sota->renderer, temptexture, SDL_PIXELFORMAT_ARGB8888);
        // memset(&dumpname, 0, DEFAULT_BUFFER_SIZE);
        s8_free(&dumpname);
    }
}
