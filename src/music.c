
#include "log.h"
#include "music.h"
#include "macros.h"
#include "platform.h"
#include "filesystem.h"

#include "physfs.h"
#include "physfsrwops.h" /* SDL1, SDL2 */

/* --- MUSIC --- */
#define MUSIC_PREXIF "assets", "music"
#define MUSIC_DEBUG_FILE "FE6_FortheCommanders_NESRemix.wav"
#define MUSIC_DEBUG_PATH PATH_JOIN(MUSIC_PREXIF, MUSIC_DEBUG_FILE)
const s8 music_path[SOTA_MUSIC_NUM] = {
    /* SOTA_MUSIC_DEBUG */ s8_literal(MUSIC_DEBUG_PATH),
};

#undef MUSIC_PREXIF
#undef MUSIC_DEBUG_FILE
#undef MUSIC_DEBUG_PATH

Mix_Music *Music_Load(int i) {
    IES_check_ret((i > SOTA_MUSIC_NULL) && (i < SOTA_MUSIC_NUM), NULL);

    SDL_RWops *rwops = PHYSFSRWOPS_openRead(music_path[i].data);
    IES_check_ret(rwops, NULL);

    Mix_Music *music = Mix_LoadMUS_RW(rwops, 1);
    IES_check_ret(music, NULL);
    return (music);
}

Mix_Music *Music_Load_Debug(void) {
    return (Music_Load(SOTA_MUSIC_DEBUG));
}

/* --- SOUNDFX / SOUND EFFECTS --- */
#define SOUNDF_PREXIF "assets", "soundfx", "GBA_FE8_Other"
#define SOUNDFX_CURSOR_PATH PATH_JOIN(SOUNDF_PREXIF, "Select 5.wav")
#define SOUNDFX_NEXT_TURN_PATH PATH_JOIN(SOUNDF_PREXIF, "Next Turn.wav")

const s8 soundfx_path[SOTA_SOUNDFX_NUM] = {
    /* SOTA_SOUNDFX_CURSOR      */ s8_literal(SOUNDFX_CURSOR_PATH),
    /* SOTA_SOUNDFX_NEXT_TURN   */ s8_literal(SOUNDFX_NEXT_TURN_PATH),
};

#undef SOUNDF_PREXIF
#undef SOUNDFX_CURSOR_PATH
#undef SOUNDFX_NEXT_TURN_PATH

Mix_Chunk *Soundfx_Load(int i) {
    IES_check_ret((i > SOTA_SOUNDFX_NULL) && (i < SOTA_SOUNDFX_NUM), NULL);

    SDL_RWops *rwops = PHYSFSRWOPS_openRead(soundfx_path[i].data);
    IES_check_ret(rwops, NULL);

    Mix_Chunk *soundfx = Mix_LoadWAV_RW(rwops, 1);
    IES_check_ret(soundfx, NULL);
    return (soundfx);
}

Mix_Chunk *Soundfx_Load_Cursor(void) {
    return (Soundfx_Load(SOTA_SOUNDFX_CURSOR));
}

Mix_Chunk *Soundfx_Load_Next_Turn(void) {
    return (Soundfx_Load(SOTA_SOUNDFX_NEXT_TURN));
}
