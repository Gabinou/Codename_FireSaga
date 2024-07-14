#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <string.h>
#include "events.h"
#include "types.h"
#include "utilities.h"
#include "filesystem.h"
#include "jsonio.h"
#include "structs.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "nmath.h"
#include "cJSON.h"
#include "bitfields.h"
#include "SDL2/SDL.h"

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
