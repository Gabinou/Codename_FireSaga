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
** WhichHandMenu (WHM): choose how to equip item
*/

#include "menu/which_hand.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "menu/menu.h"
#include "item.h"
#include "filesystem.h"
#include "utilities.h"
WhichHandMenu WhichHandMenu_default = {0};

/* --- ELEMENTS --- */
n4Directions whm_links[WHM_ELEM_NUM] = {
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

/* -- Private -- */
static void _WhichHandMenu_Draw_LH(WhichHandMenu    *whm,
                                   i32               elem,
                                   SDL_Texture      *rt,
                                   SDL_Renderer     *r);
static void _WhichHandMenu_Draw_RH(WhichHandMenu    *whm,
                                   i32               elem,
                                   SDL_Texture      *rt,
                                   SDL_Renderer     *r);

void WhichHandMenu_Load(WhichHandMenu   *whm,
                        SDL_Renderer    *renderer,
                        n9Patch         *n9patch) {
    _WhichHandMenu_Free(whm);

    n9Patch_Free(n9patch);
    n9patch->px.x       = WHM_PATCH_PIXELS;
    n9patch->px.y       = WHM_PATCH_PIXELS;
    n9patch->num.x      = WHM_PATCH_X_SIZE;
    n9patch->num.y      = WHM_PATCH_Y_SIZE;
    n9patch->scale.x    = WHM_N9PATCH_SCALE_X;
    n9patch->scale.y    = WHM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (WHM_PATCH_PIXELS * WHM_PATCH_X_SIZE),
        .y  = (WHM_PATCH_PIXELS * WHM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }

    char *path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
    whm->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(whm->texture_hands);
}

void _WhichHandMenu_Free(WhichHandMenu *whm) {
    if (whm->texture_hands != NULL) {
        SDL_DestroyTexture(whm->texture_hands);
        whm->texture_hands = NULL;
    }
}

WhichHandMenu *WhichHandMenu_Alloc(void) {
    WhichHandMenu *whm  = IES_malloc(sizeof(WhichHandMenu));
    *whm = WhichHandMenu_default;
    IES_assert(whm);
    return (whm);
}

void WhichHandMenu_Free(WhichHandMenu *whm, Menu *mc) {
    if (whm == NULL) {
        IES_assert(0);
        return;
    }
    if (mc == NULL) {
        IES_assert(0);
        return;
    }
    Menu_Free(mc);
    _WhichHandMenu_Free(whm);
    IES_free(whm);
}


i32 WhichHandMenu_Selected_Hand(const WhichHandMenu *whm) {
    if (whm == NULL) {
        SDL_assert(0);
        return (0);
    }
    return (whm->handedness[whm->selected]);
}

i32 WhichHandMenu_Select(WhichHandMenu *whm,
                         i32 elem) {
    /* Player selects hand from list of menu elements */
    if (whm == NULL) {
        SDL_assert(0);
        return (0);
    }

    SDL_assert(elem < whm->num_handedness);
    /* Output Hand from present elems
    ** 1. Some items are one hand only, L or R only...
    ** 2. Some items can't be two handed
    ** 3. Some units have onbe hand missing */
    whm->selected = elem;
    return (whm->handedness[whm->selected]);
}

void _WhichHandMenu_Elements(WhichHandMenu  *whm,
                             n9Patch        *n9patch,
                             Item           *item) {
    /* Build list of menu elements from
    **  1. Weapon handedness
    **  2. Unit hands */
    whm->num_handedness = 0;
    /* -- Unit hands -- */
    /* TODO: use canEquip */
    b32 unitL   = Unit_hasHand(whm->unit, UNIT_HAND_LEFT);
    b32 unitR   = Unit_hasHand(whm->unit, UNIT_HAND_RIGHT);

    /* -- Item handedness -- */
    b32 wpnL    = (Item_Handedness(item) == WEAPON_HAND_LEFT);
    b32 wpnR    = (Item_Handedness(item) == WEAPON_HAND_RIGHT);
    b32 wpn1H   = (Item_Handedness(item) == WEAPON_HAND_ONE);
    b32 wpn2H   = (Item_Handedness(item) == WEAPON_HAND_TWO);
    b32 wpnAny  = (Item_Handedness(item) == WEAPON_HAND_ANY);

    /* -- Can item be equipped in Left hand only? -- */
    if (unitL && (wpn1H || wpnL || wpnAny)) {
        SDL_Log("LH");
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_LEFT;
    }

    /* -- Can item be equipped in Right hand only? -- */
    if (unitR && (wpn1H || wpnR || wpnAny)) {
        SDL_Log("RH");
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_RIGHT;
    }

    /* -- Can item be equipped in two hands? -- */
    if (unitR && unitL && (wpn2H || wpnAny)) {
        SDL_Log("TWO_HANDS");
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_TWO_HANDS;
    }

    /* Dynamically set menu patch size */
    if (whm->num_handedness == 1) {
        n9patch->num.y = 3;
    } else if (whm->num_handedness == 2) {
        n9patch->num.y = 5;
    } else if (whm->num_handedness == 3) {
        n9patch->num.y = WHM_PATCH_Y_SIZE;
    } else {
        IES_assert(0);
    }

    /* Reset texture size */
    if (whm->texture != NULL) {
        SDL_DestroyTexture(whm->texture);
        whm->texture = NULL;
    }

    whm->update = true;
}

void WhichHandMenu_Elements(Menu *mc,   Unit *unit,
                            Item *item) {

    WhichHandMenu   *whm        = mc->data;
    SDL_assert(whm  != NULL);

    SDL_assert(unit != NULL);
    whm->unit = unit;

    n9Patch  *n9patch    = &mc->n9patch;
    SDL_assert(n9patch != NULL);

    /* Find which hand can Player select from */
    _WhichHandMenu_Elements(whm, n9patch, item);
    mc->elem_num    = whm->num_handedness;

    /* Set links between menu elements */
    WhichHandMenu_Elem_Links(mc);
}

void WhichHandMenu_Elem_Links(struct Menu *mc) {
    /* Get number of elements for the menu */
    WhichHandMenu   *whm        = mc->data;
    SDL_assert(whm != NULL);

    mc->elem_links  = whm_links;
    SDL_Log("whm->num_handedness %d", whm->num_handedness);
    SDL_assert(whm->num_handedness > 0);
    SDL_assert(whm->num_handedness <= WHM_ELEM_NUM);

    for (i32 i = 0; i < whm->num_handedness; i++) {
        /* Link to elem top */
        mc->elem_links[i].top = (i > 0) ? i - 1 : WHM_ELEM_NULL;

        /* Link to elem bottom */
        mc->elem_links[i].bottom = (i < (mc->elem_num - 1)) ? i + 1 : WHM_ELEM_NULL;
    }
}

/* --- Drawing --- */
void _WhichHandMenu_Draw_LH(struct WhichHandMenu *whm,
                            i32                   elem,
                            SDL_Texture          *target,
                            SDL_Renderer         *renderer) {
    SDL_Rect dstrect = {0};
    SDL_Rect srcrect = {0};

    /* - HANDS - */
    int stronghand = Unit_Hand_Strong(whm->unit);
    i8 uHandedness = Unit_Handedness(whm->unit);

    srcrect.w = HANDS_TILESIZE;
    srcrect.h = HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* Left hand */
    b32 big_LH = (stronghand == UNIT_HAND_LEFT) || (uHandedness == UNIT_HAND_AMBIDEXTROUS);
    int index = big_LH ? HANDS_BIG_L : HANDS_SMALL_L;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Setting hand to correct height */
    dstrect.x = WHM_ELEM_X;
    dstrect.y = WHM_ELEM_Y_0 + WHM_ELEM_Y_LINE_SPACING * elem;

    // Moving hand if small
    if (!big_LH) {
        dstrect.x += WHM_HAND_SMALLX_OFFSET;
        dstrect.y += WHM_HAND_SMALLY_OFFSET;
    }

    SDL_RenderCopy(renderer, whm->texture_hands,
                   &srcrect, &dstrect);
}

void _WhichHandMenu_Draw_RH(struct WhichHandMenu *whm,
                            i32                   elem,
                            SDL_Texture          *target,
                            SDL_Renderer         *renderer) {
    SDL_Rect dstrect = {0};
    SDL_Rect srcrect = {0};

    /* - HANDS - */
    SDL_assert(whm->unit);
    int stronghand = Unit_Hand_Strong(whm->unit);
    i8 uHandedness = Unit_Handedness(whm->unit);

    srcrect.w = HANDS_TILESIZE;
    srcrect.h = HANDS_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    /* Right hand */
    b32 big_RH = (stronghand == UNIT_HAND_RIGHT) || (uHandedness == UNIT_HAND_AMBIDEXTROUS);
    int index = big_RH ? HANDS_BIG_R : HANDS_SMALL_R;
    srcrect.x = index * srcrect.w;
    srcrect.y = 0;

    /* Setting hand to correct height */
    dstrect.x = WHM_ELEM_X + WHM_RH_X_OFFSET;
    dstrect.y = WHM_ELEM_Y_0 + WHM_ELEM_Y_LINE_SPACING * elem;

    // Moving hand if small
    if (!big_RH) {
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
    SDL_assert(whm != NULL);
    struct n9Patch  *n9patch    = &mc->n9patch;


    if (whm->update) {
        WhichHandMenu_Update(whm, n9patch, target, renderer);
        whm->update = false;
    }

    /* TODO: set position of DeploymentMenu */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = whm->pos.x,
        .y = whm->pos.y,
    };
    SDL_assert(whm->texture != NULL);
    SDL_RenderCopy(renderer, whm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch        *n9patch,
                          SDL_Texture           *target,
                          SDL_Renderer          *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(renderer != NULL);
    SDL_assert(whm      != NULL);
    /* - variable declaration/ ants definition - */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_assert(size.x           > 0);
    SDL_assert(size.y           > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);

    /* - create render target texture - */
    /* TODO: utility for menu texture creation.
    **          It's an optimization, save for later. */
    if (whm->texture == NULL) {
        whm->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         size.x, size.y);
        SDL_assert(whm->texture != NULL);
        SDL_SetTextureBlendMode(whm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_assert(whm->texture != NULL);
    SDL_SetRenderTarget(renderer, whm->texture);

    /* Clearing old render */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    Utilities_DrawColor_Reset(renderer);

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
    for (i32 elem = 0; elem < whm->num_handedness; elem++) {
        if (whm->handedness[elem] != UNIT_EQUIP_RIGHT) {
            _WhichHandMenu_Draw_LH(whm, elem, target, renderer);
        }
        if (whm->handedness[elem] != UNIT_EQUIP_LEFT) {
            _WhichHandMenu_Draw_RH(whm, elem, target, renderer);
        }
    }

    /* -- Finish -- */
    Utilities_DrawColor_Reset(renderer);
    SDL_SetRenderTarget(renderer, target);
    SDL_assert(whm->texture);
}
