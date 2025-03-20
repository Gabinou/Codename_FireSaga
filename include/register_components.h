
#include "sprite.h"
#include "boss.h"
#include "menu/menu.h"
#include "structs.h"

#define REGISTER_ENUM(x) TNECS_REGISTER_COMPONENT(world, x);
#include "names/components.h"
#undef REGISTER_ENUM
