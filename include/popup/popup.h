#ifndef POPUP_H
#define POPUP_H

#include "enums.h"
#include "nmath.h"
#include "structs.h"
#include "n9patch.h"

typedef struct PopUp PopUp;

typedef void (* popup_draw_function_t)(struct PopUp *p, struct Point pos,
                                       SDL_Texture *rt, SDL_Renderer *r);

struct PopUp {
    struct n9Patch        n9patch;
    struct Point          target_pos;
    popup_draw_function_t draw;

    void    *data;
    float   slidefactor;
    i32    update_wait_ns;
    i32    timer_ns;
    i8     type;
    u8     slidetype;
    bool    visible;
};

extern struct PopUp PopUp_default;

#endif /* POPUP_H */
