#include "nourstest.h"
#include "bars/slider_bar.h"

void test_bars_slider_bar() {
    sota_mkdir("bars_slider_bar");

    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(10, 256);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_Texture *render_target = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_SetRenderTarget(renderer, render_target);
    SliderBar sliderbar = SliderBar_default;
    sliderbar.len   = 128;
    sliderbar.fill  = 0;
    sliderbar.pos.x = 1;
    sliderbar.pos.y = 5;

    SliderBar_Draw(&sliderbar, renderer);
    char *path = PATH_JOIN("bars_slider_bar", "Sliderbar.png");
    Filesystem_Texture_Dump(path, renderer, render_target, SDL_PIXELFORMAT_ARGB8888, render_target);
    // Filesystem_Surface_Dump(path, surface);

    /* FREE */
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
}
