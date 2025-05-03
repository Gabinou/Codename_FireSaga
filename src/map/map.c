
#include "map/map.h"
#include "map/render.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/conditions.h"
#include "filesystem.h"
#include "map/find.h"
#include "index_shader.h"
#include "aura.h"
#include "nmath.h"
#include "macros.h"
#include "palette.h"
#include "position.h"
#include "inventory.h"
#include "jsonio.h"
#include "platform.h"
#include "tile.h"
#include "names.h"
#include "arrow.h"
#include "music.h"
#include "reinforcement.h"
#include "pathfinding.h"
#include "structs.h"
#include "sprite.h"
#include "unit/unit.h"
#include "unit/anim.h"
#include "unit/boss.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/bonus.h"
#include "unit/flags.h"
#include "unit/status.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "stb_sprintf.h"

const NewMap NewMap_default = {
    .tilesize   = {SOTA_TILESIZE, SOTA_TILESIZE},
    .stack_mode = MAP_SETTING_STACK_DANGERMAP,
};

const Map Map_default = {
    .jsonio_header.json_element = JSON_MAP,
    .flags.show_icons       = true,
    .cost.multiplier        =  1,
    .render.frames          = 10,
    .render.frames_shadow   = 10,
    .render.speed           = 50,

    .perimiter.danger_color = SOTA_RED,
    .perimiter.aura_color   = SOTA_PURPLE,

    .render.shadow_frame_pause     = 3,
    .render.tilemap_frame_pause    = 6,
    .render.shadow_frame_factor    = 0.10f,
    .render.shadow_frame_counter   = UINT8_MAX - 1, /* shadow renders on first frame */
    .render.tilemap_frame_factor   = 0.15f,
    .render.tilemap_frame_counter  = UINT8_MAX - 1, /* map renders on first frame */

    .camera.offset.x        = DEFAULT_CAMERA_XOFFSET,
    .camera.offset.y        = DEFAULT_CAMERA_YOFFSET,
    .camera.zoom            = DEFAULT_CAMERA_ZOOM,

    .palette.base           = PALETTE_SOTA,
    .palette.red            = PALETTE_SOTA_FILTER_RED,
    .palette.blue           = PALETTE_SOTA_FILTER_BLUE,
    .palette.green          = PALETTE_SOTA_FILTER_GREEN,
    .palette.purple         = PALETTE_SOTA_FILTER_PURPLE,
    .palette.darkred        = PALETTE_SOTA_FILTER_DARKREDwSHADOW,
    .palette.shadow         = PALETTE_SOTA_SHADOW,
    .palette.enemy          = PALETTE_SOTA,

    .turn                   =  1, /* Automatic loss if turn 255. */
    /* --- MUSIC --- */
    .music.i_friendly       = SOTA_MUSIC_DEBUG,
    .music.i_enemy          = -1,
};

void Map_Reinforcements_Free(struct Map *map) {
    if (map->reinforcements.arr == NULL)
        return;

    for (int r = 0; r < DARR_NUM(map->reinforcements.arr); r++) {
        Reinforcement_Free(&map->reinforcements.arr[r]);
    }

    DARR_FREE(map->reinforcements.arr);
    map->reinforcements.arr = NULL;
}

void Map_Tilemap_Shader_Free(struct Map *map) {
    Tilemap_Shader_Free(map->render.tilemap_shader);
    if (map->render.tilemap_shader != NULL) {
        SDL_free(map->render.tilemap_shader);
        map->render.tilemap_shader = NULL;
    }
}

void Map_Tilemap_Shader_Init(Map *map) {
    map->render.tilemap_shader         = SDL_malloc(sizeof(struct Tilemap_Shader));
    *map->render.tilemap_shader        = Tilemap_Shader_default;
    map->render.tilemap_shader->map    = map;
    map->render.tilemap_shader->to     = palette_table_SOTA_shadow;
}

/* --- GLOBAL FUNCTIONS --- */
/* --- Constructor/Destructors --- */
Map *Map_New(NewMap new_map) {
    Map *map        = SDL_malloc(sizeof(Map));
    *map            = Map_default;
    SDL_assert(map->cost.multiplier == 1);
    map->world      = new_map.world;
    map->stack.mode = new_map.stack_mode;

    Map_Size_Set(       map,    new_map.col_len,        new_map.row_len);
    Map_Tilesize_Set(   map,    new_map.tilesize[0],    new_map.tilesize[1]);
    Map_Renderer_Set(map, new_map.renderer);
    Map_Members_Alloc(map);
    SDL_assert(map->cost.multiplier == 1);

    return (map);
}

void Map_Unitmap_Free(struct Map *map) {
    /* -- SDL_free non-PC units on unitmap -- */
    SDL_assert(map->darrs.unitmap);
    for (size_t i = 0; i < map->col_len * map->row_len ; i++) {
        tnecs_entity uent = map->darrs.unitmap[i];
        if (uent == TNECS_NULL) {
            map->darrs.unitmap[i] = TNECS_NULL;
            continue;
        }
        if (!TNECS_ENTITY_EXISTS(map->world, uent)) {
            map->darrs.unitmap[i] = TNECS_NULL;
            continue;
        }

        Unit    *unit   = IES_GET_COMPONENT(map->world, uent, Unit);
        Sprite  *sprite = IES_GET_COMPONENT(map->world, uent, Sprite);

        if (sprite != NULL)
            Sprite_Free(sprite);

        // Freeing unit if not in party
        if ((unit != NULL) && (Unit_id(unit) > UNIT_ID_PC_END)) {
            Unit_Free(unit);
            tnecs_entity_destroy(map->world, uent);
        } else if (unit == NULL) {
            tnecs_entity_destroy(map->world, uent);
        }
        map->darrs.unitmap[i] = TNECS_NULL;
    }

    s8_free(&map->jsonio_header.json_filename);

    /* -- SDL_free unitmap -- */
    SDL_free(map->darrs.unitmap);
    map->darrs.unitmap = NULL;
}

