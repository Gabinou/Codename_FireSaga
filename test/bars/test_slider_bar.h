
#include "nourstest.h"
#include "bars/slider_bar.h"

void test_bars_slider_bar() {
    sota_mkdir("bars_slider_bar");

    /* -- Preparing -- */
    char *path;
    SliderBar sliderbar = SliderBar_default;
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(10, 256);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_Texture *render_target;
    render_target = SDL_CreateTexture(renderer,
                                      SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_TARGET, 128, 10);
    SDL_assert(render_target != NULL);
    SDL_SetTextureBlendMode(render_target, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, render_target);

    /* -- Test 1 -- */
    /* - Setting - */
    sliderbar.len   = 128;
    sliderbar.fill  = 64;
    sliderbar.pos.x = 0;
    sliderbar.pos.y = 5;

    /* - Drawing - */
    SliderBar_Draw(&sliderbar, renderer);

    /* - Dumping - */
    path = PATH_JOIN("bars_slider_bar", "Sliderbar_64.png");
    Filesystem_Texture_Dump(path, renderer, render_target, SDL_PIXELFORMAT_ARGB8888, render_target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    /* -- Test 2 -- */
    /* - Setting - */
    sliderbar.len   = 128;
    sliderbar.fill  = 0;
    sliderbar.pos.x = 0;
    sliderbar.pos.y = 5;

    /* - Drawing - */
    SliderBar_Draw(&sliderbar, renderer);

    /* - Dumping - */
    path = PATH_JOIN("bars_slider_bar", "Sliderbar_0.png");
    Filesystem_Texture_Dump(path, renderer, render_target, SDL_PIXELFORMAT_ARGB8888, render_target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    /* -- Test 2 -- */
    /* - Setting - */
    sliderbar.len   = 128;
    sliderbar.fill  = 128;
    sliderbar.pos.x = 0;
    sliderbar.pos.y = 5;

    /* - Drawing - */
    SliderBar_Draw(&sliderbar, renderer);

    /* - Dumping - */
    path = PATH_JOIN("bars_slider_bar", "Sliderbar_128.png");
    Filesystem_Texture_Dump(path, renderer, render_target, SDL_PIXELFORMAT_ARGB8888, render_target);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    /* SDL_free */
    SDL_DestroyTexture(render_target);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
}
