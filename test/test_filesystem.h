
#include "nourstest.h"
#include "filesystem.h"

void test_leaks() {
    sota_mkdir("filesystem");

    /* Filesystem_indexedSurface_Init leak */
    SDL_Surface *surf = Filesystem_indexedSurface_Init(1028, 1028);
    char *path = PATH_JOIN("filesystem", "empty1.png");
    SDL_SaveBMP(surf, path);
    /* -- SDL_free -- */
    SDL_FreeSurface(surf);
    surf = NULL;

    /* Filesystem_Surface_Load leak */
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_test.png");
    SDL_Surface *surf2 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    int success = SDL_SetSurfacePalette(surf2, palette_SOTA);
    SDL_assert(success == 0);

    path = PATH_JOIN("filesystem", "pixelnours_test1.png");
    SDL_SaveBMP(surf2, path);
    /* -- SDL_free -- */
    SDL_FreeSurface(surf2);
    surf2 = NULL;

    /* Filesystem_Surface_Palette_Swap leak */
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_test.png");
    SDL_Surface *surf3 = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    success = SDL_SetSurfacePalette(surf3, palette_SOTA);
    SDL_assert(success == 0);
    path = PATH_JOIN("filesystem", "pixelnours_test2.png");
    SDL_SaveBMP(surf3, path);

    SDL_Surface *surf4 = Filesystem_Surface_Palette_Swap(surf3, palette_SOTA_filter_red);
    path = PATH_JOIN("filesystem", "pixelnours_test_red.png");
    SDL_SaveBMP(surf4, path);
    /* -- SDL_free -- */
    SDL_FreeSurface(surf3);
    SDL_FreeSurface(surf4);
    surf3 = NULL;
    surf4 = NULL;

    /* --- Filesystem_Surface_Pixels2Indices leak --- */
    /* -- Load abgr image -- */
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_test.png");
    SDL_Surface *surf_raw  = IMG_Load(path);
    path = PATH_JOIN("filesystem", "pixelnours_raw.png");
    SDL_SaveBMP(surf_raw, path);

    SDL_Surface *surf_abgr = SDL_ConvertSurfaceFormat(surf_raw, SDL_PIXELFORMAT_ABGR8888, SDL_IGNORE);
    SDL_FreeSurface(surf_raw);

    path = PATH_JOIN("filesystem", "pixelnours_abgr.png");
    SDL_SaveBMP(surf_abgr, path);

    /* -- Convert to index8 -- */
    SDL_Surface *surf_index = Filesystem_indexedSurface_Init(surf_abgr->w, surf_abgr->h);
    success = SDL_SetSurfacePalette(surf_index, palette_SOTA);
    SDL_SetColorKey(surf_index, SDL_TRUE, PALETTE_COLORKEY);
    SDL_assert(surf_index->format->palette->ncolors == PALETTE_SOTA_COLOR_NUM);
    SDL_assert(success == 0);

    Filesystem_Surface_Pixels2Indices(surf_abgr, surf_index);

    path = PATH_JOIN("filesystem", "pixelnours_index.png");
    SDL_SaveBMP(surf_index, path);

    /* -- SDL_free -- */
    SDL_FreeSurface(surf_abgr);
    SDL_FreeSurface(surf_index);
}

void test_filesystem() {
    test_leaks();
}