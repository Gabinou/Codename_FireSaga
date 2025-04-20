#ifndef SYSTEMRENDER_H
#define SYSTEMRENDER_H

#include "tnecs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

void Draw_Text(         tnecs_input *input);
void Draw_Menu(         tnecs_input *input);
void Draw_PopUp(        tnecs_input *input);
void Draw_Actor(        tnecs_input *input);
void Draw_Sprite(       tnecs_input *input);
void Scroll_Text(       tnecs_input *input);
void Animate_Sprite(    tnecs_input *input);
void Draw_Map_HPBar(    tnecs_input *input);
void Draw_Text_Timer(   tnecs_input *input);
void Draw_Map_Boss_Icon(tnecs_input *input);

void Animate_Scene(             tnecs_input *input);
void Animate_Cutscene(          tnecs_input *input);
void Animate_Game_Over(         tnecs_input *input);
void Animate_Combat_onMap(      tnecs_input *input);
void Animate_Map_Animation(     tnecs_input *input);
void Animate_Unit_Move_onMap(   tnecs_input *input);

#endif /* SYSTEMRENDER_H */