void Map_Free(struct Map *map) {
    SDL_assert(map);
    /* - Music -*/
    if (map->music.enemy != NULL) {
        Mix_FreeMusic(map->music.enemy);
        map->music.enemy = NULL;
    }
    if (map->party_filename.data != NULL) {
        s8_free(&map->party_filename);
    }
    if (map->music.friendly != NULL) {
        Mix_FreeMusic(map->music.friendly);
        map->music.friendly = NULL;
    }

    if (map->tiles.index != NULL) {
        DARR_FREE(map->tiles.index);
    }

    /* - Conditions - */
    if (map->conditions.death_enemy != NULL) {
        DARR_FREE(map->conditions.death_enemy);
        map->conditions.death_enemy = NULL;
    }
    if (map->conditions.death_friendly != NULL) {
        DARR_FREE(map->conditions.death_friendly);
        map->conditions.death_friendly = NULL;
    }
    if (map->conditions.turn_end != NULL) {
        DARR_FREE(map->conditions.turn_end);
        map->conditions.turn_end = NULL;
    }
    if (map->conditions.waits_friendly != NULL) {
        DARR_FREE(map->conditions.waits_friendly);
        map->conditions.waits_friendly = NULL;
    }

    /* - Edges - */
    if (map->perimiter.edges_danger != NULL) {
        SDL_free(map->perimiter.edges_danger);
        map->perimiter.edges_danger = NULL;
    }
    if (map->entities.breakables != NULL) {
        DARR_FREE(map->entities.breakables);
        map->entities.breakables = NULL;
    }
    if (map->entities.chests != NULL) {
        DARR_FREE(map->entities.chests);
        map->entities.chests = NULL;
    }
    if (map->entities.doors != NULL) {
        DARR_FREE(map->entities.doors);
        map->entities.doors = NULL;
    }

    Map_Tilesets_Free(map);
    Map_Tilemap_Texture_Free(map);
    Map_Tiles_Free(map);
    if (map->units.onfield.friendlies != NULL) {
        DARR_FREE(map->units.onfield.friendlies);
        map->units.onfield.friendlies = NULL;
    }
    if (map->reinforcements.equipments != NULL) {
        for (size_t i = 0; i < DARR_NUM(map->reinforcements.equipments); i++) {
            DARR_FREE(map->reinforcements.equipments[i]);
        }
        DARR_FREE(map->reinforcements.equipments);
        map->reinforcements.equipments = NULL;
    }
    if (map->reinforcements.items_num != NULL) {
        DARR_FREE(map->reinforcements.items_num);
        map->reinforcements.items_num = NULL;
    }
    if (map->darrs.tilemap != NULL) {
        SDL_free(map->darrs.tilemap);
        map->darrs.tilemap = NULL;
    }
    if (map->tiles.arr != NULL) {
        DARR_FREE(map->tiles.arr);
        map->tiles.arr = NULL;
    }
    if (map->tiles.id != NULL) {
        DARR_FREE(map->tiles.id);
        map->tiles.id = NULL;
    }
    if (map->armies.onfield != NULL) {
        DARR_FREE(map->armies.onfield);
        map->armies.onfield = NULL;
    }
    if (map->start_pos.arr != NULL) {
        DARR_FREE(map->start_pos.arr);
        map->start_pos.arr = NULL;
    }
    if (map->units.onfield.enemies != NULL) {
        DARR_FREE(map->units.onfield.enemies);
        map->units.onfield.enemies = NULL;
    }
    if (map->units.onfield.arr != NULL) {
        DARR_FREE(map->units.onfield.arr);
        map->units.onfield.arr = NULL;
    }
    Map_Tilemap_Surface_Free(map);
    Map_Reinforcements_Free(map);
    Arrow_Free(map->arrow);
    if (map->render.tilemap_shader != NULL) {
        Tilemap_Shader_Free(map->render.tilemap_shader);
        SDL_free(map->render.tilemap_shader);
        map->render.tilemap_shader = NULL;
    }
    if (map->render.texture != NULL) {
        SDL_DestroyTexture(map->render.texture);
        map->render.texture = NULL;
    }
    if (map->darrs.unitmap != NULL) {
        SDL_free(map->darrs.unitmap);
        map->darrs.unitmap = NULL;
    }
    if (map->darrs.costmap != NULL) {
        SDL_free(map->darrs.costmap);
        map->darrs.costmap = NULL;
    }
    if (map->darrs.movemap != NULL) {
        SDL_free(map->darrs.movemap);
        map->darrs.movemap = NULL;
    }
    if (map->start_pos.map != NULL) {
        SDL_free(map->start_pos.map);
        map->start_pos.map = NULL;
    }

    if (map->darrs.attacktomap != NULL) {
        SDL_free(map->darrs.attacktomap);
        if (map->darrs.attacktomap == map->darrs.healtomap) {
            map->darrs.healtomap = NULL;
        }
        map->darrs.attacktomap = NULL;
    }
    if (map->darrs.attackfrommap != NULL) {
        SDL_free(map->darrs.attackfrommap);
        map->darrs.attackfrommap = NULL;
    }
    if (map->darrs.healtomap != NULL) {
        SDL_free(map->darrs.healtomap);
        map->darrs.healtomap = NULL;
    }
    if (map->darrs.healfrommap != NULL) {
        SDL_free(map->darrs.healfrommap);
        map->darrs.healfrommap = NULL;
    }
    if (map->darrs.dangermap != NULL) {
        SDL_free(map->darrs.dangermap);
        map->darrs.dangermap = NULL;
    }
    if (map->darrs.temp != NULL) {
        SDL_free(map->darrs.temp);
        map->darrs.temp = NULL;
    }
    if (map->darrs.global_rangemap != NULL) {
        SDL_free(map->darrs.global_rangemap);
        map->darrs.global_rangemap = NULL;
    }
    if (map->palette.map != NULL) {
        SDL_free(map->palette.map);
        map->palette.map = NULL;
    }
    if (map->palette.temp != NULL) {
        SDL_free(map->palette.temp);
        map->palette.temp = NULL;
    }
    if (map->stack.dangermap != NULL) {
        SDL_free(map->stack.dangermap);
        map->stack.dangermap = NULL;
    }
    if (map->stack.global_dangermap != NULL) {
        SDL_free(map->stack.global_dangermap);
        map->stack.global_dangermap = NULL;
    }
    if (map->darrs.global_dangermap != NULL) {
        SDL_free(map->darrs.global_dangermap);
        map->darrs.global_dangermap = NULL;
    }

    Map_Members_Free(map);
}

