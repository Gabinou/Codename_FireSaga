
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
Map* Game_Map(const struct Game *const IES) {
    // sota->map
    if ((gl_world == NULL) || (IES->map == TNECS_NULL)) {
        return (NULL);
    }
    Map *map = IES_GET_COMPONENT(gl_world, IES->map, Map);
    return (map);
}

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
    new_map.world       = gl_world;
    new_map.renderer    = sota->render.er;
    new_map.stack_mode  = sota->settings.map_settings.stack_mode;

    SDL_assert(gl_world != NULL);
    sota->map = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Map_ID);
    Map *map1 = Game_Map(sota);
    SDL_assert(map1 != NULL);
    Map *map2 = IES_GET_COMPONENT(gl_world, sota->map, Map);
    SDL_assert(map1 == map2);
    Map_Init(map1, new_map);

    jsonio_readJSON(mapFilenames[in_map_index], map1);

    map1->flags.update = true;
}

void Game_Map_Free(struct Game *IES) {
    Map *map = Game_Map(IES);
    if (IES->map != TNECS_NULL) {
        tnecs_entity_destroy(gl_world, IES->map);
        IES->map = TNECS_NULL;
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
        strncpy(sota->debug.reason, "Debug map preparation", sizeof(sota->debug.reason));
    if (Game_State_Current(sota) != state)
        Game_State_Set(sota, state, sota->debug.reason);
    if (Game_Substate_Current(sota) != substate)
        Game_State_Set(sota, substate, sota->debug.reason);

    Map *map = Game_Map(sota);
    if (Game_State_Current(sota) == GAME_STATE_Preparation) {
        /* -- Deployment Menu -- */
        // TODO: move to start deployment event or something
        Game_DeploymentMenu_Enable(sota);
        struct Menu *mc = IES_GET_COMPONENT(gl_world, sota->menus.deployment, Menu);
        struct DeploymentMenu *dm = mc->data;
        SDL_assert(dm->_party_size > 0);
        DeploymentMenu_Party_Set(dm, &sota->party);

        Game_cursorFocus_onMenu(sota);

        /* -- Show starting positions -> for deployment -- */
        Map_Palettemap_Autoset(map, MAP_OVERLAY_START_POS, TNECS_NULL);
    } else if (Game_State_Current(sota) == GAME_STATE_Gameplay_Map) {
        Game_cursorFocus_onMap(sota);
    }

    // TODO: move music start to when game inits.
    // SDL_Log("Loading Music\n");
    Map_Music_Load(map);

    /* -- Load reinforcements -- */
    Game_Map_Reinforcements_Load(sota);
}

/* --- Reinforcements --- */
void GameMap_Reinforcements_Free(struct Game *sota) {

    if (sota == NULL) {
        return;
    }
    Map *map = Game_Map(sota);
    if (map == NULL) {
        return;
    }
    if (map->reinforcements.arr == NULL) {
        return;
    }

    int reinf_num = DARR_NUM(map->reinforcements.arr);
    for (int i = 0; i < reinf_num; i++) {
        tnecs_entity temp_unit_ent =  map->reinforcements.arr[i].entity;

        if (temp_unit_ent == TNECS_NULL)
            continue;
        if (gl_world->entities.id[temp_unit_ent] == TNECS_NULL)
            continue;

        // struct Unit *unit = IES_GET_COMPONENT(gl_world, temp_unit_ent, Unit);
        // if (unit) {
        // b32 isPC = (Unit_id(unit) > UNIT_ID_PC_START) && (Unit_id(unit) < UNIT_ID_PC_END);

        // if (!isPC) {
        //     Unit_Free_tnecs(unit);
        //     SDL_assert(Unit_Stats_Grown(unit) == NULL);
        // }
        // }

        struct Sprite *sprite = IES_GET_COMPONENT(gl_world, temp_unit_ent, Sprite);
        if (sprite) {
            Sprite_Free(sprite);
        }

        tnecs_entity_destroy(gl_world, temp_unit_ent);
    }
}

void Game_Map_Reinforcements_Load(struct Game *sota) {
    Map *map = Game_Map(sota);
    SDL_assert(map != NULL);
    int reinf_num = DARR_NUM(map->reinforcements.arr);
    for (int i = 0; i < reinf_num; i++) {
        struct Reinforcement *reinf = &(map->reinforcements.arr[i]);
        // Skip if reinforcement is not for THIS turn
        if ((reinf->turn != map->turn))
            continue;

        // SDL_Log("-- Reinforcement turn: %d %d --", reinf->turn, map->turn);

        // TODO: Skip if something blocks tile reinforcements come to
        // SDL_Log("-- loading reinforcements %ld --", i);
        // SDL_Log("-- create entity --");
        tnecs_entity temp_unit_ent;
        temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Unit_ID, Position_ID,
                                                        Sprite_ID, Timer_ID, MapHPBar_ID, Unit_AI_ID);
        reinf->entity = temp_unit_ent;

        // SDL_Log("-- checks --");
        tnecs_component archetype;
        archetype = TNECS_COMPONENT_IDS2ARCHETYPE(Unit_ID, Position_ID, Sprite_ID, Timer_ID, MapHPBar_ID,
                                                  Unit_AI_ID);

        size_t archetype_id1 = tnecs_archetypeid(gl_world, archetype);

        int num_archetype1 = gl_world->bytype.num_entities[archetype_id1];

        // SDL_Log("- current -");
        tnecs_entity **entities_bytype = gl_world->bytype.entities;
        SDL_assert(entities_bytype != NULL);

        // SDL_Log("-- loading position --");
        struct Position *position = IES_GET_COMPONENT(gl_world, temp_unit_ent, Position);
        SDL_assert(position != NULL);
        *position = Position_default;
        // SDL_memcpy(position, &Position_default, sizeof(Position));
        position->onTilemap = true;
        Position_Bounds_Set(position, 0, Map_col_len(map), 0, Map_row_len(map));
        const Point *tilesize = Map_Tilesize(map);
        position->scale[0]      = tilesize->x;
        position->scale[1]      = tilesize->y;
        position->tilemap_pos.x = reinf->position.x;
        position->tilemap_pos.y = reinf->position.y;
        position->pixel_pos.x   = (i32)lround(position->tilemap_pos.x * position->scale[0]);
        position->pixel_pos.y   = (i32)lround(position->tilemap_pos.y * position->scale[1]);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);

        int index = position->tilemap_pos.y * Map_col_len(map) + position->tilemap_pos.x;

        // check if another unit already on the map.
        if (map->darrs.unitmap[index] != TNECS_NULL) {
            // DESIGN QUESTION. If another entity is already on the map.
            //  -> FE does not even load unit
            tnecs_entity_destroy(gl_world, temp_unit_ent);
            reinf->entity = TNECS_NULL;
            continue;
        }

        if ((reinf->boss_icon > BOSS_ICON_NULL) && (reinf->boss_icon < BOSS_ICON_NUM)) {
            // SDL_Log("-- loading unit Boss --");
            tnecs_entity entity = TNECS_ADD_COMPONENT(gl_world, temp_unit_ent, Boss_ID);
            position = IES_GET_COMPONENT(gl_world, temp_unit_ent, Position);

            SDL_assert(temp_unit_ent == entity);
            struct Boss *boss = IES_GET_COMPONENT(gl_world, temp_unit_ent, Boss);
            SDL_assert(boss != NULL);
            *boss = Boss_default;
            boss->icon = reinf->boss_icon;
            Boss_Icon_Load(boss, sota->render.er);
            archetype += TNECS_COMPONENT_IDS2ARCHETYPE(Boss_ID);
            archetype_id1 = tnecs_archetypeid(gl_world, archetype);
        }

        // TODO: Walking around on the map
        // SDL_Log("-- loading slider --");
        // struct Slider * slider = IES_GET_COMPONENT(gl_world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        // SDL_Log("-- loading unit --");
        struct Unit *unit = IES_GET_COMPONENT(gl_world, temp_unit_ent, Unit);
        *unit = Unit_default;
        SDL_assert(unit->equipment._arr[0] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[1] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[2] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[3] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[4] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[5] == TNECS_NULL);
        SDL_assert(unit->equipment._arr[6] == TNECS_NULL);

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
        SDL_assert(map->reinforcements.items_num[i] + 1 == DARR_NUM(
                           map->reinforcements.equipments[i]));
        for (int j = ITEM1; j < DARR_NUM(map->reinforcements.equipments[i]); j++) {
            if (Item_ID_isValid(map->reinforcements.equipments[i][j].id)) {
                tnecs_entity ent = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Inventory_item_ID);
                Inventory_item *invitem = IES_GET_COMPONENT(gl_world, ent, Inventory_item);

                *invitem = map->reinforcements.equipments[i][j];

                Unit_Item_Take(unit, ent);
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

        s8_free(&unit_path);

        // SDL_Log("-- loading unit AI --");
        struct Unit_AI *ai = IES_GET_COMPONENT(gl_world, temp_unit_ent, Unit_AI);
        *ai = Unit_AI_default;
        s8 ai_path = s8_mut("ai"PHYSFS_SEPARATOR);

        /* - Default ai - */
        if (!AI_ID_isvalid(Unit_AI_Type(unit)))
            Unit_AI_set(unit, AI_DEFAULT);
        SDL_assert(AI_ID_isvalid(Unit_AI_Type(unit)));
        s8 ai_filename = AI_ID_isvalid(reinf->ai_id) ? AI_filename(reinf->ai_id) : AI_filename(Unit_AI_Type(
                                 unit));
        ai_path = s8cat(ai_path, ai_filename);

        jsonio_readJSON(ai_path, ai);

        s8_free(&ai_filename);
        s8_free(&ai_path);
        // TODO: AI_Free
        s8_free(&ai->jsonio_header.json_filename);

        // SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(gl_world, temp_unit_ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent    = temp_unit_ent;
        map_hp_bar->len         = sota->settings.tilesize[0];
        map_hp_bar->update      = true;
        map_hp_bar->visible     = true;

        // SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = IES_GET_COMPONENT(gl_world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        *timer = Timer_default;

        struct Sprite *sprite = IES_GET_COMPONENT(gl_world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->render.er);
        Sprite_Palette_Swap(sprite, sota_palettes[map->palette.enemy], sota->render.er);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        // SDL_Log("-- put on map --");
        SDL_assert(gl_world->entities.archetypes[temp_unit_ent] == archetype);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);
        Map_Unit_Put(map, position->tilemap_pos.x, position->tilemap_pos.y, temp_unit_ent);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == temp_unit_ent);

        SDL_assert(Unit_Army(unit) == reinf->army);
        SDL_assert(global_unitNames[*(u64 *)dtab_get(global_unitOrders, Unit_id(unit))].data != NULL);
    }
    map->reinforcements.loaded = map->turn;
}

/* --- Tiles & tilesets  --- */
void Game_Tilesets_Dump(struct Game *sota) {
    Map *map = Game_Map(sota);
    SDL_assert(map != NULL);
    i32 tile_ind;
    for (size_t i = 0; i < DARR_NUM(map->tiles.arr); i++) {
        s8 dumpname = s8_mut("Tileset_");
        if (map->tiles.index[i] > TILE_ID_MAX)
            tile_ind = map->tiles.index[i] / TILE_DIVISOR;
        else
            tile_ind = map->tiles.index[i];
        SDL_assert(tile_ind > 0);
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles.arr + tile_order;
        dumpname = s8cat(dumpname, Tile_Name(temp_tile));
        dumpname = s8cat(dumpname, s8_literal(".png"));
        SDL_Log("%s", dumpname.data);
        // SDL_Texture *temptexture = DTAB_GET(map->textures, (map->tilesindex[i]));
        // Filesystem_Texture_Dump(dumpname, sota->render.er, temptexture, SDL_PIXELFORMAT_ARGB8888);
        // memset(&dumpname, 0, DEFAULT_BUFFER_SIZE);
        s8_free(&dumpname);
    }
}
