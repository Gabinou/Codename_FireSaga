#include "nourstest.h"
#include "filesystem.h"

void test_leaks() {
    SDL_Log("test_leaks");
   
    /* Filesystem_indexedSurface_Init leak */
    SDL_Surface *surf = Filesystem_indexedSurface_Init(1028, 1028);
    SDL_FreeSurface(surf);
    surf = NULL;
    
    /* Filesystem_Surface_Load leak */
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface * surf2 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_FreeSurface(surf2);
    surf2 = NULL;
    
    /* Filesystem_Surface_Palette_Swap leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface * surf3 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_Surface *surf4 = Filesystem_Surface_Palette_Swap(surf3, palette_NES_filter_red);
    SDL_FreeSurface(surf3);
    SDL_FreeSurface(surf4);
    surf3 = NULL;
    surf4 = NULL;
    
    /* Filesystem_Surface_Palette_Swap leak */
    extern SDL_Surface *Filesystem_Surface_Pixels2Indices(SDL_Surface *r, SDL_Surface *i);


}

void test_filesystem() {
    SDL_Log("test_filesystem");
    test_leaks();
}