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
    /* WeaponSelectMenu_Load_n9Patch(ism, renderer, n9patch); */
    /* ItemSelectMenu_Load(ism, map, renderer, n9patch); */
}

void ItemSelectMenu_Select( ItemSelectMenu *ism,
                            i32 select) {
    /* Loadout_Set(&ism->selected, UNIT_HAND_LEFT, select); */
}

i32 ItemSelectMenu_Selected( ItemSelectMenu *ism) {
    /* return (Loadout_Eq(&ism->selected, UNIT_HAND_LEFT)); */
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
}

static void _ItemSelectMenu_Draw_Items( ItemSelectMenu  *ism,
                                        SDL_Renderer    *renderer) {
}

void ItemSelectMenu_Texture_Create( ItemSelectMenu  *ism,
                                    n9Patch         *n9patch,
                                    SDL_Renderer    *renderer) {
    /* -- Compute menu size -- */
    /* ItemSelectMenu_Size(ism, n9patch); */

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

