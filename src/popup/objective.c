
#include "popup/objective.h"
#include "popup/popup.h"
#include "platform.h"
#include "macros.h"
#include "filesystem.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "stb_sprintf.h"

const struct PopUp_Objective PopUp_Objective_default =  {
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
    n9patch->px.x  = PO_PATCH_PIXELS;
    n9patch->px.y  = PO_PATCH_PIXELS;
    n9patch->scale.x         = 1;
    n9patch->scale.y         = 1;
    n9patch->num.x  = 1;
    n9patch->num.y  = 1;
    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;
    Point size = {.x = (PO_PATCH_PIXELS), .y = (PO_PATCH_PIXELS)};
    n9Patch_Pixels_Total_Set(n9patch, size);

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
    po->objective = SDL_malloc(len);
    memcpy(po->objective, obj, len);
    // SDL_Log("Objective set to '%s'", po->objective);
}

void PopUp_Objective_Set_SubObj(struct PopUp_Objective *po, char *subobj) {
    if (po->objective != NULL) {
        SDL_Log("Sub-Objective %s removed.", po->sub_objective);
        SDL_free(po->sub_objective);
    }
    size_t len = strlen(subobj) + 1;
    po->sub_objective = SDL_malloc(len);
    memcpy(po->sub_objective, subobj, len);
    // SDL_Log("Sub-Objective set to '%s'", po->sub_objective);
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
    Point pf_size = PixelFont_Glyph_Size(po->pixelnours);
    Point size = {
        .x = po->text_width,
        .y = po->padding.top + po->padding.bottom +
        pf_size.y * PO_ROW_NUM,
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

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
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
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
    Point size = n9Patch_Pixels_Total(n9patch);

    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    /* - create render target texture - */
    if (po->texture == NULL) {
        po->texture = SDL_CreateTexture(renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.x, size.y);
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
    Point pf_size = PixelFont_Glyph_Size(po->pixelnours);

    int total_text_height = PO_ROW_NUM * pf_size.y +
                            n9patch->pos.y + po->padding.top;
    int posx = n9patch->pos.x + po->padding.left;
    int shift_y = (n9patch->num.y * n9patch->px.y) - total_text_height;
    shift_y /= 2;

    /* -- OBJECTIVE -- */
    if (po->objective != NULL) {
        i32 posy = shift_y + n9patch->pos.y + po->padding.top;
        PixelFont_In pxin = {
            .renderer   = renderer,
            .text       = po->objective,
            .pos        =  {
                .x = posx,
                .y = posy
            }
        };

        PixelFont_Write(po->pixelnours, pxin);
    }

    /* -- SUB-OBJECTIVE -- */
    if (po->sub_objective != NULL) {
        i32 posy = shift_y + n9patch->pos.y + po->padding.top + pf_size.y;
        PixelFont_In pxin = {
            .renderer   = renderer,
            .text       = po->objective,
            .pos        =  {
                .x = posx,
                .y = posy
            }
        };
        PixelFont_Write(po->pixelnours, pxin);
    }

    po->update = false;
    SDL_SetRenderTarget(renderer, render_target);
}
