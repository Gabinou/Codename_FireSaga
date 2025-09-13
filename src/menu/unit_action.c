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

#include "globals.h"
#include "position.h"
#include "platform.h"
#include "title_screen.h"

#include "game/map.h"
#include "game/game.h"

#include "map/map.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/unit_action.h"

#include "unit/unit.h"

const i32 UAM_Options[UAM_OPTION_NUM] = {
    MENU_OPTION_ITEMS,
    MENU_OPTION_TRADE,
    MENU_OPTION_SEIZE,
    MENU_OPTION_TALK,
    MENU_OPTION_ATTACK,
    MENU_OPTION_STAFF,
    MENU_OPTION_DANCE,
    MENU_OPTION_RESCUE,
    MENU_OPTION_OPEN,
    MENU_OPTION_WAIT
};

UnitActionMenu *UnitActionMenu_Alloc(void) {
    return (ActionMenu_Alloc());
}

void UnitActionMenu_Free(UnitActionMenu *uam, Menu *mc) {
    ActionMenu_Free(uam, mc);
}

/* --- Elem Move --- */
i32 UnitActionMenu_Elem_Move(Menu *mc, i32 direction) {
    return (ActionMenu_Elem_Move(mc, direction));
}

void UnitActionMenu_Elem_Pos(   UnitActionMenu  *uam,
                                Menu            *mc) {
    ActionMenu_Elem_Pos(uam, mc);
}

void UnitActionMenu_Elem_Links( UnitActionMenu  *uam,
                                Menu            *mc) {
    ActionMenu_Elem_Links(uam, mc);
}

void UnitActionMenu_Elem_Boxes( UnitActionMenu  *uam,
                                Menu            *mc) {
    ActionMenu_Elem_Boxes(uam, mc);
}

void UnitActionMenu_Dynamic(UnitActionMenu  *uam,
                            n9Patch         *n9,
                            tnecs_E          unit_E, 
                            Game            *sota) {
    IES_assert(uam                  != NULL);
    Map *map = Game_Map(sota);
    IES_assert(map                  != NULL);
    IES_assert(map->darrs.tilemap   != NULL);

    ActionMenu_Options_Reset(uam);

    /* Items Option is always first. */
    Menu_Option option = {MENU_OPTION_ITEMS, 1};
    ActionMenu_Option_Add(uam, option);

    /* Trade option IF unit to trade with */
    // TODO: Reinsert trade menu when implemented.
    // if (DARR_NUM(sota->targets.passives) > 0)
    // ActionMenu_Option_Add(psm, MENU_OPTION_TRADE, 1));

    /* --- Check if unit can SEIZE --- */
    /* -- Seizable: Check if tile is a throne --  */
    Position *pos = IES_GET_C(gl_world, unit_E, Position);
    IES_assert(pos != NULL);
    i32 index = sota_2D_index(  pos->tilemap_pos.x,
                                pos->tilemap_pos.y,
                                Map_col_len(map));
    i32 tile_ind = map->darrs.tilemap[index] / TILE_DIVISOR;
    b32 isthrone = (tile_ind == TILE_THRONE);

    /* -- Seizable: Check if unit is a main character --  */
    Unit *unit = IES_GET_C(gl_world, unit_E, Unit);
    IES_assert(unit != NULL);

#ifdef DEBUG_SEIZE_ANYONE
    b32 ismainchar = true;
#else
    b32 ismainchar = (Unit_id(unit) == UNIT_ID_ERWIN);
#endif /* DEBUG_SEIZE_ANYONE */

    if (isthrone && ismainchar) {
        option.id = MENU_OPTION_SEIZE;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.auditors) > 0) {
        option.id = MENU_OPTION_TALK;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.defendants) > 0) {
        option.id = MENU_OPTION_ATTACK;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.patients) > 0) {
        option.id = MENU_OPTION_TALK;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.spectators) > 0) {
        option.id = MENU_OPTION_DANCE;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.victims) > 0) {
        option.id = MENU_OPTION_RESCUE;
        ActionMenu_Option_Add(uam, option);
    }
    if (DARR_NUM(sota->targets.openables) > 0) {
        option.id = MENU_OPTION_OPEN;
        ActionMenu_Option_Add(uam, option);
    }

    option.id = MENU_OPTION_WAIT;
    ActionMenu_Option_Add(uam, option);
    ActionMenu_Compute_Size(uam, n9);
}

i32 UnitActionMenu_Options_Num(const UnitActionMenu *uam) {
    return (ActionMenu_Options_Num(uam));
}

void UnitActionMenu_Load(UnitActionMenu *uam, n9Patch *n9) {
    /* Set n9 size, load its textures */
    n9Patch_Free(n9);
    *n9             = n9Patch_default;
    n9->px.x        = MENU_PATCH_PIXELS;
    n9->px.y        = MENU_PATCH_PIXELS;
    n9->scale.x     = UAM_N9PATCH_SCALE_X;
    n9->scale.y     = UAM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * UAM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * UAM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    n9->pos.x           = 0;
    n9->pos.y           = 0;

    pActionMenu_Load(uam->platform, n9);
}

i32 UnitActionMenu_Option_Order(UnitActionMenu *m,
                                i32 option) {
    for (int i = 0; i < UAM_OPTION_NUM; i++) {
        if (UAM_Options[i] == option) {
            return (i);
        }
    }
    return (-1);
}

void UnitActionMenu_Draw(   Menu            *mc,
                            SDL_Texture     *render_target,
                            SDL_Renderer    *renderer) {
    ActionMenu_Draw(mc, render_target, renderer);
}
