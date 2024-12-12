#ifndef TITLE_PAGE_H
#define TITLE_PAGE_H

#include "enums.h"

enum TITLE_PAGE {
    TITLE_PAGE_NULL = -1,
};

enum TITLE_PAGE_ELEMS {
    TITLE_PAGE_ELEMS_NULL = -1,
};

enum SOTA_FIRST_MENU {
    SOTA_FIRST_MENU_PATCH_X_SIZE    = 8,
    SOTA_FIRST_MENU_PATCH_Y_SIZE    = 4,
    SOTA_FIRST_MENU_N9PATCH_SCALE_X = 6,
    SOTA_FIRST_MENU_N9PATCH_SCALE_Y = 6,
};

enum SOTA_FIRST_MENU_ELEMS {
    SOTA_FIRST_MENU_ELEMS_NULL  = -1,
    SOTA_FIRST_MENU_LOAD        = 0,
    SOTA_FIRST_MENU_ERASE,
    SOTA_FIRST_MENU_COPY,
    SOTA_FIRST_MENU_NEW_GAME,
    SOTA_FIRST_MENU_OPTIONS,
    SOTA_FIRST_MENU_EXTRAS,
    SOTA_FIRST_MENU_DEBUG_MAP,
};

#endif /* TITLE_PAGE_H */
