
#include "map/render.h"
#include "map/map.h"
#include "map/path.h"
#include "map/tiles.h"
#include "index_shader.h"
#include "sprite.h"
#include "pathfinding.h"
#include "macros.h"
#include "arrow.h"
#include "nmath.h"
#include "palette.h"
#include "utilities.h"

void Map_Units_Hide(struct Map *map) {
    if (map->unitmap == NULL)
        return;

    for (size_t i = 0; i < (map->col_len * map->row_len); i++) {
        tnecs_entity uent = map->unitmap[i];
        if (uent == TNECS_NULL)
            continue;
        struct Sprite *sprite = IES_GET_COMPONENT(map->world, uent, Sprite);
        if (sprite != NULL)
            sprite->visible = false;
    }
}

void Map_Frame_Pauses(struct Map *map,  struct Settings *settings) {
    // TODO: input Map_Render only
    SDL_assert(map);
    SDL_assert(map->render.tilemap_frame_factor > 0.0f);
    SDL_assert(map->render.tilemap_frame_factor < 1.0f);
    SDL_assert(map->render.shadow_frame_factor  > 0.0f);
    SDL_assert(map->render.shadow_frame_factor  < 1.0f);
    map->render.tilemap_frame_pause = (u8)(map->render.tilemap_frame_factor * settings->FPS.cap);
    map->render.shadow_frame_pause  = (u8)(map->render.shadow_frame_factor  * settings->FPS.cap);
}

void Map_Palettemap_Reset(struct Map *map) {
    SDL_assert(map->row_len < MAP_MAX_ROWS);
    SDL_assert(map->col_len < MAP_MAX_COLS);
    size_t bytesize = map->col_len * map->row_len * sizeof(*map->palette.map);
    SDL_assert(map->palette.map != NULL);
    memset(map->palette.map, map->palette.base, bytesize);
}

void Map_Palettemap_addMap(struct Map *map, i32 *palettemap, u8 palette) {
    SDL_assert(palette >= PALETTE_NES);
    SDL_assert(palette  < PALETTE_NUM);
    for (size_t i = 0; i < (map->col_len * map->row_len); i++)
        (palettemap[i] > 0) && (map->palette.map[i] = palette); /* short-circuit */
}

void Map_Palettemap_addList(struct Map *map, i32 *list, u8 palette) {
    size_t bytesize = map->row_len * map->col_len * sizeof(*map->palette.map);
    memset(map->palette.map, map->palette.base, bytesize);
    for (size_t i = 0; i < DARR_NUM(list) / TWO_D; i++)
        map->palette.map[list[TWO_D * i] * map->col_len + list[TWO_D * i + 1]] = palette;
}

void Map_Renderer_Set(struct Map *map, SDL_Renderer *renderer) {
    SDL_assert(map);
    if (renderer != NULL) {
        map->renderer = renderer;
    }
}

void Map_Palettemap_Autoset(struct Map *map, u16 flagsum, tnecs_entity self) {
    Map_Palettemap_Reset(map);
    int size        = map->row_len * map->col_len;
    i32 *palette    = map->palette.temp;
    size_t bytesize = map->col_len * map->row_len * sizeof(*map->palette.temp);

    /* Last set Map_Palettemap_addMap is rendered */
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_GLOBAL_DANGER, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->global_dangermap, 0, size);
        Map_Palettemap_addMap(map, palette, map->palette.purple);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_DANGER, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->dangermap, 0, size);
        i32 *temp_palette2 = matrix_ssmaller(map->dangermap, DANGERMAP_UNIT_DIVISOR, size);
        palette = matrix_and_noM(palette, temp_palette2, palette, size);
        SDL_free(temp_palette2);
        Map_Palettemap_addMap(map, palette, map->palette.darkred);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_ATTACK, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->attacktomap, 0, size);
        Map_Palettemap_addMap(map, palette, map->palette.red);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_HEAL, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->healtomap, 0, size);

        Map_Palettemap_addMap(map, palette, map->palette.green);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_MOVE, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->movemap, 0, size);

        // DON'T show move if can heal AND move to an occupied tile
        // EXCEPT if self sota->hovered_unit_entity
        for (size_t i = 0; i < size; i++) {
            if (map->healtomap[i] && map->unitmap[i] && (self != TNECS_NULL) && (map->unitmap[i] != self)) {
                palette[i] = 0;
            }
        }

        Map_Palettemap_addMap(map, palette, map->palette.blue);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_START_POS, flagsum)) {
        SDL_assert(palette);
        SDL_assert(map->start_pos);
        palette = matrix_sgreater_noM(palette, map->start_posmap, 0, size);
        Map_Palettemap_addMap(map, palette, map->palette.purple);
    }
    memset(palette, 0, bytesize);
}

