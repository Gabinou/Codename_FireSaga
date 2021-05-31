
#include "controllerTouchpad.h"

void controllerTouchpadModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, controllerTouchpadModule);
    ECS_COMPONENT(in_world, controllerTouchpad);
    ECS_SET_COMPONENT(controllerTouchpad);
    ECS_EXPORT_COMPONENT(controllerTouchpad);
}
