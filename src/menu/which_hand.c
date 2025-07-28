
#include "menu/which_hand.h"
#include "unit/unit.h"
#include "menu/menu.h"
#include "item.h"
#include "utilities.h"

/* --- ELEMENTS --- */
MenuElemDirections whm_links[WHM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* HAND1 */ {
        WHM_ELEM_NULL,  WHM_ELEM_NULL,
        WHM_ELEM_NULL,  WHM_ELEM_HAND2
    },
    /* HAND2 */ {
        WHM_ELEM_NULL,  WHM_ELEM_NULL,
        WHM_ELEM_HAND1, WHM_ELEM_HAND3
    },
    /* HAND3 */ {
        WHM_ELEM_NULL,  WHM_ELEM_NULL,
        WHM_ELEM_HAND2, WHM_ELEM_NULL
    },
};

Point whm_elem_pos[WHM_ELEM_NUM] = {
    /* HAND1 */  {8,  21},
    /* HAND1 */  {8,  39},
    /* HAND1 */  {8,  57},
};

Point whm_elem_box[WHM_ELEM_NUM] = {
    /* HAND1 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* HAND2 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* HAND3 */  {SOTA_TILESIZE, SOTA_TILESIZE},
};

i32 WhichHandMenu_Select(struct WhichHandMenu *whm,
                         i32 elem) {
    /* Player selects hand from list of menu elements */

    SDL_assert(elem < whm->num_handedness);
    /* Ouput Hand from present elems
    ** 1. Some items are one hand only, L or R only...
    ** 2. Some items can't be two handed
    ** 3. Some units have onbe hand missing */
    return (whm->handedness[elem]);
}

