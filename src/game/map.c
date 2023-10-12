#include "game/map.h"

#define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
void Game_Map_Unload(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    Game_Map_Free(sota);
    Game_Tiles_Unload(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Map_Load(struct Game *sota, const if16 in_map_index) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%ld \n", in_map_index);
    SDL_assert((in_map_index > CHAPTER_START) && (in_map_index < CHAPTER_END));
    SOTA_Log_Debug("Associated map filename     %s \n", mapFilenames[in_map_index]);
    Game_Map_Free(sota);
    Game_Tiles_Unload(sota);
    // Game_Units_Unload(sota);
    sota->map = Map_Init(sota->map, sota->settings.tilesize[0], sota->settings.tilesize[1]);
    SDL_assert(sota->world != NULL);
    sota->map->world = sota->world;
    sota->map->stack_mode = sota->settings.map_settings.stack_mode;
    // sota->map->tiles = sota->tiles_loaded;
    Map_Renderer_Set(sota->map, sota->renderer);
    jsonio_readJSON(mapFilenames[in_map_index], sota->map);
    // read_json sets the row_len and col_len necessary for dArrays_Init
    Map_dArrays_Init(sota->map, &sota->settings);
    // Game_Tilesets_Dump(sota);
    char *path = "assets"PHYSFS_SEPARATOR"Tiles"PHYSFS_SEPARATOR"Tileset_Shadow.json";
    struct cJSON *jshadow_tileset_file = jsonio_parseJSON(path);
    struct cJSON *jshadow_tileset = cJSON_GetObjectItem(jshadow_tileset_file, "shadow_tileset");
    Tilemap_Shader_Load_Tileset_JSON(sota->map->tilemap_shader, jshadow_tileset);
    cJSON_Delete(jshadow_tileset_file);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Map_Free(struct Game *sota) {

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    if (sota->map != NULL) {
        Map_Units_Hide(sota->map);
        Map_Free(sota->map);
        free(sota->map);
        sota->map = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_debugMap_Free(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Game_Map_Load(sota, CHAPTER_TEST_V6);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_debugMap_Load(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SOTA_Log_Debug("Loading in test Map\n");
    strncpy(sota->reason, "for testing", sizeof(sota->reason));
    Game_State_Set(sota, GAME_STATE_Gameplay_Map, sota->reason);
    strncpy(sota->reason, "on Init state to GAME_STATE_Gameplay_Map substate is idle",
            sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    Game_Map_Load(sota, CHAPTER_TEST_V6);
    Game_PopUp_Tile_Create(sota);
    Game_cursorFocus_onMap(sota);

    SOTA_Log_Debug("Loading in test party\n");
    /* -- Party -- */
    /* - Preliminaries - */
    if16 *unit_inds;
    unit_inds = DARR_INIT(unit_inds, if16, 16);
    struct Point temp_point = {.x = 4, .y = 4};
    struct Point *positions_list;
    positions_list = DARR_INIT(positions_list, struct Point, 16);

    /* - Silou (Mage) - */
    DARR_PUT(unit_inds, UNIT_ID_SILOU);
    DARR_PUT(positions_list, temp_point);

    /* - Kiara (Cleric) - */
    DARR_PUT(unit_inds, UNIT_ID_KIARA);
    temp_point.x = 3;
    temp_point.y = 4;
    DARR_PUT(positions_list, temp_point);

    /* - Rayan (Dancer) - */
    DARR_PUT(unit_inds, UNIT_ID_RAYAN);
    temp_point.x = 3;
    temp_point.y = 3;

    /* -- Putting party on map -- */
    Game_Party_Load(sota, unit_inds, DARR_NUM(unit_inds));
    DARR_PUT(positions_list, temp_point);

    SDL_assert(DARR_NUM(unit_inds) == DARR_NUM(positions_list));
    Game_putPConMap(sota, unit_inds, positions_list, DARR_NUM(unit_inds));
    DARR_FREE(unit_inds);
    DARR_FREE(positions_list);

    SOTA_Log_Debug("Loading in reinforcements\n");
    Game_Map_Reinforcements_Load(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Reinforcements --- */
void Game_Map_Reinforcements_Free(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota                 != NULL);
    if (sota->map_enemies == NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    if (DARR_NUM(sota->map_enemies) == 0) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    char filename[DEFAULT_BUFFER_SIZE];
    char unitname[DEFAULT_BUFFER_SIZE];
    for (if16 i = 0; i < DARR_NUM(sota->map_enemies); i++) {
        tnecs_entity_t temp_unit_ent =  DARR_POP(sota->map_enemies);

        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        if (unit)
            Unit_Free(unit);

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        if (sprite)
            Sprite_Free(sprite);
    }
    DARR_NUM(sota->map_enemies) = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Map_Reinforcements_Load(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->map != NULL);
    char filename[DEFAULT_BUFFER_SIZE];
    char unitname[DEFAULT_BUFFER_SIZE];
    for (if16 i = 0; i < DARR_NUM(sota->map->reinforcements); i++) {
        SOTA_Log_Debug("-- turn: %d --", sota->map->reinforcements[i].turn);

        if ((sota->map->reinforcements[i].turn |= sota->map->turn))
            continue;

        SOTA_Log_Debug("-- loading reinforcements %ld --", i);
        SOTA_Log_Debug("-- create entity --");
        tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position, Sprite,
                                       Timer, MapHPBar);
        DARR_PUT(sota->map_enemies, temp_unit_ent);
        // tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position, Sprite, Timer, MapHPBar);

        SOTA_Log_Debug("-- checks --");
        tnecs_component_t typeflag = TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Unit, Position, Sprite,
                                                                    Timer, MapHPBar);

        SOTA_Log_Debug("- 1 -");
        size_t typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        SOTA_Log_Debug("- 2 -");
        size_t typeflag_id2 = tnecs_typeflagid(sota->world, sota->world->entity_typeflags[temp_unit_ent]);
        int num_typeflag1 = sota->world->num_entities_bytype[typeflag_id1];
        int num_typeflag2 = sota->world->num_entities_bytype[typeflag_id2];

        SOTA_Log_Debug("- current -");
        size_t current_num = sota->world->num_entities_bytype[typeflag_id1];

        tnecs_entity_t **entities_bytype = sota->world->entities_bytype;
        SDL_assert(entities_bytype != NULL);

        // SOTA_Log_Debug("-- loading slider --");
        // struct Slider * slider = TNECS_GET_COMPONENT(sota->world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        SOTA_Log_Debug("-- loading unit --");
        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        SDL_assert(unit != NULL);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        *unit = Unit_default;
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->weapons_dtab = sota->weapons_dtab;
        size_t order = *(uf16 *)DTAB_GET(global_unitOrders, sota->map->reinforcements[i].id);
        strncpy(unitname, "", 1);
        strcat(unitname, global_unitNames[order]);
        strcat(unitname, ".json");
        SOTA_Log_Debug("unitname: %ld %s", sota->map->reinforcements[i].id, unitname);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        jsonio_readJSON(unitname, unit);
        if (!Unit_ID_Valid(unit->_id)) {
            SDL_Log("Unit %s.json has wrong ID. Should be %d.", unit->name, sota->map->reinforcements[i].id);
            exit(ERROR_Generic);
        }
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->army = sota->map->reinforcements[i].army;
        SOTA_Log_Debug("-- loading unit equipment --");
        for (int j = 0; j < DARR_NUM(sota->map->reinf_equipments[i]); j++) {
            unit->_equipment[j] = sota->map->reinf_equipments[i][j];
        }

        if (unit->_equipment[UNIT_HAND_RIGHT].id > 0)
            Unit_Equip_inHand(unit, UNIT_HAND_RIGHT);
        if (unit->_equipment[UNIT_HAND_LEFT].id > 0)
            Unit_Equip_inHand(unit, UNIT_HAND_LEFT);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Unit_Init(unit);
        SDL_assert(unit->status_queue != NULL);

        // // SOTA_Log_Debug("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent = temp_unit_ent;
        map_hp_bar->len = sota->settings.tilesize[0];

        SOTA_Log_Debug("-- loading position --");
        struct Position *position = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Position);
        SDL_assert(position != NULL);
        *position = Position_default;
        // SDL_memcpy(position, &Position_default, sizeof(Position));
        position->onTilemap = true;
        Position_Bounds_Set(position, sota->map->boundsmin.x, sota->map->boundsmax.x,
                            sota->map->boundsmin.y, sota->map->boundsmax.y);
        position->scale[0] = (float)sota->map->tilesize[0];
        position->scale[1] = (float)sota->map->tilesize[0];
        position->tilemap_pos.x = sota->map->reinforcements[i].position.x;
        position->tilemap_pos.y = sota->map->reinforcements[i].position.y;
        position->pixel_pos.x = (i32)lround(position->tilemap_pos.x * position->scale[0]);
        position->pixel_pos.y = (i32)lround(position->tilemap_pos.y * position->scale[1]);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);

        SOTA_Log_Debug("-- loading sprite --");
        struct Timer *timer;
        timer = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        * timer = Timer_default;

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        Sprite_Palette_Swap(sprite, palette_NES_enemy, sota->renderer);
        // sprite->flip = SDL_FLIP_HORIZONTAL;

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_SPRITE_LOOP_IDLE);
        SDL_assert(sprite->spritesheet->frames[0] == 4);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);

        SOTA_Log_Debug("-- put on map --");
        SDL_assert(sota->world->entity_typeflags[temp_unit_ent] == typeflag);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Map_Unit_Put(sota->map, sota->world, position->tilemap_pos.x,
                     position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Tiles & tilesets  --- */
void Game_Tiles_Unload(struct Game *sota) {

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Tilesets_Dump(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char dumpname[DEFAULT_BUFFER_SIZE] = "";
    char temp[DEFAULT_BUFFER_SIZE];
    i32 tile_ind;
    for (size_t i = 0; i < DARR_NUM(sota->map->tiles); i++) {
        strcat(dumpname, "Tileset_");
        if (sota->map->tilesindex[i] > TILE_ID_MAX)
            tile_ind = sota->map->tilesindex[i] / TILE_DIVISOR;
        else
            tile_ind = sota->map->tilesindex[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(sota->map, tile_ind);
        struct Tile *temp_tile = sota->map->tiles + tile_order;
        strcat(dumpname, temp_tile->name);
        strcat(dumpname, ".png");
        SOTA_Log_Debug("%s", dumpname);
        // SDL_Texture * temptexture = DTAB_GET(sota->map->textures, (sota->map->tilesindex[i]));
        // Filesystem_Texture_Dump(dumpname, sota->renderer, temptexture, SDL_PIXELFORMAT_ARGB8888);
        // memset(&dumpname, 0, DEFAULT_BUFFER_SIZE);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
