#include "index_shader.h"

struct Index_Shader Index_Shader_default = {
    .to            = NULL,
    .pixels_list   = NULL,
    .pixels_num    = 0,
    .shaded_pixels = NULL, /* only shade pixels in here */
};

struct Tilemap_Shader Tilemap_Shader_default = {
    .to                      = NULL,
    .shadowtile_pixels_lists = NULL,
    .shadowtile_num          = 0,
    .shadowtile_pixels_num   = NULL,
    .shadow_tilemaps         = NULL,
    .map                     = NULL,
};

i32 *matrix_circ_noise(i32 *matrix, i32 origx, i32 origy,
                       size_t width, size_t height, size_t row_len, size_t col_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (NULL);
}

i32 *matrix_rect_noise(i32 *matrix, i32 origx, i32 origy,
                       size_t diameter, size_t row_len, size_t col_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // How to add noise?
    // flip bits inside a certain radius, greater than the circle? uniformly distributed points?
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (NULL);
}

/* --- pixels --- */
u8 *pixels2list_noM(u8 *matrix, u8 *list, size_t row_len, size_t col_len) {
    DARR_NUM(list) = 0;
    for (size_t col = 0; col < col_len; col++) {
        for (size_t row = 0; row < row_len; row++) {
            if (matrix[row * col_len + col] > 0) {
                DARR_PUT(list, col);
                DARR_PUT(list, row);
            }
        }
    }
    return (list);
}

u8 *pixels2list(u8 *matrix, size_t row_len, size_t col_len) {
    u8 *list = DARR_INIT(list, u8, row_len * col_len * 2);
    list = pixels2list_noM(matrix, list, row_len, col_len);
    size_t newsize = (DARR_NUM(list) < SOTA_MINLEN) ? SOTA_MINLEN : DARR_NUM(list);
    list = DARR_REALLOC(list, newsize);
    return (list);
}

u8 *pixels_and_noM(u8 *out, u8 *matrix1, u8 *matrix2, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] && matrix2[i]);
    return (out);
}

u8 *pixels_and(u8 *matrix1, u8 *matrix2, size_t arr_len) {
    u8 *out = calloc(arr_len, sizeof(*out));
    return (pixels_and_noM(out, matrix1, matrix2, arr_len));
}


