
#include "bars/map_hp.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "palette.h"
#include "macros.h"
#include "utilities.h"

/* --- MAPHPBAR --- */
const struct MapHPBar MapHPBar_default = {
    .update         = true,
};

void MapHPBar_Init(struct MapHPBar *maphpbar) {
    *maphpbar = MapHPBar_default;
}

void MapHPBar_Init_tnecs(void *voidmaphpbar) {
    MapHPBar_Init(voidmaphpbar);
}

void MapHPBar_Draw(struct MapHPBar *mbar,
                   struct Camera *camera,
                   i32 tilesize[TWO_D],
                   SDL_Texture  *render_target,
                   SDL_Renderer *renderer,
                   tnecs_W  *world) {
    SDL_assert(mbar     != NULL);
    SDL_assert(world    != NULL);
    SDL_assert(camera   != NULL);
    SDL_assert(renderer != NULL);

    SDL_Rect dstrect = {
        .x = SOTA_TILEMAP2PIXEL(mbar->tilemap_pos.x, tilesize[0], camera->offset.x, camera->zoom),
        .y = SOTA_TILEMAP2PIXEL((mbar->tilemap_pos.y + 1), tilesize[1], camera->offset.y, camera->zoom),
        .w = mbar->len * camera->zoom,
        .h = MAP_HPBAR_HEIGHT * camera->zoom * MAP_HPBAR_SQUISH_FACTOR
    };
    dstrect.y -= camera->zoom * MAP_HPBAR_HEIGHT * MAP_HPBAR_SQUISH_FACTOR * 2 / 3;

    if (mbar->update)
        MapHPBar_Update(mbar, camera, tilesize, render_target, renderer, world);

    if (mbar->visible)
        SDL_RenderCopy(renderer, mbar->texture, NULL, &dstrect);
}

void MapHPBar_Update(struct MapHPBar *mbar,
                     struct Camera *camera,
                     i32 tilesize[TWO_D],
                     SDL_Texture    *render_target,
                     SDL_Renderer   *renderer,
                     tnecs_W    *world) {
    SDL_assert(mbar->unit_ent > TNECS_NULL);
    mbar->update = false;

    /* -- Getting current HP -- */
    struct Unit *unit = IES_GET_C(world, mbar->unit_ent, Unit);
    SDL_assert(unit != NULL);
    int current_hp = Unit_Current_HP(unit);

    /* -- Computing visibility -- */
    struct Unit_stats effective = Unit_effectiveStats(unit);
    int max_hp = effective.hp;
    mbar->visible = (max_hp != current_hp);

    /* -- Drawing black -- */
    SDL_Rect rect = {0, 0, mbar->len, MAP_HPBAR_HEIGHT};
    if (mbar->texture == NULL) {
        mbar->texture = SDL_CreateTexture(renderer,
                                          SDL_PIXELFORMAT_ARGB8888,
                                          SDL_TEXTUREACCESS_TARGET,
                                          rect.w, rect.h);
        SDL_assert(mbar->texture != NULL);
        SDL_SetTextureBlendMode(mbar->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, mbar->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect); /* faster/slower than SDL_FillRects? */

    /* -- Drawing background -- */
    rect.x++;
    rect.y++;
    rect.w -= 2;
    rect.h -= 2;
    SDL_Color color = palette_SOTA->colors[MAP_HPBAR_BACKGROUND];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect); /* faster/slower than SDL_FillRects? */

    /* -- Set fill color -- */
    if ((int)(max_hp * MAP_HPBAR_THRESHOLD_LOW) >= current_hp) {
        color = palette_SOTA->colors[MAP_HPBAR_RED];
    } else if ((int)(max_hp * MAP_HPBAR_THRESHOLD_MID) <= current_hp) {
        color = palette_SOTA->colors[MAP_HPBAR_GREEN];
    } else {
        color = palette_SOTA->colors[MAP_HPBAR_YELLOW];
    }

    /* -- Drawing fill -- */
    rect.w = current_hp * (mbar->len - 2) / max_hp;
    rect.w = rect.w < 1 ? 1 : rect.w;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rect); /* faster/slower than SDL_FillRects? */

    /* -- Finish -- */
    Utilities_DrawColor_Reset(renderer);
    SDL_SetRenderTarget(renderer, render_target);
}
