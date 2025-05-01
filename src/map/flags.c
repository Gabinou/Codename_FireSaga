
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
