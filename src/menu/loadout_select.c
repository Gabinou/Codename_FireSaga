/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** LoadoutSelectMenu: Menu to equip weapons, items.
**
*/

#include "item.h"
#include "nmath.h"
#include "names.h"
#include "macros.h"
#include "weapon.h"
#include "globals.h"
#include "platform.h"
#include "utilities.h"
#include "filesystem.h"
#include "pixelfonts.h"

#include "map/map.h"

#include "menu/stats.h"
#include "menu/loadout_select.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

#include "stb_sprintf.h"

const struct Point wsm_cursor_pos[LSM_ELEMS_NUM] = {
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

const struct n4Directions wsm_links_start[LSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* LSM_ELEM_ITEM1 */ {LSM_ELEM_NULL, LSM_ELEM_NULL,  LSM_ELEM_NULL, LSM_ELEM_ITEM2},
    /* LSM_ELEM_ITEM2 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM1, LSM_ELEM_NULL, LSM_ELEM_ITEM3},
    /* LSM_ELEM_ITEM3 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM2, LSM_ELEM_NULL, LSM_ELEM_ITEM4},
    /* LSM_ELEM_ITEM4 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM3, LSM_ELEM_NULL, LSM_ELEM_ITEM5},
    /* LSM_ELEM_ITEM5 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM4, LSM_ELEM_NULL, LSM_ELEM_ITEM6},
    /* LSM_ELEM_ITEM6 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM5, LSM_ELEM_NULL, LSM_ELEM_NULL},
};

struct n4Directions wsm_links[LSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* LSM_ELEM_ITEM1 */ {LSM_ELEM_NULL, LSM_ELEM_NULL,  LSM_ELEM_NULL, LSM_ELEM_ITEM2},
    /* LSM_ELEM_ITEM2 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM1, LSM_ELEM_NULL, LSM_ELEM_ITEM3},
    /* LSM_ELEM_ITEM3 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM2, LSM_ELEM_NULL, LSM_ELEM_ITEM4},
    /* LSM_ELEM_ITEM4 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM3, LSM_ELEM_NULL, LSM_ELEM_ITEM5},
    /* LSM_ELEM_ITEM5 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM4, LSM_ELEM_NULL, LSM_ELEM_ITEM6},
    /* LSM_ELEM_ITEM6 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM5, LSM_ELEM_NULL, LSM_ELEM_NULL},
};

const struct LoadoutSelectMenu LoadoutSelectMenu_default = {
    .update                 = true,
    .archetype_stronghand   = ITEM_ARCHETYPE_STRONGHAND_ATTACK,
    .archetype_weakhand     = ITEM_ARCHETYPE_WEAKHAND,
    .black                  = SOTA_BLACK,
    .white                  = SOTA_WHITE,
};

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static void _LoadoutSelectMenu_Draw_Names(      LoadoutSelectMenu   *l,
                                                SDL_Renderer        *r);
static void _LoadoutSelectMenu_Draw_Hands(      Menu                *m,
                                                LoadoutSelectMenu   *l,
                                                SDL_Renderer        *r);
static void _LoadoutSelectMenu_Draw_Header(     LoadoutSelectMenu   *l,
                                                SDL_Renderer        *r);
static void _LoadoutSelectMenu_Draw_Highlight(  LoadoutSelectMenu   *l,
                                                SDL_Renderer        *r);

/* --- Constructors/Destructors --- */
struct LoadoutSelectMenu *LoadoutSelectMenu_Alloc(void) {
    struct LoadoutSelectMenu *lsm = SDL_malloc(sizeof(struct LoadoutSelectMenu));
    SDL_assert(lsm != NULL);
    *lsm = LoadoutSelectMenu_default;
    SDL_assert(lsm->texture == NULL);

    if (lsm->texture_hands != NULL) {
        SDL_DestroyTexture(lsm->texture_hands);
        lsm->texture_hands = NULL;
    }

    return (lsm);
}

void LoadoutSelectMenu_Free(LoadoutSelectMenu *lsm) {
    if (lsm == NULL) {
        return;
    }

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

    SDL_free(lsm);
}

void WeaponSelectMenu_Load_n9Patch( struct LoadoutSelectMenu *lsm,
                                    SDL_Renderer *r,
                                    struct n9Patch *n9patch)  {
    SDL_assert(n9patch != NULL);
    n9patch->px.x  = MENU_PATCH_PIXELS;
    n9patch->px.y  = MENU_PATCH_PIXELS;
    n9patch->num.x  = LSM_PATCH_X_SIZE;
    n9patch->num.y  = LSM_PATCH_X_SIZE;
    n9patch->scale.x         = LSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = LSM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * LSM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * LSM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);
}

void WeaponSelectMenu_Load( LoadoutSelectMenu   *lsm,
                            Map                 *map,
                            SDL_Renderer        *renderer,
                            n9Patch             *n9patch) {
    /* Load n9patch sizes and textures */
    WeaponSelectMenu_Load_n9Patch(lsm, renderer, n9patch);
    LoadoutSelectMenu_Load(lsm, map, renderer, n9patch);
}

void StaffSelectMenu_Load(  struct LoadoutSelectMenu    *lsm,
                            struct Map                  *map,
                            SDL_Renderer                *renderer,
                            struct n9Patch              *n9patch) {
    SDL_assert(n9patch != NULL);
    n9patch->px.x  = MENU_PATCH_PIXELS;
    n9patch->px.y  = MENU_PATCH_PIXELS;
    n9patch->num.x  = LSM_PATCH_X_SIZE;
    n9patch->num.y  = LSM_PATCH_Y_SIZE;
    n9patch->scale.x         = LSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = LSM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * LSM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * LSM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    lsm->archetype_stronghand = ITEM_ARCHETYPE_STRONGHAND_STAFF;
    LoadoutSelectMenu_Load(lsm, map, renderer, n9patch);
}

void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *lsm,
                            struct Map *map,
                            SDL_Renderer *renderer,
                            struct n9Patch *n9patch) {
    SDL_assert(lsm      != NULL);
    SDL_assert(gl_world != NULL);
    SDL_assert(map      != NULL);
    SDL_assert(n9patch  != NULL);
    lsm->map    = map;

    if (lsm->texture_hands == NULL) {
        char *path = PATH_JOIN("assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
        lsm->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(lsm->texture_hands != NULL);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);
}

/* --- Elem Move --- */
i32 WeaponSelectMenu_Elem_Move(struct Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

/* --- Elements --- */
void LoadoutSelectMenu_Elem_Reset(  LoadoutSelectMenu   *lsm,
                                    Menu                *mc) {
    /* Get number of elements for the menu */
    SDL_assert(lsm        != NULL);
    SDL_assert(gl_world   != NULL);

    mc->elem_num    = lsm->equippable.num;
    size_t bytesize = sizeof(*wsm_links_start) * LSM_ELEMS_NUM;
    memcpy(mc->elem_links, wsm_links_start, bytesize);

    mc->elem_links[0].top                   = LSM_ELEM_NULL;
    mc->elem_links[mc->elem_num - 1].bottom = LSM_ELEM_NULL;
}

void LoadoutSelectMenu_Elem_Pos(LoadoutSelectMenu   *lsm,
                                Menu                *mc) {
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
    struct Unit *unit = IES_GET_C(gl_world, lsm->_unit, Unit);

    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    if (!Loadout_isEquipped(&lsm->selected, stronghand) &&
        !Loadout_isEquipped(&lsm->selected, weakhand)) {
        /* No weapon was selected, there SHOULD be a weapon remaining */
        remains = lsm->equippable.num > 0;
        SDL_assert(remains == true);
    } else if (Loadout_isEquipped(&lsm->selected, stronghand) &&
               !Loadout_isEquipped(&lsm->selected, weakhand)) {
        /* After first weapon was selected */
        remains = lsm->equippable.num > 0;
    }

    return (remains);
}
/* --- CanEquip --- */
void LoadoutSelectMenu_canEquip(struct LoadoutSelectMenu *lsm) {
    // If no hand selected: Map_canEquip of archetype, without movement, any loadout
    // If any hand selected:  Map_canEquip of archetype, without movement, selected loadout
}

/* --- Item placement --- */
void LoadoutSelectMenu_Unit(LoadoutSelectMenu *lsm,
                            tnecs_E ent) {
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->map     != NULL);
    SDL_assert(gl_world     != NULL);
    SDL_assert(ent  > TNECS_NULL);
    lsm->_unit = ent;
    Unit *unit = IES_GET_C(gl_world, ent, Unit);
    Unit_Loadout_Export(unit, &lsm->initial);
    lsm->update  = true;
}

void LoadoutSelectMenu_Select_Reset(LoadoutSelectMenu *lsm) {
    for (i32 hand = UNIT_HAND_LEFT; hand <= MAX_ARMS_NUM; hand++) {
        Loadout_None(&lsm->selected, hand);
    }
    lsm->update = true;
}

/* - Select Weapon/Staff - */
void LoadoutSelectMenu_Select(  LoadoutSelectMenu *lsm,
                                i32 select) {
    SDL_assert(lsm          != NULL);
    SDL_assert(gl_world   != NULL);
    SDL_assert(lsm->_unit    > TNECS_NULL);
    /* Player just selected loadout, equip it */

    /* - Equip weapons according to player choice - */
    Unit *unit      = IES_GET_C(gl_world, lsm->_unit, Unit);
    i32 eq          = lsm->equippable.arr[select];
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    // There should be always be usable weapons
    SDL_assert(select < lsm->equippable.num);

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

void LoadoutSelectMenu_Deselect(struct LoadoutSelectMenu *lsm) {
    /* -- Revert selected item -- */
    SDL_assert(lsm       != NULL);
    SDL_assert(lsm->_unit > TNECS_NULL);
    lsm->update           = true;

    /*- Get the tophand -*/
    Unit *unit      = IES_GET_C(gl_world, lsm->_unit, Unit);
    SDL_assert(unit       != NULL);
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    /*- Skip if no item to revert -*/
    if (!Loadout_isEquipped(&lsm->selected, stronghand) &&
        !Loadout_isEquipped(&lsm->selected, weakhand)) {
        SDL_Log("Warning: No item to deselect");
        return;
    } else if (Loadout_isEquipped(&lsm->selected, weakhand)) {
        Loadout_None(&lsm->selected, weakhand);
    } else if (Loadout_isEquipped(&lsm->selected, stronghand)) {
        Loadout_None(&lsm->selected, stronghand);
    }
}

/* --- Drawing --- */
void LoadoutSelectMenu_Size(LoadoutSelectMenu   *lsm,
                            n9Patch             *n9patch) {
    /* - Compute new menu width and height - */
    Unit *unit  = IES_GET_C(gl_world, lsm->_unit, Unit);
    i32 width, max_width = LSM_ITEM_MIN_WIDTH;
    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    i32 stronghand = Unit_Hand_Strong(unit);

    /* If stronghand is selected, menu should change to show all items in equipment */
    b32 strong_selected = Loadout_isEquipped(&lsm->selected,
                                             stronghand);
    i32 num_items = lsm->equippable.num;

    for (i32 i = 0; i < num_items; i++) {
        i32 eq = strong_selected ? i + ITEM1 : lsm->equippable.arr[i];
        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 id = Unit_Id_Equipment(unit, eq);

        /* unit_item ensures tophand is stronghand */
        // struct InvItem *item = Unit_InvItem(unit, eq);

        if (id == ITEM_NULL)
            continue;

        s8_free(&lsm->item_name);
        Item_Load(id);
        const Item *item = _Item_Get(id);
        lsm->item_name = s8_mut(Item_Name(item->ids.id).data);

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
    i32 patch_size_x  = size_raw_x   / n9patch->px.x;
    lsm->menu_w       = patch_size_x * n9patch->px.x;

    i32 size_raw_y    = num_items * LSM_ROW_HEIGHT + LSM_TOP_OF_TEXT + LSM_BOTTOM_OF_TEXT + header_drawn
                        * LSM_ROW_HEIGHT;
    i32 patch_size_y  = size_raw_y   / n9patch->px.y;
    lsm->menu_h       = patch_size_y * n9patch->px.y;

    /* -- Check if new menu texture should be created if menu size changed -- */
    Point size = n9Patch_Pixels_Total(n9patch);


    if ((size.x != lsm->menu_w) || (size.y != lsm->menu_h)) {
        Point size = {
            .x  = lsm->menu_w,
            .y  = lsm->menu_h,
        };
        n9Patch_Pixels_Total_Set(n9patch, size);

        /* n9patch->num.x = patch_size_x; */
        /* n9patch->num.y = patch_size_y; */
        if (lsm->texture != NULL) {
            SDL_DestroyTexture(lsm->texture);
            lsm->texture = NULL;
        }
    }
}

void LoadoutSelectMenu_Draw(Menu            *mc,
                            SDL_Texture     *target,
                            SDL_Renderer    *renderer) {
    LoadoutSelectMenu *lsm = (LoadoutSelectMenu *)mc->data;
    SDL_assert(lsm != NULL);
    SDL_assert(lsm->_unit > TNECS_NULL);
    SDL_assert(gl_world != NULL);
    struct n9Patch *n9patch = &mc->n9patch;

    if (lsm->update) {
        LoadoutSelectMenu_Update(mc, lsm, n9patch, target, renderer);
        lsm->update = false;
    }
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = lsm->pos.x,
        .y = lsm->pos.y,
    };
    SDL_assert(lsm->texture != NULL);
    SDL_RenderCopy(renderer, lsm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void LoadoutSelectMenu_Header_Set(  struct LoadoutSelectMenu *lsm,
                                    char *header) {
    s8_free(&lsm->header);
    lsm->header = s8_mut(header);
}

static void _LoadoutSelectMenu_Draw_Header(struct LoadoutSelectMenu *lsm,
                                           SDL_Renderer      *renderer) {
    /* Skip if no header to draw */
    if (lsm->header.data == NULL)
        return;

    /* Draw textual header: information to the player */
    // i32 dura_w = PixelFont_Width(lsm->pixelnours_big, lsm->header.data, lsm->header.num);
    PixelFont_In pxin = {
        .renderer   = renderer,
        .text       = lsm->header.data,
        .len        = lsm->header.num,
        .pos        =  {
            .x = LSM_HEADER_LEFT,
            .y = LSM_TOP_OF_TEXT
        }
    };
    PixelFont_Write(lsm->pixelnours_big, pxin);
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
//     i32 stronghand = Unit_Hand_Strong(lsm->_unit);

//     SDL_assert(lsm->_unit->gl_weapons_dtab != NULL);
//     struct InvItem *item   = Unit_Item_Equipped(lsm->_unit, Unit_Hand_Strong(lsm->_unit));
//     SDL_Log("item->id %d", item->id);
//     Weapon_Load(lsm->_unit->gl_weapons_dtab, item->id);
//     struct Weapon         *weapon = DTAB_GET(lsm->_unit->gl_weapons_dtab, item->id);
//     SDL_assert(weapon               != NULL);
//     SDL_assert(weapon.item         != NULL);
//     s8 name = weapon->item.name;
//     srcrect.w  = PixelFont_Width(lsm->pixelnours, s8_toUpper(name).data, name.num);
//     i32 uses_left = weapon->item.stats.uses - item->used;
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

static void _LoadoutSelectMenu_Draw_Hands(Menu              *mc,
                                          LoadoutSelectMenu *lsm,
                                          SDL_Renderer      *renderer) {
    /* -- Preliminaries -- */
    SDL_assert(lsm != NULL);
    SDL_assert(lsm->_unit > TNECS_NULL);
    SDL_assert(gl_world != NULL);

    Unit *unit          = IES_GET_C(gl_world, lsm->_unit, Unit);
    // i32 num_items       = lsm->equippable.num;
    b32 stronghand      = Unit_Hand_Strong(unit);
    b32 weakhand        = Unit_Hand_Weak(unit);
    b32 header_drawn    = (lsm->header.data != NULL);
    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {0};

    b32 strong_selected = Loadout_isEquipped(&lsm->selected, stronghand);

    b32 skip_draw_left  = !strong_selected && (UNIT_HAND_LEFT   == weakhand);
    b32 skip_draw_right = !strong_selected && (UNIT_HAND_RIGHT  == weakhand);

    do {
        if (skip_draw_left) {
            break;
        }

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
        int left_hand_row = mc->elem;
        if ((UNIT_HAND_LEFT == stronghand) && strong_selected) {
            // left hand is strong hand, selected first

            // Find if eq is in CanEquip weapons. If so, put hand there.
            i32 order = Unit_Order_canEquip(&lsm->equippable, Loadout_Eq(&lsm->selected, stronghand));
            if (!eq_valid(order)) {
                // If not, don't draw hand.
                break;
            }
            left_hand_row = order;
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
    } while (false);

    do {
        if (skip_draw_right) {
            break;
        }

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
        int right_hand_row = mc->elem;
        if ((UNIT_HAND_RIGHT == stronghand) && strong_selected) {
            // right hand is strong hand, was selected
            // SDL_Log("right hand is strong hand, was selected");

            // Find if eq is in CanEquip weapons. If so, put hand there.
            i32 order = Unit_Order_canEquip(&lsm->equippable, Loadout_Eq(&lsm->selected, stronghand));
            if (!eq_valid(order)) {
                // If not, don't draw hand.
                break;
            }
            right_hand_row = order;
        }

        /* Computing y offset for weak hand, or twohanding icon placement */
        int ry_offset = (stronghand == UNIT_HAND_RIGHT) ? LSM_STRONGHAND_Y_OFFSET : LSM_WEAKHAND_Y_OFFSET;

        dstrect.x = lsm->menu_w * 9 / 10;
        dstrect.y = ry_offset + (header_drawn + right_hand_row) * LSM_ROW_HEIGHT;

        // /* Moving hand if small */
        if (stronghand != UNIT_HAND_RIGHT) {
            dstrect.x += LSM_HAND_SMALLX_OFFSET;
            dstrect.y += LSM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, lsm->texture_hands, &srcrect, &dstrect);
    } while (false);
}

static void _LoadoutSelectMenu_Draw_Names(
        LoadoutSelectMenu  *lsm,
        SDL_Renderer       *renderer) {
    SDL_assert(lsm          != NULL);
    SDL_assert(lsm->_unit    > TNECS_NULL);
    SDL_assert(gl_world     != NULL);

    /* -- Preliminaries -- */
    b32 header_drawn = (lsm->header.data != NULL);
    SDL_Rect srcrect;
    char numbuff[10];

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    Unit *unit      = IES_GET_C(gl_world, lsm->_unit, Unit);
    i32 stronghand  = Unit_Hand_Strong(unit);
    b32 highlight   = false;

    /* TODO: If stronghand is selected, menu should change to show all items in equipment */
    // b32 strong_selected = Loadout_isEquipped(&lsm->selected, stronghand);

    SDL_assert(lsm->equippable.num > 0);
    SDL_assert(lsm->equippable.num <= EQM_SIZE);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;

    for (i32 i = 0; i < lsm->equippable.num; i++) {
        /* - Icons - */
        i32 eq = lsm->equippable.arr[i];
        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 id = Unit_Id_Equipment(unit, eq);
        InvItem *item = Unit_InvItem(unit, eq);

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
        i32 item_y_offset = LSM1_NAME_Y_OFFSET +
                            i * (ITEM_ICON_H + 2) +
                            (header_drawn * LSM_ROW_HEIGHT);

        if ((id == ITEM_NULL) || !Weapon_ID_isValid(id)) {
            PixelFont_In pxin = {
                .renderer   = renderer,
                .text       = "-",
                .len        = 1,
                .pos        =  {
                    .x = item_x_offset,
                    .y = item_y_offset
                }
            };

            PixelFont_Write(lsm->pixelnours, pxin);
            continue;
        }

        SDL_assert(gl_weapons_dtab != NULL);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        SDL_assert(weapon != NULL);

        /* - Uses left - */
        i32 item_dura_x_offset = LSM1_DURA_X_OFFSET;
        i32 item_dura_y_offset = LSM1_DURA_Y_OFFSET +
                                 i * (ITEM_ICON_H + 2) +
                                 (header_drawn * LSM_ROW_HEIGHT);

        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 uses_left = weapon->item.stats.uses - item->used;
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

        i32 dura_w = PixelFont_Width(lsm->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;

        PixelFont_In pxin = {
            .renderer   = renderer,
            .text       =   numbuff,
            .pos        =  {
                .x = item_dura_x_offset,
                .y = item_dura_y_offset
            }
        };
        PixelFont_Write(lsm->pixelnours_big, pxin);

        /* - Weapon name - */
        lsm->item_name = s8cpy(lsm->item_name, Item_Name(id));
        lsm->item_name = s8_toUpper(lsm->item_name);
        // i32 name_w = PixelFont_Width(lsm->pixelnours, lsm->item_name.data, lsm->item_name.num);

        pxin.text   = lsm->item_name.data;
        pxin.len    = lsm->item_name.num;
        pxin.pos.x  = item_x_offset;
        pxin.pos.y  = item_y_offset;
        PixelFont_Write(lsm->pixelnours, pxin);
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

    // TODO: draw weapon icons.
    _LoadoutSelectMenu_Draw_Header(lsm, renderer);
    // _LoadoutSelectMenu_Draw_Highlight(lsm, renderer);
    _LoadoutSelectMenu_Draw_Hands(mc, lsm, renderer);
    _LoadoutSelectMenu_Draw_Names(lsm, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}
