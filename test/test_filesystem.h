#include "nourstest.h"
#include "filesystem.h"

void test_leaks() {
    SDL_Log("test_leaks");
    sota_mkdir("filesystem");

    /* Filesystem_indexedSurface_Init leak */
    SDL_Surface *surf = Filesystem_indexedSurface_Init(1028, 1028);
    char *path = PATH_JOIN("filesystem", "empty1.png");
    SDL_SaveBMP(surf, path);
    SDL_FreeSurface(surf);
    surf = NULL;

    /* Filesystem_Surface_Load leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface *surf2 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    int success = SDL_SetSurfacePalette(surf2, palette_NES);
    SDL_assert(success == 0);

    path = PATH_JOIN("filesystem", "pixelnours_test1.png");
    SDL_SaveBMP(surf2, path);
    SDL_FreeSurface(surf2);
    surf2 = NULL;

    /* Filesystem_Surface_Palette_Swap leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface *surf3 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    success = SDL_SetSurfacePalette(surf3, palette_NES);
    SDL_assert(success == 0);
    path = PATH_JOIN("filesystem", "pixelnours_test2.png");
    SDL_SaveBMP(surf3, path);

    SDL_Surface *surf4 = Filesystem_Surface_Palette_Swap(surf3, palette_NES_filter_red);
    path = PATH_JOIN("filesystem", "pixelnours_test_red.png");
    SDL_SaveBMP(surf4, path);
    SDL_FreeSurface(surf3);
    SDL_FreeSurface(surf4);
    surf3 = NULL;
    surf4 = NULL;

    /* Filesystem_Surface_Pixels2Indices leak */
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_test.png");
    SDL_Surface *surf_abgr  = IMG_Load(path);
    success = SDL_SetSurfacePalette(surf_abgr, palette_NES);
    SDL_assert(success == 0);
    path = PATH_JOIN("filesystem", "pixelnours_abgr.png");
    SDL_SaveBMP(surf_abgr, path);
    SDL_Surface *surf_index =  Filesystem_indexedSurface_Init(surf_abgr->w, surf_abgr->h);
    success = SDL_SetSurfacePalette(surf_index, palette_NES);
    SDL_assert(success == 0);

    Filesystem_Surface_Pixels2Indices(surf_abgr, surf_index);

    path = PATH_JOIN("filesystem", "pixelnours_index.png");
    SDL_SaveBMP(surf_index, path);
    SDL_FreeSurface(surf_abgr);
    SDL_FreeSurface(surf_index);

}

void test_filesystem() {
    SDL_Log("test_filesystem");
    test_leaks();
}