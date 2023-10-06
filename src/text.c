#include "text.h"

struct Text Text_default = {
    .texture     = NULL,
    .pixelfont   = NULL,
    .line        = 0,
    .len         = 0,
    .rect        = {0},
    .onUpdate    = NULL,
    .visible     = true,
    .update      = true,
};

/* -- Text: Standalone Pixelfont -- */
void Text_Set(struct Text *text, char *line) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    text->len = strlen(line);
    SDL_assert(text->len > 0);
    memset(text->line, 0, DEFAULT_BUFFER_SIZE);
    strncpy(text->line, line, text->len);

    text->rect.w = PixelFont_Width(text->pixelfont, line, text->len);
    text->rect.h = text->pixelfont->glyph_height;

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_onUpdate_FPS(struct Game *sota, tnecs_entity_t entity_fps,
                       u32 frame_count, i64 last_update_ns, void *data) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota != NULL);
    SDL_assert(sota->world != NULL);
    SDL_assert(entity_fps != TNECS_NULL);

    struct Text *text   = TNECS_GET_COMPONENT(sota->world, entity_fps, Text);
    float ratio             = (float)SOTA_ns / (float)last_update_ns;
    float fps               = (frame_count * ratio);
    if (sota->fast_forward) {
        int fps_cap     = sota->settings.FPS.cap;
        int fps_ratio   = fps / fps_cap * SOTA_100PERCENT;
        snprintf(text->line, sizeof(char) * DEFAULT_BUFFER_SIZE, "%d%%", fps_ratio);
    } else {
        snprintf(text->line, sizeof(char) * DEFAULT_BUFFER_SIZE, "%.1f", fps);
    }

    text->len       = strlen(text->line);
    text->rect.w    = PixelFont_Width(text->pixelfont, text->line, text->len);
    text->rect.h    = text->pixelfont->glyph_height;
    text->update    = true;

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_Update(struct Text *text, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(renderer         != NULL);
    SDL_assert(text->pixelfont  != NULL);

    /* - create render target texture - */
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));

    if (text->texture == NULL) {
        text->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET,
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_Draw(struct Text *text, SDL_Renderer *renderer, SDL_Rect *dstrect) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(text     != NULL);
    SDL_assert(renderer != NULL);
    if (text->update) {
        Text_Update(text, renderer);
        text->update = false;
    }
    SDL_RenderCopy(renderer, text->texture, NULL, dstrect);
    Utilities_DrawColor_Reset(renderer);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
