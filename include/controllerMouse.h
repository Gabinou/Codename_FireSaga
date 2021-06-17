#ifndef CONTROLLERMOUSE_H
#define CONTROLLERMOUSE_H

#include <math.h>
#include "narrative.h"

typedef struct controllerMouse {
    struct MouseInputMap * inputmap;
} controllerMouse;
extern struct controllerMouse controllerMouse_default;

#endif /* CONTROLLERMOUSE_H */
