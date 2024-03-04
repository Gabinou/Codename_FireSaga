
#include "music.h"

/* --- MUSIC --- */
#define MUSIC_PREXIF "../", "assets", "music"
#define MUSIC_DEBUG_FILE "FE6_FortheCommanders_NESRemix.wav"
#define MUSIC_DEBUG_PATH PATH_JOIN(MUSIC_PREXIF, MUSIC_DEBUG_FILE)
struct s8 music_path[SOTA_MUSIC_NUM] = {
    /* SOTA_MUSIC_DEBUG */ s8_literal(MUSIC_DEBUG_PATH),
};

#undef MUSIC_PREXIF
#undef MUSIC_DEBUG_FILE
#undef MUSIC_DEBUG_PATH

Mix_Music *Music_Load(int i) {
    if ((i <= SOTA_MUSIC_NULL) || (i >= SOTA_MUSIC_NUM)) {
        SDL_LogDebug(SOTA_LOG_AUDIO, "Music index %i is invalid", i);
        return (NULL);
    }
    Mix_Music *music = Mix_LoadMUS(music_path[i].data);
    SDL_assert(music);
    return (music);
}

Mix_Music *Music_Load_Debug(void) {
    return (Music_Load(SOTA_MUSIC_DEBUG));
}

/* --- SOUNDFX / SOUND EFFECTS --- */
#define SOUNDF_PREXIF "../", "assets", "soundfx", "GBA_FE8_Other"
#define SOUNDFX_CURSOR_PATH PATH_JOIN(SOUNDF_PREXIF, "Select 5.wav")
#define SOUNDFX_NEXT_TURN_PATH PATH_JOIN(SOUNDF_PREXIF, "Next Turn.wav")

struct s8 soundfx_path[SOTA_SOUNDFX_NUM] = {
    /* SOTA_SOUNDFX_CURSOR      */ s8_literal(SOUNDFX_CURSOR_PATH),
    /* SOTA_SOUNDFX_NEXT_TURN   */ s8_literal(SOUNDFX_NEXT_TURN_PATH),
};

#undef SOUNDF_PREXIF
#undef SOUNDFX_CURSOR_PATH
#undef SOUNDFX_NEXT_TURN_PATH

Mix_Chunk *Soundfx_Load(int i) {
    if ((i <= SOTA_SOUNDFX_NULL) || (i >= SOTA_SOUNDFX_NUM)) {
        SDL_LogDebug(SOTA_LOG_AUDIO, "Soundfx index %i is invalid", i);
        return (NULL);
    }
    Mix_Chunk *soundfx = Mix_LoadWAV(soundfx_path[i].data);
    SDL_assert(soundfx);
    return (soundfx);
}

Mix_Chunk *Soundfx_Load_Cursor(void) {
    return (Soundfx_Load(SOTA_SOUNDFX_CURSOR));
}

Mix_Chunk *Soundfx_Load_Next_Turn(void) {
    return (Soundfx_Load(SOTA_SOUNDFX_NEXT_TURN));
}
