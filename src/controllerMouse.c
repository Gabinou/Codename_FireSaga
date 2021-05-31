
#include "controllerMouse.h"

void controllerMouseModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, controllerMouseModule);
    ECS_COMPONENT(in_world, controllerMouse);
    ECS_SET_COMPONENT(controllerMouse);
    ECS_EXPORT_COMPONENT(controllerMouse);
}
