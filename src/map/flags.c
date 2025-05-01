
#include "map/map.h"

b32 Map_isWon(const Unit *map) {
    if (map == NULL) {
        SDL_assert(false);
        return (0);
    }
    return (map->flags.win);
}
