
// TODO: free function for all components.
#define REGISTER_ENUM(name, init, free) TNECS_REGISTER_C(world, name, init, free);
#include "names/components.h"
#undef REGISTER_ENUM
