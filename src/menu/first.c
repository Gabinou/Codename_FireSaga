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
** FirstMenu (FM): Core
**
*/

#include "title_screen.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/first.h"

const i32 FM_Options[FM_OPTION_NUM] = {
    MENU_OPTION_DEBUG_MAP,
    MENU_OPTION_NEW_GAME,
    MENU_OPTION_SETTINGS,
};

FirstMenu *FirstMenu_Alloc(void) {
    return (ActionMenu_Alloc());
}

void FirstMenu_Free(FirstMenu *fm, Menu *mc) {
    ActionMenu_Free(fm, mc);
}

void FirstMenu_Load(ActionMenu  *am, n9Patch     *n9) {
    /* Set n9 size, load its textures */
    n9Patch_Free(n9);
    *n9            = n9Patch_default;
    n9->px.x       = MENU_PATCH_PIXELS;
    n9->px.y       = MENU_PATCH_PIXELS;
    n9->scale.x    = SOTA_FIRST_MENU_N9PATCH_SCALE_X;
    n9->scale.y    = SOTA_FIRST_MENU_N9PATCH_SCALE_Y;
    n9->num.x      = SOTA_FIRST_MENU_PATCH_X_SIZE;
    n9->num.y      = SOTA_FIRST_MENU_PATCH_X_SIZE;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    n9->pos.x           = 0;
    n9->pos.y           = 0;

    pActionMenu_Load(am->platform, n9);
}

/* --- Elem Move --- */
i32 FirstMenu_Elem_Move(struct Menu *mc, i32 direction) {
    return (ActionMenu_Elem_Move(mc, direction));
}

void FirstMenu_Elem_Pos(    FirstMenu *fm, Menu *mc) {
    ActionMenu_Elem_Pos(fm, mc);
}

void FirstMenu_Elem_Links(  FirstMenu *fm, Menu *mc) {
    ActionMenu_Elem_Links(fm, mc);
}

void FirstMenu_Elem_Boxes(  FirstMenu *fm, Menu *mc) {
    ActionMenu_Elem_Boxes(fm, mc);
}

void FirstMenu_Dynamic(FirstMenu *fm, n9Patch *n9) {
    /* -- Put all options in FirstMenu -- */
    ActionMenu_Options_Reset(fm);
    Menu_Option option = {MENU_OPTION_DEBUG_MAP, 1};
    ActionMenu_Option_Add(fm, option);
    option.id = MENU_OPTION_NEW_GAME;
    ActionMenu_Option_Add(fm, option);
    option.id = MENU_OPTION_SETTINGS;
    ActionMenu_Option_Add(fm, option);
    ActionMenu_Compute_Size(fm, n9);
}

i32 FirstMenu_Options_Num(const FirstMenu *fm) {
    return (ActionMenu_Options_Num(fm));
}

void FirstMenu_Draw(Menu            *mc,
                    SDL_Texture     *render_target,
                    SDL_Renderer    *renderer) {
    ActionMenu_Draw(mc, render_target, renderer);
}

