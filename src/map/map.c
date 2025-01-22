
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
#include "unit/equipment.h"
#include "unit/unit.h"
#include "unit/loadout.h"
#include "unit/anim.h"
#include "unit/status.h"
#include "unit/equipment.h"
#include "unit/boss.h"
#include "unit/range.h"
#include "unit/stats.h"
#include "unit/mount.h"
#include "unit/bonus.h"
#include "stb_sprintf.h"

const NewMap NewMap_default = {
    .tilesize   = {SOTA_TILESIZE, SOTA_TILESIZE},
    .stack_mode = MAP_SETTING_STACK_DANGERMAP,
};

const Map Map_default = {
    .json_element           = JSON_MAP,
    .show_icons             = true,
    .cost_multiplier        =  1,
    .frames                 = 10,
    .frames_shadow          = 10,
    .speed                  = 50,

    .perimiter_danger_color = SOTA_RED,
    .perimiter_aura_color   = SOTA_PURPLE,

    .shadow_frame_pause     = 3,
    .tilemap_frame_pause    = 6,
    .shadow_frame_factor    = 0.10f,
    .shadow_frame_counter   = UINT8_MAX - 1, /* shadow renders on first frame */
    .tilemap_frame_factor   = 0.15f,
    .tilemap_frame_counter  = UINT8_MAX - 1, /* map renders on first frame */

    .camera.offset.x        = DEFAULT_CAMERA_XOFFSET,
    .camera.offset.y        = DEFAULT_CAMERA_YOFFSET,
    .camera.zoom            = DEFAULT_CAMERA_ZOOM,

    .ipalette_base          = PALETTE_SOTA,
    .ipalette_red           = PALETTE_SOTA_FILTER_RED,
    .ipalette_blue          = PALETTE_SOTA_FILTER_BLUE,
    .ipalette_green         = PALETTE_SOTA_FILTER_GREEN,
    .ipalette_purple        = PALETTE_SOTA_FILTER_PURPLE,
    .ipalette_darkred       = PALETTE_SOTA_FILTER_DARKREDwSHADOW,
    .ipalette_shadow        = PALETTE_SOTA_SHADOW,
    .ipalette_enemy         = PALETTE_SOTA,

    .turn                   =  1, /* Automatic loss if turn 255. */
    /* --- MUSIC --- */
    .music_i_friendly       = SOTA_MUSIC_DEBUG,
    .music_i_enemy          = -1,
};

void Map_Reinforcements_Free(struct Map *map) {
    if (map->reinforcements == NULL)
        return;

    for (int r = 0; r < DARR_NUM(map->reinforcements); r++) {
        Reinforcement_Free(&map->reinforcements[r]);
    }

    DARR_FREE(map->reinforcements);
    map->reinforcements = NULL;
}

void Map_Tilemap_Shader_Free(struct Map *map) {
    Tilemap_Shader_Free(map->tilemap_shader);
    if (map->tilemap_shader != NULL) {
        SDL_free(map->tilemap_shader);
        map->tilemap_shader = NULL;
    }
}

void Map_Tilemap_Shader_Init(Map *map) {
    map->tilemap_shader         = SDL_malloc(sizeof(struct Tilemap_Shader));
    *map->tilemap_shader        = Tilemap_Shader_default;
    map->tilemap_shader->map    = map;
    map->tilemap_shader->to     = palette_table_SOTA_shadow;
}

/* --- GLOBAL FUNCTIONS --- */
/* --- Constructor/Destructors --- */
Map *Map_New(NewMap new_map) {
    Map *map        = SDL_malloc(sizeof(Map));
    *map            = Map_default;
    map->world      = new_map.world;
    map->stack_mode = new_map.stack_mode;

    Map_Size_Set(       map,    new_map.col_len,        new_map.row_len);
    Map_Tilesize_Set(   map,    new_map.tilesize[0],    new_map.tilesize[1]);
    Map_Renderer_Set(map, new_map.renderer);
    Map_Members_Alloc(map);

    return (map);
}

