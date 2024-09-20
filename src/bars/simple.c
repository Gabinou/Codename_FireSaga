#include "bars/simple.h"

struct SimpleBar SimpleBar_default = {
    .fill   = 1.0f,
    .len    = 10,  /* [pixels] */
    .height = SIMPLEBAR_HEIGHT,  /* [pixels] */
    .flip   = SDL_FLIP_NONE,
};

void SimpleBar_Draw(struct SimpleBar *sb, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    SDL_Rect FG_dark_rects[SIMPLEBAR_FG_DARK_RECT_NUM];
    SDL_Rect black_rects[SIMPLEBAR_BLACK_RECT_NUM];
    SDL_Rect temp_rect;
    SDL_Rect out_rect;
    out_rect.w = sb->len * sb->fill;
    out_rect.h = sb->height * sb->scale.y;

    /* -- BG_dark: actual bar -- */
    SDL_SetRenderDrawColor(renderer, sb->BG_dark.r, sb->BG_dark.g,
                           sb->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    temp_rect.w = out_rect.w * sb->scale.y;
    temp_rect.h = sb->height * sb->scale.y;
    temp_rect.x = sb->pos.x + 1 * sb->scale.x;
    temp_rect.y = sb->pos.y + 1 * sb->scale.y;
    SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */

    /* -- FG_Dark: border box -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_dark.r, sb->FG_dark.g,
                           sb->FG_dark.b, SDL_ALPHA_OPAQUE);
    /* top */
    temp_rect.w = (sb->len - 1) * sb->scale.x;
    temp_rect.w = (sb->len - 1) * sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = sb->pos.x + sb->scale.x;
    temp_rect.y = sb->pos.y;
    FG_dark_rects[0] = temp_rect;
    /* bottom */
    temp_rect.y += (sb->height + 1) * sb->scale.y;
    FG_dark_rects[1] = temp_rect;
    /* right */
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->height * sb->scale.y;
    temp_rect.x -= sb->scale.x;
    temp_rect.y = sb->pos.y + 1 * sb->scale.y;
    FG_dark_rects[2] = temp_rect;
    /* right */
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->height * sb->scale.y;
    temp_rect.x = sb->pos.x + sb->len * sb->scale.x;
    FG_dark_rects[3] = temp_rect;
    SDL_RenderFillRects(renderer, FG_dark_rects, SIMPLEBAR_FG_DARK_RECT_NUM);

    /* -- black shadows -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    /* bottom bar side */
    temp_rect.w = (sb->len - 1) * sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = sb->pos.x + sb->scale.x;
    temp_rect.y = sb->pos.y + (sb->height + 2) * sb->scale.y;
    black_rects[0] = temp_rect;
    /* left side */
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = sb->pos.x;
    temp_rect.y = sb->pos.y + (1 + sb->height) * sb->scale.y;
    black_rects[1] = temp_rect;
    /* right side */
    temp_rect.h = 2 * sb->scale.y;
    temp_rect.x = sb->pos.x + sb->len * sb->scale.x;
    black_rects[2] = temp_rect;
    temp_rect.h = (sb->height + 1) * sb->scale.y;
    temp_rect.x += sb->scale.x;
    temp_rect.y = sb->pos.y + 1 * sb->scale.y;
    black_rects[3] = temp_rect;
    temp_rect.h = sb->scale.y;
    temp_rect.x -= sb->scale.x;
    temp_rect.y -= sb->scale.y;
    black_rects[4] = temp_rect;
    SDL_RenderFillRects(renderer, black_rects, SIMPLEBAR_BLACK_RECT_NUM);
}
