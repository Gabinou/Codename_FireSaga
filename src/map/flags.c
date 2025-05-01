
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