void Map_Tilesize_Set(struct Map *map, i32 width, i32 height) {
    SDL_assert(width  > 0);
    SDL_assert(height > 0);
    SDL_assert(width  == SOTA_TILESIZE);
    SDL_assert(height == SOTA_TILESIZE);
    map->tilesize[0] = width;
    map->tilesize[1] = height;
    if (map->arrow) {
        map->arrow->map_tilesize[0] = width;
        map->arrow->map_tilesize[1] = height;
    }
}

void Map_Size_Set(struct Map *map, i32 col_len, i32 row_len) {
    map->row_len = row_len;
    map->col_len = col_len;

    SDL_assert(map->row_len > 0);
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len < MAP_MAX_COLS);

    SDL_assert(map->col_len < MAP_MAX_ROWS);
}

void Map_Members_Alloc(struct Map *map) {
    SDL_assert(map->row_len > 0);
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len < MAP_MAX_COLS);
    SDL_assert(map->col_len < MAP_MAX_ROWS);
    int len = map->row_len * map->col_len;

    SDL_assert(map->entities.doors == NULL);
    map->entities.doors = DARR_INIT(map->entities.doors, tnecs_entity, 4);

    SDL_assert(map->entities.breakables == NULL);
    map->entities.breakables = DARR_INIT(map->entities.breakables, tnecs_entity, 4);

    SDL_assert(map->entities.chests == NULL);
    map->entities.chests = DARR_INIT(map->entities.chests, tnecs_entity, 4);

    Map_Texture_Alloc(map);
    Map_Tilemap_Shader_Init(map);
    SDL_assert(map->darrs.tilemap == NULL);
    map->darrs.tilemap = SDL_calloc(len, sizeof(*map->darrs.tilemap));

    SDL_assert(map->start_pos.arr == NULL);
    map->start_pos.arr = DARR_INIT(map->start_pos.arr, struct Point, 16);

    SDL_assert(map->conditions.death_enemy == NULL);
    map->conditions.death_enemy = DARR_INIT(map->conditions.death_enemy, struct Map_condition, 2);

    SDL_assert(map->conditions.death_friendly == NULL);
    map->conditions.death_friendly = DARR_INIT(map->conditions.death_friendly, struct Map_condition, 2);

    SDL_assert(map->reinforcements.arr == NULL);
    map->reinforcements.arr = DARR_INIT(map->reinforcements.arr, struct Reinforcement, 16);

    SDL_assert(map->reinforcements.items_num == NULL);
    map->reinforcements.items_num = DARR_INIT(map->reinforcements.items_num, u8, SOTA_EQUIPMENT_SIZE);

    SDL_assert(map->tiles.index == NULL);
    map->tiles.index = DARR_INIT(map->tiles.index, i32, DEFAULT_TILESPRITE_BUFFER);

    SDL_assert(map->darrs.healtolist == NULL);
    map->darrs.healtolist = DARR_INIT(map->darrs.healtolist, i32, 32);

    SDL_assert(map->darrs.attacktolist == NULL);
    map->darrs.attacktolist = DARR_INIT(map->darrs.attacktolist, i32, 32);

    SDL_assert(map->darrs.attackfromlist == NULL);
    map->darrs.attackfromlist = DARR_INIT(map->darrs.attackfromlist, i32, 32);

    SDL_assert(map->units.onfield.enemies == NULL);
    map->units.onfield.enemies = DARR_INIT(map->units.onfield.enemies, tnecs_entity, 20);

    SDL_assert(map->units.onfield.friendlies == NULL);
    map->units.onfield.friendlies = DARR_INIT(map->units.onfield.friendlies, tnecs_entity, 20);

    SDL_assert(map->units.onfield.arr == NULL);
    map->units.onfield.arr = DARR_INIT(map->units.onfield.arr, tnecs_entity, 20);

    SDL_assert(map->reinforcements.equipments == NULL);
    map->reinforcements.equipments = DARR_INIT(map->reinforcements.equipments, Inventory_item *, 30);

    SDL_assert(map->armies.onfield == NULL);
    map->armies.onfield = DARR_INIT(map->armies.onfield, i32, 5);

    SDL_assert(map->darrs.temp == NULL);
    map->darrs.temp = SDL_calloc(len,  sizeof(*map->darrs.temp));

    SDL_assert(map->darrs.unitmap == NULL);
    map->darrs.unitmap = SDL_calloc(len,  sizeof(*map->darrs.unitmap));

    SDL_assert(map->darrs.costmap == NULL);
    map->darrs.costmap = SDL_calloc(len,  sizeof(*map->darrs.costmap));

    SDL_assert(map->darrs.movemap == NULL);
    map->darrs.movemap = SDL_calloc(len,  sizeof(*map->darrs.movemap));

    SDL_assert(map->start_pos.map == NULL);
    map->start_pos.map = SDL_calloc(len,  sizeof(*map->start_pos.map));

    SDL_assert(map->darrs.dangermap == NULL);
    map->darrs.dangermap = SDL_calloc(len,  sizeof(*map->darrs.dangermap));

    SDL_assert(map->palette.map == NULL);
    map->palette.map = SDL_malloc(len * sizeof(*map->palette.map));

    SDL_assert(map->darrs.attacktomap == NULL);
    map->darrs.attacktomap = SDL_calloc(len,  sizeof(*map->darrs.attacktomap));

    SDL_assert(map->darrs.healtomap == NULL);
    map->darrs.healtomap = SDL_calloc(len,  sizeof(*map->darrs.healtomap));

    SDL_assert(map->darrs.healfrommap == NULL);
    map->darrs.healfrommap = SDL_calloc(len,  sizeof(*map->darrs.healfrommap));

    SDL_assert(map->palette.temp == NULL);
    map->palette.temp = SDL_malloc(len * sizeof(*map->palette.temp));

    SDL_assert(map->darrs.attackfrommap == NULL);
    map->darrs.attackfrommap = SDL_calloc(len,  sizeof(*map->darrs.attackfrommap));

    SDL_assert(map->darrs.global_rangemap == NULL);
    map->darrs.global_rangemap = SDL_calloc(len,  sizeof(*map->darrs.global_rangemap));

    SDL_assert(map->darrs.global_dangermap == NULL);
    map->darrs.global_dangermap = SDL_calloc(len,  sizeof(*map->darrs.global_dangermap));

    SDL_assert(map->perimiter.edges_danger == NULL);
    map->perimiter.edges_danger = SDL_calloc(len, sizeof(*map->perimiter.edges_danger));

    if (map->renderer != NULL) {
        Map_Tilemap_Surface_Init(map);
        Map_Tilemap_Texture_Init(map);

        SDL_assert(map->arrow == NULL);
        map->arrow = Arrow_Init(map->tilesize);
        SDL_assert(map->arrow != NULL);
        Arrow_Textures_Load(map->arrow, ARROW_FILENAME, map->renderer);
        SDL_assert(map->arrow->textures != NULL);
    } else {
        SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Map: Renderer is NULL, skipping related member allocs");
    }

    // SDL_Log("map->stack.mode %d %d", map->stack.mode, MAP_SETTING_STACK_DANGERMAP);
    if (map->stack.mode == MAP_SETTING_STACK_DANGERMAP) {
        // SDL_Log("Allocating stack.dangermap");
        SDL_assert(map->stack.dangermap == NULL);
        map->stack.dangermap          = SDL_calloc(len, sizeof(*map->stack.dangermap));
        SDL_assert(map->stack.global_dangermap == NULL);
        map->stack.global_dangermap   = SDL_calloc(len, sizeof(*map->stack.global_dangermap));
    }

    Map_Palettemap_Reset(map);
}

