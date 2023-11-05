
#include "palette.h"
#include "stb_sprintf.h"

SDL_Palette *palette_NES = NULL;
SDL_Palette *palette_NES_enemy;
SDL_Palette *palette_NES_filter_green = NULL;
SDL_Palette *palette_NES_filter_darkredwshadow = NULL;
SDL_Palette *palette_NES_filter_darkred = NULL;
SDL_Palette *palette_NES_filter_blue = NULL;
SDL_Palette *palette_NES_filter_red = NULL;
SDL_Palette *palette_NES_night = NULL;
SDL_Palette *palette_NES_shadow = NULL;
SDL_Palette *palette_NES_darken = NULL;
SDL_Palette *palette_NES_lighten = NULL;
SDL_Palette *palette_NES_lightenmore = NULL;
SDL_Palette *palette_NES_purple = NULL;
SDL_Palette **sota_palettes = NULL;

void Palettes_Load() {
    palette_NES =                        SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_enemy =                  SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_green =           SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_darkredwshadow =  SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_darkred =         SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_blue =            SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_filter_red =             SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_night =                  SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_shadow =                 SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_darken =                 SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_lighten =                SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_lightenmore =            SDL_AllocPalette(PALETTE_NES_COLOR_NUM);
    palette_NES_purple =                 SDL_AllocPalette(PALETTE_NES_COLOR_NUM);

    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes", "palette_NES.json"),
                                palette_NES);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          "palette_NES_map_unit_enemy.json"), palette_NES_enemy);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_green)".json"), palette_NES_filter_green);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_darkredwshadow)".json"), palette_NES_filter_darkredwshadow);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_darkred)".json"), palette_NES_filter_darkred);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_blue)".json"), palette_NES_filter_blue);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_red)".json"), palette_NES_filter_red);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_filter_purple)".json"), palette_NES_purple);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_night)".json"), palette_NES_night);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_shadow)".json"), palette_NES_shadow);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_darken)".json"), palette_NES_darken);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_lighten)".json"), palette_NES_lighten);
    Filesystem_readJSON_Palette(PATH_JOIN("assets", "Palettes",
                                          STRINGIZE(palette_NES_lightenmore)".json"), palette_NES_lightenmore);

    Palettes_SDL_free();
    sota_palettes = calloc(PALETTE_NUM, sizeof(*sota_palettes));
    sota_palettes[PALETTE_NULL] =                 NULL;
    sota_palettes[PALETTE_NES] =                  palette_NES;
    sota_palettes[PALETTE_NES_ENEMY] =            palette_NES_enemy;
    sota_palettes[PALETTE_NES_RED] =              palette_NES_filter_red;
    sota_palettes[PALETTE_NES_GREEN] =            palette_NES_filter_green;
    sota_palettes[PALETTE_NES_BLUE] =             palette_NES_filter_blue;
    sota_palettes[PALETTE_NES_DARKRED] =          palette_NES_filter_darkred;
    sota_palettes[PALETTE_NES_DARKREDwSHADOW] =   palette_NES_filter_darkredwshadow;
    sota_palettes[PALETTE_NES_SHADOW] =           palette_NES_shadow;
    sota_palettes[PALETTE_NES_NIGHT] =            palette_NES_night;
    sota_palettes[PALETTE_NES_DARKEN] =           palette_NES_darken;
    sota_palettes[PALETTE_NES_LIGHTEN] =          palette_NES_lighten;
    sota_palettes[PALETTE_NES_LIGHTENMORE] =      palette_NES_lightenmore;
    sota_palettes[PALETTE_NES_PURPLE] =           palette_NES_purple;
    SDL_assert(palette_NES != NULL);

}

void Palettes_SDL_free() {
    if (sota_palettes != NULL) {
        for (size_t i = 0; i < PALETTE_NUM; i++) {
            if (sota_palettes[i] != NULL)
                SDL_FreePalette(sota_palettes[i]);
        }
        SDL_free(sota_palettes);
        sota_palettes = NULL;
    }
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

void Palette_Tables_Load() {
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_shadow)".json",palette_table_NES_shadow);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_night)".json",palette_table_NES_night);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_filter_darkred)".json",palette_table_NES_filter_darkred);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_filter_red)".json",palette_table_NES_filter_red);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_filter_green)".json",palette_table_NES_filter_green);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_filter_blue)".json",palette_table_NES_filter_blue);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_darken)".json",palette_table_NES_darken);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_lighten)".json",palette_table_NES_lighten);
    Filesystem_readJSON_PaletteTable("assets"PHYSFS_SEPARATOR"Palettes"PHYSFS_SEPARATOR STRINGIZE(palette_table_NES_lightenmore)".json",palette_table_NES_lightenmore);
}