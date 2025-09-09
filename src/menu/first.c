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
    ActionMenu *am = IES_malloc(sizeof(ActionMenu));
    *am = ActionMenu_default;
    IES_assert(am);

    return (am);
}

void ActionMenu_Free(ActionMenu *am, Menu *mc) {
    IES_assert(am);
    IES_assert(mc);
    Menu_Free(mc);
    if (am->texture != NULL) {
        SDL_DestroyTexture(am->texture);
        am->texture = NULL;
    }
    IES_free(am);
}

void FirstMenu_Load(ActionMenu  *am,
                    n9Patch     *n9) {
    /* Set n9 size, load its textures */
    /* TODO: use ActionMenu_Load */
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

    am->row_height = ASCII_GLYPH_HEIGHT + 2; /* pixel fonts have python8 pixels*/

    pActionMenu_Load(am->platform, n9);
}

/* --- Elem Move --- */
i32 ActionMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

void ActionMenu_Options_Reset(ActionMenu *am) {
    am->option_num = 0;
}

int ActionMenu_Option_Index(ActionMenu *am, i32 option) {
    int out = -1;
    i32 num = AM_Options_Num(am);
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
    IES_assert(am);
    IES_assert(option.id > MENU_OPTION_START);
    IES_assert(option.id < MENU_OPTION_END);
    IES_assert(am->option_num >= 0);

    /* - Skip if no more options - */
    if (am->option_num >= SOTA_MAX_MENU_OPTIONS) {
        IES_assert(0);
        return;
    }

    /* - adding option - */

    /* - Computing option width, check if increase menu width - */
    IES_assert(option.name.data != NULL);
    s8 name = Menu_Option_Name(option.id);
    int text_width  = PixelFont_Width(  am->pixelnours, name.data,
                                        name.num);
    int padding = Margin_XY(am->menu_padding);
    if ((text_width + padding) > am->text_width) {
        am->text_width = text_width + padding;
    }

    option.enabled = enabled;

    am->options[am->option_num++] =  option;
}

void ActionMenu_Compute_Size( ActionMenu    *am,
                              n9Patch             *n9) {
    /* - Compute patch sizes from text - */
    struct Padding mp = am->menu_padding;
    i32 num = AM_Options_Num(am);
    int text_height = mp.top + mp.bottom + am->row_height * num;
    Point content = {am->text_width, text_height};
    n9Patch_Fit(n9, content);

    /* - Destroy texture because it does not fit new size - */
    SDL_DestroyTexture(am->texture);
    am->texture = NULL;
}

void ActionMenu_Elem_Links(   ActionMenu *am,
                              Menu *mc) {
    if (mc->elem_links != NULL)
        IES_free(mc->elem_links);
    i32 num = AM_Options_Num(am);
    IES_assert(mc->elem_num == num);
    mc->elem_links = IES_malloc(num * sizeof(*mc->elem_links));
    for (i32 i = 0; i < num; i++) {
        mc->elem_links[i] = n4Directions_default;
        if (i < (num - 1))
            mc->elem_links[i].bottom = i + 1;
        if (i > 0)
            mc->elem_links[i].top = i - 1;
    }
}

void ActionMenu_Cursor_Boxes( ActionMenu *m,
                              Menu *mc) {

}

void ActionMenu_Cursor_Pos(   ActionMenu *m,
                              Menu *mc) {

}

void ActionMenu_Elem_Boxes(   ActionMenu *am,
                              Menu *mc) {
    if (mc->elem_box != NULL)
        IES_free(mc->elem_box);
    IES_assert(mc->elem_num > 0);
    mc->elem_box = IES_malloc(mc->elem_num * sizeof(*mc->elem_box));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_box[i].x = SOTA_TILESIZE;
        mc->elem_box[i].y = SOTA_TILESIZE;
    }
}

void ActionMenu_Elem_Pos( ActionMenu    *am,
                          Menu                *mc) {
    struct Padding mp = am->menu_padding;
    struct Point pos9 = mc->n9.pos, scale = mc->n9.scale;

    if (mc->elem_pos != NULL)
        IES_free(mc->elem_pos);
    mc->elem_pos = SDL_calloc(mc->elem_num, sizeof(*mc->elem_pos));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = am->pos.x + pos9.x + mp.left * scale.x;
        mc->elem_pos[i].y = am->pos.y + (pos9.y + ((i * am->row_height + mp.top))) * scale.y;
    }
}

