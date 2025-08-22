
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
    /* Total size in pixels of the whole n9patch */
    Point out = {
        .x  = n9->size_patches.x * n9->patch_pixels.x,
        .y  = n9->size_patches.y * n9->patch_pixels.y
    };
    return (out);
}

Point n9Patch_Num(const n9Patch *n9) {
    /* Number of patches */
    return (n9->size_patches);
}

Point n9Patch_Pixels_Patch(const n9Patch *n9) {
    /* Number of pixels in one patch */
    return (n9->patch_pixels);
}

void n9Patch_Num_Set(n9Patch *n9, Point num) {
    /* Set number of patches */
    n9->size_patches = num;
}

void n9Patch_Pixels_Total_Set(n9Patch *n9, Point size) {
    /* Changing number of patches to best fit
    **  input total pixel size */
    n9->size_patches.x = size.x / n9->patch_pixels.x;
    n9->size_patches.y = size.y / n9->patch_pixels.y;
}

void n9Patch_Pixels_Patch_Set(n9Patch *n9, Point px) {
    /* Set number of pixels in one patch */
    n9->patch_pixels = px;
}

int n9Patch_Id( const n9Patch *n9,
                int patch_x, int patch_y) {
    /* ID of patch texture */
    int texture_x = 2 * (patch_x == (n9->size_patches.x - 1)) +
                    (patch_x > 0) * (patch_x < (n9->size_patches.x - 1));
    int texture_y = 2 * (patch_y == (n9->size_patches.y - 1)) +
                    (patch_y > 0) * (patch_y < (n9->size_patches.y - 1));
    return (texture_x + texture_y * N9PATCH_COL_LEN);
}

void n9Patch_Draw(  n9Patch         *n9,
                    SDL_Renderer    *renderer) {
    SDL_assert(n9);
    SDL_assert(n9->texture);
    SDL_assert(n9->patch_pixels.x  > 0);
    SDL_assert(n9->patch_pixels.y  > 0);
    SDL_assert(n9->scale.x         > 0);
    SDL_assert(n9->scale.y         > 0);
    SDL_assert(n9->size_patches.x  > 0);
    SDL_assert(n9->size_patches.y  > 0);

    int texture_id;
    SDL_Rect dstrect = {
        .w = n9->patch_pixels.x * n9->scale.x,
        .h = n9->patch_pixels.y * n9->scale.y,
    };
    SDL_Rect srcrect = {
        .w = n9->patch_pixels.x,
        .h = n9->patch_pixels.y,
    };

    for (int y = 0; y < n9->size_patches.y; y++) {
        for (int x = 0; x < n9->size_patches.x; x++) {
            /* -- Get patch texture -- */
            /* 0 when patch == 0, 1 if x between, 2 if patch == size_patches - 1 */
            texture_id = n9Patch_Id(n9, x, y);
            srcrect.x = texture_id % N9PATCH_COL_LEN * n9->patch_pixels.x;
            srcrect.y = texture_id / N9PATCH_ROW_LEN * n9->patch_pixels.y;

            /* -- Draw patch texture -- */
            dstrect.x = x * n9->patch_pixels.x * n9->scale.x + n9->pos.x;
            dstrect.y = y * n9->patch_pixels.y * n9->scale.y + n9->pos.y;

            SDL_RenderCopy(renderer, n9->texture, &srcrect, &dstrect);
        }
    }
}
