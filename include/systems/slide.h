#ifndef SLIDE_SYSTEM_H
#define SLIDE_SYSTEM_H

#include "tnecs.h"
#include "sprite.h"
#include "slider.h"
#include "hover.h"
#include "game/game.h"
#include "position.h"
#include "popup/tile.h"
#include "popup/unit.h"
#include "popup/popup.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

void Sprite_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);
void Cursor_Target(struct Slider *sl, struct Sprite *sp, struct Position *pos);

extern void slidePopUp(         tnecs_system_input_t *input);
extern void slideSprite(        tnecs_system_input_t *input);
extern void slidePopUpOffscreen(tnecs_system_input_t *input);

extern void hoverAny(tnecs_system_input_t *input);

#endif /* SLIDE_SYSTEM_H */
