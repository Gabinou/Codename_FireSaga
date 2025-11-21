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

void ItemSelectMenu_Unit(Menu *mc, tnecs_E unit_E) {
    /* --- Set Unit + set dynamic width, height --- */
    ItemSelectMenu *ism = mc->data;
    SDL_assert(ism);
    ism->_unit_E = unit_E;

    Unit *unit = IES_GET_C(gl_world, ism->_unit_E, Unit);
    SDL_assert(unit);

    /* Get number of items in equipment */
    ism->_num = Unit_Equipment_Num(unit);
    mc->elem_num = ism->_num - ITEM1;

    /* -- Compute maximal text width for equipment -- */
    i32 width = 0;
    ism->_max_width = 0;
    for (i32 eq = ITEM1; eq < (ism->_num + ITEM1); eq++) {
        i32 id = Unit_Id_Equipment(unit, eq);

        /* - Invalid weapon - */
        if ((id == ITEM_NULL) ||
            (!Weapon_ID_isValid(id) &&
             !Item_Pure_ID_isValid(id))) {
            /* Should not happen. */
            continue;
        }

        /* - Valid weapon - */
        s8 raw_name = Item_Name(id);
        s8 name = {0};
        name = s8cpy(name, raw_name);
        name = s8_toUpper(name);

        width = PixelFont_Width(ism->pixelnours_big,
                                name.data, name.num);

        if (ism->_max_width < width) {
            ism->_max_width = width;
        }
    }
    /* Add hands width */
    ism->_max_width +=  ISM_HANDS_TILESIZE * 2 +
                        ISM_SPACE * 4;

    /* Reset texture */
    if (ism->texture != NULL) {
        SDL_DestroyTexture(ism->texture);
        ism->texture = NULL;
    }
    ism->update = true;
}

/* --- Elem Move --- */
i32 ItemSelectMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

void ItemSelectMenu_Elem_Pos(   ItemSelectMenu  *ism,
                                Menu            *mc) {
    /* Scales elem_pos to menu size
    **  1. Makes the cursor focus on right place on the Screen
    **  2. Box lined are drawn in menu frame, making thinner lines   */

    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME)
        return;

    /* remove */

    /* Set elem_pos to positions on screen */
    for (size_t i = 0; i < mc->elem_num; i++) {
        i32 scale_x = mc->n9patch.scale.x;
        i32 scale_y = mc->n9patch.scale.y;
        i32 x       = ism->pos.x + mc->n9patch.pos.x;
        i32 y       = ism->pos.y;
        i32 elem_x  = mc->elem_pos[i].x;
        i32 elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = x + elem_x * scale_x;
        mc->elem_pos[i].y = y + elem_y * scale_y;
    }
    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;

}