void Map_Members_Free(struct Map *map) {
    if (map->darrs.attacktolist != NULL) {
        DARR_FREE(map->darrs.attacktolist);
        map->darrs.attacktolist = NULL;
    }
    if (map->darrs.attackfromlist != NULL) {
        DARR_FREE(map->darrs.attackfromlist);
        map->darrs.attackfromlist = NULL;
    }

    if (map->darrs.healtolist != NULL) {
        DARR_FREE(map->darrs.healtolist);
        map->darrs.healtolist = NULL;
    }
}

void Map_Texture_Alloc(struct Map *map) {
    if (map->render.texture != NULL)
        SDL_DestroyTexture(map->render.texture);

    map->render.texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, map->tilesize[0] * map->col_len,
                                            map->tilesize[1] * map->row_len);
    SDL_SetTextureBlendMode(map->render.texture, SDL_BLENDMODE_BLEND);

}

void Map_Tilemap_Texture_Free(struct Map *map) {
    if (map->tiles.tilemap_texture != NULL) {
        SDL_DestroyTexture(map->tiles.tilemap_texture);
        map->tiles.tilemap_texture = NULL;
    }
}

void Map_Tilemap_Texture_Init(struct Map *map) {
    SDL_assert(map->renderer);
    Map_Tilemap_Texture_Free(map);
    int x_size = map->tilesize[0] * map->col_len;
    int y_size = map->tilesize[1] * map->row_len;
    map->tiles.tilemap_texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_RGB888,
                                                   SDL_TEXTUREACCESS_TARGET, x_size, y_size);
    SDL_assert(map->tiles.tilemap_texture);
}

void Map_Tilemap_Surface_Free(struct Map *map) {
    if (map->tiles.tilemap_surface != NULL) {
        SDL_FreeSurface(map->tiles.tilemap_surface);
        map->tiles.tilemap_surface = NULL;
    }
}

void Map_Tilemap_Surface_Init(struct Map *map) {
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len > 0);
    SDL_assert(map->col_len < MAP_MAX_COLS);
    SDL_assert(map->row_len < MAP_MAX_ROWS);

    Map_Tilemap_Surface_Free(map);
    int x_size = map->tilesize[0] * map->col_len;
    int y_size = map->tilesize[1] * map->row_len;
    map->tiles.tilemap_surface = Filesystem_indexedSurface_Init(x_size, y_size);
    SDL_assert(map->tiles.tilemap_surface->w == map->tilesize[0] * map->col_len);
    SDL_assert(map->tiles.tilemap_surface->h == map->tilesize[1] * map->row_len);
}

