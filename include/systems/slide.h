#ifndef SLIDE_SYSTEM_H
#define SLIDE_SYSTEM_H

#include "tnecs.h"
#include "sprite.h"
#include "slider.h"
#include "sprite.h"
#include "hover.h"
#include "game/game.h"
#include "position.h"
#include "popup/tile.h"
#include "popup/unit.h"
#include "popup/popup.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Sprite;

void Sprite_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);
void Cursor_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);

void Slide_PopUp(         tnecs_system_input *input);
void Slide_Sprite(        tnecs_system_input *input);
void Slide_PopUp_Offscreen(tnecs_system_input *input);

void Hover_Any(tnecs_system_input *input);

#endif /* SLIDE_SYSTEM_H */