void WhichHandMenu_Elements(struct WhichHandMenu   *whm,
                            struct Unit            *unit,
                            struct Item            *item) {
    /* Build list of menu elements from
    **  1. Weapon handedness
    **  2. Unit hands */
    whm->num_handedness = 0;

    /* -- Unit hands -- */
    b32 unitL   = Unit_hasHand(unit, UNIT_HAND_LEFT);
    b32 unitR   = Unit_hasHand(unit, UNIT_HAND_LEFT);

    /* -- Item handedness -- */
    b32 wpnL    = (Item_Handedness(item) == WEAPON_HAND_LEFT);
    b32 wpnR    = (Item_Handedness(item) == WEAPON_HAND_RIGHT);
    b32 wpn1H   = (Item_Handedness(item) == WEAPON_HAND_ONE);
    b32 wpn2H   = (Item_Handedness(item) == WEAPON_HAND_TWO);
    b32 wpnAny  = (Item_Handedness(item) == WEAPON_HAND_ANY);

    /* -- Can item be equipped in Left hand only? -- */
    if (unitL && (wpn1H || wpnL || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_LEFT;
    }

    /* -- Can item be equipped in Right hand only? -- */
    if (unitR && (wpn1H || wpnR || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_RIGHT;
    }

    /* -- Can item be equipped in two hands? -- */
    if (unitR && unitL && (wpn2H || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_TWO_HANDS;
    }
}

void WhichHandMenu_Elem_Links(struct WhichHandMenu *whm,
                              struct Menu *mc) {
    /* Get number of elements for the menu */

    mc->elem_num    = whm->num_handedness;
    mc->elem_links  = whm_links;
    SDL_assert(mc->elem_num > 0);
    SDL_assert(mc->elem_num <= WHM_ELEM_NULL);

    for (i32 i = 0; i < mc->elem_num; i++) {
        /* Link to elem top */
        mc->elem_links[i].top = (i > 0) ? i - 1 : WHM_ELEM_NULL;

        /* Link to elem bottom */
        mc->elem_links[i].bottom = (i < (mc->elem_num - 1)) ? i + 1 : WHM_ELEM_NULL;
    }
}

/* --- Drawing --- */
void WhichHandMenu_Draw_LH(struct WhichHandMenu *whm,
                           i32                   elem,
                           SDL_Texture          *target,
                           SDL_Renderer         *renderer) {
    SDL_Rect dstrect = {0};
    SDL_Rect srcrect = {0};

    /* - HANDS - */
    int stronghand = Unit_Hand_Strong(whm->unit);

    srcrect.w = HANDS_TILESIZE;
    srcrect.h = HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* Left hand */
    int index = (stronghand == UNIT_HAND_RIGHT) ? HANDS_SMALL_L : HANDS_BIG_L;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Setting hand to correct height */
    dstrect.x = WHM_ELEM_X;
    dstrect.y = WHM_ELEM_Y_0 + WHM_ELEM_Y_SLOPE * elem;

    // Moving hand if small
    if (stronghand == UNIT_HAND_RIGHT) {
        dstrect.x += WHM_HAND_SMALLX_OFFSET;
        dstrect.y += WHM_HAND_SMALLY_OFFSET;
    }

    SDL_RenderCopy(renderer, whm->texture_hands,
                   &srcrect, &dstrect);
}

void WhichHandMenu_Draw_RH(struct WhichHandMenu *whm,
                           i32                   elem,
                           SDL_Texture          *target,
                           SDL_Renderer         *renderer) {
    SDL_Rect dstrect = {0};
    SDL_Rect srcrect = {0};

    /* - HANDS - */
    int stronghand = Unit_Hand_Strong(whm->unit);

    srcrect.w = HANDS_TILESIZE;
    srcrect.h = HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* Right hand */
    int index = (stronghand == UNIT_HAND_LEFT) ? HANDS_SMALL_L : HANDS_BIG_L;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Setting hand to correct height */
    dstrect.x = WHM_ELEM_X + WHM_RH_X_OFFSET;
    dstrect.y = WHM_ELEM_Y_0 + WHM_ELEM_Y_SLOPE * elem;

    // Moving hand if small
    if (stronghand == UNIT_HAND_LEFT) {
        dstrect.x += WHM_HAND_SMALLX_OFFSET;
        dstrect.y += WHM_HAND_SMALLY_OFFSET;
    }

    SDL_RenderCopy(renderer, whm->texture_hands,
                   &srcrect, &dstrect);

}

void WhichHandMenu_Draw(struct Menu     *mc,
                        SDL_Texture     *target,
                        SDL_Renderer    *renderer) {
    WhichHandMenu   *whm        = mc->data;
    struct n9Patch  *n9patch    = &mc->n9patch;

    SDL_assert(whm != NULL);

    if (whm->update) {
        WhichHandMenu_Update(whm, n9patch, mc->elem_num, target, renderer);
        whm->update = false;
    }

    /* TODO: set position of DeploymentMenu */
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = whm->pos.x,
        .y = whm->pos.y,
    };
    SDL_assert(whm->texture != NULL);
    SDL_RenderCopy(renderer, whm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch        *n9patch,
                          i32                    elem_num,
                          SDL_Texture           *target,
                          SDL_Renderer          *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(renderer != NULL);
    SDL_assert(whm      != NULL);
    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x       > 0);
    SDL_assert(n9patch->scale.y       > 0);

    /* - create render target texture - */
    if (whm->texture == NULL) {
        int x = n9patch->size_pixels.x;
        int y = n9patch->size_pixels.y;
        whm->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         x, y);
        SDL_assert(whm->texture != NULL);
        SDL_SetTextureBlendMode(whm->texture,
                                SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, whm->texture);
    SDL_assert(whm->texture != NULL);

    /* --- RENDERING DEPLOYMENT-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    /* -- HANDS DRAW -- */
    for (i32 elem = 0; elem < elem_num; elem++) {
        if (whm->handedness[elem] != UNIT_EQUIP_RIGHT) {
            WhichHandMenu_Draw_LH(whm, elem, target, renderer);
        }
        if (whm->handedness[elem] != UNIT_EQUIP_LEFT) {
            WhichHandMenu_Draw_RH(whm, elem, target, renderer);
        }
    }

    /* -- Finish -- */
    Utilities_DrawColor_Reset(renderer);
    SDL_SetRenderTarget(renderer, target);
    SDL_assert(whm->texture);
}
