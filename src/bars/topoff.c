
#include "bars/topoff.h"

/* --- TopoffBar --- */
void TopoffBar_Draw(struct SimpleBar *tob, SDL_Renderer *renderer) {
    /*   FLIPPED BAR     NO FLIP BAR    */
    /*  |     -------|  |--------    |  */
    /*  |____________|  |____________|  */

    SDL_Rect rect = {0};

    /* -- Draw shadow -- */
    SDL_Color shadow = palette_NES->colors[TOPOFF_BAR_SHADOW];
    SDL_SetRenderDrawColor(renderer, shadow.r, shadow.g, shadow.b, SDL_ALPHA_OPAQUE);
    rect.x = tob->pos.x + 1, rect.y = tob->pos.y + 3;
    rect.w = tob->len - 1, rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);
    rect.x = tob->pos.x + tob->len - 1, rect.y = tob->pos.y + 1;
    rect.w = 1, rect.h = 3;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw border -- */
    /* - dark border - */
    SDL_SetRenderDrawColor(renderer, tob->BG_dark.r, tob->BG_dark.g, tob->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    rect.x = tob->pos.x, rect.y = tob->pos.y + 2;
    rect.w = tob->len - 1, rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    if (tob->flip == SDL_FLIP_HORIZONTAL) {
        rect.x = tob->pos.x, rect.y = tob->pos.y;
    } else {
        rect.x = tob->pos.x + tob->len - 2, rect.y = tob->pos.y;
    }
    rect.w = 1, rect.h = 2;
    SDL_RenderDrawRect(renderer, &rect);

    /* - light border - */
    SDL_SetRenderDrawColor(renderer, tob->BG_light.r, tob->BG_light.g,
                           tob->BG_light.b, SDL_ALPHA_OPAQUE);
    if (tob->flip == SDL_FLIP_HORIZONTAL) {
        rect.x = tob->pos.x + tob->len - 2, rect.y = tob->pos.y;
    } else {
        rect.x = tob->pos.x, rect.y = tob->pos.y;
    }
    rect.w = 1, rect.h = 2;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Compute Fill -- */
    int fill_px = (int)ceil((tob->len - 3) * tob->fill);
    SDL_assert(fill_px <= tob->len);

    /* -- Draw Fill -- */
    if (fill_px > 0) {
        /* - light fill - */
        SDL_SetRenderDrawColor(renderer, tob->FG_light.r, tob->FG_light.g,
                               tob->FG_light.b, SDL_ALPHA_OPAQUE);
        if (tob->flip == SDL_FLIP_HORIZONTAL) {
            rect.x = tob->pos.x + 1, rect.y = tob->pos.y;
        } else {
            rect.x = tob->pos.x + tob->len - fill_px - 2, rect.y = tob->pos.y;
        }
        rect.w = fill_px, rect.h = 1;
        SDL_RenderDrawRect(renderer, &rect);

        /* - dark fill - */
        SDL_SetRenderDrawColor(renderer, tob->FG_dark.r, tob->FG_dark.g,
                               tob->FG_dark.b, SDL_ALPHA_OPAQUE);

        if (tob->flip == SDL_FLIP_HORIZONTAL) {
            rect.x = tob->pos.x + 1, rect.y = tob->pos.y + 1;
        } else {
            rect.x = tob->pos.x + tob->len - 2 - fill_px, rect.y = tob->pos.y + 1;
        }
        rect.w = fill_px, rect.h = 1;
        SDL_RenderDrawRect(renderer, &rect);
    }

    /* -- Finish -- */
    Utilities_DrawColor_Reset(renderer);

}

