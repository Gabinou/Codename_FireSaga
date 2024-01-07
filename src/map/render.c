
#include "map/render.h"

void Map_Frame_Pauses(struct Map *map,  struct Settings *settings) {
    SDL_assert(map);
    SDL_assert(map->tilemap_frame_factor > 0.0f);
    SDL_assert(map->tilemap_frame_factor < 1.0f);
    SDL_assert(map->shadow_frame_factor  > 0.0f);
    SDL_assert(map->shadow_frame_factor  < 1.0f);
    map->tilemap_frame_pause = (u8)(map->tilemap_frame_factor * settings->FPS.cap);
    map->shadow_frame_pause  = (u8)(map->shadow_frame_factor  * settings->FPS.cap);
}

void Map_Palettemap_Reset(struct Map *map) {
    SDL_assert(map->row_len > 0);
    SDL_assert(map->col_len > 0);
    size_t bytesize = map->col_len * map->row_len * sizeof(*map->palettemap);
    SDL_assert(map->palettemap != NULL);
    memset(map->palettemap, PALETTE_NES, bytesize);
}

void Map_Palettemap_addMap(struct Map *map, i32 *palettemap, u8 palette) {
    SDL_assert(palette >= PALETTE_NES);
    SDL_assert(palette  < PALETTE_NUM);
    for (size_t i = 0; i < (map->col_len * map->row_len); i++)
        (palettemap[i] > 0) && (map->palettemap[i] = palette); /* short-circuit */
}

void Map_Palettemap_addList(struct Map *map, i32 *list, u8 palette) {
    size_t bytesize = map->row_len * map->col_len * sizeof(*map->palettemap);
    memset(map->palettemap, PALETTE_NES, bytesize);
    for (size_t i = 0; i < DARR_NUM(list) / TWO_D; i++)
        map->palettemap[list[TWO_D * i] * map->col_len + list[TWO_D * i + 1]] = palette;
}

void Map_Renderer_Set(struct Map *map, SDL_Renderer *renderer) {
    SDL_assert(renderer);
    map->renderer = renderer;
    if (map->arrow != NULL)
        Arrow_Textures_Load(map->arrow, ARROW_FILENAME, map->renderer);
}

void Map_Palettemap_Autoset(struct Map *map, u16 flagsum) {
    Map_Palettemap_Reset(map);
    int size = map->row_len * map->col_len;
    i32 *palette = map->temp_palette;
    size_t bytesize = map->col_len * map->row_len * sizeof(*map->temp_palette);

    /* Last set Map_Palettemap_addMap is rendered */
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_GLOBAL_DANGER, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->global_dangermap, 0, size);
        Map_Palettemap_addMap(map, palette, PALETTE_NES_FILTER_PURPLE);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_ATTACK, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->attacktomap, 0, size);
        Map_Palettemap_addMap(map, palette, PALETTE_NES_FILTER_RED);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_DANGER, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->dangermap, 0, size);
        i32 *temp_palette2 = matrix_ssmaller(map->dangermap, DANGERMAP_UNIT_DIVISOR, size);
        palette = matrix_and_noM(palette, temp_palette2, palette, size);
        SDL_free(temp_palette2);
        Map_Palettemap_addMap(map, palette, PALETTE_NES_FILTER_DARKREDwSHADOW);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_HEAL, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->healtomap, 0, size);
        Map_Palettemap_addMap(map, palette, PALETTE_NES_FILTER_GREEN);
    }
    memset(palette, 0, bytesize);
    if (flagsum_isIn(MAP_OVERLAY_MOVE, flagsum)) {
        SDL_assert(palette);
        palette = matrix_sgreater_noM(palette, map->movemap, 0, size);
        Map_Palettemap_addMap(map, palette, PALETTE_NES_FILTER_BLUE);
    }
    memset(palette, 0, bytesize);

}

