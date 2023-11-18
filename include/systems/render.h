#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "tnecs.h"
#include "sprite.h"
#include "structs.h"
#include "game/game.h"
#include "position.h"
#include "slider.h"
#include "popup/tile.h"
#include "map/animation.h"
#include "SDL.h"

extern b32 CursorFlag;
extern b32 MouseFlag;
extern b32 RenderTop;

void Draw_Text(      tnecs_system_input *input);
void Draw_Menu(      tnecs_system_input *input);
void Draw_PopUp(     tnecs_system_input *input);
void Draw_Mouse(     tnecs_system_input *input);
void Draw_Cursor(    tnecs_system_input *input);
void Scroll_Text(    tnecs_system_input *input);
void Draw_Map_Unit(  tnecs_system_input *input);
void Animate_Sprite( tnecs_system_input *input);
void Draw_Map_HPBar( tnecs_system_input *input);
void Draw_Text_Timer(tnecs_system_input *input);

void Animate_Combat_onMap(   tnecs_system_input *input);
void Animate_Turn_Transition(tnecs_system_input *input);

#endif /* SYSTEMRENDER_H */
