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
** ItemSelectMenu: Menu to select ANY item
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
#include "menu/item_select.h"
#include "menu/loadout_select.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

#include "stb_sprintf.h"

const ItemSelectMenu ItemSelectMenu_default = {
    .update                 = true,
    .black                  = SOTA_BLACK,
    .white                  = SOTA_WHITE
};

/* --- Constructors/Destructors --- */
ItemSelectMenu *ItemSelectMenu_Alloc(void) {
    ItemSelectMenu *ism = SDL_malloc(sizeof(ItemSelectMenu));
    SDL_assert(ism != NULL);
    *ism = ItemSelectMenu_default;
    SDL_assert(ism->texture         == NULL);
    SDL_assert(ism->texture_hands   == NULL);

    return (ism);
}

void ItemSelectMenu_Free(ItemSelectMenu *ism) {
    if (ism == NULL) {
        return;
    }

    if (ism->texture_hands != NULL) {
        SDL_DestroyTexture(ism->texture_hands);
        ism->texture_hands = NULL;
    }
    if (ism->texture != NULL) {
        SDL_DestroyTexture(ism->texture);
        ism->texture = NULL;
    }

    SDL_free(ism);
}


/* --- Elem Move --- */
i32 ItemSelectMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move( mc, direction, 0,
                                SOTA_EQUIPMENT_SIZE));
}

void ItemSelectMenu_Load(   ItemSelectMenu  *ism,
                            SDL_Renderer    *renderer,
                            n9Patch         *n9patch) {
    /* Load n9patch sizes and textures */
    SDL_assert(n9patch != NULL);
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->size_patches.x  = ISM_PATCH_X_SIZE;
    n9patch->size_patches.y  = ISM_PATCH_X_SIZE;
    n9patch->scale.x         = ISM_N9PATCH_SCALE_X;
    n9patch->scale.y         = ISM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE;
    n9patch->size_pixels.y   = MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE;

    if (ism->texture_hands == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
        ism->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(ism->texture_hands != NULL);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);
}

void ItemSelectMenu_Select( ItemSelectMenu *ism,
                            i32 select) {
    SDL_assert(select >= ITEM1);
    SDL_assert(select <= ITEM6);
    ism->selected_eq = select + ITEM1;
}

i32 ItemSelectMenu_Selected( ItemSelectMenu *ism) {
    return (ism->selected_eq);
}

