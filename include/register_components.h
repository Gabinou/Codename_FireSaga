
// TODO: free function for all components.
#define REGISTER_ENUM(x, y, z) TNECS_REGISTER_COMPONENT(world, x, y, z);
#include "names/components.h"
#undef REGISTER_ENUM
