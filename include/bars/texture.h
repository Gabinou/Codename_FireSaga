#ifndef TEXTURE_BAR_H
#define TEXTURE_BAR_H

#include "structs.h"
#include "SDL.h"

/* --- TextureBar --- */
// Bar drawn with textures. Essentially an improved n3patch.
// Textures arranged in 2D array, 3 col, n rows, more fill with n.
// Has more texture rows to draw additional fill states.
// Textures used determined by fill state and row number

struct TextureBar {
    SDL_Texture *textures;  /* [row * col_len + col] */
    struct Point pos; // [pixels]
    struct Point scale;
    struct Point size_pixels; // [pixels]
    i32 row_len; /* number of rows of textures */
    i32 len; /* [patches] */
    float fill;
};
extern const struct TextureBar TextureBar_default;

/* -- Drawing -- */
void TextureBar_Draw(struct TextureBar *tb, SDL_Renderer *r);

#endif /* TEXTURE_BAR_H */
