#ifndef PALETTE_H
#define PALETTE_H

#include <math.h>
#include "platform.h"
#include "types.h"
#include "enums.h"
#include "filesystem.h"
#include "debug.h"
#include "macros.h"
#include "SDL.h"

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
extern u8 palette_table_NES_shadow[];
extern u8 palette_table_NES_night[];
extern u8 palette_table_NES_filter_darkred[];
extern u8 palette_table_NES_filter_red[];
extern u8 palette_table_NES_filter_green[];
extern u8 palette_table_NES_filter_blue[];
extern u8 palette_table_NES_darken[];
extern u8 palette_table_NES_lighten[];
extern u8 palette_table_NES_filter_lightenmore[];
extern u8 palette_table_NES_filter_purple[];

/* --- API --- */
void Palettes_Load();
void Palettes_SDL_free();

void Palette_Tables_Load();

#endif /* PALETTE_H */
