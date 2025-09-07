
#include "bars/stat.h"
#include "palette.h"

const int statbar_highlights[STATBAR_HIGHLIGHT_NUM] = {
    4, 12, 18, 22, 25, 26, 32, 35
};

void StatBar_Init(SimpleBar *sb,
                  int stat, int cap,
                  int posx, int posy) {

    sb->pos.x = posx * sb->scale.x;
    sb->pos.y = posy * sb->scale.y;
    sb->len = StatBar_Len(cap) * sb->scale.x;

    if (cap <= 0) {
        sb->fill = 0;
        return;
    }
    /* cap is non-zero */
    sb->fill = (((float) stat) / ((float)cap));
}

void HPBar_Init(SimpleBar *sb,
                int stat, int cap,
                int posx, int posy) {
    StatBar_Init(sb, stat, cap, posx, posy);
    sb->len = HPBAR_LEN * sb->scale.x;
}

/* --- STATBAR --- */
void StatBar_Colors(SimpleBar *sb,
                    int BG_dark, int BG_light,
                    int FG_dark, int FG_light,
                    int empty) {
    sb->BG_dark    = palette_SOTA->colors[BG_dark];
    sb->BG_light   = palette_SOTA->colors[BG_light];
    sb->FG_dark    = palette_SOTA->colors[FG_dark];
    sb->FG_light   = palette_SOTA->colors[FG_light];
    sb->empty      = palette_SOTA->colors[empty];
}

int StatBar_Len(int cap) {
    int max_len = STATBAR_MAX_LEN;
    int cap_len = ((float)cap / STATBAR_MAX_CAP) * STATBAR_MAX_LEN;
    return (cap_len < max_len ? cap_len : max_len);
}