void Map_Unitmap_Free(struct Map *map) {
    /* -- SDL_free non-PC units on unitmap -- */
    SDL_assert(map->unitmap);
    for (size_t i = 0; i < map->col_len * map->row_len ; i++) {
        tnecs_entity uent = map->unitmap[i];
        if (uent == TNECS_NULL) {
            map->unitmap[i] = TNECS_NULL;
            continue;
        }
        if (map->world->entities[uent] == TNECS_NULL) {
            map->unitmap[i] = TNECS_NULL;
            continue;
        }

        Unit    *unit   = IES_GET_COMPONENT(map->world, uent, Unit);
        Sprite  *sprite = IES_GET_COMPONENT(map->world, uent, Sprite);

        if (sprite != NULL)
            Sprite_Free(sprite);

        // Freeing unit if not in party
        if ((unit != NULL) && (unit->_id > UNIT_ID_PC_END)) {
            Unit_Free(unit);
            tnecs_entity_destroy(map->world, uent);
        } else if (unit == NULL) {
            tnecs_entity_destroy(map->world, uent);
        }
        map->unitmap[i] = TNECS_NULL;
    }

    s8_free(&map->json_filename);

    /* -- SDL_free unitmap -- */
    SDL_free(map->unitmap);
    map->unitmap = NULL;
}

