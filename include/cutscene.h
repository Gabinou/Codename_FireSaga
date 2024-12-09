#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "types.h"
#include "structs.h"
#include "nstr.h"
#include "tnecs.h"
#include "SDL.h"

/* -------------------------------- Cutscene -------------------------------- */
//  An animation
//      - Sprites, text moving around
//      - Non-interactive (Player can skip scene)

typedef struct Cutscene {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

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
extern struct Cutscene Cutscene_default;
extern struct Cutscene Cutscene_GameOver;

void Cutscene_Free(Cutscene *cutscene);
void Cutscene_Init(Cutscene *cutscene);

void Cutscene_Finish(Cutscene *cutscene, Game *sota);

#endif /* CUTSCENE_H */
