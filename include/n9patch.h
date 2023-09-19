#ifndef N9PATCH_H
#define N9PATCH_H

#include "enums.h"
#include "nmath.h"
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
    struct nmath_point_int32_t scale;
    struct nmath_point_int32_t patch_pixels;    /* [pixels]     */
    struct nmath_point_int32_t size_patches;    /* [patches]    */
    struct nmath_point_int32_t size_pixels;     /* [pixels]     */
    struct nmath_point_int32_t pos;             /* [pixels]     */
    SDL_Texture *texture;                       /* texture is 3x3 patches */
} n9Patch;
extern struct n9Patch n9Patch_default;

void n9Patch_Draw(struct n9Patch *n9patch, struct SDL_Renderer *renderer);
void n9Patch_Free(struct n9Patch *n9patch);

#endif /* N9PATCH_H */
