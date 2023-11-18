
#include "text_bubble.h"

struct Text_Bubble_Tail Text_Bubble_Tail_default = {
    .pos        = {0, 0},
    .flip       = SDL_FLIP_NONE,
    .texture    = NULL,
    .angle      = 0.0,
    .half       = false,
    .index      = false,
    .index      = false,
};

struct Text_Bubble TextBubble_default = {
    .width              =  0,
    .height             =  0,
    .row_height         = ASCII_GLYPH_HEIGHT,
    .lines              = {0},
    .line_len_px        = 64,
    .line_num_max       = -1,
    .text               = NULL,
    .target             = {-100, -100},
    .pixelfont          = NULL,
    .texture            = NULL,
    .texture_vscroll    = NULL,
    .update             = false,
    .vscroll_speed      = TEXT_BUBBLE_VSCROLL_SPEED,
    .vscroll_dir        = TEXT_BUBBLE_VSCROLL_TOP,
    .vscroll            = 0,

    .padding     = {
        .left       = TEXT_BUBBLE_PADDING_LEFT,
        .top        = TEXT_BUBBLE_PADDING_TOP,
        .bottom     = TEXT_BUBBLE_PADDING_BOTTOM,
        .right      = TEXT_BUBBLE_PADDING_RIGHT,
    },

    .tail     = {
        .pos            = {0, 0},
        .flip           = SDL_FLIP_NONE,
        .texture        = NULL,
        .angle          = 0.0,
        .half           = false,
        .index          = false,
    },
};

void TextBubble_Free(struct Text_Bubble *bubble) {
    if (bubble->text != NULL) {
        SDL_free(bubble->text);
        bubble->text = NULL;
    }

    if (bubble->texture != NULL) {
        SDL_DestroyTexture(bubble->texture);
        bubble->texture = NULL;
    }

    TextLines_Free(&bubble->lines);

    if (bubble->tail.texture != NULL) {
        SDL_DestroyTexture(bubble->tail.texture);
        bubble->tail.texture = NULL;
    }

}

