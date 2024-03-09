#ifndef CREDITS_H
#define CREDITS_H

#include "enums.h"

enum CREDITS {
    CREDIT_DEVS             = 1,
    CREDIT_SPECIAL_THANKS   = 1,
};

extern char names_devs[CREDIT_SPECIAL_THANKS][DEFAULT_BUFFER_SIZE / 4];
extern char roles_devs[CREDIT_SPECIAL_THANKS][DEFAULT_BUFFER_SIZE];
extern char names_special_thanks[CREDIT_SPECIAL_THANKS][DEFAULT_BUFFER_SIZE / 4];
extern char roles_special_thanks[CREDIT_SPECIAL_THANKS][DEFAULT_BUFFER_SIZE];

#endif /* CREDITS_H */
