#ifndef N9PATCH_H
#define N9PATCH_H

#include "enums.h"
// #include "nmath.h"
#include "structs.h"
#include "globals.h"
#include "macros.h"
#include "debug.h"
#include "types.h"
#include "SDL2/SDL.h"

/* --- ENUMS --- */
enum N9PATCH {
    N9PATCH_PATCHES_ROW_LEN = 3,
    N9PATCH_PATCHES_COL_LEN = 3,
};

typedef struct n9Patch {
    /* Note: size_patches = size_pixels / patch_pixels */
    struct Point scale;
    // patch_pixels: how many pixels per patch
    struct Point patch_pixels;    /* [pixels]     */
    struct Point size_patches;    /* [patches]    */
    struct Point size_pixels;     /* [pixels]     */
    // fit: amount of pixels to move last row back to fit content snuggly
    struct Point fit;             /* [pixels]     */
    struct Point pos;             /* [pixels]     */
    SDL_Texture *texture;                       /* texture is 3x3 patches */
} n9Patch;
extern struct n9Patch n9Patch_default;

/* --- Constructor/Destructor --- */
void n9Patch_Free(struct n9Patch *n9);

/* --- Fitting --- */
void n9Patch_Fit(        struct n9Patch *n9, struct Point c);

/* --- Drawing --- */
void n9Patch_Draw(struct n9Patch *n9, struct SDL_Renderer *r);

#endif /* N9PATCH_H */
