
#define REGISTER_ENUM(pfunc, pipe, phase, excl, ...) \
    TNECS_REGISTER_SYSTEM(IES->ecs.world, pfunc, pipe, phase, excl, __VA_ARGS__);
#include "names/systems.h"
#undef REGISTER_ENUM
