
#include "nourstest.h"
#include "arrow.h"
#include "macros.h"
#include "platform.h"
#include "map/map.h"

#define ROW_LEN 21
#define COL_LEN 25

void test_arrow_draw_start_end(struct Camera *camera,  struct Arrow *arrow,
                               SDL_Renderer *renderer, struct Point start,
                               struct Point end) {

    struct Map_Size size = {
        .grid = {COL_LEN, ROW_LEN},
        .tile = {SOTA_TILESIZE, SOTA_TILESIZE},
    };

    /* -- Fill canvas with white -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_assert(SDL_RenderFillRect(renderer, NULL) == 0);

    /* -- Dstrect common -- */
    SDL_Rect dstrect    = {0};
    dstrect.w           = SOTA_ZOOM(size.tile.x, camera->zoom);
    dstrect.h           = SOTA_ZOOM(size.tile.y, camera->zoom);

    /* -- Drawing light grey start square -- */
    i32 x_zoom          = SOTA_ZOOM(start.x * size.tile.x, camera->zoom);
    i32 y_zoom          = SOTA_ZOOM(start.y * size.tile.y, camera->zoom);
    dstrect.x           = x_zoom + camera->offset.x;
    dstrect.y           = y_zoom + camera->offset.y;
    SDL_SetRenderDrawColor(renderer, 0x44, 0x44, 0x44, SDL_ALPHA_OPAQUE);
    SDL_assert(SDL_RenderFillRect(renderer, &dstrect) == 0);

    /* -- Drawing gray end square -- */
    x_zoom        = SOTA_ZOOM(end.x * size.tile.x, camera->zoom);
    y_zoom        = SOTA_ZOOM(end.y * size.tile.y, camera->zoom);
    dstrect.x     = x_zoom + camera->offset.x;
    dstrect.y     = y_zoom + camera->offset.y;
    SDL_SetRenderDrawColor(renderer, 0x88, 0x88, 0x88, SDL_ALPHA_OPAQUE);
    SDL_assert(SDL_RenderFillRect(renderer, &dstrect) == 0);
}

void test_arrow() {
    /* -- Preliminaries -- */
    sota_mkdir("arrow");

    struct Map_Size size = {
        .grid = {COL_LEN, ROW_LEN},
        .tile = {SOTA_TILESIZE, SOTA_TILESIZE},
    };

    /* -- costmap -- */
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
    struct Point start  = {0};
    struct Point end    = {0};
    int move = 5;

    /* -- Create Camera -- */
    struct Camera camera;
    camera.offset.x = DEFAULT_CAMERA_XOFFSET;
    camera.offset.y = DEFAULT_CAMERA_YOFFSET;
    camera.zoom     = DEFAULT_CAMERA_ZOOM;

    /* -- Create arrow -- */
    i32 tilesize[TWO_D] = {SOTA_TILESIZE, SOTA_TILESIZE};
    struct Arrow *arrow = Arrow_Init(tilesize);

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

    /* -- Loading arrow textures -- */
    Arrow_Textures_Load(arrow, ARROW_FILENAME, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_Textures.png"), renderer,
    //                         arrow->texture, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Start arrow -- */
    start.x = 4;
    start.y = 4;
    Arrow_Path_Init(arrow, costmap, move, start);
    /* -- Add 1 path down -- */
    Arrow_Path_Add(arrow, size, 4, 5);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 2);
    end.x = 4;
    end.y = 5;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_00.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path down -- */
    Arrow_Path_Add(arrow, size, 4, 6);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 3);
    end.x = 4;
    end.y = 6;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);

    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_01.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path down -- */
    Arrow_Path_Add(arrow, size, 4, 7);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 4);
    end.x = 4;
    end.y = 7;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_02.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path down -- */
    Arrow_Path_Add(arrow, size, 4, 8);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 4;
    end.y = 8;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_03.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path right -- */
    Arrow_Path_Add(arrow, size, 5, 8);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 5;
    end.y = 8;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_04.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 7);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 5;
    end.y = 7;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_05.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 6);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 5;
    end.y = 6;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_06.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 5);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 3);
    end.x = 5;
    end.y = 5;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_07.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 4);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 4);
    end.x = 5;
    end.y = 4;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_08.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 3);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 5;
    end.y = 3;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_09.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 2);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 5;
    end.y = 2;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_10.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 1);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 5;
    end.y = 1;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_11.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path up -- */
    Arrow_Path_Add(arrow, size, 5, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 5;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_12.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 4, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 4;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_13.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 3, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 3;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_14.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 2, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 7);
    end.x = 2;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_15.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 1, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 8);
    end.x = 1;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_16.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 0, 0);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 9);
    end.x = 0;
    end.y = 0;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_17.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 0, 1);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 8);
    end.x = 0;
    end.y = 1;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_18.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 0, 2);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 7);
    end.x = 0;
    end.y = 2;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_19.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 0, 3);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 6);
    end.x = 0;
    end.y = 3;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_20.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    // /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 0, 4);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 5);
    end.x = 0;
    end.y = 4;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_21.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 1, 4);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 4);
    end.x = 1;
    end.y = 4;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_22.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 2, 4);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 3);
    end.x = 2;
    end.y = 4;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_23.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 3, 4);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 2);
    end.x = 3;
    end.y = 4;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_24.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 3, 3);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 3);
    end.x = 3;
    end.y = 3;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_25.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* -- Add 1 path left -- */
    Arrow_Path_Add(arrow, size, 4, 3);
    SDL_assert(DARR_NUM(arrow->path) / TWO_D == 4);
    end.x = 4;
    end.y = 3;
    test_arrow_draw_start_end(&camera, arrow, renderer, start, end);
    Arrow_Draw(arrow, size, &camera, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("arrow", "Arrow_26.png"), renderer,
                            render_target, SDL_PIXELFORMAT_ARGB8888,  render_target);

    /* --- SDL_free --- */
    Arrow_Free(arrow);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(render_target);
    SDL_DestroyRenderer(renderer);
}

#undef ROW_LEN
#undef COL_LEN
