
#define REGISTER_ENUM(phase) tnecs_register_phase(world, TNECS_PIPELINE_RENDER);
#include "names/tnecs_render_phases.h"
#undef REGISTER_ENUM

#define REGISTER_ENUM(phase) tnecs_register_phase(world, TNECS_PIPELINE_TURN_END);
#include "names/tnecs_turn_end_phases.h"
#undef REGISTER_ENUM
