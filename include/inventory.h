#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <math.h>
#include "types.h"
#include "enums.h"

void Equipment_Copy(Inventory_item *d, Inventory_item *s, size_t size);
void Equipment_Swap(Inventory_item *d, i16 i1, i16 i2);

#endif /* EQUIPMENT_H */
