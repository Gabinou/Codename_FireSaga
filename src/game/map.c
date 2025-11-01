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
** Top level map handling
**  - Ex: Loading map from chapter number
*/

#include "AI.h"
#include "log.h"
#include "tile.h"
#include "item.h"
#include "names.h"
#include "jsonio.h"
#include "weapon.h"
#include "sprite.h"
#include "globals.h"
#include "palette.h"
#include "position.h"
#include "platform.h"
#include "reinforcement.h"

#include "bars/map_hp.h"

#include "game/game.h"

#include "map/map.h"
#include "map/find.h"
#include "map/tiles.h"
#include "map/render.h"
#include "map/ontile.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/deployment.h"

#include "game/map.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"

#include "unit/unit.h"
#include "unit/boss.h"
#include "unit/stats.h"
#include "unit/party.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

// #define STB_SPRINTF_IMPLEMENTATION
// #ifndef STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Map utilities --- */
Map* Game_Map(const struct Game *const IES) {
    if ((gl_world == NULL) ||
        (IES->map == TNECS_NULL)
       ) {
        return (NULL);
    }
    Map *map = IES_GET_C(gl_world, IES->map, Map);
    return (map);
}

void Game_Map_Load(struct Game *sota, i32 map_i) {
    SDL_assert((map_i > CHAPTER_START) &&
               (map_i < CHAPTER_NUM)
              );
    SDL_LogDebug(SOTA_LOG_SYSTEM, "%d \n", map_i);
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Associated map filename     %s \n", mapFilenames[map_i].data);

    /* --- PRELIMINARIES --- */
    Game_Map_Free(sota);

    /* --- Allocating map --- */
    /* --- Reading map from json files --- */
    i32 rowcol[TWO_D];
    Map_RowCol_readJSON(mapFilenames[map_i], rowcol);

    NewMap new_map      = NewMap_default;
    new_map.tilesize[0] = sota->settings.tilesize[0];
    new_map.tilesize[1] = sota->settings.tilesize[1];
    new_map.row_len     = rowcol[SOTA_ROW_INDEX];
    new_map.col_len     = rowcol[SOTA_COL_INDEX];
    new_map.renderer    = sota->render.er;
    new_map.stack_mode  = sota->settings.map_settings.stack_mode;

    SDL_assert(gl_world != NULL);
    sota->map = IES_E_CREATE_wC(gl_world, Map_ID);
    Map *map1 = Game_Map(sota);
    SDL_assert(map1 != NULL);
    Map *map2 = IES_GET_C(gl_world, sota->map, Map);
    SDL_assert(map1 == map2);
    Map_Init(map1, new_map);

    jsonio_readJSON(mapFilenames[map_i], map1);

    map1->flags.update = true;
}

void Game_Map_Free(struct Game *IES) {
    if (IES->map != TNECS_NULL) {
        tnecs_E_destroy(gl_world, IES->map);
        IES->map = TNECS_NULL;
    }
}

/* Game_Gameplay_Start
** Pre-requisites:
**  - Map loaded
**  - Party loaded
**  - Convoy loaded
** TODO: Check pre-requisites */
void Game_Gameplay_Start(Game   *sota,
                         i32     state,
                         i32     substate) {
    SDL_LogDebug(SOTA_LOG_SYSTEM, "Starting Gameplay\n");
    /* -- Preliminaries -- */
    /* - Updating game states - */
    s8 reason;
    if (state == GAME_STATE_Preparation) {
        reason = s8_literal("Debug map preparation");
    } else {
        reason = s8_literal("TODO");
    }
    if (Game_State_Current(sota) != state)
        Game_State_Set(sota, state, reason.data);
    if (Game_Substate_Current(sota) != substate)
        Game_State_Set(sota, substate, reason.data);

    Map *map = Game_Map(sota);
    if (Game_State_Current(sota) == GAME_STATE_Preparation) {
        /* -- Deployment Menu -- */
        // TODO: move to start deployment event or something
        Game_DeploymentMenu_Enable(sota);
        struct Menu *mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
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
        tnecs_E ent = map->reinforcements.arr[i].entity;

        if (ent == TNECS_NULL)
            continue;

        if (!TNECS_E_EXISTS(gl_world, ent))
            continue;

        tnecs_E_destroy(gl_world, ent);
    }
}

