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
** MapActionMenu (MAM): Core
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
#include "menu/map_action.h"

#include "unit/unit.h"

const i32 MAM_Options[MAM_OPTION_NUM] = {
    MENU_OPTION_UNITS,
    MENU_OPTION_CONVOY,
    MENU_OPTION_SETTINGS,
    MENU_OPTION_QUIT,
    MENU_OPTION_END_TURN
};

MapActionMenu *MapActionMenu_Alloc(void) {
    return (ActionMenu_Alloc());
}

void MapActionMenu_Free(MapActionMenu *mam, Menu *mc) {
    ActionMenu_Free(mam, mc);
}

/* --- Elem Move --- */
i32 MapActionMenu_Elem_Move(struct Menu *mc, i32 direction) {
    return (ActionMenu_Elem_Move(mc, direction));
}

void MapActionMenu_Elem_Pos(   MapActionMenu *mam,
                               Menu *mc) {
    ActionMenu_Elem_Pos(mam, mc);
}

void MapActionMenu_Elem_Links( MapActionMenu *mam,
                               Menu *mc) {
    ActionMenu_Elem_Links(mam, mc);
}

void MapActionMenu_Elem_Boxes( MapActionMenu *mam,
                               Menu *mc) {
    ActionMenu_Elem_Boxes(mam, mc);
}

void MapActionMenu_Dynamic( MapActionMenu *mam, n9Patch *n9) {
    IES_assert(mam                  != NULL);
    ActionMenu_Options_Reset(mam);

    Menu_Option option = {MENU_OPTION_UNITS, 1};
    ActionMenu_Option_Add(mam, option);
    option.id = MENU_OPTION_CONVOY;
    ActionMenu_Option_Add(mam, option);
    option.id = MENU_OPTION_SETTINGS;
    ActionMenu_Option_Add(mam, option);
    option.id = MENU_OPTION_QUIT;
    ActionMenu_Option_Add(mam, option);
    option.id = MENU_OPTION_END_TURN;
    ActionMenu_Option_Add(mam, option);
    ActionMenu_Compute_Size(mam, n9);
}

i32 MapActionMenu_Options_Num(const MapActionMenu *mam) {
    return (ActionMenu_Options_Num(mam));
}

void MapActionMenu_Load(MapActionMenu *mam, n9Patch *n9) {
    /* Set n9 size, load its textures */
    n9Patch_Free(n9);
    *n9             = n9Patch_default;
    n9->px.x        = MENU_PATCH_PIXELS;
    n9->px.y        = MENU_PATCH_PIXELS;
    n9->scale.x     = MAM_N9PATCH_SCALE_X;
    n9->scale.y     = MAM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * MAM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * MAM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    n9->pos.x           = 0;
    n9->pos.y           = 0;

    pActionMenu_Load(mam->platform, n9);
}

i32 MapActionMenu_Option_Order(MapActionMenu *m, i32 option) {
    for (int i = 0; i < MAM_OPTION_NUM; i++) {
        if (MAM_Options[i] == option) {
            return (i);
        }
    }
    return (-1);
}

void MapActionMenu_Draw(Menu            *mc,
                        SDL_Texture     *render_target,
                        SDL_Renderer    *renderer) {
    if (mc == NULL) {
        IES_assert(0);
        return;
    }
    IES_assert(mc->type == MENU_TYPE_MAP_ACTION);
    ActionMenu *am = mc->data;
    if (am == NULL) {
        IES_assert(0);
        return;
    }

    _ActionMenu_Draw(   am, &mc->n9patch,
                        render_target, renderer);
}
