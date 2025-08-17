
#include "equipment.h"

void Equipment_Copy(tnecs_E *dest,
                    tnecs_E *src,
                    size_t size) {
    size_t bytesize = sizeof(*dest) * size;
    memcpy(dest, src, bytesize);
}

void Equipment_Swap(tnecs_E *equipment,
                    i16 i1, i16 i2) {
    // TODO: NOT exit -> assert and print warning
    if ((i1 < ITEM1) || (i1 > SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Item index1 out of bounds");
        SDL_assert(false);
        return;
    }
    if ((i2 < ITEM1) || (i2 > SOTA_EQUIPMENT_SIZE)) {
        SDL_Log("Item index2 out of bounds");
        SDL_assert(false);
        return;
    }
    tnecs_E buffer = equipment[i1];
    equipment[i1]       = equipment[i2];
    equipment[i2]       = buffer;
}