void Game_Map_Reinforcements_Load(struct Game *sota) {
    Map *map = Game_Map(sota);
    SDL_assert(map != NULL);
    int reinf_num = DARR_NUM(map->reinforcements.arr);
    for (int i = 0; i < reinf_num; i++) {
        struct Reinforcement *reinf = NULL;
        reinf = &(map->reinforcements.arr[i]);
        /* Skip if reinforcement is not for THIS turn */
        if ((reinf->turn != map->turn))
            continue;

        // SDL_Log("-- Reinforcement turn: %d %d --", reinf->turn, map->turn);

        // TODO: Skip if something blocks tile reinforcements come to
        // SDL_Log("-- loading reinforcements %ld --", i);
        // SDL_Log("-- create entity --");
        tnecs_E ent;
        ent = IES_E_CREATE_wC(  gl_world,       Unit_ID,
                                Position_ID,    Sprite_ID,
                                Timer_ID,       MapHPBar_ID,
                                Unit_AI_ID);
        reinf->entity = ent;

        // SDL_Log("-- checks --");
        tnecs_C archetype;
        archetype = IES_C_ID_2_A(Unit_ID,       Position_ID,
                                 Sprite_ID,     Timer_ID,
                                 MapHPBar_ID,   Unit_AI_ID);

        size_t archetype_id1 = tnecs_A_id(gl_world, archetype);

        int num_archetype1 = gl_world->byA.num_Es[archetype_id1];

        // SDL_Log("- current -");
        tnecs_E **entities_bytype = gl_world->byA.Es;
        SDL_assert(entities_bytype != NULL);

        // SDL_Log("-- loading position --");
        struct Position *position = IES_GET_C(gl_world, ent, Position);
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
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);

        int index = position->tilemap_pos.y * Map_col_len(map) + position->tilemap_pos.x;

        // check if another unit already on the map.
        if (map->darrs.unitmap[index] != TNECS_NULL) {
            // DESIGN QUESTION. If another entity is already on the map.
            //  -> FE does not even load unit
            tnecs_E_destroy(gl_world, ent);
            reinf->entity = TNECS_NULL;
            continue;
        }

        if ((reinf->boss_icon > BOSS_ICON_NULL) && (reinf->boss_icon < BOSS_ICON_NUM)) {
            // SDL_Log("-- loading unit Boss --");
            tnecs_E entity = TNECS_ADD_C(gl_world, ent, Boss_ID);
            position = IES_GET_C(gl_world, ent, Position);

            SDL_assert(ent == entity);
            struct Boss *boss = IES_GET_C(gl_world, ent, Boss);
            SDL_assert(boss != NULL);
            *boss = Boss_default;
            boss->icon = reinf->boss_icon;
            Boss_Icon_Load(boss, sota->render.er);
            archetype += IES_C_ID_2_A(Boss_ID);
            archetype_id1 = tnecs_A_id(gl_world, archetype);
        }

        // TODO: Walking around on the map
        // SDL_Log("-- loading slider --");
        // struct Slider * slider = IES_GET_C(gl_world,  temp./_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIM_X] = 2.0f;
        // slider->slidefactors[DIM_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;

        // SDL_Log("-- loading unit --");
        struct Unit *unit = IES_GET_C(gl_world, ent, Unit);
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
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);

        /* DESIGN: Reinforcements wait! */
        Unit_Waiting_set(unit, true);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);
        s8 unit_path  = s8_mut("units"PHYSFS_SEPARATOR);
        unit_path     = s8cat(unit_path, reinf->filename);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);
        jsonio_readJSON(unit_path, unit);

        Unit_Reinforcement_Load(unit, reinf);
        s8_free(&unit_path);
        SDL_assert(Unit_Name(unit).data != NULL);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);
        SDL_assert(map->reinforcements.items_num[i] + 1 == DARR_NUM(
                           map->reinforcements.equipments[i]));
        int num_eq = DARR_NUM(map->reinforcements.equipments[i]);
        for (int j = ITEM1; j < num_eq; j++) {
            int id = map->reinforcements.equipments[i][j].id;
            if (Item_Pure_ID_isValid(id) || Weapon_ID_isValid(id)) {
                tnecs_E ent = IES_E_CREATE_wC(gl_world, InvItem_ID);
                InvItem *invitem = IES_GET_C(gl_world, ent, InvItem);

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

        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);

        s8_free(&unit_path);

        // SDL_Log("-- loading unit AI --");
        struct Unit_AI *ai = IES_GET_C(gl_world, ent, Unit_AI);
        *ai = Unit_AI_default;
        s8 ai_path = s8_mut("ai"PHYSFS_SEPARATOR);

        ai_path = s8cat(ai_path, reinf->ai_filename);

        jsonio_readJSON(ai_path, ai);

        s8_free(&ai_path);
        /* s8_free(&ai->jsonio_header.json_filename); */

        // SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = IES_GET_C(gl_world, ent, MapHPBar);
        // *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent    = ent;
        map_hp_bar->len         = sota->settings.tilesize[0];
        map_hp_bar->update      = true;
        map_hp_bar->visible     = true;

        // SDL_Log("-- loading sprite --");
        struct Timer *timer;
        timer = IES_GET_C(gl_world, ent, Timer);
        SDL_assert(timer != NULL);
        *timer = Timer_default;

        struct Sprite *sprite = IES_GET_C(gl_world, ent, Sprite);
        SDL_assert(sprite != NULL);
        *sprite = Sprite_default;
        Sprite_Map_Unit_Load(sprite, unit, sota->render.er);
        Sprite_Palette_Swap(sprite, sota_palettes[map->palette.enemy], sota->render.er);

        sprite->visible = true;
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        // SDL_Log("-- put on map --");
        SDL_assert(gl_world->Es.As[ent] == archetype);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);
        Map_Unit_Put(map, position->tilemap_pos.x, position->tilemap_pos.y, ent);
        SDL_assert(entities_bytype[archetype_id1][num_archetype1 - 1] == ent);

        SDL_assert(Unit_Army(unit) == reinf->army);
        SDL_assert(Unit_Name(unit).data != NULL);
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
