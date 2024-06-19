
#include "equipment.h"

void Equipment_Copy(Inventory_item *dest, Inventory_item *src, size_t size) {
    size_t bytesize = sizeof(*dest) * size;
    memcpy(dest, src, bytesize);
}

void Equipment_Swap(struct Inventory_item *equipment, i16 i1, i16 i2) {
    if ((i1 < 0) || (i1 >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item index1 out of bounds");
        exit(ERROR_OutofBounds);
    }
    if ((i2 < 0) || (i2 >= DEFAULT_EQUIPMENT_SIZE)) {
        SDL_Log("Item index2 out of bounds");
        exit(ERROR_OutofBounds);
    }
    struct Inventory_item buffer  = equipment[i1];
    equipment[i1]          = equipment[i2];
    equipment[i2]          = buffer;
}
