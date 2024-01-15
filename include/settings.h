#ifndef SETTINGS_H
#define SETTINGS_H

#include "types.h"
#include "debug.h"
#include "structs.h"
#include "enums.h"
#include "SDL.h"
#include "SDL_mixer.h"

/* --- Enums --- */
#define SOTA_BRIGHTNESS_MAX 1.0f
#define SOTA_BRIGHTNESS_MIN 0.0f

enum PLAYER_SETTINGS {
    SOTA_SETTINGS_NULL              = -1,
    SOTA_SETTINGS_ANIMATION         =  0,
    SOTA_SETTINGS_TEXT_SPEED        =  1,
    SOTA_SETTINGS_AUTOCURSOR        =  2,
    SOTA_SETTINGS_AUTOEND_TURNS     =  3,
    SOTA_SETTINGS_MUSIC             =  4,
    SOTA_SETTINGS_SOUNDFX           =  5,
    SOTA_SETTINGS_VOLUME_MASTER     =  6,
    SOTA_SETTINGS_VOLUME_MUSIC      =  7,
    SOTA_SETTINGS_VOLUME_SOUNDFX    =  8,
    SOTA_SETTINGS_NUM,
};

enum ANIMATION_SETTINGS {
    ANIMATION_SETTINGS_NULL         = -1,
    ANIMATION_SETTINGS_ON           =  0,
    ANIMATION_SETTINGS_FRIENDLY     =  1,
    ANIMATION_SETTINGS_ENEMY        =  2,
    ANIMATION_SETTINGS_OFF          =  3,
    ANIMATION_SETTINGS_NUM,
};

enum TEXT_SPEED_SETTINGS {
    TEXT_SPEED_SETTINGS_NULL        = -1,
    TEXT_SPEED_SETTINGS_SLOW        =  0,
    TEXT_SPEED_SETTINGS_MID         =  1,
    TEXT_SPEED_SETTINGS_FAST        =  2,
    TEXT_SPEED_SETTINGS_MAX         =  3,
    TEXT_SPEED_SETTINGS_NUM,
};

enum AUTOCURSOR_SETTINGS {
    AUTOCURSOR_SETTINGS_NULL        = -1,
    AUTOCURSOR_SETTINGS_OFF         =  0,
    AUTOCURSOR_SETTINGS_ON          =  1,
};

enum AUTOEND_TURNS_SETTINGS {
    AUTOEND_TURNS_SETTINGS_NULL     = -1,
    AUTOEND_TURNS_SETTINGS_OFF      =  0,
    AUTOEND_TURNS_SETTINGS_ON       =  1,
};

enum MUSIC_SETTINGS {
    MUSIC_SETTINGS_NULL             = -1,
    MUSIC_SETTINGS_OFF              =  0,
    MUSIC_SETTINGS_ON               =  1,
};

enum SOUNDFX_SETTINGS {
    SOUNDFX_SETTINGS_NULL           = -1,
    SOUNDFX_SETTINGS_OFF            =  0,
    SOUNDFX_SETTINGS_ON             =  1,
};

/* --- Structs definition --- */
struct Music_settings {
    i32 frequency;          /*  [Hz]    */
    i32 sample_size;        /* [byte]   */
    u16 format;             /* AUDIO_*  */
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
/* grid_thickness 1 ->  |   (center line only)          */
/* grid_thickness 2 -> |||  (center line, +/- 1 lines)  */
/*                  and so on...                        */

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

    int music_volume;
    int soundfx_volume;
    float brightness;

    bool fullscreen         : 1;
    bool tophand_stronghand : 1;
};
extern struct Settings Settings_default;



#endif /* SETTINGS_H */
