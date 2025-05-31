
// TODO: free function for all components.
#define REGISTER_ENUM(x) TNECS_REGISTER_COMPONENT(world, x, NULL, NULL);
#include "names/components.h"
#undef REGISTER_ENUM
