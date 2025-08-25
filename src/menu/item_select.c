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
    n9patch->px.x           = MENU_PATCH_PIXELS;
    n9patch->px.y           = MENU_PATCH_PIXELS;
    n9patch->num.x          = ISM_PATCH_X_SIZE;
    n9patch->num.y          = ISM_PATCH_Y_SIZE;
    n9patch->scale.x        = ISM_N9PATCH_SCALE_X;
    n9patch->scale.y        = ISM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

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
    Point size = n9Patch_Pixels_Total(n9patch);

    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
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
    /* TODO */
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

static void _ItemSelectMenu_Draw_Names( ItemSelectMenu  *ism,
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
        i32 i = eq - ITEM1;
        i32 id = Unit_Id_Equipment(unit, eq);

        /* -- Weapon name -- */
        if ((id == ITEM_NULL) ||
            !Weapon_ID_isValid(id) ||
            !Item_ID_isValid(id)) {
            /* invalid weapon. Should not happen. */
            PixelFont_Write(ism->pixelnours, renderer, "-", 1, item_x_offset, item_y_offset);
            continue;
        }

        s8 raw_name = Item_Name(id);
        s8 name     = s8_toUpper(s8cpy(name, raw_name));

        Point pos = {
            .x = ISM1_DURA_X_OFFSET,
            .y = ISM1_DURA_Y_OFFSET +
                i * (ITEM_ICON_H + 2)
        };

        PixelFont_Write(lsm->pixelnours, renderer, 
                        name.data,  name.num,
                        pos.x,      pos.y);

        /* -- Uses -- */
        pos.x = ISM1_DURA_X_OFFSET;
        pos.y = ISM1_DURA_Y_OFFSET + i * (ITEM_ICON_H + 2);


        const Inventory_item *invitem = Unit_InvItem(unit, eq);
        i32 uses = Item_Uses(id, invitem);
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses);

        i32 width = PixelFont_Width(lsm->pixelnours_big, 
                                    numbuff, 
                                    strlen(numbuff));
        pos.x -= dura_w / 2;

        PixelFont_Write(lsm->pixelnours_big, renderer, 
                        numbuff, strlen(numbuff),
                        pos.x,  pox.y);
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
        Point size = n9Patch_Pixels_Total(n9);
        SDL_assert(size.x > 0);
        SDL_assert(size.y > 0);

        ism->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         size.x, size.y);
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
    _ItemSelectMenu_Draw_Names(ism, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}

