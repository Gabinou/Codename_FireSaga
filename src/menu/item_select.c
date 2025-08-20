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
#include "menu/loadout_select.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

#include "stb_sprintf.h"

const ItemSelectMenu ItemSelectMenu_default = {
    .update                 = true,
    .black                  = SOTA_BLACK,
    .white                  = SOTA_WHITE,
};


/* --- Constructors/Destructors --- */
ItemSelectMenu *ItemSelectMenu_Alloc(void) {
    ItemSelectMenu *ism = SDL_malloc(sizeof(ItemSelectMenu));
    SDL_assert(ism != NULL);
    *ism = ItemSelectMenu_default;
    SDL_assert(ism->texture == NULL);

    if (ism->texture_hands != NULL) {
        SDL_DestroyTexture(ism->texture_hands);
        ism->texture_hands = NULL;
    }

    return (ism);
}

/* --- Elem Move --- */
i32 ItemSelectMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move( mc, direction, 0,
                                SOTA_EQUIPMENT_SIZE));
}

void ItemSelectMenu_Load(   ItemSelectMenu  *ism,
                            Map             *map,
                            SDL_Renderer    *renderer,
                            n9Patch         *n9patch) {
    /* Load n9patch sizes and textures */
    WeaponSelectMenu_Load_n9Patch(ism, renderer, n9patch);
    ItemSelectMenu_Load(ism, map, renderer, n9patch);
}

void ItemSelectMenu_Select( ItemSelectMenu *ism,
                            i32 select) {
    Loadout_Set(&ism->selected, UNIT_HAND_LEFT, select);
}