void HPBar_Draw(SimpleBar       *sb,
                SDL_Renderer    *renderer) {
    SDL_Rect out_rect;
    SDL_Rect temp_rect;
#define BG_DARK_RECT_NUM 3
#define BG_LIGHT_RECT_NUM 2
#define FG_DARK_RECT_NUM 6
#define FG_LIGHT_RECT_NUM 4
#define BLACK_RECT_NUM 7
    SDL_Rect BG_dark_rects[BG_DARK_RECT_NUM];
    SDL_Rect FG_dark_rects[FG_DARK_RECT_NUM];
    // SDL_Rect BG_light_rects[BG_LIGHT_RECT_NUM];
    SDL_Rect FG_light_rects[FG_LIGHT_RECT_NUM];
    SDL_Rect black_rects[BLACK_RECT_NUM];

    /* -- empty: non-filled space -- */
    SDL_SetRenderDrawColor( renderer,
                            sb->empty.r, sb->empty.g,
                            sb->empty.b, SDL_ALPHA_OPAQUE);
    temp_rect.w = sb->len       * sb->scale.y - 1;
    temp_rect.h = sb->height    * sb->scale.y - 1;
    temp_rect.x = sb->pos.x + 2 * sb->scale.x;
    temp_rect.y = sb->pos.y + 1 * sb->scale.y;
    SDL_RenderFillRect(renderer, &temp_rect);

    /* -- BG_dark: actual bar -- */
    SDL_SetRenderDrawColor(renderer, sb->BG_dark.r, sb->BG_dark.g,
                           sb->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    /* Note: always show more than one pixel */
    out_rect.w = sb->len * sb->fill;
    if ((sb->fill > 0.001f) && (out_rect.w < (3 * sb->scale.x))) {
        out_rect.w = (3 * sb->scale.x);
    }
    out_rect.h = sb->height * sb->scale.y;
    out_rect.x = sb->pos.x;
    out_rect.y = sb->pos.y;
    temp_rect.w = out_rect.w - 1 * sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x + 3 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    BG_dark_rects[0] = temp_rect;
    temp_rect.x = out_rect.x + 2 * sb->scale.x;
    temp_rect.y = out_rect.y + 2 * sb->scale.y;
    BG_dark_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    BG_dark_rects[2] = temp_rect;
    SDL_RenderFillRects(renderer, BG_dark_rects,
                        BG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* top */
    /* -- FG_Dark: border box -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_dark.r, sb->FG_dark.g,
                           sb->FG_dark.b,
                           SDL_ALPHA_OPAQUE);
    temp_rect.h = sb->scale.y;
    temp_rect.w = sb->len - 4 * sb->scale.x;
    temp_rect.x = out_rect.x + 3 * sb->scale.y;
    temp_rect.y = out_rect.y;
    FG_dark_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = sb->len - 3 * sb->scale.x;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 4 * sb->scale.y;
    FG_dark_rects[1] = temp_rect;
    /* right */
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y;
    FG_dark_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y + 1 * sb->scale.x;
    FG_dark_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 2 * sb->scale.x;
    FG_dark_rects[4] = temp_rect;
    temp_rect.x = out_rect.x + sb->len - 2 * sb->scale.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.x;
    FG_dark_rects[5] = temp_rect;
    SDL_RenderFillRects(renderer, FG_dark_rects,
                        FG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_light.r, sb->FG_light.g,
                           sb->FG_light.b, SDL_ALPHA_OPAQUE);
    /* left */
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    FG_light_rects[0] = temp_rect;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 2 * sb->scale.y;
    FG_light_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 2 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    FG_light_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + 3 * sb->scale.x;
    temp_rect.y = out_rect.y;
    FG_light_rects[3] = temp_rect;
    SDL_RenderFillRects(renderer, FG_light_rects,
                        FG_LIGHT_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- black shadows -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 4 * sb->scale.y;
    black_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = sb->len - 2 * sb->scale.x;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 5 * sb->scale.y;
    black_rects[1] = temp_rect;
    /* right */
    temp_rect.h = 2 * sb->scale.x;
    temp_rect.w = sb->scale.y;
    temp_rect.x = out_rect.x + sb->len - 2 * sb->scale.x;
    temp_rect.y = out_rect.y + 4 * sb->scale.y;
    black_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    black_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y + 2 * sb->scale.y;
    black_rects[4] = temp_rect;
    temp_rect.x = out_rect.x + sb->len + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    black_rects[5] = temp_rect;
    temp_rect.h = sb->scale.x;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y;
    black_rects[6] = temp_rect;
    SDL_RenderFillRects(renderer, black_rects,
                        BLACK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    temp_rect.h = sb->scale.y;
    temp_rect.w = sb->scale.x * 2;
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_light.r, sb->FG_light.g,
                           sb->FG_light.b, SDL_ALPHA_OPAQUE);
    for (i16 i = (HPBAR_PATTERN_START * sb->scale.x); i < sb->len;
         i += (HPBAR_PATTERN_MULTIPLIER * sb->scale.x)) {
        temp_rect.x = out_rect.x + i;
        temp_rect.y = out_rect.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - 4 * sb->scale.x;
        temp_rect.y = out_rect.y + 4 * sb->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
    }
    /* -- BG_light: bar highlights -- */
    SDL_SetRenderDrawColor(renderer, sb->BG_light.r, sb->BG_light.g,
                           sb->BG_light.b, SDL_ALPHA_OPAQUE);
    for (i16 i = (HPBAR_PATTERN_START * sb->scale.x);
         i <= (sb->len * sb->fill); i += (HPBAR_PATTERN_MULTIPLIER * sb->scale.x)) {
        temp_rect.x = out_rect.x + i - (sb->scale.x);
        temp_rect.y = out_rect.y + 1 * sb->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - (2 * sb->scale.x);
        temp_rect.y = out_rect.y + 2 * sb->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - (3 * sb->scale.x);
        temp_rect.y = out_rect.y + 3 * sb->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
    }
#undef BG_DARK_RECT_NUM
#undef BG_LIGHT_RECT_NUM
#undef FG_DARK_RECT_NUM
#undef FG_LIGHT_RECT_NUM
#undef BLACK_RECT_NUM
}

void StatBar_Draw(  SimpleBar       *sb,
                    SDL_Renderer    *renderer) {
    SDL_Rect out_rect;
    SDL_Rect temp_rect;
#define BG_DARK_RECT_NUM 2
#define BG_LIGHT_RECT_NUM 2
#define FG_DARK_RECT_NUM 5
#define FG_LIGHT_RECT_NUM 3
#define BLACK_RECT_NUM 6
    SDL_Rect BG_dark_rects[BG_DARK_RECT_NUM];
    SDL_Rect FG_dark_rects[FG_DARK_RECT_NUM];
    // SDL_Rect BG_light_rects[BG_LIGHT_RECT_NUM];
    SDL_Rect FG_light_rects[FG_LIGHT_RECT_NUM];
    SDL_Rect black_rects[BLACK_RECT_NUM];
    /* -- BG_dark: actual bar -- */
    SDL_SetRenderDrawColor(renderer, sb->BG_dark.r, sb->BG_dark.g,
                           sb->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    out_rect.w = sb->len * sb->fill;
    out_rect.h = sb->height * sb->scale.y;
    out_rect.x = sb->pos.x;
    out_rect.y = sb->pos.y;
    temp_rect.w = out_rect.w - 1 * sb->scale.x;
    temp_rect.h = ceil(((float)out_rect.h / 2.0f));
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    BG_dark_rects[0] = temp_rect;
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x + out_rect.w;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    BG_dark_rects[1] = temp_rect;
    SDL_RenderFillRects(renderer, BG_dark_rects,
                        BG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* top */
    /* -- FG_Dark: border box -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_dark.r, sb->FG_dark.g,
                           sb->FG_dark.b,
                           SDL_ALPHA_OPAQUE);
    temp_rect.h = out_rect.h / 4;
    temp_rect.w = sb->len - 3 * sb->scale.x;
    temp_rect.x = out_rect.x + 2 * sb->scale.y;
    temp_rect.y = out_rect.y;
    FG_dark_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = sb->len - 2 * sb->scale.x;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    FG_dark_rects[1] = temp_rect;
    /* right */
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y;
    FG_dark_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y +  1 * sb->scale.x;
    FG_dark_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 2 * sb->scale.x;
    FG_dark_rects[4] = temp_rect;
    SDL_RenderFillRects(renderer, FG_dark_rects,
                        FG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, sb->FG_light.r, sb->FG_light.g,
                           sb->FG_light.b, SDL_ALPHA_OPAQUE);
    /* left */
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    FG_light_rects[0] = temp_rect;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 2 * sb->scale.y;
    FG_light_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 2 * sb->scale.x;
    temp_rect.y = out_rect.y;
    FG_light_rects[2] = temp_rect;
    SDL_RenderFillRects(renderer, FG_light_rects,
                        FG_LIGHT_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- black shadows -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    temp_rect.w = sb->scale.x;
    temp_rect.h = sb->scale.y;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    black_rects[0] = temp_rect;
    temp_rect.w = sb->len - 1 * sb->scale.x;
    temp_rect.x = out_rect.x + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 4 * sb->scale.y;
    black_rects[1] = temp_rect;
    temp_rect.h = 2 * sb->scale.x;
    temp_rect.w = sb->scale.y;
    temp_rect.x = out_rect.x + sb->len - 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 3 * sb->scale.y;
    black_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y + 2 * sb->scale.y;
    black_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + sb->len + 1 * sb->scale.x;
    temp_rect.y = out_rect.y + 1 * sb->scale.y;
    black_rects[4] = temp_rect;
    temp_rect.h = sb->scale.x;
    temp_rect.x = out_rect.x + sb->len;
    temp_rect.y = out_rect.y;
    black_rects[5] = temp_rect;
    SDL_RenderFillRects(renderer, black_rects,
                        BLACK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    temp_rect.h = sb->scale.y;
    temp_rect.w = sb->scale.x;
    /* -- FG_light: border highlights -- */
    int i = 0;
    SDL_SetRenderDrawColor(renderer, sb->FG_light.r, sb->FG_light.g,
                           sb->FG_light.b, SDL_ALPHA_OPAQUE);
    while ((statbar_highlights[i] < (sb->len / sb->scale.x))
           && (i < STATBAR_HIGHLIGHT_NUM)) {
        temp_rect.x = out_rect.x + statbar_highlights[i] * sb->scale.x;
        temp_rect.y = out_rect.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 3) * sb->scale.x;
        temp_rect.y = out_rect.y + 3 * sb->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        i++;
    }
    /* -- BG_light: bar highlights -- */
    i = 0;
    SDL_SetRenderDrawColor(renderer, sb->BG_light.r, sb->BG_light.g,
                           sb->BG_light.b, SDL_ALPHA_OPAQUE);
    while ((statbar_highlights[i] <= (sb->len * sb->fill / sb->scale.x + 1))
           && (i < STATBAR_HIGHLIGHT_NUM)) {
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 1) * sb->scale.x;
        temp_rect.y = out_rect.y + 1 * sb->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 2) * sb->scale.x;
        temp_rect.y = out_rect.y + 2 * sb->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        i++;
    }
#undef BG_DARK_RECT_NUM
#undef BG_LIGHT_RECT_NUM
#undef FG_DARK_RECT_NUM
#undef FG_LIGHT_RECT_NUM
#undef BLACK_RECT_NUM
}
