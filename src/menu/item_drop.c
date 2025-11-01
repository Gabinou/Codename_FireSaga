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
** ItemDropMenu: Give player 1 chance to cancel item drop
**
*/

#include "names.h"
#include "nmath.h"
#include "macros.h"
#include "globals.h"
#include "platform.h"
#include "utilities.h"
#include "filesystem.h"
#include "pixelfonts.h"

#include "menu/stats.h"
#include "menu/item_drop.h"

#include "unit/loadout.h"
#include "unit/equipment.h"

const ItemDropMenu ItemDropMenu_default = {0};

const i32 IDM_Options[IDM_OPTION_NUM] = {
    MENU_OPTION_YES,
    MENU_OPTION_NO
};

const struct Point idm_cursor_pos[IDM_OPTION_NUM] = {
    {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    {IDM_ELEM_NO_X, IDM_ELEM_NO_Y},
};

const struct Point idm_elem_box[IDM_OPTION_NUM] = {
    {SOTA_TILESIZE, SOTA_TILESIZE},
    {SOTA_TILESIZE, SOTA_TILESIZE},
};

const struct Point idm_elem_pos[IDM_OPTION_NUM] = {
    {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    {IDM_ELEM_NO_X,  IDM_ELEM_NO_Y},
};

const struct n4Directions idm_links[IDM_OPTION_NUM] = {
    /*right, top, left, bottom */
    {IDM_ELEM_NULL, IDM_ELEM_0, IDM_ELEM_NULL, IDM_ELEM_0},
    {IDM_ELEM_NULL, IDM_ELEM_1,  IDM_ELEM_NULL, IDM_ELEM_1},
};

/* --- Constructors/Destructors --- */
ItemDropMenu *ItemDropMenu_Alloc(void) {
    ItemDropMenu *idm = IES_malloc(sizeof(ItemDropMenu));
    SDL_assert(idm != NULL);
    *idm = ItemDropMenu_default;
    SDL_assert(idm->texture == NULL);

    return (idm);
}

void ItemDropMenu_Load( ItemDropMenu    *idm,
                        SDL_Renderer    *renderer,
                        n9Patch         *n9patch) {
    /* n9patch init */
    n9patch->px.x       = MENU_PATCH_PIXELS;
    n9patch->px.y       = MENU_PATCH_PIXELS;
    n9patch->num.x      = IDM_PATCH_X_SIZE;
    n9patch->num.y      = IDM_PATCH_Y_SIZE;
    n9patch->scale.x    = IDM_N9PATCH_SCALE_X;
    n9patch->scale.y    = IDM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * IDM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * IDM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load( renderer, path,
                                                    SDL_PIXELFORMAT_INDEX8);
    }
}

void ItemDropMenu_Free(ItemDropMenu *idm) {
    if (idm == NULL) {
        return;
    }
    if (idm->texture != NULL) {
        SDL_DestroyTexture(idm->texture);
        idm->texture = NULL;
    }

    IES_free(idm);
}

/* --- Drawing --- */
void ItemDropMenu_Draw(Menu *mc, SDL_Texture *target,
                       SDL_Renderer *renderer) {
    ItemDropMenu *idm = (ItemDropMenu *)mc->data;
    SDL_assert(idm          != NULL);
    SDL_assert(idm->unit_E  != TNECS_NULL);
    struct n9Patch *n9patch = &mc->n9patch;

    SDL_assert(idm != NULL);
    if (idm->update) {
        ItemDropMenu_Update(idm, n9patch,  target, renderer);
        idm->update = false;
    }

    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = idm->pos.x,
        .y = idm->pos.y,
    };
    SDL_assert(idm->texture != NULL);
    SDL_RenderCopy(renderer, idm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

/* --- Elements --- */
i32 ItemDropMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, IDM_OPTION_NUM));
}


void ItemDropMenu_Elem_Links(ItemDropMenu *idm, Menu *mc) {
    if (mc->elem_links != NULL) {
        IES_free(mc->elem_links);
    }
    size_t bytesize = IDM_OPTION_NUM * sizeof(*mc->elem_links);
    mc->elem_links = IES_malloc(bytesize);
    memcpy(mc->elem_links, idm_links, bytesize);
}

void ItemDropMenu_Elem_Boxes(ItemDropMenu *idm, Menu *mc) {
    if (mc->elem_box != NULL) {
        IES_free(mc->elem_box);
    }
    size_t bytesize = IDM_OPTION_NUM * sizeof(*mc->elem_box);
    mc->elem_box = IES_malloc(bytesize);
    memcpy(mc->elem_box, idm_elem_box, bytesize);
}

