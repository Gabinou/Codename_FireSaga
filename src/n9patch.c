
#include "n9patch.h"

struct n9Patch n9Patch_default =  {
    .scale =         {1, 1},
    .patch_pixels =  {8, 8},
    .size_patches =  {3, 3},
    .size_pixels =   {0, 0},
    .pos =           {0, 0},
    .fit =           {0, 0},
};


void n9Patch_SDL_free(struct n9Patch *n9patch) {
    if (n9patch->texture != NULL) {
        SDL_DestroyTexture(n9patch->texture);
        n9patch->texture = NULL;
    }
}

void n9Patch_Fit(struct n9Patch *n9patch, struct Point content) {
    SDL_assert(n9patch->patch_pixels.x  > 0);
    SDL_assert(n9patch->patch_pixels.y  > 0);
    SDL_assert(content.x                > 0);
    SDL_assert(content.y                > 0);

    n9patch->size_pixels.x  = content.x;
    n9patch->size_pixels.y  = content.y;
    n9patch->size_patches.x = n9patch->size_pixels.x / n9patch->patch_pixels.x;
    n9patch->size_patches.y = n9patch->size_pixels.y / n9patch->patch_pixels.y;
    n9patch->fit.x          = n9patch->size_pixels.x % n9patch->patch_pixels.x;
    n9patch->fit.y          = n9patch->size_pixels.y % n9patch->patch_pixels.y;
    if (n9patch->fit.x > 0) {
        n9patch->size_patches.x++;
        n9patch->fit.x = n9patch->patch_pixels.x - n9patch->fit.x;
    }
    if (n9patch->fit.y > 0) {
        n9patch->size_patches.y++;
        n9patch->fit.y = n9patch->patch_pixels.y - n9patch->fit.y;
    }
}

void n9Patch_Draw(struct n9Patch *n9patch, struct SDL_Renderer *renderer) {
    SDL_assert(n9patch);
    SDL_assert(n9patch->patch_pixels.x > 0);
    SDL_assert(n9patch->patch_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_assert(n9patch->size_patches.x > 0);
    SDL_assert(n9patch->size_patches.y > 0);

    SDL_Rect srcrect, dstrect;
    int texture_x, texture_y, texture_id;
    int size_x = n9patch->size_patches.x, size_y = n9patch->size_patches.y;

    for (int y = 0; y < size_y; y++) {
        for (int x = 0; x < size_x; x++) {
            dstrect.w = n9patch->patch_pixels.x * n9patch->scale.x;
            dstrect.h = n9patch->patch_pixels.y * n9patch->scale.y;
            srcrect.w = n9patch->patch_pixels.x;
            srcrect.h = n9patch->patch_pixels.y;
            /* -- Get patch texture -- */
            /* 0 when patch == 0, 1 if x between, 2 if patch == size_patches - 1 */
            texture_x = 2 * (x == (size_x - 1)) + (x > 0) * (x < (size_x - 1));
            texture_y = 2 * (y == (size_y - 1)) + (y > 0) * (y < (size_y - 1));
            texture_id = texture_x + texture_y * N9PATCH_PATCHES_COL_LEN;
            srcrect.x = texture_id % N9PATCH_PATCHES_COL_LEN * n9patch->patch_pixels.x;
            srcrect.y = texture_id / N9PATCH_PATCHES_ROW_LEN * n9patch->patch_pixels.y;

            /* -- Draw patch texture -- */
            dstrect.x = x * n9patch->patch_pixels.x * n9patch->scale.x + n9patch->pos.x;
            dstrect.y = y * n9patch->patch_pixels.y * n9patch->scale.y + n9patch->pos.y;
            if (x == size_x - 2) {
                srcrect.w -= n9patch->fit.x;
                dstrect.w -= n9patch->fit.x * n9patch->scale.x;
            }
            if (y == size_y - 2) {
                srcrect.h -= n9patch->fit.y;
                dstrect.h -= n9patch->fit.y * n9patch->scale.y;
            }
            if (x == size_x - 1)
                dstrect.x -= n9patch->fit.x;
            if (y == size_y - 1)
                dstrect.y -= n9patch->fit.y;

            SDL_RenderCopy(renderer, n9patch->texture, &srcrect, &dstrect);
        }
    }
}
