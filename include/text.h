#ifndef TEXT_H
#define TEXT_H

#include "filesystem.h"
#include "pixelfonts.h"
#include "SDL2/SDL.h"
#include "nmath.h"

typedef void (* onUpdate_t)(tnecs_world_t *, tnecs_entity_t, u32, i64, void *);

typedef struct Text_TTF {
    SDL_Texture      *texture;
    char             text_line[DEFAULT_BUFFER_SIZE];
    SDL_Color        text_color;
    SDL_Rect         srcrect;
    SDL_Rect         dstrect;

    TTF_Font         *font;
    if16             padding[NMATH_SQUARE_NEIGHBOURS]; /* html style: up right down left */
    if16             fontsize;

    i64              update_time_ns; /* 0 means update one time */
    onUpdate_t       onUpdate;

    bool             visible;
    float            spacingfactor;
    float            sizefactor[TWO_D]; /* height, width */
} Text_TTF;
extern struct Text_TTF Text_TTF_default;

extern void Text_TTF_Free(         struct Text_TTF *t);
extern void Text_TTF_Draw(         struct Text_TTF *t, SDL_Renderer *r);
extern void Text_TTF_Texture_Make( struct Text_TTF *t, SDL_Renderer *r);
extern void Text_TTF_Rects_Pos_Set(struct Text_TTF *t, if16 x, if16 y);

extern void Text_TTF_onUpdate_FPS(tnecs_world_t *w, tnecs_entity_t ent, u32 fc, i64 up, void *data);

typedef struct Text {
    SDL_Renderer       *renderer;
    SDL_Texture        *texture;
    struct PixelFont   *pixelfont;
    char                line[DEFAULT_BUFFER_SIZE];
    if16                strlen;
    if16                len2draw;
    /* text should go to next line inside box */
    if16                box_x;
    if16                box_y;

    i64                 update_time_ns; /* 0 means update one time */
    onUpdate_t          onUpdate;

    bool                visible;
    bool                update;
    if16                align;
    if16                scroll_speed;
} Text;
extern struct Text Text_default;

extern void Text_Draw(      struct Text *t, SDL_Renderer *renderer);
extern void Text_Update(    struct Text *t, SDL_Renderer *renderer);
extern void Text_onUpdate(  struct Text *t);
extern void Text_Box_Create(struct Text *t);

#endif /* TEXT_H */
