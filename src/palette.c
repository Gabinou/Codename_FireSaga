
#include "palette.h"
#include "debug.h"
#include "jsonio.h"
#include "macros.h"
#include "filesystem.h"
#include "platform.h"
#include "stb_sprintf.h"

SDL_Palette **sota_palettes = NULL;

#define REGISTER_ENUM(x) SDL_Palette *palette_##x = NULL;
    #include "names/palettes.h"
#undef REGISTER_ENUM

void Palettes_Load(void) {
 
#define REGISTER_ENUM(x) palette_##x = SDL_AllocPalette(PALETTE_SOTA_COLOR_NUM);
    #include "names/palettes.h"
#undef REGISTER_ENUM

    char *path;
#define REGISTER_ENUM(x) path = PATH_JOIN("assets", "palettes", "palette"STRINGIZE(x)".json");\
    Palette_readJSON(path, palette_##x);
    #include "names/palettes.h"
#undef REGISTER_ENUM

    Palettes_Free();
    sota_palettes = SDL_calloc(PALETTE_NUM, sizeof(*sota_palettes));

#define REGISTER_ENUM(x) sota_palettes[PALETTE_##x] = palette_##x;
    #include "names/palettes.h"
#undef REGISTER_ENUM

    SDL_assert(palette_SOTA != NULL);
}

void Palettes_Free(void) {
    if (sota_palettes == NULL)
        return;

    for (size_t i = 0; i < PALETTE_NUM; i++) {
        if (sota_palettes[i] != NULL)
            SDL_FreePalette(sota_palettes[i]);
    }
    SDL_free(sota_palettes);
    sota_palettes = NULL;
}

#define REGISTER_ENUM(x) u8 palette_table_##x[PALETTE_SOTA_COLOR_NUM];
    #include "names/palettes.h"
#undef REGISTER_ENUM

void Palette_Tables_Load(void) {
    char *path;
#define REGISTER_ENUM(x)     path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_##x)".json");\
    PaletteTable_readJSON(path, palette_table_##x);
    #include "names/palettes.h"
#undef REGISTER_ENUM
}

void Palette_Colors_Swap(SDL_Palette *palette, SDL_Renderer *renderer,
                         SDL_Surface **surface, SDL_Texture **texture,
                         i8 Oldw, i8 Oldb, i8 NEWw, i8 NEWb) {
    SDL_assert(palette != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED((*surface)->format->format));

    SDL_assert(Oldw < palette->ncolors);
    SDL_assert(Oldb < palette->ncolors);
    SDL_assert(NEWw < palette->ncolors);
    SDL_assert(NEWb < palette->ncolors);

    /* Save old colors from palette_NES */
    /* before changing anything */
    SDL_Color old_white, old_black, new_white, new_black;

    if (NEWw > -1) {
        old_white = palette->colors[Oldw];
        new_white = palette->colors[NEWw];
    }
    if (NEWb > -1) {
        old_black = palette->colors[Oldb];
        new_black = palette->colors[NEWb];
    }

    /* Swap colors in palette_NES */
    if (NEWw > -1) {
        palette->colors[Oldw].r = new_white.r;
        palette->colors[Oldw].g = new_white.g;
        palette->colors[Oldw].b = new_white.b;
    }
    if (NEWb > -1) {
        palette->colors[Oldb].r = new_black.r;
        palette->colors[Oldb].g = new_black.g;
        palette->colors[Oldb].b = new_black.b;
    }

    /* Swap palette of font surface, texture */
    SDL_Surface *buffer = Filesystem_Surface_Palette_Swap(*surface, palette_SOTA);
    SDL_FreeSurface(*surface);
    *surface = buffer;
    if (*texture != NULL)
        SDL_DestroyTexture(*texture);

    *texture = SDL_CreateTextureFromSurface(renderer, *surface);

    /* Swap colors BACK in palette_NES */
    if (NEWw > -1) {
        palette->colors[Oldw].g = old_white.g;
        palette->colors[Oldw].r = old_white.r;
        palette->colors[Oldw].b = old_white.b;
    }
    if (NEWb > -1) {
        palette->colors[Oldb].r = old_black.r;
        palette->colors[Oldb].g = old_black.g;
        palette->colors[Oldb].b = old_black.b;
    }
}