i32 ItemSelectMenu_Selected( ItemSelectMenu *ism) {
    return (Loadout_Eq(&ism->selected, UNIT_HAND_LEFT));
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
        ItemSelectMenu_Update(mc, ism, n9patch, target, renderer);
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

static void _ItemSelectMenu_Draw_Hands( Menu            *mc,
                                        ItemSelectMenu  *ism,
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

    b32 strong_selected = Loadout_isEquipped(&ism->selected, stronghand);

    b32 skip_draw_left  = !strong_selected && (UNIT_HAND_LEFT   == weakhand);
    b32 skip_draw_right = !strong_selected && (UNIT_HAND_RIGHT  == weakhand);

    do {
        if (skip_draw_left) {
            break;
        }

        /* -- Left hand icon -- */
        srcrect.w = ISM_HANDS_TILESIZE;
        srcrect.h = ISM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        // 1. Follows cursor when selecting for hand
        // 2. Is at selected item position if stronghnad
        int hand_i = (weakhand == UNIT_HAND_LEFT) ? ISM_HANDS_SMALL_L : ISM_HANDS_BIG_L;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = ISM_HANDL_X;

        // int left_hand_row = ((UNIT_HAND_LEFT == stronghand)
        // && strong_selected) ? Loadout_Eq(&ism->selected, stronghand) : mc->elem;
        int left_hand_row = mc->elem;
        if ((UNIT_HAND_LEFT == stronghand) && strong_selected) {
            // left hand is strong hand, selected first

            // Find if eq is in CanEquip weapons. If so, put hand there.
            i32 order = Unit_Order_canEquip(&ism->equippable, Loadout_Eq(&ism->selected, stronghand));
            if (!eq_valid(order)) {
                // If not, don't draw hand.
                break;
            }
            left_hand_row = order;
        }

        /* Computing y offset for weak hand, or twohanding icon placement */
        i32 ly_offset = (stronghand == UNIT_HAND_RIGHT) ? ISM_WEAKHAND_Y_OFFSET : ISM_STRONGHAND_Y_OFFSET;

        dstrect.y = ly_offset + left_hand_row * ISM_ROW_HEIGHT;

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_LEFT) {
            dstrect.x += ISM_HAND_SMALLX_OFFSET;
            dstrect.y += ISM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, ism->texture_hands, &srcrect, &dstrect);
    } while (false);

    do {
        if (skip_draw_right) {
            break;
        }

        /* -- Right hand icon -- */
        srcrect.w = ISM_HANDS_TILESIZE;
        srcrect.h = ISM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        // 1. Follows cursor when selecting for hand
        // 2. Is at selected item position if stronghnad
        int hand_i = (stronghand == UNIT_HAND_LEFT) ? ISM_HANDS_SMALL_R : ISM_HANDS_BIG_R;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        int right_hand_row = mc->elem;
        if ((UNIT_HAND_RIGHT == stronghand) && strong_selected) {
            // right hand is strong hand, was selected
            // SDL_Log("right hand is strong hand, was selected");

            // Find if eq is in CanEquip weapons. If so, put hand there.
            i32 order = Unit_Order_canEquip(&ism->equippable, Loadout_Eq(&ism->selected, stronghand));
            if (!eq_valid(order)) {
                // If not, don't draw hand.
                break;
            }
            right_hand_row = order;
        }

        /* Computing y offset for weak hand, or twohanding icon placement */
        int ry_offset = (stronghand == UNIT_HAND_RIGHT) ? ISM_STRONGHAND_Y_OFFSET : ISM_WEAKHAND_Y_OFFSET;

        dstrect.x = ism->menu_w * 9 / 10;
        dstrect.y = ry_offset + right_hand_row * ISM_ROW_HEIGHT;

        // /* Moving hand if small */
        if (stronghand != UNIT_HAND_RIGHT) {
            dstrect.x += ISM_HAND_SMALLX_OFFSET;
            dstrect.y += ISM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, ism->texture_hands, &srcrect, &dstrect);
    } while (false);
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
        struct Inventory_item *item = Unit_InvItem(unit, eq);

        /* Icons, text drawn on line strong_i  */

        /* -- Item icon -- */
        srcrect.x = ISM1_X_OFFSET;
        srcrect.y = ISM1_Y_OFFSET + i * (ISM_ICON_H + 2) + ISM_ROW_HEIGHT;

        SDL_RenderFillRect(renderer, &srcrect);

        /* -- Weapon name -- */
        b32 can_use     = Unit_canUse_Item(item, unit);
        b32 could_use   = Item_couldbeUsed(item);


        b32 grey = false;

        if (grey) {
            /* Change black only to grey */
            PixelFont_Swap_Palette(ism->pixelnours,     renderer, -1, 2);
            PixelFont_Swap_Palette(ism->pixelnours_big, renderer, -1, 2);
        }

        /* - Write '-' if no weapon - */
        i32 item_x_offset = ISM1_NAME_X_OFFSET;
        i32 item_y_offset = ISM1_NAME_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                            ISM_ROW_HEIGHT;


        if ((id == ITEM_NULL) || !Weapon_ID_isValid(id)) {
            PixelFont_Write(ism->pixelnours, renderer, "-", 1, item_x_offset, item_y_offset);
            continue;
        }

        SDL_assert(gl_weapons_dtab != NULL);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, id);
        SDL_assert(weapon != NULL);

        /* - Uses left - */
        i32 item_dura_x_offset = ISM1_DURA_X_OFFSET;
        i32 item_dura_y_offset = ISM1_DURA_Y_OFFSET + i * (ITEM_ICON_H + 2) +
                                 ISM_ROW_HEIGHT;

        SDL_assert((eq >= ITEM1) && (eq <= ITEM6));
        i32 uses_left = weapon->item.stats.uses - item->used;
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses_left);

        i32 dura_w = PixelFont_Width(ism->pixelnours_big, numbuff, strlen(numbuff));
        item_dura_x_offset -= dura_w / 2;

        PixelFont_Write(ism->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        item_dura_x_offset,  item_dura_y_offset);

        /* - Weapon name - */
        ism->item_name = s8cpy(ism->item_name, Item_Name(id));
        ism->item_name = s8_toUpper(ism->item_name);
        // i32 name_w = PixelFont_Width(ism->pixelnours, ism->item_name.data, ism->item_name.num);
        PixelFont_Write(ism->pixelnours, renderer, ism->item_name.data, ism->item_name.num, item_x_offset,
                        item_y_offset);
    }

    /* Reset colors */
    PixelFont_Swap_Palette(ism->pixelnours,     renderer, ism->white, ism->black);
    PixelFont_Swap_Palette(ism->pixelnours_big, renderer, ism->white, ism->black);
    Utilities_DrawColor_Reset(renderer);
}

void ItemSelectMenu_Texture_Create( ItemSelectMenu  *ism,
                                    n9Patch         *n9patch,
                                    SDL_Renderer    *renderer) {
    /* -- Compute menu size -- */
    ItemSelectMenu_Size(ism, n9patch);

    /* -- Create new texture -- */
    if (ism->texture == NULL) {
        ism->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, ism->menu_w, ism->menu_h);
        SDL_assert(ism->texture != NULL);
        SDL_SetTextureBlendMode(ism->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_assert(ism->texture != NULL);
}

void ItemSelectMenu_Update( Menu            *mc,
                            ItemSelectMenu  *ism,
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

    _ItemSelectMenu_Draw_Hands(mc, ism, renderer);
    _ItemSelectMenu_Draw_Items(ism, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}
