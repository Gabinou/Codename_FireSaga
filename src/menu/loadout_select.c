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
                           tnecs_world *world, SDL_Renderer *renderer,
                           struct n9Patch *n9patch) {
    WeaponSelectMenu_Load_n9Patch(lsm, renderer, n9patch);

    LoadoutSelectMenu_Load(lsm, map, world, renderer, n9patch);
}

void StaffSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map, tnecs_world *world,
                          SDL_Renderer *renderer, struct n9Patch *n9patch) {
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
    LoadoutSelectMenu_Load(lsm, map, world, renderer, n9patch);
}

void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm, struct Map *map,
                            tnecs_world *world, SDL_Renderer *renderer,
                            struct n9Patch *n9patch) {
    SDL_assert(lsm      != NULL);
    SDL_assert(world    != NULL);
    SDL_assert(map      != NULL);
    SDL_assert(n9patch  != NULL);
    lsm->map    = map;
    lsm->world  = world;

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
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->world   != NULL);
    Unit *unit = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);

    mc->elem_num   = unit->num_canEquip;
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
    /* Are there any eq weapons left in rest of inventory? */
    /* If both hands are equipped, returns false */
    b32 remains = false;

    /* Get stronghand */
    struct Unit *unit = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);

    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    if (!Loadout_isEquipped(&lsm->selected, stronghand) &&
        !Loadout_isEquipped(&lsm->selected, weakhand)) {
        /* After no weapon was selected */
        remains = unit->num_canEquip > 0; /* Any usable weapon remains? */
    } else if (Loadout_isEquipped(&lsm->selected, stronghand) &&
               !Loadout_isEquipped(&lsm->selected, weakhand)) {
        /* After first weapon was selected */
        remains = unit->num_equipment > 1; /* Any item remains? */
    }

    return (remains);
}
/* --- CanEquip --- */
void LoadoutSelectMenu_canEquip(struct LoadoutSelectMenu *lsm) {
    // If no hand selected: Map_canEquip of archetype, without movement, any loadout
    // If any hand selected:  Map_canEquip of archetype, without movement, selected loadout
}

/* --- Item placement --- */
void LoadoutSelectMenu_Unit(struct LoadoutSelectMenu *lsm, tnecs_entity ent) {
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->map     != NULL);
    SDL_assert(lsm->world   != NULL);
    SDL_assert(ent  > TNECS_NULL);
    lsm->unit = ent;
    Unit *unit = TNECS_GET_COMPONENT(lsm->world, ent, Unit);
    Unit_Loadout_Export(unit, &lsm->initial);
    lsm->update  = true;
}

void LoadoutSelectMenu_Select_Reset(struct LoadoutSelectMenu *lsm) {
    for (i32 hand = UNIT_HAND_LEFT; hand <= MAX_ARMS_NUM; hand++) {
        Loadout_None(&lsm->selected, hand);
    }
    lsm->update = true;
}

/* - Select Weapon/Staff - */
void LoadoutSelectMenu_Select(struct LoadoutSelectMenu *lsm, i32 select) {
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->world   != NULL);
    SDL_assert(lsm->unit    > TNECS_NULL);
    /* Player just selected loadout, equip it */

    /* - Equip weapons according to player choice - */
    Unit *unit      = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);
    i32 eq          = unit->eq_canEquip[select];
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    // There should be always be usable weapons
    SDL_assert(select < unit->num_canEquip);

    if (!Loadout_isEquipped(&lsm->selected, stronghand)) {
        Loadout_Set(&lsm->selected, stronghand, eq);
        Unit_Equip(unit, stronghand, eq);
    } else if (Loadout_isEquipped(&lsm->selected, stronghand)) {
        Loadout_Set(&lsm->selected, weakhand, eq);
        Unit_Equip(unit, weakhand, eq);
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
    /* -- Revert selected item -- */
    SDL_assert(lsm       != NULL);
    SDL_assert(lsm->unit > TNECS_NULL);
    lsm->update           = true;

    /*- Get the tophand -*/
    Unit *unit      = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);
    SDL_assert(unit       != NULL);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    /*- Skip if no item to revert -*/
    if (
            !Loadout_isEquipped(&lsm->selected, stronghand) &&
            !Loadout_isEquipped(&lsm->selected, weakhand)
    ) {
        SDL_Log("Warning: No item to deselect");
        return;
    } else if (Loadout_isEquipped(&lsm->selected, weakhand)) {
        Loadout_None(&lsm->selected, weakhand);
    } else if (Loadout_isEquipped(&lsm->selected, stronghand)) {
        Loadout_None(&lsm->selected, stronghand);
    }
}

