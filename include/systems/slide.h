#ifndef SLIDE_SYSTEM_H
#define SLIDE_SYSTEM_H

#include "tnecs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Sprite;
struct Slider;
struct Position;

void Sprite_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);
void Cursor_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);

void Slide_PopUp(           tnecs_In *input);
void Slide_Sprite(          tnecs_In *input);
void Slide_Actor(           tnecs_In *input);
void Slide_Update_FPS(      tnecs_In *input);
void Slide_PopUp_Offscreen( tnecs_In *input);

void Hover_Any(tnecs_In *input);

#endif /* SLIDE_SYSTEM_H */
