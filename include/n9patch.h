#ifndef N9PATCH_H
#define N9PATCH_H

#include "enums.h"
#include "types.h"
#include "structs.h"
#include "SDL.h"

/* --- ENUMS --- */
enum N9PATCH {
    /*  */
    N9PATCH_ROW_LEN = 3,
    N9PATCH_COL_LEN = 3,
};

typedef struct n9Patch {
    Point scale;

    /* TODO: rm patch_pixels */
    /* patch_pixels: how many pixels per patch */
    Point patch_pixels;   /* [pixels]      */
    /* TODO: rm patches_num */
    Point size_pixels;    /* [patches]     */
    Point size_patches;    /* [patches]     */

    /* px: Pixels size of one patch. */
    Point px;
    /* num: Number of patches to render. */
    Point num;
    /* TODO: rm. computed from patch pixels, size_patches */
    ;     /* [pixels]      */

    /* Position of the n9patch relative to renderer.
    **  Note: Needed cause n9patch is member of menu, and
    **          menu needs to position n9patch sometimes. */
    Point pos; /* [pixels]      */

    /* texture: the actual 9patch, i.e. 3x3 patches */
    SDL_Texture *texture;
} n9Patch;
extern const n9Patch n9Patch_default;

/* --- Constructor/Destructor --- */
void n9Patch_Free(n9Patch *n9);

/* --- Getters --- */
Point n9Patch_Num(          const n9Patch *n9);
Point n9Patch_Pixels_Total( const n9Patch *n9);
Point n9Patch_Pixels_Patch( const n9Patch *n9);

/* --- Internals --- */
i32 n9Patch_Id(const n9Patch *n9, int patch_x, int patch_y);

/* --- Setters --- */
/* Change the number of patches to best fit pixel size */
void n9Patch_Num_Set(          n9Patch *n9, Point in);
void n9Patch_Pixels_Total_Set( n9Patch *n9, Point in);
void n9Patch_Pixels_Patch_Set( n9Patch *n9, Point in);

/* --- Drawing --- */
void n9Patch_Draw(n9Patch *n9, SDL_Renderer *r);

#endif /* N9PATCH_H */
