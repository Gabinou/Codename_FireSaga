/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** UnitActionMenu (UAM): Core
**
*/

#include "title_screen.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/unit_action.h"

const i32 UAM_Options[UAM_OPTION_NUM] = {
    MENU_OPTION_DEBUG_MAP,
    MENU_OPTION_NEW_GAME,
    MENU_OPTION_SETTINGS,
};

UnitActionMenu *UnitActionMenu_Alloc(void) {
    return (ActionMenu_Alloc());
}

void UnitActionMenu_Free(UnitActionMenu *fm, Menu *mc) {
    ActionMenu_Free(fm, mc);
}

/* --- Elem Move --- */
i32 UnitActionMenu_Elem_Move(struct Menu *mc, i32 direction) {
    return (ActionMenu_Elem_Move(mc, direction));
}

void UnitActionMenu_Elem_Pos(    UnitActionMenu *fm, Menu *mc) {
    ActionMenu_Elem_Pos(fm, mc);
}

void UnitActionMenu_Elem_Links(  UnitActionMenu *fm, Menu *mc) {
    ActionMenu_Elem_Links(fm, mc);
}

void UnitActionMenu_Elem_Boxes(  UnitActionMenu *fm, Menu *mc) {
    ActionMenu_Elem_Boxes(fm, mc);
}


void UnitActionMenu_Dynamic(UnitActionMenu *uam, struct n9Patch  *n9) {
}