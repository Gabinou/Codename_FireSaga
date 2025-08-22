
#include "n9patch.h"

const n9Patch n9Patch_default =  {
    .scale          = {1, 1},
    .num            = {8, 8},
    .size_patches   = {3, 3},
};

void n9Patch_Free(n9Patch *n9) {
    if (n9->texture != NULL) {
        SDL_DestroyTexture(n9->texture);
        n9->texture = NULL;
    }
}

Point n9Patch_Start(const n9Patch *n9, Point p) {
    /* Edge patch if index is 0 */
    Point out = {.x = (p.x == 0), .y = (p.y == 0)};
    return (out);
}

Point n9Patch_Mid(const n9Patch *n9, Point p) {
    /* Mid patch if between 0 and number of patches */
    Point out = {
        .x = ((p.x > 0) && (p.x < (n9->size_patches.x - 1))),
        .y = ((p.y > 0) && (p.y < (n9->size_patches.y - 1))),
    };
    return (out);
}

Point n9Patch_End(const n9Patch *n9, Point p) {
    /* End patch if between in number of patches */
    Point out = {
        .x = (p.x == (n9->size_patches.x - 1)),
        .y = (p.y == (n9->size_patches.y - 1))
    };
    return (out);
}

Point n9Patch_Pixels_Total(const n9Patch *n9) {
    Point out = {
        .x  = n9->size_patches.x * n9->px.x,
        .y  = n9->size_patches.y * n9->px.y
    };
    return (out);
}

Point n9Patch_Num(const n9Patch *n9) {
    return (n9->size_patches);
}

Point n9Patch_Pixels_Patch(const n9Patch *n9) {
    return (n9->px);
}

void n9Patch_Num_Set(n9Patch *n9, Point num) {
    /* Set number of patches */
    n9->size_patches = num;
}

void n9Patch_Pixels_Total_Set(n9Patch *n9, Point size) {
    /* Changing number of patches to best fit
    **  input total pixel size */
    n9->size_patches.x = size.x / n9->px.x;
    n9->size_patches.y = size.y / n9->px.y;
}

void n9Patch_Pixels_Patch_Set(n9Patch *n9, Point px) {
    /* Set number of pixels in one patch */
    n9->px = px;
}

void n9Patch_Fit(n9Patch *n9, Point content) {
    /* Fit patch to content */
    SDL_assert(n9->px.x  > 0);
    SDL_assert(n9->px.y  > 0);
    SDL_assert(content.x           > 0);
    SDL_assert(content.y           > 0);

    n9->size_pixels.x  = content.x;
    n9->size_pixels.y  = content.y;
    n9->size_patches.x = n9->size_pixels.x / n9->px.x;
    n9->size_patches.y = n9->size_pixels.y / n9->px.y;
    n9->_fit.x         = n9->size_pixels.x % n9->px.x;
    n9->_fit.y         = n9->size_pixels.y % n9->px.y;
    if (n9->_fit.x > 0) {
        n9->size_patches.x++;
        n9->_fit.x = n9->px.x - n9->_fit.x;
    }
    if (n9->_fit.y > 0) {
        n9->size_patches.y++;
        n9->_fit.y = n9->px.y - n9->_fit.y;
    }
}

int n9Patch_Id( const n9Patch *n9, Point p) {
    /* ID of patch texture:
    /*  - 0 if start patch, | (0,0) | (1,0) | (2,0) |
    **  - 1 if mid patch,   | (0,1) | (1,1) | (2,1) |
    **  - 2 if end patch    | (0,2) | (1,2) | (2,2) | */
    Point end = n9Patch_End(n9, p);
    Point mid = n9Patch_Mid(n9, p);
    int x = N9PATCH_END * end.x + N9PATCH_MID * mid.x;
    int y = N9PATCH_END * end.y + N9PATCH_MID * mid.y;
    return (x + y * N9PATCH_COL_LEN);
}

void n9Patch_Draw(  const n9Patch   *n9,
                    SDL_Renderer    *renderer) {
    SDL_assert(n9);
    SDL_assert(n9->texture);
    SDL_assert(n9->px.x             > 0);
    SDL_assert(n9->px.y             > 0);
    SDL_assert(n9->scale.x          > 0);
    SDL_assert(n9->scale.y          > 0);
    SDL_assert(n9->size_patches.x   > 0);
    SDL_assert(n9->size_patches.y   > 0);

    int texture_id;
    SDL_Rect srcrect;
    SDL_Rect dstrect;

    for (int y = 0; y < n9->size_patches.y; y++) {
        for (int x = 0; x < n9->size_patches.x; x++) {
            /* -- Reset rects -- */
            dstrect.w = n9->px.x * n9->scale.x;
            dstrect.h = n9->px.y * n9->scale.y;
            srcrect.w = n9->px.x;
            srcrect.h = n9->px.y;

            /* -- Get patch texture -- */
            Point patch = {.x = x, .y = y};
            texture_id = n9Patch_Id(n9, patch);
            srcrect.x = texture_id % N9PATCH_COL_LEN * n9->px.x;
            srcrect.y = texture_id / N9PATCH_ROW_LEN * n9->px.y;

            /* -- Draw patch texture -- */
            dstrect.x = x * n9->px.x * n9->scale.x + n9->pos.x;
            dstrect.y = y * n9->px.y * n9->scale.y + n9->pos.y;

            /* -- Fit rects -- */
            if (x == n9->size_patches.x - 2) {
                srcrect.w -= n9->_fit.x;
                dstrect.w -= n9->_fit.x * n9->scale.x;
            }
            if (y == n9->size_patches.y - 2) {
                srcrect.h -= n9->_fit.y;
                dstrect.h -= n9->_fit.y * n9->scale.y;
            }
            if (x == n9->size_patches.x - 1)
                dstrect.x -= n9->_fit.x;
            if (y == n9->size_patches.y - 1)
                dstrect.y -= n9->_fit.y;

            SDL_RenderCopy( renderer, n9->texture,
                            &srcrect, &dstrect);
        }
    }
}