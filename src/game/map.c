#include "game/map.h"

#define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
void Game_Map_Load(struct Game *sota,  i16 in_map_index) {
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
    // Game_Tilesets_Dump(sota);

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

void Game_debugMap_Load(struct Game *sota) {
    /* -- Preliminaries -- */
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test Map\n");
    /* - Updating game states - */
    strncpy(sota->reason, "Debug map preparation", sizeof(sota->reason));
    if (sota->substate != GAME_STATE_Preparation)
        Game_State_Set(sota, GAME_STATE_Preparation, sota->reason);
    if (sota->substate != GAME_SUBSTATE_MENU)
        Game_subState_Set(sota, GAME_SUBSTATE_MENU, sota->reason);

    Game_Map_Load(sota, CHAPTER_TEST_V7);

    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test party\n");

    /* -- Loading Party -- */
    Game_Party_Clear(sota);

    /* - Reading party json - */
    SDL_assert(sota->map->party_filename.data != NULL);
    SDL_Log("sota->map->party_filename %s", sota->map->party_filename.data);

    sota->party_struct = Party_default;
    Party_Folder(&sota->party_struct, "units/debug_map");
    jsonio_readJSON(sota->map->party_filename, &sota->party_struct);
    SDL_assert(sota->party_struct.filenames != NULL);

    /* - Loading party units json - */
    sota->party_struct.party            = sota->party;
    sota->party_struct.party_id_stack   = sota->party_id_stack;
    Party_Load(&sota->party_struct, sota->weapons_dtab, sota->items_dtab);
    Party_Size(&sota->party_struct);
    SDL_assert(sota->party_struct.size > 0);

    Game_DeploymentMenu_Enable(sota);
    struct Menu *mc = TNECS_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
    struct DeploymentMenu *dm = mc->data;
    SDL_assert(dm->_party_size > 0);
    DeploymentMenu_Party_Set(dm, sota->party, sota->party_id_stack, sota->party_struct.size);

    Game_cursorFocus_onMenu(sota);

    SDL_Log("Loading Music\n");
    Map_Music_Load(sota->map);

    SDL_Log("Loading Conditions\n");
    sota->map->death_enemy      = DARR_INIT(sota->map->death_enemy,
                                            struct Map_condition, 1);
    sota->map->death_friendly   = DARR_INIT(sota->map->death_friendly,
                                            struct Map_condition, 2);
    DARR_PUT(sota->map->death_enemy,    Map_condition_boss_win);
    DARR_PUT(sota->map->death_friendly, Map_condition_main_char_loss);
    DARR_PUT(sota->map->death_friendly, Map_condition_debug_map_loss);

    // DARR_FREE(unit_inds);

    /* -- Load reinforcements -- */
    // TODO: Move to debug_map_load
    Game_Map_Reinforcements_Load(sota);
}

/* --- Reinforcements --- */
void Game_Map_Reinforcements_Free(struct Game *sota) {
    SDL_assert(sota                 != NULL);
    if (sota->map_enemies == NULL) {
        return;
    }
    if (DARR_NUM(sota->map_enemies) == 0) {
        return;
    }

    char filename[DEFAULT_BUFFER_SIZE];
    for (i16 i = 0; i < DARR_NUM(sota->map_enemies); i++) {
        tnecs_entity temp_unit_ent =  DARR_POP(sota->map_enemies);

        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        if (unit)
            Unit_Free(unit);

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        if (sprite)
            Sprite_Free(sprite);
    }
    DARR_NUM(sota->map_enemies) = 0;
}

void Game_Map_Reinforcements_Load(struct Game *sota) {
    SDL_assert(sota->map != NULL);
    char filename[DEFAULT_BUFFER_SIZE];
    for (i16 i = 0; i < DARR_NUM(sota->map->reinforcements); i++) {
        struct Reinforcement *reinf = &(sota->map->reinforcements[i]);
        // Skip if reinforcement is not for THIS turn
        if ((reinf->turn != sota->map->turn))
            continue;

        SDL_Log("-- Reinforcement turn: %d %d --", reinf->turn, sota->map->turn);

        // TODO: Skip if something blocks tile reinforcements come to
        SDL_Log("-- loading reinforcements %ld --", i);
        SDL_Log("-- create entity --");
        tnecs_entity temp_unit_ent;
        temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position,
                                                        Sprite, Timer, MapHPBar, AI);
        DARR_PUT(sota->map_enemies, temp_unit_ent);
        SDL_Log("-- checks --");
        tnecs_component typeflag;
        typeflag = TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Unit, Position,
                                                  Sprite, Timer, MapHPBar, AI);

        SDL_Log("- 1 -");
        size_t typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        SDL_Log("- 2 -");
        size_t typeflag_id2 = tnecs_typeflagid(sota->world, sota->world->entity_typeflags[temp_unit_ent]);

        SDL_Log("-- loading unit Boss --");
        if ((reinf->boss_icon > BOSS_ICON_NULL) && (reinf->boss_icon < BOSS_ICON_NUM)) {
            tnecs_entity entity = TNECS_ADD_COMPONENT(sota->world, temp_unit_ent, Boss);
            SDL_assert(temp_unit_ent == entity);
            struct Boss *boss = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Boss);
            SDL_assert(boss != NULL);
            *boss = Boss_default;
            boss->icon = reinf->boss_icon;
            Boss_Icon_Load(boss, sota->renderer);
            typeflag += TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Boss);
            typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        }
        int num_typeflag1 = sota->world->num_entities_bytype[typeflag_id1];
        int num_typeflag2 = sota->world->num_entities_bytype[typeflag_id2];

        SDL_Log("- current -");
        size_t current_num = sota->world->num_entities_bytype[typeflag_id1];

        tnecs_entity **entities_bytype = sota->world->entities_bytype;
        SDL_assert(entities_bytype != NULL);

        // TODO: Walking around on the map
        // SDL_Log("-- loading slider --");
        // struct Slider * slider = TNECS_GET_COMPONENT(sota->world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        SDL_Log("-- loading unit --");
        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        SDL_assert(unit != NULL);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Unit_Init(unit);

        /* DESIGN: Reinforcements wait! */
        unit->waits = true;
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->weapons_dtab = sota->weapons_dtab;
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

        SDL_Log("-- loading unit equipment --");
        for (int j = 0; j < DARR_NUM(sota->map->reinf_equipments[i]); j++) {
            unit->_equipment[j] = sota->map->reinf_equipments[i][j];
        }

        if (unit->_equipment[UNIT_HAND_RIGHT].id > 0)
            Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
        if (unit->_equipment[UNIT_HAND_LEFT].id > 0)
            Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        SDL_assert(unit->status_queue != NULL);

        s8_free(&unit_path);

        SDL_Log("-- loading unit AI --");
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

        SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent = temp_unit_ent;
        map_hp_bar->len = sota->settings.tilesize[0];
        map_hp_bar->update = true;

        SDL_Log("-- loading position --");
        struct Position *position = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Position);
        SDL_assert(position != NULL);
        *position = Position_default;
        // SDL_memcpy(position, &Position_default, sizeof(Position));
        position->onTilemap = true;
        Position_Bounds_Set(position, sota->map->boundsmin.x, sota->map->boundsmax.x,
                            sota->map->boundsmin.y, sota->map->boundsmax.y);
        position->scale[0]      = (float)sota->map->tilesize[0];
        position->scale[1]      = (float)sota->map->tilesize[0];
        position->tilemap_pos.x = reinf->position.x;
        position->tilemap_pos.y = reinf->position.y;
        position->pixel_pos.x   = (i32)lround(position->tilemap_pos.x * position->scale[0]);
        position->pixel_pos.y   = (i32)lround(position->tilemap_pos.y * position->scale[1]);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        *timer = Timer_default;

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        Sprite_Palette_Swap(sprite, palette_NES_enemy, sota->renderer);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        SDL_Log("-- put on map --");
        SDL_assert(sota->world->entity_typeflags[temp_unit_ent] == typeflag);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Map_Unit_Put(sota->map, position->tilemap_pos.x, position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        SDL_assert(unit->army == reinf->army);
        SDL_assert(unit->name.data != NULL);
    }
    sota->map->reinf_loaded = sota->map->turn;
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
        // SDL_Texture * temptexture = DTAB_GET(sota->map->textures, (sota->map->tilesindex[i]));
        // Filesystem_Texture_Dump(dumpname, sota->renderer, temptexture, SDL_PIXELFORMAT_ARGB8888);
        // memset(&dumpname, 0, DEFAULT_BUFFER_SIZE);
        s8_free(&dumpname);
    }
}
