#ifndef PALETTE_H
#define PALETTE_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "SDL.h"

/* --- DEFINITIONS --- */
/* -- Color Palettes -- */
extern SDL_Palette **sota_palettes;

#define REGISTER_ENUM(x) extern SDL_Palette *palette_##x;
    #include "names/palettes.h"
#undef REGISTER_ENUM

/* -- Palette tables (for index shaders) -- */
#define REGISTER_ENUM(x) extern u8 palette_table_##x[PALETTE_SOTA_COLOR_NUM];
    #include "names/palettes.h"
#undef REGISTER_ENUM

/* --- API --- */
void Palettes_Load(void);
void Palettes_Free(void);

void Palette_Tables_Load(void);

/* --- Colors Swap --- */
/* Change black and white to color in palette*/
void Palette_Colors_Swap(SDL_Palette *palette, SDL_Renderer  *renderer,
                         SDL_Surface  **surface, SDL_Texture **texture,
                         i8 Ow, i8 Ob, i8 Nw, i8 Nb);

#endif /* PALETTE_H */
