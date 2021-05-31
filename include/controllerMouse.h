#ifndef CONTROLLERMOUSE_H
#define CONTROLLERMOUSE_H

#include <math.h>
#include "narrative.h"
#include "flecs.h"

typedef struct controllerMouse {
    struct MouseInputMap * inputmap;
} controllerMouse;
extern struct controllerMouse controllerMouse_default;


typedef struct controllerMouseModule {
    ECS_DECLARE_COMPONENT(controllerMouse);
} controllerMouseModule;

void controllerMouseModuleImport(ecs_world_t * in_world);
#define controllerMouseModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, controllerMouse);

#endif /* CONTROLLERMOUSE_H */
