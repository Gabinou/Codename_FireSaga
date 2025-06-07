#ifndef POPUP_H
#define POPUP_H

#include "enums.h"
#include "structs.h"
#include "n9patch.h"

/* ---  Popup --- */
// Show Player useful information

/* -- Design objectives -- */
//  - Show information to player
//  - Whatever is necessary to make choice of next action
//  - Show meta - information to the player *** ? * **
//  - e.g. How does the STR stat work *** ? * **
//  - Player cannot interact directly with a Popup

/* --- STRUCT DEF --- */
typedef struct PopUp PopUp;

typedef void (* popup_draw_f)(struct PopUp *p, struct Point pos,
                              SDL_Texture *rt, SDL_Renderer *r);

// Abbreviation struct Popup *pc -> popup component
struct PopUp {
    struct n9Patch  n9patch;
    struct Point    target_pos;
    popup_draw_f    draw;

    void           *data;
    float           slidefactor;
    i32             update_wait_ns;
    i32             timer_ns;
    i8              type;
    u8              slidetype;
    b32             visible;
};
extern const struct PopUp PopUp_default;

void PopUp_Init_tnecs(void *voidpopup);
void PopUp_Init(struct PopUp *popup);

#endif /* POPUP_H */