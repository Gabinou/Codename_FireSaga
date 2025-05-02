
#include "popup/tile.h"
#include "popup/popup.h"
#include "map/tiles.h"
#include "map/map.h"
#include "tile.h"
#include "position.h"
#include "macros.h"
#include "filesystem.h"
#include "platform.h"
#include "pixelfonts.h"
#include "stb_sprintf.h"

const struct PopUp_Tile PopUp_Tile_default = {
    .update           = true,
    .corner           = SOTA_DIRECTION_BOTLEFT,
    .cursor_limit_min = {-1, -1},
    .cursor_limit_max = {-1, -1},
};

/* --- ructor/Destructor --- */
void PopUp_Tile_Free(struct PopUp_Tile *pt) {
    if (pt->texture_header != NULL) {
        SDL_DestroyTexture(pt->texture_header);
        pt->texture_header = NULL;
    }
}

void PopUp_Tile_Load(struct PopUp_Tile *pt, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    /* - Popup position, scale, n9Patch - */
    PopUp_Tile_Free(pt);
    n9Patch_Free(n9patch);

    *n9patch                  = n9Patch_default;
    n9patch->patch_pixels.x   = PT_PATCH_PIXELS;
    n9patch->patch_pixels.y   = PT_PATCH_PIXELS;
    n9patch->scale.x          = PT_N9PATCH_SCALE_X;
    n9patch->scale.y          = PT_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x    = (PT_PATCH_PIXELS * PT_PATCH_X_SIZE);
    n9patch->size_pixels.y    = (PT_PATCH_PIXELS * PT_PATCH_Y_SIZE);
    n9patch->size_patches.x   = PT_PATCH_X_SIZE;
    n9patch->size_patches.y   = PT_PATCH_Y_SIZE;
    n9patch->pos.x            = 0;
    n9patch->pos.y            = 0;
    SDL_assert(n9patch->patch_pixels.x > 0);
    SDL_assert(n9patch->patch_pixels.y > 0);

    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "tilepopup8px.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    /* - Making Popup_Tile - */
    path = PATH_JOIN("..", "assets", "GUI", "Popup", "PopUp_Tile_Header.png");
    pt->texture_header = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

}

/* --- Positioning --- */
struct Point PopUp_Tile_Offset(struct PopUp_Tile *pt, struct Settings *settings) {
    pt->offset.x = settings->res.x / PT_OFFSET_FACTOR_X;
    pt->offset.y = settings->res.y / PT_OFFSET_FACTOR_Y;
    return (pt->offset);
}

void PopUp_Tile_Limits(struct PopUp *popup, struct PopUp_Tile *pt, struct Settings *settings) {
    pt->cursor_limit_min.x = 0.4f * settings->res.x;
    pt->cursor_limit_max.x = 0.6f * settings->res.x;
}

// Set Popup_Tile_Position depending on where the cursor is on the screen
struct Point PopUp_Tile_Position(struct PopUp *popup, struct PopUp_Tile *pt,
                                 struct n9Patch *n9patch, struct Settings *settings,
                                 struct Point *pixel_pos, i8 direction) {
    // TODO: removed unused direction, popup.

    /* error checking: PopUp_TILE should NOT be on top*/
    switch (pt->corner) {
        case SOTA_DIRECTION_TOPRIGHT:
            pt->corner = SOTA_DIRECTION_BOTRIGHT;
            break;
        case SOTA_DIRECTION_TOPLEFT:
            pt->corner = SOTA_DIRECTION_BOTLEFT;
            break;
    }
    /* check if popup should move corner */
    b32 x_greater = pixel_pos->x >= pt->cursor_limit_max.x;
    b32 x_lower = pixel_pos->x <= pt->cursor_limit_min.x;
    if ((x_greater) && (pt->corner == SOTA_DIRECTION_BOTRIGHT))
        pt->corner = SOTA_DIRECTION_BOTLEFT;
    if ((x_lower) && (pt->corner == SOTA_DIRECTION_BOTLEFT))
        pt->corner = SOTA_DIRECTION_BOTRIGHT;
    /* Check if popup should move corner */
    struct Point out  = {-1, -1};
    struct Point sign = { 1, -1};
    switch (pt->corner) {
        case SOTA_DIRECTION_BOTLEFT:
            out.x = (-2) * n9patch->scale.x;
            break;
        case SOTA_DIRECTION_BOTRIGHT:
            out.x = settings->res.x - (n9patch->size_pixels.x - 1) * n9patch->scale.x;
            sign.x = -1;
            break;
        default:
            SDL_Log("Invalid PopUp_Tile corner value");
    }
    out.y = settings->res.y - n9patch->size_pixels.y * n9patch->scale.y;

    out.x += sign.x * pt->offset.x * n9patch->scale.x;
    out.y += sign.y * pt->offset.y * n9patch->scale.y;
    // if popup->pos is set with this output, sliding is skipped
    return (out);
}

struct Point PopUp_Tile_Center_Name(struct PopUp_Tile *pt, struct n9Patch *n9patch,
                                    char *numbuff, size_t str_len) {
    SDL_assert(numbuff != NULL);
    SDL_assert(pt->pixelnours_big != NULL);
    struct Point out = {PT_NAME_X, PT_NAME_Y};
    size_t len = 0;
    for (size_t i = 0; i < str_len; i++)
        len += pt->pixelnours_big->glyph_bbox_width[numbuff[i]];
    SDL_assert(len < n9patch->size_pixels.x);
    out.x = (n9patch->size_pixels.x - len) / 2 + 2;
    return (out);
}

