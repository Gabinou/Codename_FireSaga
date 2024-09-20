#include "bars/texture.h"

struct TextureBar TextureBar_default = {0};

void TextureBar_Draw(struct TextureBar *tb, SDL_Renderer *renderer) {
    /* -- Compute fill -- */
    /* - init - */
    SDL_Rect srcrect = {0, 0, tb->size_pixels.x, tb->size_pixels.y};
    SDL_Rect dstrect = {0, tb->pos.y, 0, 0};
    dstrect.w = tb->size_pixels.x * tb->scale.x;
    dstrect.h = tb->size_pixels.y * tb->scale.y;
    // Last index with filled texture,
    //  exclusive -> if last_filled is 9, fill until i == 8
    int row, col, last_filled = (int)floor(tb->fill * tb->len);

    /* -- Rendering Loop -- */
    for (int i = 0; i < tb->len; i++) {

        if (i < last_filled) {
            row = tb->row_len - 1; /* Filled texture */
        } else if (i == last_filled) {
            /* partially filled texture */
            // proportion of cell that is filled
            float fill_prop = (tb->fill * tb->len - (float)last_filled);
            row = (int)round(fill_prop * tb->row_len);
        } else {
            row = 0; /* Empty texture */
        }
        /* Left edge */
        if (i == 0) {
            col = 0; /* Left edge */
        } else if (i == (tb->len - 1)) {
            col = 2; /* Right edge */
        } else {
            col = 1; /* Middle */
        }
        srcrect.x = col * srcrect.w;
        srcrect.y = row * srcrect.h;
        dstrect.x = tb->pos.x + i * dstrect.w;

        SDL_RenderCopy(renderer, tb->textures, &srcrect, &dstrect);
    }

}

