#ifndef UNIT_ANIM_H
#define UNIT_ANIM_H

#include "types.h"
#include "structs.h"
#include "tnecs.h"

/* --- UnitMoveAnimation --- */
typedef struct UnitMoveAnimation {
    struct Point target;
    u64 time_ns;
    tnecs_E unit;
} UnitMoveAnimation;
extern const struct UnitMoveAnimation UnitMoveAnimation_default;

void UnitMoveAnimation_Init(struct UnitMoveAnimation *uanim);
void UnitMoveAnimation_Init_tnecs(void *voidscene);


#endif /* UNIT_ANIM_H */
