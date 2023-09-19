
#include "n9patch.h"

struct n9Patch n9Patch_default =  {
    .scale =         {1, 1},
    .patch_pixels =  {8, 8},
    .size_patches =  {3, 3},
    .size_pixels =   {0, 0},
    .pos =           {0, 0},
};
void n9Patch_Free(struct n9Patch *n9patch) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (n9patch->texture != NULL) {
        SDL_DestroyTexture(n9patch->texture);
        n9patch->texture = NULL;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void n9Patch_Draw(struct n9Patch *n9patch, struct SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(n9patch);
    SDL_assert(n9patch->patch_pixels.x > 0);
    SDL_assert(n9patch->patch_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_assert(n9patch->size_patches.x > 0);
    SDL_assert(n9patch->size_patches.y > 0);

    SDL_Rect srcrect, dstrect;
    srcrect.w = n9patch->patch_pixels.x;
    srcrect.h = n9patch->patch_pixels.y;
    dstrect.w = n9patch->patch_pixels.x * n9patch->scale.x;
    dstrect.h = n9patch->patch_pixels.y * n9patch->scale.y;
    if32 texture_x, texture_y, texture_id;
    if32 size_x = n9patch->size_patches.x, size_y = n9patch->size_patches.y;

    for (if32 patch_y = 0; patch_y < size_y; patch_y++) {
        for (if32 patch_x = 0; patch_x < size_x; patch_x++) {
            dstrect.x = patch_x * n9patch->patch_pixels.x * n9patch->scale.x + n9patch->pos.x;
            dstrect.y = patch_y * n9patch->patch_pixels.y * n9patch->scale.y + n9patch->pos.y;
            /* 0 when patch == 0, 1 if patch_x between, 2 if patch == size_patches - 1 */
            texture_x = 2 * (patch_x == (size_x - 1)) + (patch_x > 0) * (patch_x < (size_x - 1));
            texture_y = 2 * (patch_y == (size_y - 1)) + (patch_y > 0) * (patch_y < (size_y - 1));
            texture_id = texture_x + texture_y * N9PATCH_PATCHES_COL_LEN;
            srcrect.x = texture_id % N9PATCH_PATCHES_COL_LEN * n9patch->patch_pixels.x;
            srcrect.y = texture_id / N9PATCH_PATCHES_ROW_LEN * n9patch->patch_pixels.y;
            SDL_RenderCopy(renderer, n9patch->texture, &srcrect, &dstrect);
        }
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
