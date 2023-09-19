#include "map/map.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
//#endif /* STB_SPRINTF_IMPLEMENTATION */

struct Map_condition Map_condition_default = {
    .unit       = -1,
    .army       = -1,
    .row_min    = -1,
    .row_max    = -1,
    .col_min    = -1,
    .col_max    = -1
};

struct Map Map_default = {
    .json_element   = JSON_MAP,

    .world          = NULL,
    .renderer       = NULL,
    .show_icons     = true,
    .frames         = 10,
    .frames_shadow  = 10,
    .speed          = 50,
    .tilesize       = {0, 0},

    .items_num              = NULL,
    .shadow_frame_pause     = 3,
    .tilemap_frame_pause    = 6,
    .shadow_frame_factor    = 0.10f,
    .shadow_frame_counter   = UINT8_MAX - 1, /* shadow renders on first frame */
    .tilemap_frame_factor   = 0.15f,
    .tilemap_frame_counter  = UINT8_MAX - 1, /* map renders on first frame */

    .tilemap_shader    = NULL,
    .tileset_surfaces  = NULL,
    .tileset_textures  = NULL,
    .texture           = NULL,
    .fcostmap          = NULL,
    .fmovemap          = NULL,
    .costmap           = NULL,
    .costmap_ent       = TNECS_NULL,
    .movemap           = NULL,
    .attackfrommap     = NULL,
    .attacktomap       = NULL,
    .attacktolist      = NULL,
    .healtomap         = NULL,
    .dangermap         = NULL,
    .globalRangemap    = NULL,
    .tilesindex        = NULL,
    .tilemap_surface   = NULL,

    .show_move         = false,
    .show_attack       = false,
    .show_heal         = false,
    .show_danger       = false,
    .show_globalRange  = false,
    .show_grid         = false,

    .camera_moved      = false,
    .visible_changed   = false,
    .shading_changed   = false,

    .row_len = 0,
    .col_len = 0,

    .tiles_id          = NULL,
    .tiles             = NULL,
    .tilemap           = NULL,
    .palettes_num      = 5,
    .palettemap        = NULL,

    .palettes = {
        PALETTE_NES,
        PALETTE_NES_FILTER_RED,
        PALETTE_NES_FILTER_GREEN,
        PALETTE_NES_FILTER_BLUE,
        PALETTE_NES_DARKREDwSHADOW
    },

    .boundsmin         = {  0,   0},
    .boundsmax         = {255, 255},
    .num_friendlies    = 0,
    .num_enemies       = 0,
    .turn              = 0, // Automatic loss if turn 255.
    .chapter           = 0,

    .reinforcements    = NULL,
    .reinf_equipments  = NULL,

    .breakables_ent    = NULL,
    .chests_ent        = NULL,
    .doors_ent         = NULL,
    .breakable_num     = 0,
    .chest_num         = 0,
    .door_num          = 0,

    .start_pos               = NULL,
    .armies_onfield          = NULL,
    .num_armies_onfield      = 0,
    .units_onfield           = NULL,
    .num_units_onfield       = 0,
    .friendlies_onfield      = NULL,
    .num_friendlies_onfield  = 0,
    .enemies_onfield         = NULL,
    .num_enemies_onfield     = 0,
    .unitmap                 = NULL,
    .arrow                   = NULL,
    // .boss;
    // .bossdied;
    // .seized; // maybe unecessary if turn system.
};

/* --- Constructor/Destructors --- */

struct Map *Map_Init(struct Map *map, i32 width, i32 height) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map != NULL)
        Map_Free(map);
    map = (struct Map *)SDL_malloc(sizeof(struct Map));
    *map = Map_default;
    map->tiles      = DARR_INIT(map->tiles, struct Tile, 64);
    map->tiles_id   = DARR_INIT(map->tiles_id, i32, 64);
    map->items_num  = DARR_INIT(map->items_num, uf8, 8);
    map->start_pos  = DARR_INIT(map->start_pos, struct Point, 20);
    map->enemies_onfield    = DARR_INIT(map->enemies_onfield, tnecs_entity_t, 20);
    map->friendlies_onfield = DARR_INIT(map->friendlies_onfield, tnecs_entity_t, 20);
    map->units_onfield      = DARR_INIT(map->units_onfield, tnecs_entity_t, 20);
    map->reinf_equipments   = DARR_INIT(map->reinf_equipments, struct Inventory_item *, 30);
    map->armies_onfield = calloc(10, sizeof(*map->armies_onfield));
    Map_Tilesize_Set(map, width, height);
    map->arrow = malloc(sizeof(*map->arrow));
    *map->arrow = Arrow_default;
    Arrow_Init(map->arrow, map->tilesize);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (map); // return cause pointer address can change.
}

