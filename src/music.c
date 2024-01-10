
#include "music.h"


Mix_Music *chapter_music[CHAPTER_NUM] = {
    NULL,
};

Mix_Music *Music_Load_Debug(void) {
    char *path = PATH_JOIN("../", "assets", "music", "FE6_FortheCommanders_NESRemix.wav");
    Mix_Music *music = Mix_LoadMUS(path);
    SDL_assert(music);
    return (music);
}

