#ifndef PALETTE_H
#define PALETTE_H

#include <math.h>
#include "platform.h"
#include "types.h"
#include "enums.h"
#include "filesystem.h"
#include "debug.h"
#include "macros.h"
#include "SDL2/SDL.h"

/* --- DEFINITIONS --- */
/* -- Color Palettes -- */
extern SDL_Palette **sota_palettes;
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
extern SDL_Palette  *palette_SOTA;
extern SDL_Palette  *palette_SOTA_shadow;
extern SDL_Palette  *palette_SOTA_enemy;
extern SDL_Palette  *palette_SOTA_filter_darkred;
extern SDL_Palette  *palette_SOTA_filter_red;
extern SDL_Palette  *palette_SOTA_filter_blue;
extern SDL_Palette  *palette_SOTA_filter_green;
extern SDL_Palette  *palette_SOTA_lighten;
extern SDL_Palette  *palette_SOTA_darken;
extern SDL_Palette  *palette_SOTA_filter_purple;


/* -- Palette tables (for index shaders) -- */
extern u8 palette_table_NES_shadow[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_night[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_darkred[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_red[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_green[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_blue[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_darken[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_lighten[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_lightenmore[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_NES_filter_purple[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_shadow[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_night[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_darkred[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_red[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_green[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_blue[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_darken[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_lighten[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_lightenmore[PALETTE_NES_COLOR_NUM];
extern u8 palette_table_SOTA_filter_purple[PALETTE_NES_COLOR_NUM];

/* --- API --- */
void Palettes_Load(void);
void Palettes_Free(void);

void Palette_Tables_Load(void);

/* --- Colors Swap --- */
void Palette_Colors_Swap(SDL_Renderer  *renderer,
                         SDL_Surface  **surface, SDL_Texture **texture,
                         i8 Ow, i8 Ob, i8 Nw, i8 Nb);

#endif /* PALETTE_H */