void TextBubble_Load(struct Text_Bubble *bubble, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(bubble != NULL);

    /* -- Free before re-allocating -- */
    TextBubble_Free(bubble);
    n9Patch_Free(n9patch);

    /* -- Bubble defaults -- */
    bubble->update = true;

    /* -- n9patch defaults -- */
    *n9patch                  = n9Patch_default;
    n9patch->patch_pixels.x   = TEXT_BUBBLE_PATCH_PIXELS;
    n9patch->patch_pixels.y   = TEXT_BUBBLE_PATCH_PIXELS;
    n9patch->scale.x          = TEXT_BUBBLE_SCALE;
    n9patch->scale.y          = TEXT_BUBBLE_SCALE;
    n9patch->pos.x            = 0;
    n9patch->pos.y            = 0;

    /* -- Loading textures -- */
    char *path = PATH_JOIN("..", "assets", "GUI", "Popup", "Popup_TextBubble_n9patch.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(n9patch->texture != NULL);

    path = PATH_JOIN("..", "assets", "GUI", "Popup", "Popup_TextBubble_Tail.png");
    bubble->tail.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(bubble->tail.texture != NULL);

}

void TextBubble_Set_Text(struct Text_Bubble *bubble,  char *text, struct n9Patch *n9patch) {
    /* -- Free before re-allocating -- */
    if (bubble->text != NULL) {
        SDL_free(bubble->text);
        bubble->text = NULL;
    }
    /* -- Copying input text -- */
    size_t len   = strlen(text);
    bubble->text = calloc(len + 1, sizeof(*bubble->text));
    memcpy(bubble->text, text, len);

    /* -- Split text into n lines depending on bubble max line length -- */
    TextLines_Free(&bubble->lines);
    bubble->lines = PixelFont_Lines_Len(bubble->pixelfont, bubble->text, bubble->line_len_px);

    /* -- Compute bubble size from text lines -- */
    TextBubble_Compute_Size(bubble, n9patch);

}

void TextBubble_Set_Target(struct Text_Bubble *bubble, struct Point target) {
    /* -- Compute everything related to new target --  */
    /* target is relative to bubble position */
    bubble->target = target;
    TextBubble_Tail_Octant(bubble);
    TextBubble_Tail_Angle( bubble);
}

int TextBubble_Tail_Octant(struct Text_Bubble *bubble) {
    /* -- Find octant around text bubble target is in -- */
    struct Point pos = {TEXT_BUBBLE_RENDER_PAD, TEXT_BUBBLE_RENDER_PAD};
    struct Point ternary;

    ternary = Ternary_Direction_Octant(pos, bubble->target, bubble->width, bubble->height);
    bubble->tail.octant = Ternary_Direction_Index(ternary.x, ternary.y);
    SDL_assert(bubble->tail.octant != SOTA_DIRECTION_INSIDE);

    return (bubble->tail.octant);
}

void TextBubble_Tail_Pos(struct Text_Bubble *bubble, struct n9Patch *n9patch) {
    /* Decide tail position. */
    struct Point pos = {TEXT_BUBBLE_RENDER_PAD, TEXT_BUBBLE_RENDER_PAD};

    bubble->tail.dstrect.w = Text_Bubble_Tail_SIZE;
    bubble->tail.dstrect.h = Text_Bubble_Tail_SIZE;

    switch (bubble->tail.octant) {
        case SOTA_DIRECTION_RIGHT:
            bubble->tail.dstrect.x = bubble->width - 2;
            bubble->tail.dstrect.y = TEXT_BUBBLE_PATCH_PIXELS - 5;
            if (n9patch->size_patches.y > 2) {
                /* Place tail according to target */
                bubble->tail.dstrect.y = bubble->target.y - TEXT_BUBBLE_RENDER_PAD / 2;
            }
            break;
        case SOTA_DIRECTION_TOP:
            // bubble->tail.dstrect.x = TEXT_BUBBLE_PATCH_PIXELS;
            bubble->tail.dstrect.x = bubble->target.x - TEXT_BUBBLE_RENDER_PAD / 2;
            bubble->tail.dstrect.y = -6;
            break;
        case SOTA_DIRECTION_BOTTOM:
            // bubble->tail.dstrect.x = TEXT_BUBBLE_PATCH_PIXELS;
            bubble->tail.dstrect.x = bubble->target.x - TEXT_BUBBLE_RENDER_PAD / 2;
            bubble->tail.dstrect.y = bubble->height - 2;
            break;
        case SOTA_DIRECTION_LEFT:
            bubble->tail.dstrect.x = -6;
            bubble->tail.dstrect.y = TEXT_BUBBLE_PATCH_PIXELS - 3;
            if (n9patch->size_patches.y > 2) {
                /* Place tail according to target */
                bubble->tail.dstrect.y = bubble->target.y - TEXT_BUBBLE_RENDER_PAD / 2;
            }
            break;
        case SOTA_DIRECTION_TOPRIGHT:
            bubble->tail.dstrect.x = bubble->width - 6;
            bubble->tail.dstrect.y = -2;
            break;
        case SOTA_DIRECTION_BOTRIGHT:
            bubble->tail.dstrect.x = bubble->width  - 6;
            bubble->tail.dstrect.y = bubble->height - 6;
            break;
        case SOTA_DIRECTION_BOTLEFT:
            bubble->tail.dstrect.x = -2;
            bubble->tail.dstrect.y = bubble->height - 6;
            break;
        case SOTA_DIRECTION_TOPLEFT:
            bubble->tail.dstrect.x = -2;
            bubble->tail.dstrect.y = -2;
            break;
    }
}

void TextBubble_Tail_Angle(struct Text_Bubble *bubble) {
    /* Decide orientation of tail, except flip. */
    // Only puts tail in correct octant

    bubble->tail.flip = SDL_FLIP_NONE;
    switch (bubble->tail.octant) {
        case SOTA_DIRECTION_RIGHT:
            bubble->tail.angle = 270.0;
            bubble->tail.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_TOP:
            bubble->tail.angle = 180.0;
            bubble->tail.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_BOTTOM:
            bubble->tail.angle = 0.0;
            bubble->tail.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_LEFT:
            bubble->tail.angle = 90.0;
            bubble->tail.index = TEXT_BUBBLE_STRAIGHT;
            break;
        case SOTA_DIRECTION_TOPRIGHT:
            bubble->tail.angle = 270.0;
            bubble->tail.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_BOTRIGHT:
            bubble->tail.angle = 0.0;
            bubble->tail.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_BOTLEFT:
            bubble->tail.angle = 90.0;
            bubble->tail.index = TEXT_BUBBLE_DIAGONAL;
            break;
        case SOTA_DIRECTION_TOPLEFT:
            bubble->tail.angle = 180.0;
            bubble->tail.index = TEXT_BUBBLE_DIAGONAL;
            break;
    }
}


void TextBubble_Tail_Draw(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    SDL_Rect srcrect;
    srcrect.x = Text_Bubble_Tail_SIZE * bubble->tail.index;
    srcrect.y = 0;
    srcrect.w = Text_Bubble_Tail_SIZE;
    srcrect.h = Text_Bubble_Tail_SIZE;
    SDL_Rect dstrect;
    dstrect.x = bubble->tail.dstrect.x + TEXT_BUBBLE_RENDER_PAD;
    dstrect.y = bubble->tail.dstrect.y + TEXT_BUBBLE_RENDER_PAD;
    dstrect.w = bubble->tail.dstrect.w;
    dstrect.h = bubble->tail.dstrect.h;

    SDL_Point center = {Text_Bubble_Tail_SIZE / 2, Text_Bubble_Tail_SIZE / 2};
    SDL_RenderCopyEx(renderer, bubble->tail.texture, &srcrect, &dstrect,
                     bubble->tail.angle, &center, bubble->tail.flip);

}

void TextBubble_Compute_Size(struct Text_Bubble *bu, struct n9Patch *n9patch) {
    /* -- Check -- */
    if (bu->text == NULL) {
        SDL_Log("bubble's text is NULL");
        return;
    }

    /* -- Destroy old bubble texture. -- */
    if (bu->texture != NULL) {
        SDL_DestroyTexture(bu->texture);
        bu->texture = NULL;
    }
    if (bu->texture_vscroll != NULL) {
        SDL_DestroyTexture(bu->texture_vscroll);
        bu->texture_vscroll = NULL;
    }

    /* -- Bubble text size -- */
    int line_num = PixelFont_Lines_Num_Len(bu->pixelfont, bu->text, bu->line_len_px);
    SDL_assert(line_num == bu->lines.line_num);

    /* -- Max line num -- */
    line_num = bu->line_num_max > 0 ? bu->line_num_max : line_num;
    bu->height = line_num * bu->row_height + bu->padding.top + bu->padding.bottom;
    if (line_num <= 1) {
        int len = PixelFont_Width_Len(bu->pixelfont, bu->text);
        len = (len < TEXT_BUBBLE_MIN_WIDTH) ? (TEXT_BUBBLE_MIN_WIDTH) : len;
        bu->width = len + bu->padding.right * 2 + bu->padding.left;
    } else {
        bu->width = bu->line_len_px + bu->padding.right + bu->padding.left;
    }

    /* -- n9patch size -- */
    n9patch->size_pixels.x  = bu->width;
    n9patch->size_pixels.y  = bu->height;
    struct Point content = {bu->width, bu->height};
    n9Patch_Fit(n9patch, content);
    SDL_assert(n9patch->size_patches.x >= 2);
    SDL_assert(n9patch->size_patches.y >= 2);

}

void TextBubble_Set_All(struct Text_Bubble *bubble,  char *text, struct Point target,
                        struct n9Patch *n9patch) {

    TextBubble_Set_Text(bubble, text, n9patch);
    TextBubble_Set_Target(bubble, target);
    SDL_assert(bubble->width  > 0);
    SDL_assert(bubble->height > 0);
    TextBubble_Tail_Pos(bubble, n9patch);

}

/* -- Drawing elements -- */
void TextBubble_Copy_VScroll(struct Text_Bubble *bubble, SDL_Renderer *renderer,
                             SDL_Texture *render_target) {
    /* - Copy written text + middle of n9patch for VScroll - */
    bubble->vscroll = 0;

    /* - create render target texture - */
    SDL_Rect srcrect = {0}, dstrect = {0};
    dstrect.w = bubble->width  - TEXT_BUBBLE_COPY_PAD * 2;
    dstrect.h = bubble->height - TEXT_BUBBLE_COPY_PAD * 2;
    srcrect.x = TEXT_BUBBLE_COPY_PAD + TEXT_BUBBLE_RENDER_PAD;
    srcrect.y = TEXT_BUBBLE_COPY_PAD + TEXT_BUBBLE_RENDER_PAD;
    srcrect.w = dstrect.w;
    srcrect.h = dstrect.h;

    if (bubble->texture_vscroll == NULL) {
        bubble->texture_vscroll = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                                    SDL_TEXTUREACCESS_TARGET, dstrect.w, dstrect.h);
        SDL_assert(bubble->texture_vscroll != NULL);
        SDL_SetTextureBlendMode(bubble->texture_vscroll, SDL_BLENDMODE_BLEND);
    }

    SDL_assert(bubble->texture_vscroll != NULL);
    SDL_SetRenderTarget(renderer, bubble->texture_vscroll);
    SDL_RenderCopy(renderer, bubble->texture, &srcrect, &dstrect);
    SDL_SetRenderTarget(renderer, render_target);
}

