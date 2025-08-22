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
    N9PATCH_START   = 0,
    N9PATCH_MID     = 1,
    N9PATCH_END     = 2,
};

typedef struct n9Patch {
    Point scale;

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

    /* fit: move last row back to fit content snuggly.
    ** Computed automatically in n9Patch_Fit*/
    Point _fit; /* [px] */

    /* texture: the actual 9patch, i.e. 3x3 patches */
    SDL_Texture *texture;
} n9Patch;
extern const n9Patch n9Patch_default;

/* --- Constructor/Destructor --- */
void n9Patch_Free(n9Patch *n9);

/* --- Getters --- */
/* Number of patches */
Point n9Patch_Num(          const n9Patch *n9);
/* Number of pixels in one patch */
Point n9Patch_Pixels_Patch( const n9Patch *n9);
/* Total size in pixels of the whole n9Patch */
Point n9Patch_Pixels_Total( const n9Patch *n9);

/* --- Internals --- */
i32 n9Patch_Id(const n9Patch *n9, Point p);

void n9Patch_Fit(n9Patch *n9, Point content);

/* -- Patch types -- */
/* Change the number of patches to best fit pixel size */
Point n9Patch_Start(  const n9Patch *n9, Point p);
Point n9Patch_Mid(    const n9Patch *n9, Point p);
Point n9Patch_End(    const n9Patch *n9, Point p);

/* --- Setters --- */
/* Change the number of patches to best fit pixel size */
void n9Patch_Num_Set(          n9Patch *n9, Point in);
void n9Patch_Pixels_Total_Set( n9Patch *n9, Point in);
void n9Patch_Pixels_Patch_Set( n9Patch *n9, Point in);

/* --- Drawing --- */
void n9Patch_Draw(const n9Patch *n9, SDL_Renderer *r);

#endif /* N9PATCH_H */
