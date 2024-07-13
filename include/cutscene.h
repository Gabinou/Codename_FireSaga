#ifndef CUTSCENE_H
#define CUTSCENE_H

#include <string.h>
#include "events.h"
#include "types.h"
#include "utilities.h"
#include "filesystem.h"
#include "jsonio.h"
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

    /* -- Post-scene -- */
    // What happens after a cutscene ends?
    //      - Go back to gameplay, preparation
    //          -> event?
    //      - Go back to FirstMenu -> GameOver 
    //          -> event_Quit
    tnecs_entity event;

} Cutscene;
extern struct Cutscene Cutscene_default;
extern struct Cutscene Cutscene_GameOver;


#endif /* CUTSCENE_H */
