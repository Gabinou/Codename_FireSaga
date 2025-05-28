#ifndef MENU_H
#define MENU_H

#include "enums.h"
#include "n9patch.h"

/* --- Menu --- */
// Main point of interaction between Player and game

/* -- Design objectives -- */
// - Show selection of actions Player can take
// - Prompt for next action
// - Player must make the next choice
// - Show information to player
// - Whatever is necessary to make choice of next action
// - Show meta - information to the player
// - e.g. How does the STR stat work

/* -- Implementation details -- */
// - Arbitrary of elements (***elem***)
//     , boxes on menu
//     - Cursor can move between each ***elem*** using 4 directions
//     - Each ***elem*** linked to 4 next ***elem*** using array of directions
//     - Try to make each ***elem*** box fit snuggly in menu space
//     - All ***elem*** + attributes are independent from menu appearance
//     - Position, size, directions, links...
//     - Press button with cursor on ***elem*** to perform action
//     - Behavior taken charge by Game State FSM -> Menu FSM

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- ENUMS --- */
enum MENU_ELEM {
    MENU_ELEM_NULL = -1,
};

enum MENU_ELEM_POS_FRAME {
    ELEM_POS_MENU_FRAME     = 0,
    ELEM_POS_SCREEN_FRAME   = 1,
};

#define MENU_ELEM_DIRECTIONS_PACKED_BYTESIZE 4

/* --- TYPEDEFS --- */
typedef struct Menu Menu;
typedef void (* menu_draw_f)(  Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
typedef void (* menu_set_update_f)(Menu *mc);

// Abbreviation struct Menu *mc -> menu component
struct Menu {
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
    menu_draw_f         draw;
    menu_set_update_f   set_update;

    i32 elem;     /* -1 is NULL */
    i32 elem_num;
    i32 elem_pos_frame;                     /* elem_pos reference frame */
    i32 type;

    b32 visible;
};
extern const struct Menu Menu_default;

/* --- Constructors/Destructors --- */
void Menu_Elem_Free(struct Menu *mc);

/* --- i32ernals --- */
i32  Periodic_Elem_Move(struct Menu *mc, i32 d, i32 min, i32 max);

i32  Menu_Elem(             struct Menu *menu);
i32  Menu_Elem_Num(         struct Menu *menu);
void Menu_Elem_Set(         struct Menu *mc, struct Game *sota, i8 d);
i32  Menu_Elem_Move(        struct Menu *mc, i32 d);
void Menu_Elem_Boxes_Check( struct Menu *mc);
i32  Menu_Elem_Move(        struct Menu *mc, i32 d);

/* --- Debug --- */
void Menu_Elem_Boxes_Draw(struct Menu *mc, struct SDL_Renderer *r);

#endif /* MENU_H */