void Map_Palettemap_SetwMap(struct Map *map, u8 *palettemap) {
    size_t bytesize = map->row_len * map->col_len * sizeof(*map->palette.map);
    SDL_assert(map->palette.map != NULL);
    memcpy(map->palette.map, palettemap, bytesize);
}

SDL_Texture *Map_Tilemap_Texture_Stitch(struct Map *map, struct SDL_Texture *render_target) {
    /* Preliminaries */
    SDL_Rect dstrect = {0, 0, map->tilesize[0], map->tilesize[1]};
    SDL_Rect srcrect = {0, 0, map->tilesize[0], map->tilesize[1]};
    i32 tile_ind = 0, tile_order = 0, texture_ind = 0;
    u8 palette_ind = 0;
    int success;
    SDL_assert(map->renderer);
    if (map->tilemap_texture == NULL)
        Map_Tilemap_Texture_Init(map);
    SDL_SetRenderTarget(map->renderer, map->tilemap_texture);
    SDL_assert(map->tilemap);
    SDL_assert(map->palette.map);

    /* Stitching map from tiles */
    SDL_assert(map->render.visiblemax.x <= map->col_len);
    SDL_assert(map->render.visiblemin.x >= 0);
    SDL_assert(map->render.visiblemax.y < map->row_len);
    SDL_assert(map->render.visiblemin.y >= 0);
    int visible_min = map->render.visiblemin.y * map->col_len + map->render.visiblemin.x;
    int visible_max = map->render.visiblemax.y * map->col_len + map->render.visiblemax.x;
    for (size_t i = visible_min; i < visible_max; i++) {
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        texture_ind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        palette_ind = map->palette.map[i];
        SDL_assert(palette_ind >= PALETTE_NES);
        SDL_assert(palette_ind < PALETTE_NUM);

        /* tile position in tileset */
        srcrect.x = sota_ss_x(texture_ind, TILESET_COL_LEN) * map->tilesize[0];
        srcrect.y = sota_ss_y(texture_ind, TILESET_COL_LEN) * map->tilesize[1];

        /* tile position in tilemap */
        dstrect.x = sota_ss_x(i, map->col_len) * map->tilesize[0];
        dstrect.y = sota_ss_y(i, map->col_len) * map->tilesize[1];

        /* Get tile texture to stitch map into */
        SDL_Texture *texture = map->tileset_textures[palette_ind][tile_order];
        SDL_assert(texture != NULL);
        SDL_assert(tile_ind > 0);
        tile_order = Map_Tile_Order(map, tile_ind);
        if (map->tileset_textures[palette_ind][tile_order] == NULL)
            Map_Tileset_newPalette(map, tile_order, palette_ind);

        /* Stitching map */
        // Filesystem_Texture_Dump("dump.png", map->renderer, texture, SDL_PIXELFORMAT_ARGB8888, NULL);
        success = SDL_RenderCopy(map->renderer, texture, &srcrect, &dstrect);
        /* Stitch icon depending on stack mode */
        if (!success) {
            SDL_Log("SDL_Error: %s", SDL_GetError());
            SDL_assert(false);
        }
        if ((!map->stack.mode) || (!map->flags.show_icons))
            continue;

        tile_order = Map_Tile_Order(map, TILE_ICONS);
        texture = map->tileset_textures[map->palette.base][tile_order];
        srcrect.y = 0;
        switch (map->stack.mode) {
            case MAP_SETTING_STACK_DANGERMAP:
                if (map->stack.dangermap[i] == true) {
                    srcrect.x = (TILE_ICON_DANGER % TILESET_COL_LEN) * map->tilesize[0];
                    success = SDL_RenderCopy(map->renderer, texture, &srcrect, &dstrect);
                    SDL_assert(success);
                }
                break;
        }
    }
    SDL_SetRenderTarget(map->renderer, render_target); /* render to default again */
    return (map->tilemap_texture);
}
void Map_Visible_Tiles(struct Map *map,  struct Settings *settings, struct Camera *camera) {
    struct Point visiblemprev = map->render.visiblemin;
    struct Point visiblemax_prev = map->render.visiblemax;
    map->render.visiblemin.x = camera->offset.x > 0 ? 0 : SOTA_DEZOOM(
                                       -1 * camera->offset.x / map->tilesize[0], camera->zoom);
    map->render.visiblemin.y = camera->offset.y > 0 ? 0 : SOTA_DEZOOM(
                                       -1 * camera->offset.y / map->tilesize[1], camera->zoom);

    map->render.visiblemax.x = SOTA_DEZOOM((settings->res.x - camera->offset.x) / map->tilesize[0],
                                           camera->zoom);
    if (map->render.visiblemax.x < map->col_len)
        map->render.visiblemax.x++;
    if (map->render.visiblemax.x > map->col_len)
        map->render.visiblemax.x = map->col_len;

    map->render.visiblemax.y = SOTA_DEZOOM((settings->res.y - camera->offset.y) / map->tilesize[1],
                                           camera->zoom);
    if (map->render.visiblemax.y < map->row_len)
        map->render.visiblemax.y++;
    if (map->render.visiblemax.y >= map->row_len)
        map->render.visiblemax.y = map->row_len - 1;

    map->flags.visible_changed =  (visiblemprev.x != map->render.visiblemin.x);
    map->flags.visible_changed |= (visiblemprev.y != map->render.visiblemin.y);
    map->flags.visible_changed |= (visiblemax_prev.x != map->render.visiblemax.x);
    map->flags.visible_changed |= (visiblemax_prev.y != map->render.visiblemax.y);
}

