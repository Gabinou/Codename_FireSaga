#ifndef CONTROLLERTOUCHPAD_H
#define CONTROLLERTOUCHPAD_H

#include <math.h>
#include "flecs.h"
#include "SDL2/SDL.h"

typedef struct controllerTouchpad {
    SDL_TouchID touchID;
} controllerTouchpad;
extern struct controllerTouchpad controllerTouchpad_default;


typedef struct controllerTouchpadModule {
    ECS_DECLARE_COMPONENT(controllerTouchpad);
} controllerTouchpadModule;

void controllerTouchpadModuleImport(ecs_world_t * in_world);
#define controllerTouchpadModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, controllerTouchpad);


#endif /* CONTROLLERTOUCHPAD_H */
