
#include "menu/trade.h"
#include "menu/menu.h"
#include "menu/trade.h"
#include "menu/loadout_select.h"
#include "unit/unit.h"
#include "unit/equipment.h"
#include "utilities.h"

const struct Point trade_menu_cursor_pos[TRADE_MENU_ELEMS_NUM] = {
    /* TM_ELEM_ITEM1_A */   {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_A */   {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_A */   {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_A */   {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_A */   {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_A */   {LSM6_X_OFFSET, LSM6_Y_OFFSET},
    /* TM_ELEM_ITEM1_P */   {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_P */   {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_P */   {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_P */   {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_P */   {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_P */   {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

const struct Point trade_menu_elem_box[TRADE_MENU_ELEMS_NUM] = {
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

const struct Point trade_menu_elem_pos[TRADE_MENU_ELEMS_NUM] = {
    /* TM_ELEM_ITEM1_A */   {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_A */   {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_A */   {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_A */   {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_A */   {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_A */   {LSM6_X_OFFSET, LSM6_Y_OFFSET},
    /* TM_ELEM_ITEM1_P */   {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* TM_ELEM_ITEM2_P */   {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* TM_ELEM_ITEM3_P */   {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* TM_ELEM_ITEM4_P */   {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* TM_ELEM_ITEM5_P */   {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* TM_ELEM_ITEM6_P */   {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

const struct MenuElemDirections trade_menu_links[TRADE_MENU_ELEMS_NUM] = {
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

struct TradeMenu *TradeMenu_Alloc(void) {
    struct TradeMenu *tm = SDL_malloc(sizeof(struct TradeMenu));
    tm->selected_item   = ITEM_NULL;
    tm->selected_trader = TRADER_NULL;
    return (tm);
}

void TradeMenu_Deselect(struct TradeMenu *tm) {
    tm->selected_item   = ITEM_NULL;
    tm->selected_trader = TRADER_NULL;
}

void TradeMenu_Select(struct   TradeMenu *tm, i8 selected) {
    // TODO: FIX THIS
    SDL_assert(false);
    i8 trader  = 1 - selected / SOTA_EQUIPMENT_SIZE;
    i8 item    =     selected % SOTA_EQUIPMENT_SIZE;
    SDL_assert((trader == TRADER_PASSIVE) || (trader == TRADER_ACTIVE));
    SDL_assert((item    > ITEM_NULL) && (item    < SOTA_EQUIPMENT_SIZE));

    /* If item is selected, swap it item */
    if ((tm->selected_item == ITEM_NULL) || (tm->selected_trader == TRADER_NULL)) {
        tm->selected_item   = item;
        tm->selected_trader = trader;
        return;
    }

    /* If item swapped item is the same, deselect it */
    if ((item == tm->selected_item) && (trader == tm->selected_trader)) {
        TradeMenu_Deselect(tm);
        return;
    }

    tm->target_trader = trader;
    tm->target_item =   item;

    /* If another item was selected before, swap them */
    TradeMenu_Trade(tm);
    TradeMenu_Deselect(tm);
}

void TradeMenu_Trade(struct TradeMenu *tm) {
    SDL_assert(tm           != NULL);
    SDL_assert(tm->world    != NULL);

    tnecs_entity giver_ent = tm->selected_trader ? tm->active->unit : tm->passive->unit;
    tnecs_entity taker_ent = tm->target_trader   ? tm->active->unit : tm->passive->unit;
    struct Unit *giver = TNECS_GET_COMPONENT(tm->world, giver_ent, Unit);
    struct Unit *taker = TNECS_GET_COMPONENT(tm->world, taker_ent, Unit);

    /* If item in hand, de-equip */
    if ((tm->selected_item == Unit_Hand_Strong(giver))
        || (tm->selected_item == Unit_Hand_Weak(giver))) {
        Unit_Unequip(giver, tm->selected_item);
    }
    if ((tm->target_item == Unit_Hand_Strong(taker)) || (tm->target_item == Unit_Hand_Weak(taker))) {
        Unit_Unequip(taker, tm->target_item);
    }

    Unit_Item_Trade(giver, taker, tm->selected_item, tm->target_item);

    /* Re-equip whatever is in hands. */
    // Do that, cause otherwise can't instantly attack/staff,
    // would have to go reequip manually before, which sucks.
    if ((tm->selected_item == Unit_Hand_Strong(giver))
        || (tm->selected_item == Unit_Hand_Weak(giver))) {
        Unit_Equip(giver, tm->selected_item, tm->selected_item);
    }
    if ((tm->target_item == Unit_Hand_Strong(taker)) || (tm->target_item == Unit_Hand_Weak(taker))) {
        Unit_Equip(taker, tm->target_item, tm->target_item);
    }

    /* Update trade menu */
    tm->active->update  = true;
    tm->passive->update = true;

}

void TradeMenu_Draw(struct Menu *mc, SDL_Texture *target, SDL_Renderer *renderer) {
    struct TradeMenu *tm = (struct TradeMenu *)mc->data;
    struct LoadoutSelectMenu *active =     tm->active;
    struct LoadoutSelectMenu *passive =    tm->passive;
    struct n9Patch *n9patch =           &mc->n9patch; // shared n9patch ?

    SDL_assert(active != NULL);
    if (active->update) {
        LoadoutSelectMenu_Update(mc, active, n9patch, target, renderer);
        active->update = false;
        passive->pos.x = active->pos.x + n9patch->size_pixels.x * n9patch->scale.x;
        TradeMenu_Elem_Pos(tm, mc);
    }
    SDL_assert(active->texture != NULL);
    SDL_assert(passive != NULL);
    if (passive->update) {
        LoadoutSelectMenu_Update(mc, passive, n9patch, target, renderer);
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
}

void TradeMenu_Elem_Pos(struct TradeMenu *tm, struct Menu *mc) {
    // Scales elem_pos to menu size
    SDL_assert(tm != NULL);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.scale.y == LSM_N9PATCH_SCALE_Y);
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
}
