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
enum MENU_ELEM {
    MENU_ELEM_NULL = -1,
};

#define MENU_ELEM_DIRECTIONS_PACKED_BYTESIZE 4

/* --- TYPEDEFS --- */
typedef struct MenuComponent MenuComponent;
typedef void (* menu_draw_function_t)(MenuComponent *mc, SDL_Texture *rt, SDL_Renderer *r);

struct MenuComponent {
    struct n9Patch n9patch;
    /* -- Cursor position when on elem -- */
    struct Point *cursor_pos;               /* [elem] absolute pos on texture */

    /* -- Top left corner of elem box -- */
    struct Point *elem_pos;                 /* [elem] absolute pos on texture */

    /* -- Width/height of elem box -- */
    struct Point *elem_box;                 /* [elem] width, height of box in [pixel] */
    char **elem_description;                /* [elem] */
    struct MenuElemDirections *elem_links;  /* [elem] */
    void *data;

    /* -- Generic Draw function -- */
    menu_draw_function_t draw;

    if8 elem;     /* -1 is NULL */
    if8 elem_num;
    if8 type;

    bool visible : 1;
};
extern struct MenuComponent MenuComponent_default;

/* --- Constructors/Destructors --- */
extern void MenuComponent_Elem_Free( struct MenuComponent *mc);
extern void MenuComponent_Elem_Alloc(struct MenuComponent *mc, uf8 en);

/* --- Internals --- */
extern int Periodic_Elem_Move(             struct MenuComponent *mc, int d, int min, int max);
extern void MenuComponent_Elem_Set(        struct MenuComponent *mc, struct Game *sota, if8 d);
extern int MenuComponent_Elem_Move(        struct MenuComponent *mc, int d);
extern void MenuComponent_Elem_Boxes_Check(struct MenuComponent *mc);

/* --- Debug --- */
extern void MenuComponent_Elem_Boxes_Draw(struct MenuComponent *mc, struct SDL_Renderer *r);

#endif /* MENU_H */
