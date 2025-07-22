
#include "text.h"
#include "globals.h"
#include "game/game.h"
#include "utilities.h"
#include "pixelfonts.h"

#ifndef STB_SPRINTF_IMPLEMENTATION
    #define STB_SPRINTF_IMPLEMENTATION
    #include "stb_sprintf.h"
#endif /* STB_SPRINTF_IMPLEMENTATION */

const struct Text Text_default = {
    .visible        = true,
    .update         = true,
};

void Text_Init(         struct Text *text) {
    *text = Text_default;
}

void Text_Init_tnecs(   void *voidtext) {
    Text_Init(voidtext);
}

void Text_Free_tnecs(void *voidtext) {
    Text_Free(voidtext);
}

void Text_Free(struct Text *text) {
    if (text->texture != NULL) {
        SDL_DestroyTexture(text->texture);
        text->texture = NULL;
    }
}

/* -- Text: Standalone Pixelfont -- */
void Text_Set(struct Text *text, char *line, int offset) {
    text->len = strlen(line);
    SDL_assert(text->len > 0);
    memset(text->line, 0, DEFAULT_BUFFER_SIZE);
    memcpy(text->line, line, text->len);

    text->rect.w = PixelFont_Width(text->pixelfont, line, text->len);
    text->rect.h = text->pixelfont->glyph_height + offset;
    text->update = 1;
}

void Text_onUpdate_FPS(struct Game *sota,
                       tnecs_entity entity_fps,
                       void *data) {
    SDL_assert(sota         != NULL);
    SDL_assert(gl_world     != NULL);
    SDL_assert(entity_fps   != TNECS_NULL);

    Text *text = IES_GET_COMPONENT(gl_world, entity_fps, Text);

    if (sota->flags.fast_forward) {
        int fps_cap     = sota->settings.FPS.cap;
        int fps_ratio   = sota->fps.instant / fps_cap * SOTA_100PERCENT;
        stbsp_snprintf(text->line, sizeof(char) * 8, "%d%%\0\0\0\0", fps_ratio);
    } else {
        stbsp_snprintf(text->line, sizeof(char) * 5, "%.1f\0\0\0\0", sota->fps.instant);
    }

    text->len = strlen(text->line);
    SDL_assert(text->len        >  0);
    int width = PixelFont_Width(text->pixelfont, text->line, text->len);
    if (width != text->rect.w) {
        text->rect.w = width;
        text->texture = NULL;
        if (text->texture != NULL) {
            SDL_DestroyTexture(text->texture);
        }
        text->texture = NULL;
    }
    text->rect.h    = text->pixelfont->glyph_height;
    text->update    = true;
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));
}

void Text_Update(struct Text *text, SDL_Renderer *renderer) {
    SDL_assert(renderer         != NULL);
    SDL_assert(text->pixelfont  != NULL);
    if (text->len <= 0) {
        return;
    }
    SDL_assert(text->len        >  0);

    /* - create render target texture - */
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));

    if (text->texture == NULL) {
        text->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_TARGET,
                                          text->rect.w, text->rect.h);
        SDL_SetTextureBlendMode(text->texture, SDL_BLENDMODE_BLEND);
        SDL_assert(text->texture != NULL);
    }
    SDL_SetRenderTarget(renderer, text->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    PixelFont_Write(text->pixelfont, renderer, text->line, text->len, 0, 0);
    SDL_SetRenderTarget(renderer, NULL);
}

void Text_Draw(struct Text *text, SDL_Renderer *renderer, SDL_Rect *dstrect) {
    SDL_assert(text     != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert((dstrect->w > 0) && (dstrect->h > 0));

    if (text->update) {
        Text_Update(text, renderer);
        text->update = false;
    }
    SDL_RenderCopy(renderer, text->texture, NULL, dstrect);
    Utilities_DrawColor_Reset(renderer);
}
