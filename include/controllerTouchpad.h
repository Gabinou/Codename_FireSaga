#ifndef CONTROLLERTOUCHPAD_H
#define CONTROLLERTOUCHPAD_H

#include <math.h>
#include "SDL2/SDL.h"

typedef struct controllerTouchpad {
    SDL_TouchID touchID;
} controllerTouchpad;
extern struct controllerTouchpad controllerTouchpad_default;


#endif /* CONTROLLERTOUCHPAD_H */