/* --- I/O --- */
void Map_writeJSON(const void *input, cJSON *jmap) {
    struct Map *map = (struct Map *) input;
    SDL_assert(jmap != NULL);
    /* -- Preliminaries -- */
    cJSON *jchapter         = cJSON_CreateNumber(map->chapter);
    cJSON_AddItemToObject(jmap, "chapter", jchapter);
    cJSON *jstart_pos_arr   = cJSON_CreateObject();
    cJSON *jmap_size        = cJSON_CreateObject();
    cJSON *jrow_len         = cJSON_CreateNumber(map->row_len);
    cJSON *jcol_len         = cJSON_CreateNumber(map->col_len);
    cJSON_AddItemToObject(jmap_size, "row_len", jrow_len);
    cJSON_AddItemToObject(jmap_size, "col_len", jcol_len);
    cJSON_AddItemToObject(jmap,     "Map size", jmap_size);

    /* -- Writing Starting Positions -- */
    cJSON_AddItemToObject(jmap, "StartingPositions", jstart_pos_arr);
    cJSON *jstartingpos;
    cJSON *jstartingposx;
    cJSON *jstartingposy;
    SDL_assert(map->start_pos.arr != NULL);
    for (size_t i = 0; i < DARR_NUM(map->start_pos.arr); i++) {
        jstartingpos = cJSON_CreateObject();
        cJSON_AddItemToObject(jstart_pos_arr, "Position", jstartingpos);
        jstartingposx = cJSON_CreateNumber(map->start_pos.arr[i].x);
        jstartingposy = cJSON_CreateNumber(map->start_pos.arr[i].y);
        cJSON_AddItemToObject(jstartingpos, "col", jstartingposx);
        cJSON_AddItemToObject(jstartingpos, "row", jstartingposy);
    }

    /* -- Writing Tiles -- */
    cJSON *jtiles = cJSON_CreateObject();
    cJSON_AddItemToObject(jmap, "tiles", jtiles);
    cJSON *jtile;
    cJSON *jtilename;
    cJSON *jtileid;
    u64 temp_tile_id;
    for (size_t i = 0; i < DARR_NUM(map->tiles.arr); i++) {
        jtile = cJSON_CreateObject();
        if (map->tiles.index[i] >= TILE_END) {
            temp_tile_id = map->tiles.index[i];
            jtilename   = cJSON_CreateString(global_tilenames[temp_tile_id].data);
            jtileid     = cJSON_CreateNumber(temp_tile_id);
        } else {
            jtilename   = cJSON_CreateString(global_tilenames[map->tiles.index[i]].data);
            jtileid     = cJSON_CreateNumber(map->tiles.index[i]);
        }
        cJSON_AddItemToObject(jtile, "id", jtileid);
        cJSON_AddItemToObject(jtile, "name", jtilename);
        cJSON_AddItemToObject(jtiles, "tile", jtile);
    }
    /* -- Writing Reinforcements -- */
    struct Inventory_item *temp_equip, temp_item;
    SDL_assert((map->reinforcements.equipments != NULL) & (map->reinforcements.arr != NULL));
    cJSON *jreinforcement;
    cJSON *jreinforcementeq;
    cJSON *jreinforcements = cJSON_CreateObject();
    cJSON_AddItemToObject(jmap, "Reinforcements", jreinforcements);
    for (i32 r = 0; r < DARR_NUM(map->reinforcements.arr); r++) {
        jreinforcement   = cJSON_CreateObject();
        jreinforcementeq = cJSON_CreateObject();
        Reinforcement_writeJSON(jreinforcement, &(map->reinforcements.arr)[r]);
        temp_equip = map->reinforcements.equipments[r];
        for (i32 i = 0; i < DARR_NUM(temp_equip); i++) {
            temp_item = temp_equip[i];
            if (temp_item.id > ITEM_NULL)
                Inventory_item_writeJSON(&temp_item, jreinforcementeq);
        }
        cJSON_AddItemToObject(jreinforcement, "Equipment", jreinforcementeq);
        cJSON_AddItemToObject(jreinforcements, "Reinforcement", jreinforcement);
    }
    cJSON *jtilemap = cJSON_CreateObject();
    Array2D_writeJSON(jtilemap, map->darrs.tilemap, map->row_len, map->col_len);
    cJSON_AddItemToObject(jmap, "Tilemap", jtilemap);
}

void Map_RowCol_readJSON(s8 filename, i32 rowcol[TWO_D]) {
    // SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var((char*)filename.data));
    SDL_assert(jfile != NULL);

    s8 elem_name = jsonElementnames[JSON_MAP];
    struct cJSON *jmap = cJSON_GetObjectItem(jfile, (char*)elem_name.data);
    if (jmap == NULL) {
        SDL_Log("No '%s' element in map json", elem_name.data);
        exit(ERROR_JSONParsingFailed);
    }

    struct cJSON *jmap_size = cJSON_GetObjectItem(jmap, "Map size");
    if (jmap_size == NULL) {
        SDL_Log("No 'Map Size' element in map element");
        exit(ERROR_JSONParsingFailed);
    }

    cJSON *jrow_len     = cJSON_GetObjectItem(jmap_size,    "row_len");
    cJSON *jcol_len     = cJSON_GetObjectItem(jmap_size,    "col_len");

    rowcol[SOTA_COL_INDEX] = cJSON_GetNumberValue(jcol_len);
    rowcol[SOTA_ROW_INDEX] = cJSON_GetNumberValue(jrow_len);

    if (jfile != NULL)
        cJSON_Delete(jfile);
}