void ItemDropMenu_Elem_Pos(ItemDropMenu *idm, Menu *mc) {
    Point pos9  = mc->n9patch.pos;
    Point scale = mc->n9patch.scale;

    if (mc->elem_pos != NULL) {
        IES_free(mc->elem_pos);
    }
    mc->elem_pos = IES_calloc(IDM_OPTION_NUM, sizeof(*mc->elem_pos));
    for (i32 i = 0; i < IDM_OPTION_NUM; i++) {
        mc->elem_pos[i].x = idm->pos.x + idm_elem_pos[i].x * scale.x;
        mc->elem_pos[i].y = idm->pos.y + idm_elem_pos[i].y * scale.y;
    }
}

s8 ItemDropMenu_Name(InvItem *item) {
    // TODO: keep in memory
    s8 name = s8_mut("");
    name = s8cat(name, Item_Name(item->id));
    name = s8_toUpper(name);
    return (name);
}

s8 ItemDropMenu_Question(s8 name) {
    s8 question = s8_mut("DROP \'");
    question = s8cat(question, name);
    question = s8cat(question, s8_literal("\'?"));
    return (question);
}

void ItemDropMenu_Item_Width(ItemDropMenu *idm, s8 question) {
    idm->item_width = PixelFont_Width(  idm->pixelnours_big,
                                        question.data, question.num);
}

i32 ItemDropMenu_Menu_Option(      ItemDropMenu *idm, i32 elem) {
    SDL_assert(elem < IDM_OPTION_NUM);
    return (IDM_Options[elem]);
}

i32 ItemDropMenu_Menu_Option_Num(  ItemDropMenu *idm) {
    return (IDM_OPTION_NUM);
}

i32 ItemDropMenu_Menu_Option_Order(ItemDropMenu *idm, i32 option_id) {
    i32 num = ItemDropMenu_Menu_Option_Num(idm);
    for (int i = 0; i < IDM_OPTION_NUM; i++) {
        if (IDM_Options[i] == option_id) {
            return (i);
        }
    }
    return (-1);
}

void ItemDropMenu_Update(   ItemDropMenu    *idm,
                            n9Patch         *n9patch,
                            SDL_Texture     *render_target,
                            SDL_Renderer    *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert( (idm->eq_todrop >= 0) &&
                (idm->eq_todrop < SOTA_EQUIPMENT_SIZE));
    SDL_assert(renderer         != NULL);
    SDL_assert(idm->unit_E      != TNECS_NULL);
    SDL_assert(gl_items_dtab    != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);

    /* - Loading item - */
    Unit *unit = IES_GET_C(gl_world, idm->unit_E, Unit);

    InvItem *item = Unit_InvItem(unit, idm->eq_todrop);

    /* Item name */

    /* --- Compute menu width dynamically --- */
    s8 name     = ItemDropMenu_Name(item);
    s8 question = ItemDropMenu_Question(name);
    ItemDropMenu_Item_Width(idm, question);

    Point old_size = n9Patch_Pixels_Total(n9patch);
    Point new_size = old_size;
    new_size.x =    IDM_LEFT_OF_TEXT +
                    idm->item_width +
                    IDM_RIGHT_OF_TEXT;

    /* - create texture - */
    if ((idm->texture == NULL) || (new_size.x != old_size.x)) {
        new_size = n9Patch_Pixels_Total_Set(n9patch, new_size);

        idm->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         new_size.x, new_size.y);
        SDL_assert(idm->texture != NULL);
        SDL_SetTextureBlendMode(idm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, idm->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);
    SDL_assert(idm->texture != NULL);

    /* --- ACTUAL DRAWING --- */
    /* --- RENDERING ITEM DROP MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x      = n9patch->scale.x;
    int scale_y      = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    /* - Question - */
    PixelFont_Write(idm->pixelnours_big,    renderer,
                    question.data,          question.num,
                    IDM_ITEM_NAME_X,        IDM_ITEM_NAME_Y);
    s8_free(&question);
    s8_free(&name);

    /* - Yes - */
    int yes_w    = PixelFont_Width(idm->pixelnours_big, "Yes", 3);
    size_t yes_x = (IDM_LEFT_OF_TEXT + idm->item_width - yes_w) / 2;
    PixelFont_Write(idm->pixelnours_big, renderer, "Yes", 3, yes_x, IDM_ELEM_YES_Y);

    /* - No - */
    int no_w    = PixelFont_Width(idm->pixelnours_big, "No", 2);
    size_t no_x = (IDM_LEFT_OF_TEXT + idm->item_width - no_w) / 2;
    PixelFont_Write(idm->pixelnours_big, renderer, "No",  2, no_x,  IDM_ELEM_NO_Y);

    SDL_SetRenderTarget(renderer, render_target);
}
