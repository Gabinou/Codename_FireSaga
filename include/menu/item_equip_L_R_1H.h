
#ifndef ITEM_EQUIP_L_R_1H_H
#define ITEM_EQUIP_L_R_1H_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct n9Patch;
struct Menu;

/* Items Unit action Equip submenu:
** choose hand in which to equip item.
**  Flow:
**  1. Choose *Items* on unit action menu
**  2. Pick item in equipment
**  3. Choose what to do with item
**      1. *Equip*
**      ->  - L, R or 2H menu <- HERE
*/
enum LR2H_MENU {
    LR2H_MENU_WIDTH = 0,
};

struct ItemEquipLR2HMenu {

};

#endif /* ITEM_EQUIP_L_R_1H_H */