void ItemSelectMenu_Draw(   Menu            *mc,
                            SDL_Texture     *target,
                            SDL_Renderer    *renderer) {
    ItemSelectMenu *ism = (ItemSelectMenu *)mc->data;
    SDL_assert(ism != NULL);
    SDL_assert(ism->_unit > TNECS_NULL);
    SDL_assert(gl_world != NULL);
    struct n9Patch *n9patch = &mc->n9patch;
    ism->update           = true;

    /*- Get the tophand -*/
    // Unit *unit      = IES_GET_C(gl_world, ism->_unit, Unit);

    if (ism->update) {
        ItemSelectMenu_Update(ism, n9patch, target, renderer);
        ism->update = false;
    }

    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = ism->pos.x,
        .y = ism->pos.y,
    };
    SDL_assert(ism->texture != NULL);
    SDL_RenderCopy(renderer, ism->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void ItemSelectMenu_Size(   ItemSelectMenu  *ism,
                            n9Patch         *n9) {
    /* - Compute new menu width and height from unit - */
    SDL_assert(ism->_unit != TNECS_NULL);
    SDL_assert(gl_world != NULL);

    Unit *unit  = IES_GET_C(gl_world, ism->_unit, Unit);

    /* Find maximum width of menu  */
    i32 num = Unit_Equipment_Num(unit);
    i32 max_w = 0;
    for (i32 eq = ITEM1; eq < (num + ITEM1); eq++) {
        /* - Get width - */
        i32 id = Unit_Id_Equipment(unit, eq);
        SDL_assert(id > ITEM_ID_START);
        const s8 name = Item_Name(id);
        i32 w = PixelFont_Width(ism->pixelnours_big,
                                name.data, name.len);
        if (w > max_w) {
            max_w = w;
        }

    }
    /* Find maximum height of menu */
    i32 max_h = ISM_ROW_HEIGHT * num;

    /* Setting patch size */
    n9->size_pixels.x  = max_w;
    n9->size_pixels.y  = max_h;
    n9->size_patches.x = n9->size_pixels.x / n9->patch_pixels.x;
    n9->size_patches.y = n9->size_pixels.y / n9->patch_pixels.y;

    /* Rounding pixel size to nearest multiple of patch size */
    n9->size_pixels.x  = n9->size_patches.x * n9->patch_pixels.x;
    n9->size_pixels.y  = n9->size_patches.y * n9->patch_pixels.y;
}

static void _ItemSelectMenu_Draw_Hands( ItemSelectMenu  *ism,
                                        SDL_Renderer    *renderer) {
    /* -- Preliminaries -- */
    SDL_assert(ism != NULL);
    SDL_assert(ism->_unit > TNECS_NULL);
    SDL_assert(gl_world != NULL);

    Unit *unit          = IES_GET_C(gl_world, ism->_unit, Unit);
    // i32 num_items       = ism->equippable.num;
    b32 stronghand      = Unit_Hand_Strong(unit);
    b32 weakhand        = Unit_Hand_Weak(unit);
    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {0};

}

static void _ItemSelectMenu_Draw_Items( ItemSelectMenu  *ism,
                                        SDL_Renderer    *renderer) {
    SDL_assert(ism          != NULL);
    SDL_assert(ism->_unit    > TNECS_NULL);
    SDL_assert(gl_world     != NULL);

    /* -- Preliminaries -- */
    SDL_Rect srcrect = {0};
    char numbuff[10];

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    Unit *unit      = IES_GET_C(gl_world, ism->_unit, Unit);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = ITEM_ICON_W;
    srcrect.h = ITEM_ICON_H;

    i32 num = Unit_Equipment_Num(unit);
    for (i32 eq = ITEM1; eq < (num + ITEM1); eq++) {
        /* - Icons - */
        // TODO: weapon icons images.
        i32 id = Unit_Id_Equipment(unit, eq);
        /* Inventory_item *invitem = Unit_InvItem(unit, eq); */
        const Item *item = DTAB_GET_CONST(gl_items_dtab, id);
        /* Icons, text drawn on line strong_i  */

        /* -- Item icon: TODO -- */
        srcrect.x = ISM1_X_OFFSET;
        srcrect.y = ISM1_Y_OFFSET + (eq - ITEM1) * (ISM_ICON_H + 2) + ISM_ROW_HEIGHT;
        SDL_RenderFillRect(renderer, &srcrect);

        /* - Uses left - */
        i32 item_dura_x_offset = ISM1_DURA_X_OFFSET;
        i32 item_dura_y_offset = ISM1_DURA_Y_OFFSET + (eq - ITEM1) *
                                 (ITEM_ICON_H + 2) +
                                 ISM_ROW_HEIGHT;

        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        /* i32 uses_left = weapon->item.stats.uses - item->used; */
        /* stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left); */

        i32 dura_w = PixelFont_Width(ism->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;

        PixelFont_Write(ism->pixelnours_big, renderer,
                        numbuff, strlen(numbuff),
                        item_dura_x_offset,  item_dura_y_offset);

        /* -- Weapon name -- */
        SDL_assert(gl_weapons_dtab != NULL);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        SDL_assert(weapon != NULL);

        /* - Write '-' if no weapon - */
        i32 item_x_offset = ISM1_NAME_X_OFFSET;
        i32 item_y_offset = ISM1_NAME_Y_OFFSET +
                            (eq - ITEM1) * (ITEM_ICON_H + 2) +
                            ISM_ROW_HEIGHT;

        s8 item_name    = s8_mut(Item_Name(id).data);
        item_name       = s8_toUpper(item_name);
        // i32 name_w = PixelFont_Width(ism->pixelnours, ism->item_name.data, ism->item_name.num);
        PixelFont_Write(ism->pixelnours,    renderer,
                        item_name.data,     item_name.num,
                        item_x_offset,      item_y_offset);
        s8_free(&item_name);
    }

    /* Reset colors */
    Utilities_DrawColor_Reset(renderer);
}

void ItemSelectMenu_Texture_Create( ItemSelectMenu  *ism,
                                    n9Patch         *n9,
                                    SDL_Renderer    *renderer) {
    SDL_assert(renderer != NULL);
    /* -- Compute menu size -- */
    ItemSelectMenu_Size(ism, n9);

    /* -- Create new texture -- */
    if (ism->texture == NULL) {
        i32 x = n9->size_pixels.x;
        i32 y = n9->size_pixels.y;

        ism->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         x, y);
        SDL_assert(ism->texture != NULL);
        SDL_SetTextureBlendMode(ism->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_assert(ism->texture != NULL);
}

void ItemSelectMenu_Update( ItemSelectMenu  *ism,
                            n9Patch         *n9patch,
                            SDL_Texture     *target,
                            SDL_Renderer    *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(ism      != NULL);
    SDL_assert(renderer != NULL);
    ItemSelectMenu_Texture_Create(ism, n9patch, renderer);

    /* - Clear the texture. - */
    SDL_SetRenderTarget(renderer, ism->texture);
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

    _ItemSelectMenu_Draw_Hands(ism, renderer);
    _ItemSelectMenu_Draw_Items(ism, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}
