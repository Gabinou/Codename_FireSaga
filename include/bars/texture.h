#ifndef TEXTURE_BAR_H
#define TEXTURE_BAR_H

#include <math.h>
#include "utilities.h"
#include "SDL2/SDL.h"

/* --- TextureBar --- */
// Bar drawn with textures. Essentially an improved n3patch.
// Textures arranged in 2D array, 3 col, n rows, more fill with n.
// Has more texture rows to draw additional fill states.
// Textures used determined by fill state and row number

struct TextureBar {
    SDL_Texture *textures;  /* [row * col_len + col] */
    int row_len; /* number of rows of textures */
    int len; /* [patches] */
    float fill;
    struct Point pos; // [pixels]
    struct Point scale;
    struct Point size_pixels; // [pixels]
};
extern struct TextureBar TextureBar_default;

/* -- Drawing -- */
void TextureBar_Draw(struct TextureBar *tb, SDL_Renderer *r);

#endif /* TEXTURE_BAR_H */