void TextBubble_VScroll(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    /* - To do vscroll - */
    SDL_assert(bubble->vscroll_speed > 0);
    bubble->vscroll += bubble->vscroll_speed;
}

void TextBubble_VScroll_Draw(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    /* - To do vscroll - */
    SDL_Rect srcrect = {0}, dstrect = {0};
    dstrect.h = bubble->height - TEXT_BUBBLE_COPY_PAD * 2;
    if (bubble->vscroll > dstrect.h) {
        bubble->vscroll      = 0;
        bubble->vscroll_anim = false;
        return;
    }
    dstrect.w = bubble->width  - TEXT_BUBBLE_COPY_PAD * 2;

    dstrect.h -= bubble->vscroll;
    dstrect.x = TEXT_BUBBLE_COPY_PAD + TEXT_BUBBLE_RENDER_PAD;
    dstrect.y = TEXT_BUBBLE_COPY_PAD + TEXT_BUBBLE_RENDER_PAD;
    if (bubble->vscroll_dir == TEXT_BUBBLE_VSCROLL_BOTTOM) {
        dstrect.y += bubble->vscroll;
    } else {
        srcrect.y = bubble->vscroll;
    }
    srcrect.w = dstrect.w;
    srcrect.h = dstrect.h;

    SDL_assert(bubble->texture_vscroll != NULL);
    SDL_RenderCopy(renderer, bubble->texture_vscroll, &srcrect, &dstrect);
}


