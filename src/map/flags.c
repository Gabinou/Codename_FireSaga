
#include "map/map.h"

b32 Map_isWon(const Map *map) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (map->flags.win);
}

b32 Map_isLost(const Map *map) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (map->flags.loss);
}

b32 Map_isUpdate(const Map *map) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (map->flags.update);
}

i32 Map_Cost_Multiplier(const Map *map) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (map->cost.multiplier);
}

i32 Map_Cost_Effective(const Map *map, i32 move) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (Map_Cost_Multiplier(map) * move);
}

const Point *Map_Tilesize(const Map *map) {
    /* Size of all tiles on map in [pixels] */
    if (map == NULL) {
        SDL_assert(false);
        return (NULL);
    }
    return((const Point *)&map->size.tile);
}

const Point *Map_Gridsize(const Map *map) {
    /* Size of the map in [tiles] */
    if (map == NULL) {
        SDL_assert(false);
        return (NULL);
    }
    return((const Point *)&map->size.tile);
}

i32 Map_row_len(const Map *map) {
    /* Size of the map in [tiles] in y */
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return(map->size.tile.y);
}

i32 Map_col_len(const Map *map) {
    /* Size of the map in [tiles] in x */
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return(map->size.tile.x);
}

i32 Map_area(const Map *map) {
    /* Size of the map in [tiles] in x*y */
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return(map->size.tile.x * map->size.tile.y);
}