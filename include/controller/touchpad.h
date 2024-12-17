#ifndef CONTROLLERTOUCHPAD_H
#define CONTROLLERTOUCHPAD_H

#include "types.h"
#include "SDL.h"

typedef struct controllerTouchpad {
    SDL_TouchID touchID;
    i32 controller_type;

} controllerTouchpad;
// extern const struct controllerTouchpad controllerTouchpad_default;

#endif /* CONTROLLERTOUCHPAD_H */
