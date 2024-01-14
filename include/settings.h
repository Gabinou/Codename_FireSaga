#ifndef SETTINGS_H
#define SETTINGS_H

#include "types.h"
#include "debug.h"
#include "structs.h"
#include "enums.h"
#include "SDL.h"
#include "SDL_mixer.h"

enum PLAYER_SETTINGS {
    SOTA_SETTINGS_NULL              = -1,
    SOTA_SETTINGS_ANIMATION         =  0,
    SOTA_SETTINGS_TEXT_SPEED        =  1,
    SOTA_SETTINGS_AUTOCURSOR        =  2,
    SOTA_SETTINGS_AUTOEND_TURNS     =  3,
    SOTA_SETTINGS_MUSIC             =  4,
    SOTA_SETTINGS_SOUNDFX           =  5,
    SOTA_SETTINGS_NUM,
};

struct Music_settings {
    i32 frequency;          /* [Hz] */
    i32 sample_size;        /* [byte] */
    u16 format;             /* AUDIO_* */
    i32 channels;
};

struct Map_settings {
    u8          overlay_mode; /* tile paletteswap or opaque overlay */
    u8          stack_mode;
    u8          grid_thickness; /* Number of lines to draw (mirrored) */
    u8          perim_thickness; /* Number of lines to draw (mirrored) */
    SDL_Color   color_grid;
    bool        grid_show;
};
// grid_thickness 1 ->  |   (center line only)
// grid_thickness 2 -> |||  (center line, +/- 1 lines)

struct Enemy_Turn_settings {
    u64 pause_post_reinforcement;
};

struct Settings {
    struct Point res; /* resolution */
    struct Point pos;
    struct Fps   FPS;

    struct Cursor cursor; /* 32 bits */
    u16 tilesize[TWO_D];
    u8 fontsize;

    struct Map_settings map_settings;
    struct Music_settings music_settings;
    struct Enemy_Turn_settings enemy_turn_settings;

    struct Mouse mouse; /* 16 bits */
    char title[DEFAULT_BUFFER_SIZE];

    bool fullscreen         : 1;
    bool tophand_stronghand : 1;
};
extern struct Settings Settings_default;



#endif /* SETTINGS_H */
