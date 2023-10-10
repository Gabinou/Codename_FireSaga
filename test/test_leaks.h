#include "nourstest.h"
#include "filesystem.h"

void test_filesystem() {
    SDL_Log("test_filesystem");
    SDL_Surface *surf = Filesystem_indexedSurface_Init(1028, 1028);
    SDL_FreeSurface(surf);
    surf = NULL
}

void test_leaks() {
    SDL_Log("test_leaks");
    test_filesystem();
}