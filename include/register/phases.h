
#define REGISTER_ENUM(phase) tnecs_register_phase(IES->ecs.world, TNECS_PIPELINE_RENDER);
#include "names/tnecs_render_phases.h"
#undef REGISTER_ENUM
