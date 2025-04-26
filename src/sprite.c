
#include "sprite.h"
#include "cJSON.h"
#include "palette.h"
#include "unit/flags.h"
#include "physfs.h"
#include "jsonio.h"
#include "platform.h"
#include "macros.h"
#include "log.h"
#include "names.h"
#include "nmath.h"
#include "filesystem.h"
#include "index_shader.h"

dstrect_func_t dstrect_funcs[TWO_D][TWO_D];

const int map_unit_offsets[MAP_UNIT_LOOP_NUM][TWO_D] = {
    /* MAP_UNIT_LOOP_IDLE */     {0, 1},
    /* MAP_UNIT_LOOP_TAUNT */    {0, 1},
    /* MAP_UNIT_LOOP_ITEM */     {0, 1},
    /* MAP_UNIT_LOOP_AGONY */    {0, 1},
    /* MAP_UNIT_LOOP_ATTACKR */  {0, 1},
    /* MAP_UNIT_LOOP_ATTACKT */  {0, 1},
    /* MAP_UNIT_LOOP_ATTACKL */  {1, 1},
    /* MAP_UNIT_LOOP_ATTACKB */  {1, 1},
    /* MAP_UNIT_LOOP_MOVER */    {0, 1},
    /* MAP_UNIT_LOOP_MOVET */    {1, 1},
    /* MAP_UNIT_LOOP_MOVEL */    {0, 1},
    /* MAP_UNIT_LOOP_MOVEB */    {0, 1},
    /* MAP_UNIT_LOOP_STANCE */   {0, 1},
};

/* --- Defaults --- */
const struct Spritesheet Spritesheet_default = {
    .jsonio_header.json_element   = JSON_SPRITE,
};

const struct Sprite Sprite_default = {
    .jsonio_header.json_element   = JSON_SPRITE,
    .srcrect        = { 0,  0, 32, 32}, /* x,y,w,h */
    .dstrect        = { 0,  0, 32, 32}, /* x,y,w,h */
    .scale          = { 1,  1},
    .tilesize       = {32, 32},
    .flip           = SDL_FLIP_NONE,
    .visible        = true
};

