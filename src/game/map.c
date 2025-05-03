
#include "game/map.h"
#include "game/cursor.h"
#include "game/menu.h"
#include "game/popup.h"
#include "names.h"
#include "log.h"
#include "position.h"
#include "jsonio.h"
#include "platform.h"
#include "palette.h"
#include "map/map.h"
#include "map/render.h"
#include "map/find.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "item.h"
#include "unit/equipment.h"
#include "unit/unit.h"
#include "unit/boss.h"
#include "unit/stats.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "game/game.h"
#include "bars/map_hp.h"
#include "sprite.h"
#include "unit/party.h"
#include "menu/menu.h"
#include "menu/deployment.h"
#include "menu/stats.h"
#include "tile.h"
#include "globals.h"
#include "reinforcement.h"
#include "AI.h"

// #define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
void Game_Map_Load(struct Game *sota, i32 in_map_index) {
    SDL_assert((in_map_index > CHAPTER_START) && (in_map_index < CHAPTER_NUM));
    SDL_LogDebug(SOTA_LOG_SYSTEM, "%d \n", in_map_index);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Associated map filename     %s \n", mapFilenames[in_map_index].data);

    /* --- PRELIMINARIES --- */
    Game_Map_Free(sota);

    /* --- Allocating map --- */
    /* --- Reading map from json files --- */
    i32 rowcol[TWO_D];
    Map_RowCol_readJSON(mapFilenames[in_map_index], rowcol);

    NewMap new_map      = NewMap_default;
    new_map.tilesize[0] = sota->settings.tilesize[0];
    new_map.tilesize[1] = sota->settings.tilesize[1];
    new_map.row_len     = rowcol[SOTA_ROW_INDEX];
    new_map.col_len     = rowcol[SOTA_COL_INDEX];
    new_map.world       = sota->world;
    new_map.renderer    = sota->renderer;
    new_map.stack_mode  = sota->settings.map_settings.stack_mode;

    sota->map = Map_New(new_map);
    SDL_assert(sota->world != NULL);

    // Issue: Need to readjson file to alloc some members
    //      - Only parameters need to read are row_len and col_len
    // Solution:
    //      1. read file one time BEFORE allocation
    //          - No need to change Map_New
    //          - No allocation in readjson
    //      2. Allocate in new if row_len/col_len set, readJson otherwise

    jsonio_readJSON(mapFilenames[in_map_index], sota->map);

    /* Set initial camera position */
    sota->camera.offset.x   = sota->map->camera.offset.x;
    sota->camera.offset.y   = sota->map->camera.offset.y;
    sota->camera.zoom       = sota->map->camera.zoom;

    sota->map->flags.update = true;
}

void Game_Map_Free(struct Game *sota) {
    if (sota->map != NULL) {
        Map_Units_Hide(sota->map);
        Map_Free(sota->map);
        SDL_free(sota->map);
        sota->map = NULL;
    }
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
        struct Menu *mc = IES_GET_COMPONENT(sota->world, sota->deployment_menu, Menu);
        struct DeploymentMenu *dm = mc->data;
        SDL_assert(dm->_party_size > 0);
        DeploymentMenu_Party_Set(dm, &sota->party);

        Game_cursorFocus_onMenu(sota);

        /* -- Show starting positions -> for deployment -- */
        Map_Palettemap_Autoset(sota->map, MAP_OVERLAY_START_POS, TNECS_NULL);
    } else if (sota->state == GAME_STATE_Gameplay_Map) {
        Game_cursorFocus_onMap(sota);
    }

    // TODO: move music start to when game inits.
    // SDL_Log("Loading Music\n");
    Map_Music_Load(sota->map);

    /* -- Load reinforcements -- */
    Game_Map_Reinforcements_Load(sota);
}

void Game_Map_Party_Load(struct Game *sota, i32 mapi) {
    /* -- Preliminaries -- */
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Loading in test Map\n");
    /* - Updating game states - */

    // TODO read party from save json, not map
    // Game_Map_Load(sota, CHAPTER_TEST_V8);
    // SDL_assert(DARR_NUM(sota->map->death_enemy) == 1);
    // SDL_assert(DARR_NUM(sota->map->death_friendly) == 2);
    // Game_Map_Load(sota, CHAPTER_TEST_NES1);
    Game_Map_Load(sota, mapi);

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
    Party_Load(&sota->party, sota);
    Party_Size(&sota->party);

    SDL_assert(sota->party.size > 0);
}

