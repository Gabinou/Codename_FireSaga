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
** FirstMenu (FM): On Title screen, first choice
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
    ActionMenu *psm = SDL_malloc(sizeof(ActionMenu));
    *psm = ActionMenu_default;
    SDL_assert(psm);

    return (psm);
}

void ActionMenu_Free(ActionMenu *psm, Menu *mc) {
    SDL_assert(psm);
    SDL_assert(mc);
    Menu_Free(mc);
    if (psm->texture != NULL) {
        SDL_DestroyTexture(psm->texture);
        psm->texture = NULL;
    }
    SDL_free(psm);
}

void FirstMenu_Load(ActionMenu  *psm,       SDL_Renderer *renderer, 
                    n9Patch     *n9patch) {
    /* Set n9patch size, load its textures */
    /* TODO: use ActionMenu_Load */
    n9Patch_Free(n9patch);
    *n9patch            = n9Patch_default;
    n9patch->px.x       = MENU_PATCH_PIXELS;
    n9patch->px.y       = MENU_PATCH_PIXELS;
    n9patch->scale.x    = SOTA_FIRST_MENU_N9PATCH_SCALE_X;
    n9patch->scale.y    = SOTA_FIRST_MENU_N9PATCH_SCALE_Y;
    n9patch->num.x      = SOTA_FIRST_MENU_PATCH_X_SIZE;
    n9patch->num.y      = SOTA_FIRST_MENU_PATCH_X_SIZE;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN( "..", "assets", "GUI", 
                                "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load( renderer, path,
                                                    SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);

}

void ActionMenu_Load(ActionMenu *psm, SDL_Renderer *renderer,
                    n9Patch *n9patch) {
    n9Patch_Free(n9patch);

    *n9patch            = n9Patch_default;
    n9patch->px.x       = MENU_PATCH_PIXELS;
    n9patch->px.y       = MENU_PATCH_PIXELS;
    n9patch->scale.x    = AM_N9PATCH_SCALE_X;
    n9patch->scale.y    = AM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    psm->row_height = ASCII_GLYPH_HEIGHT + 2; /* pixel fonts have python8 pixels*/
    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN( "..", "assets", "GUI", 
                                "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load( renderer, path,
                                                    SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);
}

/* --- Elem Move --- */
i32 ActionMenu_Elem_Move(Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

void ActionMenu_Options_Reset(Action *am) {
    am->option_num = 0;
}

int ActionMenu_Option_Index(Action *psm, i32 option) {
    int out = -1;
    i32 num = AM_Options_Num(psm);
    SDL_assert(num > 0);
    SDL_assert(num < SOTA_MAX_MENU_OPTIONS);
    i32 loop = num < SOTA_MAX_MENU_OPTIONS ? num : SOTA_MAX_MENU_OPTIONS;  
    for (i32 i = 0; i < loop; i++) {
        if (psm->options[i].id == option) {
            out = i;
            break;
        }
    }
    return (out);
}

void ActionMenu_Option_Add(ActionMenu *am, Menu_Option option) {
    SDL_assert(am);
    SDL_assert(option.id > MENU_OPTION_START);
    SDL_assert(option.id < MENU_OPTION_END);
    SDL_assert(am->option_num >= 0);

    /* - Skip if no more options - */
    if (am->option_num >= SOTA_MAX_MENU_OPTIONS) {
        SDL_assert(0);
        return;
    }

    /* - adding option - */

    /* - Computing option width, check if increase menu width - */
    SDL_assert(option.name.data != NULL);
    s8 name = Menu_Option_Name(option.id);
    int text_width  = PixelFont_Width(  psm->pixelnours, name.data,
                                        name.num);
    int padding = Margin_XY(psm->menu_padding);
    if ((text_width + padding) > psm->text_width) {
        psm->text_width = text_width + padding;
    }

    option.enabled = enabled;

    psm->options[am->option_num++] =  option;
}

void ActionMenu_Compute_Size( Action    *psm,
                                    n9Patch             *n9patch) {
    /* - Compute patch sizes from text - */
    struct Padding mp = psm->menu_padding;
    i32 num = AM_Options_Num(psm);
    int text_height = mp.top + mp.bottom + psm->row_height * num;
    Point content = {psm->text_width, text_height};
    n9Patch_Fit(n9patch, content);

    /* - Destroy texture because it does not fit new size - */
    SDL_DestroyTexture(psm->texture);
    psm->texture = NULL;
}

void ActionMenu_Elem_Links(   Action *psm,
                                    Menu *mc) {
    if (mc->elem_links != NULL)
        SDL_free(mc->elem_links);
    i32 num = AM_Options_Num(psm);
    SDL_assert(mc->elem_num == num);
    mc->elem_links = SDL_malloc(num * sizeof(*mc->elem_links));
    for (i32 i = 0; i < num; i++) {
        mc->elem_links[i] = n4Directions_default;
        if (i < (num - 1))
            mc->elem_links[i].bottom = i + 1;
        if (i > 0)
            mc->elem_links[i].top = i - 1;
    }
}

void ActionMenu_Cursor_Boxes( Action *m,
                                    Menu *mc) {

}

void ActionMenu_Cursor_Pos(   Action *m,
                                    Menu *mc) {

}

void ActionMenu_Elem_Boxes(   Action *psm,
                                    Menu *mc) {
    if (mc->elem_box != NULL)
        SDL_free(mc->elem_box);
    SDL_assert(mc->elem_num > 0);
    mc->elem_box = SDL_malloc(mc->elem_num * sizeof(*mc->elem_box));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_box[i].x = SOTA_TILESIZE;
        mc->elem_box[i].y = SOTA_TILESIZE;
    }
}

