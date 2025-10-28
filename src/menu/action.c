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
** ActionMenu (AM): Core
**
*/

#include "item.h"
#include "nmath.h"
#include "names.h"
#include "weapon.h"
#include "macros.h"
#include "globals.h"
#include "platform.h"
#include "position.h"
#include "pixelfonts.h"
#include "filesystem.h"
#include "title_screen.h"

#include "game/map.h"
#include "game/menu.h"
#include "game/game.h"
#include "game/popup.h"

#include "map/map.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/first.h"

const struct ActionMenu ActionMenu_default = {
    .row_height     = ASCII_GLYPH_HEIGHT,
    .menu_padding   = {
        AM_PADDING_RIGHT,
        AM_PADDING_TOP,
        AM_PADDING_LEFT,
        AM_PADDING_BOTTOM
    },
    .update         = true,
    .icon_width     = 32,
};

ActionMenu *ActionMenu_Alloc(void) {
    ActionMenu *am  = IES_malloc(sizeof(ActionMenu));
    *am = ActionMenu_default;
    am->platform    = pActionMenu_Alloc();
    IES_assert(am);

    return (am);
}

void ActionMenu_Free(ActionMenu *am, Menu *mc) {
    if (am == NULL) {
        IES_assert(0);
        return;
    }
    if (mc == NULL) {
        IES_assert(0);
        return;
    }
    Menu_Free(mc);
    pActionMenu_Free(am->platform);
    am->platform = NULL;
    IES_free(am);
}

void ActionMenu_Load(ActionMenu *am, n9Patch *n9) {
    n9Patch_Free(n9);
    *n9            = n9Patch_default;
    n9->px.x       = MENU_PATCH_PIXELS;
    n9->px.y       = MENU_PATCH_PIXELS;
    n9->scale.x    = AM_N9PATCH_SCALE_X;
    n9->scale.y    = AM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    n9->pos.x           = 0;
    n9->pos.y           = 0;

    /* pixel fonts have python8 pixels*/
    am->row_height = ASCII_GLYPH_HEIGHT + 2;

    pActionMenu_Load(am->platform, n9);
    am->update = true;
}

/* --- Elem Move --- */
i32 ActionMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction,
                               0, mc->elem_num));
}

void ActionMenu_Options_Reset(ActionMenu *am) {
    am->option_num  = 0;
    am->update      = true;
}

int ActionMenu_Option_Index(ActionMenu *am, i32 option) {
    int out = -1;
    i32 num = ActionMenu_Options_Num(am);
    IES_assert(num > 0);
    IES_assert(num < SOTA_MAX_MENU_OPTIONS);
    i32 loop = num < SOTA_MAX_MENU_OPTIONS ? num : SOTA_MAX_MENU_OPTIONS;
    for (i32 i = 0; i < loop; i++) {
        if (am->options[i].id == option) {
            out = i;
            break;
        }
    }
    return (out);
}

void ActionMenu_Option_Add(ActionMenu *am, Menu_Option option) {
    if (am == NULL) {
        IES_assert(0);
        return;
    }
    IES_assert(option.id > MENU_OPTION_START);
    IES_assert(option.id < MENU_OPTION_END);
    IES_assert(am->option_num >= 0);

    /* - Skip if no more options - */
    if (am->option_num >= SOTA_MAX_MENU_OPTIONS) {
        IES_assert(0);
        return;
    }

    /* -- adding option -- */

    /* - Computing option width, check if increase menu width - */
    s8 name = Menu_Option_Name(option.id);
    IES_assert(name.data != NULL);
    int text_width  = PixelFont_Width(  am->pixelnours, name.data,
                                        name.num);
    Point padding = Margin_XY(am->menu_padding);
    if ((text_width + padding.x) > am->text_width) {
        am->text_width = text_width + padding.x;
    }

    am->options[am->option_num++] =  option;
    am->update = true;
}

void ActionMenu_Compute_Size( ActionMenu *am, n9Patch *n9) {
    /* - Compute patch sizes from text - */
    Padding mp = am->menu_padding;
    i32 num = ActionMenu_Options_Num(am);
    int text_height = mp.top + mp.bottom + am->row_height * num;
    Point content = {am->text_width, text_height};
    n9Patch_Fit(n9, content);

    /* - Destroy texture, it does not fit new size - */
    pActionMenu_Free_Texture(am->platform);
}

void ActionMenu_Elem_Links(ActionMenu *am, Menu *mc) {
    if (mc->elem_links != NULL) {
        IES_free(mc->elem_links);
    }
    i32 num = ActionMenu_Options_Num(am);
    IES_assert(mc->elem_num == num);
    mc->elem_links = IES_malloc(num * sizeof(*mc->elem_links));
    for (i32 i = 0; i < num; i++) {
        mc->elem_links[i] = n4Directions_default;
        if (i < (num - 1))
            mc->elem_links[i].bottom    = i + 1;
        if (i > 0)
            mc->elem_links[i].top       = i - 1;
    }
}

void ActionMenu_Elem_Boxes(ActionMenu *am, Menu *mc) {
    if (mc->elem_box != NULL)
        IES_free(mc->elem_box);
    IES_assert(mc->elem_num > 0);
    mc->elem_box = IES_malloc(mc->elem_num * sizeof(*mc->elem_box));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_box[i].x = SOTA_TILESIZE;
        mc->elem_box[i].y = SOTA_TILESIZE;
    }
}

void ActionMenu_Elem_Pos(ActionMenu *am, Menu *mc) {
    Padding mp  = am->menu_padding;
    Point pos9  = mc->n9patch.pos;
    Point scale = mc->n9patch.scale;

    if (mc->elem_pos != NULL) {
        IES_free(mc->elem_pos);
    }
    mc->elem_pos = IES_calloc(mc->elem_num, sizeof(*mc->elem_pos));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = am->pos.x + pos9.x + mp.left * scale.x;
        mc->elem_pos[i].y = am->pos.y + (pos9.y + ((i * am->row_height + mp.top))) * scale.y;
    }
}

i32 ActionMenu_Options_Num(const ActionMenu *am) {
    if (am == NULL) {
        return 0;
    }
    if (am->options == NULL) {
        return 0;
    }

    return (am->option_num);
}

s8 Menu_Option_Name(i32 id) {
    IES_assert(menuOptionnames != NULL);
    IES_assert(id >= MENU_OPTION_START);
    IES_assert(id < MENU_OPTION_END);
    return (menuOptionnames[id]);
}


void ActionMenu_Draw(   Menu            *mc,
                        SDL_Texture     *render_target,
                        SDL_Renderer    *renderer) {
    if (mc == NULL) {
        IES_assert(0);
        return;
    }
    IES_assert(mc->type == MENU_TYPE_ACTION);
    ActionMenu *am = mc->data;
    if (am == NULL) {
        IES_assert(0);
        return;
    }

    _ActionMenu_Draw(   am, &mc->n9patch,
                        render_target, renderer);
}

void _ActionMenu_Draw(  ActionMenu      *am,
                        n9Patch         *n9patch,
                        SDL_Texture     *render_target,
                        SDL_Renderer    *renderer) {

    pActionMenu_Set(am->platform, render_target,
                    renderer);
    pActionMenu_Draw(am, n9patch);
}
