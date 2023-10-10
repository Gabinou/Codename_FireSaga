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
    SDL_Surface *surf2 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_FreeSurface(surf2);
    surf2 = NULL;

    /* Filesystem_Surface_Palette_Swap leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface *surf3 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_Surface *surf4 = Filesystem_Surface_Palette_Swap(surf3, palette_NES_filter_red);
    SDL_FreeSurface(surf3);
    SDL_FreeSurface(surf4);
    surf3 = NULL;
    surf4 = NULL;

    /* Filesystem_Surface_Pixels2Indices leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface *surf_abgr  = IMG_Load(path);
    SDL_Surface *surf_index =  Filesystem_indexedSurface_Init(surf_abgr->w, surf_abgr->h);
    Filesystem_Surface_Pixels2Indices(surf_abgr, surf_index);
    SDL_FreeSurface(surf_abgr);
    SDL_FreeSurface(surf_index);

}

void test_filesystem() {
    SDL_Log("test_filesystem");
    test_leaks();
}