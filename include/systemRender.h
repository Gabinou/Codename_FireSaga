#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_platform.h"
#include "narrative.h"
#include "map.h"
#include "sprite.h"
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

extern void drawText(tnecs_system_input_t * in_input);
extern void drawSprite(tnecs_system_input_t * in_input);
extern void drawCursor(tnecs_system_input_t * in_input);
extern void drawMouse(tnecs_system_input_t * in_input);
extern void drawMenu(tnecs_system_input_t * in_input);

#endif /* SYSTEMRENDER_H */
