#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "types.h"
#include "nstr.h"
#include "tnecs.h"
#include "jsonio.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

/* -------------------------------- Cutscene -------------------------------- */
//  An animation
//      - Sprites, text moving around
//      - Non-interactive (Player can skip scene)

typedef struct Cutscene {
    struct jsonIO_Header jsonio_header;

    /* Time cutscene plays */
    u64 time_ns;

    /* -- Post-scene -- */
    // What happens after a cutscene ends?
    //      - Go back to gameplay, preparation
    //          -> event?
    //      - GameOver cutscene: Go back to FirstMenu
    //          -> event_Quit
    tnecs_entity event;

} Cutscene;
extern const struct Cutscene Cutscene_default;
extern const struct Cutscene Cutscene_GameOver;

void Cutscene_Free(Cutscene *cutscene);
void Cutscene_Init(Cutscene *cutscene);
void Cutscene_Init_tnecs(void *voidcutscene);

void Cutscene_Finish(Cutscene *cutscene, struct Game *sota);

#endif /* CUTSCENE_H */
