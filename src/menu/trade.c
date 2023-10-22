
#include "menu/trade.h"

struct Point trade_menu_cursor_pos[TRADE_MENU_ELEMS_NUM] = {
    /* TM_ELEM_ITEM1_A */   {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_A */   {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_A */   {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_A */   {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_A */   {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_A */   {WSM6_X_OFFSET, WSM6_Y_OFFSET},
    /* TM_ELEM_ITEM1_P */   {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_P */   {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_P */   {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_P */   {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_P */   {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_P */   {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct Point trade_menu_elem_box[TRADE_MENU_ELEMS_NUM] = {
    /* TM_ELEM_ITEM1_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM2_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM3_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM4_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM5_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM6_A */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM1_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM2_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM3_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM4_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM5_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
    /* TM_ELEM_ITEM6_P */   {SOTA_TILESIZE,  SOTA_TILESIZE},
};

struct Point trade_menu_elem_pos[TRADE_MENU_ELEMS_NUM] = {
    /* TM_ELEM_ITEM1_A */   {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_A */   {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_A */   {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_A */   {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_A */   {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_A */   {WSM6_X_OFFSET, WSM6_Y_OFFSET},
    /* TM_ELEM_ITEM1_P */   {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_P */   {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_P */   {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_P */   {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_P */   {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_P */   {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct MenuElemDirections trade_menu_links[TRADE_MENU_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* TM_ELEM_ITEM1_A */ {TM_ELEM_ITEM1_P, TM_ELEM_ITEM6_A, TM_ELEM_ITEM1_P, TM_ELEM_ITEM2_A},
    /* TM_ELEM_ITEM2_A */ {TM_ELEM_ITEM2_P, TM_ELEM_ITEM1_A, TM_ELEM_ITEM2_P, TM_ELEM_ITEM3_A},
    /* TM_ELEM_ITEM3_A */ {TM_ELEM_ITEM3_P, TM_ELEM_ITEM2_A, TM_ELEM_ITEM3_P, TM_ELEM_ITEM4_A},
    /* TM_ELEM_ITEM4_A */ {TM_ELEM_ITEM4_P, TM_ELEM_ITEM3_A, TM_ELEM_ITEM4_P, TM_ELEM_ITEM5_A},
    /* TM_ELEM_ITEM5_A */ {TM_ELEM_ITEM5_P, TM_ELEM_ITEM4_A, TM_ELEM_ITEM5_P, TM_ELEM_ITEM6_A},
    /* TM_ELEM_ITEM6_A */ {TM_ELEM_ITEM6_P, TM_ELEM_ITEM5_A, TM_ELEM_ITEM6_P, TM_ELEM_ITEM1_A},
    /* TM_ELEM_ITEM1_P */ {TM_ELEM_ITEM1_A, TM_ELEM_ITEM6_P, TM_ELEM_ITEM1_A, TM_ELEM_ITEM2_P},
    /* TM_ELEM_ITEM2_P */ {TM_ELEM_ITEM2_A, TM_ELEM_ITEM1_P, TM_ELEM_ITEM2_A, TM_ELEM_ITEM3_P},
    /* TM_ELEM_ITEM3_P */ {TM_ELEM_ITEM3_A, TM_ELEM_ITEM2_P, TM_ELEM_ITEM3_A, TM_ELEM_ITEM4_P},
    /* TM_ELEM_ITEM4_P */ {TM_ELEM_ITEM4_A, TM_ELEM_ITEM3_P, TM_ELEM_ITEM4_A, TM_ELEM_ITEM5_P},
    /* TM_ELEM_ITEM5_P */ {TM_ELEM_ITEM5_A, TM_ELEM_ITEM4_P, TM_ELEM_ITEM5_A, TM_ELEM_ITEM6_P},
    /* TM_ELEM_ITEM6_P */ {TM_ELEM_ITEM6_A, TM_ELEM_ITEM5_P, TM_ELEM_ITEM6_A, TM_ELEM_ITEM1_P},
};

struct TradeMenu *TradeMenu_Alloc() {
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct TradeMenu *tm = malloc(sizeof(struct TradeMenu));
    tm->selected_item   = ITEM_NULL;
    tm->selected_trader = TRADER_NULL;
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (tm);
}

void TradeMenu_Deselect(struct TradeMenu *tm) {
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tm->selected_item   = ITEM_NULL;
    tm->selected_trader = TRADER_NULL;
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TradeMenu_Select(struct   TradeMenu *tm, i8 selected) {
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i8 trader  = 1 - selected / DEFAULT_EQUIPMENT_SIZE;
    i8 item    =     selected % DEFAULT_EQUIPMENT_SIZE;
    SDL_assert((trader == TRADER_PASSIVE) || (trader == TRADER_ACTIVE));
    SDL_assert((item    > ITEM_NULL) && (item    < DEFAULT_EQUIPMENT_SIZE));

    /* If item is selected, swap it item */
    if ((tm->selected_item == ITEM_NULL) || (tm->selected_trader == TRADER_NULL)) {
        tm->selected_item   = item;
        tm->selected_trader = trader;
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    /* If item swapped item is the same, deselect it */
    if ((item == tm->selected_item) && (trader == tm->selected_trader)) {
        TradeMenu_Deselect(tm);
        SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    tm->target_trader = trader;
    tm->target_item =   item;

    /* If another item was selected before, swap them */
    TradeMenu_Trade(tm);
    TradeMenu_Deselect(tm);

    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TradeMenu_Trade(struct TradeMenu *tm) {
    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    struct Unit *giver = tm->selected_trader ? tm->active->unit : tm->passive->unit;
    struct Unit *taker = tm->target_trader   ? tm->active->unit : tm->passive->unit;

    /* If item in hand, de-equip */
    if ((tm->selected_item == UNIT_HAND_STRONG) || (tm->selected_item == UNIT_HAND_WEAK)) {
        Unit_Unequip(giver, tm->selected_item);
    }
    if ((tm->target_item == UNIT_HAND_STRONG) || (tm->target_item == UNIT_HAND_WEAK)) {
        Unit_Unequip(taker, tm->target_item);
    }

    Unit_Item_Trade(giver, taker, tm->selected_item, tm->target_item);

    /* Re-equip whatever is in hands. */
    // Do that, cause otherwise can't instantly attack/staff,
    // would have to go reequip manually before, which sucks.
    if ((tm->selected_item == UNIT_HAND_STRONG) || (tm->selected_item == UNIT_HAND_WEAK)) {
        Unit_Equip_inHand(giver, tm->selected_item);
    }
    if ((tm->target_item == UNIT_HAND_STRONG) || (tm->target_item == UNIT_HAND_WEAK)) {
        Unit_Equip_inHand(taker, tm->target_item);
    }

    /* Update trade menu */
    tm->active->update  = true;
    tm->passive->update = true;

    SOTA_Log_Debug("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TradeMenu_Draw(struct MenuComponent *mc, SDL_Texture *target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct TradeMenu *tm = (struct TradeMenu *)mc->data;
    struct LoadoutSelectMenu *active =     tm->active;
    struct LoadoutSelectMenu *passive =    tm->passive;
    struct n9Patch *n9patch =           &mc->n9patch; // shared n9patch ?

    SDL_assert(active != NULL);
    if (active->update) {
        LoadoutSelectMenu_Update(active, n9patch, target, renderer);
        active->update = false;
        passive->pos.x = active->pos.x + n9patch->size_pixels.x * n9patch->scale.x;
        TradeMenu_Elem_Pos(tm, mc);
    }
    SDL_assert(active->texture != NULL);
    SDL_assert(passive != NULL);
    if (passive->update) {
        LoadoutSelectMenu_Update(passive, n9patch, target, renderer);
        passive->update = false;
    }
    SDL_assert(passive->texture != NULL);

    // TODO: add two tm sizes
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = active->pos.x,
        .y = active->pos.y,
    };
    SDL_RenderCopy(renderer, active->texture, NULL, &dstrect);

    dstrect.w = n9patch->size_pixels.x * n9patch->scale.x,
    dstrect.h = n9patch->size_pixels.y * n9patch->scale.y,
    dstrect.x = passive->pos.x;
    dstrect.y = passive->pos.y;

    SDL_RenderCopy(renderer, passive->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TradeMenu_Elem_Pos(struct TradeMenu *tm, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Scales elem_pos to menu size
    SDL_assert(tm != NULL);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.scale.y == WSM_N9PATCH_SCALE_Y);
    for (size_t i = 0; i < mc->elem_num; i++) {
        if (i < TM_ELEM_ITEM1_P) {
            mc->elem_pos[i].x = tm->active->pos.x + mc->n9patch.pos.x + trade_menu_elem_pos[i].x *
                                mc->n9patch.scale.x;
            mc->elem_pos[i].y = tm->active->pos.y + trade_menu_elem_pos[i].y * mc->n9patch.scale.y;
        } else {
            mc->elem_pos[i].x = tm->passive->pos.x + mc->n9patch.pos.x + trade_menu_elem_pos[i].x *
                                mc->n9patch.scale.x;
            mc->elem_pos[i].y = tm->passive->pos.y + trade_menu_elem_pos[i].y * mc->n9patch.scale.y;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
