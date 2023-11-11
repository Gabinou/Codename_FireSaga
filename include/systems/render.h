#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "tnecs.h"
#include "sprite.h"
#include "game/game.h"
#include "position.h"
#include "slider.h"
#include "popup/tile.h"
#include "SDL.h"

// TODO: Check if that causes issue with Interactive Programming
extern bool CursorFlag;
extern bool MouseFlag;
extern bool RenderTop;

void drawText(     tnecs_system_input_t *in_input);
void drawMouse(    tnecs_system_input_t *in_input);
void moveMouse(    tnecs_system_input_t *in_input);
void drawMenu(     tnecs_system_input_t *in_input);
void drawPopUp(    tnecs_system_input_t *in_input);
void drawCursor(   tnecs_system_input_t *in_input);
void scrollText(   tnecs_system_input_t *in_input);
void drawMapUnit(  tnecs_system_input_t *in_input);
void drawTextTimer(tnecs_system_input_t *in_input);
void animateSprite(tnecs_system_input_t *in_input);
void drawMap_HPBar(tnecs_system_input_t *in_input);


void Animate_Combat_onMap(tnecs_system_input_t *in_input);


#endif /* SYSTEMRENDER_H */
