#include "sprite.h"

struct Sprite Sprite_default = {
    .srcrect = {0, 0, 32, 32}, //x,y,w,h
    .destrect = {0, 0, 32, 32}, //x,y,w,h
    .frames = 10,
    .speed = 50,
    .slideint = 0, // for slide_type = "geometric"
    .tilesize = {32, 32},
    .slidefactors = {2.0f, 1.025f}, // for slide_type = "geometric"
    .slidepos = {0, 0},
    .objectivepos = {0, 0},

    .update_wait = 0.0f,
    .update_timer = 0.0f,

    .visible = true,
    .animated = false,
    .ss_looping = LOOPING_PINGPONG, //ss: spritesheet
    .slidetype = SLIDETYPE_GEOMETRIC,
};

void SpriteModuleImport(ecs_world_t * in_world) {
    SDL_Log("SpriteModuleImport");
    ECS_MODULE(in_world, SpriteModule);
    ECS_COMPONENT(in_world, Sprite);
    ECS_SET_COMPONENT(Sprite);
    ECS_EXPORT_COMPONENT(Sprite);
}

void Sprite_slideType_Set(struct Sprite * in_sprite, uint8_t in_slidetype) {
    in_sprite->slidetype = in_slidetype;
    SDL_Log("Sprite_slideType_Set");
    if (in_sprite->animated) {
        Sprite_animatedRects_Init(in_sprite);
    }
}

void Sprite_Rects_init(struct Sprite * in_sprite, struct Point in_position) {
    SDL_Log("Sprite_Rects_init");
    SDL_Log("in_pos: %d %d", in_position.x, in_position.y);
    in_sprite->srcrect.x = 0;
    in_sprite->srcrect.y = 0;
    in_sprite->srcrect.w = in_sprite->tilesize[0];
    in_sprite->srcrect.h = in_sprite->tilesize[1];
    in_sprite->destrect.x = 0;
    in_sprite->destrect.y = 0;
    in_sprite->destrect.w = in_sprite->tilesize[0];
    in_sprite->destrect.h = in_sprite->tilesize[1];
    in_sprite->slidepos.x = in_sprite->objectivepos.x = in_position.x * in_sprite->tilesize[0] - in_sprite->destrect.w / 4;
    in_sprite->slidepos.y = in_sprite->objectivepos.y = in_position.y * in_sprite->tilesize[1] - in_sprite->destrect.h / 4;
    // SDL_Log("srcrect %d %d %d %d", in_sprite->srcrect.x, in_sprite->srcrect.y, in_sprite->srcrect.w, in_sprite->srcrect.h);
    // SDL_Log("destrect %d %d %d %d", in_sprite->destrect.x, in_sprite->destrect.y, in_sprite->destrect.w, in_sprite->destrect.h);
}

void Sprite_animatedRects_Init(struct Sprite * in_sprite) {
    switch (in_sprite->slidetype) {
        case SLIDETYPE_GEOMETRIC:
            in_sprite->srcrect.w = in_sprite->tilesize[0] * 2;
            in_sprite->srcrect.h = in_sprite->tilesize[1] * 2;
            in_sprite->destrect.w = in_sprite->tilesize[0] * 2;
            in_sprite->destrect.h = in_sprite->tilesize[1] * 2;
            break;
        default:
            SDL_Log("slidetype is invalid");
    }
}

void Sprite_tileSize_Set(struct Sprite * in_sprite, uint16_t * in_tilesize) {
    in_sprite->tilesize[0] = in_tilesize[0];
    in_sprite->tilesize[1] = in_tilesize[1];
    if (in_sprite->animated) {
        Sprite_animatedRects_Init(in_sprite);
    }
}

void Sprite_Texture_Set(struct Sprite * in_sprite, const char * in_asset_name) {
    SDL_Log("Sprite_Texture_Set");
    strcpy(in_sprite->asset_name, in_asset_name);
    in_sprite->texture = loadTexture(Game_renderer, in_asset_name, false);
}

void Sprite_darken(struct Sprite * in_sprite) {
    SDL_Log("Sprite_darken");
    SDL_assert(in_sprite->texture != NULL);
    SDL_SetTextureColorMod(in_sprite->texture, 128, 128, 128);
}

void Sprite_lighten(struct Sprite * in_sprite) {
    SDL_Log("Sprite_lighten");
    SDL_assert(in_sprite->texture != NULL);
    SDL_SetTextureColorMod(in_sprite->texture, 255, 255, 255);
}

void Sprite_loop(struct Sprite * in_sprite) {
    // SDL_Log("Sprite_loop");
    SDL_assert(in_sprite != NULL);
    switch (in_sprite->ss_looping) {
        case LOOPING_PINGPONG:
            in_sprite->srcrect.x = in_sprite->srcrect.w * sequence_pingpong_int16_t((int16_t)(SDL_GetTicks() / in_sprite->speed),  in_sprite->frames, 0);
            break;
        case LOOPING_LINEAR:
        case LOOPING_DIRECT:
            in_sprite->srcrect.x = in_sprite->srcrect.w * (int16_t)((SDL_GetTicks() /  in_sprite->speed) %  in_sprite->frames);
            break;

        case LOOPING_REVERSE:
            in_sprite->srcrect.x = in_sprite->srcrect.w * (in_sprite->frames - (int16_t)((SDL_GetTicks() /  in_sprite->speed) %  in_sprite->frames));
            break;

        default:
            SDL_Log("looping is invalid");
    }
}

void Sprite_slide(struct Sprite * in_sprite, bool slide_int, float dt) {
    // SDL_Log("Sprite_slide");
    SDL_assert(in_sprite != NULL);
    switch (in_sprite->slidetype) {
        case SLIDETYPE_GEOMETRIC: //for cursor mvt on map
            if (in_sprite->objectivepos.x != in_sprite->slidepos.x) {
                in_sprite->slidepos.x += sequence_geometric_int32_t(in_sprite->objectivepos.x, in_sprite->slidepos.x, in_sprite->slidefactors[slide_int]);
            }
            if (in_sprite->objectivepos.y != in_sprite->slidepos.y) {
                in_sprite->slidepos.y += sequence_geometric_int32_t(in_sprite->objectivepos.y, in_sprite->slidepos.y, in_sprite->slidefactors[slide_int]);
            }
            break;
        default:
            SDL_Log("slidetype is invalid");
    }
}