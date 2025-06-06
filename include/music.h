#ifndef MUSIC_H
#define MUSIC_H

#include "enums.h"
#include "nstr.h"
#include "SDL_mixer.h"

/* --- MUSIC --- */

enum MUSIC_CAMP {
    CAMP_MUSIC_NUM      = 4, /* Same as number of story arcs maybe? */
    COMBAT_MUSIC_NUM    = 4, /* Same as number of story arcs maybe? */
};

enum SOTA_MUSIC {
    SOTA_MUSIC_FADEIN_ms    = 250, /* [ms] */
    SOTA_MUSIC_FADEOUT_ms   = 150, /* [ms] */
    SOTA_MUSIC_NULL         =  -1,
    SOTA_MUSIC_DEBUG        =   0,
    SOTA_MUSIC_NUM,
};

extern const struct s8 music_path[SOTA_MUSIC_NUM];

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
enum SOTA_SOUNDFX_CHANNEL {
    SOTA_SOUNDFX_CURSOR_CHANNEL = 0,
};

enum SOTA_SOUNDFX_DURATION {
    SOTA_SOUNDFX_CURSOR_DURATION_ms     =  130, /* [ms] */
    SOTA_SOUNDFX_NEXT_TURN_DURATION_ms  = 1350  /* [ms] */
};

extern const struct s8 soundfx_path[SOTA_SOUNDFX_NUM];

Mix_Chunk *Soundfx_Load(int i);
Mix_Chunk *Soundfx_Load_Cursor(void);
Mix_Chunk *Soundfx_Load_Next_Turn(void);

#endif /* MUSIC_H */
