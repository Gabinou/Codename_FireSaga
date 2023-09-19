#ifndef PALETTE_H
#define PALETTE_H

#include <math.h>
#include "platform.h"
#include "types.h"
#include "enums.h"
#include "globals.h"
#include "filesystem.h"
#include "debug.h"
#include "macros.h"
#include "SDL2/SDL.h"

/* --- DEFINITIONS --- */
/* -- Color Palettes -- */
extern SDL_Palette  *palette_NES;
extern SDL_Palette  *palette_NES_shadow;
extern SDL_Palette  *palette_NES_enemy;
extern SDL_Palette  *palette_NES_night;
extern SDL_Palette  *palette_NES_filter_darkred;
extern SDL_Palette  *palette_NES_filter_darkredwshadow;
extern SDL_Palette  *palette_NES_filter_red;
extern SDL_Palette  *palette_NES_filter_blue;
extern SDL_Palette  *palette_NES_filter_green;
extern SDL_Palette  *palette_NES_darken;
extern SDL_Palette  *palette_NES_lighten;
extern SDL_Palette  *palette_NES_lightenmore;
extern SDL_Palette  *palette_NES_purple;
extern SDL_Palette **sota_palettes;

/* -- Palette tables (for index shaders) -- */
extern uf8 palette_table_NES_shadow[];
extern uf8 palette_table_NES_night[];
extern uf8 palette_table_NES_filter_darkred[];
extern uf8 palette_table_NES_filter_red[];
extern uf8 palette_table_NES_filter_green[];
extern uf8 palette_table_NES_filter_blue[];
extern uf8 palette_table_NES_darken[];
extern uf8 palette_table_NES_lighten[];
extern uf8 palette_table_NES_filter_lightenmore[];
extern uf8 palette_table_NES_filter_purple[];

/* --- API --- */
extern void Palettes_Load();
extern void Palettes_Free();

extern void Palette_Tables_Load();

#endif /* PALETTE_H */
