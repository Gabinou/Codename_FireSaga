#include "game/map.h"

#define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
void Game_Map_Load(struct Game *sota, const i16 in_map_index) {
    SDL_assert((in_map_index > CHAPTER_START) && (in_map_index < CHAPTER_END));
    SDL_LogDebug(SOTA_LOG_SYSTEM, "%ld \n", in_map_index);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Associated map filename     %s \n", mapFilenames[in_map_index]);
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
    // Game_Map_Load(sota, CHAPTER_TEST_V6);

}

void Game_debugMap_Load(struct Game *sota) {
    /* -- Preliminaries -- */
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test Map\n");
    memcpy(sota->reason, "for testing", sizeof(sota->reason));
    Game_State_Set(sota, GAME_STATE_Gameplay_Map, sota->reason);
    memcpy(sota->reason, "on Init state to GAME_STATE_Gameplay_Map substate is idle",
           sizeof(sota->reason));
    Game_subState_Set(sota, GAME_SUBSTATE_STANDBY, sota->reason);
    Game_Map_Load(sota, CHAPTER_TEST_V6);
    Game_PopUp_Tile_Create(sota);
    Game_cursorFocus_onMap(sota);

    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test party\n");
    /* -- Party -- */
    /* - Preliminaries - */
    i16 *unit_inds;
    unit_inds = DARR_INIT(unit_inds, i16, 16);
    struct Point temp_point = {.x = 4, .y = 4};
    struct Point *positions_list;
    positions_list = DARR_INIT(positions_list, struct Point, 16);

    /* - Silou.json (Mage) - */
    DARR_PUT(unit_inds, UNIT_ID_SILOU);
    DARR_PUT(positions_list, temp_point);

    /* - Kiara.json (Cleric) - */
    DARR_PUT(unit_inds, UNIT_ID_KIARA);
    temp_point.x = 3;
    temp_point.y = 4;
    DARR_PUT(positions_list, temp_point);

    /* - Rayan.json (Dancer) - */
    DARR_PUT(unit_inds, UNIT_ID_RAYAN);
    temp_point.x = 3;
    temp_point.y = 3;

    /* -- Putting party on map -- */
    // Game_Party_Load(sota, unit_inds, DARR_NUM(unit_inds));
    DARR_PUT(positions_list, temp_point);

    SDL_assert(DARR_NUM(unit_inds) == DARR_NUM(positions_list));
    Game_putPConMap(sota, unit_inds, positions_list, DARR_NUM(unit_inds));
    DARR_FREE(unit_inds);
    DARR_FREE(positions_list);

    SDL_Log("Loading in reinforcements\n");
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
        tnecs_entity_t temp_unit_ent =  DARR_POP(sota->map_enemies);

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
        SDL_Log("-- turn: %d --", sota->map->reinforcements[i].turn);

        if ((sota->map->reinforcements[i].turn |= sota->map->turn))
            continue;

        SDL_Log("-- loading reinforcements %ld --", i);
        SDL_Log("-- create entity --");
        tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position, Sprite,
                                       Timer, MapHPBar);
        DARR_PUT(sota->map_enemies, temp_unit_ent);
        // tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit, Position, Sprite, Timer, MapHPBar);

        SDL_Log("-- checks --");
        tnecs_component_t typeflag = TNECS_COMPONENT_NAMES2TYPEFLAG(sota->world, Unit,
                                                                    Position, Sprite,
                                                                    Timer, MapHPBar);

        SDL_Log("- 1 -");
        size_t typeflag_id1 = tnecs_typeflagid(sota->world, typeflag);
        SDL_Log("- 2 -");
        size_t typeflag_id2 = tnecs_typeflagid(sota->world, sota->world->entity_typeflags[temp_unit_ent]);
        int num_typeflag1 = sota->world->num_entities_bytype[typeflag_id1];
        int num_typeflag2 = sota->world->num_entities_bytype[typeflag_id2];

        SDL_Log("- current -");
        size_t current_num = sota->world->num_entities_bytype[typeflag_id1];

        tnecs_entity_t **entities_bytype = sota->world->entities_bytype;
        SDL_assert(entities_bytype != NULL);

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
        *unit = Unit_default;
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->weapons_dtab = sota->weapons_dtab;
        size_t order = *(u16 *)DTAB_GET(global_unitOrders, sota->map->reinforcements[i].id);
        s8 unitname = s8_mut(global_unitNames[order].data);
        s8 json_literal = s8_literal(".json");
        SDL_assert(json_literal.num == 5);
        SDL_assert(json_literal.num == strlen(json_literal.data));
        unitname    = s8cat(unitname, json_literal);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        jsonio_readJSON(unitname, unit);
        SDL_assert(unit->name.data != NULL);
        s8_free(&unitname);
        if (!Unit_ID_Valid(unit->_id)) {
            SDL_Log("Unit %s.json has wrong ID. Should be %d.", unit->name, sota->map->reinforcements[i].id);
            exit(ERROR_Generic);
        }
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        unit->army = sota->map->reinforcements[i].army;

        SDL_Log("-- loading unit equipment --");
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

        // // SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent = temp_unit_ent;
        map_hp_bar->len = sota->settings.tilesize[0];

        SDL_Log("-- loading position --");
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

        SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        * timer = Timer_default;

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        Sprite_Palette_Swap(sprite, palette_NES_enemy, sota->renderer);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_SPRITE_LOOP_IDLE);
        SDL_assert(sprite->spritesheet->frames[0] == 4);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);

        SDL_Log("-- put on map --");
        SDL_assert(sota->world->entity_typeflags[temp_unit_ent] == typeflag);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
        Map_Unit_Put(sota->map, sota->world, position->tilemap_pos.x,
                     position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[typeflag_id1][num_typeflag1 - 1] == temp_unit_ent);
    }
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
