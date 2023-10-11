
#include "menu/item_drop.h"
struct ItemDropMenu ItemDropMenu_default = {
    .pos            = {-1, -1},
    .texture        = NULL,
    .item_todrop    = -1,
    .item_width     = -1,
    .unit           = NULL,
    .pixelnours     = NULL,
    .pixelnours_big = NULL,
    .update         = false,
};

struct nmath_point_int32_t idm_cursor_pos[IDM_ELEM_NUM] = {
    /* ID_ELEM_YES */ {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    /* IDM_ELEM_NO */ {IDM_ELEM_NO_X, IDM_ELEM_NO_Y},
};

struct nmath_point_int32_t idm_elem_box[IDM_ELEM_NUM] = {
    /* ID_ELEM_YES */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* IDM_ELEM_NO */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct nmath_point_int32_t  idm_elem_pos[IDM_ELEM_NUM] = {
    /* IDM_ELEM_YES */ {IDM_ELEM_YES_X, IDM_ELEM_YES_Y},
    /* IDM_ELEM_NO */  {IDM_ELEM_NO_X,  IDM_ELEM_NO_Y},
};

struct MenuElemDirections idm_links[IDM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* IDM_ELEM_YES */ {IDM_ELEM_NULL, IDM_ELEM_YES, IDM_ELEM_NULL, IDM_ELEM_YES},
    /* IDM_ELEM_NO */  {IDM_ELEM_NULL, IDM_ELEM_NO,  IDM_ELEM_NULL, IDM_ELEM_NO},
};

/* --- Constructors/Destructors --- */
struct ItemDropMenu *ItemDropMenu_Alloc() {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct ItemDropMenu *idm = malloc(sizeof(struct ItemDropMenu));
    SDL_assert(idm != NULL);
    *idm = ItemDropMenu_default;
    SDL_assert(idm->texture == NULL);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (idm);
}

void ItemDropMenu_Load(struct ItemDropMenu *idm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void ItemDropMenu_Free(struct ItemDropMenu *idm) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (idm->texture != NULL) {
        SDL_DestroyTexture(idm->texture);
        idm->texture = NULL;
    }
    if (idm != NULL) {
        free(idm);
        idm = NULL;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Elements --- */
void ItemDropMenu_Elem_Pos(struct ItemDropMenu *idm, struct MenuComponent *mc) {

}

/* --- Selection --- */
void ItemDropMenu_Select(struct ItemDropMenu *idm, if8 elem) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (elem == IDM_ELEM_YES)
        ItemDropMenu_Drop(idm);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void ItemDropMenu_Drop(struct ItemDropMenu *idm) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Unit_Item_Drop(idm->unit, idm->item_todrop);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Drawing --- */
void ItemDropMenu_Draw(struct MenuComponent *mc, SDL_Texture *target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void ItemDropMenu_Update(struct  ItemDropMenu  *idm, struct n9Patch *n9patch,
                         SDL_Texture *render_target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    SDL_assert((idm->item_todrop >= 0) && (idm->item_todrop < DEFAULT_EQUIPMENT_SIZE));
    SDL_assert(renderer                != NULL);
    SDL_assert(idm->unit               != NULL);
    SDL_assert(idm->unit->items_dtab   != NULL);
    SDL_assert(idm->unit->weapons_dtab != NULL);

    /* - variable declaration/ constants definition - */
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    struct Unit *unit = idm->unit;
    SDL_Rect dstrect, srcrect;

    /* - Loading item - */
    struct WpnorItem wpn_or_item = Unit_WpnorItem(idm->unit, idm->item_todrop);

    /* Item name */
    char *name = NULL;
    size_t len;
    if (wpn_or_item.dtab == SOTA_DTAB_WEAPONS) {
        SDL_assert(wpn_or_item.item == NULL);
        len     = strlen(wpn_or_item.wpn->item->name);
        name    = calloc(len + 1, sizeof(*name));
        strncpy(name, wpn_or_item.wpn->item->name, len);
    } else {
        SDL_assert(wpn_or_item.wpn == NULL);
        len     = strlen(wpn_or_item.item->name);
        name    = calloc(len + 1, sizeof(*name));
        strncpy(name, wpn_or_item.item->name, len);
    }
    name = nstr_toUpper(name);

    /* --- Compute menu width dynamically --- */
    size_t len_done = 8;
    char  *question  = calloc((len + len_done), sizeof(*question));
    strncpy(question,           "DROP \'", 6);
    strncpy(question + 6,       name,    len);
    strncpy(question + 6 + len, "\'?",     2);
    idm->item_width         = PixelFont_Width(idm->pixelnours_big, question, (len + len_done));
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
    PixelFont_Write(idm->pixelnours_big, renderer, question, (len + len_done), IDM_ITEM_NAME_X,
                    IDM_ITEM_NAME_Y);
    free(question);
    free(name);

    /* - Yes - */
    int yes_w    = PixelFont_Width(idm->pixelnours_big, "Yes", 3);
    size_t yes_x = (IDM_LEFT_OF_TEXT + idm->item_width - yes_w) / 2;
    PixelFont_Write(idm->pixelnours_big, renderer, "Yes", 3, yes_x, IDM_ELEM_YES_Y);

    /* - No - */
    int no_w    = PixelFont_Width(idm->pixelnours_big, "No", 2);
    size_t no_x = (IDM_LEFT_OF_TEXT + idm->item_width - no_w) / 2;
    PixelFont_Write(idm->pixelnours_big, renderer, "No",  2, no_x,  IDM_ELEM_NO_Y);

    SDL_SetRenderTarget(renderer, render_target);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