SDL_Surface *Map_Tilemap_Surface_Stitch(struct Map *map) {
    SDL_assert(map->tilemap_surface != NULL);
    SDL_assert(map->tilemap_surface->w == (map->col_len * map->tilesize[0]));
    SDL_assert(map->tilemap_surface->h == (map->row_len * map->tilesize[1]));
    SDL_assert(map->tilemap          != NULL);
    SDL_assert(map->palette.map       != NULL);
    SDL_assert(map->tileset_surfaces != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(map->tilemap_surface->format->format));

    /* -- Preliminaries -- */
    SDL_Rect dstrect    = {0, 0, map->tilesize[0], map->tilesize[1]};
    SDL_Rect srcrect    = {0, 0, map->tilesize[0], map->tilesize[1]};
    i32 tile_ind        = 0;
    i32 tile_order      = 0;
    i32 texture_ind     = 0;
    u8 palette_ind      = 0;
    int success;

    /* Stitching map from tiles */
    SDL_assert(map->render.visiblemax.x <= map->col_len);
    SDL_assert(map->render.visiblemin.x >= 0);
    SDL_assert(map->render.visiblemax.y <  map->row_len);
    SDL_assert(map->render.visiblemin.y >= 0);
    int visible_min = map->render.visiblemin.y * map->col_len + map->render.visiblemin.x;
    int visible_max = map->render.visiblemax.y * map->col_len + map->render.visiblemax.x;
    for (size_t i = visible_min; i < visible_max; i++) {
        /* get palette */
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        texture_ind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        palette_ind = map->palette.map[i];
        SDL_assert(palette_ind >= PALETTE_NES);
        SDL_assert(palette_ind < PALETTE_NUM);

        /* tile position in tileset */
        srcrect.x = (texture_ind % TILESET_COL_LEN) * map->tilesize[0];
        srcrect.y = (texture_ind / TILESET_COL_LEN) * map->tilesize[1];

        /* tile position in tilemap */
        dstrect.x = (i % map->col_len) * map->tilesize[0];
        dstrect.y = (i / map->col_len) * map->tilesize[1];
        SDL_assert(tile_ind > 0);
        tile_order = Map_Tile_Order(map, tile_ind);

        /* Get tile surface to stitch map into */
        SDL_assert(map->tileset_surfaces != NULL);
        SDL_assert(map->tileset_surfaces[palette_ind] != NULL);
        SDL_Surface *surf = map->tileset_surfaces[palette_ind][tile_order];
        SDL_assert(surf != NULL);
        if (surf == NULL)
            Map_Tileset_newPalette(map, tile_order, palette_ind);
        SDL_assert(surf);
        SDL_assert(surf->format != NULL);
        SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));

        /* Stitching map */
        success = SDL_BlitSurface(surf, &srcrect, map->tilemap_surface, &dstrect);
        /* SDL_BlitSurface WEIRDNESS:
            ignores dstrect -> DOCUMENTED in https://wiki.libsdl.org/SDL_BlitSurface
        */

        /* Stitch icon depending on stack mode */
        SDL_assert(success == 0);
        if ((!map->stack.mode) || (!map->flags.show_icons))
            continue;

        tile_order = Map_Tile_Order(map, TILE_ICONS);
        surf = map->tileset_surfaces[map->palette.base][tile_order];
        srcrect.y = 0;
        switch (map->stack.mode) {
            case MAP_SETTING_STACK_DANGERMAP:
                if (map->stack.dangermap[i]) {
                    srcrect.x = (TILE_ICON_DANGER % TILESET_COL_LEN) * map->tilesize[0];
                    success = SDL_BlitSurface(surf, &srcrect, map->tilemap_surface, &dstrect);
                    SDL_assert(success == 0);
                }
                break;
        }
    }
    return (map->tilemap_surface);
}

