
#include "n9patch.h"

const n9Patch n9Patch_default =  {
    .scale      = {1, 1},
    .px         = {8, 8},
    .num        = {3, 3},
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
    /* Mid patch if between 0 and patch.num */
    Point out = {
        .x = ((p.x > 0) && (p.x < (n9->num.x - 1))),
        .y = ((p.y > 0) && (p.y < (n9->num.y - 1))),
    };
    return (out);
}

Point n9Patch_End(const n9Patch *n9, Point p) {
    /* End patch if between in number of patches */
    Point out = {
        .x = (p.x == (n9->num.x - 1)),
        .y = (p.y == (n9->num.y - 1))
    };
    return (out);
}

Point n9Patch_Pixels_Total(const n9Patch *n9) {
    Point out = {
        .x  = n9->num.x * n9->px.x,
        .y  = n9->num.y * n9->px.y
    };
    return (out);
}

Point n9Patch_Num(const n9Patch *n9) {
    return (n9->num);
}

Point n9Patch_Pixels_Patch(const n9Patch *n9) {
    return (n9->px);
}

Point n9Patch_Pixels_Total_Set(n9Patch *n9, Point size) {
    /* Changing number of patches to best fit
    **  input total pixel size.
    **  Notes:
    **    1. / truncates to 0, so contents MIGHT be cut
    **    2. term 2 adds one patch IFF remainder > 0
    /*  */
    SDL_assert(n9->px.x > 0);
    SDL_assert(n9->px.y > 0);
    Point remainder = {
        .x = ((size.x % n9->px.x) > 0),
        .y = ((size.y % n9->px.y) > 0)
    };
    n9->num.x = size.x / n9->px.x + remainder.x;
    n9->num.y = size.y / n9->px.y + remainder.y;
    return (n9Patch_Pixels_Total(n9));
}

int round_closest(int numer, int denom) {
    return (numer + (denom / 2)) / denom;
}

void n9Patch_Fit(n9Patch *n9, Point content) {
    /* Fit patch to content
    **  1. Set pixel size to input contents
    **  2. Center content in new total size */
    SDL_assert(n9->px.x     > 0);
    SDL_assert(n9->px.y     > 0);
    SDL_assert(content.x    > 0);
    SDL_assert(content.y    > 0);

    Point size_pixels  = n9Patch_Pixels_Total_Set(n9, content);
    SDL_assert(size_pixels.x >= content.x);
    SDL_assert(size_pixels.y >= content.y);

    /* Centering content */
    n9->_fit.x = (size_pixels.x - content.x);
    n9->_fit.y = (size_pixels.y - content.y);
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
    SDL_assert(n9->px.x     > 0);
    SDL_assert(n9->px.y     > 0);
    SDL_assert(n9->scale.x  > 0);
    SDL_assert(n9->scale.y  > 0);
    SDL_assert(n9->num.x    > 0);
    SDL_assert(n9->num.y    > 0);

    int texture_id;
    SDL_Rect srcrect;
    SDL_Rect dstrect;

    int num = n9->num.x * n9->num.y;
    for (int i = 0; i < num; i++) {
        int x = i % n9->num.x;
        int y = i / n9->num.x;

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
        /* Edge - 1 patch:
        **  - Reduce source patch _fit pixels wide/high to
        **      prevent mid patch from showing through edge
        **      patches empty pixels */
        if (x == n9->num.x - 2) {
            srcrect.w -= n9->_fit.x;
            dstrect.w -= n9->_fit.x * n9->scale.x;
        }
        if (y == n9->num.y - 2) {
            srcrect.h -= n9->_fit.y;
            dstrect.h -= n9->_fit.y * n9->scale.y;
        }
        /* Edge patches:
        **  - Move rendering _fit pixels up/left to compensate
                for removed pixels in edge - 1 patch */
        if (x == n9->num.x - 1)
            dstrect.x -= n9->_fit.x * n9->scale.x;
        if (y == n9->num.y - 1)
            dstrect.y -= n9->_fit.y * n9->scale.y;

        SDL_RenderCopy( renderer, n9->texture,
                        &srcrect, &dstrect);
    }
}