void Map_Units_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Free units on unitmap -- */
    SDL_assert(map->unitmap);
    for (size_t i = 0; i < map->col_len * map->row_len ; i++) {
        tnecs_entity_t uent = map->unitmap[i];
        if (uent == TNECS_NULL)
            continue;
        struct Unit *unit = TNECS_GET_COMPONENT(map->world, uent, Unit);
        if (unit != NULL)
            Unit_Free(unit);
        struct Sprite *sprite = TNECS_GET_COMPONENT(map->world, uent, Sprite);
        if (sprite != NULL)
            Sprite_Free(sprite);
        tnecs_entity_destroy(map->world, uent);

    }

    /* -- Free reinforcements -- */
    for (int i = 0; i < DARR_NUM(map->reinforcements); i++) {
        // reinforcements.id is not an entity???
        tnecs_entity_t uent = map->reinforcements[i].id;
        if ((uent == TNECS_NULL)
            || (!TNECS_ENTITY_EXISTS(map->world, uent)))
            continue;

        struct Unit *unit = TNECS_GET_COMPONENT(map->world, uent, Unit);
        if (unit != NULL)
            Unit_Free(unit);
        struct Sprite *sprite = TNECS_GET_COMPONENT(map->world, uent, Sprite);
        if (sprite != NULL)
            Sprite_Free(sprite);
        tnecs_entity_destroy(map->world, uent);

    }

    /* -- Free unitmap -- */
    free(map->unitmap);
    map->unitmap = NULL;

    /* -- Free loaded reinforcements -- */
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Units_Hide(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    for (size_t i = 0; i < (map->col_len * map->row_len); i++) {
        tnecs_entity_t uent = map->unitmap[i];
        if (uent == TNECS_NULL)
            continue;
        struct Sprite *sprite = TNECS_GET_COMPONENT(map->world, uent, Sprite);
        if (sprite != NULL)
            sprite->visible = false;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map);
    if (map->breakables_ent != NULL) {
        free(map->breakables_ent);
        map->breakables_ent = NULL;
    }
    if (map->chests_ent != NULL) {
        free(map->chests_ent);
        map->chests_ent = NULL;
    }
    if (map->doors_ent != NULL) {
        free(map->doors_ent);
        map->doors_ent = NULL;
    }

    Map_Units_Free(map);
    Map_Tilesets_Free(map);
    Map_Tilesprites_Free(map);
    Map_Tilemap_Texture_Free(map);
    Map_Tiles_Free(map);
    if (map->friendlies_onfield != NULL) {
        DARR_FREE(map->friendlies_onfield);
        map->friendlies_onfield = NULL;
    }
    if (map->reinforcements != NULL) {
        DARR_FREE(map->reinforcements);
        map->reinforcements = NULL;
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
        free(map->tilemap);
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
    SOTA_Log_Debug("armies_onfield");
    Map_Tilesindex_Free(map);
    if (map->armies_onfield != NULL) {
        free(map->armies_onfield);
        map->armies_onfield = NULL;
    }
    SOTA_Log_Debug("Arrow");
    Arrow_Free(map->arrow);
    if (map->arrow != NULL) {
        free(map->arrow);
        map->arrow = NULL;
    }
    SOTA_Log_Debug("tilemap_shader");
    if (map->tilemap_shader != NULL) {
        Tilemap_Shader_Free(map->tilemap_shader);
        SDL_free(map->tilemap_shader);
        map->tilemap_shader = NULL;
    }
    if (map->texture != NULL) {
        SDL_DestroyTexture(map->texture);
        map->texture = NULL;
    }
    Map_dArrays_Free(map);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_dArrays_Init(struct Map *map, const struct Settings *settings) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_dArrays_Free(map);
    SDL_assert(map->row_len > 0);
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len < UCHAR_MAX);
    SDL_assert(map->col_len < UCHAR_MAX);
    map->healtolist   = DARR_INIT(map->healtolist,   i32, 32);
    map->attacktolist = DARR_INIT(map->attacktolist, i32, 32);
    int len = map->row_len * map->col_len;
    map->temp           = calloc(len,  sizeof(*map->temp));
    map->unitmap        = calloc(len,  sizeof(*map->unitmap));
    map->costmap        = calloc(len,  sizeof(*map->costmap));
    map->movemap        = calloc(len,  sizeof(*map->movemap));
    map->fcostmap       = calloc(len,  sizeof(*map->fcostmap));
    map->fmovemap       = calloc(len,  sizeof(*map->fmovemap));

    map->dangermap      = calloc(len,  sizeof(*map->dangermap));
    map->palettemap     = malloc(len * sizeof(*map->palettemap));
    map->attacktomap    = calloc(len,  sizeof(*map->attacktomap));
    map->healtomap      = calloc(len,  sizeof(*map->healtomap));
    map->healfrommap    = calloc(len,  sizeof(*map->healfrommap));
    map->temp_palette   = malloc(len * sizeof(*map->temp_palette));
    map->attackfrommap  = calloc(len,  sizeof(*map->attackfrommap));
    map->globalRangemap = calloc(len,  sizeof(*map->globalRangemap));

    if (map->stack_mode == MAP_SETTING_STACK_DANGERMAP)
        map->stacked_dangermap      = calloc(len, sizeof(*map->stacked_dangermap));
    if (map->stack_mode == MAP_SETTING_STACK_MOVEMAP) {
        map->stacked_movemap        = calloc(len, sizeof(*map->stacked_movemap));
        map->stacked_attacktomap    = calloc(len, sizeof(*map->stacked_attacktomap));
    }
    Map_Palettemap_Reset(map);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_dArrays_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->unitmap != NULL) {
        free(map->unitmap);
        map->unitmap = NULL;
    }
    if (map->costmap != NULL) {
        free(map->costmap);
        map->costmap = NULL;
    }
    if (map->movemap != NULL) {
        free(map->movemap);
        map->movemap = NULL;
    }
    if (map->fcostmap != NULL) {
        free(map->fcostmap);
        map->fcostmap = NULL;
    }
    if (map->fmovemap != NULL) {
        free(map->fmovemap);
        map->fmovemap = NULL;
    }
    if (map->attacktomap != NULL) {
        free(map->attacktomap);
        if (map->attacktomap == map->healtomap) {
            map->healtomap = NULL;
        }
        map->attacktomap = NULL;
    }
    if (map->attackfrommap != NULL) {
        free(map->attackfrommap);
        map->attackfrommap = NULL;
    }
    if (map->healtomap != NULL) {
        free(map->healtomap);
        map->healtomap = NULL;
    }
    if (map->healfrommap != NULL) {
        free(map->healfrommap);
        map->healfrommap = NULL;
    }
    if (map->dangermap != NULL) {
        free(map->dangermap);
        map->dangermap = NULL;
    }
    if (map->temp != NULL) {
        free(map->temp);
        map->temp = NULL;
    }
    if (map->globalRangemap != NULL) {
        free(map->globalRangemap);
        map->globalRangemap = NULL;
    }
    if (map->palettemap != NULL) {
        free(map->palettemap);
        map->palettemap = NULL;
    }
    if (map->temp_palette != NULL) {
        free(map->temp_palette);
        map->temp = NULL;
    }
    if (map->stacked_dangermap != NULL) {
        free(map->stacked_dangermap);
        map->stacked_dangermap = NULL;
    }
    if (map->stacked_attacktomap != NULL) {
        free(map->stacked_attacktomap);
        map->stacked_attacktomap = NULL;
    }
    if (map->stacked_movemap != NULL) {
        free(map->stacked_movemap);
        map->stacked_movemap = NULL;
    }
    if (map->attacktolist != NULL) {
        DARR_FREE(map->attacktolist);
        map->attacktolist = NULL;
    }
    if (map->healtolist != NULL) {
        DARR_FREE(map->healtolist);
        map->healtolist = NULL;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Texture_Alloc(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->texture != NULL)
        SDL_DestroyTexture(map->texture);

    map->texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_TARGET, map->tilesize[0] * map->col_len,
                                     map->tilesize[1] * map->row_len);
    SDL_SetTextureBlendMode(map->texture, SDL_BLENDMODE_BLEND);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Shader_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Tilemap_Shader_Free(map->tilemap_shader);
    if (map->tilemap_shader != NULL) {
        SDL_free(map->tilemap_shader);
        map->tilemap_shader = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Shader_Init(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->tilemap_shader         = SDL_malloc(sizeof(struct Tilemap_Shader));
    *map->tilemap_shader        = Tilemap_Shader_default;
    map->tilemap_shader->map    = map;
    map->tilemap_shader->to     = palette_table_NES_shadow;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesindex_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->tilesindex != NULL)
        DARR_FREE(map->tilesindex);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesindex_Init(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Tilesindex_Free(map);
    map->tilesindex = DARR_INIT(map->tilesindex, i32, DEFAULT_TILESPRITE_BUFFER);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void Map_Tilesprites_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->tilesprites_ind == NULL)  {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    for (size_t i = 0; i < DARR_NUM(map->tilesprites_ind); i++) {
        if (map->tilesprites_ind[i] == NULL)
            continue;
        DARR_FREE(map->tilesprites_ind[i]);
        map->tilesprites_ind[i] = NULL;
    }
    DARR_FREE(map->tilesprites_ind);
    map->tilesprites_ind = NULL;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesprites_Init(struct Map *map, size_t tiles_num) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(tiles_num > 0);
    Map_Tilesprites_Free(map);
    map->tilesprites_ind = DARR_INIT(map->tilesprites_ind, uf16 *, tiles_num);
    DARR_NUM(map->tilesprites_ind) = tiles_num;
    for (size_t i = 0; i < tiles_num; i++) {
        map->tilesprites_ind[i] = DARR_INIT(map->tilesprites_ind[i], uf16, DEFAULT_TILESPRITE_BUFFER);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void Map_Tilemap_Texture_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->tilemap_texture != NULL) {
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Texture_Init(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->renderer);
    Map_Tilemap_Texture_Free(map);
    int x_size = map->tilesize[0] * map->col_len;
    int y_size = map->tilesize[1] * map->row_len;
    map->tilemap_texture = SDL_CreateTexture(map->renderer, SDL_PIXELFORMAT_RGB888,
                                             SDL_TEXTUREACCESS_TARGET, x_size, y_size);
    SDL_assert(map->tilemap_texture);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Surface_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->tilemap_surface != NULL)
        SDL_FreeSurface(map->tilemap_surface);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Surface_Init(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->col_len > 0);
    SDL_assert(map->row_len > 0);
    Map_Tilemap_Surface_Free(map);
    int x_size = map->tilesize[0] * map->col_len;
    int y_size = map->tilesize[1] * map->row_len;
    map->tilemap_surface = Filesystem_indexedSurface_Init(x_size, y_size);
    SDL_assert(map->tilemap_surface->w == map->tilesize[0] * map->col_len);
    SDL_assert(map->tilemap_surface->h == map->tilesize[1] * map->row_len);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- I/O --- */
void Map_writeJSON(const void *input, cJSON *jmap) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
            jtilename   = cJSON_CreateString(global_tilenames[temp_tile_id]);
            jtileid     = cJSON_CreateNumber(temp_tile_id);
        } else {
            jtilename   = cJSON_CreateString(global_tilenames[map->tilesindex[i]]);
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
    for (uf8 r = 0; r < DARR_NUM(map->reinforcements); r++) {
        jreinforcement = cJSON_CreateObject();
        jreinforcementeq = cJSON_CreateObject();
        Filesystem_writeJSON_arrival(jreinforcement, &(map->reinforcements)[r]);
        temp_equip = map->reinf_equipments[r];
        for (uf8 i = 0; i < DARR_NUM(temp_equip); i ++) {
            temp_item = temp_equip[i];
            if (temp_item.id > ITEM_NULL)
                Filesystem_writeJSON_item(jreinforcementeq, &temp_item);
        }
        cJSON_AddItemToObject(jreinforcement, "Equipment", jreinforcementeq);
        cJSON_AddItemToObject(jreinforcements, "Reinforcement", jreinforcement);
    }
    cJSON *jbounds  = cJSON_CreateObject();
    cJSON *jrow_min = cJSON_CreateNumber(map->boundsmin.x);
    cJSON *jrow_max = cJSON_CreateNumber(map->boundsmax.x);
    cJSON *jcol_min = cJSON_CreateNumber(map->boundsmin.y);
    cJSON *jcol_max = cJSON_CreateNumber(map->boundsmax.y);
    cJSON_AddItemToObject(jbounds, "row_min", jrow_min);
    cJSON_AddItemToObject(jbounds, "row_max", jrow_max);
    cJSON_AddItemToObject(jbounds, "col_min", jcol_min);
    cJSON_AddItemToObject(jbounds, "col_max", jcol_max);
    cJSON_AddItemToObject(jmap, "Bounds", jbounds);
    cJSON *jtilemap = cJSON_CreateObject();
    Filesystem_writeJSON_2DArray(jtilemap, map->tilemap, map->row_len, map->col_len);
    cJSON_AddItemToObject(jmap, "Tilemap", jtilemap);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_readJSON(void *input, const cJSON *const jmap) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Map *map = (struct Map *) input;
    SDL_assert(jmap != NULL);
    cJSON *jchapter     = cJSON_GetObjectItem(jmap, "chapter");
    cJSON *jmap_size    = cJSON_GetObjectItem(jmap, "Map size");
    cJSON *jrow_len     = cJSON_GetObjectItem(jmap_size, "row_len");
    cJSON *jcol_len     = cJSON_GetObjectItem(jmap_size, "col_len");

    /* -- Read map info -- */
    map->chapter = cJSON_GetNumberValue(jchapter);
    map->row_len = cJSON_GetNumberValue(jrow_len);
    map->col_len = cJSON_GetNumberValue(jcol_len);
    if (map->arrow) {
        map->arrow->col_len = map->col_len;
        map->arrow->row_len = map->row_len;
    }
    SOTA_Log_Debug("%d %d ", map->row_len, map->col_len);
    Map_dArrays_Init(map, &Settings_default);

    /* -- Read Starting Positions -- */
    SOTA_Log_Debug("Read Starting Positions");
    cJSON *jstart_pos_arr = cJSON_GetObjectItem(jmap, "Starting Positions");
    SDL_assert(cJSON_IsArray(jstart_pos_arr));
    if (map->start_pos != NULL)
        DARR_FREE(map->start_pos);
    map->start_pos = DARR_INIT(map->start_pos, struct Point, 16);
    cJSON *jstart_pos;
    struct Point pos;
    cJSON_ArrayForEach(jstart_pos, jstart_pos_arr) {
        Filesystem_readJSON_Position(jstart_pos, (struct Point *)&pos);
        Map_startingPos_Add(map, pos.x, pos.y);
    }

    /* -- Read tiles -- */
    SOTA_Log_Debug("Read tiles");
    cJSON *jtiles = cJSON_GetObjectItem(jmap, "tiles");
    cJSON *jid, *jtile;
    Map_Tilesindex_Init(map);
    Map_Tileset_Stack_Add(map);
    cJSON_ArrayForEach(jtile, jtiles) {
        jid = cJSON_GetObjectItem(jtile, "id");
        DARR_PUT(map->tilesindex, cJSON_GetNumberValue(jid));
    }
    // SDL_assert(map->tileset_surfaces == NULL);
    Map_Tiles_Load(map);
    Map_Tilesets_Load(map);
    SDL_assert(map->tileset_surfaces[0]);

    /* -- Read Reinforcements --  */
    SOTA_Log_Debug("Read Reinforcements");
    if (map->reinforcements != NULL)
        DARR_FREE(map->reinforcements);
    map->reinforcements = DARR_INIT(map->reinforcements, struct Reinforcement, 16);
    if (map->reinf_equipments != NULL)
        DARR_FREE(map->reinf_equipments);
    map->reinf_equipments = DARR_INIT(map->reinf_equipments, struct Inventory_item *, 30);
    if (map->items_num != NULL)
        DARR_FREE(map->items_num);
    map->items_num = DARR_INIT(map->items_num, uf8, DEFAULT_EQUIPMENT_SIZE);
    cJSON *jreinforcements = cJSON_GetObjectItem(jmap, "Reinforcements");
    SDL_assert(jreinforcements != NULL);
    cJSON *jequipment, *jitem;
    struct Inventory_item temp_item, *temp_equip;

    for (int i = 0; i < cJSON_GetArraySize(jreinforcements); i++) {
        struct Reinforcement temp_rein;
        cJSON *jreinforcement = cJSON_GetArrayItem(jreinforcements, i);
        Filesystem_readJSON_Reinforce(jreinforcement, &temp_rein);
        DARR_PUT(map->reinforcements, temp_rein);
        jequipment = cJSON_GetObjectItem(jreinforcement, "Equipment");
        temp_equip = DARR_INIT(temp_equip, struct Inventory_item, DEFAULT_EQUIPMENT_SIZE);
        if (cJSON_IsArray(jequipment)) {
            DARR_PUT(map->items_num, cJSON_GetArraySize(jequipment));
            cJSON_ArrayForEach(jitem, jequipment) {
                temp_item = Inventory_item_default;
                Filesystem_readJSON_Item(jitem, &temp_item);
                DARR_PUT(temp_equip, temp_item);
            }
        } else {
            SOTA_Log_Debug("Missing Equipment array in Reinforcement %d", i);
        }
        DARR_PUT(map->reinf_equipments, temp_equip);
    }

    cJSON *jbounds      = cJSON_GetObjectItem(jmap, "Bounds");
    cJSON *jrow_min     = cJSON_GetObjectItem(jbounds, "row_min");
    cJSON *jrow_max     = cJSON_GetObjectItem(jbounds, "row_max");
    cJSON *jcol_min     = cJSON_GetObjectItem(jbounds, "col_min");
    cJSON *jcol_max     = cJSON_GetObjectItem(jbounds, "col_max");
    map->boundsmin.x    = cJSON_GetNumberValue(jrow_min);
    map->boundsmax.x    = cJSON_GetNumberValue(jrow_max);
    map->boundsmin.y    = cJSON_GetNumberValue(jcol_min);
    map->boundsmax.y    = cJSON_GetNumberValue(jcol_max);

    /* -- Read Map objects -- */
    SOTA_Log_Debug("Read Map objects");
    cJSON *jmobj        = cJSON_GetObjectItem(jmap,  "Map_Objects");
    cJSON *jdoors       = cJSON_GetObjectItem(jmobj, "Doors");
    cJSON *jchests      = cJSON_GetObjectItem(jmobj, "Chests");
    cJSON *jbreakables  = cJSON_GetObjectItem(jmobj, "Breakables");

    SDL_assert(map->world != NULL);

    /* -- Loading chests -- */
    do { /*Loop never executes, just used for break*/
        if (jchests == NULL)
            break;

        if (map->chests_ent != NULL)
            SDL_free(map->chests_ent);
        SDL_assert(cJSON_IsArray(jchests));
        map->chest_num = cJSON_GetArraySize(jchests);

        if (map->chest_num <= 0)
            break;

        map->chests_ent = calloc(map->chest_num, sizeof(*map->chests_ent));
        for (size_t i = 0; i < map->chest_num; i++) {
            tnecs_entity_t temp_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Chest, Position);
            struct Chest *chest     = TNECS_GET_COMPONENT(map->world, temp_ent, Chest);
            struct Position *pos    = TNECS_GET_COMPONENT(map->world, temp_ent, Position);
            cJSON *jchest   = cJSON_GetArrayItem(jchests, i);
            cJSON *jpos     = cJSON_GetObjectItem(jchest, "position");
            if (jpos != NULL)
                Filesystem_readJSON_Position(jpos, (struct Point *)&pos->tilemap_pos);
            if (jchest != NULL)
                Filesystem_readJSON_Chest(jchest, chest);
            map->chests_ent[i] = temp_ent;
        }
    } while (0);

    /* -- Loading doors -- */
    do { /*Loop never executes, just used for break*/
        if (jdoors == NULL)
            break;

        if (map->doors_ent != NULL)
            SDL_free(map->doors_ent);

        SDL_assert(cJSON_IsArray(jdoors));
        map->door_num = cJSON_GetArraySize(jdoors);

        if (map->door_num <= 0)
            break;

        map->doors_ent = calloc(map->door_num, sizeof(*map->doors_ent));
        for (size_t i = 0; i < map->door_num; i++) {
            tnecs_entity_t temp_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Door, Position);
            struct Door *door       = TNECS_GET_COMPONENT(map->world, temp_ent, Door);
            struct Position *pos    = TNECS_GET_COMPONENT(map->world, temp_ent, Position);
            cJSON *jdoor    = cJSON_GetArrayItem(jdoors, i);
            cJSON *jpos     = cJSON_GetObjectItem(jdoor, "position");
            if (jpos != NULL)
                Filesystem_readJSON_Position(jpos, (struct Point *)&pos->tilemap_pos);
            if (jdoor != NULL)
                Filesystem_readJSON_Door(jdoor, door);
            map->doors_ent[i] = temp_ent;
        }
    } while (0);

    /* -- Loading breakables -- */
    do { /*Loop never executes, just used for break*/
        // Note: Breakable component should be added to already existing DOORS/CHESTS
        if (jbreakables == NULL)
            break;

        if (map->breakables_ent != NULL)
            SDL_free(map->breakables_ent);
        SDL_assert(cJSON_IsArray(jbreakables));
        map->breakable_num = cJSON_GetArraySize(jbreakables);

        if (map->breakable_num <= 0)
            break;

        map->breakables_ent = calloc(map->breakable_num, sizeof(*map->breakables_ent));
        for (size_t i = 0; i < map->breakable_num; i++) {
            tnecs_entity_t temp_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(map->world, Breakable, Position);
            struct Position *pos    = TNECS_GET_COMPONENT(map->world, temp_ent, Position);
            cJSON *jbreakable   = cJSON_GetArrayItem(jbreakables, i);
            cJSON *jpos         = cJSON_GetObjectItem(jbreakable, "position");
            if (jpos != NULL)
                Filesystem_readJSON_Position(jpos, (struct Point *)&pos->tilemap_pos);
            // if position of breakaable is already a Door/Chest
            // -> add Breakable component to Door/Chest instead
            // -> add Door/Chest + breakable entity to breakable list
            int x = pos->tilemap_pos.x, y = pos->tilemap_pos.y;
            tnecs_entity_t door_ent     = Map_Find_Door_Ent(map, x, y);
            tnecs_entity_t chest_ent    = Map_Find_Chest_Ent(map, x, y);

            if ((door_ent) || (chest_ent)) {
                tnecs_entity_destroy(map->world, temp_ent);
                temp_ent = door_ent > TNECS_NULL ? door_ent : chest_ent;
                TNECS_ADD_COMPONENT(map->world, temp_ent, Breakable);
            }

            struct Breakable *breaka = TNECS_GET_COMPONENT(map->world, temp_ent, Breakable);
            SDL_assert(breaka != NULL);
            Filesystem_readJSON_Breakable(jbreakables, breaka);
            map->breakables_ent[i] = temp_ent;
        }
    } while (0);

    cJSON *jframe;
    cJSON *jarray;
    cJSON *jtilemap = cJSON_GetObjectItem(jmap, "tilemap");
    cJSON *jframes  = cJSON_GetObjectItem(jtilemap, "frames");
    map->frames     = cJSON_GetNumberValue(jframes);

    if (map->tilemap != NULL)
        free(map->tilemap);

    cJSON_ArrayForEach(jframe, jframes) {
        map->tilemap = calloc(map->row_len * map->col_len, sizeof(*map->tilemap));
        jarray = cJSON_GetObjectItem(jframe, "array");
        Filesystem_readJSON_2DArray(jarray, map->tilemap, map->row_len, map->col_len);
        SDL_assert(map->tilemap);
    }

    /* - Map objects override tiles - */
    Map_Tilemap_MapObjects(map);

    /* -- Misc. Computations -- */
    Map_Bounds_Compute(map);
    Map_Tilemap_Surface_Init(map);
    Map_swappedTextures_All(map);
    Map_Tilemap_Shader_Init(map);
    map->tilemap_shader->frames = map->frames;
    Tilemap_Shader_Load_Tilemap_JSON(map->tilemap_shader, jmap);
    SDL_assert(map->tilemap_shader->shadow_tilemaps);
    Map_Tilemap_Texture_Init(map);
    Map_Texture_Alloc(map);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Map events / Triggers --- */
void Map_Turn_Increment(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->turn++;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
