#include "menu/loadout_select.h"

struct Point wsm_cursor_pos[LSM_ELEMS_NUM] = {
    /* LSM_ELEM_ITEM2 */ {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* LSM_ELEM_ITEM2 */ {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* LSM_ELEM_ITEM3 */ {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* LSM_ELEM_ITEM4 */ {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* LSM_ELEM_ITEM5 */ {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* LSM_ELEM_ITEM6 */ {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

struct Point wsm_elem_box[LSM_ELEMS_NUM] = {
    /* LSM_ELEM_ITEM1 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* LSM_ELEM_ITEM2 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* LSM_ELEM_ITEM3 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* LSM_ELEM_ITEM4 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* LSM_ELEM_ITEM5 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* LSM_ELEM_ITEM6 */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct Point  wsm_elem_pos[LSM_ELEMS_NUM] = {
    /* LSM_ELEM_ITEM1 */ {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* LSM_ELEM_ITEM2 */ {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* LSM_ELEM_ITEM3 */ {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* LSM_ELEM_ITEM4 */ {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* LSM_ELEM_ITEM5 */ {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* LSM_ELEM_ITEM6 */ {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

struct MenuElemDirections wsm_links_start[LSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* LSM_ELEM_ITEM1 */ {LSM_ELEM_NULL, LSM_ELEM_NULL,  LSM_ELEM_NULL, LSM_ELEM_ITEM2},
    /* LSM_ELEM_ITEM2 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM1, LSM_ELEM_NULL, LSM_ELEM_ITEM3},
    /* LSM_ELEM_ITEM3 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM2, LSM_ELEM_NULL, LSM_ELEM_ITEM4},
    /* LSM_ELEM_ITEM4 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM3, LSM_ELEM_NULL, LSM_ELEM_ITEM5},
    /* LSM_ELEM_ITEM5 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM4, LSM_ELEM_NULL, LSM_ELEM_ITEM6},
    /* LSM_ELEM_ITEM6 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM5, LSM_ELEM_NULL, LSM_ELEM_NULL},
};

struct MenuElemDirections wsm_links[LSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* LSM_ELEM_ITEM1 */ {LSM_ELEM_NULL, LSM_ELEM_NULL,  LSM_ELEM_NULL, LSM_ELEM_ITEM2},
    /* LSM_ELEM_ITEM2 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM1, LSM_ELEM_NULL, LSM_ELEM_ITEM3},
    /* LSM_ELEM_ITEM3 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM2, LSM_ELEM_NULL, LSM_ELEM_ITEM4},
    /* LSM_ELEM_ITEM4 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM3, LSM_ELEM_NULL, LSM_ELEM_ITEM5},
    /* LSM_ELEM_ITEM5 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM4, LSM_ELEM_NULL, LSM_ELEM_ITEM6},
    /* LSM_ELEM_ITEM6 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM5, LSM_ELEM_NULL, LSM_ELEM_NULL},
};

struct LoadoutSelectMenu LoadoutSelectMenu_default = {
    .update                 = true,
    .selected               = {LSM_ELEM_NULL, LSM_ELEM_NULL},
    .archetype_stronghand   = ITEM_ARCHETYPE_STRONGHAND_ATTACK,
    .archetype_weakhand     = ITEM_ARCHETYPE_WEAKHAND,
    .black                  = SOTA_BLACK,
    .white                  = SOTA_WHITE,
};

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static void _LoadoutSelectMenu_Draw_Items(      struct    LoadoutSelectMenu *l,
                                                SDL_Renderer                *r);
static void _LoadoutSelectMenu_Draw_Hands(      struct Menu                 *m,
                                                struct  LoadoutSelectMenu   *l,
                                                SDL_Renderer                *r);
static void _LoadoutSelectMenu_Draw_Header(     struct  LoadoutSelectMenu   *l,
                                                SDL_Renderer                *r);
static void _LoadoutSelectMenu_Draw_Highlight(  struct  LoadoutSelectMenu   *l,
                                                SDL_Renderer                *r);

/* --- Constructors/Destructors --- */
struct LoadoutSelectMenu *LoadoutSelectMenu_Alloc(void) {
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
    n9patch->size_patches.x  = LSM_PATCH_X_SIZE;
    n9patch->size_patches.y  = LSM_PATCH_X_SIZE;
    n9patch->scale.x         = LSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = LSM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = MENU_PATCH_PIXELS * LSM_PATCH_X_SIZE;
    n9patch->size_pixels.y   = MENU_PATCH_PIXELS * LSM_PATCH_Y_SIZE;
}

void WeaponSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map,
                           tnecs_world *world, tnecs_entity unit_ent,
                           SDL_Renderer *renderer, struct n9Patch *n9patch) {
    WeaponSelectMenu_Load_n9Patch(lsm, renderer, n9patch);

    lsm->archetype_stronghand = ITEM_ARCHETYPE_STRONGHAND_ATTACK;
    LoadoutSelectMenu_Load(lsm, map, world, unit_ent, renderer, n9patch, lsm->archetype_stronghand);

    matrix_print(map->attacktomap, map->row_len, map->col_len);

    SDL_Log("num %d", lsm->unit->num_canEquip);
    getchar();
}

void StaffSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map, tnecs_world *world,
                          tnecs_entity unit_ent, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(n9patch != NULL);
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->size_patches.x  = LSM_PATCH_X_SIZE;
    n9patch->size_patches.y  = LSM_PATCH_Y_SIZE;
    n9patch->scale.x         = LSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = LSM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = (MENU_PATCH_PIXELS * LSM_PATCH_X_SIZE);
    n9patch->size_pixels.y   = (MENU_PATCH_PIXELS * LSM_PATCH_Y_SIZE);

    lsm->archetype_stronghand = ITEM_ARCHETYPE_STRONGHAND_STAFF;
    LoadoutSelectMenu_Load(lsm, map, world, unit_ent, renderer, n9patch, lsm->archetype_stronghand);
}

void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map,
                            tnecs_world *world, tnecs_entity unit_ent,
                            SDL_Renderer *renderer, struct n9Patch *n9patch,
                            u64 archetype) {
    SDL_assert(n9patch != NULL);
    SDL_assert(lsm != NULL);
    SDL_assert(unit_ent > TNECS_NULL);
    SDL_assert(map          != NULL);
    SDL_assert(map->world   != NULL);

    struct Unit     *unit = TNECS_GET_COMPONENT(map->world, unit_ent, Unit);
    _LoadoutSelectMenu_Load(lsm, unit, renderer, n9patch);
    Map_canEquip(map, unit_ent, false, archetype);
}

void _LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Unit *unit,
                             SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(unit               != NULL);
    SDL_assert(n9patch            != NULL);
    SDL_assert(unit->weapons_dtab != NULL);
    LoadoutSelectMenu_Unit(lsm, unit);
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

/* --- Elem Move --- */
i32 ItemSelectMenu_Elem_Move( struct Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, SOTA_EQUIPMENT_SIZE));
}

i32 WeaponSelectMenu_Elem_Move(struct Menu *mc, i32 direction) {
    struct LoadoutSelectMenu *wsm = mc->data;
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

/* --- Elements --- */
void LoadoutSelectMenu_Elem_Reset(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Get number of elements for the menu */
    mc->elem_num   = lsm->unit->num_canEquip;
    size_t bytesize = sizeof(*wsm_links_start) * LSM_ELEMS_NUM;
    memcpy(mc->elem_links, wsm_links_start, bytesize);

    mc->elem_links[0].top                   = LSM_ELEM_NULL;
    mc->elem_links[mc->elem_num - 1].bottom = LSM_ELEM_NULL;
}

void LoadoutSelectMenu_Elem_Pos(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    b32 header_drawn = (lsm->header.data != NULL);
    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME)
        return;

    for (size_t i = 0; i < mc->elem_num; i++) {
        i32 scale_x = mc->n9patch.scale.x;
        i32 scale_y = mc->n9patch.scale.y;
        i32 x       = lsm->pos.x + mc->n9patch.pos.x;
        i32 y       = lsm->pos.y + header_drawn * LSM_ROW_HEIGHT;
        i32 elem_x  = mc->elem_pos[i].x;
        i32 elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = x + elem_x * scale_x;
        mc->elem_pos[i].y = y + elem_y * scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;
}

void LoadoutSelectMenu_Elem_Pos_Revert(struct LoadoutSelectMenu *lsm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    b32 header_drawn = (lsm->header.data != NULL);
    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_MENU_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        i32 scale_x = mc->n9patch.scale.x;
        i32 scale_y = mc->n9patch.scale.y;
        i32 x       = lsm->pos.x + mc->n9patch.pos.x;
        i32 y       = lsm->pos.y + header_drawn * LSM_ROW_HEIGHT;
        i32 elem_x  = mc->elem_pos[i].x;
        i32 elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = (elem_x - x) / scale_x;
        mc->elem_pos[i].y = (elem_y - y) / scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_MENU_FRAME;
}

/* --- Checking --- */
b32 WeaponSelectMenu_Usable_Remains(struct LoadoutSelectMenu *lsm) {
    /* Are there any side_i weapons left in rest of inventory? */
    b32 remains = false;

    /* Get stronghand */
    i32 stronghand = Unit_Hand_Strong(lsm->unit);
    i32 weakhand   = 1 - stronghand;

    /* After no weapon was selected */
    if ((lsm->selected[stronghand] < 0) && (lsm->selected[weakhand] < 0)) {
        remains = lsm->unit->num_canEquip > 0; /* Any usable weapon remains? */
    }

    /* After first weapon was selected */
    if ((lsm->selected[stronghand] >= 0) && (lsm->selected[weakhand] < 0)) {
        remains = lsm->unit->num_equipment > 1; /* Any item remains? */
    }

    return (remains);
}

/* --- Item placement --- */
void LoadoutSelectMenu_Unit(struct LoadoutSelectMenu *lsm, struct Unit *unit) {
    SDL_assert(lsm  != NULL);
    SDL_assert(unit != NULL);
    lsm->unit = unit;
}


void LoadoutSelectMenu_Select_Reset(struct LoadoutSelectMenu *lsm) {
    lsm->selected[UNIT_HAND_LEFT]  = LSM_ELEM_NULL;
    lsm->selected[UNIT_HAND_RIGHT] = LSM_ELEM_NULL;
    lsm->update                    = true;
}

/* - Select Weapon/Staff - */
void LoadoutSelectMenu_Select(struct LoadoutSelectMenu *lsm, i32 select) {
    /* Player just selected loadout. */

    /* Note: select is in strong space: stronghandd first hand */
    /* - Equip weapons according to player choice - */
    i32 eq          = lsm->unit->eq_canEquip[select];
    i32 stronghand  = Unit_Hand_Strong(lsm->unit);
    i32 weakhand    = 1 - stronghand;

    // If stronghand is unselected, there should be usable weapons
    if (lsm->selected[stronghand] < 0) {
        SDL_assert(select < lsm->unit->num_canEquip);
    }

    if (lsm->selected[stronghand] >= 0) {
        lsm->selected[weakhand] = eq;
        Unit_Equip(lsm->unit, weakhand, eq);
    } else if (lsm->selected[stronghand] < 0) {
        lsm->selected[stronghand] = eq;
        Unit_Equip(lsm->unit, stronghand, eq);
    } else {
        /* - Both Hands already selected - */
        SDL_Log("Both weapons already selected, but select sent to LoadoutSelectMenu");
        SDL_assert(false);      /* For  debug   */
        exit(ERROR_Generic);    /* For release  */
    }
    lsm->update = true;
}

void ItemSelectMenu_Select(struct LoadoutSelectMenu *lsm, i32 s) {
    /* Select item: 1 among equipment  */
    // TODO: return i32 that says if selection remains?

}

void LoadoutSelectMenu_Deselect(struct LoadoutSelectMenu *lsm) {
    /* -- Revert selected item, only for second hand -- */
    SDL_assert(lsm       != NULL);
    SDL_assert(lsm->unit != NULL);
    lsm->update           = true;

    /*- Get the tophand -*/
    i32 stronghand = Unit_Hand_Strong(lsm->unit);
    i32 weakhand   = 1 - stronghand;

    /*- Skip if no item to revert -*/
    if ((lsm->selected[weakhand] == LSM_ELEM_NULL) && (lsm->selected[stronghand] == LSM_ELEM_NULL)) {
        SDL_Log("Warning: No item to deselect");
        return;
    }
    if (lsm->selected[weakhand] != LSM_ELEM_NULL) {
        lsm->selected[weakhand] = LSM_ELEM_NULL;
    } else if (lsm->selected[stronghand] != LSM_ELEM_NULL) {
        lsm->selected[stronghand] = LSM_ELEM_NULL;
    }
}

/* --- Drawing --- */
void LoadoutSelectMenu_Size(struct  LoadoutSelectMenu  *lsm, struct n9Patch *n9patch) {
    /* - Compute new menu width and height - */
    i32 width, max_width = LSM_ITEM_MIN_WIDTH;
    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    i32 stronghand = Unit_Hand_Strong(lsm->unit);

    /* If stronghand is selected, menu should change to show all items in equipment */
    b32 strong_selected = (lsm->selected[stronghand] >= 0)
                          && (lsm->selected[stronghand] < SOTA_EQUIPMENT_SIZE);
    i32 num_items = lsm->unit->num_canEquip;

    for (i32 i = 0; i < num_items; i++) {
        /* If stronghand was selected, i is in eq_space */
        i32 side_i = strong_selected ? i : lsm->unit->eq_canEquip[i];  /* side space */
        SDL_assert((side_i >= 0) && (side_i < SOTA_EQUIPMENT_SIZE));

        /* unit_item ensures tophand is stronghand */
        struct Inventory_item *item = Unit_InvItem(lsm->unit, side_i);
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
    i32 header_w = 0;
    b32 header_drawn = (lsm->header.data != NULL);
    if (header_drawn) {
        header_w = PixelFont_Width(lsm->pixelnours_big, lsm->header.data,
                                   lsm->header.num) + LSM_HEADER_LEFT + LSM_HEADER_RIGHT;
    }

    i32 size_raw_x    = max_width + LSM_LEFT_OF_TEXT + LSM_RIGHT_OF_TEXT;
    size_raw_x = header_w > size_raw_x ? header_w : size_raw_x;
    i32 patch_size_x  = size_raw_x   / n9patch->patch_pixels.x;
    lsm->menu_w       = patch_size_x * n9patch->patch_pixels.x;

    i32 size_raw_y    = num_items * LSM_ROW_HEIGHT + LSM_TOP_OF_TEXT + LSM_BOTTOM_OF_TEXT + header_drawn
                        * LSM_ROW_HEIGHT;
    i32 patch_size_y  = size_raw_y   / n9patch->patch_pixels.y;
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
        LoadoutSelectMenu_Update(mc, lsm, n9patch, target, renderer);
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

void LoadoutSelectMenu_Header_Set(struct LoadoutSelectMenu *lsm,  char *header) {
    s8_free(&lsm->header);
    lsm->header = s8_mut(header);
}

static void _LoadoutSelectMenu_Draw_Header(struct LoadoutSelectMenu *lsm,
                                           SDL_Renderer      *renderer) {
    /* Skip if no header to draw */
    if (lsm->header.data == NULL)
        return;

    /* Draw textual header: information to the player */
    i32 dura_w = PixelFont_Width(lsm->pixelnours_big, lsm->header.data, lsm->header.num);
    PixelFont_Write(lsm->pixelnours_big, renderer, lsm->header.data, lsm->header.num,
                    LSM_HEADER_LEFT,  LSM_TOP_OF_TEXT);
}

// static void _LoadoutSelectMenu_Draw_Highlight(struct LoadoutSelectMenu  *lsm,
//                                               SDL_Renderer       *renderer) {
//     /* - Skip if no highlight - */
//     b32 highlight = (lsm->selected[0] >= 0);
//     if (!highlight)
//         return;

//     /* - Top Weapon highlight - */
//     b32 header_drawn = (lsm->header.data != NULL);
//     SDL_Rect srcrect;

//     SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
//     i32 stronghand = Unit_Hand_Strong(lsm->unit);

//     SDL_assert(lsm->unit->weapons_dtab != NULL);
//     struct Inventory_item *item   = Unit_Item_Equipped(lsm->unit, Unit_Hand_Strong(lsm->unit));
//     SDL_Log("item->id %d", item->id);
//     Weapon_Load(lsm->unit->weapons_dtab, item->id);
//     struct Weapon         *weapon = DTAB_GET(lsm->unit->weapons_dtab, item->id);
//     SDL_assert(weapon               != NULL);
//     SDL_assert(weapon->item         != NULL);
//     s8 name = weapon->item->name;
//     srcrect.w  = PixelFont_Width(lsm->pixelnours, s8_toUpper(name).data, name.num);
//     i32 uses_left = weapon->item->stats.uses - item->used;
//     char numbuff[8];
//     stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

//     i32 dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));

//     if (stronghand == UNIT_HAND_LEFT) {
//         srcrect.x = LSM1_X_OFFSET + LSM_HANDS_TILESIZE - 3;
//     } else if (stronghand == UNIT_HAND_RIGHT) {
//         srcrect.x = lsm->menu_w - LSM_HANDS_TILESIZE - LSM_ICON_W * 2 - dura_w - srcrect.w;
//     }
//     srcrect.y = LSM1_Y_OFFSET + (header_drawn * LSM_ROW_HEIGHT);
//     srcrect.w += ITEM_ICON_W + LSM_ICONTOTEXT_W + LSM_HIGHLIGHT_EXTRA_W;
//     srcrect.h  = ITEM_ICON_H;
//     srcrect.y += 2 ;
//     srcrect.h -= 4;
//     SDL_RenderFillRect(renderer, &srcrect);
//     srcrect.y -= 1;
//     srcrect.x += 1;
//     srcrect.h += 2;
//     srcrect.w -= 2;
//     SDL_RenderFillRect(renderer, &srcrect);
//     srcrect.y -= 1;
//     srcrect.x += 1;
//     srcrect.h += 2;
//     srcrect.w -= 2;
//     SDL_RenderFillRect(renderer, &srcrect);
// }

static void _LoadoutSelectMenu_Draw_Hands(struct Menu *mc,
                                          struct LoadoutSelectMenu *lsm,
                                          SDL_Renderer      *renderer) {
    /* -- Preliminaries -- */
    i32 num_items      = lsm->unit->num_canEquip;
    b32 stronghand     = Unit_Hand_Strong(lsm->unit);
    b32 weakhand       = 1 - stronghand;
    b32 header_drawn   = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;

    b32 strong_selected = (lsm->selected[stronghand] >= 0)
                          && (lsm->selected[stronghand] < SOTA_EQUIPMENT_SIZE);

    b32 draw_left_hand  = (strong_selected && (weakhand == UNIT_HAND_LEFT)) ||
                          (stronghand == UNIT_HAND_LEFT);
    b32 draw_right_hand = (strong_selected && (weakhand == UNIT_HAND_RIGHT)) ||
                          (stronghand == UNIT_HAND_RIGHT);

    /* -- Left hand icon -- */
    if (draw_left_hand) {
        srcrect.w = LSM_HANDS_TILESIZE;
        srcrect.h = LSM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        // 1. Follows cursor when selecting for hand
        // 2. Is at selected item position if stronghnad
        int hand_i = (weakhand == UNIT_HAND_LEFT) ? LSM_HANDS_SMALL_L : LSM_HANDS_BIG_L;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = LSM_HANDL_X;

        int left_hand_row = ((UNIT_HAND_LEFT == stronghand)
                             && !strong_selected) ? mc->elem : lsm->selected[stronghand];

        /* Computing y offset for weak hand, or twohanding icon placement */
        i32 ly_offset = (stronghand == UNIT_HAND_RIGHT) ? LSM_WEAKHAND_Y_OFFSET : LSM_STRONGHAND_Y_OFFSET;

        dstrect.y = ly_offset + (header_drawn + left_hand_row) * LSM_ROW_HEIGHT;

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_LEFT) {
            dstrect.x += LSM_HAND_SMALLX_OFFSET;
            dstrect.y += LSM_HAND_SMALLY_OFFSET;
        }
        SDL_RenderCopy(renderer, lsm->texture_hands, &srcrect, &dstrect);
    }

    /* -- Right hand icon -- */
    if (draw_right_hand) {
        srcrect.w = LSM_HANDS_TILESIZE;
        srcrect.h = LSM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        // 1. Follows cursor when selecting for hand
        // 2. Is at selected item position if stronghnad
        int hand_i = (stronghand == UNIT_HAND_LEFT) ? LSM_HANDS_SMALL_R : LSM_HANDS_BIG_R;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = lsm->menu_w - LSM_HANDS_TILESIZE;

        int right_hand_row = ((UNIT_HAND_RIGHT == stronghand)
                              && !strong_selected) ? mc->elem : lsm->selected[stronghand];


        /* Computing y offset for weak hand, or twohanding icon placement */
        int ry_offset = (stronghand == UNIT_HAND_RIGHT) ? LSM_STRONGHAND_Y_OFFSET : LSM_WEAKHAND_Y_OFFSET;

        dstrect.y = ry_offset + (header_drawn + right_hand_row) * LSM_ROW_HEIGHT;

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_RIGHT) {
            dstrect.x += LSM_HAND_SMALLX_OFFSET;
            dstrect.y += LSM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, lsm->texture_hands, &srcrect, &dstrect);
    }
}

static void _LoadoutSelectMenu_Draw_Items(struct LoadoutSelectMenu  *lsm,
                                          SDL_Renderer       *renderer) {
    /* -- Preliminaries -- */
    b32 header_drawn = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;
    struct Unit *unit = lsm->unit;
    char numbuff[10];
    struct Inventory_item *item;

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    i32 stronghand = Unit_Hand_Strong(lsm->unit);
    i32 weakhand   = 1 - stronghand;
    i32 num_items  = lsm->unit->num_canEquip;
    // b32 highlight  = (lsm->selected[0] >= 0);
    b32 highlight  = false;

    /* If stronghand is selected, menu should change to show all items in equipment */
    b32 strong_selected = (lsm->selected[stronghand] > LSM_ELEM_NULL);

    SDL_assert(lsm->unit->num_canEquip > 0);
    SDL_assert(lsm->unit->num_canEquip <= SOTA_EQUIPMENT_SIZE);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;

    for (i32 i = 0; i < lsm->unit->num_canEquip; i++) {
        /* - Icons - */
        // TODO: weapon icons images.
        i32 side_i   = lsm->unit->eq_canEquip[i];

        item = Unit_InvItem(unit, side_i);

        /* Icons, text drawn on line strong_i  */

        /* -- Weapon icon -- */
        srcrect.x = LSM1_X_OFFSET;
        srcrect.y = LSM1_Y_OFFSET + i * (LSM_ICON_H + 2) + (header_drawn * LSM_ROW_HEIGHT);

        SDL_RenderFillRect(renderer, &srcrect);

        /* -- Weapon name -- */
        if ((side_i == stronghand) && highlight) {
            /* Change black only to grey */
            PixelFont_Swap_Palette(lsm->pixelnours,     renderer, -1, 2);
            PixelFont_Swap_Palette(lsm->pixelnours_big, renderer, -1, 2);
        }

        /* - Write '-' if no weapon - */
        i32 item_x_offset = LSM1_NAME_X_OFFSET;
        i32 item_y_offset = LSM1_NAME_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                            (header_drawn * LSM_ROW_HEIGHT);


        if ((item->id == ITEM_NULL) || !Weapon_ID_isValid(item->id)) {
            PixelFont_Write(lsm->pixelnours, renderer, "-", 1, item_x_offset, item_y_offset);
            continue;
        }

        SDL_assert(unit->weapons_dtab != NULL);
        struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, item->id);
        SDL_assert(weapon != NULL);

        /* - Uses left - */
        i32 item_dura_x_offset = LSM1_DURA_X_OFFSET;
        i32 item_dura_y_offset = LSM1_DURA_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                                 (header_drawn * LSM_ROW_HEIGHT);

        SDL_assert((side_i >= 0) && (side_i <= SOTA_EQUIPMENT_SIZE));
        i32 uses_left = weapon->item->stats.uses - item->used;
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

        i32 dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;

        PixelFont_Write(lsm->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        item_dura_x_offset,  item_dura_y_offset);

        /* - Weapon name - */
        lsm->item_name = s8cpy(lsm->item_name, Unit_Name(item->id));
        lsm->item_name = s8_toUpper(lsm->item_name);
        i32 name_w = PixelFont_Width(lsm->pixelnours, lsm->item_name.data, lsm->item_name.num);
        PixelFont_Write(lsm->pixelnours, renderer, lsm->item_name.data, lsm->item_name.num, item_x_offset,
                        item_y_offset);
    }

    /* Reset colors */
    PixelFont_Swap_Palette(lsm->pixelnours,     renderer, lsm->white, lsm->black);
    PixelFont_Swap_Palette(lsm->pixelnours_big, renderer, lsm->white, lsm->black);
    Utilities_DrawColor_Reset(renderer);
}

void LoadoutSelectMenu_Texture_Create(struct LoadoutSelectMenu  *lsm,
                                      struct n9Patch *n9patch,
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

void LoadoutSelectMenu_Update(struct  Menu *mc, struct LoadoutSelectMenu *lsm,
                              struct n9Patch *n9patch,
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
    i32 scale_x      = n9patch->scale.x;
    i32 scale_y      = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    _LoadoutSelectMenu_Draw_Header(lsm, renderer);
    // _LoadoutSelectMenu_Draw_Highlight(lsm, renderer);
    _LoadoutSelectMenu_Draw_Hands(mc, lsm, renderer);
    _LoadoutSelectMenu_Draw_Items(lsm, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}
