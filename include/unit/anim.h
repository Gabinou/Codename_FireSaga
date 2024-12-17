#ifndef UNIT_ANIM_H
#define UNIT_ANIM_H

#include "types.h"
#include "structs.h"
#include "tnecs.h"

/* --- UnitMoveAnimation --- */
typedef struct UnitMoveAnimation {
    u64 time_ns;
    struct Point target;
    tnecs_entity unit;
} UnitMoveAnimation;
extern const struct UnitMoveAnimation UnitMoveAnimation_default;

#endif /* UNIT_ANIM_H */
