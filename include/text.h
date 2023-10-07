#ifndef TEXT_H
#define TEXT_H

#include "game/game.h"
#include "filesystem.h"
#include "pixelfonts.h"
#include "SDL2/SDL.h"
#include "nmath.h"

typedef void (* onUpdate_t)(struct Game *, tnecs_entity_t, u32, i64, void *);

enum SOTA_TEXT_CONSTANTS {
    SOTA_FPS_BUFFER = 8,
};

/* -- Text: Standalone Pixelfont -- */
typedef struct Text {
    SDL_Texture        *texture;
    struct PixelFont   *pixelfont;
    char                line[DEFAULT_BUFFER_SIZE];
    if16                len;
    SDL_Rect            rect;
    i64                 update_time_ns; /* 0 means update one time */
    onUpdate_t          onUpdate;
    bool                visible;
    bool                update;
} Text;
extern struct Text Text_default;

extern void Text_Set(       struct Text *t, char *l);
extern void Text_Draw(      struct Text *t, SDL_Renderer *r, SDL_Rect *d);
extern void Text_Update(    struct Text *t, SDL_Renderer *r);
extern void Text_onUpdate(  struct Text *t);

extern void Text_onUpdate_FPS(struct Game *g, tnecs_entity_t ent, u32 fc, i64 up, void *data);

#endif /* TEXT_H */