void Map_Free(struct Map *map) {
    SDL_assert(map);
    /* - Music -*/
    if (map->music_enemy != NULL) {
        Mix_FreeMusic(map->music_enemy);
        map->music_enemy = NULL;
    }
    if (map->party_filename.data != NULL) {
        s8_free(&map->party_filename);
    }
    if (map->music_friendly != NULL) {
        Mix_FreeMusic(map->music_friendly);
        map->music_friendly = NULL;
    }

    if (map->tilesindex != NULL) {
        DARR_FREE(map->tilesindex);
    }

    /* - Conditions - */
    if (map->death_enemy != NULL) {
        DARR_FREE(map->death_enemy);
        map->death_enemy = NULL;
    }
    if (map->death_friendly != NULL) {
        DARR_FREE(map->death_friendly);
        map->death_friendly = NULL;
    }
    if (map->turn_end != NULL) {
        DARR_FREE(map->turn_end);
        map->turn_end = NULL;
    }
    if (map->waits_friendly != NULL) {
        DARR_FREE(map->waits_friendly);
        map->waits_friendly = NULL;
    }

    /* - Edges - */
    if (map->edges_danger != NULL) {
        SDL_free(map->edges_danger);
        map->edges_danger = NULL;
    }
    if (map->breakables_ent != NULL) {
        DARR_FREE(map->breakables_ent);
        map->breakables_ent = NULL;
    }
    if (map->chests_ent != NULL) {
        DARR_FREE(map->chests_ent);
        map->chests_ent = NULL;
    }
    if (map->doors_ent != NULL) {
        DARR_FREE(map->doors_ent);
        map->doors_ent = NULL;
    }

    Map_Tilesets_Free(map);
    Map_Tilemap_Texture_Free(map);
    Map_Tiles_Free(map);
    if (map->friendlies_onfield != NULL) {
        DARR_FREE(map->friendlies_onfield);
        map->friendlies_onfield = NULL;
    }
    if (map->reinf_equipments != NULL) {
        for (size_t i = 0; i < DARR_NUM(map->reinf_equipments); i++) {
            DARR_FREE(map->reinf_equipments[i]);
        }
        DARR_FREE(map->reinf_equipments);
        map->reinf_equipments = NULL;
    }
    if (map->items_num != NULL) {
        DARR_FREE(map->items_num);
        map->items_num = NULL;
    }
    if (map->tilemap != NULL) {
        SDL_free(map->tilemap);
        map->tilemap = NULL;
    }
    if (map->tiles != NULL) {
        DARR_FREE(map->tiles);
        map->tiles = NULL;
    }
    if (map->tiles_id != NULL) {
        DARR_FREE(map->tiles_id);
        map->tiles_id = NULL;
    }
    if (map->items_num != NULL) {
        DARR_FREE(map->items_num);
        map->items_num = NULL;
    }
    if (map->army_onfield != NULL) {
        DARR_FREE(map->army_onfield);
        map->army_onfield = NULL;
    }
    if (map->start_pos != NULL) {
        DARR_FREE(map->start_pos);
        map->start_pos = NULL;
    }
    if (map->enemies_onfield != NULL) {
        DARR_FREE(map->enemies_onfield);
        map->enemies_onfield = NULL;
    }
    if (map->units_onfield != NULL) {
        DARR_FREE(map->units_onfield);
        map->units_onfield = NULL;
    }
    Map_Tilemap_Surface_Free(map);
    Map_Reinforcements_Free(map);
    Arrow_Free(map->arrow);
    if (map->tilemap_shader != NULL) {
        Tilemap_Shader_Free(map->tilemap_shader);
        SDL_free(map->tilemap_shader);
        map->tilemap_shader = NULL;
    }
    if (map->texture != NULL) {
        SDL_DestroyTexture(map->texture);
        map->texture = NULL;
    }
    if (map->unitmap != NULL) {
        SDL_free(map->unitmap);
        map->unitmap = NULL;
    }
    if (map->costmap != NULL) {
        SDL_free(map->costmap);
        map->costmap = NULL;
    }
    if (map->movemap != NULL) {
        SDL_free(map->movemap);
        map->movemap = NULL;
    }
    if (map->start_posmap != NULL) {
        SDL_free(map->start_posmap);
        map->start_posmap = NULL;
    }

    if (map->attacktomap != NULL) {
        SDL_free(map->attacktomap);
        if (map->attacktomap == map->healtomap) {
            map->healtomap = NULL;
        }
        map->attacktomap = NULL;
    }
    if (map->attackfrommap != NULL) {
        SDL_free(map->attackfrommap);
        map->attackfrommap = NULL;
    }
    if (map->healtomap != NULL) {
        SDL_free(map->healtomap);
        map->healtomap = NULL;
    }
    if (map->healfrommap != NULL) {
        SDL_free(map->healfrommap);
        map->healfrommap = NULL;
    }
    if (map->dangermap != NULL) {
        SDL_free(map->dangermap);
        map->dangermap = NULL;
    }
    if (map->temp != NULL) {
        SDL_free(map->temp);
        map->temp = NULL;
    }
    if (map->global_rangemap != NULL) {
        SDL_free(map->global_rangemap);
        map->global_rangemap = NULL;
    }
    if (map->palettemap != NULL) {
        SDL_free(map->palettemap);
        map->palettemap = NULL;
    }
    if (map->temp_palette != NULL) {
        SDL_free(map->temp_palette);
        map->temp = NULL;
    }
    if (map->stacked_dangermap != NULL) {
        SDL_free(map->stacked_dangermap);
        map->stacked_dangermap = NULL;
    }
    if (map->stacked_global_dangermap != NULL) {
        SDL_free(map->stacked_global_dangermap);
        map->stacked_global_dangermap = NULL;
    }
    if (map->global_dangermap != NULL) {
        SDL_free(map->global_dangermap);
        map->global_dangermap = NULL;
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

    SDL_assert(map->doors_ent == NULL);
    map->doors_ent = DARR_INIT(map->doors_ent, tnecs_entity, 4);

    SDL_assert(map->breakables_ent == NULL);
    map->breakables_ent = DARR_INIT(map->breakables_ent, tnecs_entity, 4);

    SDL_assert(map->chests_ent == NULL);
    map->chests_ent = DARR_INIT(map->chests_ent, tnecs_entity, 4);

    Map_Texture_Alloc(map);
    Map_Tilemap_Shader_Init(map);
    SDL_assert(map->tilemap == NULL);
    map->tilemap = SDL_calloc(len, sizeof(*map->tilemap));

    SDL_assert(map->start_pos == NULL);
    map->start_pos = DARR_INIT(map->start_pos, struct Point, 16);

    SDL_assert(map->death_enemy == NULL);
    map->death_enemy = DARR_INIT(map->death_enemy, struct Map_condition, 2);

    SDL_assert(map->death_friendly == NULL);
    map->death_friendly = DARR_INIT(map->death_friendly, struct Map_condition, 2);

    SDL_assert(map->reinforcements == NULL);
    map->reinforcements = DARR_INIT(map->reinforcements, struct Reinforcement, 16);

    SDL_assert(map->items_num == NULL);
    map->items_num = DARR_INIT(map->items_num, u8, SOTA_EQUIPMENT_SIZE);

    SDL_assert(map->tilesindex == NULL);
    map->tilesindex = DARR_INIT(map->tilesindex, i32, DEFAULT_TILESPRITE_BUFFER);

    SDL_assert(map->healtolist == NULL);
    map->healtolist = DARR_INIT(map->healtolist, i32, 32);

    SDL_assert(map->attacktolist == NULL);
    map->attacktolist = DARR_INIT(map->attacktolist, i32, 32);

    SDL_assert(map->attackfromlist == NULL);
    map->attackfromlist = DARR_INIT(map->attackfromlist, i32, 32);

    SDL_assert(map->enemies_onfield == NULL);
    map->enemies_onfield = DARR_INIT(map->enemies_onfield, tnecs_entity, 20);

    SDL_assert(map->friendlies_onfield == NULL);
    map->friendlies_onfield = DARR_INIT(map->friendlies_onfield, tnecs_entity, 20);

    SDL_assert(map->units_onfield == NULL);
    map->units_onfield = DARR_INIT(map->units_onfield, tnecs_entity, 20);

    SDL_assert(map->reinf_equipments == NULL);
    map->reinf_equipments = DARR_INIT(map->reinf_equipments, Inventory_item *, 30);

    SDL_assert(map->army_onfield == NULL);
    map->army_onfield = DARR_INIT(map->army_onfield, i32, 5);

    SDL_assert(map->temp == NULL);
    map->temp = SDL_calloc(len,  sizeof(*map->temp));

    SDL_assert(map->unitmap == NULL);
    map->unitmap = SDL_calloc(len,  sizeof(*map->unitmap));

    SDL_assert(map->costmap == NULL);
    map->costmap = SDL_calloc(len,  sizeof(*map->costmap));

    SDL_assert(map->movemap == NULL);
    map->movemap = SDL_calloc(len,  sizeof(*map->movemap));

    SDL_assert(map->start_posmap == NULL);
    map->start_posmap = SDL_calloc(len,  sizeof(*map->start_posmap));

    SDL_assert(map->dangermap == NULL);
    map->dangermap = SDL_calloc(len,  sizeof(*map->dangermap));

    SDL_assert(map->palettemap == NULL);
    map->palettemap = SDL_malloc(len * sizeof(*map->palettemap));

    SDL_assert(map->attacktomap == NULL);
    map->attacktomap = SDL_calloc(len,  sizeof(*map->attacktomap));

    SDL_assert(map->healtomap == NULL);
    map->healtomap = SDL_calloc(len,  sizeof(*map->healtomap));

    SDL_assert(map->healfrommap == NULL);
    map->healfrommap = SDL_calloc(len,  sizeof(*map->healfrommap));

    SDL_assert(map->temp_palette == NULL);
    map->temp_palette = SDL_malloc(len * sizeof(*map->temp_palette));

    SDL_assert(map->attackfrommap == NULL);
    map->attackfrommap = SDL_calloc(len,  sizeof(*map->attackfrommap));

    SDL_assert(map->global_rangemap == NULL);
    map->global_rangemap = SDL_calloc(len,  sizeof(*map->global_rangemap));

    SDL_assert(map->global_dangermap == NULL);
    map->global_dangermap = SDL_calloc(len,  sizeof(*map->global_dangermap));

    SDL_assert(map->edges_danger == NULL);
    map->edges_danger = SDL_calloc(len, sizeof(*map->edges_danger));

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

    // SDL_Log("map->stack_mode %d %d", map->stack_mode, MAP_SETTING_STACK_DANGERMAP);
    if (map->stack_mode == MAP_SETTING_STACK_DANGERMAP) {
        // SDL_Log("Allocating stacked_dangermap");
        SDL_assert(map->stacked_dangermap == NULL);
        map->stacked_dangermap          = SDL_calloc(len, sizeof(*map->stacked_dangermap));
        SDL_assert(map->stacked_global_dangermap == NULL);
        map->stacked_global_dangermap   = SDL_calloc(len, sizeof(*map->stacked_global_dangermap));
    }

    Map_Palettemap_Reset(map);
}

void Map_Members_Free(struct Map *map) {
    if (map->attacktolist != NULL) {
        DARR_FREE(map->attacktolist);
        map->attacktolist = NULL;
    }
    if (map->attackfromlist != NULL) {
        DARR_FREE(map->attackfromlist);
        map->attackfromlist = NULL;
    }

    if (map->healtolist != NULL) {
        DARR_FREE(map->healtolist);
        map->healtolist = NULL;
    }
}

void Map_Texture_Alloc(struct Map *map) {
    if (map->texture != NULL)
        SDL_DestroyTexture(map->texture);

    map->texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_TARGET, map->tilesize[0] * map->col_len,
                                     map->tilesize[1] * map->row_len);
    SDL_SetTextureBlendMode(map->texture, SDL_BLENDMODE_BLEND);

}