void _Index_Shade_Pixels(u8 *to, SDL_Surface *unlocked_surface, u8 *pixels_list,
                         size_t pixels_num, size_t offset_x, size_t offset_y) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(unlocked_surface->format->format));
    SDL_assert(pixels_num > 0);
    u8 *pixels = unlocked_surface->pixels;

    for (size_t i = 0; i < pixels_num; i++) {
        size_t y = pixels_list[TWO_D * i + 1];
        size_t x = pixels_list[TWO_D * i];
        SDL_assert(x < unlocked_surface->w);
        SDL_assert(y < unlocked_surface->h);
        SDL_assert((x + offset_x) < unlocked_surface->w);
        SDL_assert((y + offset_y) < unlocked_surface->h);
        size_t pos = Util_SDL_Surface_Index(unlocked_surface, (x + offset_x), (y + offset_y));
        pixels[pos] = to[pixels[pos]];
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Index_Shade_Rect( u8 *to, SDL_Surface *unlocked_surface, SDL_Rect *rect) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    u8 *pixels = unlocked_surface->pixels;
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(unlocked_surface->format->format));
    for (size_t x = rect->x; x < rect->x + rect->w; x++) {
        for (size_t y = rect->y; y < rect->y + rect->h; y++) {
            size_t index = Util_SDL_Surface_Index(unlocked_surface, x, y);
            pixels[index] = to[pixels[index]];
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- TILEMAP SHADER --- */
void Tilemap_Shader_Load_Tilemap_JSON(struct Tilemap_Shader *shd, const cJSON *const jmap) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(shd != NULL);
    SDL_assert(jmap != NULL);
    SDL_assert(shd->map != NULL);
    SDL_assert(shd->map->row_len > 0);
    SDL_assert(shd->map->col_len > 0);

    /* - Frames from shadow tilemap - */
    cJSON *jtilemap_shadow = cJSON_GetObjectItem(jmap, "tilemap_shadow");
    SDL_assert(jtilemap_shadow);
    cJSON *jframes = cJSON_GetObjectItem(jtilemap_shadow, "frames");
    SDL_assert(jframes);
    SDL_assert(cJSON_IsArray(jframes));
    shd->frames = cJSON_GetArraySize(jframes);

    /* - Init shadow tilemaps - */
    shd->shadow_tilemaps = DARR_INIT(shd->shadow_tilemaps, i32 *, SOTA_TILESIZE);
    SDL_assert(shd->shadow_tilemaps);
    SDL_assert(shd->frames > 0);

    /* - Load shadow_tilemaps for every frames - */
    for (size_t i = 0; i < shd->frames; i++) {
        /* - Get frame - */
        int current = DARR_NUM(shd->shadow_tilemaps);
        SDL_assert(current == i);
        cJSON *jframe = cJSON_GetArrayItem(jframes, i);
        SDL_assert(jframe);

        /* - Read shadow tilemap for current frame - */
        int row_len = shd->map->row_len, col_len = shd->map->col_len;
        i32 *shadow_tilemap = malloc(row_len * col_len * sizeof(*shadow_tilemap));
        cJSON *jarr = cJSON_GetObjectItem(jframe, "array");
        Filesystem_readJSON_2DArray(jarr, shadow_tilemap, row_len, col_len);

        /* - Put shadow tilemap in list of shadow tilemaps - */
        DARR_PUT(shd->shadow_tilemaps, shadow_tilemap);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tilemap_Shader_Free(struct Tilemap_Shader *shd) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("shadowtile_pixels_lists");
    SDL_assert(shd != NULL);
    if (shd->shadowtile_pixels_lists != NULL) {
        for (size_t i = 0; i < shd->shadowtile_num; i++) {
            if (shd->shadowtile_pixels_lists[i] != NULL) {
                free(shd->shadowtile_pixels_lists[i]);
                shd->shadowtile_pixels_lists[i] = NULL;
            }
        }
        free(shd->shadowtile_pixels_lists);
        shd->shadowtile_pixels_lists = NULL;
    }
    SOTA_Log_Debug("shadowtile_pixels_num");
    if (shd->shadowtile_pixels_num != NULL) {
        free(shd->shadowtile_pixels_num);
        shd->shadowtile_pixels_num = NULL;
    }
    SOTA_Log_Debug("shadow_tilemaps i");
    if (shd->shadow_tilemaps != NULL) {
        for (size_t i = 0; i < shd->frames; i++) {
            if (shd->shadow_tilemaps[i] != NULL) {
                free(shd->shadow_tilemaps[i]);
                shd->shadow_tilemaps[i] = NULL;
            }
        }
        SOTA_Log_Debug("shadow_tilemaps");
        DARR_FREE(shd->shadow_tilemaps);
        shd->shadow_tilemaps = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void _Tilemap_Shader_Shadow_Free( struct Tilemap_Shader *shd) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (shd->shadowtile_pixels_num != NULL)
        free(shd->shadowtile_pixels_num);
    if (shd->shadowtile_pixels_lists != NULL)
        free(shd->shadowtile_pixels_lists);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tilemap_Shader_Alloc(struct Tilemap_Shader *shd, size_t tilenum) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    shd->shadowtile_num = tilenum;
    shd->shadowtile_pixels_num = calloc(tilenum, sizeof(*shd->shadowtile_pixels_num));
    shd->shadowtile_pixels_lists = calloc(tilenum, sizeof(*shd->shadowtile_pixels_lists));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tilemap_Shader_Load_Tileset_pixels(struct Tilemap_Shader *shd, const char *filename,
                                        size_t tilenum, i32 tilesize[TWO_D]) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    _Tilemap_Shader_Shadow_Free( shd);
    Tilemap_Shader_Alloc(shd, tilenum);
    SDL_Surface *surf = Filesystem_Surface_Load(filename, SDL_PIXELFORMAT_INDEX8);
    SDL_LockSurface(surf);
    size_t x, y, offset, bytesize;
    u8 *temp_arr = NULL;

    /* -- Read shadow tilemaps from file surface -- */
    for (size_t i = 0; i < shd->shadowtile_num; i++) {
        x = (i % TILESET_COL_LEN) * tilesize[0];
        y = (i / TILESET_COL_LEN) * tilesize[1];
        offset = Util_SDL_Surface_Index(surf, x, y);

        /* - read shaded pixels - */
        temp_arr = pixels2list(surf->pixels + offset, tilesize[1], tilesize[0]);
        /* - alloc shadowtile pixels - */
        shd->shadowtile_pixels_num[i] = DARR_NUM(temp_arr) / 2;
        bytesize = shd->shadowtile_pixels_num[i] * sizeof(*temp_arr);
        shd->shadowtile_pixels_lists[i] = malloc(bytesize);
        /* - copy shaded pixels into shadowtile_pixels - */
        memcpy(shd->shadowtile_pixels_lists[i], temp_arr, bytesize);
        DARR_FREE(temp_arr);
    }
    SDL_UnlockSurface(surf);
    SDL_FreeSurface(surf);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tilemap_Shader_Load_Tileset_JSON(struct Tilemap_Shader *shd,
                                      const cJSON *const jshadow_tileset) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(shd != NULL);
    SDL_assert(jshadow_tileset != NULL);

    /* -- Read shadow tiles number -- */
    cJSON *jtilesnum = cJSON_GetObjectItem(jshadow_tileset, "tiles_num");

    /* -- Alloc shadow tiles -- */
    _Tilemap_Shader_Shadow_Free( shd);
    Tilemap_Shader_Alloc(shd, cJSON_GetNumberValue(jtilesnum));
    cJSON *jtiles_pixelnum = cJSON_GetObjectItem(jshadow_tileset, "tiles_pixelnum");
    SDL_assert(jtiles_pixelnum != NULL);

    /* -- Read number of pixels in each shadow tile -- */
    struct cJSON *jnum;
    size_t j = 0;
    cJSON_ArrayForEach(jnum, jtiles_pixelnum) {
        shd->shadowtile_pixels_num[j++] = (size_t)cJSON_GetNumberValue(jnum);
    }

    /* -- Read pixels of each shadow tile -- */
    for (size_t i = 0; i < shd->shadowtile_num; i++) {
        if (shd->shadowtile_pixels_num[i] == 0)
            continue;

        /* - Alloc shadow tile - */
        size_t bytesize = sizeof(*shd->shadowtile_pixels_lists[i]);
        bytesize *= TWO_D * shd->shadowtile_pixels_num[i];
        shd->shadowtile_pixels_lists[i] = malloc(bytesize);

        /* - Get tile in json - */
        char tilename[TILENAME_MAX_LEN];
        stbsp_sprintf(tilename, "tile%d", i);
        struct cJSON *jtile = cJSON_GetObjectItem(jshadow_tileset, tilename);

        /* - Read pixels in shadow tile - */
        j = 0;
        cJSON_ArrayForEach(jnum, jtile) {
            shd->shadowtile_pixels_lists[i][j++] = (u8)cJSON_GetNumberValue(jnum);
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Tilemap_Shader_Load_JSON(struct Tilemap_Shader *shd, const cJSON *const jmap,
                              const cJSON *const jshadow_tileset) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(shd);
    SDL_assert(jmap);
    SDL_assert(jshadow_tileset);
    Tilemap_Shader_Load_Tilemap_JSON(shd, jmap);
    Tilemap_Shader_Load_Tileset_JSON(shd, jshadow_tileset);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

SDL_Surface *Tilemap_Shade_Surface(struct Tilemap_Shader *shd, SDL_Surface *surf, u8 frame,
                                   const struct Settings *settings, struct Camera *camera) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(shd != NULL);
    SDL_assert(surf != NULL);
    SDL_assert(shd->shadow_tilemaps != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));
    u8 minpos[TWO_D], maxpos[TWO_D], st_index;
    int row_len = shd->map->row_len, col_len = shd->map->col_len;
    i32 *tsize = shd->map->tilesize;
    Map_Visible_Bounds(minpos, maxpos, row_len, col_len, tsize, &settings->res, camera);
    SDL_assert(minpos[0] <= maxpos[0]);
    SDL_assert(minpos[1] <= maxpos[1]);

    /* -- Shade tiles -- */
    SDL_LockSurface(surf);
    for (size_t col = minpos[0]; col < maxpos[0]; col++) {
        for (size_t row = minpos[1]; row < maxpos[1]; row++) {
            int index = row * col_len + col;
            st_index = shd->shadow_tilemaps[frame][index] - TILE_DIVISOR * TILE_SHADOW;
            u8 *list = shd->shadowtile_pixels_lists[st_index];
            size_t num = shd->shadowtile_pixels_num[st_index];
            if ((st_index > 0) && (st_index < shd->shadowtile_num))
                _Index_Shade_Pixels(shd->to, surf, list, num, col * tsize[1], row * tsize[0]);
        }
    }
    SDL_UnlockSurface(surf);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (surf);
}

/* --- INDEX SHADER --- */
void Index_Shader_Load(struct Index_Shader *shd, SDL_Surface *surf, SDL_Rect *rect) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(surf != NULL);
    SDL_assert(rect != NULL);
    SDL_assert(surf->format->format == SDL_PIXELFORMAT_INDEX8);
    SDL_assert(surf->pixels != NULL);

    /* -- Copy pixels for surface -- */
    SDL_LockSurface(surf);
    size_t bytesize = rect->w * rect->h;
    size_t offset = Util_SDL_Surface_Index(surf, rect->x, rect->y);

    u8 *temp_arr = malloc(bytesize);
    memcpy(temp_arr, surf->pixels + offset, bytesize);
    SDL_UnlockSurface(surf);

    /* -- Find which pixels have to be shaded -- */
    if (shd->shaded_pixels != NULL)
        temp_arr = pixels_and(temp_arr, shd->shaded_pixels, rect->h * rect->w);

    /* - Make list of shaded pixels from 2D array - */
    if (shd->pixels_list != NULL)
        DARR_FREE(shd->pixels_list);
    shd->pixels_list = pixels2list(temp_arr, rect->h, rect->w);
    shd->pixels_num = (DARR_NUM(shd->pixels_list) / 2);
    free(temp_arr);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Index_Shader_Free(struct Index_Shader *shd) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (shd != NULL) {
        if (shd->shaded_pixels != NULL)
            free(shd->shaded_pixels);
        if (shd->pixels_list != NULL)
            DARR_FREE(shd->pixels_list);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

SDL_Surface *Index_Shade_Surface(struct Index_Shader *shd, SDL_Surface *surf, SDL_Rect *rect) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(shd != NULL);
    SDL_assert(shd->pixels_list != NULL);
    SDL_assert(surf != NULL);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(surf->format->format));

    /* -- Create new surf to shade -- */
    /* - copy pixel data - */
    SDL_LockSurface(surf);
    void *new_pixels = SDL_malloc(surf->h * surf->pitch);
    SDL_memcpy(new_pixels, surf->pixels, surf->h * surf->pitch);
    SDL_UnlockSurface(surf);

    SDL_Surface *out = SDL_CreateRGBSurfaceWithFormatFrom(new_pixels, surf->w, surf->h,
                                                          surf->format->BitsPerPixel, surf->pitch, surf->format->format);
    SDL_assert(out != NULL);
    SDL_assert((out->w > 0) && (out->h > 0));
    int success = SDL_SetColorKey(out, SDL_TRUE, PALETTE_COLORKEY);
    SDL_assert(success == 0);
    SDL_assert(SDL_ISPIXELFORMAT_INDEXED(out->format->format));
    success = SDL_SetSurfacePalette(out, palette_NES);
    SDL_assert(success == 0);

    /* -- Shade surf -- */
    SDL_LockSurface(out);
    SDL_LockSurface(surf);
    _Index_Shade_Pixels(shd->to, out, shd->pixels_list, shd->pixels_num, rect->x, rect->y);
    SDL_UnlockSurface(out);
    SDL_UnlockSurface(surf);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}
