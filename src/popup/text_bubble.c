

#include "text_bubble.h"

struct Text_Bubble_Pointer Text_Bubble_Pointer_default = {
    .pos        = {0, 0},
    .flip       = SDL_FLIP_NONE,
    .texture    = NULL,
    .angle      = 0.0,
    .half       = false,
    .index      = false,
};

struct Text_Bubble TextBubble_default = {
    .width      = 0,
    .height     = 0,
    .row_height = ASCII_GLYPH_HEIGHT,
    .line_len_px   = 0,
    .line_num   = 0,
    .text       = NULL,
    .padding    = {0},
    .target     = {0, 0},
    .pixelnours = NULL,
    .texture    = NULL,
    .update     = false,
    .pointer = {
        .pos        = {0, 0},
        .flip       = SDL_FLIP_NONE,
        .texture    = NULL,
        .angle      = 0.0,
        .half       = false,
        .index      = false,
    },
};

void TextBubble_Free(struct Text_Bubble *bubble) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (bubble->text != NULL) {
        free(bubble->text);
        bubble->text = NULL;
    }

    if (bubble->texture != NULL) {
        SDL_DestroyTexture(bubble->texture);
        bubble->texture = NULL;
    }

    if (bubble->pointer.texture != NULL) {
        SDL_DestroyTexture(bubble->pointer.texture);
        bubble->pointer.texture = NULL;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Load(struct Text_Bubble *bubble, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(bubble != NULL);

    TextBubble_Free(bubble);
    n9Patch_Free(n9patch);

    bubble->update = true;

    *n9patch                  = n9Patch_default;
    n9patch->patch_pixels.x   = PLS_PATCH_PIXELS;
    n9patch->patch_pixels.y   = PLS_PATCH_PIXELS;
    n9patch->scale.x          = PLS_N9PATCH_SCALE_X;
    n9patch->scale.y          = PLS_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x    = PLS_PATCH_PIXELS * PLS_PATCH_X_SIZE;
    n9patch->size_pixels.y    = PLS_PATCH_PIXELS * PLS_PATCH_Y_SIZE;
    n9patch->size_patches.x   = PLS_PATCH_X_SIZE;
    n9patch->size_patches.y   = PLS_PATCH_Y_SIZE;
    n9patch->pos.x            = 0;
    n9patch->pos.y            = 0;

    char *path = PATH_JOIN("..", "assets", "GUI", "Popup", "Popup_TextBubble.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(bubble->texture != NULL);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "texture_pointer.png");
    bubble->pointer.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(bubble->pointer.texture != NULL);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Set_Text(struct Text_Bubble *bubble, const char *text) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (bubble->text != NULL)
        free(bubble->text);

    size_t len   = strlen(text);
    bubble->text = calloc(len, sizeof(*bubble->text));
    strncpy(bubble->text, text, len);
    TextBubble_Compute_Size(bubble);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Set_Target(struct Text_Bubble *bubble, struct Point target) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bubble->target = target;
    // TextBubble_Pointer_Octant(bubble, target);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

int TextBubble_Pointer_Octant(struct Text_Bubble *bubble, struct Point pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Find octant around text bubble target is in
    SDL_assert(bubble->target.x >= 0);
    SDL_assert(bubble->target.y >= 0);
    struct Point ternary;
    ternary = Ternary_Direction_Octant(pos, bubble->target, bubble->width, bubble->height);
    bubble->pointer.octant  = Ternary_Direction_Index(ternary.x, ternary.y);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (bubble->pointer.octant);
}

int TextBubble_Pointer_Flip(struct Text_Bubble *bubble, struct Point pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Decide pointer flip. */
    // Points pointer ion direction of target
    // Only for straight quadrants
    SDL_assert(bubble->target.x >= 0);
    SDL_assert(bubble->target.y >= 0);
    struct Point half = {pos.x > bubble->target.x, pos.y > bubble->target.y};

    switch (bubble->pointer.octant) {
        case SOTA_DIRECTION_RIGHT:
            bubble->pointer.flip = half.y ? SDL_FLIP_VERTICAL   : SDL_FLIP_NONE;
            break;
        case SOTA_DIRECTION_TOP:
            bubble->pointer.flip = half.x ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            break;
        case SOTA_DIRECTION_BOTTOM:
            bubble->pointer.flip = half.x ? SDL_FLIP_NONE       : SDL_FLIP_HORIZONTAL;
            break;
        case SOTA_DIRECTION_LEFT:
            bubble->pointer.flip = half.y ? SDL_FLIP_NONE       : SDL_FLIP_VERTICAL;
            break;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

int TextBubble_Pointer_Angle(struct Text_Bubble *bubble) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Decide orientation of pointer, except flip. */
    // Only puts pointer in correct octant
    bubble->pointer.flip = SDL_FLIP_NONE;
    switch (bubble->pointer.octant) {
        case SOTA_DIRECTION_RIGHT:
            bubble->pointer.angle = 270.0;
            bubble->pointer.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_TOP:
            bubble->pointer.angle = 180.0;
            bubble->pointer.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_BOTTOM:
            bubble->pointer.angle = 0.0;
            bubble->pointer.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_LEFT:
            bubble->pointer.angle = 90.0;
            bubble->pointer.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_DIAGONAL_TR:
            bubble->pointer.angle = 180.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_BR:
            bubble->pointer.angle = 90.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_BL:
            bubble->pointer.angle = 0.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_TL:
            bubble->pointer.angle = 270.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void TextBubble_Pointer_Draw(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Compute_Size(struct Text_Bubble *bu) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (bu->text == NULL) {
        SOTA_Log_Debug("bubble's text is NULL");
        return;
    }
    PixelFont_Width_Len(bu->pixelnours, bu->text);
    int line_num = PixelFont_Lines_Num_Len(bu->pixelnours, bu->text, bu->line_len_px);
    bu->height = line_num * bu->row_height + bu->padding.top + bu->padding.bottom;
    bu->width  = bu->line_len_px + bu->padding.right + bu->padding.left;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Drawing elements -- */
void TextBubble_Write(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Drawing --- */
void TextBubble_Update(struct Text_Bubble *bubble, struct n9Patch *n9patch,
                       SDL_Texture *target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    SDL_assert(bubble != NULL);
    SDL_assert(renderer != NULL);
    /* - variable declaration/ constants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_assert(bubble->width > 0);
    SDL_assert(bubble->height > 0);

    /* - create render target texture - */
    if (bubble->texture == NULL) {
        bubble->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, bubble->width, bubble->height);
        SDL_assert(bubble->texture != NULL);
        SDL_SetTextureBlendMode(bubble->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, bubble->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_assert(bubble->texture != NULL);

    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = PLS_DEST_Y;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    TextBubble_Write(       bubble, renderer);
    TextBubble_Pointer_Draw(bubble, renderer);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Draw(struct PopUp *popup, struct Point pos,
                     SDL_Texture *target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct n9Patch     *n9patch  = &popup->n9patch;
    struct Text_Bubble *bubble   =  popup->data;

    SDL_assert(bubble != NULL);
    if (bubble->update) {
        TextBubble_Update(bubble, n9patch,  target, renderer);
        bubble->update = false;
    }

    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y,
    };
    SDL_assert(bubble->texture != NULL);
    SDL_RenderCopy(renderer, bubble->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);


    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
