#include "nourstest.h"
#include "arrow.h"

#define ROW_LEN = 25;
#define COL_LEN = 21;

void test_arrow() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);

    /* -- Create Camera -- */
    struct Camera camera;
    camera.offset.x = DEFAULT_CAMERA_XOFFSET;
    camera.offset.y = DEFAULT_CAMERA_YOFFSET;
    camera.zoom     = DEFAULT_CAMERA_ZOOM;

    /* -- Create arrow -- */
    i32 tilesize[TWO_D] = {SOTA_TILESIZE, SOTA_TILESIZE};
    struct Arrow *arrow = Arrow_Init(arrow, tilesize);
    arrow->col_len = COL_LEN; 
    arrow->row_len = ROW_LEN; 

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    Arrow_Draw(arrow, renderer, &camera);

    /* --- FREE --- */
    struct Arrow *arrow = Arrow_Free(arrow);
}