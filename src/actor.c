#include "actor.h"
#include "palette.h"
#include "utilities.h"

const Actor Actor_default = {
    .visible    = 1,
    .update     = 1,
};

/* Position of elements rows on spritesheet */
//  Every character has different numbers of elements
i32 Actor_Spritesheet_Body_Y(i32 body_id) {
    // TODO: check for valid index
    return (ACTOR_BODY_HEIGHT * body_id);
}

i32 Actor_Spritesheet_Face_Y(i32 face_id) {
    // TODO: check for valid index
    return (ACTOR_FACE_HEIGHT * face_id);
}

i32 Actor_Spritesheet_Lips_Y(i32 lips_id) {
    // TODO: check for valid index
    return (ACTOR_LIPS_HEIGHT * ACTOR_LIPS_FRAMES * lips_id);
}

i32 Actor_Spritesheet_Eyes_Y(i32 eyes_id) {
    // TODO: check for valid index
    return (ACTOR_EYES_HEIGHT * ACTOR_EYES_FRAMES * eyes_id);
}

void Actor_Update(struct Actor *actor, struct Point *pos, SDL_Texture *render_target,
                  SDL_Renderer *renderer) {
    SDL_assert(actor                != NULL);
    SDL_assert(renderer             != NULL);
    SDL_assert(palette_SOTA         != NULL);

    // Draw a rectangle for every actor
    SDL_Rect dstrect = { 0, 0,
                         SCENE_ACTOR_POS_W,
                         SCENE_ACTOR_POS_H
                       };
    // TODO: get rid of index
    static int index = 0;
    if (actor->texture == NULL) {
        actor->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_TARGET, dstrect.w, dstrect.h);
        SDL_assert(actor->texture != NULL);
        SDL_SetTextureBlendMode(actor->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, actor->texture);

    SDL_Color color = palette_SOTA->colors[SCENE_ACTOR_COLOR_OFFSET + index++];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &dstrect);

    Utilities_DrawColor_Reset(renderer);
    SDL_SetRenderTarget(renderer, NULL);
}

void Actor_Draw(struct Actor *actor, struct Point *pos,
                SDL_Texture *render_target,
                SDL_Renderer *renderer) {
    SDL_assert(actor    != NULL);
    SDL_assert(renderer != NULL);

    if (actor->update) {
        Actor_Update(actor, pos, render_target, renderer);
        actor->update = false;
    }

    /* Copy Actor texture to render_target */
    SDL_SetRenderTarget(renderer, render_target);
    SDL_assert(actor->texture != NULL);
    SDL_Rect dstrect = { pos->x, pos->y,
                         SCENE_ACTOR_POS_W,
                         SCENE_ACTOR_POS_H
                       };

    SDL_RenderCopy(renderer, actor->texture, NULL, &dstrect);
}

