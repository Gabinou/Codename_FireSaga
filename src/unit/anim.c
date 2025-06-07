
#include "unit/anim.h"

const struct UnitMoveAnimation UnitMoveAnimation_default = {
    .time_ns         = SOTA_ns / 2ULL,
};

void UnitMoveAnimation_Init(struct UnitMoveAnimation *uanim) {
    *uanim = UnitMoveAnimation_default;
}

void UnitMoveAnimation_Init_tnecs(void *voidscene) {
    UnitMoveAnimation_Init(voidscene);
}