/* --- SPRITESHEET --- */
void Spritesheet_Free(struct Spritesheet *spritesheet) {
    if (spritesheet->loops_pos != NULL) {
        SDL_free(spritesheet->loops_pos);
        spritesheet->loops_pos = NULL;
    }
    if (spritesheet->loops != NULL) {
        SDL_free(spritesheet->loops);
        spritesheet->loops = NULL;
    }
    if (spritesheet->speeds != NULL) {
        for (int i = 0; i < spritesheet->loop_num; i++) {
            SDL_free(spritesheet->speeds[i]);
        }
        SDL_free(spritesheet->speeds);
        spritesheet->speeds = NULL;
    }
    if (spritesheet->frames != NULL) {
        SDL_free(spritesheet->frames);
        spritesheet->frames = NULL;
    }
    if (spritesheet->loop_modes != NULL) {
        SDL_free(spritesheet->loop_modes);
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
    s8_free(&spritesheet->jsonio_header.json_filename);
}

void Sprite_readJSON(void *input, const cJSON *jsprite) {
    struct Sprite *sprite = input;
    cJSON *jspritesheet = cJSON_GetObjectItem(jsprite, "Spritesheet");
    if (jspritesheet == NULL) {
        SDL_Log("JSON element Spritesheet does not exist in Sprite elem");
        exit(ERROR_JSONElementNotSet);
    }

    SDL_assert(sprite->spritesheet != NULL);
    Spritesheet_readJSON(sprite->spritesheet, jspritesheet);
}

void Spritesheet_readJSON(void *input, const cJSON *jspritesheet) {
    struct Spritesheet *spritesheet = (struct Spritesheet *)input;
    SDL_assert(spritesheet);
    Spritesheet_Free(spritesheet);
    cJSON *jloops       = cJSON_GetObjectItem(jspritesheet, "loops");
    cJSON *jframes      = cJSON_GetObjectItem(jspritesheet, "frames");
    cJSON *jspeeds      = cJSON_GetObjectItem(jspritesheet, "speeds");
    cJSON *jloops_pos   = cJSON_GetObjectItem(jspritesheet, "loops_pos");
    cJSON *jloop_modes  = cJSON_GetObjectItem(jspritesheet, "loop_modes");

    if (cJSON_IsArray(jloops)) {
        spritesheet->loop_num = cJSON_GetArraySize(jloops);
        spritesheet->loops = SDL_malloc(spritesheet->loop_num * sizeof(*spritesheet->loops));
        for (int i = 0; i < spritesheet->loop_num; i++) {
            cJSON *jloop = cJSON_GetArrayItem(jloops, i);
            spritesheet->loops[i] = cJSON_GetNumberValue(jloop);
        }
    }

    if (cJSON_IsArray(jloops_pos)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jloops_pos));
        spritesheet->loops_pos = SDL_malloc(spritesheet->loop_num * sizeof(*spritesheet->loops_pos));
        for (int i = 0; i < spritesheet->loop_num; i++) {
            cJSON *jpos = cJSON_GetArrayItem(jloops_pos, i);
            spritesheet->loops_pos[i] = cJSON_GetNumberValue(jpos);
        }
    }

    if (cJSON_IsArray(jframes)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jframes));
        spritesheet->frames = SDL_malloc(spritesheet->loop_num * sizeof(*spritesheet->frames));
        for (int i = 0; i < spritesheet->loop_num; i++) {
            cJSON *jframe = cJSON_GetArrayItem(jframes, i);
            spritesheet->frames[i] = cJSON_GetNumberValue(jframe);
        }
    }

    if (cJSON_IsArray(jspeeds)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jspeeds));
        spritesheet->speeds = SDL_malloc(spritesheet->loop_num * sizeof(*spritesheet->speeds));
        for (int i = 0; i < spritesheet->loop_num; i++) {
            cJSON *jspeed_arr = cJSON_GetArrayItem(jspeeds, i);
            SDL_assert(cJSON_IsArray(jspeed_arr));
            SDL_assert(spritesheet->frames[i] == cJSON_GetArraySize(jspeed_arr));
            spritesheet->speeds[i] = SDL_malloc(spritesheet->frames[i] * sizeof(**spritesheet->speeds));
            for (int j = 0; j < spritesheet->frames[i]; j++) {
                cJSON *jspeed = cJSON_GetArrayItem(jspeed_arr, j);
                spritesheet->speeds[i][j] = cJSON_GetNumberValue(jspeed);
            }
        }
    }

    if (cJSON_IsArray(jloop_modes)) {
        SDL_assert(spritesheet->loop_num == cJSON_GetArraySize(jloop_modes));
        spritesheet->loop_modes = SDL_malloc(spritesheet->loop_num * sizeof(*spritesheet->loop_modes));
        for (int i = 0; i < spritesheet->loop_num; i++) {
            cJSON *jloop_mode = cJSON_GetArrayItem(jloop_modes, i);
            spritesheet->loop_modes[i] = cJSON_GetNumberValue(jloop_mode);
        }
    }

}

void Spritesheet_Loop_Set(struct Spritesheet *spritesheet, int loop,
                          SDL_RendererFlip flip) {
    SDL_assert(loop < spritesheet->loop_num);
    SDL_assert(loop >= 0);
    i32 next_loop = loop;
    if (flip == SDL_FLIP_HORIZONTAL) {
        switch (loop) {
            case MAP_UNIT_LOOP_ATTACKR:
                next_loop = MAP_UNIT_LOOP_ATTACKL;
                break;
            case MAP_UNIT_LOOP_ATTACKL:
                next_loop = MAP_UNIT_LOOP_ATTACKR;
                break;
        }
    }
    /* Check if next loop is in loops of spritesheet */
    for (int i = 0; i < spritesheet->loop_num; i++) {
        if (next_loop == spritesheet->loops[i]) {
            spritesheet->current_loop   = next_loop;
            spritesheet->current_frame  = 0;
            spritesheet->frame_i        = 0;
            break;
        }
    }
}

