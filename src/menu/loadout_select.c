#include "menu/loadout_select.h"

struct Point wsm_cursor_pos[WSM_ELEMS_NUM] = {
    /* WSM_ELEM_ITEM2 */ {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* WSM_ELEM_ITEM2 */ {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* WSM_ELEM_ITEM3 */ {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* WSM_ELEM_ITEM4 */ {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* WSM_ELEM_ITEM5 */ {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* WSM_ELEM_ITEM6 */ {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct Point wsm_elem_box[WSM_ELEMS_NUM] = {
    /* WSM_ELEM_ITEM1 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* WSM_ELEM_ITEM2 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* WSM_ELEM_ITEM3 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* WSM_ELEM_ITEM4 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* WSM_ELEM_ITEM5 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* WSM_ELEM_ITEM6 */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct Point  wsm_elem_pos[WSM_ELEMS_NUM] = {
    /* WSM_ELEM_ITEM1 */ {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* WSM_ELEM_ITEM2 */ {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* WSM_ELEM_ITEM3 */ {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* WSM_ELEM_ITEM4 */ {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* WSM_ELEM_ITEM5 */ {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* WSM_ELEM_ITEM6 */ {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct MenuElemDirections wsm_links[WSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* WSM_ELEM_ITEM1 */ {WSM_ELEM_NULL, WSM_ELEM_NULL,  WSM_ELEM_NULL, WSM_ELEM_ITEM2},
    /* WSM_ELEM_ITEM2 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM1, WSM_ELEM_NULL, WSM_ELEM_ITEM3},
    /* WSM_ELEM_ITEM3 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM2, WSM_ELEM_NULL, WSM_ELEM_ITEM4},
    /* WSM_ELEM_ITEM4 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM3, WSM_ELEM_NULL, WSM_ELEM_ITEM5},
    /* WSM_ELEM_ITEM5 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM4, WSM_ELEM_NULL, WSM_ELEM_ITEM6},
    /* WSM_ELEM_ITEM6 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM5, WSM_ELEM_NULL, WSM_ELEM_NULL},
};

struct LoadoutSelectMenu LoadoutSelectMenu_default = {
    .pos                    = {-1, -1},
    .texture                = NULL,
    .texture_hands          = NULL,
    .selected               = {-1, -1},
    .archetype_stronghand   = ITEM_ARCHETYPE_WEAPON,
    .archetype_weakhand     = ITEM_ARCHETYPE_ITEM,
    .update                 = true,
    .unit                   = NULL,
    .pixelnours             = NULL,
    .pixelnours_big         = NULL,
    .header                 = {0},
    .item_name              = {0},
};

/* --- Constructors/Destructors --- */
struct LoadoutSelectMenu *LoadoutSelectMenu_Alloc() {
    struct LoadoutSelectMenu *lsm = malloc(sizeof(struct LoadoutSelectMenu));
    SDL_assert(lsm != NULL);
    *lsm = LoadoutSelectMenu_default;
    SDL_assert(lsm->texture == NULL);

    if (lsm->texture_hands != NULL) {
        SDL_DestroyTexture(lsm->texture_hands);
        lsm->texture_hands = NULL;
    }

    return (lsm);
}

void LoadoutSelectMenu_Free(struct LoadoutSelectMenu *lsm) {
    if (lsm->texture != NULL) {
        SDL_DestroyTexture(lsm->texture);
        lsm->texture = NULL;
    }
    if (lsm->texture_hands != NULL) {
        SDL_DestroyTexture(lsm->texture_hands);
        lsm->texture_hands = NULL;
    }
    s8_free(&lsm->header);
    s8_free(&lsm->item_name);
    if (lsm != NULL) {
        SDL_free(lsm);
        lsm = NULL;
    }
}

void WeaponSelectMenu_Load_n9Patch(struct LoadoutSelectMenu *lsm, SDL_Renderer *r,
                                   struct n9Patch *n9patch)  {
    SDL_assert(n9patch != NULL);
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->size_patches.x  = WSM_PATCH_X_SIZE;
    n9patch->size_patches.y  = WSM_PATCH_X_SIZE;
    n9patch->scale.x         = WSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = WSM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = MENU_PATCH_PIXELS * WSM_PATCH_X_SIZE;
    n9patch->size_pixels.y   = MENU_PATCH_PIXELS * WSM_PATCH_Y_SIZE;
}

void WeaponSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map,
                           tnecs_world_t *world, tnecs_entity_t unit_ent,
                           SDL_Renderer *renderer, struct n9Patch *n9patch) {
    WeaponSelectMenu_Load_n9Patch(lsm, renderer, n9patch);

    lsm->archetype_stronghand = ITEM_ARCHETYPE_WEAPON;
    LoadoutSelectMenu_Load(lsm, map, world, unit_ent, renderer, n9patch, lsm->archetype_stronghand);
}

void StaffSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map, tnecs_world_t *world,
                          tnecs_entity_t unit_ent, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(n9patch != NULL);
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->size_patches.x  = WSM_PATCH_X_SIZE;
    n9patch->size_patches.y  = WSM_PATCH_Y_SIZE;
    n9patch->scale.x         = WSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = WSM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = (MENU_PATCH_PIXELS * WSM_PATCH_X_SIZE);
    n9patch->size_pixels.y   = (MENU_PATCH_PIXELS * WSM_PATCH_Y_SIZE);

    lsm->archetype_stronghand = ITEM_ARCHETYPE_STAFF;
    LoadoutSelectMenu_Load(lsm, map, world, unit_ent, renderer, n9patch, lsm->archetype_stronghand);
}

void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map, tnecs_world_t *world,
                            tnecs_entity_t unit_ent, SDL_Renderer *renderer, struct n9Patch *n9patch, int archetype) {
    SDL_assert(n9patch != NULL);
    SDL_assert(lsm != NULL);
    SDL_assert(unit_ent > TNECS_NULL);
    struct Unit     *unit = TNECS_GET_COMPONENT(world, unit_ent, Unit);
    _LoadoutSelectMenu_Load(lsm, unit, renderer, n9patch);
    Map_Find_Usable(map, world, unit_ent, false, archetype);
}

void _LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Unit *unit,
                             SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(unit               != NULL);
    SDL_assert(n9patch            != NULL);
    SDL_assert(unit->weapons_dtab != NULL);
    lsm->unit    = unit;
    lsm->update  = true;
    if (lsm->texture_hands == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
        lsm->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(lsm->texture_hands != NULL);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);


}


/* --- Elements --- */
void LoadoutSelectMenu_Elem_Reset(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Get number of elements for the menu */
    int stronghand       = Unit_Hand_Strong(lsm->unit);
    bool strong_selected = (lsm->selected[stronghand] > -1);
    int num_eq     = lsm->unit->num_equipment;
    int num_eq_wsm = num_eq < DEFAULT_EQUIPMENT_SIZE ? num_eq + 1 : DEFAULT_EQUIPMENT_SIZE;

    mc->elem_num   = strong_selected ? num_eq_wsm : lsm->unit->num_usable;
    mc->elem_links = wsm_links;

    for (int i = mc->elem_num - 1; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        mc->elem_links[i].top    = WSM_ELEM_NULL;
        mc->elem_links[i].bottom = WSM_ELEM_NULL;
    }

}

void LoadoutSelectMenu_Elem_Pos(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    bool header_drawn = (lsm->header.data != NULL);
    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        int scale_x = mc->n9patch.scale.x;
        int scale_y = mc->n9patch.scale.y;
        int x       = lsm->pos.x + mc->n9patch.pos.x;
        int y       = lsm->pos.y + header_drawn * WSM_ROW_HEIGHT;
        int elem_x  = mc->elem_pos[i].x;
        int elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = x + elem_x * scale_x;
        mc->elem_pos[i].y = y + elem_y * scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;
}

void LoadoutSelectMenu_Elem_Pos_Revert(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    bool header_drawn = (lsm->header.data != NULL);
    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_MENU_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        int scale_x = mc->n9patch.scale.x;
        int scale_y = mc->n9patch.scale.y;
        int x       = lsm->pos.x + mc->n9patch.pos.x;
        int y       = lsm->pos.y + header_drawn * WSM_ROW_HEIGHT;
        int elem_x  = mc->elem_pos[i].x;
        int elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = (elem_x - x) / scale_x;
        mc->elem_pos[i].y = (elem_y - y) / scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_MENU_FRAME;
}


/* --- Checking --- */
bool WeaponSelectMenu_Usable_Remains(struct LoadoutSelectMenu *lsm) {
    /* Are there any side_i weapons left in rest of inventory? */
    bool remains = false;

    /* Get stronghand */
    int stronghand = Unit_Hand_Strong(lsm->unit);
    int weakhand   = 1 - stronghand;

    /* After no weapon was selected */
    if ((lsm->selected[stronghand] < 0) && (lsm->selected[weakhand] < 0)) {
        remains = lsm->unit->num_usable > 0; /* Any usable weapon remains */
    }

    /* After first weapon was selected */
    if ((lsm->selected[stronghand] >= 0) && (lsm->selected[weakhand] < 0)) {
        remains = lsm->unit->num_equipment > 1; /* Any item remains */
    }

    return (remains);
}

/* --- Item placement --- */
int LoadoutSelectMenu_num_items(struct LoadoutSelectMenu *lsm) {
    int stronghand = Unit_Hand_Strong(lsm->unit);
    bool strong_selected = (lsm->selected[stronghand] > -1);
    return (strong_selected ? DEFAULT_EQUIPMENT_SIZE : lsm->unit->num_usable);
}

void LoadoutSelectMenu_Select_Reset(struct LoadoutSelectMenu *lsm) {
    lsm->selected[UNIT_HAND_LEFT]  = -1;
    lsm->selected[UNIT_HAND_RIGHT] = -1;
    lsm->update                    = true;
}

/* - Select Weapon/Staff - */
void LoadoutSelectMenu_Select(struct LoadoutSelectMenu *lsm, int select) {
    /* Select loadout: 2 weapons if two hands  */
    // TODO: return int that says if selection remains?

    /* - Swap weapons according to player choice - */
    int usable_i    = Unit_Id_Strong(lsm->unit, select);                        /* strong space? */
    int side_i      = lsm->unit->eq_usable[usable_i];                           /* side space */
    int stronghand  = Unit_Hand_Strong(lsm->unit);                              /* side space */
    int weakhand    = 1 - stronghand;                                           /* side space */

    if (lsm->selected[stronghand] < 0) {
        SDL_assert(select < lsm->unit->num_usable);
    }

    if (lsm->selected[stronghand] < 0) {
        lsm->selected[stronghand] = side_i;                                     /* side space */
        /* - SWAP - */
        if (side_i != stronghand) {
            Unit_Item_Swap(lsm->unit, side_i, stronghand);                      /* side space */
        }
        Unit_Equip_inHand(lsm->unit, stronghand);

    } else if (lsm->selected[weakhand] < 0) {
        /* For weakhand select, side space == usable space*/
        lsm->selected[weakhand] = select;                                       /* usable_space */

        if (usable_i == stronghand) {
            /* - TWOHANDING - */
            if (Unit_Equipment_Full(lsm->unit)) {
                /* - Drop weapon in weakhand - */
                // TODO: Ask user with item drop menu
                Unit_Item_Drop(lsm->unit, weakhand);
            } else {
                /* - Swap weapon in weakhand - */
                Unit_Item_Swap(lsm->unit, weakhand, lsm->unit->num_equipment);  /* eq space */
            }
            Unit_Equip_TwoHanding(lsm->unit);
        } else if (usable_i != weakhand) {
            /* - SWAP - */
            if (usable_i != weakhand)
                Unit_Item_Swap(lsm->unit, usable_i, weakhand);                  /* side space */
            Unit_Equip_inHand(lsm->unit, weakhand);                             /* eq space */
        }
    } else {
        /* - Both Hands already selected - */
        SDL_Log("Both weapons already selected, but select sent to LoadoutSelectMenu");
        exit(ERROR_Generic);
    }
    lsm->update = true;
}

void ItemSelectMenu_Select(struct LoadoutSelectMenu *lsm, int s) {
    /* Select item: 1 among equipment  */
    // TODO: return int that says if selection remains?

}


void LoadoutSelectMenu_Deselect(struct LoadoutSelectMenu *lsm) {
    /* -- Revert selected item, only for second hand -- */
    SDL_assert(lsm       != NULL);
    SDL_assert(lsm->unit != NULL);

    /*- Get the tophand -*/
    int stronghand = Unit_Hand_Strong(lsm->unit);
    int weakhand   = 1 - stronghand;

    /*- Get the hand to revert -*/
    int reverthand = (lsm->selected[weakhand] > -1) ? weakhand : stronghand;
    int select     = Unit_Id_Strong(lsm->unit, lsm->selected[reverthand]);

    /*- Skip if no item to revert -*/
    if (select == -1) {
        SDL_Log("Warning: No item to deselect");
        return;
    }

    /*- Reverting item -*/
    /* If weakhand revert in "eq_space", otherwise in "usable_space" */
    /* eq_space */     /* usable_space */
    int item    = (reverthand == weakhand) ? select     : lsm->unit->eq_usable[select];
    int to_swap = (reverthand == weakhand) ? reverthand : lsm->unit->eq_usable[reverthand];

    // if (item != to_swap)
    //     Unit_Item_Swap(lsm->unit, item, to_swap);

    lsm->selected[reverthand] = -1;
    lsm->update               = true;
    Unit_Equip_inHand(lsm->unit, reverthand);

}

/* --- Drawing --- */
void LoadoutSelectMenu_Size(struct  LoadoutSelectMenu  *lsm, struct n9Patch *n9patch) {
    /* - Compute new menu width and height - */
    int width, max_width = WSM_ITEM_MIN_WIDTH;
    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    int stronghand = Unit_Hand_Strong(lsm->unit);

    /* If stronghand is selected, menu should change to show all items in equipment */
    bool strong_selected = (lsm->selected[stronghand] > -1);
    int num_items = LoadoutSelectMenu_num_items(lsm);

    for (int i = 0; i < num_items; i++) {
        /* If stronghand was selected, i is in eq_space */
        int side_i = strong_selected ? i : lsm->unit->eq_usable[i];  /* side space */
        SDL_assert((side_i >= 0) && (side_i < DEFAULT_EQUIPMENT_SIZE));

        /* Unit_Item_Strong ensures tophand is stronghand */
        struct Inventory_item *item = Unit_Item_Strong(lsm->unit, side_i);
        if (item->id == ITEM_NULL)
            continue;

        s8_free(&lsm->item_name);
        if (Weapon_ID_isValid(item->id)) {
            /* Item is a weapon */
            SDL_assert(lsm->unit->weapons_dtab != NULL);
            struct Weapon *weapon = DTAB_GET(lsm->unit->weapons_dtab, item->id);
            SDL_assert(weapon != NULL);
            lsm->item_name = s8_mut(weapon->item->name.data);
        } else if (Item_ID_isValid(item->id)) {
            /* Pure item */
            Item_Load(lsm->unit->items_dtab, item->id);
            struct Item *pure_item = DTAB_GET(lsm->unit->items_dtab, item->id);
            lsm->item_name = s8_mut(pure_item->name.data);
        } else {
            SDL_Log("LoadoutSelectMenu: Neither a valid item nor weapon");
            exit(ERROR_Generic);
        }

        width = PixelFont_Width(lsm->pixelnours_big, lsm->item_name.data, lsm->item_name.num);
        max_width = width > max_width ? width : max_width;
    }
    int header_w = 0;
    bool header_drawn = (lsm->header.data != NULL);
    if (header_drawn) {
        header_w = PixelFont_Width(lsm->pixelnours_big, lsm->header.data,
                                   lsm->header.num) + WSM_HEADER_LEFT + WSM_HEADER_RIGHT;
    }

    int size_raw_x    = max_width + WSM_LEFT_OF_TEXT + WSM_RIGHT_OF_TEXT;
    size_raw_x = header_w > size_raw_x ? header_w : size_raw_x;
    int patch_size_x  = size_raw_x   / n9patch->patch_pixels.x;
    lsm->menu_w       = patch_size_x * n9patch->patch_pixels.x;

    int size_raw_y    = num_items * WSM_ROW_HEIGHT + WSM_TOP_OF_TEXT + WSM_BOTTOM_OF_TEXT + header_drawn
                        * WSM_ROW_HEIGHT;
    int patch_size_y  = size_raw_y   / n9patch->patch_pixels.y;
    lsm->menu_h       = patch_size_y * n9patch->patch_pixels.y;

    /* -- Check if new menu texture should be created if menu size changed -- */
    if ((n9patch->size_pixels.x != lsm->menu_w) || (n9patch->size_pixels.y != lsm->menu_h)) {
        n9patch->size_pixels.x  = lsm->menu_w;
        n9patch->size_patches.x = patch_size_x;
        n9patch->size_pixels.y  = lsm->menu_h;
        n9patch->size_patches.y = patch_size_y;
        if (lsm->texture != NULL) {
            SDL_DestroyTexture(lsm->texture);
            lsm->texture = NULL;
        }
    }
}

void LoadoutSelectMenu_Draw(struct Menu *mc, SDL_Texture *target, SDL_Renderer *renderer) {
    struct LoadoutSelectMenu *lsm = (struct LoadoutSelectMenu *)mc->data;
    SDL_assert(lsm != NULL);
    struct n9Patch *n9patch = &mc->n9patch;
    struct Unit *unit       = lsm->unit;

    if (lsm->update) {
        LoadoutSelectMenu_Update(lsm, n9patch, target, renderer);
        lsm->update = false;
    }

    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = lsm->pos.x,
        .y = lsm->pos.y,
    };
    SDL_assert(lsm->texture != NULL);
    SDL_RenderCopy(renderer, lsm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void LoadoutSelectMenu_Header_Set(struct LoadoutSelectMenu *lsm, const char *header) {
    s8_free(&lsm->header);
    lsm->header = s8_mut(header);
}


void _LoadoutSelectMenu_Draw_Header(struct LoadoutSelectMenu  *lsm, SDL_Renderer *renderer) {
    /* Skip if no header to draw */
    if (lsm->header.data == NULL)
        return;

    /* Draw textual header: information to the player */
    int dura_w = PixelFont_Width(lsm->pixelnours_big, lsm->header.data, lsm->header.num);
    PixelFont_Write(lsm->pixelnours_big, renderer, lsm->header.data, lsm->header.num,
                    WSM_HEADER_LEFT,  WSM_TOP_OF_TEXT);
}

void _LoadoutSelectMenu_Draw_Highlight(struct LoadoutSelectMenu  *lsm, SDL_Renderer *renderer) {
    /* - Skip if no highlight - */
    bool highlight = (lsm->selected[UNIT_HAND_LEFT] >= 0);
    if (!highlight)
        return;

    /* - Top Weapon highlight - */
    bool header_drawn = (lsm->header.data != NULL);
    SDL_Rect srcrect;

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    int stronghand = Unit_Hand_Strong(lsm->unit);

    SDL_assert(lsm->unit->weapons_dtab != NULL);
    struct Inventory_item *item   = Unit_Item_Strong(lsm->unit, UNIT_HAND_STRONG);
    struct Weapon         *weapon = DTAB_GET(lsm->unit->weapons_dtab, item->id);
    s8 name = weapon->item->name;
    srcrect.w  = PixelFont_Width(lsm->pixelnours, s8_toUpper(name).data, name.num);
    int uses_left = weapon->item->stats.uses - item->used;
    char numbuff[2];
    stbsp_sprintf(numbuff, "%d", uses_left);

    int dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));

    if (stronghand == UNIT_HAND_LEFT) {
        srcrect.x = WSM1_X_OFFSET + WSM_HANDS_TILESIZE - 3;
    } else if (stronghand == UNIT_HAND_RIGHT) {
        srcrect.x = lsm->menu_w - WSM_HANDS_TILESIZE - WSM_ICON_W * 2 - dura_w - srcrect.w;
    }
    srcrect.y = WSM1_Y_OFFSET + (header_drawn * WSM_ROW_HEIGHT);
    srcrect.w += ITEM_ICON_W + WSM_ICONTOTEXT_W + WSM_HIGHLIGHT_EXTRA_W;
    srcrect.h  = ITEM_ICON_H;
    srcrect.y += 2 ;
    srcrect.h -= 4;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.y -= 1;
    srcrect.x += 1;
    srcrect.h += 2;
    srcrect.w -= 2;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.y -= 1;
    srcrect.x += 1;
    srcrect.h += 2;
    srcrect.w -= 2;
    SDL_RenderFillRect(renderer, &srcrect);
}

void _LoadoutSelectMenu_Draw_Hands(struct LoadoutSelectMenu  *lsm, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    int num_items       = LoadoutSelectMenu_num_items(lsm);
    int stronghand      = Unit_Hand_Strong(lsm->unit);
    bool header_drawn   = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;

    /* Computing y offset for weak hand, or twohanding icon placement */
    int ly_offset = 0, ry_offset = 0;
    if (stronghand == UNIT_HAND_RIGHT) {
        ly_offset = WSM_WEAKHAND_Y_OFFSET;
    } else if (stronghand == UNIT_HAND_LEFT) {
        ry_offset = WSM_WEAKHAND_Y_OFFSET;
    }

    srcrect.w = WSM_HANDS_TILESIZE;
    srcrect.h = WSM_HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* -- Left hand icon -- */
    int hand_i = (stronghand != UNIT_HAND_LEFT) ? WSM_HANDS_SMALL_L : WSM_HANDS_BIG_L;
    /* - Skip if hand lower outside of menu - */
    int limit = (stronghand == UNIT_HAND_LEFT) ? 1 : 2;
    if (num_items >= limit) {
        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = WSM_HANDL_X;
        dstrect.y = WSM_HANDL_Y + ly_offset + (header_drawn * WSM_ROW_HEIGHT);

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_LEFT) {
            dstrect.x += WSM_HAND_SMALLX_OFFSET;
            dstrect.y += WSM_HAND_SMALLY_OFFSET;
        }
        SDL_RenderCopy(renderer, lsm->texture_hands, &srcrect, &dstrect);
    }

    /* -- Right hand icon -- */
    srcrect.w = WSM_HANDS_TILESIZE;
    srcrect.h = WSM_HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    hand_i = (stronghand == UNIT_HAND_LEFT) ? WSM_HANDS_SMALL_R : WSM_HANDS_BIG_R;
    /* - Skip if hand lower outside of menu - */
    limit = (stronghand == UNIT_HAND_RIGHT) ? 1 : 2;
    if (num_items >= limit) {
        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = lsm->menu_w - WSM_HANDS_TILESIZE;
        dstrect.y = WSM_HANDR_Y + ry_offset + (header_drawn * WSM_ROW_HEIGHT);

        /* Moving hand if small */
        if (stronghand == UNIT_HAND_LEFT) {
            dstrect.x += WSM_HAND_SMALLX_OFFSET;
            dstrect.y += WSM_HAND_SMALLY_OFFSET;
        }
        SDL_RenderCopy(renderer, lsm->texture_hands, &srcrect, &dstrect);
    }
}

void _LoadoutSelectMenu_Draw_Items(struct LoadoutSelectMenu  *lsm, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    bool header_drawn = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;
    struct Unit *unit = lsm->unit;
    char numbuff[10];
    struct Inventory_item *item;

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    int stronghand = Unit_Hand_Strong(lsm->unit);
    int weakhand   = 1 - stronghand;
    int num_items  = LoadoutSelectMenu_num_items(lsm);
    bool highlight = (lsm->selected[UNIT_HAND_LEFT] >= 0);

    /* If stronghand is selected, menu should change to show all items in equipment */
    bool strong_selected = (lsm->selected[stronghand] > -1);

    SDL_assert(lsm->unit->num_usable > 0);
    SDL_assert(lsm->unit->num_usable <= DEFAULT_EQUIPMENT_SIZE);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;

    // Unit_Equipment_Print(lsm->unit);
    for (int i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        /* - Icons - */
        // TODO: weapon icons images.
        int side_i   = strong_selected ? i : lsm->unit->eq_usable[i];

        /* Icons, text drawn on line strong_i  */
        int strong_i = Unit_Id_Strong(unit, i);
        /* Skip if hand is out of bounds */
        if (strong_i >= num_items)
            continue;

        item         = Unit_Item_Side(unit, side_i);

        /* -- Weapon icon -- */
        srcrect.x = WSM1_X_OFFSET;
        srcrect.y = WSM1_Y_OFFSET + strong_i * (WSM_ICON_H + 2) + (header_drawn * WSM_ROW_HEIGHT);

        if (strong_i == UNIT_HAND_LEFT) {
            if (stronghand == UNIT_HAND_LEFT) {
                srcrect.x = WSM1_X_OFFSET + WSM_HANDS_TILESIZE - 3;
            } else if (stronghand == UNIT_HAND_RIGHT) {
                srcrect.x = lsm->menu_w - WSM_HANDS_TILESIZE - WSM_ICON_W - 3;
            }
        }

        if (strong_i == UNIT_HAND_RIGHT) {
            if (weakhand == UNIT_HAND_LEFT) {
                srcrect.x = WSM1_X_OFFSET + WSM_HANDS_TILESIZE - 3;
            } else if (weakhand == UNIT_HAND_RIGHT) {
                srcrect.x = lsm->menu_w - WSM_HANDS_TILESIZE - WSM_ICON_W - 3;
            }
        }

        SDL_RenderFillRect(renderer, &srcrect);

        /* -- Weapon name -- */
        if ((side_i == stronghand) && highlight) {
            /* Change black only to grey */
            PixelFont_Swap_Palette(lsm->pixelnours,     renderer, -1, 2);
            PixelFont_Swap_Palette(lsm->pixelnours_big, renderer, -1, 2);
        } else {
            /* Reset black and white */
            PixelFont_Swap_Palette(lsm->pixelnours,     renderer, NES_WHITE, NES_BLACK);
            PixelFont_Swap_Palette(lsm->pixelnours_big, renderer, NES_WHITE, NES_BLACK);
        }

        /* - Write '-' if no weapon - */
        int item_x_offset = WSM1_NAME_X_OFFSET;
        int item_y_offset = WSM1_NAME_Y_OFFSET + strong_i * (ITEM_ICON_H + 2) +
                            (header_drawn * WSM_ROW_HEIGHT);

        if (item->id == ITEM_NULL) {
            PixelFont_Write(lsm->pixelnours, renderer, "-", 1, item_x_offset, item_y_offset);
            continue;
        }
        SDL_assert(unit->weapons_dtab != NULL);
        struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, item->id);

        /* - Write uses left - */
        int item_dura_x_offset = WSM1_DURA_X_OFFSET;
        int item_dura_y_offset = WSM1_DURA_Y_OFFSET + strong_i * (ITEM_ICON_H + 2) +
                                 (header_drawn * WSM_ROW_HEIGHT);

        if (strong_i == UNIT_HAND_LEFT) {
            if (stronghand == UNIT_HAND_LEFT) {
                item_dura_x_offset = srcrect.x + WSM_HANDS_TILESIZE + 3;
            } else if (stronghand == UNIT_HAND_RIGHT) {
                item_dura_x_offset = srcrect.x - WSM_ICON_W / 2 - 1;
            }
        }

        if (strong_i == UNIT_HAND_RIGHT) {
            if (weakhand == UNIT_HAND_LEFT) {
                item_dura_x_offset = srcrect.x + WSM_HANDS_TILESIZE + 3;
            } else if (weakhand == UNIT_HAND_RIGHT) {
                item_dura_x_offset = srcrect.x - WSM_ICON_W / 2 - 1;
            }
        }

        SDL_assert(weapon != NULL);
        SDL_assert((side_i >= 0) && (side_i <= DEFAULT_EQUIPMENT_SIZE));
        int uses_left = weapon->item->stats.uses - item->used;
        stbsp_sprintf(numbuff, "%d", uses_left);

        int dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;
        PixelFont_Write(lsm->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        item_dura_x_offset,  item_dura_y_offset);

        /* - Write weapon name - */
        lsm->item_name = s8cpy(lsm->item_name, weapon->item->name);
        lsm->item_name = s8_toUpper(lsm->item_name);
        int name_w = PixelFont_Width(lsm->pixelnours, lsm->item_name.data, lsm->item_name.num);

        if (strong_i == UNIT_HAND_LEFT) {
            if (stronghand == UNIT_HAND_LEFT) {
                item_x_offset = item_dura_x_offset + dura_w / 2 + 8;
            } else if (stronghand == UNIT_HAND_RIGHT) {
                item_x_offset = item_dura_x_offset - name_w - 4;
            }
        }

        if (strong_i == UNIT_HAND_RIGHT) {
            if (weakhand == UNIT_HAND_LEFT) {
                item_x_offset = item_dura_x_offset + dura_w / 2 + 8;
            } else if (weakhand == UNIT_HAND_RIGHT) {
                item_x_offset = item_dura_x_offset - name_w - 4;
            }
        }

        PixelFont_Write(lsm->pixelnours, renderer, lsm->item_name.data, lsm->item_name.num, item_x_offset,
                        item_y_offset);
    }

}
void LoadoutSelectMenu_Texture_Create(struct LoadoutSelectMenu  *lsm, struct n9Patch *n9patch,
                                      SDL_Renderer *renderer) {
    /* -- Compute menu size -- */
    LoadoutSelectMenu_Size(lsm, n9patch);

    /* -- Create new texture -- */
    if (lsm->texture == NULL) {
        lsm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, lsm->menu_w, lsm->menu_h);
        SDL_assert(lsm->texture != NULL);
        SDL_SetTextureBlendMode(lsm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_assert(lsm->texture != NULL);
}

void LoadoutSelectMenu_Update(struct LoadoutSelectMenu *lsm, struct n9Patch *n9patch,
                              SDL_Texture *target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(lsm      != NULL);
    SDL_assert(renderer != NULL);
    LoadoutSelectMenu_Texture_Create(lsm, n9patch, renderer);

    /* - Clear the texture. - */
    SDL_SetRenderTarget(renderer, lsm->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);

    /* --- RENDERING --- */
    /* -- PATCHES DRAW -- */
    int scale_x      = n9patch->scale.x;
    int scale_y      = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    _LoadoutSelectMenu_Draw_Header(lsm, renderer);
    _LoadoutSelectMenu_Draw_Highlight(lsm, renderer);
    _LoadoutSelectMenu_Draw_Hands(lsm, renderer);
    _LoadoutSelectMenu_Draw_Items(lsm, renderer);

    SDL_SetRenderTarget(renderer, target);
}