void Map_Tilemap_Texture_Free(struct Map *map) {
    if (map->tilemap_texture != NULL) {
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = NULL;
    }
}

void Map_Tilemap_Texture_Init(struct Map *map) {
    SDL_assert(map->renderer);
    Map_Tilemap_Texture_Free(map);
    int x_size = map->tilesize[0] * map->col_len;
    int y_size = map->tilesize[1] * map->row_len;
    map->tilemap_texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_RGB888,
                                             SDL_TEXTUREACCESS_TARGET, x_size, y_size);
    SDL_assert(map->tilemap_texture);
}

void Map_Tilemap_Surface_Free(struct Map *map) {
    if (map->tilemap_surface != NULL) {
        SDL_FreeSurface(map->tilemap_surface);
        map->tilemap_surface = NULL;
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
    map->tilemap_surface = Filesystem_indexedSurface_Init(x_size, y_size);
    SDL_assert(map->tilemap_surface->w == map->tilesize[0] * map->col_len);
    SDL_assert(map->tilemap_surface->h == map->tilesize[1] * map->row_len);
}

/* --- I/O --- */
void Map_writeJSON( void *input, cJSON *jmap) {
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
    SDL_assert(map->start_pos != NULL);
    for (size_t i = 0; i < DARR_NUM(map->start_pos); i++) {
        jstartingpos = cJSON_CreateObject();
        cJSON_AddItemToObject(jstart_pos_arr, "Position", jstartingpos);
        jstartingposx = cJSON_CreateNumber(map->start_pos[i].x);
        jstartingposy = cJSON_CreateNumber(map->start_pos[i].y);
        cJSON_AddItemToObject(jstartingpos, "col", jstartingposx);
        cJSON_AddItemToObject(jstartingpos, "row", jstartingposy);
    }

    /* -- Writing Tiles -- */
    cJSON *jtiles = cJSON_CreateObject();
    cJSON_AddItemToObject(jmap, "tiles", jtiles);
    cJSON *jtile;
    cJSON *jtilename;
    cJSON *jtileid;
    uint64_t temp_tile_id;
    for (size_t i = 0; i < DARR_NUM(map->tiles); i++) {
        jtile = cJSON_CreateObject();
        if (map->tilesindex[i] >= TILE_END) {
            temp_tile_id = map->tilesindex[i];
            jtilename   = cJSON_CreateString(global_tilenames[temp_tile_id].data);
            jtileid     = cJSON_CreateNumber(temp_tile_id);
        } else {
            jtilename   = cJSON_CreateString(global_tilenames[map->tilesindex[i]].data);
            jtileid     = cJSON_CreateNumber(map->tilesindex[i]);
        }
        cJSON_AddItemToObject(jtile, "id", jtileid);
        cJSON_AddItemToObject(jtile, "name", jtilename);
        cJSON_AddItemToObject(jtiles, "tile", jtile);
    }
    /* -- Writing Reinforcements -- */
    struct Inventory_item *temp_equip, temp_item;
    SDL_assert((map->reinf_equipments != NULL) & (map->reinforcements != NULL));
    cJSON *jreinforcement;
    cJSON *jreinforcementeq;
    cJSON *jreinforcements = cJSON_CreateObject();
    cJSON_AddItemToObject(jmap, "Reinforcements", jreinforcements);
    for (i32 r = 0; r < DARR_NUM(map->reinforcements); r++) {
        jreinforcement   = cJSON_CreateObject();
        jreinforcementeq = cJSON_CreateObject();
        Reinforcement_writeJSON(jreinforcement, &(map->reinforcements)[r]);
        temp_equip = map->reinf_equipments[r];
        for (i32 i = 0; i < DARR_NUM(temp_equip); i++) {
            temp_item = temp_equip[i];
            if (temp_item.id > ITEM_NULL)
                Inventory_item_writeJSON(&temp_item, jreinforcementeq);
        }
        cJSON_AddItemToObject(jreinforcement, "Equipment", jreinforcementeq);
        cJSON_AddItemToObject(jreinforcements, "Reinforcement", jreinforcement);
    }
    cJSON *jtilemap = cJSON_CreateObject();
    Array2D_writeJSON(jtilemap, map->tilemap, map->row_len, map->col_len);
    cJSON_AddItemToObject(jmap, "Tilemap", jtilemap);
}

void Map_RowCol_readJSON(s8 filename, i32 rowcol[TWO_D]) {
    // SDL_Log("%s", filename);
    struct cJSON *jfile = jsonio_parseJSON(s8_var(filename.data));
    SDL_assert(jfile != NULL);

    s8 elem_name = jsonElementnames[JSON_MAP];
    struct cJSON *jmap = cJSON_GetObjectItem(jfile, elem_name.data);
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

void Map_readJSON(void *input,  cJSON *jmap) {
    struct Map *map = (struct Map *) input;
    SDL_assert(map->death_enemy         != NULL);
    SDL_assert(map->death_friendly      != NULL);
    SDL_assert(map->reinforcements      != NULL);
    SDL_assert(map->reinf_equipments    != NULL);
    SDL_assert(map->items_num           != NULL);
    SDL_assert(map->chests_ent          != NULL);
    SDL_assert(map->breakables_ent      != NULL);
    SDL_assert(map->doors_ent           != NULL);

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
        SDL_Log("map->col_len should be set to '%d'", cJSON_GetNumberValue(jcol_len));
        exit(ERROR_Generic);
    }

    if (map->row_len != cJSON_GetNumberValue(jrow_len)) {
        SDL_Log("map->row_len should be set to '%d'", cJSON_GetNumberValue(jrow_len));
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
        DARR_PUT(map->tilesindex, cJSON_GetNumberValue(jid));
    }
    // SDL_assert(map->tileset_surfaces == NULL);
    Map_Tilesets_Free(map);
    Map_Tiles_Free(map);

    Map_Tiles_Load(map);
    Map_Tilesets_Load(map);
    SDL_assert(map->tileset_surfaces[0]);

    /* -- Read Reinforcements --  */
    // SDL_Log("Read Reinforcements");

    cJSON *jreinforcements = cJSON_GetObjectItem(jmap, "Reinforcements");
    SDL_assert(jreinforcements != NULL);
    cJSON *jequipment, *jitem;
    struct Inventory_item temp_item, *temp_equip;

    for (int i = 0; i < cJSON_GetArraySize(jreinforcements); i++) {
        struct Reinforcement temp_rein = Reinforcement_default;
        cJSON *jreinforcement = cJSON_GetArrayItem(jreinforcements, i);
        Reinforcement_readJSON(jreinforcement, &temp_rein);
        DARR_PUT(map->reinforcements, temp_rein);
        jequipment = cJSON_GetObjectItem(jreinforcement, "Equipment");
        temp_equip = DARR_INIT(temp_equip, struct Inventory_item, SOTA_EQUIPMENT_SIZE);
        /* FIRST ITEM IS NULL */
        DARR_PUT(temp_equip, temp_item);

        if (!cJSON_IsArray(jequipment)) {
            SDL_Log("Missing Equipment array in Reinforcement %d", i);
            continue;
        }

        DARR_PUT(map->items_num, cJSON_GetArraySize(jequipment));
        cJSON_ArrayForEach(jitem, jequipment) {
            temp_item = Inventory_item_default;
            Inventory_item_readJSON(&temp_item, jitem);
            // NOTE:    Do not ignore empty items.
            //          To be able to put weapons in right hand.
            DARR_PUT(temp_equip, temp_item);
        }
        DARR_PUT(map->reinf_equipments, temp_equip);
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

        DARR_NUM(map->chests_ent) = 0;

        for (size_t i = 0; i < DARR_NUM(map->chests_ent); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Chest, Position);
            struct Chest    *chest  = IES_GET_COMPONENT(map->world, temp_ent, Chest);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos      != NULL);
            SDL_assert(chest    != NULL);
            cJSON *jchest   = cJSON_GetArrayItem(jchests, i);
            cJSON *jpos     = cJSON_GetObjectItem(jchest, "position");

            if (jchest == NULL) {
                SDL_Log("Warning: could not read chest %d", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read chest %d's position", i);
                continue;
            }

            Point_readJSON((struct Point *)&pos->tilemap_pos, jpos);
            Chest_readJSON(chest, jchest);

            DARR_PUT(map->chests_ent, temp_ent);
        }
    } while (0);

    /* -- Loading doors -- */
    do { /*Loop never executes, just used for break*/
        if (jdoors == NULL)
            break;
        SDL_assert(cJSON_IsArray(jdoors));

        DARR_NUM(map->doors_ent) = 0;

        for (size_t i = 0; i < cJSON_GetArraySize(jdoors); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Door, Position);
            struct Door     *door   = IES_GET_COMPONENT(map->world, temp_ent, Door);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos  != NULL);
            SDL_assert(door != NULL);

            cJSON *jdoor    = cJSON_GetArrayItem(jdoors, i);
            cJSON *jpos     = cJSON_GetObjectItem(jdoor, "position");
            if (jdoor == NULL) {
                SDL_Log("Warning: could not read door %d", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read door %d's position", i);
                continue;
            }
            Point_readJSON((struct Point *)&pos->tilemap_pos, jpos);
            Door_readJSON(door, jdoor);

            DARR_PUT(map->doors_ent, temp_ent);
        }
    } while (0);

    /* -- Loading breakables -- */
    do { /*Loop never executes, just used for break*/
        // Note: Breakable component should be added to already existing DOORS/CHESTS
        if (jbreakables == NULL)
            break;

        SDL_assert(cJSON_IsArray(jbreakables));

        DARR_NUM(map->breakables_ent) = 0;

        for (size_t i = 0; i < cJSON_GetArraySize(jbreakables); i++) {
            tnecs_entity temp_ent   = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Breakable, Position);
            struct Position *pos    = IES_GET_COMPONENT(map->world, temp_ent, Position);
            SDL_assert(pos != NULL);
            cJSON *jbreakable       = cJSON_GetArrayItem(jbreakables, i);
            cJSON *jpos             = cJSON_GetObjectItem(jbreakable, "position");
            if (jbreakable == NULL) {
                SDL_Log("Warning: could not read breakable %d", i);
                continue;
            }
            if (jpos == NULL) {
                SDL_Log("Warning: could not read breakable %d's position", i);
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
                TNECS_ADD_COMPONENT(map->world, temp_ent, Breakable);
            }

            struct Breakable *breaka = IES_GET_COMPONENT(map->world, temp_ent, Breakable);
            SDL_assert(breaka != NULL);
            Breakable_readJSON(breaka, jbreakables);

            DARR_PUT(map->breakables_ent, temp_ent);
        }
    } while (0);

    cJSON *jframe;
    cJSON *jarray;
    cJSON *jtilemap = cJSON_GetObjectItem(jmap, "tilemap");
    cJSON *jframes  = cJSON_GetObjectItem(jtilemap, "frames");
    map->frames     = cJSON_GetNumberValue(jframes);

    cJSON_ArrayForEach(jframe, jframes) {
        // TODO: tilemaps[i]
        jarray = cJSON_GetObjectItem(jframe, "array");
        Array2D_readJSON(jarray, map->tilemap, map->row_len, map->col_len);
        SDL_assert(map->tilemap);
    }

    /* - Map objects override tiles - */
    Map_Tilemap_MapObjects(map);

    /* -- Misc. Computations -- */
    Map_swappedTextures_All(map);
    map->tilemap_shader->frames = map->frames;
    Tilemap_Shader_Load_Tilemap_JSON(map->tilemap_shader, jmap);
    // SDL_assert(map->tilemap_shader->shadow_tilemaps);

    /* --- Parsing shadow tileset --- */
    if (map->tilemap_shader->shadow_tilemaps) {
        s8 path = s8_var(PATH_JOIN("assets", "tiles", "Tileset_Shadow.json"));
        struct cJSON *jshadow_tileset_file  = jsonio_parseJSON(path);
        struct cJSON *jshadow_tileset       = cJSON_GetObjectItem(jshadow_tileset_file, "shadow_tileset");
        SDL_assert(jshadow_tileset);
        Tilemap_Shader_Load_Tileset_JSON(map->tilemap_shader, jshadow_tileset);
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
        DARR_PUT(map->death_enemy, map_cond);
    }

    cJSON_ArrayForEach(jmap_condition, jdeath_friendly) {
        struct Map_condition map_cond = Map_condition_default;
        Map_Condition_readJSON(&map_cond, jmap_condition);
        DARR_PUT(map->death_friendly, map_cond);
    }
}

