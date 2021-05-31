#ifndef SPRITE_H
#define SPRITE_H

#include "map.h"
#include "filesystem.h"
#include "position.h"
#include "controllerKeyboard.h"
#include "controllerGamepad.h"
#include "utilities.h"
#include "q_math.h"
#include "SDL2/SDL.h"

extern SDL_Renderer * Game_renderer;


typedef struct Sprite {
    // Unaligned size  bytes
    // Aligned size 212 bytes
    SDL_Rect srcrect; //x,y,w,h
    SDL_Rect destrect; //x,y,w,h

    SDL_Texture * texture;
    uint16_t tilesize[2];

    struct Point objectivepos;
    struct Point slidepos;

    int16_t frames;
    int16_t speed;
    int16_t slideint; // for slide_type = "geometric"
    bool visible;
    bool animated;

    float slidefactors[2]; // for slide_type = "geometric"

    float update_wait;
    float update_timer;

    char asset_name[DEFAULT_BUFFER_SIZE];

    uint8_t ss_looping; //ss: spritesheet
    uint8_t slidetype;

} Sprite;
extern struct Sprite Sprite_default;

typedef struct SpriteModule {
    ECS_DECLARE_COMPONENT(Sprite);
} SpriteModule;

void SpriteModuleImport(ecs_world_t * in_world);
#define SpriteModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Sprite);

extern void Sprite_Rects_init(struct Sprite * in_sprite, struct Point in_position);
extern void Sprite_slideType_Set(struct Sprite * in_sprite, uint8_t in_slidetype);
extern void Sprite_animatedRects_Init(struct Sprite * in_sprite);
extern void Sprite_tileSize_Set(struct Sprite * in_sprite, uint16_t * in_tilesize);
extern void Sprite_Texture_Set(struct Sprite * in_sprite, const char * in_asset_name);
extern void Sprite_darken(struct Sprite * in_sprite);
extern void Sprite_lighten(struct Sprite * in_sprite);
extern void Sprite_highlight(struct Sprite * in_sprite);
extern void Sprite_loop(struct Sprite * in_sprite);
extern void Sprite_slide(struct Sprite * in_sprite, bool slide_int, float dt);

#endif /* SPRITE_H */
