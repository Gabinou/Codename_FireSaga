#ifndef TEXT_H
#define TEXT_H

#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct PixelFont;
struct Game;

typedef void (* onUpdate_t)(struct Game *, tnecs_entity, u32, i64, void *);

enum SOTA_TEXT_COMPONENT {
    SOTA_FPS_BUFFER = 8,
};

/* -- Text: Standalone Pixelfont -- */
typedef struct Text {
    char                line[DEFAULT_BUFFER_SIZE];
    SDL_Texture        *texture;
    struct PixelFont   *pixelfont;
    onUpdate_t          onUpdate;
    i64                 update_time_ns; /* 0 means update one time */
    i32                 len;
    b32                 visible;
    b32                 update;
    SDL_Rect            rect;
} Text;
extern const struct Text Text_default;

void Text_Free(         struct Text *t);
void Text_Free_tnecs(   void *t);

void Text_Set(     struct Text *t, char *l, int o);
void Text_Draw(    struct Text *t, SDL_Renderer *r, SDL_Rect *d);
void Text_Update(  struct Text *t, SDL_Renderer *r);
void Text_onUpdate(struct Text *t);

void Text_onUpdate_FPS(struct Game *g, tnecs_entity ent, u32 fc, i64 up, void *data);

#endif /* TEXT_H */