void Map_Palettemap_SetwMap(struct Map *map, u8 *palettemap) {
    size_t bytesize = map->row_len * map->col_len * sizeof(*map->palettemap);
    SDL_assert(map->palettemap != NULL);
    memcpy(map->palettemap, palettemap, bytesize);
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
    SDL_assert(map->palettemap);

    /* Stitching map from tiles */
    SDL_assert(map->visiblemax.x <= map->col_len);
    SDL_assert(map->visiblemin.x >= 0);
    SDL_assert(map->visiblemax.y < map->row_len);
    SDL_assert(map->visiblemin.y >= 0);
    int visible_min = map->visiblemin.y * map->col_len + map->visiblemin.x;
    int visible_max = map->visiblemax.y * map->col_len + map->visiblemax.x;
    for (size_t i = visible_min; i < visible_max; i++) {
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        texture_ind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        palette_ind = map->palettemap[i];
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
        SDL_assert(tile_ind > 0);
        tile_order = Map_Tile_Order(map, tile_ind);
        if (map->tileset_textures[palette_ind][tile_order] == NULL)
            Map_Tileset_newPalette(map, tile_order, palette_ind);

        /* Stitching map */
        success = SDL_RenderCopy(map->renderer, texture, &srcrect, &dstrect);

        /* Stitch icon depending on stack mode */
        SDL_assert(success);
        if ((!map->stack_mode) || (!map->show_icons))
            continue;

        tile_order = Map_Tile_Order(map, TILE_ICONS);
        texture = map->tileset_textures[PALETTE_NES][tile_order];
        srcrect.y = 0;
        switch (map->stack_mode) {
            case MAP_SETTING_STACK_DANGERMAP:
                if (map->stacked_dangermap[i] == true) {
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
    struct Point visiblemprev = map->visiblemin;
    struct Point visiblemax_prev = map->visiblemax;
    map->visiblemin.x = camera->offset.x > 0 ? 0 : SOTA_DEZOOM(
                                -1 * camera->offset.x / map->tilesize[0], camera->zoom);
    map->visiblemin.y = camera->offset.y > 0 ? 0 : SOTA_DEZOOM(
                                -1 * camera->offset.y / map->tilesize[1], camera->zoom);

    map->visiblemax.x = SOTA_DEZOOM((settings->res.x - camera->offset.x) / map->tilesize[0],
                                    camera->zoom);
    if (map->visiblemax.x < map->col_len)
        map->visiblemax.x++;
    if (map->visiblemax.x > map->col_len)
        map->visiblemax.x = map->col_len;

    map->visiblemax.y = SOTA_DEZOOM((settings->res.y - camera->offset.y) / map->tilesize[1],
                                    camera->zoom);
    if (map->visiblemax.y < map->row_len)
        map->visiblemax.y++;
    if (map->visiblemax.y >= map->row_len)
        map->visiblemax.y = map->row_len - 1;

    map->visible_changed = (visiblemprev.x != map->visiblemin.x);
    map->visible_changed |= (visiblemprev.y != map->visiblemin.y);
    map->visible_changed |= (visiblemax_prev.x != map->visiblemax.x);
    map->visible_changed |= (visiblemax_prev.y != map->visiblemax.y);
}

SDL_Surface *Map_Tilemap_Surface_Stitch(struct Map *map) {
    SDL_assert(map->tilemap_surface != NULL);
    SDL_assert(map->tilemap_surface->w == (map->col_len * map->tilesize[0]));
    SDL_assert(map->tilemap_surface->h == (map->row_len * map->tilesize[1]));
    SDL_assert(map->tilemap          != NULL);
    SDL_assert(map->palettemap       != NULL);
    SDL_assert(map->tileset_surfaces != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(map->tilemap_surface->format->format));

    /* -- Preliminaries -- */
    SDL_Rect dstrect = {0, 0, map->tilesize[0], map->tilesize[1]};
    SDL_Rect srcrect = {0, 0, map->tilesize[0], map->tilesize[1]};
    i32 tile_ind     = 0;
    i32 tile_order   = 0;
    i32 texture_ind  = 0;
    u8 palette_ind  = 0;
    int success;

    /* Stitching map from tiles */
    SDL_assert(map->visiblemax.x <= map->col_len);
    SDL_assert(map->visiblemin.x >= 0);
    SDL_assert(map->visiblemax.y <  map->row_len);
    SDL_assert(map->visiblemin.y >= 0);
    int visible_min = map->visiblemin.y * map->col_len + map->visiblemin.x;
    int visible_max = map->visiblemax.y * map->col_len + map->visiblemax.x;
    for (size_t i = visible_min; i < visible_max; i++) {
        /* get palette */
        tile_ind = map->tilemap[i] / TILE_DIVISOR;
        texture_ind = map->tilemap[i] - tile_ind * TILE_DIVISOR;
        palette_ind = map->palettemap[i];
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
        if ((!map->stack_mode) || (!map->show_icons))
            continue;

        tile_order = Map_Tile_Order(map, TILE_ICONS);
        surf = map->tileset_surfaces[PALETTE_NES][tile_order];
        srcrect.y = 0;
        switch (map->stack_mode) {
            case MAP_SETTING_STACK_DANGERMAP:
                if (map->stacked_dangermap[i]) {
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

void Map_Danger_Perimeter_Draw(struct Map *map, struct Settings    *s, struct Camera *c) {
    int thick = settings->map_settings.perim_thickness;
    i32 outside = 0;
    
    /* -- Draw Lines -- */
    for (i32 i = 0; i < row_len * col_len; i++) {
      
        /* - Skip if tile is outside - */
        if (map->stacked_dangermap[i] == outside) 
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
            
                 
         //   for (int t = -(thick / 2); t < thick; t++) {
            
       //     }
        }
    }
}

void Map_Grid_Draw(struct Map *map,  struct Settings *settings, struct Camera *camera) {
    /* -- Preliminaries -- */
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

bool Map_Shadowmap_newFrame(struct Map *map) {
    bool shadow_updates = false;
    #if !defined(MAP_NO_ANIMATION) || !defined(MAP_SHADOW_NO_ANIMATION)
    if (map->shadow_frame_counter++ > map->shadow_frame_pause) {
        shadow_updates = true;
        map->shadow_frame_counter = 0;
    }
    #endif /* !defined(MAP_NO_ANIMATION) || !defined(MAP_SHADOW_NO_ANIMATION) */
    return (shadow_updates);
}

bool Map_Tilemap_newFrame(struct Map *map) {
    bool tm_up = false ;
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
    if (map->tilemap_shader != NULL) {
        Map_Tilemap_Surface_Stitch(map);
        SDL_assert(map->tilemap_surface);
        map->tilemap_surface = Tilemap_Shade_Surface(map->tilemap_shader,
                                                     map->tilemap_surface, 0, settings, camera);
        SDL_assert(map->tilemap_surface);
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = SDL_CreateTextureFromSurface(map->renderer, map->tilemap_surface);
    } else {
        SDL_assert(map->tilemap_texture);
        SDL_DestroyTexture(map->tilemap_texture);
        map->tilemap_texture = NULL;
        Map_Tilemap_Texture_Stitch(map, render_target);
    }
    map->camera_moved = false;
    map->shading_changed = false;
    map->update = false;
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
    bool sm_up = Map_Shadowmap_newFrame(map);
    bool tm_up = Map_Tilemap_newFrame(map);
    if (map->update || map->camera_moved)
        Map_Visible_Tiles(map, settings, camera);
    else
        map->visible_changed = false;

    /* -- Update Map -- */
    if (map->update || sm_up || tm_up || map->visible_changed || map->shading_changed)
        Map_Update(map, settings, camera, render_target);

    SDL_assert(map->tilemap_texture != NULL);
    SDL_RenderCopy(map->renderer, map->tilemap_texture, NULL, &dstrect); /* slow */

    /* -- Draw Arrow -- */
    if (map->arrow && map->arrow->show)
        Arrow_Draw(map->arrow, map->renderer, camera);

    Utilities_DrawColor_Reset(map->renderer);
}

