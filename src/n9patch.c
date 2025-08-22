
#include "n9patch.h"

const n9Patch n9Patch_default =  {
    .scale =         {1, 1},
    .patch_pixels =  {8, 8},
    .size_patches =  {3, 3},
};

void n9Patch_Free(n9Patch *n9) {
    if (n9->texture != NULL) {
        SDL_DestroyTexture(n9->texture);
        n9->texture = NULL;
    }
}

Point n9Patch_Pixels_Total(const n9Patch *n9) {
    Point out = {
        .x  = n9->size_patches.x * n9->patch_pixels.x,
        .y  = n9->size_patches.y * n9->patch_pixels.y
    };
    return (out);
}

Point n9Patch_Num(const n9Patch *n9) {
    return (n9->size_patches);
}

Point n9Patch_Pixels_Patch(const n9Patch *n9) {
    return (n9->patch_pixels);
}

void n9Patch_Num_Set(n9Patch *n9, Point num) {
    n9->size_patches = num;
}

void n9Patch_Pixels_Total_Set(n9Patch *n9, Point size) {
    /* Setting number of patches size */
    n9->size_patches.x = size.x / n9->patch_pixels.x;
    n9->size_patches.y = size.y / n9->patch_pixels.y;
}

void n9Patch_Pixels_Patch_Set(n9Patch *n9, Point px) {
    n9->patch_pixels = px;
}

void n9Patch_Draw(  n9Patch         *n9,
                    SDL_Renderer    *renderer) {
    SDL_assert(n9);
    SDL_assert(n9->patch_pixels.x  > 0);
    SDL_assert(n9->patch_pixels.y  > 0);
    SDL_assert(n9->scale.x         > 0);
    SDL_assert(n9->scale.y         > 0);
    SDL_assert(n9->size_patches.x  > 0);
    SDL_assert(n9->size_patches.y  > 0);

    SDL_Rect srcrect, dstrect;
    int texture_x, texture_y, texture_id;
    int size_x = n9->size_patches.x;
    int size_y = n9->size_patches.y;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            dstrect.w = n9->patch_pixels.x * n9->scale.x;
            dstrect.h = n9->patch_pixels.y * n9->scale.y;
            srcrect.w = n9->patch_pixels.x;
            srcrect.h = n9->patch_pixels.y;
            /* -- Get patch texture -- */
            /* 0 when patch == 0, 1 if x between, 2 if patch == size_patches - 1 */
            texture_x = 2 * (x == (size_x - 1)) + (x > 0) * (x < (size_x - 1));
            texture_y = 2 * (y == (size_y - 1)) + (y > 0) * (y < (size_y - 1));
            texture_id = texture_x + texture_y * N9PATCH_PATCHES_COL_LEN;
            srcrect.x = texture_id % N9PATCH_PATCHES_COL_LEN * n9->patch_pixels.x;
            srcrect.y = texture_id / N9PATCH_PATCHES_ROW_LEN * n9->patch_pixels.y;

            /* -- Draw patch texture -- */
            dstrect.x = x * n9->patch_pixels.x * n9->scale.x + n9->pos.x;
            dstrect.y = y * n9->patch_pixels.y * n9->scale.y + n9->pos.y;

            SDL_RenderCopy(renderer, n9->texture, &srcrect, &dstrect);
        }
    }
}