/* --- Scrolling --- */
void TextBubble_Write(struct Text_Bubble *bubble, SDL_Renderer *renderer) {
    /* - name - */
    int x = bubble->padding.left + TEXT_BUBBLE_RENDER_PAD, y;
    int scroll_len_rem = bubble->pixelfont->scroll_len;

    /* - find the start line if vertical scroll - */
    int start_line = 0;
    if (bubble->line_num_max > 0) {
        int lines_to_render = 0;
        for (int i = 0; i < bubble->lines.line_num; i++) {
            lines_to_render++;
            scroll_len_rem -= bubble->lines.lines_len[i];
            if (scroll_len_rem <= 0)
                break;
        }
        int max_lines = bubble->line_num_max;
        start_line = (lines_to_render > max_lines) ? ((lines_to_render - 1) / max_lines) * max_lines : 0;
    }

    /* - Render the lines - */
    scroll_len_rem = bubble->pixelfont->scroll_len;

    for (int i = 0; i < bubble->lines.line_num; i++) {
        int draw_i      = bubble->line_num_max > 0 ? i % bubble->line_num_max : i;
        int line_len    = bubble->lines.lines_len[i];
        int to_render   = (line_len >= scroll_len_rem) ? scroll_len_rem : line_len;

        if ((bubble->line_num_max > 0) && (i < start_line)) {
            scroll_len_rem -= to_render;
            continue;
        }

        y = bubble->padding.top + bubble->row_height * draw_i + TEXT_BUBBLE_RENDER_PAD;
        if (!bubble->scroll) {
            PixelFont_Write_Len(bubble->pixelfont, renderer, bubble->lines.lines[i], x, y);
            continue;
        }

        /* Exit if not more scrolled chatacters to print */
        if (scroll_len_rem <= 0)
            break;

        /* -- Scrolling -- */
        SDL_assert(bubble->lines.lines_len[i] > 0);

        scroll_len_rem -= to_render;
        PixelFont_Write(bubble->pixelfont, renderer, bubble->lines.lines[i], to_render, x, y);
    }

}

/* --- Drawing --- */
void TextBubble_Update(struct Text_Bubble *bubble, struct n9Patch *n9patch,
                       SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(bubble != NULL);
    SDL_assert(renderer != NULL);
    /* - variable declaration/ ants definition - */
    SDL_assert(bubble->width            > 0);
    SDL_assert(bubble->height           > 0);
    SDL_assert(n9patch->scale.x         > 0);
    SDL_assert(n9patch->scale.y         > 0);
    SDL_assert(n9patch->size_pixels.x   > 0);
    SDL_assert(n9patch->size_pixels.y   > 0);
    SDL_assert(n9patch->size_pixels.x   == bubble->width);
    SDL_assert(n9patch->size_pixels.y   == bubble->height);

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

    if (bubble->vscroll_anim) {
        TextBubble_VScroll_Draw(bubble, renderer);
    } else {
        TextBubble_Write(bubble, renderer);
    }
    TextBubble_Tail_Draw(bubble, renderer);

    SDL_SetRenderTarget(renderer, render_target);

}

void TextBubble_Draw(struct PopUp *popup, struct Point pos,
                     SDL_Texture *target, SDL_Renderer *renderer) {
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

}
