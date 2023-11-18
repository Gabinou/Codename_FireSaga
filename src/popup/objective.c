#include "popup/objective.h"

struct PopUp_Objective PopUp_Objective_default =  {
    .objective =       NULL,
    .sub_objective =   NULL,
    .texture =         NULL,
    .texture_header =  NULL,
    .pixelnours =      NULL,
    .padding =         {PO_PADDING_RIGHT, PO_PADDING_TOP, PO_PADDING_LEFT, PO_PADDING_BOTTOM},
    .update =          true,
};

/* --- ructor/Destructor --- */
struct PopUp_Objective *PopUp_Objective_Alloc(void) {
    struct PopUp_Objective *po = SDL_malloc(sizeof(struct PopUp_Objective));
    *po = PopUp_Objective_default;
    return (po);
}

void PopUp_Objective_Load(struct PopUp_Objective *po, SDL_Renderer *renderer,
                          struct n9Patch *n9patch) {
    SDL_assert(po != NULL);
    *n9patch                 = n9Patch_default;
    n9patch->patch_pixels.x  = PO_PATCH_PIXELS;
    n9patch->patch_pixels.y  = PO_PATCH_PIXELS;
    n9patch->scale.x         = 1;
    n9patch->scale.y         = 1;
    n9patch->size_pixels.x   = PO_PATCH_PIXELS;
    n9patch->size_pixels.y   = PO_PATCH_PIXELS;
    n9patch->size_patches.x  = 1;
    n9patch->size_patches.y  = 1;
    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;
    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "tilepopup8px.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
}

void PopUp_Objective_Free(struct PopUp_Objective *po) {
    SDL_assert(po != NULL);
    if (po->texture != NULL) {
        SDL_DestroyTexture(po->texture);
        po->texture = NULL;
    }
    if (po->texture_header != NULL) {
        SDL_DestroyTexture(po->texture_header);
        po->texture_header = NULL;
    }
    if (po->objective != NULL) {
        SDL_free(po->objective);
        po->objective = NULL;
    }
    if (po->sub_objective != NULL) {
        SDL_free(po->sub_objective);
        po->sub_objective = NULL;
    }
    SDL_free(po);

}

/* --- Setters --- */
void PopUp_Objective_Set_Obj(struct PopUp_Objective *po, char *obj) {
    if (po->objective != NULL) {
        SDL_Log("Objective '%s' removed.", po->objective);
        SDL_free(po->objective);
    }
    size_t len = strlen(obj) + 1;
    po->objective = malloc(len);
    memcpy(po->objective, obj, len);
    SDL_Log("Objective set to '%s'", po->objective);
}

void PopUp_Objective_Set_SubObj(struct PopUp_Objective *po, char *subobj) {
    if (po->objective != NULL) {
        SDL_Log("Sub-Objective %s removed.", po->sub_objective);
        SDL_free(po->sub_objective);
    }
    size_t len = strlen(subobj) + 1;
    po->sub_objective = malloc(len);
    memcpy(po->sub_objective, subobj, len);
    SDL_Log("Sub-Objective set to '%s'", po->sub_objective);
}

/* --- Computers --- */
void PopUp_Objective_Compute_Size(struct PopUp_Objective *po, struct n9Patch *n9patch) {

    /* - Compute qith opf current objectives - */
    int obj_w = 0, subobj_w = 0;
    if (po->objective != NULL)
        obj_w = PixelFont_Width(po->pixelnours, po->objective, strlen(po->objective));

    if (po->sub_objective != NULL)
        subobj_w = PixelFont_Width(po->pixelnours, po->sub_objective, strlen(po->sub_objective));

    int text_width = obj_w > subobj_w ? obj_w : subobj_w;
    if (text_width > po->text_width) {
        int padding = po->padding.left + po->padding.right;
        po->text_width = text_width + padding;
    }

    n9patch->size_pixels.x = po->text_width;
    n9patch->size_pixels.y = po->padding.top + po->padding.bottom + ASCII_GLYPH_HEIGHT * PO_ROW_NUM;
    n9patch->size_patches.x = n9patch->size_pixels.x / n9patch->patch_pixels.x + 1;
    n9patch->size_patches.y = n9patch->size_pixels.y / n9patch->patch_pixels.y;

    /* - Destroy texture because it does not fit new size - */
    SDL_DestroyTexture(po->texture);
    po->texture = NULL;
}

/* --- Drawing --- */
void PopUp_Objective_Draw(struct PopUp *popup, struct Point pos,
                          SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PopUp_Objective *po = (struct PopUp_Objective *)popup->data;
    struct n9Patch *n9patch = &popup->n9patch;

    if (po->update) {
        PopUp_Objective_Update(po, n9patch, render_target, renderer);
        po->update = false;
    }
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y
    };
    SDL_assert(po->texture != NULL);
    SDL_RenderCopy(renderer, po->texture, NULL, &dstrect);
}

void PopUp_Objective_Update(struct PopUp_Objective *po,
                            struct n9Patch *n9patch, SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(n9patch != NULL);
    SDL_assert(po != NULL);
    SDL_assert(renderer != NULL);

    PopUp_Objective_Compute_Size(po, n9patch);
    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_Rect dstrect, srcrect;
    i16 menu_w = n9patch->size_pixels.x;
    i16 menu_h = n9patch->size_pixels.y;
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);
    /* - create render target texture - */
    if (po->texture == NULL) {
        po->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(po->texture != NULL);
        SDL_SetTextureBlendMode(po->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, po->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_assert(po->texture != NULL);
    /* --- RENDERING popup objective --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9patch->scale.x;
    int scale_y = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x = 0;
    n9patch->pos.y = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    /* -- Writing preliminaries -- */
    int total_text_height = PO_ROW_NUM * ASCII_GLYPH_HEIGHT + n9patch->pos.y + po->padding.top;
    int posx = n9patch->pos.x + po->padding.left, posy;
    int shift_y = (n9patch->size_patches.y * n9patch->patch_pixels.y) - total_text_height;
    shift_y /= 2;

    /* -- OBJECTIVE -- */
    if (po->objective != NULL) {
        posy = shift_y + n9patch->pos.y + po->padding.top;
        PixelFont_Write(po->pixelnours, renderer, po->objective, strlen(po->objective), posx, posy);
    }

    /* -- SUB-OBJECTIVE -- */
    if (po->sub_objective != NULL) {
        posy = shift_y + n9patch->pos.y + po->padding.top + ASCII_GLYPH_HEIGHT;
        PixelFont_Write(po->pixelnours, renderer, po->sub_objective, strlen(po->sub_objective), posx, posy);
    }

    po->update = false;
    SDL_SetRenderTarget(renderer, render_target);

}