/* --- SPRITE --- */
/* -- Constructor/Destructors -- */
void Sprite_Free(struct Sprite *sprite) {
    // SDL_Log("Freeing shaders");
    if (sprite->shader_any != NULL) {
        Index_Shader_Free(sprite->shader_any);
        SDL_free(sprite->shader_any);
        sprite->shader_any = NULL;
    }
    if (sprite->shader_darken != NULL) {
        Index_Shader_Free(sprite->shader_darken);
        SDL_free(sprite->shader_darken);
        sprite->shader_darken = NULL;
    }
    if (sprite->shader_lighten != NULL) {
        Index_Shader_Free(sprite->shader_lighten);
        SDL_free(sprite->shader_lighten);
        sprite->shader_lighten = NULL;
    }
    // SDL_Log("Freeing spritesheets");
    if (sprite->spritesheet != NULL) {
        Spritesheet_Free(sprite->spritesheet);
        SDL_free(sprite->spritesheet);
        sprite->spritesheet = NULL;
    }

    // SDL_Log("Freeing name");
    s8_free(&sprite->asset_name);
    s8_free(&sprite->jsonio_header.json_filename);

    // SDL_Log("Freeing Textures");
    if (sprite->texture != NULL) {
        SDL_DestroyTexture(sprite->texture);
        sprite->texture = NULL;
    }
}

/* --- Initialization --- */
void Sprite_Tilesize_Set(struct Sprite *sprite, u16 *tilesize) {
    SDL_assert(sprite != NULL);
    sprite->tilesize[0] = tilesize[0];
    sprite->tilesize[1] = tilesize[1];
}

void Sprite_defaultShaders_Load(struct Sprite *sprite) {
    SDL_assert(sprite->spritesheet->surface != NULL);
    /* -- Shade whole surface always -- */
    sprite->srcrect_shadow.x = 0;
    sprite->srcrect_shadow.y = 0;
    sprite->srcrect_shadow.w = sprite->spritesheet->surface->w;
    sprite->srcrect_shadow.h = sprite->spritesheet->surface->h;

    /* -- Darken shader -- */
    sprite->shader_darken  = SDL_malloc(sizeof(struct Index_Shader));
    *sprite->shader_darken = Index_Shader_default;
    Index_Shader_Load(sprite->shader_darken, sprite->spritesheet->surface, &sprite->srcrect_shadow);
    SDL_assert(sprite->shader_darken != NULL);
    sprite->shader_darken->to = palette_table_SOTA_darken;

    /* -- Lighten shader -- */
    sprite->shader_lighten  = SDL_malloc(sizeof(struct Index_Shader));
    *sprite->shader_lighten = Index_Shader_default;
    Index_Shader_Load(sprite->shader_lighten, sprite->spritesheet->surface, &sprite->srcrect_shadow);
    SDL_assert(sprite->shader_lighten != NULL);
    sprite->shader_lighten->to = palette_table_SOTA_lighten;
}

void Sprite_Map_Unit_Load(struct Sprite *sprite, struct Unit *unit,
                          SDL_Renderer *renderer) {
    SDL_assert(renderer != NULL);
    SDL_assert(sprite   != NULL);
    SDL_assert(unit     != NULL);

    /* -- Alloc Spritesheet -- */
    if (sprite->spritesheet != NULL) {
        Spritesheet_Free(sprite->spritesheet);
    } else {
        sprite->spritesheet  = SDL_malloc(sizeof(*sprite->spritesheet));
        *sprite->spritesheet = Spritesheet_default;
    }
    SDL_assert(sprite->spritesheet->jsonio_header.json_element == JSON_SPRITE);

    /* -- Loading spritesheet metadata -- */
    s8 filename = s8_mut(PATH_JOIN("assets", "map_units")PHYSFS_SEPARATOR);

    SDL_assert(classNames != NULL);
    SDL_assert((Unit_Class(unit) > UNIT_CLASS_START) && (Unit_Class(unit) < UNIT_CLASS_END));
    SDL_assert(classNames[Unit_Class(unit)].data != NULL);
    filename = s8cat(filename, s8_var(classNames[Unit_Class(unit)].data));
    filename = s8cat(filename, s8_literal(".json"));
    filename = s8_replaceSingle(filename, ' ', '_');
    if (!PHYSFS_exists(filename.data)) {
        SDL_LogError(SOTA_LOG_SYSTEM, "FILE '%s' does not exist", filename.data);
        exit(ERROR_CannotOpenFile);
    }

    jsonio_readJSON(filename, sprite);

    /* -- Loading spritesheet surface -- */
    s8_free(&filename);

    s8_free(&sprite->asset_name);
    sprite->asset_name = s8_mut(PATH_JOIN("..", "assets", "map_units")DIR_SEPARATOR);
    sprite->asset_name = s8cat(sprite->asset_name, s8_var(classNames[Unit_Class(unit)].data));
    sprite->asset_name = s8cat(sprite->asset_name, s8_literal(".png"));
    sprite->asset_name = s8_replaceSingle(sprite->asset_name, ' ', '_');

    /* -- Loading Surface, creating Texture -- */
    SDL_assert(sprite->spritesheet->surface == NULL);
    sprite->spritesheet->surface = Filesystem_Surface_Load(sprite->asset_name.data,
                                                           SDL_PIXELFORMAT_INDEX8);
    SDL_assert(sprite->spritesheet->surface != NULL);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
    sprite->map_unit = true;
    SDL_assert(sprite->spritesheet->current_loop == 0);

}

