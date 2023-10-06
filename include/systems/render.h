#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "tnecs.h"
#include "sprite.h"
#include "game/game.h"
#include "position.h"
#include "slider.h"
#include "popup/tile.h"
#include "SDL2/SDL.h"

extern bool CursorFlag;
extern bool MouseFlag;
extern bool RenderTop;

extern void drawText(     tnecs_system_input_t *in_input);
extern void drawMouse(    tnecs_system_input_t *in_input);
extern void moveMouse(    tnecs_system_input_t *in_input);
extern void drawMenu(     tnecs_system_input_t *in_input);
extern void drawPopUp(    tnecs_system_input_t *in_input);
extern void drawCursor(   tnecs_system_input_t *in_input);
extern void scrollText(   tnecs_system_input_t *in_input);
extern void drawMapUnit(  tnecs_system_input_t *in_input);
extern void drawTextTimer(tnecs_system_input_t *in_input);
extern void animateSprite(tnecs_system_input_t *in_input);
extern void drawMap_HPBar(tnecs_system_input_t *in_input);

#endif /* SYSTEMRENDER_H */
