#include "nourstest.h"
#include "arrow.h"

#define ROW_LEN 25
#define COL_LEN 21

void test_arrow() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("arrow");

    /* -- Costmap -- */
    i32 costmap[ROW_LEN * COL_LEN] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 4, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 9, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 4, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    /* -- Position, move -- */
    struct Point tilemap_pos = {0};
    int move = 5;

    /* -- Create Camera -- */
    struct Camera camera;
    camera.offset.x = DEFAULT_CAMERA_XOFFSET;
    camera.offset.y = DEFAULT_CAMERA_YOFFSET;
    camera.zoom     = DEFAULT_CAMERA_ZOOM;

    /* -- Create arrow -- */
    i32 tilesize[TWO_D] = {SOTA_TILESIZE, SOTA_TILESIZE};
    struct Arrow *arrow = Arrow_Init(tilesize);
    arrow->col_len = COL_LEN;
    arrow->row_len = ROW_LEN;

    /* -- Create renderer -- */
    SDL_Surface  *surface       = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer      = SDL_CreateSoftwareRenderer(surface);
    SDL_Texture  *render_target = SDL_CreateTexture(renderer,
                                                    SDL_PIXELFORMAT_ARGB8888,
                                                    SDL_TEXTUREACCESS_TARGET,
                                                    1024, 1024);
    SDL_SetTextureBlendMode(render_target, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, render_target);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_assert(SDL_RenderFillRect(renderer, NULL) == 0);

    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_init0.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);



    /* -- Loading arrow textures -- */
    Arrow_Textures_Load(arrow, ARROW_FILENAME, renderer);

    tilemap_pos.x = 4;
    tilemap_pos.y = 4;
    Arrow_Path_Init(arrow, costmap, move, tilemap_pos);
    Arrow_Path_Add(arrow, 4, 5);

    /* -- Drawing -- */
    Arrow_Draw(arrow, renderer, &camera);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_init1.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);
    getchar();

    /* --- FREE --- */
    Arrow_Free(arrow);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(render_target);
}

#undef ROW_LEN
#undef COL_LEN