struct Point PopUp_Tile_Center_Avoid(struct PopUp_Tile *pt, char *numbuff) {
    struct Point out = {PT_AVOID_STAT_X, PT_AVOID_STAT_Y};
    size_t len = 0;
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[0]];
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[1]];
    out.x = (out.x - len / 2);
    return (out);
}

struct Point PopUp_Tile_Center_Heal(struct PopUp_Tile *pt, char *numbuff) {
    struct Point out = {PT_HEAL_STAT_X, PT_HEAL_STAT_Y};
    size_t len = 0;
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[0]];
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[1]];
    out.x = (out.x - len / 2);
    return (out);
}

struct Point PopUp_Tile_Center_Prot(struct PopUp_Tile *pt, char *numbuff) {
    struct Point out = {PT_PROT_STAT_X, PT_PROT_STAT_Y};
    size_t len = 0;
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[0]];
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[1]];
    len += pt->pixelnours_big->glyph_bbox_width[numbuff[2]];
    out.x = (out.x - len / 2);
    return (out);
}

/* --- Setters --- */
void PopUp_Tile_Set(struct PopUp_Tile *popup_tile, struct Game *sota) {
    SDL_assert(popup_tile != NULL);
    const struct Position *cursor_position;
    cursor_position = IES_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

    /* -- Give tile to popup_tile -- */
    int i = cursor_position->tilemap_pos.y * sota->map->col_len + cursor_position->tilemap_pos.x;
    i32 tile_ind = sota->map->grids.tilemap[i] / TILE_DIVISOR;
    SDL_assert(tile_ind > 0);
    i32 tile_order = Map_Tile_Order(sota->map, tile_ind);
    popup_tile->tile = &sota->map->tiles.arr[tile_order];
    popup_tile->update = true;
}

/* --- Drawing --- */
void PopUp_Tile_Draw(struct PopUp *popup, struct Point pos,
                     SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PopUp_Tile *pt = (struct PopUp_Tile *)popup->data;
    struct n9Patch *n9patch = &popup->n9patch;

    if (pt->update) {
        PopUp_Tile_Update(pt, n9patch, render_target, renderer);
        pt->update = false;
    }
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y
    };
    SDL_assert(pt->texture != NULL);
    SDL_RenderCopy(renderer, pt->texture, NULL, &dstrect);
}

void PopUp_Tile_Update(struct PopUp_Tile *pt, struct n9Patch *n9patch,
                       SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(n9patch != NULL);
    SDL_assert(pt != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert(pt->tile != NULL);
    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_Rect dstrect;
    char numbuff[10];
    i16 menu_w = n9patch->size_pixels.x;
    i16 menu_h = (n9patch->size_pixels.y + PT_HEADER_Y) ;
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);
    /* - create render target texture - */
    if (pt->texture == NULL) {
        pt->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(pt->texture != NULL);
        SDL_SetTextureBlendMode(pt->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pt->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_assert(pt->texture != NULL);
    SDL_assert(pt->texture_header != NULL);
    /* --- RENDERING POPUP_TILE --- */
    /* -- PATCHES DRAW -- */
    int scale_x      = n9patch->scale.x;
    int scale_y      = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = (PT_HEADER_H - 3);
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    /* -- HEADER -- */
    dstrect.x = PT_HEADER_X;
    dstrect.y = PT_HEADER_Y;
    dstrect.w = PT_HEADER_W;
    dstrect.h = PT_HEADER_H;
    SDL_RenderCopy(renderer, pt->texture_header, NULL, &dstrect);

    /* -- NAME -- */
    struct Point pos;
    s8 name = Tile_Name(pt->tile);
    SDL_assert(name.data != NULL);
    pos = PopUp_Tile_Center_Name(pt, n9patch, name.data, name.num);
    PixelFont_Write(pt->pixelnours, renderer, name.data, name.num, pos.x, pos.y);

    /* -- TILE STATS -- */
    SDL_assert(pt->pixelnours != NULL);
    SDL_assert(pt->pixelnours_big != NULL);

    /* - prot - */
    PixelFont_Write(pt->pixelnours, renderer, "DEF", 3, PT_PROT_X, PT_PROT_Y);
    stbsp_sprintf(numbuff, "%01d/%01d\0\0\0\0", pt->tile->stats.Pprot, pt->tile->stats.Mprot);
    pos = PopUp_Tile_Center_Prot(pt, numbuff);
    PixelFont_Write(pt->pixelnours_big, renderer, numbuff, 3, pos.x, pos.y);

    /* - avoid - */
    PixelFont_Write(pt->pixelnours, renderer, "AVOID", 5, PT_AVOID_X, PT_AVOID_Y);
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", pt->tile->stats.dodge);
    pos = PopUp_Tile_Center_Avoid(pt, numbuff);
    PixelFont_Write(pt->pixelnours_big, renderer, numbuff, 2, pos.x, pos.y);

    /* - heal- */
    PixelFont_Write(pt->pixelnours, renderer, "HEAL", 4, PT_HEAL_X, PT_HEAL_Y);
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", pt->tile->stats.heal);
    pos = PopUp_Tile_Center_Heal(pt, numbuff);
    PixelFont_Write(pt->pixelnours_big, renderer, numbuff, 2, pos.x, pos.y);

    SDL_SetRenderTarget(renderer, render_target);
}
