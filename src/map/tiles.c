
#include "map/tiles.h"

void Map_Tiles_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (map->tiles != NULL) {
        for (size_t i = 0; i < DARR_NUM(map->tiles); i++)
            Tile_Free(&map->tiles[i]);
        DARR_FREE(map->tiles);
        map->tiles = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tiles_Load(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Tile *temp_tile = NULL;
    char filename[DEFAULT_BUFFER_SIZE];
    for (size_t i = 0; i < DARR_NUM(map->tilesindex); i++) {
        /* -- Get tile_id -- */
        i32 tile_id = map->tilesindex[i];
        /* - If above TILE_ID_MAX, tile_id encodes the tile sprite too - */
        (tile_id > TILE_ID_MAX) &&(tile_id /= TILE_DIVISOR);   /* short-circuit */

        /* -- Alloc tile -- */
        temp_tile   = malloc(sizeof(struct Tile));
        *temp_tile  = Tile_default;

        /* -- Load tile -- */
        strcpy(filename, "");
        strcat(filename, "tiles");
        strcat(filename, PHYSFS_SEPARATOR);
        strcat(filename, global_tilenames[tile_id]);
        strcat(filename, ".json");
        jsonio_readJSON(filename, temp_tile);
        Tile_makeMvtCostarray(temp_tile);
        DARR_PUT(map->tiles, *temp_tile);
        DARR_PUT(map->tiles_id, tile_id);
        free(temp_tile);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesets_Free(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(DARR_NUM(map->tiles) > 0);
    SDL_assert(DARR_NUM(map->tiles) < 1000);

    /* -- Free Tileset Surfaces -- */
    do { /*Loop never executes, just used for break*/

        if (map->tileset_surfaces == NULL)
            break;

        // Note: All surfaces use data from nes palette surface
        for (int i = PALETTE_NUM - 1; i >= 0; i--) {
            if (map->tileset_surfaces[i] == NULL)
                continue;

            SOTA_Free_Surfaces(map->tileset_surfaces[i], DARR_NUM(map->tiles));
            free(map->tileset_surfaces[i]);
            map->tileset_surfaces[i] = NULL;
        }
        free(map->tileset_surfaces);
        map->tileset_surfaces = NULL;
    } while (0);

    /* -- Free Tileset Textures -- */
    do { /*Loop never executes, just used for break*/
        if (map->tileset_textures == NULL)
            break;
        // Note: All surfaces use data from nes palette surface
        for (int i = PALETTE_NUM - 1; i >= 0; i--) {
            if (map->tileset_textures[i] == NULL)
                continue;

            SOTA_Free_Textures(map->tileset_textures[i], DARR_NUM(map->tiles));
            free(map->tileset_textures[i]);
            map->tileset_textures[i] = NULL;
        }
        free(map->tileset_textures);
        map->tileset_textures = NULL;
    } while (0);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesets_Load(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(DARR_NUM(map->tiles) > 0);
    SDL_assert(map->tilesindex != NULL);
    /* -- Preliminaries -- */
    i32 tile_ind;
    char tilesetname[DEFAULT_BUFFER_SIZE] = "";
    Map_Tilesets_Free(map);

    /* -- Alloc tilesets -- */
    map->tileset_surfaces = calloc(PALETTE_NUM, sizeof(*map->tileset_surfaces));
    map->tileset_textures = calloc(PALETTE_NUM, sizeof(*map->tileset_textures));
    for (size_t i = 0; i < PALETTE_NUM; i++) {
        map->tileset_surfaces[i] = calloc(DARR_NUM(map->tiles), sizeof(*map->tileset_surfaces[i]));
        map->tileset_textures[i] = calloc(DARR_NUM(map->tiles), sizeof(*map->tileset_textures[i]));
    }

    /* -- Load tilesets -- */
    for (size_t i = 0; i < DARR_NUM(map->tiles); i++) {
        tile_ind = map->tilesindex[i];
        /* - If above TILE_ID_MAX, tile_id encodes the tile sprite too - */
        (tile_ind > TILE_ID_MAX) &&(tile_ind /= TILE_DIVISOR);   /* short-circuit */
        SDL_assert(tile_ind > 0);

        /* - Get tile - */
        strcat(tilesetname, PATH_JOIN("..", "assets", "Tiles"));
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles + tile_order;

        /* - Load tileset - */
        strcat(tilesetname, DIR_SEPARATOR);
        strcat(tilesetname, "Tileset_");
        strcat(tilesetname, temp_tile->name);
        strcat(tilesetname, ".png");
        if (map->tileset_surfaces[PALETTE_NES][i] != NULL)
            SDL_FreeSurface(map->tileset_surfaces[PALETTE_NES][i]);

        SDL_Surface *surf = Filesystem_Surface_Load(tilesetname, SDL_PIXELFORMAT_INDEX8);
        // SDL_SaveBMP(map->tileset_surfaces[PALETTE_NES][i], strcat(temp_tile->name, ".bmp"));
        SDL_assert(surf != NULL);
        SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));
        map->tileset_surfaces[PALETTE_NES][i] = surf;
        memset(&tilesetname, 0, DEFAULT_BUFFER_SIZE);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_MapObjects(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Map_Tilemap_Breakables(map);
    /* Breakables tiles overriden by doors/chests */
    Map_Tilemap_Doors(map);
    Map_Tilemap_Chests(map);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Chests(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->chest_num > 0);
    for (size_t i = 0; i < map->chest_num; i++) {
        struct Chest *chest     = TNECS_GET_COMPONENT(map->world, map->chests_ent[i], Chest);
        struct Position *pos    = TNECS_GET_COMPONENT(map->world, map->chests_ent[i], Position);
        if (chest->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = chest->tile + TILE_CHEST * TILE_DIVISOR;
        map->tilemap[y * map->col_len + x] = tile;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Breakables(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->breakable_num > 0);
    for (size_t i = 0; i < map->breakable_num; i++) {
        struct Breakable *breaka;
        breaka = TNECS_GET_COMPONENT(map->world, map->breakables_ent[i], Breakable);
        SDL_assert(breaka != NULL);
        struct Position *pos;
        pos = TNECS_GET_COMPONENT(map->world, map->breakables_ent[i], Position);
        SDL_assert(pos != NULL);
        if (breaka->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = breaka->tile + TILE_DOOR * TILE_DIVISOR;
        map->tilemap[y * map->col_len + x] = tile;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilemap_Doors(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->door_num > 0);
    for (size_t i = 0; i < map->door_num; i++) {
        struct Door *door = TNECS_GET_COMPONENT(map->world, map->doors_ent[i], Door);
        struct Position *pos = TNECS_GET_COMPONENT(map->world, map->doors_ent[i], Position);
        if (door->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = door->tile + TILE_DOOR * TILE_DIVISOR;
        map->tilemap[y * map->col_len + x] = tile;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tileset_Stack_Add(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->tilesindex);
    DARR_PUT(map->tilesindex, TILE_ICONS);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Tilesize_Set(struct Map *map, i32 width, i32 height) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    map->tilesize[0] = width;
    map->tilesize[1] = height;
    if (map->arrow) {
        map->arrow->map_tilesize[0] = width;
        map->arrow->map_tilesize[1] = height;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Map_Unique_TilesindexfromTilemap(struct Map *map) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(map->tilemap);
    for (size_t i = 0; i < map->col_len * map->row_len; i++) {
        i32 tile_ind = map->tilemap[i] / TILE_DIVISOR;
        i32 tofind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        size_t *tile_orders = linalg_where_int32_t(map->tilesindex, tile_ind, DARR_NUM(map->tilesindex));
        SDL_assert(DARR_LEN(tile_orders) == 1);
        uf16 *tilesprite_ind = map->tilesprites_ind[tile_orders[0]];
        uf16 tilesnum = map->tilesprites_num[tile_orders[0]];
        bool found = linalg_isIn_uint_fast16_t(tilesprite_ind, tofind, tilesnum);
        if (!found)
            DARR_PUT(tilesprite_ind, tofind);
        DARR_FREE(tile_orders);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

uf8 Map_Tile_Order(struct Map *map, i32 tile) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(tile > TILE_START);
    SDL_assert(map->tiles_id);
    SDL_assert(map->tiles);
    SDL_assert(DARR_NUM(map->tiles) == DARR_NUM(map->tiles_id));
    uf8 out = UINT8_MAX;
    for (uf8 i = 0; i < DARR_NUM(map->tiles); i++) {
        if (map->tiles_id[i] == tile) {
            out = i;
            break;
        }
    }
    SDL_assert(out != UINT8_MAX);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void Map_Tileset_newPalette(struct Map *map, i32 tile, uf8 palette) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(map);
    SDL_assert(tile > 0);
    uf8 tileset_order = Map_Tile_Order(map, tile);
    SDL_assert(map->tileset_surfaces);
    SDL_assert(map->tileset_surfaces[PALETTE_NES]);
    SDL_assert(map->tileset_surfaces[PALETTE_NES][tileset_order]);
    SDL_assert(palette < PALETTE_NUM);

    /* -- Swap surface palette -- */
    SDL_Surface *old_surf = map->tileset_surfaces[PALETTE_NES][tileset_order];
    /* - Allocate new surface - */
    SDL_Surface *new_surf = Filesystem_Surface_Palette_Swap(old_surf, sota_palettes[palette]);

    SDL_FreeSurface(map->tileset_surfaces[palette][tileset_order]);
    map->tileset_surfaces[palette][tileset_order] = new_surf;
    SDL_assert(new_surf != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(new_surf->format->format));
    SDL_assert(new_surf->w > 0);
    SDL_assert(new_surf->h > 0);
    SDL_assert(map->renderer);

    /* -- New texture from surface -- */
    if (map->tileset_textures[palette][tileset_order] != NULL)
        SDL_DestroyTexture(map->tileset_textures[palette][tileset_order]);
    SDL_Texture *new_textu = SDL_CreateTextureFromSurface(map->renderer, new_surf);
    map->tileset_textures[palette][tileset_order] = new_textu;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