void Map_swappedTextures_All(struct Map *map) {
    SDL_assert(map->tilesindex != NULL);
    for (size_t palette_id = PALETTE_START + 1; palette_id < PALETTE_NUM; palette_id++) {
        for (size_t tile_order = 0; tile_order < DARR_NUM(map->tiles); tile_order++)
            Map_Tileset_newPalette(map, map->tilesindex[tile_order], palette_id);
    }
}

/* Indices of tiles visible to player depending on zoom level */
/* min is inclusive, max is exclusive */
void Map_Visible_Bounds(u8 *min, u8 *max, size_t row_len, size_t col_len,
                        i32 tilesize[TWO_D],  struct Point *res, struct Camera *camera) {
    i32 edge_min[TWO_D], edge_max[TWO_D];
    edge_max[0] = SOTA_PIXEL2TILEMAP(res->x, tilesize[0], camera->offset.x, camera->zoom) + 2;
    edge_max[1] = SOTA_PIXEL2TILEMAP(res->y, tilesize[1], camera->offset.y, camera->zoom) + 2;
    edge_min[0] = SOTA_PIXEL2TILEMAP(0,      tilesize[0], camera->offset.x, camera->zoom) - 1;
    edge_min[1] = SOTA_PIXEL2TILEMAP(0,      tilesize[1], camera->offset.y, camera->zoom) - 1;
    max[0]      = (edge_max[0] > col_len) ? col_len : edge_max[0];
    max[1]      = (edge_max[1] > row_len) ? row_len : edge_max[1];
    min[0]      = (edge_min[0] <    0   ) ?    0    : edge_min[0];
    min[1]      = (edge_min[1] <    1   ) ?    0    : edge_min[1];
}

void _Map_Perimeter_Draw(struct Map *map, struct Settings *settings,
                         struct Camera *camera, i32 *insidemap,
                         SDL_Color color, struct Padding *edges) {
    SDL_assert(map          != NULL);
    SDL_assert(settings     != NULL);
    SDL_assert(camera       != NULL);
    SDL_assert(insidemap    != NULL);

    int thick = settings->map_settings.perim_thickness;

    int success = SDL_SetRenderDrawColor(map->renderer, color.r, color.g, color.b, color.a);
    SDL_assert(success == 0);

    size_t row_len = map->row_len, col_len = map->col_len;
    /* -- Draw Lines -- */
    for (i32 i = 0; i < row_len * col_len; i++) {

        /* - Skip if tile is outside - */
        if (insidemap[i] == 0)
            continue;

        i32 x = i % col_len;
        i32 y = i / col_len;

        for (i32 ii = 0; ii < SQUARE_NEIGHBOURS; ii++) {
            /* ii = 0: SOTA_PADDING_RIGHT   +x  */
            /* ii = 1: SOTA_PADDING_TOP     -y  */
            /* ii = 2: SOTA_PADDING_LEFT    -x  */
            /* ii = 3: SOTA_PADDING_BOTTOM  +y  */

            // Note: top left is origin. m in cycle ignored
            i32 *pad_arr = (i32 *)&edges[i];

            /* - Skip if not perimeter - */
            if (!pad_arr[ii])
                continue;

            i32 plusx_1 = q_cycle4_pmmp(ii) > 0;
            i32 plusy_1 = q_cycle4_pmmp(ii) > 0;

            i32 plusx_2 = q_cycle4_ppmm(ii) > 0;
            i32 plusy_2 = q_cycle4_mmpp(ii) > 0;

            i32 x_1 = x + plusx_1;
            i32 y_1 = y + plusy_1;

            i32 x_2 = x + plusx_2;
            i32 y_2 = y + plusy_2;

            i32 line1_x = SOTA_ZOOM(map->tilesize[0] * x_1, camera->zoom) + camera->offset.x;
            i32 line1_y = SOTA_ZOOM(map->tilesize[1] * y_1, camera->zoom) + camera->offset.y;

            i32 line2_x = SOTA_ZOOM(map->tilesize[0] * x_2, camera->zoom) + camera->offset.x;
            i32 line2_y = SOTA_ZOOM(map->tilesize[1] * y_2, camera->zoom) + camera->offset.y;

            /* - Skip if line not on screen - */
            if ((line1_x <= 0) || (line1_x > settings->res.x))
                continue;

            if ((line1_y <= 0) || (line1_y > settings->res.y))
                continue;

            /* - Render the thickness - */
            i32 thickx = q_cycle2_pz(ii);
            i32 thicky = q_cycle2_zp(ii);
            for (int t = -(thick / 2); t < thick; t++) {
                SDL_RenderDrawLine(map->renderer, line1_x + thickx * t,
                                   line1_y + thicky * t,
                                   line2_x + thickx * t,
                                   line2_y + thicky * t);
            }
        }
    }
}