/* --- Drawing --- */
void LoadoutSelectMenu_Size(struct  LoadoutSelectMenu  *lsm, struct n9Patch *n9patch) {
    /* - Compute new menu width and height - */
    Unit *unit      = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);
    i32 width, max_width = LSM_ITEM_MIN_WIDTH;
    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    i32 stronghand = Unit_Hand_Strong(unit);

    /* If stronghand is selected, menu should change to show all items in equipment */
    b32 strong_selected = Loadout_isEquipped(&lsm->selected, stronghand);
    i32 num_items = unit->num_canEquip;

    for (i32 i = 0; i < num_items; i++) {

        i32 eq = strong_selected ? i + ITEM1 : unit->eq_canEquip[i];
        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 id = Unit_Id_Equipment(unit, eq);

        /* unit_item ensures tophand is stronghand */
        // struct Inventory_item *item = Unit_InvItem(unit, eq);

        if (id == ITEM_NULL)
            continue;

        s8_free(&lsm->item_name);
        if (Weapon_ID_isValid(id)) {
            /* Item is a weapon */
            SDL_assert(unit->weapons_dtab != NULL);
            struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
            SDL_assert(weapon != NULL);
            lsm->item_name = s8_mut(weapon->item->name.data);
        } else if (Item_ID_isValid(id)) {
            /* Pure item */
            Item_Load(unit->items_dtab, id);
            struct Item *item = DTAB_GET(unit->items_dtab, id);
            lsm->item_name = s8_mut(item->name.data);
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
    SDL_assert(lsm->unit > TNECS_NULL);
    SDL_assert(lsm->world != NULL);
    struct n9Patch *n9patch = &mc->n9patch;
    lsm->update           = true;

    /*- Get the tophand -*/
    Unit *unit      = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);

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
    SDL_assert(lsm != NULL);
    SDL_assert(lsm->unit > TNECS_NULL);
    SDL_assert(lsm->world != NULL);

    Unit *unit          = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);
    i32 num_items       = unit->num_canEquip;
    b32 stronghand      = Unit_Hand_Strong(unit);
    b32 weakhand        = Unit_Hand_Weak(unit);
    b32 header_drawn    = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;

    b32 strong_selected = Loadout_isEquipped(&lsm->selected, stronghand);

    b32 skip_draw_left  = !strong_selected && (UNIT_HAND_LEFT   == weakhand);
    b32 skip_draw_right = !strong_selected && (UNIT_HAND_RIGHT  == weakhand);

    if (!skip_draw_left) {
        /* -- Left hand icon -- */
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

        // int left_hand_row = ((UNIT_HAND_LEFT == stronghand)
        // && strong_selected) ? Loadout_Eq(&lsm->selected, stronghand) : mc->elem;
        int left_hand_row = 0;
        if (UNIT_HAND_LEFT == stronghand) {
            // left hand is strong hand, selected first
            left_hand_row = strong_selected ? Loadout_Eq(&lsm->selected, stronghand) : mc->elem;
        } else {
            // if left hand is weakhand and strong is not selected should not be drawn
            left_hand_row = mc->elem;
        }

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

    if (!skip_draw_right) {
        /* -- Right hand icon -- */
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

        // int right_hand_row = ((UNIT_HAND_RIGHT == stronghand)
        // && strong_selected) ? Loadout_Eq(&lsm->selected, stronghand) : mc->elem;
        int right_hand_row = 0;
        if (UNIT_HAND_RIGHT == stronghand) {
            // left hand is strong hand, selected first
            right_hand_row = strong_selected ? Loadout_Eq(&lsm->selected, stronghand) : mc->elem;
        } else {
            // if left hand is weakhand and strong is not selected should not be drawn
            right_hand_row = mc->elem;
        }

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
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->unit    > TNECS_NULL);
    SDL_assert(lsm->world   != NULL);

    /* -- Preliminaries -- */
    b32 header_drawn = (lsm->header.data != NULL);
    SDL_Rect srcrect, dstrect;
    char numbuff[10];

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    Unit *unit      = TNECS_GET_COMPONENT(lsm->world, lsm->unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Strong(unit);
    i32 num_items   = unit->num_canEquip;
    // b32 highlight  = (lsm->selected[0] >= 0);
    b32 highlight  = false;

    /* TODO: If stronghand is selected, menu should change to show all items in equipment */
    b32 strong_selected = Loadout_isEquipped(&lsm->selected, stronghand);

    SDL_assert(unit->num_canEquip > 0);
    SDL_assert(unit->num_canEquip <= SOTA_EQUIPMENT_SIZE);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;

    for (i32 i = 0; i < unit->num_canEquip; i++) {
        /* - Icons - */
        // TODO: weapon icons images.
        i32 eq = unit->eq_canEquip[i];
        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 id = Unit_Id_Equipment(unit, eq);
        struct Inventory_item *item = Unit_InvItem(unit, eq);

        /* Icons, text drawn on line strong_i  */

        /* -- Weapon icon -- */
        srcrect.x = LSM1_X_OFFSET;
        srcrect.y = LSM1_Y_OFFSET + i * (LSM_ICON_H + 2) + (header_drawn * LSM_ROW_HEIGHT);

        SDL_RenderFillRect(renderer, &srcrect);

        /* -- Weapon name -- */
        if ((eq == stronghand) && highlight) {
            /* Change black only to grey */
            PixelFont_Swap_Palette(lsm->pixelnours,     renderer, -1, 2);
            PixelFont_Swap_Palette(lsm->pixelnours_big, renderer, -1, 2);
        }

        /* - Write '-' if no weapon - */
        i32 item_x_offset = LSM1_NAME_X_OFFSET;
        i32 item_y_offset = LSM1_NAME_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                            (header_drawn * LSM_ROW_HEIGHT);


        if ((id == ITEM_NULL) || !Weapon_ID_isValid(id)) {
            PixelFont_Write(lsm->pixelnours, renderer, "-", 1, item_x_offset, item_y_offset);
            continue;
        }

        SDL_assert(unit->weapons_dtab != NULL);
        struct Weapon *weapon = DTAB_GET(unit->weapons_dtab, id);
        SDL_assert(weapon != NULL);

        /* - Uses left - */
        i32 item_dura_x_offset = LSM1_DURA_X_OFFSET;
        i32 item_dura_y_offset = LSM1_DURA_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                                 (header_drawn * LSM_ROW_HEIGHT);

        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 uses_left = weapon->item->stats.uses - item->used;
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

        i32 dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;

        PixelFont_Write(lsm->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        item_dura_x_offset,  item_dura_y_offset);

        /* - Weapon name - */
        lsm->item_name = s8cpy(lsm->item_name, Unit_Name(id));
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
