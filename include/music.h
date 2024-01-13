#ifndef MUSIC_H
#define MUSIC_H

#include "enums.h"
#include "filesystem.h"
#include "SDL_mixer.h"

/* --- MUSIC --- */

enum MUSIC_CAMP {
    CAMP_MUSIC_NUM      = 4, /* Same as number of story arcs maybe? */
    COMBAT_MUSIC_NUM    = 4, /* Same as number of story arcs maybe? */
};

enum SOTA_MUSIC {
    SOTA_MUSIC_NULL         = -1,
    SOTA_MUSIC_DEBUG        =  0,
    SOTA_MUSIC_NUM,
};

extern struct s8 music_path[SOTA_MUSIC_NUM];

Mix_Music *Music_Load_Debug(void);
Mix_Music *Music_Load(int i);

/* --- Dynamically loaded music: Scenes --- */

/* --- SOUNDFX / SOUND EFFECTS --- */
enum SOTA_SOUNDFX {
    SOTA_SOUNDFX_NULL       = -1,
    SOTA_SOUNDFX_CURSOR     =  0,
    SOTA_SOUNDFX_NEXT_TURN  =  1,
    SOTA_SOUNDFX_NUM
};

enum SOTA_SOUNDFX_DURATION {
    SOTA_SOUNDFX_CURSOR_DURATION    =  130, /* [ms] */
    SOTA_SOUNDFX_NEXT_TURN_DURATION = 1350 /* [ms] */
};

extern struct s8 soundfx_path[SOTA_SOUNDFX_NUM];

#define SOTA_CURSOR_SOUNDFX PATH_JOIN("assets", "soundfx", "GBA_FE8_Other", "Select 5.wav")
#define SOTA_TURN_SOUNDFX PATH_JOIN("assets", "soundfx", "GBA_FE8_Other", "Next Turn.wav")


#endif /* MUSIC_H */
