#ifndef CONTROLLERTOUCHPAD_H
#define CONTROLLERTOUCHPAD_H

#include <math.h>
#include "types.h"
#include "SDL2/SDL.h"

typedef struct controllerTouchpad {
    SDL_TouchID touchID;
    if32 controller_type;

} controllerTouchpad;
extern struct controllerTouchpad controllerTouchpad_default;

#endif /* CONTROLLERTOUCHPAD_H */
