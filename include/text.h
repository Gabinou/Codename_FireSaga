#ifndef TEXT_H
#define TEXT_H

#include "structs.h"
#include "types.h"
#include "enums.h"
#include "tnecs.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct PixelFont;
struct Position;
struct Game;

typedef void (* onUpdate_t)(struct Game *, tnecs_entity, void *);

enum SOTA_TEXT_COMPONENT {
    SOTA_FPS_BUFFER = 8,
};

/* -- Text: Standalone Pixelfont -- */
struct P_Text;
typedef struct Text {
    char                line[DEFAULT_BUFFER_SIZE];
    struct PixelFont   *pixelfont;
    onUpdate_t          onUpdate;
    i64                 update_time_ns;
    i32                 len;
    b32                 visible;
    b32                 update;
    Rect                dstrect;
    Point               size;

    /* -- Platform-code -- */
    struct P_Text      *plat;
    /* SDL_Texture        *texture; */
} Text;
extern const struct Text Text_default;

/* --- Init --- */
void Text_Init(struct Text *t);
/* --- tnecs: component auto init --- */
void Text_Init_tnecs(   void *t);

/* --- Free --- */
void Text_Free(         struct Text *t);
void Text_Free_tnecs(   void *t);

/* --- Text setting --- */
void Text_Set(struct Text *t, char *l, int o);
void Text_Place(struct Text *t,
                const struct Position *const p);

/* -- onUpdate -- */
void Text_onUpdate_FPS(struct Game  *g,
                       tnecs_entity  ent,
                       void         *data);

/* --- PLATFORM: Rendering  --- */
void P_Text_Free(  struct P_Text *t);

void Text_Draw(    struct Text *t, SDL_Renderer *r);
void Text_Update(  struct Text *t, SDL_Renderer *r);


#endif /* TEXT_H */
