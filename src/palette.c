
#include "palette.h"
#include "stb_sprintf.h"

SDL_Palette *palette_NES                       = NULL;
SDL_Palette *palette_NES_enemy                 = NULL;
SDL_Palette *palette_NES_filter_green          = NULL;
SDL_Palette *palette_NES_filter_darkredwshadow = NULL;
SDL_Palette *palette_NES_filter_darkred        = NULL;
SDL_Palette *palette_NES_filter_blue           = NULL;
SDL_Palette *palette_NES_filter_red            = NULL;
SDL_Palette *palette_NES_night                 = NULL;
SDL_Palette *palette_NES_shadow                = NULL;
SDL_Palette *palette_NES_darken                = NULL;
SDL_Palette *palette_NES_lighten               = NULL;
SDL_Palette *palette_NES_lightenmore           = NULL;
SDL_Palette *palette_NES_purple                = NULL;
SDL_Palette **sota_palettes                    = NULL;

void Palettes_Load(void) {
    palette_NES                        = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_enemy                  = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_green           = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_darkredwshadow  = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_darkred         = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_blue            = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_red             = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_night                  = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_shadow                 = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_darken                 = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_lighten                = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_lightenmore            = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_purple                 = SDL_AllocPalette(PALETTE_NES_COLOR_NUM);

    char *path;
    path = PATH_JOIN("assets", "palettes", "palette_NES.json");
    jsonio_Read_Palette(path, palette_NES);
    path = PATH_JOIN("assets", "palettes", "palette_NES_map_unit_enemy.json");
    jsonio_Read_Palette(path, palette_NES_enemy);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_green)".json");
    jsonio_Read_Palette(path, palette_NES_filter_green);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_darkredwshadow)".json");
    jsonio_Read_Palette(path, palette_NES_filter_darkredwshadow);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_darkred)".json");
    jsonio_Read_Palette(path, palette_NES_filter_darkred);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_blue)".json");
    jsonio_Read_Palette(path, palette_NES_filter_blue);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_red)".json");
    jsonio_Read_Palette(path, palette_NES_filter_red);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_filter_purple)".json");
    jsonio_Read_Palette(path, palette_NES_purple);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_night)".json");
    jsonio_Read_Palette(path, palette_NES_night);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_shadow)".json");
    jsonio_Read_Palette(path, palette_NES_shadow);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_darken)".json");
    jsonio_Read_Palette(path, palette_NES_darken);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_lighten)".json");
    jsonio_Read_Palette(path, palette_NES_lighten);
    path = PATH_JOIN("assets", "palettes", STRINGIZE(palette_NES_lightenmore)".json");
    jsonio_Read_Palette(path, palette_NES_lightenmore);

    Palettes_Free();
    sota_palettes = calloc(PALETTE_NUM, sizeof(*sota_palettes));
    sota_palettes[PALETTE_NULL]               = NULL;
    sota_palettes[PALETTE_NES]                = palette_NES;
    sota_palettes[PALETTE_NES_ENEMY]          = palette_NES_enemy;
    sota_palettes[PALETTE_NES_RED]            = palette_NES_filter_red;
    sota_palettes[PALETTE_NES_GREEN]          = palette_NES_filter_green;
    sota_palettes[PALETTE_NES_BLUE]           = palette_NES_filter_blue;
    sota_palettes[PALETTE_NES_DARKRED]        = palette_NES_filter_darkred;
    sota_palettes[PALETTE_NES_DARKREDwSHADOW] = palette_NES_filter_darkredwshadow;
    sota_palettes[PALETTE_NES_SHADOW]         = palette_NES_shadow;
    sota_palettes[PALETTE_NES_NIGHT]          = palette_NES_night;
    sota_palettes[PALETTE_NES_DARKEN]         = palette_NES_darken;
    sota_palettes[PALETTE_NES_LIGHTEN]        = palette_NES_lighten;
    sota_palettes[PALETTE_NES_LIGHTENMORE]    = palette_NES_lightenmore;
    sota_palettes[PALETTE_NES_PURPLE]         = palette_NES_purple;
    SDL_assert(palette_NES != NULL);

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

u8 palette_table_NES_shadow[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_night [PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_filter_darkred[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_filter_red[PALETTE_NES_COLOR_NUM];;
u8 palette_table_NES_filter_green[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_filter_blue[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_darken[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_lighten[PALETTE_NES_COLOR_NUM];
u8 palette_table_NES_lightenmore[PALETTE_NES_COLOR_NUM];

void Palette_Tables_Load(void) {
    char *path;
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_shadow)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_shadow);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_night)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_night);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_filter_darkred)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_filter_darkred);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_filter_red)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_filter_red);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_filter_green)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_filter_green);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_filter_blue)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_filter_blue);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_darken)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_darken);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_lighten)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_lighten);
    path = PATH_JOIN("assets", "palettes",  STRINGIZE(palette_table_NES_lightenmore)".json");
    jsonio_Read_PaletteTable(path, palette_table_NES_lightenmore);
}

SDL_Surface *Palette_Colors_Swap(SDL_Surface *surface, i8 Oldw, i8 Oldb, i8 NEWw, i8 NEWb) {
    SDL_assert(Oldw < PALETTE_NES_COLOR_NUM);
    SDL_assert(Oldb < PALETTE_NES_COLOR_NUM);
    SDL_assert(NEWw < PALETTE_NES_COLOR_NUM);
    SDL_assert(NEWb < PALETTE_NES_COLOR_NUM);
    SDL_assert(palette_NES->ncolors == PALETTE_NES_COLOR_NUM);

    /* Save old colors from palette_NES */
    /* before changing anything */
    SDL_Color old_white, old_black, new_white, new_black;
    if (NEWw > -1) {
        old_white = palette_NES->colors[Oldw];
        new_white = palette_NES->colors[NEWw];
    }
    if (NEWb > -1) {
        old_black = palette_NES->colors[Oldb];
        new_black = palette_NES->colors[NEWb];
    }

    /* Swap colors in palette_NES */
    if (NEWw > -1) {
        palette_NES->colors[Oldw].r = new_white.r;
        palette_NES->colors[Oldw].g = new_white.g;
        palette_NES->colors[Oldw].b = new_white.b;
    }
    if (NEWb > -1) {
        palette_NES->colors[Oldb].r = new_black.r;
        palette_NES->colors[Oldb].g = new_black.g;
        palette_NES->colors[Oldb].b = new_black.b;
    }

    /* Swap palette of font surface, texture */
    SDL_Surface *buffer = Filesystem_Surface_Palette_Swap(surface, palette_NES);
    SDL_FreeSurface(surface);

    /* Revert colors in palette_NES */
    if (NEWw > -1) {
        palette_NES->colors[Oldw].g = old_white.g;
        palette_NES->colors[Oldw].r = old_white.r;
        palette_NES->colors[Oldw].b = old_white.b;
    }
    if (NEWb > -1) {
        palette_NES->colors[Oldb].r = old_black.r;
        palette_NES->colors[Oldb].g = old_black.g;
        palette_NES->colors[Oldb].b = old_black.b;
    }
    return (buffer);
}
