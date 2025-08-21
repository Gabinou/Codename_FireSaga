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

typedef struct Patch {
    Point px;
    Point num;
} Patch;

typedef struct n9Patch {
    /* Note: size_patches = size_pixels / patch_pixels */
    Point scale;

    /* patch_pixels: how many pixels per patch */
    Point patch_pixels;   /* [pixels]      */
    Point patches_num;    /* [patches]     */

    Patch patch;
    /* TODO: rm. computed from patch pixels, size_patches */
    ;     /* [pixels]      */

    /* TODO: fit is unused. REMOVE */
    /* fit: amount of pixels to move last row back to fit content snuggly */
    
    struct Point fit;             /* [pixels]      */
    
    /* TODO: rm .n9patch SHOULDN'T have a pos. -> 
    **Composition, Position component */
    struct Point pos;             /* [pixels]      */

    /* texture: the actual 9patch, i.e. 3x3 patches */
    SDL_Texture *texture; 
} n9Patch;
extern const struct n9Patch n9Patch_default;

/* --- ructor/Destructor --- */
void n9Patch_Free(n9Patch *n9);

/* --- Fitting --- */
/* TODO: rm */
void n9Patch_Fit( n9Patch *n9, Point c);

Point n9Patch_Size_Pixels(  n9Patch *n9);
Point n9Patch_Size_Patches( n9Patch *n9);
Point n9Patch_Patch_Pixels( n9Patch *n9);

/* --- Setters --- */
/* Change the number of patches to best fit pixel size */
void n9Patch_Size_Pixels_Set(n9Patch *n9, Point pixels);
void n9Patch_Patch_Num_Set(n9Patch *n9, Point patches);
void n9Patch_Patch_Pixels_Set(n9Patch *n9, Point px_per_patch);

/* --- Drawing --- */
void n9Patch_Draw(n9Patch *n9, SDL_Renderer *r);

#endif /* N9PATCH_H */
