
#include "bars/circle.h"
#include "structs.h"
#include "palette.h"
#include "utilities.h"

/* --- CIRCLE_BAR --- */
const struct CircleBar CircleBar_default = {
    .flip = SDL_FLIP_NONE,
};

void CircleBar_Fill(struct CircleBar *circlebar, int attack_i, int attack_num) {
    SDL_assert(attack_num > 0);
    SDL_assert(attack_i <= attack_num);
    circlebar->fill = attack_i * CIRCLE_BAR_FILL_LEN / attack_num;
}

void CircleBar_Draw(struct CircleBar *circlebar, SDL_Renderer *renderer) {
    /*   FLIPPED BAR     NO FLIP BAR    */
    /*  |     -------|  |--------    |  */
    /*  |____________|  |____________|  */

    SDL_Rect rect = {0};

    /* -- Draw shadow -- */
    SDL_Color shadow = palette_SOTA->colors[CIRCLE_BAR_SHADOW];
    SDL_SetRenderDrawColor(renderer, shadow.r, shadow.g, shadow.b, SDL_ALPHA_OPAQUE);
    rect.x = circlebar->pos.x, rect.y = circlebar->pos.y + 6;
    rect.w = CIRCLE_BAR_WIDTH, rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);
    rect.x = circlebar->pos.x + 7, rect.y = circlebar->pos.y + 1;
    rect.w = 1, rect.h = 6;
    SDL_RenderDrawRect(renderer, &rect);

    rect.x = circlebar->pos.x + 1, rect.y = circlebar->pos.y + 7;
    rect.w = 6, rect.h = 1;
    SDL_RenderDrawRect(renderer, &rect);

    /* -- Draw border -- */
    /* - in border - */
    SDL_Color border_light = palette_SOTA->colors[CIRCLE_BAR_BORDER_DARK];
    SDL_SetRenderDrawColor(renderer, border_light.r, border_light.g, border_light.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 1);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 1);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 5);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 5);

    rect.w = 3, rect.h = 1;
    rect.x = circlebar->pos.x + 2, rect.y = circlebar->pos.y + 0;
    SDL_RenderDrawRect(renderer, &rect);
    rect.x = circlebar->pos.x + 2, rect.y = circlebar->pos.y + 6;
    SDL_RenderDrawRect(renderer, &rect);
    rect.w = 1, rect.h = 3;
    rect.x = circlebar->pos.x + 0, rect.y = circlebar->pos.y + 2;
    SDL_RenderDrawRect(renderer, &rect);
    rect.x = circlebar->pos.x + 6, rect.y = circlebar->pos.y + 2;
    SDL_RenderDrawRect(renderer, &rect);

    /* - out border - */
    SDL_Color border_dark = palette_SOTA->colors[CIRCLE_BAR_BORDER_LIGHT];
    SDL_SetRenderDrawColor(renderer, border_dark.r, border_dark.g, border_dark.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 0, circlebar->pos.y + 1);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 0, circlebar->pos.y + 5);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 0);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 0);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 6, circlebar->pos.y + 1);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 6);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 6);
    SDL_RenderDrawPoint(renderer, circlebar->pos.x + 6, circlebar->pos.y + 5);

    /* -- Draw Fill -- */
    SDL_Color fill_dark = palette_SOTA->colors[CIRCLE_BAR_FILL_DARK];
    SDL_Color fill_light = palette_SOTA->colors[CIRCLE_BAR_FILL_LIGHT];
    SDL_Color fill = palette_SOTA->colors[CIRCLE_BAR_FILL];

    if (circlebar->flip == SDL_FLIP_NONE) {
        /*      NO FLIP     */
        /*  |--------    |  */
        /*  |____________|  */
        switch (circlebar->fill) {
            // Fall through cases, filling more the higher.
            case 5:
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 2);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 3);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 4);
            case 4:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 4, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 4, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 3;
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 4, circlebar->pos.y + 5);
            case 3:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 3, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 3, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 3;
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 3, circlebar->pos.y + 5);
            case 2:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 2, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 2, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 4;
                SDL_RenderDrawRect(renderer, &rect);
            case 1:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 2);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 3);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 4);
        }
    } else if (circlebar->flip == SDL_FLIP_HORIZONTAL)  {
        /*     FLIPPED      */
        /*  |     -------|  */
        /*  |____________|  */

        switch (circlebar->fill) {
            // Fall through cases, filling more the higher.
            case 5:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 2);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 3);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 1, circlebar->pos.y + 4);
            case 4:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 2, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 2, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 4;
                SDL_RenderDrawRect(renderer, &rect);
            case 3:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 3, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 3, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 3;
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 3, circlebar->pos.y +  5);
            case 2:
                SDL_SetRenderDrawColor(renderer, fill_light.r, fill_light.g, fill_light.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 4, circlebar->pos.y + 1);
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                rect.x = circlebar->pos.x + 4, rect.y = circlebar->pos.y + 2;
                rect.w = 1, rect.h = 3;
                SDL_RenderDrawRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 4, circlebar->pos.y + 5);
            case 1:
                SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 2);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 3);
                SDL_SetRenderDrawColor(renderer, fill_dark.r, fill_dark.g, fill_dark.b, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawPoint(renderer, circlebar->pos.x + 5, circlebar->pos.y + 4);
        }
    }

    Utilities_DrawColor_Reset(renderer);
}