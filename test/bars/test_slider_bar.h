#include "nourstest.h"
#include "bars/slider_bar.h"

void test_bars_slider_bar() {
    sota_mkdir("bars_slider_bar");

    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(10, 256);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);
    SDL_Texture *render_target;
    render_target = SDL_CreateTexture(renderer,
                                      SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_TARGET, 132, 10);

    SDL_assert(render_target != NULL);
    SDL_SetTextureBlendMode(render_target, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, render_target);
    SliderBar sliderbar = SliderBar_default;
    sliderbar.len   = 128;
    sliderbar.fill  = 64;
    sliderbar.pos.x = 1;
    sliderbar.pos.y = 5;

    SliderBar_Draw(&sliderbar, renderer);
    char *path;

    path = PATH_JOIN("bars_slider_bar", "Sliderbar.png");
    Filesystem_Texture_Dump(path, renderer, render_target, SDL_PIXELFORMAT_ARGB8888, NULL);

    /* FREE */
    SDL_DestroyTexture(render_target);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
}