void ActionMenu_Draw( Menu            *mc,
                      SDL_Texture     *render_target,
                      SDL_Renderer    *renderer) {
    struct ActionMenu *am = (struct ActionMenu *)mc->data;
    struct n9Patch *n9 = &mc->n9;

    IES_assert(am != NULL);
    IES_assert(n9->pos.x == 0);
    IES_assert(n9->pos.y == 0);
    if (am->update) {
        ActionMenu_Update(am, n9, render_target, renderer);
        am->update = false;
    }
    IES_assert(n9->pos.x == 0);
    IES_assert(n9->pos.y == 0);

    /* TODO: set position of ActionMenu_menu */
    Point size = n9Patch_Pixels_Total(n9);
    SDL_Rect dstrect = {
        .w = size.x * n9->scale.x,
        .h = size.y * n9->scale.y,
        .x = am->pos.x,
        .y = am->pos.y,
    };
    IES_assert(am->texture != NULL);
    SDL_RenderCopy(renderer, am->texture, NULL, &dstrect);
}

void ActionMenu_Update(   ActionMenu    *am,
                          n9Patch             *n9,
                          SDL_Texture         *render_target,
                          SDL_Renderer        *renderer) {
    /* --- PRELIMINARIES --- */
    IES_assert(am              != NULL);
    IES_assert(renderer         != NULL);
    IES_assert(am->pixelnours  != NULL);
    IES_assert(am->options     != NULL);

    /* - variable declaration/ants definition - */
    Point size = n9Patch_Pixels_Total(n9);
    IES_assert(size.x > 0);
    IES_assert(size.y > 0);
    IES_assert(n9->scale.x > 0);
    IES_assert(n9->scale.y > 0);

    /* - create render target texture - */
    if (am->texture == NULL) {
        am->texture = SDL_CreateTexture(renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.x, size.y);
        IES_assert(am->texture != NULL);
        SDL_SetTextureBlendMode(am->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, am->texture);

    /* Clear the target to our selected color. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    IES_assert(am->texture != NULL);
    /* --- RENDERING STATS-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9->scale.x;
    int scale_y = n9->scale.y;
    n9->scale.x = 1;
    n9->scale.y = 1;
    n9->pos.x = 0;
    n9->pos.y = 0;
    n9Patch_Draw(n9, renderer);
    n9->scale.x = scale_x;
    n9->scale.y = scale_y;

    i32 posx = n9->pos.x + am->menu_padding.left, posy;
    // int total_text_height = am->option_num * am->row_height +  n9->pos.y + am->menu_padding.top;
    // int shift_y = (n9->num.y * n9->px.y) - total_text_height;
    // shift_y /= 2;

    i32 num = AM_Options_Num(am);
    for (i32 i = 0; i < num; i++) {
        posy = n9->pos.y + am->menu_padding.top + (i * am->row_height);
        PixelFont_Write(am->pixelnours, renderer,
                        am->options[i].name.data,
                        am->options[i].name.len,
                        posx, posy);
    }
    am->update = false;
    // Filesystem_Texture_Dump("ActionMenu.png", renderer, am->texture, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetRenderTarget(renderer, render_target);
}

void makeContent_FirstMenu(struct Game *sota) {
    tnecs_E menu_entity = sota->title_screen.menu;
    IES_assert(menu_entity > 0);
    struct Menu *mc;
    mc = IES_GET_C(gl_world, menu_entity, Menu);
    IES_assert(mc != NULL);
    struct ActionMenu *am = mc->data;
    IES_assert(am != NULL);
    ActionMenu_Options_Reset(am);
    ActionMenu_Option_Add(am, MENU_OPTION_DEBUG_MAP, 1);
    ActionMenu_Option_Add(am, MENU_OPTION_NEW_GAME,  1);
    ActionMenu_Option_Add(am, MENU_OPTION_SETTINGS,  1);
    ActionMenu_Compute_Size(am, &mc->n9);
}

void makeContent_AM_TRADE(struct Game *sota, void *data1, void *data2) {
    // tnecs_E menu_entity = sota->menus.ACTION[MENU_ActionMenu_TRADE];
    // struct Menu *mc = IES_GET_C(gl_world, menu_entity, Menu);
    // struct ActionMenu *am = mc->data;
}

i32 AM_Options_Num(const ActionMenu *am) {
    if (am == NULL) {
        return 0;
    }
    if (am->options == NULL) {
        return 0;
    }

    return (DARR_NUM(am->options));
}

s8 Menu_Option_Name(i32 id) {
    return (menuOptionnames[opt_id]);
}
