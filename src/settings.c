
#include "settings.h"

struct Settings Settings_default = {
    .title = "Codename:Firesaga",

    .res                = {DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y},
    .pos                = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
    .fontsize           = 28,
    .fullscreen         = false,
    .tophand_stronghand = true, // TODO: REMOVE
    .tilesize           = {SOTA_TILESIZE, SOTA_TILESIZE},

    .FPS = {
        .pos            = {.x = FPS_DEFAULT_POSX, .y = FPS_DEFAULT_POSY},
        .textcolor      = {
            FPS_DEFAULT_COLORR, FPS_DEFAULT_COLORG,
            FPS_DEFAULT_COLORB, FPS_DEFAULT_COLORA
        },
        .sizefactor     = {FPS_DEFAULT_SIZEFACTORX, FPS_DEFAULT_SIZEFACTORY},
        .show           = FPS_DEFAULT_SHOW,
        .cap            = FPS_DEFAULT_CAP,
        .ff_cap         = FF_DEFAULT_CAP,
    },
    .map_settings = {
        .overlay_mode       = MAP_OVERLAY_MODE_PALETTESWAP,
        .color_grid         = {0, 0, 0, SDL_ALPHA_OPAQUE},
        .grid_thickness     = 1,
        .perim_thickness    = 3,
        .grid_show          = true,
        .stack_mode         = MAP_SETTING_STACK_DANGERMAP
    },
    .music_settings = {
        .frequency         = 44100,                /* [Hz] */
        .sample_size        =  2048,                /* [byte] */
        .channels  =     2,
        .format             = MIX_DEFAULT_FORMAT,  /* [byte] */

    },
    .enemy_turn_settings = {
        .pause_post_reinforcement = 2ULL * SOTA_ns / 2ULL,
    },

    .mouse              = {MOUSE_DEFAULT_ONHOLD, MOUSE_DEFAULT_MOVE},
    .cursor             = {DEFAULT_CURSOR_FRAMES, DEFAULT_CURSOR_SPEED},
};
