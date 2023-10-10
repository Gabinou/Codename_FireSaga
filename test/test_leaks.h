#include "nourstest.h"
#include "filesystem.h"

void test_filesystem() {
    SDL_Log("test_filesystem");
   
    /* Filesystem_indexedSurface_Init leak */
    SDL_Surface *surf = Filesystem_indexedSurface_Init(1028, 1028);
    SDL_FreeSurface(surf);
    surf = NULL;
    
    /* Filesystem_Surface_Load leak */
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface * surf2 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_FreeSurface(surf2);
    surf2 = NULL;
    
}

void test_leaks() {
    SDL_Log("test_leaks");
    test_filesystem();
}