void ActionMenu_Elem_Pos( Action    *psm,
                                Menu                *mc) {
    struct Padding mp = psm->menu_padding;
    struct Point pos9 = mc->n9patch.pos, scale = mc->n9patch.scale;

    if (mc->elem_pos != NULL)
        SDL_free(mc->elem_pos);
    mc->elem_pos = SDL_calloc(mc->elem_num, sizeof(*mc->elem_pos));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = psm->pos.x + pos9.x + mp.left * scale.x;
        mc->elem_pos[i].y = psm->pos.y + (pos9.y + ((i * psm->row_height + mp.top))) * scale.y;
    }
}

void ActionMenu_Draw( Menu            *mc,
                            SDL_Texture     *render_target,
                            SDL_Renderer    *renderer) {
    struct ActionMenu *psm = (struct ActionMenu *)mc->data;
    struct n9Patch *n9patch = &mc->n9patch;

    SDL_assert(psm != NULL);
    SDL_assert(n9patch->pos.x == 0);
    SDL_assert(n9patch->pos.y == 0);
    if (psm->update) {
        ActionMenu_Update(psm, n9patch, render_target, renderer);
        psm->update = false;
    }
    SDL_assert(n9patch->pos.x == 0);
    SDL_assert(n9patch->pos.y == 0);

    /* TODO: set position of ActionMenu_menu */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = psm->pos.x,
        .y = psm->pos.y,
    };
    SDL_assert(psm->texture != NULL);
    SDL_RenderCopy(renderer, psm->texture, NULL, &dstrect);
}

void ActionMenu_Update(   Action    *psm,
                                n9Patch             *n9patch,
                                SDL_Texture         *render_target,
                                SDL_Renderer        *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(psm              != NULL);
    SDL_assert(renderer         != NULL);
    SDL_assert(psm->pixelnours  != NULL);
    SDL_assert(psm->options     != NULL);

    /* - variable declaration/ants definition - */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);

    /* - create render target texture - */
    if (psm->texture == NULL) {
        psm->texture = SDL_CreateTexture(renderer,
                                         SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET,
                                         size.x, size.y);
        SDL_assert(psm->texture != NULL);
        SDL_SetTextureBlendMode(psm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, psm->texture);

    /* Clear the target to our selected color. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

    SDL_assert(psm->texture != NULL);
    /* --- RENDERING STATS-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9patch->scale.x;
    int scale_y = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x = 0;
    n9patch->pos.y = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    i32 posx = n9patch->pos.x + psm->menu_padding.left, posy;
    // int total_text_height = psm->option_num * psm->row_height +  n9patch->pos.y + psm->menu_padding.top;
    // int shift_y = (n9patch->num.y * n9patch->px.y) - total_text_height;
    // shift_y /= 2;

    i32 num = AM_Options_Num(psm);
    for (i32 i = 0; i < num; i++) {
        posy = n9patch->pos.y + psm->menu_padding.top + (i * psm->row_height);
        PixelFont_Write(psm->pixelnours, renderer,
                        psm->options[i].name.data,
                        psm->options[i].name.len,
                        posx, posy);
    }
    psm->update = false;
    // Filesystem_Texture_Dump("Action.png", renderer, psm->texture, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetRenderTarget(renderer, render_target);
}

void makeContent_FirstMenu(struct Game *sota) {
    tnecs_E menu_entity = sota->title_screen.menu;
    SDL_assert(menu_entity > 0);
    struct Menu *mc;
    mc = IES_GET_C(gl_world, menu_entity, Menu);
    SDL_assert(mc != NULL);
    struct ActionMenu *psm = mc->data;
    SDL_assert(psm != NULL);
    ActionMenu_Options_Reset(psm);
    ActionMenu_Option_Add(psm, MENU_OPTION_DEBUG_MAP, 1);
    ActionMenu_Option_Add(psm, MENU_OPTION_NEW_GAME,  1);
    ActionMenu_Option_Add(psm, MENU_OPTION_SETTINGS,  1);
    ActionMenu_Compute_Size(psm, &mc->n9patch);
}

void makeContent_AM_TRADE(struct Game *sota, void *data1, void *data2) {
    // tnecs_E menu_entity = sota->menus.ACTION[MENU_ActionMenu_TRADE];
    // struct Menu *mc = IES_GET_C(gl_world, menu_entity, Menu);
    // struct ActionMenu *psm = mc->data;
}

i32 AM_Options_Num(const Action *psm) {
    if (psm == NULL) {
        return 0;
    }
    if (psm->options == NULL) {
        return 0;
    }

    return (DARR_NUM(psm->options));
}

s8 Menu_Option_Name(i32 id) {
    return(menuOptionnames[opt_id]);
}