void ItemSelectMenu_Load(   ItemSelectMenu  *ism,
                            SDL_Renderer    *renderer,
                            n9Patch         *n9patch) {

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

tnecs_E ItemSelectMenu_Select(  ItemSelectMenu  *ism,
                                i32              select) {
    SDL_assert(select >= 0);
    SDL_assert(select <= SOTA_EQUIPMENT_SIZE);
    ism->selected_eq = select + ITEM1;
    return (ItemSelectMenu_Selected(ism));
}

tnecs_E ItemSelectMenu_Selected(ItemSelectMenu *ism) {
    SDL_assert(ism != NULL);
    SDL_assert(ism->_unit_E > TNECS_NULL);
    SDL_assert(gl_world != NULL);
    Unit *unit = IES_GET_C(gl_world, ism->_unit_E, Unit);
    return (Unit_InvItem_Entity(unit, ism->selected_eq));
}

void ItemSelectMenu_Size(   ItemSelectMenu  *ism,
                            n9Patch         *n9) {
    /* Compute size from max size measured */
    SDL_assert(ism != NULL);

    SDL_assert(n9 != NULL);
    n9->px.x           = MENU_PATCH_PIXELS;
    n9->px.y           = MENU_PATCH_PIXELS;
    n9->scale.x        = ISM_N9PATCH_SCALE_X;
    n9->scale.y        = ISM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    /* Skip if unit was not set */
    if ((ism->_max_width <= 0) ||
        (ism->_unit_E == TNECS_NULL)
       ) {
        return;
    }
    size.x  = ism->_max_width;
    size.y  = (ism->_num - ITEM1) * ISM_ROW_HEIGHT + ISM1_Y_OFFSET;
    n9Patch_Pixels_Total_Set(n9, size);
    n9Patch_Fit(n9, size);
}

void ItemSelectMenu_Draw(   Menu            *mc,
                            SDL_Texture     *target,
                            SDL_Renderer    *renderer) {
    ItemSelectMenu *ism = (ItemSelectMenu *)mc->data;
    SDL_assert(ism != NULL);
    SDL_assert(ism->_unit_E > TNECS_NULL);
    SDL_assert(gl_world != NULL);
    struct n9Patch *n9patch = &mc->n9patch;

    if (ism->update) {
        ItemSelectMenu_Update(  ism,    n9patch,
                                target, renderer);
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

static void _ItemSelectMenu_Draw_Hands( ItemSelectMenu  *ism,
                                        SDL_Renderer    *renderer) {
    /* -- Preliminaries -- */
    SDL_assert(ism != NULL);
    SDL_assert(ism->_unit_E > TNECS_NULL);
    SDL_assert(gl_world != NULL);

    Unit *unit          = IES_GET_C(gl_world, ism->_unit_E, Unit);
    b32 stronghand      = Unit_Hand_Strong(unit);
    b32 weakhand        = Unit_Hand_Weak(unit);
    b32 ambidextrous    = Unit_Handedness(unit) == UNIT_HAND_AMBIDEXTROUS;
    SDL_Rect srcrect = {0};
    SDL_Rect dstrect = {0};
    b32 LH_equipped = Unit_isEquipped(unit, UNIT_HAND_LEFT);
    b32 RH_equipped = Unit_isEquipped(unit, UNIT_HAND_RIGHT);

    if (LH_equipped) {
        /* -- Left hand icon -- */
        srcrect.w = ISM_HANDS_TILESIZE;
        srcrect.h = ISM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        int hand_i = ((weakhand == UNIT_HAND_LEFT) &&
                      (!ambidextrous)) ?
                     ISM_HANDS_SMALL_L :
                     ISM_HANDS_BIG_L;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        dstrect.x = ISM_HANDL_X;
        int eq = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT) ;
        int hand_row = eq - ITEM1;

        /* Computing y offset for weak hand, or twohanding icon placement */
        i32 ly_offset = (stronghand == UNIT_HAND_RIGHT) ? ISM_WEAKHAND_Y_OFFSET : ISM_STRONGHAND_Y_OFFSET;

        dstrect.y = ly_offset + hand_row * ISM_ROW_HEIGHT;

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_LEFT) {
            dstrect.x += ISM_HAND_SMALLX_OFFSET;
            dstrect.y += ISM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy( renderer, ism->texture_hands,
                        &srcrect, &dstrect);
    }
    if (RH_equipped) {
        /* -- Right hand icon -- */
        srcrect.w = ISM_HANDS_TILESIZE;
        srcrect.h = ISM_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        int hand_i = ((weakhand == UNIT_HAND_RIGHT) &&
                      (!ambidextrous)) ?
                     ISM_HANDS_SMALL_R :
                     ISM_HANDS_BIG_R;

        srcrect.x = hand_i * srcrect.w;
        srcrect.y = 0;

        /* Moving hand if two handing or weak hand */
        /* TODO: dynamic X position */
        dstrect.x = ism->_max_width - ISM_SPACE - ITEM_ICON_W;
        int eq = Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT) ;
        int hand_row = eq - ITEM1;

        /* Computing y offset for weak hand, or twohanding icon placement */
        i32 ly_offset = (stronghand == UNIT_HAND_LEFT) ? ISM_WEAKHAND_Y_OFFSET : ISM_STRONGHAND_Y_OFFSET;

        dstrect.y = ly_offset + hand_row * ISM_ROW_HEIGHT;

        /* Moving hand if small */
        if (stronghand != UNIT_HAND_RIGHT) {
            dstrect.x += ISM_HAND_SMALLX_OFFSET;
            dstrect.y += ISM_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy( renderer, ism->texture_hands,
                        &srcrect, &dstrect);
    }
}


static void _ItemSelectMenu_Draw_Names( ItemSelectMenu  *ism,
                                        n9Patch         *n9,
                                        SDL_Renderer    *renderer) {
    SDL_assert(ism          != NULL);
    SDL_assert(ism->_unit_E  > TNECS_NULL);
    SDL_assert(gl_world     != NULL);

    /* -- Preliminaries -- */
    char numbuff[10];

    /* -- HANDS --  */
    /* Icons, text drawn on stronghand's side */
    Unit *unit = IES_GET_C(gl_world, ism->_unit_E, Unit);

    /* -- Inventory -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    i32 eq_num = Unit_Equipment_Num(unit);
    SDL_assert(ism->_num == eq_num);

    for (i32 eq = ITEM1; eq < ism->_num ; eq++) {
        /* - Icons - */
        i32 i = eq - ITEM1;
        i32 id = Unit_Id_Equipment(unit, eq);

        /* -- Weapon name -- */
        Point pos = {
            .x = ISM1_NAME_X_OFFSET,
            .y = ISM1_NAME_Y_OFFSET + i * (ITEM_ICON_H + 2)
        };

        /* - Invalid weapon - */
        if ((id == ITEM_NULL) ||
            (!Weapon_ID_isValid(id) &&
             !Item_Pure_ID_isValid(id))) {
            /* This should not happen. */

            PixelFont_In pxin = {
                .renderer   = renderer,
                .text       = "-",
                .len        =  1,
                .pos        =  pos,
            };

            PixelFont_Write(ism->pixelnours, pxin);
            continue;
        }

        /* - Valid weapon - */
        s8 raw_name = Item_Name(id);
        s8 name = {0};
        name = s8cpy(name, raw_name);
        name = s8_toUpper(name);

        PixelFont_In pxin = {
            .renderer   = renderer,
            .text       = name.data,
            .len        = name.num,
            .pos        = pos,
        };
        PixelFont_Write(ism->pixelnours, pxin);

        /* -- Uses -- */
        pos.x = ISM1_DURA_X_OFFSET;
        pos.y = ISM1_DURA_Y_OFFSET + i * (ITEM_ICON_H + 2);

        const InvItem *invitem = Unit_InvItem(unit, eq);
        i32 uses = Item_remUses(invitem);
        stbsp_sprintf(numbuff, "%d\0\0\0\0", uses);

        i32 width = PixelFont_Width(ism->pixelnours_big,
                                    numbuff,
                                    strlen(numbuff));
        pos.x -= width / 2;

        pxin.text       = numbuff;
        pxin.pos        = pos;
        PixelFont_Write(ism->pixelnours_big, pxin);
    }

    Utilities_DrawColor_Reset(renderer);
}

void ItemSelectMenu_Texture_Create( ItemSelectMenu  *ism,
                                    n9Patch         *n9,
                                    SDL_Renderer    *renderer) {
    SDL_assert(ism      != NULL);
    SDL_assert(renderer != NULL);
    /* Skip if texture already exists */
    if (ism->texture != NULL) {
        return;
    }
    /* -- Compute menu size -- */
    ItemSelectMenu_Size(ism, n9);

    /* -- Create new texture -- */
    Point size = n9Patch_Pixels_Total(n9);
    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);

    ism->texture = SDL_CreateTexture(renderer,
                                     SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_TARGET,
                                     size.x, size.y);
    SDL_assert(ism->texture != NULL);

    SDL_SetTextureBlendMode(ism->texture, SDL_BLENDMODE_BLEND);
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
    _ItemSelectMenu_Draw_Names(ism, n9patch, renderer);

    SDL_SetRenderTarget(renderer, target);
    Utilities_DrawColor_Reset(renderer);
}