void Map_readJSON(void *input, const cJSON *jmap) {
    struct Map *map = (struct Map *) input;
    SDL_assert(map->conditions.death_enemy         != NULL);
    SDL_assert(map->conditions.death_friendly      != NULL);
    SDL_assert(map->reinforcements.arr  != NULL);
    SDL_assert(map->reinforcements.equipments    != NULL);
    SDL_assert(map->reinforcements.items_num != NULL);
    SDL_assert(map->entities.chests          != NULL);
    SDL_assert(map->entities.breakables      != NULL);
    SDL_assert(map->entities.doors           != NULL);

    SDL_assert(jmap != NULL);
    /* -- Read party filename (for debug) -- */
    cJSON *jparty = cJSON_GetObjectItem(jmap, "party");
    if (jparty != NULL)
        map->party_filename = s8_mut(cJSON_GetStringValue(jparty));

    /* -- Read map info -- */
    cJSON *jchapter     = cJSON_GetObjectItem(jmap,         "chapter");
    cJSON *jmap_size    = cJSON_GetObjectItem(jmap,         "Map size");
    cJSON *jrow_len     = cJSON_GetObjectItem(jmap_size,    "row_len");
    cJSON *jcol_len     = cJSON_GetObjectItem(jmap_size,    "col_len");

    cJSON *jcamera      = cJSON_GetObjectItem(jmap,     "Camera");
    cJSON *joffset      = cJSON_GetObjectItem(jcamera,  "offset");
    cJSON *jzoom        = cJSON_GetObjectItem(jcamera,  "zoom");
    if (joffset != NULL) {
        SDL_assert(cJSON_IsArray(joffset));
        SDL_assert(cJSON_GetArraySize(joffset) == TWO_D);
    }

    map->camera.offset.x = cJSON_GetNumberValue(cJSON_GetArrayItem(joffset, 0));
    map->camera.offset.y = cJSON_GetNumberValue(cJSON_GetArrayItem(joffset, 1));
    map->camera.zoom     = cJSON_GetNumberValue(jzoom);

    map->chapter = cJSON_GetNumberValue(jchapter);
    map->chapter = cJSON_GetNumberValue(jchapter);

    if (map->col_len != cJSON_GetNumberValue(jcol_len)) {
        SDL_Log("map->col_len should be set to '%f'", cJSON_GetNumberValue(jcol_len));
        exit(ERROR_Generic);
    }

    if (map->row_len != cJSON_GetNumberValue(jrow_len)) {
        SDL_Log("map->row_len should be set to '%f'", cJSON_GetNumberValue(jrow_len));
        exit(ERROR_Generic);
    }

    if (map->arrow) {
        map->arrow->col_len = map->col_len;
        map->arrow->row_len = map->row_len;
    }

    /* -- Read Starting Positions -- */
    // SDL_Log("Read Starting Positions");
    cJSON *jstart_pos_arr = cJSON_GetObjectItem(jmap, "Starting Positions");
    SDL_assert(cJSON_IsArray(jstart_pos_arr));

    cJSON *jstart_pos;
    struct Point pos;
    cJSON_ArrayForEach(jstart_pos, jstart_pos_arr) {
        Point_readJSON((struct Point *)&pos, jstart_pos);
        Map_startingPos_Add(map, pos.x, pos.y);
    }

    /* -- Read tiles -- */
    // SDL_Log("Read tiles");
    cJSON *jtiles = cJSON_GetObjectItem(jmap, "tiles");
    cJSON *jid, *jtile;
    cJSON_ArrayForEach(jtile, jtiles) {
        jid = cJSON_GetObjectItem(jtile, "id");
        DARR_PUT(map->tiles.index, cJSON_GetNumberValue(jid));
    }
    // SDL_assert(map->tileset_surfaces == NULL);
    Map_Tilesets_Free(map);
    Map_Tiles_Free(map);

    Map_Tiles_Load(map);
    Map_Tilesets_Load(map);
    SDL_assert(map->tiles.tileset_surfaces[0]);

    /* -- Read Reinforcements --  */
    // SDL_Log("Read Reinforcements");

    cJSON *jreinforcements = cJSON_GetObjectItem(jmap, "Reinforcements");
    SDL_assert(jreinforcements != NULL);
    cJSON *jequipment, *jitem;
    struct Inventory_item temp_item = Inventory_item_default;
    struct Inventory_item *temp_equip = NULL;

    for (int i = 0; i < cJSON_GetArraySize(jreinforcements); i++) {
        struct Reinforcement temp_rein = Reinforcement_default;
        cJSON *jreinforcement = cJSON_GetArrayItem(jreinforcements, i);
        Reinforcement_readJSON(jreinforcement, &temp_rein);
        DARR_PUT(map->reinforcements.arr, temp_rein);
        jequipment = cJSON_GetObjectItem(jreinforcement, "Equipment");
        temp_equip = DARR_INIT(temp_equip, struct Inventory_item, SOTA_EQUIPMENT_SIZE);
        /* FIRST ITEM IS NULL */
        temp_item = Inventory_item_default;
        DARR_PUT(temp_equip, temp_item);

        if (!cJSON_IsArray(jequipment)) {
            SDL_Log("Missing Equipment array in Reinforcement %d", i);
            continue;
        }

        DARR_PUT(map->reinforcements.items_num, cJSON_GetArraySize(jequipment));
        cJSON_ArrayForEach(jitem, jequipment) {
            temp_item = Inventory_item_default;
            Inventory_item_readJSON(&temp_item, jitem);
            // NOTE:    Do not ignore empty items.
            //          To be able to put weapons in right hand.
            DARR_PUT(temp_equip, temp_item);
        }
        DARR_PUT(map->reinforcements.equipments, temp_equip);
    }

    /* -- Read Map objects -- */
    // SDL_Log("Read Map objects");
    cJSON *jmobj        = cJSON_GetObjectItem(jmap,  "Map_Objects");
    cJSON *jdoors       = cJSON_GetObjectItem(jmobj, "Doors");
    cJSON *jchests      = cJSON_GetObjectItem(jmobj, "Chests");
    cJSON *jbreakables  = cJSON_GetObjectItem(jmobj, "Breakables");

    SDL_assert(map->world != NULL);

    /* -- Loading chests -- */
    do { /*Loop never executes, just used for break*/
        if (jchests == NULL)
            break;

        SDL_assert(cJSON_IsArray(jchests));

        DARR_NUM(map->entities.chests) = 0;

        for (size_t i = 0; i < DARR_NUM(map->entities.chests); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Chest_ID, Position_ID);
            struct Chest    *chest  = IES_GET_COMPONENT(map->world, temp_ent, Chest);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos      != NULL);
            SDL_assert(chest    != NULL);
            cJSON *jchest   = cJSON_GetArrayItem(jchests, i);
            cJSON *jpos     = cJSON_GetObjectItem(jchest, "position");

            if (jchest == NULL) {
                SDL_Log("Warning: could not read chest %ld", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read chest %ld's position", i);
                continue;
            }

            Point_readJSON((struct Point *)&pos->tilemap_pos, jpos);
            Chest_readJSON(chest, jchest);

            DARR_PUT(map->entities.chests, temp_ent);
        }
    } while (0);

    /* -- Loading doors -- */
    do { /*Loop never executes, just used for break*/
        if (jdoors == NULL)
            break;
        SDL_assert(cJSON_IsArray(jdoors));

        DARR_NUM(map->entities.doors) = 0;

        for (size_t i = 0; i < cJSON_GetArraySize(jdoors); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Door_ID, Position_ID);
            struct Door     *door   = IES_GET_COMPONENT(map->world, temp_ent, Door);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos  != NULL);
            SDL_assert(door != NULL);

            cJSON *jdoor    = cJSON_GetArrayItem(jdoors, i);
            cJSON *jpos     = cJSON_GetObjectItem(jdoor, "position");
            if (jdoor == NULL) {
                SDL_Log("Warning: could not read door %ld", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read door %ld's position", i);
                continue;
            }
            Point_readJSON((struct Point *)&pos->tilemap_pos, jpos);
            Door_readJSON(door, jdoor);

            DARR_PUT(map->entities.doors, temp_ent);
        }
    } while (0);

    /* -- Loading breakables -- */
    do { /*Loop never executes, just used for break*/
        // Note: Breakable component should be added to already existing DOORS/CHESTS
        if (jbreakables == NULL)
            break;

        SDL_assert(cJSON_IsArray(jbreakables));

        DARR_NUM(map->entities.breakables) = 0;

        for (size_t i = 0; i < cJSON_GetArraySize(jbreakables); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Breakable_ID, Position_ID);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos != NULL);
            cJSON *jbreakable       = cJSON_GetArrayItem(jbreakables, i);
            cJSON *jpos             = cJSON_GetObjectItem(jbreakable, "position");
            if (jbreakable == NULL) {
                SDL_Log("Warning: could not read breakable %ld", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read breakable %ld's position", i);
                continue;
            }

            Point_readJSON((struct Point *)&pos->tilemap_pos, jpos);

            // if position of breakaable is already a Door/Chest
            // -> add Breakable component to Door/Chest instead
            // -> add Door/Chest + breakable entity to breakable list
            int x = pos->tilemap_pos.x, y = pos->tilemap_pos.y;
            tnecs_entity door_ent     = Map_Find_Door_Ent(map, x, y);
            tnecs_entity chest_ent    = Map_Find_Chest_Ent(map, x, y);

            if ((door_ent) || (chest_ent)) {
                tnecs_entity_destroy(map->world, temp_ent);
                temp_ent = door_ent > TNECS_NULL ? door_ent : chest_ent;
                TNECS_ADD_COMPONENT(map->world, temp_ent, Breakable_ID);
            }

            struct Breakable *breaka = IES_GET_COMPONENT(map->world, temp_ent, Breakable);
            SDL_assert(breaka != NULL);
            Breakable_readJSON(breaka, jbreakables);

            DARR_PUT(map->entities.breakables, temp_ent);
        }
    } while (0);

    cJSON *jframe;
    cJSON *jarray;
    cJSON *jtilemap = cJSON_GetObjectItem(jmap, "tilemap");
    cJSON *jframes  = cJSON_GetObjectItem(jtilemap, "frames");
    map->render.frames     = cJSON_GetNumberValue(jframes);

    cJSON_ArrayForEach(jframe, jframes) {
        // TODO: tilemaps[i]
        jarray = cJSON_GetObjectItem(jframe, "array");
        Array2D_readJSON(jarray, map->darrs.tilemap, map->row_len, map->col_len);
        SDL_assert(map->darrs.tilemap);
    }

    /* - Map objects override tiles - */
    Map_Tilemap_MapObjects(map);

    /* -- Misc. Computations -- */
    Map_swappedTextures_All(map);
    map->render.tilemap_shader->frames = map->render.frames;
    Tilemap_Shader_Load_Tilemap_JSON(map->render.tilemap_shader, jmap);
    // SDL_assert(map->render.tilemap_shader->shadow_tilemaps);

    /* --- Parsing shadow tileset --- */
    if (map->render.tilemap_shader->shadow_tilemaps) {
        s8 path = s8_var(PATH_JOIN("assets", "tiles", "Tileset_Shadow.json"));
        struct cJSON *jshadow_tileset_file  = jsonio_parseJSON(path);
        struct cJSON *jshadow_tileset       = cJSON_GetObjectItem(jshadow_tileset_file, "shadow_tileset");
        SDL_assert(jshadow_tileset);
        Tilemap_Shader_Load_Tileset_JSON(map->render.tilemap_shader, jshadow_tileset);
        cJSON_Delete(jshadow_tileset_file);
    }

    /* --- Parsing conditions --- */

    cJSON *jmap_conditions  = cJSON_GetObjectItem(jmap, "Conditions");
    cJSON *jdeath_enemy     = cJSON_GetObjectItem(jmap_conditions, "death_enemy");
    cJSON *jdeath_friendly  = cJSON_GetObjectItem(jmap_conditions, "death_friendly");
    cJSON *jmap_condition;

    cJSON_ArrayForEach(jmap_condition, jdeath_enemy) {
        struct Map_condition map_cond = Map_condition_default;
        Map_Condition_readJSON(&map_cond, jmap_condition);
        DARR_PUT(map->conditions.death_enemy, map_cond);
    }

    cJSON_ArrayForEach(jmap_condition, jdeath_friendly) {
        struct Map_condition map_cond = Map_condition_default;
        Map_Condition_readJSON(&map_cond, jmap_condition);
        DARR_PUT(map->conditions.death_friendly, map_cond);
    }
}

