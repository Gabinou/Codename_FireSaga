#ifndef UNIT_ANIM_H
#define UNIT_ANIM_H

/* --- UnitMoveAnimation --- */
typedef struct UnitMoveAnimation {
    u64 time_ns;
    struct Point target;
    tnecs_entity unit;
} UnitMoveAnimation;
extern struct UnitMoveAnimation UnitMoveAnimation_default;

#endif /* UNIT_ANIM_H */
