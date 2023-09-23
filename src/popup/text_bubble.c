
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
    .width       = 0,
    .height      = 0,
    .row_height  = ASCII_GLYPH_HEIGHT,
    .lines       = {0},
    .line_len_px = 64,
    .line_num    = 0,
    .text        = NULL,
    .padding     = {
        .left   = TEXT_BUBBLE_PADDING_LEFT,
        .top    = TEXT_BUBBLE_PADDING_TOP,
        .bottom = TEXT_BUBBLE_PADDING_BOTTOM,
        .right  = TEXT_BUBBLE_PADDING_RIGHT,
    },
    .target      = {-100, -100},
    .pixelfont   = NULL,
    .texture     = NULL,
    .update      = false,
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

    TextLines_Free(&bubble->lines);

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
    n9patch->patch_pixels.x   = TEXT_BUBBLE_PATCH_PIXELS;
    n9patch->patch_pixels.y   = TEXT_BUBBLE_PATCH_PIXELS;
    n9patch->scale.x          = TEXT_BUBBLE_SCALE;
    n9patch->scale.y          = TEXT_BUBBLE_SCALE;
    n9patch->pos.x            = 0;
    n9patch->pos.y            = 0;

    char *path = PATH_JOIN("..", "assets", "GUI", "Popup", "Popup_TextBubble_n9patch.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch->texture != NULL);

    path = PATH_JOIN("..", "assets", "GUI", "Popup", "Popup_TextBubble_Pointer.png");
    bubble->pointer.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(bubble->pointer.texture != NULL);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Set_Text(struct Text_Bubble *bubble, const char *text, struct n9Patch *n9patch) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (bubble->text != NULL) {
        free(bubble->text);
        bubble->text = NULL;
    }
    size_t len   = strlen(text);
    bubble->text = calloc(len + 1, sizeof(*bubble->text));
    strncpy(bubble->text, text, len);

    TextLines_Free(&bubble->lines);
    bubble->lines = PixelFont_Lines_Len(bubble->pixelfont, bubble->text, bubble->line_len_px);

    TextBubble_Compute_Size(bubble, n9patch);

    if (bubble->texture != NULL) {
        SDL_DestroyTexture(bubble->texture);
        bubble->texture = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Set_Target(struct Text_Bubble *bubble, struct Point target) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bubble->target = target;
    struct Point pos = {0, 0};
    TextBubble_Pointer_Octant(bubble, pos);
    TextBubble_Pointer_Flip(bubble, pos);
    TextBubble_Pointer_Pos(bubble, pos);
    TextBubble_Pointer_Angle(bubble);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

int TextBubble_Pointer_Octant(struct Text_Bubble *bubble, struct Point pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // Find octant around text bubble target is in
    struct Point ternary;
    ternary = Ternary_Direction_Octant(pos, bubble->target, bubble->width, bubble->height);
    bubble->pointer.octant = Ternary_Direction_Index(ternary.x, ternary.y);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (bubble->pointer.octant);
}

void TextBubble_Pointer_Flip(struct Text_Bubble *bubble, struct Point pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Decide pointer flip. */
    // Points pointer in direction of target
    // Only for straight quadrants
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
    // TODO: IF diagonal octant, flip one way or another if target more x than y and vice-versa

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Pointer_Pos(struct Text_Bubble *bubble, struct Point pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Decide pointer position. */
    bubble->pointer.dstrect.w = TEXT_BUBBLE_POINTER_SIZE;
    bubble->pointer.dstrect.h = TEXT_BUBBLE_POINTER_SIZE;

    switch (bubble->pointer.octant) {
        case SOTA_DIRECTION_RIGHT:
            bubble->pointer.dstrect.x = bubble->width - 2;
            bubble->pointer.dstrect.y = TEXT_BUBBLE_PATCH_PIXELS - 5;
            break;
        case SOTA_DIRECTION_TOP:
            bubble->pointer.dstrect.x = TEXT_BUBBLE_PATCH_PIXELS;
            bubble->pointer.dstrect.y = -6;
            break;
        case SOTA_DIRECTION_BOTTOM:
            bubble->pointer.dstrect.x = TEXT_BUBBLE_PATCH_PIXELS;
            bubble->pointer.dstrect.y = bubble->height - 2;
            break;
        case SOTA_DIRECTION_LEFT:
            // if (bubble->)
            bubble->pointer.dstrect.x = -6;
            bubble->pointer.dstrect.y = TEXT_BUBBLE_PATCH_PIXELS - 3;
            break;
        case SOTA_DIRECTION_DIAGONAL_TR:
            bubble->pointer.dstrect.x = bubble->width - 6;
            bubble->pointer.dstrect.y = -2;
            break;
        case SOTA_DIRECTION_DIAGONAL_BR:
            bubble->pointer.dstrect.x = bubble->width  - 6;
            bubble->pointer.dstrect.y = bubble->height - 6;
            break;
        case SOTA_DIRECTION_DIAGONAL_BL:
            bubble->pointer.dstrect.x = -2;
            bubble->pointer.dstrect.y = bubble->height - 6;
            break;
        case SOTA_DIRECTION_DIAGONAL_TL:
            bubble->pointer.dstrect.x = -2;
            bubble->pointer.dstrect.y = -2;
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Pointer_Angle(struct Text_Bubble *bubble) {
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
            bubble->pointer.angle = 270.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_BR:
            bubble->pointer.angle = 0.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_BL:
            bubble->pointer.angle = 90.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_DIAGONAL_TL:
            bubble->pointer.angle = 180.0;
            bubble->pointer.index = TEXT_BUBBLE_DIAGONAL;
            break;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void TextBubble_Pointer_Draw(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_Rect srcrect;
    srcrect.x = TEXT_BUBBLE_POINTER_SIZE * bubble->pointer.index;
    srcrect.y = 0;
    srcrect.w = TEXT_BUBBLE_POINTER_SIZE;
    srcrect.h = TEXT_BUBBLE_POINTER_SIZE;
    SDL_Rect dstrect;
    dstrect.x = bubble->pointer.dstrect.x + TEXT_BUBBLE_RENDER_PAD;
    dstrect.y = bubble->pointer.dstrect.y + TEXT_BUBBLE_RENDER_PAD;
    dstrect.w = bubble->pointer.dstrect.w;
    dstrect.h = bubble->pointer.dstrect.h;

    SDL_Point center = {TEXT_BUBBLE_POINTER_SIZE / 2, TEXT_BUBBLE_POINTER_SIZE / 2};
    SDL_RenderCopyEx(renderer, bubble->pointer.texture, &srcrect, &dstrect,
                     bubble->pointer.angle, &center, bubble->pointer.flip);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void TextBubble_Compute_Size(struct Text_Bubble *bu, struct n9Patch *n9patch) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (bu->text == NULL) {
        SOTA_Log_Debug("bubble's text is NULL");
        return;
    }

    /* -- Bubble text size -- */
    int line_num = PixelFont_Lines_Num_Len(bu->pixelfont, bu->text, bu->line_len_px);
    SDL_assert(line_num == bu->lines.line_num);
    bu->height = line_num * bu->row_height + bu->padding.top + bu->padding.bottom;
    if (line_num <= 1) {
        bu->width = PixelFont_Width_Len(bu->pixelfont, bu->text) + bu->padding.right * 2 + bu->padding.left;
    } else {
        bu->width = bu->line_len_px + bu->padding.right + bu->padding.left;
    }

    /* -- n9patch size -- */
    n9patch->size_pixels.x  = bu->width;
    n9patch->size_pixels.y  = bu->height;
    struct Point content = {bu->width, bu->height};
    n9Patch_Fit(n9patch, content);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Drawing elements -- */
void TextBubble_Write(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - name - */
    int x = bubble->padding.left + TEXT_BUBBLE_RENDER_PAD, y;
    for (int i = 0; i < bubble->lines.line_num; i++) {
        y = bubble->padding.top + bubble->row_height * i + TEXT_BUBBLE_RENDER_PAD;
        PixelFont_Write_Len(bubble->pixelfont, renderer, bubble->lines.lines[i], x, y);
    }
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
    SDL_assert(bubble->width            > 0);
    SDL_assert(bubble->height           > 0);
    SDL_assert(n9patch->scale.x         > 0);
    SDL_assert(n9patch->scale.y         > 0);
    SDL_assert(n9patch->size_pixels.x   > 0);
    SDL_assert(n9patch->size_pixels.y   > 0);

    /* - create render target texture - */
    if (bubble->texture == NULL) {
        int x = n9patch->size_pixels.x + TEXT_BUBBLE_RENDER_PAD * 2;
        int y = n9patch->size_pixels.y + TEXT_BUBBLE_RENDER_PAD * 2;
        bubble->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET, x, y);
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
    n9patch->pos.x      = TEXT_BUBBLE_RENDER_PAD;
    n9patch->pos.y      = TEXT_BUBBLE_RENDER_PAD;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    TextBubble_Write(       bubble, renderer);
    TextBubble_Pointer_Draw(bubble, renderer);

    SDL_SetRenderTarget(renderer, target);

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
