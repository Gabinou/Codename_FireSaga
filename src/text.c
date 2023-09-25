#include "text.h"

struct Text_TTF Text_TTF_default = {
    .texture =    NULL,
    .text_line =  {'\0'},

    // .text_color = NULL,
    // .srcrects = NULL,
    // .dstrects = NULL,

    .padding =    {10, 10, 10, 10}, //html style: up right down left
    .fontsize =   14,

    .visible =         true,
    .spacingfactor =   1.1f,
    // .sizefactor = {1.0f, 1.0f}, // height, width
};

void Text_TTF_onUpdate_FPS(struct Game *sota, tnecs_entity_t entity_fps,
                           u32 frame_count, i64 last_update_ns, void *data) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota != NULL);
    SDL_assert(sota->world != NULL);
    SDL_assert(entity_fps != TNECS_NULL);

    struct Text_TTF *fps_text_ptr = TNECS_GET_COMPONENT(sota->world, entity_fps, Text_TTF);
    float ratio = (float)SOTA_ns / (float)last_update_ns;
    float fps = (frame_count * ratio);
    if (sota->fast_forward) {
        int fps_cap = sota->settings.FPS.cap;
        int fps_ratio = fps / fps_cap;
        snprintf(fps_text_ptr->text_line, sizeof(char) * DEFAULT_BUFFER_SIZE, "x%d", fps_ratio);
    } else {
        snprintf(fps_text_ptr->text_line, sizeof(char) * DEFAULT_BUFFER_SIZE, "%.1f", fps);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_TTF_Free(struct Text_TTF *text) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (text->texture != NULL)
        SDL_DestroyTexture(text->texture);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_TTF_Texture_Make(struct Text_TTF *text, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // SDL_Surface * textsurface = textToSurface(line, text_color, text->font);
    Text_TTF_Free(text);

    // Blended is better than Solid
    struct SDL_Surface *textsurface;
    textsurface = TTF_RenderText_Blended(text->font, text->text_line, text->text_color);
    i32 temp_height, temp_width;
    SDL_assert(text->font != NULL);
    TTF_SizeUTF8(text->font, text->text_line, &temp_width, &temp_height);
    text->srcrect.w = temp_width;
    text->srcrect.h = temp_height;
    text->srcrect.x = 0;
    text->srcrect.y = 0;
    text->dstrect.w = temp_width;
    text->dstrect.h = temp_height;
    text->dstrect.x = 0;
    text->dstrect.y = 0;
    text->texture = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_TTF_Rects_Pos_Set(struct Text_TTF *text, if16 x, if16 y) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    text->dstrect.x = x + text->padding[3];
    text->dstrect.y = y + text->padding[0];
    i32 hgt, wdt;
    i32 temp_height, temp_width;
    SDL_QueryTexture(text->texture, NULL, NULL, &wdt, &hgt);
    SDL_assert(!((hgt != text->srcrect.h) | (wdt != text->srcrect.w)));
    TTF_SizeUTF8(text->font, text->text_line, &temp_width, &temp_height);
    text->dstrect.h = (i32)temp_height * text->sizefactor[0];
    text->dstrect.w = (i32)temp_width * text->sizefactor[1];
    text->dstrect.x = text->dstrect.x;
    text->dstrect.y = text->dstrect.y;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_TTF_Draw(struct Text_TTF *text, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // SOTA_Log_Debug("Drawing text");
    // Find a way to draw text letter by letter, word by word, etc. for future script.
    SDL_RenderCopy(renderer, text->texture, &text->srcrect, &text->dstrect);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_onUpdate_scroll(struct Text *text) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // After a specified time, increase number of characters rendered, making text scroll
    text->len2draw += (text->len2draw >= text->strlen) ? 1 : 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_Update(struct Text *text, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* - create render target texture - */
    if (text->texture == NULL) {
        text->texture = SDL_CreateTexture(text->renderer, SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_TARGET, text->box_x, text->box_y);
        SDL_assert(text->texture != NULL);
        SDL_SetTextureBlendMode(text->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(text->renderer, text->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(text->renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_assert(text->pixelfont != NULL);
    SDL_assert(text->renderer != NULL);
    SDL_assert(text->len2draw > 0);
    PixelFont_Write(text->pixelfont, text->renderer, text->line, text->len2draw, 0, 0);
    SDL_SetRenderTarget(text->renderer, NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Text_Draw(struct Text *text, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(text != NULL);
    if (text->update) {
        Text_Update(text, renderer);
        text->update = false;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