/* --- Reinforcements --- */
void GameMap_Reinforcements_Free(struct Game *sota) {

    SDL_assert(sota != NULL);
    if (sota->map_enemies == NULL) {
        // SDL_Log("Map enemies uninitialized");
        return;
    }
    if (DARR_NUM(sota->map_enemies) == 0) {
        // SDL_Log("No map enemies");
        return;
    }

    while (DARR_NUM(sota->map_enemies) > 0) {
        tnecs_entity temp_unit_ent =  DARR_POP(sota->map_enemies);
        if (temp_unit_ent == TNECS_NULL)
            continue;
        if (sota->world->entities.id[temp_unit_ent] == TNECS_NULL)
            continue;

        struct Unit *unit = IES_GET_COMPONENT(sota->world, temp_unit_ent, Unit);

        if (unit) {
            b32 isPC = (Unit_id(unit) > UNIT_ID_PC_START) && (Unit_id(unit) < UNIT_ID_PC_END);

            if (!isPC) {
                Unit_Free(unit);
                SDL_assert(Unit_Stats_Grown(unit) == NULL);
            }
        }

        struct Sprite *sprite = IES_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
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
    DARR_NUM(sota->map_enemies) = 0;
    for (int i = 0; i < DARR_NUM(sota->map->reinforcements.arr); i++) {
        struct Reinforcement *reinf = &(sota->map->reinforcements.arr[i]);
        // Skip if reinforcement is not for THIS turn
        if ((reinf->turn != sota->map->turn))
            continue;

        // SDL_Log("-- Reinforcement turn: %d %d --", reinf->turn, sota->map->turn);

        // TODO: Skip if something blocks tile reinforcements come to
        // SDL_Log("-- loading reinforcements %ld --", i);
        // SDL_Log("-- create entity --");
        tnecs_entity temp_unit_ent;
        temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Unit_ID, Position_ID,
                                                        Sprite_ID, Timer_ID, MapHPBar_ID, AI_ID);
        DARR_PUT(sota->map_enemies, temp_unit_ent);

        // SDL_Log("-- checks --");
        tnecs_component archetype;
        archetype = TNECS_COMPONENT_IDS2ARCHETYPE(Unit_ID, Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID,
                                                  AI_ID);

        size_t archetype_id1 = tnecs_archetypeid(sota->world, archetype);

        int num_archetype1 = sota->world->bytype.num_entities[archetype_id1];

        // SDL_Log("- current -");
        tnecs_entity **entities_bytype = sota->world->bytype.entities;
        SDL_assert(entities_bytype != NULL);

        // SDL_Log("-- loading position --");
        struct Position *position = IES_GET_COMPONENT(sota->world, temp_unit_ent, Position);
        SDL_assert(position != NULL);
        *position = Position_default;
        // SDL_memcpy(position, &Position_default, sizeof(Position));
        position->onTilemap = true;
        Position_Bounds_Set(position, 0, Map_col_len(sota->map), 0, Map_row_len(sota->map));
        const Point *tilesize = Map_Tilesize(sota->map);
        position->scale[0]      = tilesize->x;
        position->scale[1]      = tilesize->y;
        position->tilemap_pos.x = reinf->position.x;
        position->tilemap_pos.y = reinf->position.y;
        position->pixel_pos.x   = (i32)lround(position->tilemap_pos.x * position->scale[0]);
        position->pixel_pos.y   = (i32)lround(position->tilemap_pos.y * position->scale[1]);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);

        int index = position->tilemap_pos.y * Map_col_len(sota->map) + position->tilemap_pos.x;

        // check if another unit already on the map.
        if (sota->map->darrs.unitmap[index] != TNECS_NULL) {
            // DESIGN QUESTION. If another entity is already on the map.
            //  -> FE does not even load unit
            tnecs_entity_destroy(sota->world, DARR_POP(sota->map_enemies));
            continue;
        }

        if ((reinf->boss_icon > BOSS_ICON_NULL) && (reinf->boss_icon < BOSS_ICON_NUM)) {
            // SDL_Log("-- loading unit Boss --");
            tnecs_entity entity = TNECS_ADD_COMPONENT(sota->world, temp_unit_ent, Boss_ID);
            position = IES_GET_COMPONENT(sota->world, temp_unit_ent, Position);

            SDL_assert(temp_unit_ent == entity);
            struct Boss *boss = IES_GET_COMPONENT(sota->world, temp_unit_ent, Boss);
            SDL_assert(boss != NULL);
            *boss = Boss_default;
            boss->icon = reinf->boss_icon;
            Boss_Icon_Load(boss, sota->renderer);
            archetype += TNECS_COMPONENT_IDS2ARCHETYPE(Boss_ID);
            archetype_id1 = tnecs_archetypeid(sota->world, archetype);
        }

        // TODO: Walking around on the map
        // SDL_Log("-- loading slider --");
        // struct Slider * slider = IES_GET_COMPONENT(sota->world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        // SDL_Log("-- loading unit --");
        struct Unit *unit = IES_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        *unit = Unit_default;
        SDL_assert(unit->equipment.arr[0].id == 0);
        SDL_assert(unit->equipment.arr[1].id == 0);
        SDL_assert(unit->equipment.arr[2].id == 0);
        SDL_assert(unit->equipment.arr[3].id == 0);
        SDL_assert(unit->equipment.arr[4].id == 0);
        SDL_assert(unit->equipment.arr[5].id == 0);
        SDL_assert(unit->equipment.arr[6].id == 0);

        Unit_Members_Alloc(unit);
        SDL_assert(unit != NULL);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);

        /* DESIGN: Reinforcements wait! */
        Unit_Waiting_set(unit, true);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        s8 unit_path  = s8_mut("units"PHYSFS_SEPARATOR);
        unit_path     = s8cat(unit_path, reinf->filename);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        jsonio_readJSON(unit_path, unit);

        Unit_Reinforcement_Load(unit, reinf);
        s8_free(&unit_path);
        SDL_assert(global_unitNames[*(u64 *)dtab_get(global_unitOrders, Unit_id(unit))].data != NULL);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        SDL_assert(sota->map->reinforcements.items_num[i] + 1 == DARR_NUM(
                           sota->map->reinforcements.equipments[i]));
        for (int j = ITEM1; j < DARR_NUM(sota->map->reinforcements.equipments[i]); j++) {
            if (Item_ID_isValid(sota->map->reinforcements.equipments[i][j].id)) {
                Unit_Item_Take(unit, sota->map->reinforcements.equipments[i][j]);
            }
        }

        /* Make AI reinforcements levelup */
        Unit_Reinforcement_Levelups(unit, reinf);

        // SDL_Log("-- loading unit equipment --");
        canEquip can_equip = canEquip_default;
        canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  UNIT_HAND_LEFT);
        canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
        canEquip_Eq(&can_equip, UNIT_HAND_LEFT);
        can_equip.hand      = UNIT_HAND_LEFT;

        if (Unit_canEquip(unit, can_equip)) {
            Unit_Equip(unit, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
        }

        canEquip_Eq(&can_equip, UNIT_HAND_RIGHT);
        can_equip.hand      = UNIT_HAND_RIGHT;
        if (Unit_canEquip(unit, can_equip)) {
            Unit_Equip(unit, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
        }

        Unit_effectiveStats(unit);

        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        SDL_assert(unit->statuses.queue != NULL);

        s8_free(&unit_path);

        // SDL_Log("-- loading unit AI --");
        struct AI *ai = IES_GET_COMPONENT(sota->world, temp_unit_ent, AI);
        *ai = AI_default;
        s8 ai_path = s8_mut("ai"PHYSFS_SEPARATOR);

        /* - Default ai - */
        if (!AI_ID_isvalid(Unit_AI(unit)))
            Unit_AI_set(unit, AI_DEFAULT);
        SDL_assert(AI_ID_isvalid(Unit_AI(unit)));
        s8 ai_filename = AI_ID_isvalid(reinf->ai_id) ? AI_filename(reinf->ai_id) : AI_filename(Unit_AI(
                                 unit));
        ai_path = s8cat(ai_path, ai_filename);

        jsonio_readJSON(ai_path, ai);

        s8_free(&ai_filename);
        s8_free(&ai_path);
        // TODO: AI_Free
        s8_free(&ai->jsonio_header.json_filename);

        // SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent    = temp_unit_ent;
        map_hp_bar->len         = sota->settings.tilesize[0];
        map_hp_bar->update      = true;
        map_hp_bar->visible     = true;

        // SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = IES_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        *timer = Timer_default;

        struct Sprite *sprite = IES_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        Sprite_Palette_Swap(sprite, sota_palettes[sota->map->palette.enemy], sota->renderer);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        // SDL_Log("-- put on map --");
        SDL_assert(sota->world->entities.archetypes[temp_unit_ent] == archetype);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        Map_Unit_Put(sota->map, position->tilemap_pos.x, position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);

        SDL_assert(Unit_Army(unit) == reinf->army);
        SDL_assert(global_unitNames[*(u64 *)dtab_get(global_unitOrders, Unit_id(unit))].data != NULL);
    }
    sota->map->reinforcements.loaded = sota->map->turn;
    SDL_assert(DARR_NUM(sota->map_enemies) <= DARR_NUM(sota->map->reinforcements.arr));
}

/* --- Tiles & tilesets  --- */
void Game_Tilesets_Dump(struct Game *sota) {
    i32 tile_ind;
    for (size_t i = 0; i < DARR_NUM(sota->map->tiles.arr); i++) {
        s8 dumpname = s8_mut("Tileset_");
        if (sota->map->tiles.index[i] > TILE_ID_MAX)
            tile_ind = sota->map->tiles.index[i] / TILE_DIVISOR;
        else
            tile_ind = sota->map->tiles.index[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(sota->map, tile_ind);
        struct Tile *temp_tile = sota->map->tiles.arr + tile_order;
        dumpname = s8cat(dumpname, Tile_Name(temp_tile));
        dumpname = s8cat(dumpname, s8_literal(".png"));
        SDL_Log("%s", dumpname.data);
        // SDL_Texture *temptexture = DTAB_GET(sota->map->textures, (sota->map->tilesindex[i]));
        // Filesystem_Texture_Dump(dumpname, sota->renderer, temptexture, SDL_PIXELFORMAT_ARGB8888);
        // memset(&dumpname, 0, DEFAULT_BUFFER_SIZE);
        s8_free(&dumpname);
    }
}
