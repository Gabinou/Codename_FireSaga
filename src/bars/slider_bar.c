
#include "bars/slider_bar.h"

SliderBar SliderBar_default = {
    .fill   = 0,
    .len    = SLIDER_BAR_MIN_LEN,
    .pos    = {0},
};

/* --- Drawing --- */

void _SliderBar_Draw_Bar(SliderBar *sliderbar, SDL_Renderer *renderer) {

    SDL_Rect rect = {0};

    /* -- Draw shadow -- */
    SDL_Color blk = palette_NES->colors[SLIDER_BAR_BLACK];
    SDL_SetRenderDrawColor(renderer, blk.r, blk.g, blk.b, SDL_ALPHA_OPAQUE);
    rect.x = sliderbar->pos.x;
    rect.y = sliderbar->pos.y;
    rect.w = sliderbar->len + 1;
    rect.h = 2;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw bar first half -- */
    SDL_Color wht = palette_NES->colors[SLIDER_BAR_WHITE];
    SDL_SetRenderDrawColor(renderer, wht.r, wht.g, wht.b, SDL_ALPHA_OPAQUE);
    rect.x = sliderbar->pos.x;
    rect.y = sliderbar->pos.y;
    rect.w = sliderbar->fill;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw bar second half -- */
    SDL_Color dark = palette_NES->colors[SLIDER_BAR_DARK];
    SDL_SetRenderDrawColor(renderer, dark.r, dark.g, dark.b, SDL_ALPHA_OPAQUE);
    rect.x = sliderbar->pos.x + sliderbar->fill;
    rect.y = sliderbar->pos.y;
    rect.w = sliderbar->len - sliderbar->fill;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);
}

void _SliderBar_Draw_Slider(SliderBar *sliderbar, SDL_Renderer *renderer) {

    SDL_Rect rect = {0};

    /* -- Draw slider shadow -- */
    SDL_Color blk = palette_NES->colors[SLIDER_BAR_BLACK];
    SDL_SetRenderDrawColor(renderer, blk.r, blk.g, blk.b, SDL_ALPHA_OPAQUE);

    rect.x = sliderbar->pos.x + sliderbar->fill + 2;
    rect.y = sliderbar->pos.y + 2;
    rect.w = 1;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sliderbar->pos.x + sliderbar->fill - 1;
    rect.y = sliderbar->pos.y + 3;
    rect.w = 4;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sliderbar->pos.x + sliderbar->fill;
    rect.y = sliderbar->pos.y + 4;
    rect.w = 2;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw slider light -- */
    SDL_Color lgt = palette_NES->colors[SLIDER_BAR_LIGHT];
    SDL_SetRenderDrawColor(renderer, lgt.r, lgt.g, lgt.b, SDL_ALPHA_OPAQUE);

    rect.x = sliderbar->pos.x + sliderbar->fill;
    rect.y = sliderbar->pos.y - 3;
    rect.w = 1;
    rect.h = 7;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sliderbar->pos.x + sliderbar->fill - 1;
    rect.y = sliderbar->pos.y - 2;
    rect.w = 3;
    rect.h = 5;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw slider white -- */
    SDL_Color wht = palette_NES->colors[SLIDER_BAR_WHITE];
    SDL_SetRenderDrawColor(renderer, wht.r, wht.g, wht.b, SDL_ALPHA_OPAQUE);

    rect.x = sliderbar->pos.x + sliderbar->fill;
    rect.y = sliderbar->pos.y - 1;
    rect.w = 1;
    rect.h = 3;
    SDL_RenderDrawRect(renderer, &rect);

}

void SliderBar_Draw(SliderBar *sliderbar, SDL_Renderer *renderer) {
    /*     Slider           */
    /*  |----|-------| bar  */
    /*  |----|-------|      */
    SDL_assert(sliderbar->len  > SLIDER_BAR_MIN_LEN);
    SDL_assert(sliderbar->fill >= 0);
    SDL_assert(sliderbar->fill <= sliderbar->len);

    _SliderBar_Draw_Bar(sliderbar, renderer);
    _SliderBar_Draw_Slider(sliderbar, renderer);

    Utilities_DrawColor_Reset(renderer);
}
