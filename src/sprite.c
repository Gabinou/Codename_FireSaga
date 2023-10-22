#include "sprite.h"

dstrect_func_t dstrect_funcs[TWO_D][TWO_D];

int map_unit_offsets[MAP_UNIT_SPRITE_LOOP_NUM][TWO_D] = {
    /* MAP_UNIT_SPRITE_LOOP_IDLE */         {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_TAUNT */        {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_ITEM */         {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_AGONY */        {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_ATTACKR */      {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_ATTACKT */      {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_ATTACKL */      {1, 1},
    /* MAP_UNIT_SPRITE_LOOP_ATTACKB */      {1, 1},
    /* MAP_UNIT_SPRITE_LOOP_MOVER */        {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_MOVET */        {1, 1},
    /* MAP_UNIT_SPRITE_LOOP_MOVEL */        {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_MOVEB */        {0, 1},
    /* MAP_UNIT_SPRITE_LOOP_STANCE */       {0, 1},
};

/* --- Defaults --- */
struct Spritesheet Spritesheet_default = {
    .json_element   = JSON_SPRITE,
    .surface        = NULL,
    .palette        = NULL,
    .loops_pos      = NULL,
    .frames         = NULL,
    .speeds         = NULL,
    .loop_modes     = NULL,
    .loop_num       = 0,
    .current_loop   = 0,
    .current_frame  = 0,
};

struct Sprite Sprite_default = {
    .json_element   = JSON_SPRITE,
    .srcrect        = { 0,  0, 32, 32}, /* x,y,w,h */
    .dstrect        = { 0,  0, 32, 32}, /* x,y,w,h */
    .scale          = { 1,  1},
    .tilesize       = {32, 32},
    .spritesheet    = NULL,
    .texture        = NULL,
    .flip           = SDL_FLIP_NONE,
    .visible        = true,
    .map_unit       = false
};

/* --- SPRITESHEET --- */
void Spritesheet_Free(struct Spritesheet *spritesheet) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (spritesheet->loops_pos != NULL) {
        free(spritesheet->loops_pos);
        spritesheet->loops_pos = NULL;
    }
    if (spritesheet->speeds != NULL) {
        for (int i = 0; i < spritesheet->loop_num; ++i) {
            free(spritesheet->speeds[i]);
        }
        free(spritesheet->speeds);
        spritesheet->speeds = NULL;
    }
    if (spritesheet->frames != NULL) {
        free(spritesheet->frames);
        spritesheet->frames = NULL;
    }
    if (spritesheet->loop_modes != NULL) {
        free(spritesheet->loop_modes);
        spritesheet->loop_modes = NULL;
    }
    if (spritesheet->surface != NULL) {
        SDL_FreeSurface(spritesheet->surface);
        spritesheet->surface = NULL;
    }
    if (spritesheet->surface_shaded != NULL) {
        SDL_FreeSurface(spritesheet->surface_shaded);
        spritesheet->surface_shaded = NULL;
    }

    spritesheet->loop_num = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_readJSON(void *input, const cJSON *const jspritesheet) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Sprite *sprite = input;
    SDL_assert(sprite->spritesheet != NULL);
    Spritesheet_readJSON(sprite->spritesheet, jspritesheet);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void Spritesheet_readJSON(void *input, const cJSON *const jspritesheet) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Spritesheet *spritesheet = (struct Spritesheet *) input;
    SDL_assert(spritesheet);
    Spritesheet_Free(spritesheet);
    cJSON *jframes      = cJSON_GetObjectItem(jspritesheet, "frames");
    cJSON *jspeeds      = cJSON_GetObjectItem(jspritesheet, "speeds");
    cJSON *jloops_pos   = cJSON_GetObjectItem(jspritesheet, "loops_pos");
    cJSON *jloop_modes  = cJSON_GetObjectItem(jspritesheet, "loop_modes");

    if (cJSON_IsArray(jloops_pos)) {
        spritesheet->loop_num = cJSON_GetArraySize(jloops_pos);
        spritesheet->loops_pos = malloc(spritesheet->loop_num * sizeof(*spritesheet->loops_pos));
        for (int i = 0; i < spritesheet->loop_num; ++i) {
            cJSON *jpos = cJSON_GetArrayItem(jloops_pos, i);
            spritesheet->loops_pos[i] = cJSON_GetNumberValue(jpos);
        }
    }

    if (cJSON_IsArray(jframes)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jframes));
        spritesheet->frames = malloc(spritesheet->loop_num * sizeof(*spritesheet->frames));
        for (int i = 0; i < spritesheet->loop_num; ++i) {
            cJSON *jframe = cJSON_GetArrayItem(jframes, i);
            spritesheet->frames[i] = cJSON_GetNumberValue(jframe);
        }
    }

    if (cJSON_IsArray(jspeeds)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jspeeds));
        spritesheet->speeds = malloc(spritesheet->loop_num * sizeof(*spritesheet->speeds));
        for (int i = 0; i < spritesheet->loop_num; ++i) {
            cJSON *jspeed_arr = cJSON_GetArrayItem(jspeeds, i);
            SDL_assert(cJSON_IsArray(jspeed_arr));
            SDL_assert(spritesheet->frames[i] == cJSON_GetArraySize(jspeed_arr));
            spritesheet->speeds[i] = malloc(spritesheet->frames[i] * sizeof(**spritesheet->speeds));
            for (int j = 0; j < spritesheet->frames[i]; ++j) {
                cJSON *jspeed = cJSON_GetArrayItem(jspeed_arr, j);
                spritesheet->speeds[i][j] = cJSON_GetNumberValue(jspeed);
            }
        }
    }

    if (cJSON_IsArray(jloop_modes)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jloop_modes));
        spritesheet->loop_modes = malloc(spritesheet->loop_num * sizeof(*spritesheet->loop_modes));
        for (int i = 0; i < spritesheet->loop_num; ++i) {
            cJSON *jloop_mode = cJSON_GetArrayItem(jloop_modes, i);
            spritesheet->loop_modes[i] = cJSON_GetNumberValue(jloop_mode);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Spritesheet_Loop_Set(struct Spritesheet *spritesheet, int loop, SDL_RendererFlip flip) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(loop < spritesheet->loop_num);
    SDL_assert(loop >= 0);
    spritesheet->current_loop = loop;
    if (flip == SDL_FLIP_HORIZONTAL) {
        switch (loop) {
            case MAP_UNIT_SPRITE_LOOP_ATTACKR:
                spritesheet->current_loop = MAP_UNIT_SPRITE_LOOP_ATTACKL;
                break;
            case MAP_UNIT_SPRITE_LOOP_ATTACKL:
                spritesheet->current_loop = MAP_UNIT_SPRITE_LOOP_ATTACKR;
                break;
        }
    }

    spritesheet->current_frame = 0;
    spritesheet->frame_i = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- SPRITE --- */
/* -- Constructor/Destructors -- */
void Sprite_Free(struct Sprite *sprite) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("Freeing shaders");
    if (sprite->shader_any != NULL) {
        Index_Shader_Free(sprite->shader_any);
        free(sprite->shader_any);
        sprite->shader_any = NULL;
    }
    if (sprite->shader_darken != NULL) {
        Index_Shader_Free(sprite->shader_darken);
        free(sprite->shader_darken);
        sprite->shader_darken = NULL;
    }
    if (sprite->shader_lighten != NULL) {
        Index_Shader_Free(sprite->shader_lighten);
        free(sprite->shader_lighten);
        sprite->shader_lighten = NULL;
    }
    SOTA_Log_Debug("Freeing spritesheets");
    if (sprite->spritesheet != NULL) {
        Spritesheet_Free(sprite->spritesheet);
        free(sprite->spritesheet);
        sprite->spritesheet = NULL;
    }

    SOTA_Log_Debug("Freeing name");
    if (sprite->asset_name != NULL) {
        free(sprite->asset_name);
        sprite->asset_name = NULL;
    }
    SOTA_Log_Debug("Freeing Textures");
    if (sprite->texture != NULL) {
        SDL_DestroyTexture(sprite->texture);
        sprite->texture = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Initialization --- */
void Sprite_Tilesize_Set(struct Sprite *sprite, uf16 *tilesize) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite != NULL);
    sprite->tilesize[0] = tilesize[0];
    sprite->tilesize[1] = tilesize[1];
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_defaultShaders_Load(struct Sprite *sprite) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite->spritesheet->surface != NULL);
    /* -- Shade whole surface always -- */
    sprite->srcrect_shadow.x = 0;
    sprite->srcrect_shadow.y = 0;
    sprite->srcrect_shadow.w = sprite->spritesheet->surface->w;
    sprite->srcrect_shadow.h = sprite->spritesheet->surface->h;

    /* -- Darken shader -- */
    sprite->shader_darken = (struct Index_Shader *)SDL_malloc(sizeof(struct Index_Shader));
    *sprite->shader_darken = Index_Shader_default;
    Index_Shader_Load(sprite->shader_darken, sprite->spritesheet->surface, &sprite->srcrect_shadow);
    SDL_assert(sprite->shader_darken != NULL);
    sprite->shader_darken->to = palette_table_NES_darken;

    /* -- Lighten shader -- */
    sprite->shader_lighten = (struct Index_Shader *)SDL_malloc(sizeof(struct Index_Shader));
    *sprite->shader_lighten = Index_Shader_default;
    Index_Shader_Load(sprite->shader_lighten, sprite->spritesheet->surface, &sprite->srcrect_shadow);
    SDL_assert(sprite->shader_lighten != NULL);
    sprite->shader_lighten->to = palette_table_NES_lighten;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Map_Unit_Load(struct Sprite *sprite, struct Unit *unit, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    /* -- Alloc Spritesheet -- */
    if (sprite->spritesheet != NULL) {
        Spritesheet_Free(sprite->spritesheet);
    } else {
        sprite->spritesheet  = malloc(sizeof(*sprite->spritesheet));
        *sprite->spritesheet = Spritesheet_default;
    }
    SDL_assert(sprite->spritesheet->json_element == JSON_SPRITE);

    /* -- Loading spritesheet metadata -- */
    char filename[DEFAULT_BUFFER_SIZE] = {0};
    strcat(filename, PATH_JOIN("assets", "Map_Units")PHYSFS_SEPARATOR);
    SOTA_Log_Debug("unit->id %lu",    unit->_id);
    SOTA_Log_Debug("unit->name %s",   unit->name);
    SOTA_Log_Debug("unit->class %ld", unit->class);
    SDL_assert(classNames[unit->class] != NULL);

    strcat(filename, classNames[unit->class]);
    strcat(filename, ".json");
    SOTA_Log_Debug("FILE %s", filename);
    SDL_assert(PHYSFS_exists(filename));
    jsonio_readJSON(filename, sprite);

    /* -- Loading spritesheet surface -- */
    memset(filename, 0, DEFAULT_BUFFER_SIZE);
    strcat(filename, PATH_JOIN("..", "assets", "Map_Units")DIR_SEPARATOR);
    strcat(filename, classNames[unit->class]);
    strcat(filename, ".png");

    SOTA_Log_Debug("FILE %s", filename);
    // SDL_assert(PHYSFS_exists(filename));
    /* -- Saving asset name -- */
    if (sprite->asset_name != NULL)
        SDL_free(sprite->asset_name);
    size_t len = strlen(filename);
    sprite->asset_name = calloc(len, sizeof(*sprite->asset_name));
    strncpy(sprite->asset_name, filename, len);

    /* -- Loading Surface, creating Texture -- */
    SDL_assert(sprite->spritesheet->surface == NULL);
    SOTA_Log_Debug("FILE %s", filename);
    sprite->spritesheet->surface = Filesystem_Surface_Load(filename, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(sprite->spritesheet->surface != NULL);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
    sprite->map_unit = true;
    SDL_assert(sprite->spritesheet->current_loop == 0);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

// Load Sprite to spritesheet by default
void Sprite_Load(struct Sprite *sprite, const char *asset_name, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Saving asset name -- */
    if (sprite->asset_name != NULL)
        SDL_free(sprite->asset_name);
    size_t len = strlen(asset_name);
    sprite->asset_name = calloc(len, sizeof(*sprite->asset_name));
    strncpy(sprite->asset_name, asset_name, len);

    /* -- Putting surface in default spritesheet -- */
    if (sprite->spritesheet == NULL) {
        sprite->spritesheet = malloc(sizeof(*sprite->spritesheet));
        *sprite->spritesheet = Spritesheet_default;
    }
    SDL_assert(sprite->spritesheet->json_element == JSON_SPRITE);

    /* -- Create texture from surface -- */
    if (sprite->spritesheet->surface != NULL)
        SDL_FreeSurface(sprite->spritesheet->surface);
    if (sprite->texture != NULL)
        SDL_DestroyTexture(sprite->texture);

    /* -- Keep spritesheed in memory -- */
    sprite->spritesheet->surface = Filesystem_Surface_Load(asset_name, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(sprite->spritesheet->surface != NULL);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- rect computation -- */
void Cursor_Rects_Init(struct Sprite *sprite) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite != NULL);
    sprite->srcrect.w = DEFAULT_CURSOR_SRC_W;
    sprite->srcrect.h = DEFAULT_CURSOR_SRC_H;
    sprite->dstrect.w = sprite->tilesize[0] * DEFAULT_CURSOR_FACTOR;
    sprite->dstrect.h = sprite->tilesize[1] * DEFAULT_CURSOR_FACTOR;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Rects_Init(struct Sprite *sprite) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite != NULL);
    sprite->srcrect.w        = DEFAULT_SPRITE_SRC_W;
    sprite->srcrect.h        = DEFAULT_SPRITE_SRC_H;
    sprite->srcrect_shadow.w = sprite->srcrect.w;
    sprite->srcrect_shadow.h = sprite->srcrect.h;
    if (sprite->map_unit) {
        sprite->dstrect.w = sprite->tilesize[0];
        sprite->dstrect.h = sprite->tilesize[1];
    } else {
        sprite->dstrect.w = sprite->tilesize[0] * MAP_UNIT_TILESIZE_X;
        sprite->dstrect.h = sprite->tilesize[1] * MAP_UNIT_TILESIZE_Y;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Cursor_Dstrect_Relative(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    float zoom = camera->zoom;
    sprite->dstrect.x = SOTA_ZOOM(pixel_pos->x, zoom) + camera->offset.x;
    sprite->dstrect.y = SOTA_ZOOM(pixel_pos->y, zoom) + camera->offset.y;
    sprite->dstrect.w = SOTA_ZOOM(sprite->tilesize[0] * DEFAULT_CURSOR_FACTOR, zoom);
    sprite->dstrect.h = SOTA_ZOOM(sprite->tilesize[1] * DEFAULT_CURSOR_FACTOR, zoom);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Dstrect_Relative(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    float zoom = camera->zoom;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sprite->map_unit) {
        struct Spritesheet *spritesheet = sprite->spritesheet;
        SDL_assert(spritesheet != NULL);
        int offset_x = 0, offset_y = 0;
        if (sprite->flip == SDL_FLIP_HORIZONTAL) {
            offset_x = (MAP_UNIT_TILESIZE_X - map_unit_offsets[spritesheet->current_loop][0] - 1) *
                       sprite->tilesize[0];
            offset_y = (map_unit_offsets[spritesheet->current_loop][1]) * sprite->tilesize[1];
        } else {
            offset_x = map_unit_offsets[spritesheet->current_loop][0] * sprite->tilesize[0];
            offset_y = map_unit_offsets[spritesheet->current_loop][1] * sprite->tilesize[1];
        }
        sprite->dstrect.x = SOTA_ZOOM((pixel_pos->x - offset_x), zoom) + camera->offset.x;
        sprite->dstrect.y = SOTA_ZOOM((pixel_pos->y - offset_y), zoom) + camera->offset.y;
        sprite->dstrect.w = SOTA_ZOOM((sprite->tilesize[0] * MAP_UNIT_TILESIZE_X), zoom);
        sprite->dstrect.h = SOTA_ZOOM((sprite->tilesize[1] * MAP_UNIT_TILESIZE_Y), zoom);
    } else {
        sprite->dstrect.x = SOTA_ZOOM(pixel_pos->x, zoom) + camera->offset.x;
        sprite->dstrect.y = SOTA_ZOOM(pixel_pos->y, zoom) + camera->offset.y;
        sprite->dstrect.w = SOTA_ZOOM(sprite->tilesize[0], zoom);
        sprite->dstrect.h = SOTA_ZOOM(sprite->tilesize[1], zoom);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Dstrect_Absolute(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sprite->dstrect.x = pixel_pos->x;
    sprite->dstrect.y = pixel_pos->y;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Cursor_Dstrect_Absolute(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sprite->dstrect.x = pixel_pos->x;
    sprite->dstrect.y = pixel_pos->y;
    sprite->dstrect.w = sprite->tilesize[0] * sprite->scale.x;
    sprite->dstrect.h = sprite->tilesize[1] * sprite->scale.y;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Shading --- */
void Sprite_Palette_Swap(struct Sprite *sprite, SDL_Palette *palette, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sprite->spritesheet->palette = palette;
    SDL_Surface *surface = sprite->spritesheet->surface;
    sprite->spritesheet->surface = Filesystem_Surface_Palette_Swap(surface, palette);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Shade(struct Sprite *sprite, SDL_Renderer *renderer, struct Index_Shader *is) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_Surface *surface = sprite->spritesheet->surface;
    SDL_Surface *surface_shaded = sprite->spritesheet->surface_shaded;
    SDL_assert(surface);
    SDL_assert(is);

    if (surface_shaded != NULL)
        SDL_FreeSurface(surface_shaded);
    if (sprite->texture != NULL)
        SDL_DestroyTexture(sprite->texture);

    /* -- Shade surface -- */
    surface_shaded = Index_Shade_Surface(is, surface, &sprite->srcrect_shadow);
    SDL_assert(surface_shaded != NULL);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, surface_shaded);
    SDL_FreeSurface(surface_shaded);
    surface_shaded = NULL;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Unveil(struct Sprite *sprite, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Darken(struct Sprite *sprite, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Sprite_Shade(sprite, renderer, sprite->shader_darken);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Lighten(struct Sprite *sprite, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Sprite_Shade(sprite, renderer, sprite->shader_lighten);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Animation --- */
void Sprite_Animation_Restart(struct Sprite *sprite, int loop) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite != NULL);
    struct Spritesheet *spritesheet = sprite->spritesheet;
    Spritesheet_Loop_Set(spritesheet, loop, sprite->flip);

    spritesheet->current_frame = 0;
    spritesheet->frame_i = 0;

    int pos     = spritesheet->loops_pos[spritesheet->current_loop];
    int frame_x = pos % SPRITESHEET_COL_LEN;
    int frame_y = pos / SPRITESHEET_COL_LEN;

    sprite->srcrect.x = sprite->srcrect.w * frame_x;
    sprite->srcrect.y = sprite->srcrect.h * frame_y;

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Animation_Loop(struct Sprite *sprite) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sprite != NULL);
    struct Spritesheet *spritesheet = sprite->spritesheet;
    if16 next_frame_i = ++spritesheet->frame_i;
    if16 next_frame = 0;
    if16 frames = spritesheet->frames[spritesheet->current_loop];

    /*  - Compute next frame depending on loop mode - */
    switch (spritesheet->loop_modes[spritesheet->current_loop]) {
        case LOOPING_PINGPONG:
            /*next_f should go between [0, 2 * (frames-1)] to have full pingpong loop]*/
            next_frame = q_sequence_pingpong_int_fast16_t(next_frame_i, frames, 0);
            break;
        case LOOPING_FORWARD:
            next_frame = next_frame_i % frames;
            break;
        case LOOPING_REVERSE:
            next_frame = (frames - next_frame_i) % frames;
            break;
        default:
            exit(NO_ERROR);
            break;
    }

    spritesheet->current_frame = next_frame;

    /*  - Reset frame counter - */
    if (next_frame == 0)
        spritesheet->frame_i = 0;

    int pos     = spritesheet->loops_pos[spritesheet->current_loop];
    int frame_x = (pos + next_frame) % SPRITESHEET_COL_LEN;
    int frame_y = (pos + next_frame) / SPRITESHEET_COL_LEN;

    sprite->srcrect.x = sprite->srcrect.w * frame_x;
    sprite->srcrect.y = sprite->srcrect.h * frame_y;

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Sprite_Draw(struct Sprite *sp, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sp->flip != SDL_FLIP_NONE)
        SDL_RenderCopyEx(renderer, sp->texture, &sp->srcrect, &sp->dstrect, 0.0f, NULL, sp->flip);
    else
        SDL_RenderCopy(renderer, sp->texture, &sp->srcrect, &sp->dstrect);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
