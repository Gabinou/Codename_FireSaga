#ifndef TRADE_MENU_H
#define TRADE_MENU_H

#include "tnecs.h"
#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;

enum TRADE_MENU {
    TM_PATCH_X_PIXELS = 8,
    TM_PATCH_Y_PIXELS = 8,
};

enum TRADE_MENU_ELEMS {
    TM_ELEM_NULL    = -1,
    TM_ELEM_ITEM1_A =  0,
    TM_ELEM_ITEM2_A =  1,
    TM_ELEM_ITEM3_A =  2,
    TM_ELEM_ITEM4_A =  3,
    TM_ELEM_ITEM5_A =  4,
    TM_ELEM_ITEM6_A =  5,
    TM_ELEM_ITEM1_P =  6,
    TM_ELEM_ITEM2_P =  7,
    TM_ELEM_ITEM3_P =  8,
    TM_ELEM_ITEM4_P =  9,
    TM_ELEM_ITEM5_P = 10,
    TM_ELEM_ITEM6_P = 11,
    TRADE_MENU_ELEMS_NUM,
};
extern struct MenuElemDirections trade_menu_links[TRADE_MENU_ELEMS_NUM];
extern struct Point trade_menu_elem_pos[TRADE_MENU_ELEMS_NUM];
extern struct Point trade_menu_elem_box[TRADE_MENU_ELEMS_NUM];
extern struct Point trade_cursor_pos[TRADE_MENU_ELEMS_NUM];
extern struct Point trade_cursor_box[TRADE_MENU_ELEMS_NUM];

struct TradeMenu {
    struct LoadoutSelectMenu *active;
    struct LoadoutSelectMenu *passive;
    i32 selected_trader;
    i32 selected_item;
    i32 target_trader;
    i32 target_item;
    tnecs_world *world;
};

/* --- Constructors/Destructors --- */
struct TradeMenu *TradeMenu_Alloc(void);
void TradeMenu_Free(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);

/* --- Item placement --- */
void TradeMenu_Trade(   struct TradeMenu *tm);
void TradeMenu_Select(  struct TradeMenu *tm, i8 s);
void TradeMenu_Deselect(struct TradeMenu *tm);

/* --- Positioning --- */
void TradeMenu_Elem_Pos(struct TradeMenu *tm, struct Menu *mc);

/* --- Drawing --- */
void TradeMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);

#endif /* TRADE_MENU_H */