/* --- Map events / Triggers --- */
/* Ouputs index of army in army_onfield*/
i32 Map_Army_Next(struct Map *map) {
    SDL_assert(map->turn > 0);
    SDL_assert(map->turn < SOTA_MAX_TURNS);
    /* Get next army in line for control */
    SDL_assert(map->army_onfield != NULL);

    /* Get number of armies on field */
    size_t army_num = DARR_NUM(map->army_onfield);
    SDL_assert(army_num <= ARMY_NUM);

    /* Check for wrap around back to player */
    map->army_i = (map->army_i + (map->turn > 1)) % army_num;
    SDL_assert(map->army_i >= 0);
    SDL_assert(map->army_i <= army_num);

    return (map->army_i);
}

void Map_Turn_Increment(struct Map *map) {
    map->turn++;
}

/* --- Music --- */
void Map_Music_Load(struct Map *map) {
    map->music_enemy       = Music_Load(map->music_i_enemy);
    map->music_friendly    = Music_Load(map->music_i_friendly);
}

/* --- Boss --- */
b32 Map_Boss_Alive(struct Map *map, i16 army) {
    // TODO:
    return (true);
}

/* --- Tile --- */
struct Tile *Map_Tile_Get(struct Map *map, i32 x, i32 y) {
    SDL_assert(map          != NULL);
    SDL_assert(map->tiles   != NULL);
    i32 index = sota_2D_index(x, y, map->col_len);
    i32 tile_ind = map->tilemap[index] / TILE_DIVISOR;
    size_t tile_order = Map_Tile_Order(map, tile_ind);
    return (map->tiles + tile_order);
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
    SDL_assert(unit->class  == UNIT_CLASS_STANDARD_BEARER);
    b32 active  = false;
    b32 instant = true;
    u16 skill   = SKILL_START;

    /* Check if unit has a standard equipped */
    struct Weapon *wpnL = Unit_Equipped_Weapon(unit, UNIT_HAND_LEFT);
    if ((wpnL != NULL) && (wpnL->item->type == ITEM_TYPE_STANDARD))
        Map_Aura_Apply(map, wpnL->item->aura, entities, ent, wpnL->item->id, skill, active, instant);

    struct Weapon *wpnR = Unit_Equipped_Weapon(unit, UNIT_HAND_RIGHT);
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

        if (unit->class == UNIT_CLASS_STANDARD_BEARER)
            Map_Bonus_Standard_Apply_Unit(map, ent, entities);

    }
}

/* -- Entities -- */
tnecs_entity *Map_Get_onField(struct Map *map, i32 army) {
    tnecs_entity *entities = NULL;
    if (army_alignment[army] == ALIGNMENT_FRIENDLY)
        entities = map->friendlies_onfield;
    else if (army_alignment[army] == ALIGNMENT_ENEMY)
        entities = map->enemies_onfield;
    return (entities);
}
