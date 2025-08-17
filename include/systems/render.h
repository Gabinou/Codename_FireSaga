
#ifndef SYSTEM_RENDER_H
#define SYSTEM_RENDER_H

#include "tnecs.h"

// TODO: rn. prefix system?
void Draw_Map(          tnecs_In *input);
void Draw_Text(         tnecs_In *input);
void Draw_Menu(         tnecs_In *input);
void Draw_Actor(        tnecs_In *input);
void Draw_PopUp(        tnecs_In *input);
void Draw_Actor(        tnecs_In *input);
void Draw_Scene(        tnecs_In *input);
void Draw_Sprite(       tnecs_In *input);
void Scroll_Text(       tnecs_In *input);
void Animate_Sprite(    tnecs_In *input);
void Draw_Map_HPBar(    tnecs_In *input);
void Draw_Text_Timer(   tnecs_In *input);
void Draw_Map_Boss_Icon(tnecs_In *input);

void Animate_Scene(             tnecs_In *input);
void Animate_Cutscene(          tnecs_In *input);
void Animate_Game_Over(         tnecs_In *input);
void Animate_Combat_onMap(      tnecs_In *input);
void Animate_Map_Animation(     tnecs_In *input);
void Animate_Unit_Move_onMap(   tnecs_In *input);

#endif /* SYSTEM_RENDER_H */
