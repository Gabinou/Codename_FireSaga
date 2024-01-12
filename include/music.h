#ifndef MUSIC_H
#define MUSIC_H

#include "enums.h"
#include "filesystem.h"
#include "SDL_mixer.h"

enum MUSIC_CAMP {
    CAMP_MUSIC_NUM      = 4, /* Same as number of story arcs maybe? */
    COMBAT_MUSIC_NUM    = 4, /* Same as number of story arcs maybe? */
};

/* --- Pre-loaded music --- */
/* Maybe make arrays of filenames? */
/* Maybe just ONE array? */
extern Mix_Music *title_screen;
extern Mix_Music *chapter_music[CHAPTER_NUM];
extern Mix_Music *camp[CAMP_MUSIC_NUM];
extern Mix_Music *combat[COMBAT_MUSIC_NUM];

/* --- Dynamically loaded music: Scenes --- */
enum SOTA_SOUNDFX {
    SOTA_SOUNDFX_NULL       = -1,
    SOTA_SOUNDFX_CURSOR     =  0,
    SOTA_SOUNDFX_NEXT_TURN  =  1,
    SOTA_SOUNDFX_NUM        =  2,
};

extern s8 soundfx_path[SOTA_SOUNDFX_NUM];

#define SOTA_CURSOR_SOUNDFX PATH_JOIN("assets", "soundfx", "GBA_FE8_Other", "Select 5.wav")
#define SOTA_CURSOR_SOUNDFX_DURATION 130 /* [ms] */
#define SOTA_TURN_SOUNDFX PATH_JOIN("assets", "soundfx", "GBA_FE8_Other", "Next Turn.wav")
#define SOTA_TURN_SOUNDFX_DURATION 1350 /* [ms] */

Mix_Music *Music_Load_Debug(void);

#endif /* MUSIC_H */
