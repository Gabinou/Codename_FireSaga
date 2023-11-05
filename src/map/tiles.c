
#include "map/tiles.h"

void Map_Tiles_SDL_free(struct Map *map) {
    Map_Tilesets_SDL_free(map);
    if (map->tiles != NULL) {
        for (size_t i = 0; i < DARR_NUM(map->tiles); i++)
            Tile_SDL_free(&map->tiles[i]);
        DARR_FREE(map->tiles);
        map->tiles = NULL;
    }
    if (map->tiles_id != NULL) {
        DARR_FREE(map->tiles_id);
        map->tiles_id = NULL;
    }
}

void Map_Tiles_Load(struct Map *map) {
    Map_Tiles_SDL_free(map);
    struct Tile *temp_tile = NULL;
    map->tiles      = DARR_INIT(map->tiles, struct Tile, 64);
    map->tiles_id   = DARR_INIT(map->tiles_id, i32, 64);

    for (size_t i = 0; i < DARR_NUM(map->tilesindex); i++) {
        /* -- Get tile_id -- */
        i32 tile_id = map->tilesindex[i];
        /* - If above TILE_ID_MAX, tile_id encodes the tile sprite too - */
        (tile_id > TILE_ID_MAX) && (tile_id /= TILE_DIVISOR);   /* short-circuit */

        /* -- Alloc tile -- */
        temp_tile   = malloc(sizeof(struct Tile));
        *temp_tile  = Tile_default;

        /* -- Load tile -- */
        s8 filename = s8_mut("tiles" PHYSFS_SEPARATOR);
        filename    = s8cat(filename, global_tilenames[tile_id]);
        filename    = s8cat(filename, s8_literal(".json"));
        jsonio_readJSON(filename, temp_tile);
        Tile_makeMvtCostarray(temp_tile);
        DARR_PUT(map->tiles, *temp_tile);
        DARR_PUT(map->tiles_id, tile_id);
        s8_free(&filename);
        SDL_free(temp_tile);
    }
}

void Map_Tilesets_SDL_free(struct Map *map) {
    if (map->tiles == NULL)
        return;

    if (DARR_NUM(map->tiles) <= 0)
        return;

    SDL_assert(DARR_NUM(map->tiles) < 1000);

    /* -- Free Tileset Surfaces -- */
    do { /*Loop never executes, just used for break*/

        if (map->tileset_surfaces == NULL)
            break;

        /* Note: All surfaces use data from nes palette surface */
        for (int i = PALETTE_NUM - 1; i >= 0; i--) {
            if (map->tileset_surfaces[i] == NULL)
                continue;

            SOTA_Free_Surfaces(map->tileset_surfaces[i], DARR_NUM(map->tiles));
            SDL_free(map->tileset_surfaces[i]);
            map->tileset_surfaces[i] = NULL;
        }
        SDL_free(map->tileset_surfaces);
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
            SDL_free(map->tileset_textures[i]);
            map->tileset_textures[i] = NULL;
        }
        SDL_free(map->tileset_textures);
        map->tileset_textures = NULL;
    } while (0);

}

void Map_Tilesets_Load(struct Map *map) {
    SDL_assert(DARR_NUM(map->tiles) > 0);
    SDL_assert(map->tilesindex != NULL);
    /* -- Preliminaries -- */
    i32 tile_ind;
    char tilesetname[DEFAULT_BUFFER_SIZE] = "";
    Map_Tilesets_SDL_free(map);

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
        s8 tilesetname = s8_mut(PATH_JOIN("..", "assets", "Tiles"));
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles + tile_order;

        /* - Load tileset - */
        tilesetname = s8cat(tilesetname, s8_literal(DIR_SEPARATOR"Tileset_"));
        tilesetname = s8cat(tilesetname, s8_var(temp_tile->name));
        tilesetname = s8cat(tilesetname, s8_literal(".png"));
        if (map->tileset_surfaces[PALETTE_NES][i] != NULL)
            SDL_FreeSurface(map->tileset_surfaces[PALETTE_NES][i]);

        SDL_Surface *surf = Filesystem_Surface_Load(tilesetname.data, SDL_PIXELFORMAT_INDEX8);
        SDL_assert(surf != NULL);
        SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));
        map->tileset_surfaces[PALETTE_NES][i] = surf;
        s8_free(&tilesetname);
    }
}

void Map_Tilemap_MapObjects(struct Map *map) {
    Map_Tilemap_Breakables(map);
    /* Breakables tiles overriden by doors/chests */
    Map_Tilemap_Doors(map);
    Map_Tilemap_Chests(map);
}

void Map_Tilemap_Chests(struct Map *map) {
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
}

void Map_Tilemap_Breakables(struct Map *map) {
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

}

void Map_Tilemap_Doors(struct Map *map) {
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
}

void Map_Tileset_Stack_Add(struct Map *map) {
    SDL_assert(map->tilesindex);
    DARR_PUT(map->tilesindex, TILE_ICONS);
}

void Map_Tilesize_Set(struct Map *map, i32 width, i32 height) {
    map->tilesize[0] = width;
    map->tilesize[1] = height;
    if (map->arrow) {
        map->arrow->map_tilesize[0] = width;
        map->arrow->map_tilesize[1] = height;
    }
}

void Map_Unique_TilesindexfromTilemap(struct Map *map) {
    SDL_assert(map->tilemap);
    for (size_t i = 0; i < map->col_len * map->row_len; i++) {
        i32 tile_ind = map->tilemap[i] / TILE_DIVISOR;
        i32 tofind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        size_t *tile_orders = matrix_where(map->tilesindex, tile_ind, DARR_NUM(map->tilesindex));
        SDL_assert(DARR_LEN(tile_orders) == 1);
        u16 *tilesprite_ind = map->tilesprites_ind[tile_orders[0]];
        u16 tilesnum = map->tilesprites_num[tile_orders[0]];
        bool found = false;
        for (size_t i = 0; i < tilesnum; i++) {
            if (tilesprite_ind[i] == tofind) {
                found = true;
                break;
            }
        }
        if (!found)
            DARR_PUT(tilesprite_ind, tofind);
        DARR_FREE(tile_orders);
    }
}

u8 Map_Tile_Order(struct Map *map, i32 tile) {
    SDL_assert(tile > TILE_START);
    SDL_assert(map->tiles_id);
    SDL_assert(map->tiles);
    SDL_assert(DARR_NUM(map->tiles) == DARR_NUM(map->tiles_id));
    u8 out = UINT8_MAX;
    for (u8 i = 0; i < DARR_NUM(map->tiles); i++) {
        if (map->tiles_id[i] == tile) {
            out = i;
            break;
        }
    }
    SDL_assert(out != UINT8_MAX);
    return (out);
}

void Map_Tileset_newPalette(struct Map *map, i32 tile, u8 palette) {
    /* -- Preliminaries -- */
    SDL_assert(map);
    SDL_assert(tile > 0);
    u8 tileset_order = Map_Tile_Order(map, tile);
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
}