// Load Sprite to spritesheet by default
void Sprite_Load(struct Sprite *sprite, char *asset_name, SDL_Renderer *renderer) {
    /* -- Saving asset name -- */
    s8_free(&sprite->asset_name);
    sprite->asset_name = s8_mut(asset_name);

    /* -- Putting surface in default spritesheet -- */
    if (sprite->spritesheet == NULL) {
        sprite->spritesheet  = SDL_malloc(sizeof(*sprite->spritesheet));
        *sprite->spritesheet = Spritesheet_default;
    }
    SDL_assert(sprite->spritesheet->jsonio_header.json_element == JSON_SPRITE);

    /* -- Create texture from surface -- */
    if (sprite->spritesheet->surface != NULL)
        SDL_FreeSurface(sprite->spritesheet->surface);
    if (sprite->texture != NULL)
        SDL_DestroyTexture(sprite->texture);

    /* -- Keep spritesheed in memory -- */
    sprite->spritesheet->surface = Filesystem_Surface_Load(sprite->asset_name.data,
                                                           SDL_PIXELFORMAT_INDEX8);
    SDL_assert(renderer                     != NULL);
    SDL_assert(sprite->spritesheet->surface != NULL);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
}

/* -- rect computation -- */
void Cursor_Rects_Init(struct Sprite *sprite) {
    SDL_assert(sprite != NULL);
    sprite->srcrect.w = DEFAULT_CURSOR_SRC_W;
    sprite->srcrect.h = DEFAULT_CURSOR_SRC_H;
    sprite->dstrect.w = sprite->tilesize[0] * DEFAULT_CURSOR_FACTOR;
    sprite->dstrect.h = sprite->tilesize[1] * DEFAULT_CURSOR_FACTOR;
}

void Sprite_Rects_Init(struct Sprite *sprite) {
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
}

void Cursor_Dstrect_Relative(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    /* Compute dstrect from camera position and zoom */
    float zoom = camera->zoom;
    sprite->dstrect.x = SOTA_ZOOM(pixel_pos->x, zoom) + camera->offset.x;
    sprite->dstrect.y = SOTA_ZOOM(pixel_pos->y, zoom) + camera->offset.y;
    sprite->dstrect.w = SOTA_ZOOM(sprite->tilesize[0] * DEFAULT_CURSOR_FACTOR, zoom);
    sprite->dstrect.h = SOTA_ZOOM(sprite->tilesize[1] * DEFAULT_CURSOR_FACTOR, zoom);
}

void Sprite_Dstrect_Relative(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    /* Compute dstrect from camera position and zoom */
    float zoom = camera->zoom;
    if (sprite->map_unit) {
        /* Sprite is a map unit */
        // - Map unit sprites are bigger than tiles: 32x32 > 16x16
        // - Map units are on bottom left of 32x32 texture
        // - Compute offset so map_unit looks to be in middle of tile
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
        /* Generic sprite */
        sprite->dstrect.x = SOTA_ZOOM(pixel_pos->x, zoom) + camera->offset.x;
        sprite->dstrect.y = SOTA_ZOOM(pixel_pos->y, zoom) + camera->offset.y;
        sprite->dstrect.w = SOTA_ZOOM(sprite->tilesize[0], zoom);
        sprite->dstrect.h = SOTA_ZOOM(sprite->tilesize[1], zoom);
    }
}


