
#include "map/tiles.h"
#include "map/map.h"
#include "tile.h"
#include "names.h"
#include "jsonio.h"
#include "log.h"
#include "nmath.h"
#include "macros.h"
#include "utilities.h"
#include "position.h"
#include "palette.h"
#include "platform.h"
#include "filesystem.h"

void Map_Tiles_Free(struct Map *map) {
    Map_Tilesets_Free(map);
    if (map->tiles.arr != NULL) {
        for (size_t i = 0; i < DARR_NUM(map->tiles.arr); i++)
            Tile_Free(&map->tiles.arr[i]);
        DARR_FREE(map->tiles.arr);
        map->tiles.arr = NULL;
    }
    if (map->tiles.id != NULL) {
        DARR_FREE(map->tiles.id);
        map->tiles.id = NULL;
    }
}

void Map_Tiles_Load(struct Map *map) {
    Map_Tiles_Free(map);
    if (map->tiles.arr == NULL) {
        map->tiles.arr      = DARR_INIT(map->tiles.arr, struct Tile, 64);
    }
    if (map->tiles.id == NULL) {
        map->tiles.id   = DARR_INIT(map->tiles.id, i32, 64);
    }

    for (size_t i = 0; i < DARR_NUM(map->tiles.index); i++) {
        /* -- Get tile_id -- */
        i32 tile_id = map->tiles.index[i];
        /* - If above TILE_ID_MAX, tile_id encodes the tile sprite too - */
        (tile_id > TILE_ID_MAX) && (tile_id /= TILE_DIVISOR);   /* short-circuit */

        SDL_assert(Tile_Valid_ID(tile_id));
        /* -- Alloc tile -- */
        struct Tile temp_tile = Tile_default;

        /* -- Load tile -- */
        s8 filename = s8_mut("tiles" PHYSFS_SEPARATOR);
        filename    = s8cat(filename, global_tilenames[tile_id]);
        filename    = s8cat(filename, s8_literal(".json"));
        jsonio_readJSON(filename, &temp_tile);
        temp_tile.id = tile_id;
        DARR_PUT(map->tiles.arr, temp_tile);
        DARR_PUT(map->tiles.id, tile_id);
        s8_free(&filename);
    }
    SDL_assert(DARR_NUM(map->tiles.id) == DARR_NUM(map->tiles.index));
}

void Map_Tilesets_Free(struct Map *map) {
    if (map->tiles.arr == NULL)
        return;

    if (DARR_NUM(map->tiles.arr) <= 0)
        return;

    SDL_assert(DARR_NUM(map->tiles.arr) < 1000);

    /* -- SDL_free Tileset Surfaces -- */
    do { /*Loop never executes, just used for break*/

        if (map->tiles.tileset_surfaces == NULL)
            break;

        /* Note: All surfaces use data from nes palette surface */
        for (int i = PALETTE_NUM - 1; i >= 0; i--) {
            if (map->tiles.tileset_surfaces[i] == NULL)
                continue;

            SOTA_Free_Surfaces(map->tiles.tileset_surfaces[i], DARR_NUM(map->tiles.arr));
            SDL_free(map->tiles.tileset_surfaces[i]);
            map->tiles.tileset_surfaces[i] = NULL;
        }
        SDL_free(map->tiles.tileset_surfaces);
        map->tiles.tileset_surfaces = NULL;
    } while (0);

    /* -- SDL_free Tileset Textures -- */
    do { /*Loop never executes, just used for break*/
        if (map->tiles.tileset_textures == NULL)
            break;
        // Note: All surfaces use data from nes palette surface
        for (int i = PALETTE_NUM - 1; i >= 0; i--) {
            if (map->tiles.tileset_textures[i] == NULL)
                continue;

            SOTA_Free_Textures(map->tiles.tileset_textures[i], DARR_NUM(map->tiles.arr));
            SDL_free(map->tiles.tileset_textures[i]);
            map->tiles.tileset_textures[i] = NULL;
        }
        SDL_free(map->tiles.tileset_textures);
        map->tiles.tileset_textures = NULL;
    } while (0);

}

