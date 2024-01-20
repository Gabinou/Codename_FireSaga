
#include "bars/slider_bar.h"

SliderBar SliderBar_default = {
    .fill       = 0,
    .len        = SLIDER_BAR_MIN_LEN,
    .increment  = SLIDER_BAR_INCREMENTS,
    .pos        = {0},
};

/* --- Moving Cursor --- */
void SliderBar_Increment(SliderBar *sb) {
    sb->fill += sb->len / sb->increment;
    sb->fill = sb->fill > sb->len ? sb->len : sb->fill;
}

void SliderBar_Decrement(SliderBar *sb) {
    sb->fill -= sb->len / sb->increment;
    sb->fill = sb->fill < 0 ? 0 : sb->fill;
}

/* --- Drawing --- */
void _SliderBar_Draw_Bar(SliderBar *sb, SDL_Renderer *renderer) {

    SDL_Rect rect = {0};

    int eff_fill = sb->fill > sb->len ? sb->len : sb->fill;
    /* -- Draw shadow -- */
    SDL_Color blk = palette_NES->colors[SLIDER_BAR_BLACK];
    SDL_SetRenderDrawColor(renderer, blk.r, blk.g, blk.b, SDL_ALPHA_OPAQUE);
    rect.x = sb->pos.x;
    rect.y = sb->pos.y;
    rect.w = sb->len + 1;
    rect.h = 2;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw bar first half -- */
    SDL_Color wht = palette_NES->colors[SLIDER_BAR_WHITE];
    SDL_SetRenderDrawColor(renderer, wht.r, wht.g, wht.b, SDL_ALPHA_OPAQUE);
    rect.x = sb->pos.x;
    rect.y = sb->pos.y;
    rect.w = eff_fill - 1;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw bar second half -- */
    SDL_Color dark = palette_NES->colors[SLIDER_BAR_DARK];
    SDL_SetRenderDrawColor(renderer, dark.r, dark.g, dark.b, SDL_ALPHA_OPAQUE);
    rect.x = sb->pos.x + eff_fill;
    rect.y = sb->pos.y;
    rect.w = sb->len - eff_fill - 1;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);
}

void _SliderBar_Draw_Slider(SliderBar *sb, SDL_Renderer *renderer) {

    SDL_Rect rect = {0};

    int eff_fill = sb->fill > sb->len ? sb->len : sb->fill;

    /* 1 pixel lost from left, 2 from right side */
    eff_fill = (eff_fill - 3) > 1 ? eff_fill - 3 : 1;

    /* -- Draw slider shadow -- */
    SDL_Color blk = palette_NES->colors[SLIDER_BAR_BLACK];
    SDL_SetRenderDrawColor(renderer, blk.r, blk.g, blk.b, SDL_ALPHA_OPAQUE);

    rect.x = sb->pos.x + eff_fill + 2;
    rect.y = sb->pos.y - 2;
    rect.w = 1;
    rect.h = 6;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sb->pos.x + eff_fill + 1;
    rect.y = sb->pos.y - 3;
    rect.w = 1;
    rect.h = 8;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sb->pos.x + eff_fill;
    rect.y = sb->pos.y + 4;
    rect.w = 1;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sb->pos.x + eff_fill - 1;
    rect.y = sb->pos.y + 3;
    rect.w = 1;
    rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw slider light -- */
    SDL_Color lgt = palette_NES->colors[SLIDER_BAR_LIGHT];
    SDL_SetRenderDrawColor(renderer, lgt.r, lgt.g, lgt.b, SDL_ALPHA_OPAQUE);

    rect.x = sb->pos.x + eff_fill;
    rect.y = sb->pos.y - 3;
    rect.w = 1;
    rect.h = 7;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = sb->pos.x + eff_fill - 1;
    rect.y = sb->pos.y - 2;
    rect.w = 3;
    rect.h = 5;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw slider white -- */
    SDL_Color wht = palette_NES->colors[SLIDER_BAR_WHITE];
    SDL_SetRenderDrawColor(renderer, wht.r, wht.g, wht.b, SDL_ALPHA_OPAQUE);

    rect.x = sb->pos.x + eff_fill;
    rect.y = sb->pos.y - 1;
    rect.w = 1;
    rect.h = 3;
    SDL_RenderDrawRect(renderer, &rect);

}

void SliderBar_Draw(SliderBar *sb, SDL_Renderer *renderer) {
    /*     Slider           */
    /*  |----|-------| bar  */
    /*  |----|-------|      */
    SDL_assert(sb->len  > SLIDER_BAR_MIN_LEN);
    SDL_assert(sb->fill >= 0);
    SDL_assert(sb->fill <= sb->len);

    _SliderBar_Draw_Bar(sb, renderer);
    _SliderBar_Draw_Slider(sb, renderer);

    Utilities_DrawColor_Reset(renderer);
}