/* --- Map events / Triggers --- */
/* Ouputs index of army in armies.onfield*/
i32 Map_Army_Next(struct Map *map) {
    SDL_assert(map->turn > 0);
    SDL_assert(map->turn < SOTA_MAX_TURNS);
    /* Get next army in line for control */
    SDL_assert(map->armies.onfield != NULL);

    /* Get number of armies on field */
    size_t army_num = DARR_NUM(map->armies.onfield);
    SDL_assert(army_num <= ARMY_NUM);

    /* Check for wrap around back to player */
    map->armies.current = (map->armies.current + (map->turn > 1)) % army_num;
    SDL_assert(map->armies.current >= 0);
    SDL_assert(map->armies.current <= army_num);

    return (map->armies.current);
}

void Map_Turn_Increment(struct Map *map) {
    map->turn++;
}

/* --- Music --- */
void Map_Music_Load(struct Map *map) {
    map->music.enemy       = Music_Load(map->music.i_enemy);
    map->music.friendly    = Music_Load(map->music.i_friendly);
}

/* --- Boss --- */
b32 Map_Boss_Alive(struct Map *map, i16 army) {
    // TODO:
    return (true);
}

/* --- Tile --- */
struct Tile *Map_Tile_Get(struct Map *map, i32 x, i32 y) {
    SDL_assert(map              != NULL);
    SDL_assert(map->tiles.arr   != NULL);
    i32 index = sota_2D_index(x, y, map->col_len);
    i32 tile_ind = map->darrs.tilemap[index] / TILE_DIVISOR;
    size_t tile_order = Map_Tile_Order(map, tile_ind);
    return (map->tiles.arr + tile_order);
}

