
#include "bars/stat.h"
#include "palette.h"

const int statbar_highlights[STATBAR_HIGHLIGHT_NUM] = {4, 12, 18, 22, 25, 26, 32, 35};

void StatBar_Init(SimpleBar *statbar,
                  int stat, int cap,
                  int posx, int posy) {

    statbar->pos.x = posx * statbar->scale.x;
    statbar->pos.y = posy * statbar->scale.y;
    statbar->len = StatBar_Len(cap) * statbar->scale.x;
    if (cap > 0)
        statbar->fill = ((float) stat) / ((float)cap);
    else
        statbar->fill = 0;
}

void HPBar_Init(SimpleBar *statbar,
                int stat, int cap,
                int posx, int posy) {
    StatBar_Init(statbar, stat, cap, posx, posy);
    statbar->len = HPBAR_LEN * statbar->scale.x;
}

/* --- STATBAR --- */
void StatBar_Colors(SimpleBar *statbar,
                    int BG_dark,
                    int BG_light,
                    int FG_dark,
                    int FG_light) {
    statbar->BG_dark    = palette_SOTA->colors[BG_dark];
    statbar->BG_light   = palette_SOTA->colors[BG_light];
    statbar->FG_dark    = palette_SOTA->colors[FG_dark];
    statbar->FG_light   = palette_SOTA->colors[FG_light];
}

int StatBar_Len(int cap) {
    int max_len = STATBAR_MAX_LEN;
    int cap_len = ((float)cap / STATBAR_MAX_CAP) * STATBAR_MAX_LEN;
    return (cap_len < max_len ? cap_len : max_len);
}

