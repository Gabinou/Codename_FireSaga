#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "tnecs.h"

void Equipment_Copy(tnecs_entity *d,
                    tnecs_entity *s,
                    size_t size);
void Equipment_Swap(tnecs_entity *d,
                    i16 i1, i16 i2);

#endif /* EQUIPMENT_H */