// TODO: _Absolute functions are for camera == NULL too
void Sprite_Dstrect_Absolute(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    sprite->dstrect.x = pixel_pos->x;
    sprite->dstrect.y = pixel_pos->y;
}

void Cursor_Dstrect_Absolute(struct Sprite *sprite, struct Point *pixel_pos,
                             struct Camera *camera) {
    sprite->dstrect.x = pixel_pos->x;
    sprite->dstrect.y = pixel_pos->y;
    sprite->dstrect.w = sprite->tilesize[0] * sprite->scale.x;
    sprite->dstrect.h = sprite->tilesize[1] * sprite->scale.y;
}

/* --- Shading --- */
void Sprite_Palette_Swap(struct Sprite *sprite, SDL_Palette *palette, SDL_Renderer *renderer) {
    sprite->spritesheet->palette = palette;
    SDL_Surface *old_surface = sprite->spritesheet->surface;

    sprite->spritesheet->surface = Filesystem_Surface_Palette_Swap(old_surface, palette);
    SDL_FreeSurface(old_surface);
    if (sprite->texture != NULL)
        SDL_DestroyTexture(sprite->texture);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
}

void Sprite_Shade(struct Sprite *sprite, SDL_Renderer *renderer, struct Index_Shader *is) {
    /* -- Preliminaries -- */
    SDL_Surface *surface        = sprite->spritesheet->surface;
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
}

void Sprite_Unveil(struct Sprite *sprite, SDL_Renderer *renderer) {
    SDL_assert(sprite);
    SDL_assert(renderer);
    sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->spritesheet->surface);
}

void Sprite_Darken(struct Sprite *sprite, SDL_Renderer *renderer) {
    Sprite_Shade(sprite, renderer, sprite->shader_darken);
}

void Sprite_Lighten(struct Sprite *sprite, SDL_Renderer *renderer) {
    Sprite_Shade(sprite, renderer, sprite->shader_lighten);
}

/* --- Animation --- */
void Sprite_Animation_Restart(struct Sprite *sprite, int loop) {
    SDL_assert(sprite != NULL);
    struct Spritesheet *spritesheet = sprite->spritesheet;
    if (loop >= spritesheet->loop_num) {
        return;
    }
    Spritesheet_Loop_Set(spritesheet, loop, sprite->flip);

    spritesheet->current_frame  = 0;
    spritesheet->frame_i        = 0;

    int pos     = spritesheet->loops_pos[spritesheet->current_loop];
    int frame_x = pos % SPRITESHEET_COL_LEN;
    int frame_y = pos / SPRITESHEET_COL_LEN;

    sprite->srcrect.x = sprite->srcrect.w * frame_x;
    sprite->srcrect.y = sprite->srcrect.h * frame_y;
}

void Sprite_Animation_Loop(struct Sprite *sprite) {
    SDL_assert(sprite != NULL);
    struct Spritesheet *spritesheet = sprite->spritesheet;
    /* Skip if no animation to do. */
    i16 frames = spritesheet->frames[spritesheet->current_loop];
    if (frames <= 1)
        return;

    i16 next_frame_i = ++spritesheet->frame_i;
    i16 next_frame = 0;

    /*  - Compute next frame depending on loop mode - */
    switch (spritesheet->loop_modes[spritesheet->current_loop]) {
        case LOOPING_PINGPONG:
            /*next_f should go between [0, 2 * (frames-1)] to have full pingpong loop]*/
            next_frame = q_sequence_pingpong_int16_t(next_frame_i, frames, 0);
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
}

void Sprite_Draw(struct Sprite *sp, SDL_Renderer *renderer) {
    if (sp->flip != SDL_FLIP_NONE)
        SDL_RenderCopyEx(renderer, sp->texture, &sp->srcrect, &sp->dstrect, 0.0f, NULL, sp->flip);
    else
        SDL_RenderCopy(renderer, sp->texture, &sp->srcrect, &sp->dstrect);
}
