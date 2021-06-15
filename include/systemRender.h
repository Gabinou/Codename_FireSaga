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
#include "tnecs.h"
#include "position.h"
#include "controllerKeyboard.h"
#include "controllerGamepad.h"
#include "controllerMouse.h"
#include "controllerTouchpad.h"

// SDL_Renderer * Game_renderer;

extern void tnecs_drawText(tnecs_system_input_t * in_input);
extern void tnecs_drawSprite(tnecs_system_input_t * in_input);
extern void tnecs_drawCursor(tnecs_system_input_t * in_input);
extern void tnecs_drawMouse(tnecs_system_input_t * in_input);
extern void tnecs_drawMenu(tnecs_system_input_t * in_input);

#endif /* SYSTEMRENDER_H */
