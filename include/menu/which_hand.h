
#ifndef WHICH_HAND_H
#define WHICH_HAND_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct n9Patch;
struct Menu;

/* --- Which hand menu--- 
** Items Unit action Equip submenu:
** choose hand in which to equip item.
**  Flow:
**  1. Choose *Items* on unit action menu
**  2. Pick item in equipment
**  3. Choose what to do with item
**      1. *Equip* or *Use*
**      ->  - which hand menu (L, R or 2H)  <- HERE
*/
enum LR2H_MENU {
    LR2H_MENU_WIDTH = 0,
};

struct WhichHandMenu {
    SDL_Texture *texture;

    struct PixelFont *pixelnours;
};



#endif /* WHICH_HAND_H */
