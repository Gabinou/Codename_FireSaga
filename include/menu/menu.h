#ifndef MENU_H
#define MENU_H

#include "enums.h"
#include "globals.h"
#include "n9patch.h"
#include "game/game.h"
#include "game/menu.h"
#include "game/popup.h"
#include "game/cursor.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- ENUMS --- */
enum MENU_bOFFSET {
    MENU_UPDATE_bOFFSET = 0,
};

enum MENU_ELEM {
    MENU_ELEM_NULL = -1,
};

enum MENU_ELEM_POS_FRAME {
    ELEM_POS_MENU_FRAME     = 0,
    ELEM_POS_SCREEN_FRAME   = 1,
};

#define MENU_ELEM_DIRECTIONS_PACKED_BYTESIZE 4

/* --- TYPEDEFS --- */
typedef struct MenuComponent MenuComponent;
typedef void (* menu_draw_f)(  MenuComponent *mc, SDL_Texture *rt, SDL_Renderer *r);
typedef void (* menu_set_update_f)(MenuComponent *mc);

struct MenuComponent {
    struct n9Patch n9patch;
    /* -- Cursor position when on elem -- */
    struct Point *cursor_pos;               /* [elem] absolute pos on texture */

    /* -- Top left corner of elem box -- */
    struct Point *elem_pos;                 /* [elem] absolute pos on texture */
    int elem_pos_frame;                     /* elem_pos reference frame */

    /* -- Width/height of elem box -- */
    struct Point *elem_box;                 /* [elem] width, height of box in [pixel] */
    char **elem_description;                /* [elem] */
    struct MenuElemDirections *elem_links;  /* [elem] */
    void *data;

    /* -- Generic Draw function -- */
    menu_draw_f         draw;
    menu_set_update_f   set_update;

    i8 elem;     /* -1 is NULL */
    i8 elem_num;
    i8 type;

    bool visible : 1;
};
extern struct MenuComponent MenuComponent_default;

/* --- Constructors/Destructors --- */
void MenuComponent_Elem_Free(struct MenuComponent *mc);

/* --- Internals --- */
int Periodic_Elem_Move(     struct MenuComponent *mc, int d, int min, int max);
void MenuComponent_Elem_Set(struct MenuComponent *mc, struct Game *sota, i8 d);
int MenuComponent_Elem_Move(struct MenuComponent *mc, int d);
void MenuComponent_Elem_Boxes_Check(struct MenuComponent *mc);

/* --- Debug --- */
void MenuComponent_Elem_Boxes_Draw(struct MenuComponent *mc, struct SDL_Renderer *r);

#endif /* MENU_H */
