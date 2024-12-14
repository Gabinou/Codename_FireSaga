
#include "menu/item_drop.h"
#include "menu/stats.h"
#include "unit/equipment.h"
#include "platform.h"
#include "utilities.h"
#include "filesystem.h"
#include "unit/loadout.h"
#include "pixelfonts.h"
#include "names.h"
#include "nmath.h"
#include "macros.h"

struct ItemDropMenu ItemDropMenu_default = {0};

struct Point idm_cursor_pos[IDM_ELEM_NUM] = {
    /* ID_ELEM_YES */ {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    /* IDM_ELEM_NO */ {IDM_ELEM_NO_X, IDM_ELEM_NO_Y},
};

struct Point idm_elem_box[IDM_ELEM_NUM] = {
    /* ID_ELEM_YES */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* IDM_ELEM_NO */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct Point idm_elem_pos[IDM_ELEM_NUM] = {
    /* IDM_ELEM_YES */ {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    /* IDM_ELEM_NO */  {IDM_ELEM_NO_X,  IDM_ELEM_NO_Y},
};

struct MenuElemDirections idm_links[IDM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* IDM_ELEM_YES */ {IDM_ELEM_NULL, IDM_ELEM_YES, IDM_ELEM_NULL, IDM_ELEM_YES},
    /* IDM_ELEM_NO */  {IDM_ELEM_NULL, IDM_ELEM_NO,  IDM_ELEM_NULL, IDM_ELEM_NO},
};

/* --- Constructors/Destructors --- */
struct ItemDropMenu *ItemDropMenu_Alloc(void) {
    struct ItemDropMenu *idm = SDL_malloc(sizeof(struct ItemDropMenu));
    SDL_assert(idm != NULL);
    *idm = ItemDropMenu_default;
    SDL_assert(idm->texture == NULL);

    return (idm);
}

void ItemDropMenu_Load(struct ItemDropMenu *idm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    /* n9patch init */
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->size_patches.x  = IDM_PATCH_X_SIZE;
    n9patch->size_patches.y  = IDM_PATCH_Y_SIZE;
    n9patch->scale.x         = IDM_N9PATCH_SCALE_X;
    n9patch->scale.y         = IDM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = MENU_PATCH_PIXELS * IDM_PATCH_X_SIZE;
    n9patch->size_pixels.y   = MENU_PATCH_PIXELS * IDM_PATCH_Y_SIZE;

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }

}

void ItemDropMenu_Free(struct ItemDropMenu *idm) {
    if (idm->texture != NULL) {
        SDL_DestroyTexture(idm->texture);
        idm->texture = NULL;
    }
    if (idm != NULL) {
        SDL_free(idm);
        idm = NULL;
    }
}

/* --- Elements --- */
void ItemDropMenu_Elem_Pos(struct ItemDropMenu *idm, struct Menu *mc) {

}

/* --- Selection --- */
void ItemDropMenu_Select(struct ItemDropMenu *idm, i8 elem) {
    if (elem == IDM_ELEM_YES)
        ItemDropMenu_Drop(idm);
}

void ItemDropMenu_Drop(struct ItemDropMenu *idm) {
    Unit_Item_Drop(idm->unit, idm->item_todrop);
}

/* --- Drawing --- */
void ItemDropMenu_Draw(struct Menu *mc, SDL_Texture *target, SDL_Renderer *renderer) {
    struct ItemDropMenu *idm = (struct ItemDropMenu *)mc->data;
    SDL_assert(idm       != NULL);
    SDL_assert(idm->unit != NULL);
    struct n9Patch *n9patch = &mc->n9patch;

    SDL_assert(idm != NULL);
    if (idm->update) {
        ItemDropMenu_Update(idm, n9patch,  target, renderer);
        idm->update = false;
    }

    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = idm->pos.x,
        .y = idm->pos.y,
    };
    SDL_assert(idm->texture != NULL);
    SDL_RenderCopy(renderer, idm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);

}

void ItemDropMenu_Update(struct  ItemDropMenu  *idm, struct n9Patch *n9patch,
                         SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert((idm->item_todrop >= 0) && (idm->item_todrop < SOTA_EQUIPMENT_SIZE));
    SDL_assert(renderer                != NULL);
    SDL_assert(idm->unit               != NULL);
    SDL_assert(idm->unit->items_dtab   != NULL);
    SDL_assert(idm->unit->weapons_dtab != NULL);

    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    struct Unit *unit = idm->unit;
    SDL_Rect dstrect, srcrect;

    /* - Loading item - */
    Inventory_item *item = Unit_InvItem(idm->unit, idm->item_todrop);

    /* Item name */
    s8 name = s8_mut("");
    int *order = DTAB_GET(global_itemOrders, item->id);
    name = s8cat(name, global_itemNames[*order]);
    name = s8_toUpper(name);

    /* --- Compute menu width dynamically --- */
    // size_t len_done = 8;
    s8 question = s8_mut("DROP \'");
    question = s8cat(question, name);
    question = s8cat(question, s8_literal("\'?"));
    // char  *question  = SDL_calloc((len + len_done), sizeof(*question));
    // memcpy(question,           "DROP \'", 6);
    // memcpy(question + 6,       name.data,    len);
    // memcpy(question + 6 + len, "\'?",     2);
    idm->item_width         = PixelFont_Width(idm->pixelnours_big, question.data, question.num);
    int new_size_x          = IDM_LEFT_OF_TEXT + idm->item_width + IDM_RIGHT_OF_TEXT;

    /* - create texture - */
    ;
    if ((idm->texture == NULL) || (new_size_x != n9patch->size_pixels.x)) {
        n9patch->size_pixels.x  = new_size_x;
        n9patch->size_patches.x = n9patch->size_pixels.x / n9patch->patch_pixels.x;
        n9patch->size_pixels.x  = n9patch->size_patches.x * n9patch->patch_pixels.x;

        int x = n9patch->size_pixels.x, y = n9patch->size_pixels.y;
        idm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, x, y);
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
    PixelFont_Write(idm->pixelnours_big, renderer, question.data, question.num,
                    IDM_ITEM_NAME_X, IDM_ITEM_NAME_Y);
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

