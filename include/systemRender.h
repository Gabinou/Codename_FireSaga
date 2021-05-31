#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_platform.h"
#include "narrative.h"
#include "map.h"
#include "sprite.h"
#include "flecs.h"
#include "text.h"
#include "map.h"
#include "game.h"
#include "position.h"
#include "controllerKeyboard.h"
#include "controllerGamepad.h"
#include "controllerMouse.h"
#include "controllerTouchpad.h"

// SDL_Renderer * Game_renderer;

typedef struct systemRenderModule {
    ECS_DECLARE_ENTITY(drawText);
    ECS_DECLARE_ENTITY(drawSprite);
    ECS_DECLARE_ENTITY(drawCursor);
    ECS_DECLARE_ENTITY(drawMouse);
    ECS_DECLARE_ENTITY(drawMenu);
} systemRenderModule;

void systemRenderModuleImport(ecs_world_t * in_world);
#define systemRenderModuleImportHandles(handles)\
    ECS_IMPORT_ENTITY(handles, drawText);\
    ECS_IMPORT_ENTITY(handles, drawSprite);\
ECS_IMPORT_ENTITY(handles, drawCursor);\
ECS_IMPORT_ENTITY(handles, drawMenu);\
ECS_IMPORT_ENTITY(handles, drawMouse);

#endif /* SYSTEMRENDER_H */
