
#include "equipment.h"

void Equipment_Copy(Inventory_item *dest, Inventory_item *src, size_t size) {
    size_t bytesize = sizeof(*dest) * size;
    memcpy(dest, src, bytesize);
}