/* --- Bonus --- */
void Map_Bonus_Remove_Instant(struct Map *map, i32 army) {
    tnecs_entity *entities = Map_Get_onField(map, army);
    SDL_assert(entities != NULL);

    size_t num_ent = DARR_NUM(entities);
    for (int i = 0; i < num_ent; i++) {
        tnecs_entity ent = entities[i];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(map->world, ent, Unit);
        SDL_assert(unit != NULL);
        Unit_Bonus_Instant_Decay(unit);
    }
}

void Map_Bonus_Remove_Persistent(struct Map *map, i32 army) {
    tnecs_entity *entities = Map_Get_onField(map, army);
    SDL_assert(entities != NULL);

    size_t num_ent = DARR_NUM(entities);
    for (int i = 0; i < num_ent; i++) {
        tnecs_entity ent = entities[i];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(map->world, ent, Unit);
        SDL_assert(unit != NULL);
        Unit_Bonus_Persistent_Decay(unit);
    }
}

void Map_Aura_Apply(struct Map *map, struct Aura aura, tnecs_entity *entities,
                    tnecs_entity source_ent, u16 item, u16 skill, b32 active, b32 instant) {
    // SDL_Log("Map_Aura_Apply");
    /* aura:                bonus to apply.                  */
    /* entities:            units to appy bonus to.          */
    /* aura source info:    source_ent, item, skill, active. */

    /* Apply standard bonus to all unit in range */
    struct Bonus_Stats  bonus       = Aura2Bonus(&aura, source_ent, item, skill, active, instant);
    struct Position    *source_pos  = IES_GET_COMPONENT(map->world, source_ent, Position);
    SDL_assert(source_pos != NULL);

    size_t num_ent              = DARR_NUM(entities);
    for (int i = 0; i < num_ent; i++) {
        tnecs_entity ent = entities[i];
        SDL_assert(ent > TNECS_NULL);
        struct Position *pos = IES_GET_COMPONENT(map->world, ent, Position);

        i32 distance = Pathfinding_Manhattan(source_pos->tilemap_pos, pos->tilemap_pos);
        // SDL_Log("source %d %d", source_pos->tilemap_pos.x, source_pos->tilemap_pos.y);
        // SDL_Log("pos %d %d", pos->tilemap_pos.x, pos->tilemap_pos.y);
        // SDL_Log("distance %d", distance);
        // SDL_Log("aura %d %d", aura.range.min, aura.range.max);
        if ((distance >= aura.range.min) && (distance <= aura.range.max)) {
            struct Unit *unit = IES_GET_COMPONENT(map->world, ent, Unit);
            SDL_assert(unit != NULL);
            Unit_Bonus_Refresh(unit, bonus);
        }
    }

}

void Map_Bonus_Standard_Apply_Unit(struct Map *map, tnecs_entity ent, tnecs_entity *entities) {
    /* Apply passive instant standard bonus to unit */
    // SDL_Log("Map_Bonus_Standard_Apply_Unit");
    SDL_assert(ent > TNECS_NULL);
    struct Unit     *unit   = IES_GET_COMPONENT(map->world, ent, Unit);
    struct Position *pos    = IES_GET_COMPONENT(map->world, ent, Position);
    SDL_assert(pos          != NULL);
    SDL_assert(unit         != NULL);
    SDL_assert(Unit_Class(unit)  == UNIT_CLASS_STANDARD_BEARER);
    b32 active  = false;
    b32 instant = true;
    u16 skill   = SKILL_START;

    /* Check if unit has a standard equipped */
    const struct Weapon *wpnL = Unit_Equipped_Weapon(unit, UNIT_HAND_LEFT);
    if ((wpnL != NULL) && (wpnL->item->type == ITEM_TYPE_STANDARD))
        Map_Aura_Apply(map, wpnL->item->aura, entities, ent, wpnL->item->id, skill, active, instant);

    const struct Weapon *wpnR = Unit_Equipped_Weapon(unit, UNIT_HAND_RIGHT);
    if ((wpnR != NULL) && (wpnR->item->type == ITEM_TYPE_STANDARD))
        Map_Aura_Apply(map, wpnR->item->aura, entities, ent, wpnR->item->id, skill, active, instant);
}

void Map_Bonus_Standard_Apply(struct Map *map, i32 army) {
    SDL_assert(map != NULL);
    SDL_assert((army > ARMY_START) && (army < ARMY_END));

    /* Get army */
    tnecs_entity *entities = Map_Get_onField(map, army);
    SDL_assert(entities != NULL);

    /* Check if any unit in army is a standard bearer */
    size_t num_ent = DARR_NUM(entities);
    for (int i = 0; i < num_ent; i++) {
        tnecs_entity ent = entities[i];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(map->world, ent, Unit);
        SDL_assert(unit != NULL);

        if (Unit_Class(unit) == UNIT_CLASS_STANDARD_BEARER)
            Map_Bonus_Standard_Apply_Unit(map, ent, entities);

    }
}

/* -- Entities -- */
tnecs_entity *Map_Get_onField(struct Map *map, i32 army) {
    tnecs_entity *entities = NULL;
    if (army_alignment[army] == ALIGNMENT_FRIENDLY)
        entities = map->units.onfield.friendlies;
    else if (army_alignment[army] == ALIGNMENT_ENEMY)
        entities = map->units.onfield.enemies;
    return (entities);
}