void Map_Tilesets_Load(struct Map *map) {
    SDL_assert(DARR_NUM(map->tiles.arr) > 0);
    SDL_assert(map->tiles.index != NULL);
    /* -- Preliminaries -- */
    i32 tile_ind;
    Map_Tilesets_Free(map);

    /* -- Alloc tilesets -- */
    // TODO: Alloc only used palettes
    map->tiles.tileset_surfaces = SDL_calloc(PALETTE_NUM, sizeof(*map->tiles.tileset_surfaces));
    map->tiles.tileset_textures = SDL_calloc(PALETTE_NUM, sizeof(*map->tiles.tileset_textures));
    for (size_t i = 0; i < PALETTE_NUM; i++) {
        map->tiles.tileset_surfaces[i] = SDL_calloc(DARR_NUM(map->tiles.arr),
                                                    sizeof(*map->tiles.tileset_surfaces[i]));
        map->tiles.tileset_textures[i] = SDL_calloc(DARR_NUM(map->tiles.arr),
                                                    sizeof(*map->tiles.tileset_textures[i]));
    }

    /* -- Load tilesets -- */
    for (size_t i = 0; i < DARR_NUM(map->tiles.arr); i++) {
        tile_ind = map->tiles.index[i];
        /* - If above TILE_ID_MAX, tile_id encodes the tile sprite too - */
        (tile_ind > TILE_ID_MAX) && (tile_ind /= TILE_DIVISOR);   /* short-circuit */
        SDL_assert(tile_ind > 0);

        /* - Get tile - */
        s8 tilesetname = s8_mut(PATH_JOIN("..", "assets", "tiles"));
        size_t tile_order = Map_Tile_Order(map, tile_ind);
        struct Tile *temp_tile = map->tiles.arr + tile_order;

        /* - Load tileset - */
        tilesetname = s8cat(tilesetname, s8_literal(DIR_SEPARATOR"Tileset_"));
        SDL_assert(Tile_Valid_ID(temp_tile->id));
        s8 tilename = Tile_Name(temp_tile);
        SDL_assert(tilename.data != NULL);
        tilesetname = s8cat(tilesetname, tilename);
        tilesetname = s8cat(tilesetname, s8_literal(".png"));
        if (map->tiles.tileset_surfaces[map->palette.base][i] != NULL)
            SDL_FreeSurface(map->tiles.tileset_surfaces[map->palette.base][i]);
        SDL_Surface *surf = Filesystem_Surface_Load(tilesetname.data, SDL_PIXELFORMAT_INDEX8);
        /* To check if tilesets get loaded properly: check the output surface */
        SDL_assert(surf != NULL);
        SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));
        map->tiles.tileset_surfaces[map->palette.base][i] = surf;
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
    if (DARR_NUM(map->entities.chests) <= 0)
        return;

    for (size_t i = 0; i < DARR_NUM(map->entities.chests); i++) {
        struct Chest    *chest  = IES_GET_COMPONENT(map->world, map->entities.chests[i], Chest);
        struct Position *pos    = IES_GET_COMPONENT(map->world, map->entities.chests[i], Position);
        if (chest->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = chest->tile + TILE_CHEST * TILE_DIVISOR;
        map->darrs.tilemap[y * Map_col_len(map) + x] = tile;
    }
}

void Map_Tilemap_Breakables(struct Map *map) {
    if (DARR_NUM(map->entities.breakables) <= 0)
        return;

    for (size_t i = 0; i < DARR_NUM(map->entities.breakables); i++) {
        struct Breakable *breaka;
        breaka = IES_GET_COMPONENT(map->world, map->entities.breakables[i], Breakable);
        SDL_assert(breaka != NULL);
        struct Position *pos;
        pos = IES_GET_COMPONENT(map->world, map->entities.breakables[i], Position);
        SDL_assert(pos != NULL);
        if (breaka->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = breaka->tile + TILE_DOOR * TILE_DIVISOR;
        map->darrs.tilemap[y * Map_col_len(map) + x] = tile;
    }
}

void Map_Tilemap_Doors(struct Map *map) {
    if (DARR_NUM(map->entities.doors) <= 0)
        return;

    for (size_t i = 0; i < DARR_NUM(map->entities.doors); i++) {
        struct Door *door = IES_GET_COMPONENT(map->world, map->entities.doors[i], Door);
        struct Position *pos = IES_GET_COMPONENT(map->world, map->entities.doors[i], Position);
        if (door->tile == 0)
            continue;
        int x = pos->tilemap_pos.x;
        int y = pos->tilemap_pos.y;
        i32 tile = door->tile + TILE_DOOR * TILE_DIVISOR;
        map->darrs.tilemap[y * Map_col_len(map) + x] = tile;
    }
}

u8 Map_Tile_Order(struct Map *map, i32 tile) {
    SDL_assert(Tile_Valid_ID(tile));
    SDL_assert(tile > TILE_START);
    SDL_assert(map->tiles.id);
    SDL_assert(map->tiles.arr);
    SDL_assert(DARR_NUM(map->tiles.arr) == DARR_NUM(map->tiles.id));
    SDL_assert(DARR_NUM(map->tiles.arr)     > 0);
    SDL_assert(DARR_NUM(map->tiles.id)  > 0);
    u8 out = UINT8_MAX;
    for (u8 i = 0; i < DARR_NUM(map->tiles.id); i++) {
        if (map->tiles.id[i] == tile) {
            out = i;
            break;
        }
    }
    if (out >= DARR_NUM(map->tiles.arr)) {
        SDL_LogError(SOTA_LOG_SYSTEM, "Tile %d '%s' not found in map", tile, global_tilenames[tile].data);
        exit(1);
    }
    return (out);
}

void Map_Tileset_newPalette(struct Map *map, i32 tile, u8 palette) {
    /* -- Preliminaries -- */
    SDL_assert(map);
    SDL_assert(tile > 0);
    u8 tileset_order = Map_Tile_Order(map, tile);
    SDL_assert(map->tiles.tileset_surfaces);
    SDL_assert(map->tiles.tileset_surfaces[map->palette.base]);
    SDL_assert(map->tiles.tileset_surfaces[map->palette.base][tileset_order]);
    SDL_assert(palette < PALETTE_NUM);

    /* -- Swap surface palette -- */
    SDL_Surface *old_surf = map->tiles.tileset_surfaces[map->palette.base][tileset_order];
    /* - Allocate new surface - */
    SDL_Surface *new_surf = Filesystem_Surface_Palette_Swap(old_surf, sota_palettes[palette]);

    SDL_FreeSurface(map->tiles.tileset_surfaces[palette][tileset_order]);
    map->tiles.tileset_surfaces[palette][tileset_order] = new_surf;
    SDL_assert(new_surf != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(new_surf->format->format));
    SDL_assert(new_surf->w > 0);
    SDL_assert(new_surf->h > 0);
    SDL_assert(map->renderer);

    /* -- New texture from surface -- */
    if (map->tiles.tileset_textures[palette][tileset_order] != NULL)
        SDL_DestroyTexture(map->tiles.tileset_textures[palette][tileset_order]);
    SDL_Texture *new_textu = SDL_CreateTextureFromSurface(map->renderer, new_surf);
    map->tiles.tileset_textures[palette][tileset_order] = new_textu;
}