void Map_Perimeter_Draw_Danger(struct Map *map, struct Settings *settings, struct Camera *camera) {
    if (map->rendered_dangermap == NULL)
        return;

    SDL_Palette *palette_base = sota_palettes[map->palette.base];
    SDL_Color red = palette_base->colors[map->perimiter.danger_color];

    _Map_Perimeter_Draw(map, settings, camera, map->rendered_dangermap, red,
                        map->perimiter.edges_danger);
}

void Map_Perimeter_Draw_Aura(struct Map     *map,    struct Settings *settings,
                             struct Camera  *camera, struct Point pos,
                             struct Range    range,  int colori) {
    i32 include = range.min == 0 ? MOVETILE_INCLUDE : MOVETILE_EXCLUDE;
    i32 len = map->row_len * map->col_len;
    i32 *temp = alloca(len);
    memset(temp, 0, len * sizeof(*temp));

    PathfindingAct actto    = PathfindingAct_default;
    actto.acttomap          = temp;
    actto.movemap           = NULL;
    actto.occupymap         = NULL;
    actto.row_len           = map->row_len;
    actto.col_len           = map->col_len;
    actto.point             = pos;
    actto.range             = range;
    actto.mode_movetile     = include;

    _Pathfinding_Attackto(actto);

    Map_Perimeter(map->perimiter.edges_danger, temp, map->row_len, map->col_len);

    SDL_Palette *palette_base = sota_palettes[map->palette.base];
    SDL_Color purple = palette_base->colors[colori];

    _Map_Perimeter_Draw(map, settings, camera, temp, purple, map->perimiter.edges_danger);
}


void Map_Grid_Draw(struct Map *map,  struct Settings *settings, struct Camera *camera) {
    /* -- Preliminaries -- */
    SDL_assert(map      != NULL);
    SDL_assert(camera   != NULL);
    SDL_assert(settings != NULL);

    if (!settings->map_settings.grid_show)
        return;

    int thick = settings->map_settings.grid_thickness;
    SDL_assert(thick > 0);
    struct Point line;
    int success, edge1, edge2;
    SDL_Color map_bg = settings->map_settings.color_grid;
    success = SDL_SetRenderDrawColor(map->renderer, map_bg.r, map_bg.g, map_bg.b, map_bg.a);
    SDL_assert(success == 0);

    /* -- Draw Lines -- */
    // Note: edge grids are not drawn
    /* - Draw Vertical lines - */
    for (size_t col = 1; col < map->col_len; col++) {
        line.x = SOTA_ZOOM(map->tilesize[0] * col, camera->zoom) + camera->offset.x;
        /* if line.x is outside the screen, skip it */
        if ((line.x <= 0) || (line.x > settings->res.x))
            continue;

        line.y = SOTA_ZOOM(map->tilesize[1] * map->row_len, camera->zoom) + camera->offset.y;
        edge1 = camera->offset.y < 0 ? 0 : camera->offset.y;
        edge2 = line.y > settings->res.y ? settings->res.y : line.y;
        for (int t = -(thick / 2); t < thick; t++) {
            SDL_RenderDrawLine(map->renderer, line.x + t, edge1, line.x + t, edge2);
        }
    }

    /* - Draw Horizontal lines - */
    for (size_t row = 1; row < map->row_len; row++) {
        line.y = SOTA_ZOOM(map->tilesize[1] * row, camera->zoom) + camera->offset.y;
        /* if line.y is outside the screen, draw it */
        if ((line.y <= 0) || (line.y > settings->res.y))
            continue;

        line.x = SOTA_ZOOM(map->tilesize[0] * map->col_len, camera->zoom) + camera->offset.x;
        edge1 = camera->offset.x < 0 ? 0 : camera->offset.x;
        edge2 = line.x > settings->res.x ? settings->res.x : line.x;
        for (int t = -(thick / 2); t < thick; t++) {
            SDL_RenderDrawLine(map->renderer, edge1, line.y + t, edge2, line.y + t);
        }
    }
    Utilities_DrawColor_Reset(map->renderer);
}

