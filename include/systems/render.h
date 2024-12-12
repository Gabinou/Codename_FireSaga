#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

extern b32 CursorFlag;
extern b32 MouseFlag;
extern b32 RenderTop;

void Draw_Text(         tnecs_system_input *input);
void Draw_Menu(         tnecs_system_input *input);
void Draw_PopUp(        tnecs_system_input *input);
void Draw_Mouse(        tnecs_system_input *input);
void Draw_Cursor(       tnecs_system_input *input);
void Scroll_Text(       tnecs_system_input *input);
void Draw_Map_Unit(     tnecs_system_input *input);
void Animate_Sprite(    tnecs_system_input *input);
void Draw_Map_HPBar(    tnecs_system_input *input);
void Draw_Text_Timer(   tnecs_system_input *input);
void Draw_Map_Boss_Icon(tnecs_system_input *input);

void Animate_Scene(             tnecs_system_input *input);
void Animate_Cutscene(          tnecs_system_input *input);
void Animate_Game_Over(         tnecs_system_input *input);
void Animate_Combat_onMap(      tnecs_system_input *input);
void Animate_Map_Animation(     tnecs_system_input *input);
void Animate_Unit_Move_onMap(   tnecs_system_input *input);

#endif /* SYSTEMRENDER_H */