void HPBar_Draw(SimpleBar       *statbar,
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
    /* -- BG_dark: actual bar -- */
    SDL_SetRenderDrawColor(renderer, statbar->BG_dark.r, statbar->BG_dark.g,
                           statbar->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    out_rect.w = statbar->len * statbar->fill;
    out_rect.h = statbar->height * statbar->scale.y;
    out_rect.x = statbar->pos.x;
    out_rect.y = statbar->pos.y;
    temp_rect.w = out_rect.w - 1 * statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x + 3 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    BG_dark_rects[0] = temp_rect;
    temp_rect.x = out_rect.x + 2 * statbar->scale.x;
    temp_rect.y = out_rect.y + 2 * statbar->scale.y;
    BG_dark_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    BG_dark_rects[2] = temp_rect;
    SDL_RenderFillRects(renderer, BG_dark_rects,
                        BG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* top */
    /* -- FG_Dark: border box -- */
    SDL_SetRenderDrawColor(renderer, statbar->FG_dark.r, statbar->FG_dark.g,
                           statbar->FG_dark.b,
                           SDL_ALPHA_OPAQUE);
    temp_rect.h = statbar->scale.y;
    temp_rect.w = statbar->len - 4 * statbar->scale.x;
    temp_rect.x = out_rect.x + 3 * statbar->scale.y;
    temp_rect.y = out_rect.y;
    FG_dark_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = statbar->len - 3 * statbar->scale.x;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 4 * statbar->scale.y;
    FG_dark_rects[1] = temp_rect;
    /* right */
    temp_rect.w = statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y;
    FG_dark_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y + 1 * statbar->scale.x;
    FG_dark_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 2 * statbar->scale.x;
    FG_dark_rects[4] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len - 2 * statbar->scale.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.x;
    FG_dark_rects[5] = temp_rect;
    SDL_RenderFillRects(renderer, FG_dark_rects,
                        FG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, statbar->FG_light.r, statbar->FG_light.g,
                           statbar->FG_light.b, SDL_ALPHA_OPAQUE);
    /* left */
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    FG_light_rects[0] = temp_rect;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 2 * statbar->scale.y;
    FG_light_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 2 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    FG_light_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + 3 * statbar->scale.x;
    temp_rect.y = out_rect.y;
    FG_light_rects[3] = temp_rect;
    SDL_RenderFillRects(renderer, FG_light_rects,
                        FG_LIGHT_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- black shadows -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    temp_rect.w = statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 4 * statbar->scale.y;
    black_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = statbar->len - 2 * statbar->scale.x;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 5 * statbar->scale.y;
    black_rects[1] = temp_rect;
    /* right */
    temp_rect.h = 2 * statbar->scale.x;
    temp_rect.w = statbar->scale.y;
    temp_rect.x = out_rect.x + statbar->len - 2 * statbar->scale.x;
    temp_rect.y = out_rect.y + 4 * statbar->scale.y;
    black_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    black_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y + 2 * statbar->scale.y;
    black_rects[4] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    black_rects[5] = temp_rect;
    temp_rect.h = statbar->scale.x;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y;
    black_rects[6] = temp_rect;
    SDL_RenderFillRects(renderer, black_rects,
                        BLACK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    temp_rect.h = statbar->scale.y;
    temp_rect.w = statbar->scale.x * 2;
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, statbar->FG_light.r, statbar->FG_light.g,
                           statbar->FG_light.b, SDL_ALPHA_OPAQUE);
    for (i16 i = (HPBAR_PATTERN_START * statbar->scale.x); i < statbar->len;
         i += (HPBAR_PATTERN_MULTIPLIER * statbar->scale.x)) {
        temp_rect.x = out_rect.x + i;
        temp_rect.y = out_rect.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - 4 * statbar->scale.x;
        temp_rect.y = out_rect.y + 4 * statbar->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
    }
    /* -- BG_light: bar highlights -- */
    SDL_SetRenderDrawColor(renderer, statbar->BG_light.r, statbar->BG_light.g,
                           statbar->BG_light.b, SDL_ALPHA_OPAQUE);
    for (i16 i = (HPBAR_PATTERN_START * statbar->scale.x);
         i <= (statbar->len * statbar->fill); i += (HPBAR_PATTERN_MULTIPLIER * statbar->scale.x)) {
        temp_rect.x = out_rect.x + i - (statbar->scale.x);
        temp_rect.y = out_rect.y + 1 * statbar->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - (2 * statbar->scale.x);
        temp_rect.y = out_rect.y + 2 * statbar->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + i - (3 * statbar->scale.x);
        temp_rect.y = out_rect.y + 3 * statbar->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
    }
#undef BG_DARK_RECT_NUM
#undef BG_LIGHT_RECT_NUM
#undef FG_DARK_RECT_NUM
#undef FG_LIGHT_RECT_NUM
#undef BLACK_RECT_NUM
}

void StatBar_Draw(  SimpleBar       *statbar,
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
    SDL_SetRenderDrawColor(renderer, statbar->BG_dark.r, statbar->BG_dark.g,
                           statbar->BG_dark.b,
                           SDL_ALPHA_OPAQUE);
    out_rect.w = statbar->len * statbar->fill;
    out_rect.h = statbar->height * statbar->scale.y;
    out_rect.x = statbar->pos.x;
    out_rect.y = statbar->pos.y;
    temp_rect.w = out_rect.w - 1 * statbar->scale.x;
    temp_rect.h = ceil(((float)out_rect.h / 2.0f));
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    BG_dark_rects[0] = temp_rect;
    temp_rect.w = statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x + out_rect.w;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    BG_dark_rects[1] = temp_rect;
    SDL_RenderFillRects(renderer, BG_dark_rects,
                        BG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* top */
    /* -- FG_Dark: border box -- */
    SDL_SetRenderDrawColor(renderer, statbar->FG_dark.r, statbar->FG_dark.g,
                           statbar->FG_dark.b,
                           SDL_ALPHA_OPAQUE);
    temp_rect.h = out_rect.h / 4;
    temp_rect.w = statbar->len - 3 * statbar->scale.x;
    temp_rect.x = out_rect.x + 2 * statbar->scale.y;
    temp_rect.y = out_rect.y;
    FG_dark_rects[0] = temp_rect;
    /* bottom */
    temp_rect.w = statbar->len - 2 * statbar->scale.x;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    FG_dark_rects[1] = temp_rect;
    /* right */
    temp_rect.w = statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y;
    FG_dark_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y +  1 * statbar->scale.x;
    FG_dark_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 2 * statbar->scale.x;
    FG_dark_rects[4] = temp_rect;
    SDL_RenderFillRects(renderer, FG_dark_rects,
                        FG_DARK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- FG_light: border highlights -- */
    SDL_SetRenderDrawColor(renderer, statbar->FG_light.r, statbar->FG_light.g,
                           statbar->FG_light.b, SDL_ALPHA_OPAQUE);
    /* left */
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    FG_light_rects[0] = temp_rect;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 2 * statbar->scale.y;
    FG_light_rects[1] = temp_rect;
    temp_rect.x = out_rect.x + 2 * statbar->scale.x;
    temp_rect.y = out_rect.y;
    FG_light_rects[2] = temp_rect;
    SDL_RenderFillRects(renderer, FG_light_rects,
                        FG_LIGHT_RECT_NUM); /* faster/slower than SDL_FillRects? */
    /* -- black shadows -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    temp_rect.w = statbar->scale.x;
    temp_rect.h = statbar->scale.y;
    temp_rect.x = out_rect.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    black_rects[0] = temp_rect;
    temp_rect.w = statbar->len - 1 * statbar->scale.x;
    temp_rect.x = out_rect.x + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 4 * statbar->scale.y;
    black_rects[1] = temp_rect;
    temp_rect.h = 2 * statbar->scale.x;
    temp_rect.w = statbar->scale.y;
    temp_rect.x = out_rect.x + statbar->len - 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 3 * statbar->scale.y;
    black_rects[2] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y + 2 * statbar->scale.y;
    black_rects[3] = temp_rect;
    temp_rect.x = out_rect.x + statbar->len + 1 * statbar->scale.x;
    temp_rect.y = out_rect.y + 1 * statbar->scale.y;
    black_rects[4] = temp_rect;
    temp_rect.h = statbar->scale.x;
    temp_rect.x = out_rect.x + statbar->len;
    temp_rect.y = out_rect.y;
    black_rects[5] = temp_rect;
    SDL_RenderFillRects(renderer, black_rects,
                        BLACK_RECT_NUM); /* faster/slower than SDL_FillRects? */
    temp_rect.h = statbar->scale.y;
    temp_rect.w = statbar->scale.x;
    /* -- FG_light: border highlights -- */
    int i = 0;
    SDL_SetRenderDrawColor(renderer, statbar->FG_light.r, statbar->FG_light.g,
                           statbar->FG_light.b, SDL_ALPHA_OPAQUE);
    while ((statbar_highlights[i] < (statbar->len / statbar->scale.x))
           && (i < STATBAR_HIGHLIGHT_NUM)) {
        temp_rect.x = out_rect.x + statbar_highlights[i] * statbar->scale.x;
        temp_rect.y = out_rect.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 3) * statbar->scale.x;
        temp_rect.y = out_rect.y + 3 * statbar->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        i++;
    }
    /* -- BG_light: bar highlights -- */
    i = 0;
    SDL_SetRenderDrawColor(renderer, statbar->BG_light.r, statbar->BG_light.g,
                           statbar->BG_light.b, SDL_ALPHA_OPAQUE);
    while ((statbar_highlights[i] <= (statbar->len * statbar->fill / statbar->scale.x + 1))
           && (i < STATBAR_HIGHLIGHT_NUM)) {
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 1) * statbar->scale.x;
        temp_rect.y = out_rect.y + 1 * statbar->scale.y;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        temp_rect.x = out_rect.x + (statbar_highlights[i] - 2) * statbar->scale.x;
        temp_rect.y = out_rect.y + 2 * statbar->scale.x;
        SDL_RenderFillRect(renderer, &temp_rect); /* faster/slower than SDL_FillRects? */
        i++;
    }
#undef BG_DARK_RECT_NUM
#undef BG_LIGHT_RECT_NUM
#undef FG_DARK_RECT_NUM
#undef FG_LIGHT_RECT_NUM
#undef BLACK_RECT_NUM
}
