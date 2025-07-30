
#include "text.h"
#include "globals.h"
#include "game/game.h"
#include "utilities.h"
#include "position.h"
#include "pixelfonts.h"

#ifndef STB_SPRINTF_IMPLEMENTATION
    #define STB_SPRINTF_IMPLEMENTATION
    #include "stb_sprintf.h"
#endif /* STB_SPRINTF_IMPLEMENTATION */

/* --- Platform: SDL --- */
/* Opaque struct for SDL specific code */
struct P_Text {
    SDL_Texture     *texture;
    SDL_Renderer    *renderer;
};

const struct Text Text_default = {
    .visible        = true,
    .update         = true,
};

void Text_Free_tnecs(void *voidtext) {
    Text_Free(voidtext);
}

void Text_Init_tnecs(void *voidtext) {
    Text_Init(voidtext);
}
/* -- Text: Standalone Pixelfont -- */
void Text_Place(Text           *text,
                const struct Position * const pos) {
    SDL_assert(pos  != NULL);
    SDL_assert(text != NULL);

    text->dstrect.x = pos->pixel_pos.x;
    text->dstrect.y = pos->pixel_pos.y;
    text->dstrect.w = text->size.x * pos->scale[0];
    text->dstrect.h = text->size.y * pos->scale[1];
}

void Text_Set(struct Text *text,
              char *line, int offset) {
    /* -- Check: line can fit in buffer -- */
    text->len = strlen(line);
    SDL_assert(text->len > 0);
    SDL_assert(text->len < DEFAULT_BUFFER_SIZE);

    /* -- Copy text up to buffer size -- */
    memset(text->line, 0, DEFAULT_BUFFER_SIZE);
    memcpy(text->line, line, text->len);

    /* -- Compute text width in pixels -- */
    text->size.x = PixelFont_Width(text->pixelfont, line, text->len);
    text->size.y = text->pixelfont->glyph_height + offset;
    text->update = 1;
}

void Text_onUpdate_FPS(struct Game *sota,
                       tnecs_entity entity_fps,
                       void *data) {
    SDL_assert(sota         != NULL);
    SDL_assert(gl_world     != NULL);
    SDL_assert(entity_fps   != TNECS_NULL);

    Text *text = IES_GET_COMPONENT(gl_world, entity_fps, Text);
    SDL_assert(text         != NULL);
    SDL_assert(text->plat   != NULL);

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
    if (width != text->size.x) {
        text->size.x = width;
        P_Text_Free(text);
    }
    text->size.y    = text->pixelfont->glyph_height;
    text->update    = true;
    SDL_assert((text->size.x > 0) && (text->size.y > 0));
}

void Text_Free(struct Text *text) {
    if (text == NULL) {
        return;
    }
    if (text->plat == NULL) {
        return;
    }
    P_Text_Free(text);
    SDL_free(text->plat);
    text->plat = NULL;
}

void Text_Init(struct Text *text) {
    Text_Free(text);
    *text = Text_default;
}

/* --- PLATFORM: Rendering --- */
void P_Text_Init(struct Text    *text,
                 void           *renderer) {
    SDL_assert(text->plat   == NULL);
    SDL_assert(renderer     != NULL);
    size_t bytesize = sizeof(struct P_Text);
    text->plat = SDL_calloc(1, bytesize);
    text->plat->renderer = renderer;
}

void P_Text_Free(struct Text *text) {
    if (text == NULL) {
        return;
    }

    if (text->plat == NULL) {
        return;
    }

    if (text->plat->texture != NULL) {
        SDL_DestroyTexture(text->plat->texture);
        text->plat->texture = NULL;
    }
}

void Text_Update(struct Text    *text) {
    SDL_assert(text                 != NULL);
    SDL_assert(text->plat           != NULL);
    SDL_assert(text->pixelfont      != NULL);
    SDL_assert(text->plat->renderer != NULL);

    if (text->len <= 0) {
        /* Text was not set, skip */
        return;
    }

    /* - create render target texture - */
    SDL_assert(text->size.x > 0);
    SDL_assert(text->size.y > 0);
    SDL_Texture     *texture    = text->plat->texture;
    SDL_Renderer    *renderer   = text->plat->renderer;
    if (texture == NULL) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    text->size.x, text->size.y
                                   );
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        text->plat->texture = texture;
    }
    SDL_assert(texture != NULL);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    PixelFont_Write(text->pixelfont, renderer, text->line, text->len, 0, 0);
    SDL_SetRenderTarget(renderer, NULL);
}

void Text_Draw(struct Text  *text) {
    SDL_assert(text                 != NULL);
    SDL_assert(text->plat           != NULL);
    SDL_assert(text->pixelfont      != NULL);
    SDL_assert(text->plat->renderer != NULL);
    SDL_assert(text->dstrect.w > 0);
    SDL_assert(text->dstrect.h > 0);


    if (text->update) {
        Text_Update(text);
        text->update = false;
    }

    SDL_Rect sdl_dstrect =  {
        sdl_dstrect.x = text->dstrect.x,
        sdl_dstrect.y = text->dstrect.y,
        sdl_dstrect.w = text->dstrect.w,
        sdl_dstrect.h = text->dstrect.h
    };
    SDL_Texture     *texture    = text->plat->texture;
    SDL_Renderer    *renderer   = text->plat->renderer;
    SDL_RenderCopy(renderer,    texture,
                   NULL,        &sdl_dstrect);
    Utilities_DrawColor_Reset(renderer);
}
