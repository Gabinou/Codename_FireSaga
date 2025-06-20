
// #include "systems/control.h"
// #include "systems/map.h"
// #include "systems/slide.h"
// #include "systems/render.h"
// #include "systems/time_system.h"
// #include "systems/turn_end.h"

#define REGISTER_ENUM(pfunc, pipe, phase, excl, ...) \
    TNECS_REGISTER_SYSTEM(world, pfunc, pipe, phase, excl, __VA_ARGS__);
#include "names/systems.h"
#undef REGISTER_ENUM
