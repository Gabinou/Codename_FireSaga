#ifndef N9PATCH_H
#define N9PATCH_H

#include "enums.h"
#include "types.h"
#include "structs.h"
#include "SDL.h"

/* --- ENUMS --- */
enum N9PATCH {
    N9PATCH_PATCHES_ROW_LEN = 3,
    N9PATCH_PATCHES_COL_LEN = 3,
};

typedef struct n9Patch {
    /* Note: size_patches = size_pixels / patch_pixels */
    Point scale;

    /* patch_pixels: how many pixels per patch */
    /* TODO: struct in struct, simpler names */
    Point patch_pixels;    /* [pixels]      */
    Point size_patches;    /* [patches]     */
    Point size_pixels;     /* [pixels]      */

    /* TODO: fit is unused. REMOVE */
    /* fit: amount of pixels to move last row back to fit content snuggly */
    
    struct Point fit;             /* [pixels]      */
    
    /* TODO: n9patch SHOULDN'T have a pos. -> 
    **Composition, Position component */
    struct Point pos;             /* [pixels]      */
    SDL_Texture *texture; /* texture is 3x3 patches */
} n9Patch;
extern const struct n9Patch n9Patch_default;

/* --- ructor/Destructor --- */
void n9Patch_Free(struct n9Patch *n9);

/* --- Fitting --- */
void n9Patch_Fit( struct n9Patch *n9, struct Point c);

/* --- Drawing --- */
void n9Patch_Draw(struct n9Patch *n9, struct SDL_Renderer *r);

#endif /* N9PATCH_H */