b32 Map_Shadowmap_newFrame(struct Map *map) {
    b32 shadow_updates = false;
#if !defined(MAP_NO_ANIMATION) || !defined(MAP_SHADOW_NO_ANIMATION)
    if (map->shadow_frame_counter++ > map->shadow_frame_pause) {
        shadow_updates = true;
        map->shadow_frame_counter = 0;
    }
#endif /* !defined(MAP_NO_ANIMATION) || !defined(MAP_SHADOW_NO_ANIMATION) */
    return (shadow_updates);
}

b32 Map_Tilemap_newFrame(struct Map *map) {
    b32 tm_up = false ;
#if !defined(MAP_NO_ANIMATION) || !defined(MAP_TILEMAP_NO_ANIMATION)
    if (map->tilemap_frame_counter++ > map->tilemap_frame_pause) {
        tm_up = true;
        map->tilemap_frame_counter = 0;
    }
#endif /* !defined(MAP_NO_ANIMATION) || !defined(MAP_TILEMAP_NO_ANIMATION) */
    return (tm_up);
}

void Map_Update(struct Map *map,  struct Settings *settings,
                struct Camera *camera, struct SDL_Texture *render_target) {
    SDL_assert(map->tilemap_texture);
    if ((map->render.tilemap_shader != NULL)) {
        Map_Tilemap_Surface_Stitch(map);
        SDL_assert(map->tilemap_surface);
        if (map->render.tilemap_shader->shadow_tilemaps != NULL) {
            map->tilemap_surface = Tilemap_Shade_Surface(map->render.tilemap_shader,
                                                         map->tilemap_surface, 0, settings, camera);
        }
        SDL_assert(map->tilemap_surface);
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = SDL_CreateTextureFromSurface(map->renderer, map->tilemap_surface);
    } else {
        SDL_assert(map->tilemap_texture);
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = NULL;
        // BROKEN
        Map_Tilemap_Texture_Stitch(map, render_target);
    }
    map->flags.camera_moved       = false;
    map->flags.shading_changed    = false;
    map->flags.update             = false;
}

void Map_Draw(struct Map *map,  struct Settings *settings,
              struct Camera *camera, struct SDL_Texture *render_target) {
    SDL_assert(map->renderer != NULL);

    /* -- Preliminaries -- */
    SDL_Color map_bg = settings->map_settings.color_grid;
    SDL_SetRenderDrawColor(map->renderer, map_bg.r, map_bg.g, map_bg.b, map_bg.a);
    SDL_Rect dstrect = {
        .x = camera->offset.x,
        .y = camera->offset.y,
        .w = SOTA_ZOOM(map->col_len * map->tilesize[0], camera->zoom),
        .h = SOTA_ZOOM(map->row_len * map->tilesize[1], camera->zoom)
    };

    /* -- Map update switches -- */
    b32 sm_up = Map_Shadowmap_newFrame(map);
    b32 tm_up = Map_Tilemap_newFrame(map);
    if (map->flags.update || map->flags.camera_moved)
        Map_Visible_Tiles(map, settings, camera);
    else
        map->flags.visible_changed = false;

    /* -- Update Map -- */
    if (map->flags.update || sm_up || tm_up || map->flags.visible_changed || map->flags.shading_changed)
        Map_Update(map, settings, camera, render_target);

    SDL_assert(map->tilemap_texture != NULL);
    SDL_RenderCopy(map->renderer, map->tilemap_texture, NULL, &dstrect); /* slow */

    /* -- Draw Arrow -- */
    if (map->arrow && map->arrow->show) {
        SDL_assert(map->arrow->textures != NULL);
        Arrow_Draw(map->arrow, map->renderer, camera);
    }

    Utilities_DrawColor_Reset(map->renderer);
}

