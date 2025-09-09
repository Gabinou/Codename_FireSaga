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
** FirstMenu (FM): Platform implementation
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

typedef struct pActionMenu {
    SDL_Texture  *texture;
    SDL_Texture  *render_target;
    SDL_Renderer *renderer;
} pActionMenu;

pActionMenu *pActionMenu_Alloc(void) {
    pActionMenu *pam  = IES_calloc(1, sizeof(pActionMenu));
    IES_assert(pam);

    return (pam);
}

void pActionMenu_Free(pActionMenu *pam) {
    pActionMenu_Free_Texture(pam);
    IES_free(pam);
}

void pActionMenu_Set(   pActionMenu     *pam,
                        SDL_Texture     *render_target,
                        SDL_Renderer    *renderer) {
    /* Todo remove SDL stuff if ever all DRAW funcs
    **      are split core/platform */
    pam->render_target  = render_target;
    pam->renderer       = renderer;

}

void pActionMenu_Load(const pActionMenu *pAM, n9Patch *n9) {
    IES_assert(n9               != NULL);
    IES_assert(pAM              != NULL);
    IES_assert(pAM->renderer    != NULL);
    if (n9->texture == NULL) {
        char *path = PATH_JOIN( "..", "assets", "GUI",
                                "n9Patch", "menu8px.png");
        n9->texture = Filesystem_Texture_Load(  pAM->renderer, path,
                                                SDL_PIXELFORMAT_INDEX8);
    }
    IES_assert(n9->texture != NULL);
}

void pActionMenu_Free_Texture(pActionMenu *pam) {
    if (pam->texture != NULL) {
        SDL_DestroyTexture(pam->texture);
        pam->texture = NULL;
    }
}

void pActionMenu_Draw(ActionMenu *am, n9Patch *n9) {
    pActionMenu *pam = am->platform;
    IES_assert(am           != NULL);
    IES_assert(pam          != NULL);
    IES_assert(n9->pos.x    == 0);
    IES_assert(n9->pos.y    == 0);
    if (am->update) {
        pActionMenu_Update(am, n9);
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
    IES_assert(pam->texture != NULL);
    SDL_RenderCopy( pam->renderer,  pam->texture,
                    NULL,           &dstrect);
}

void pActionMenu_Update(ActionMenu *am, n9Patch *n9) {
    /* --- PRELIMINARIES --- */
    pActionMenu *pam = am->platform;
    IES_assert(am              != NULL);
    IES_assert(pam             != NULL);
    IES_assert(pam->renderer   != NULL);
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
        am->texture = SDL_CreateTexture(pam->renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.x, size.y);
        IES_assert(am->texture != NULL);
        SDL_SetTextureBlendMode(am->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(pam->renderer, am->texture);

    /* Clear the target to our selected color. */
    SDL_SetRenderDrawColor(pam->renderer, 0, 0, 0, 0);
    SDL_RenderClear(pam->renderer);
    SDL_SetRenderDrawColor(pam->renderer, 0, 0, 0, 0xFF);

    IES_assert(am->texture != NULL);
    /* --- RENDERING STATS-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9->scale.x;
    int scale_y = n9->scale.y;
    n9->scale.x = 1;
    n9->scale.y = 1;
    n9->pos.x = 0;
    n9->pos.y = 0;
    n9Patch_Draw(n9, pam->renderer);
    n9->scale.x = scale_x;
    n9->scale.y = scale_y;

    i32 posx = n9->pos.x + am->menu_padding.left, posy;
    // int total_text_height = am->option_num * am->row_height +  n9->pos.y + am->menu_padding.top;
    // int shift_y = (n9->num.y * n9->px.y) - total_text_height;
    // shift_y /= 2;

    i32 num = ActionMenu_Options_Num(am);
    for (i32 i = 0; i < num; i++) {
        posy = n9->pos.y + am->menu_padding.top + (i * am->row_height);
        s8 name = Menu_Option_Name(am->options[i].id);

        PixelFont_Write(am->pixelnours, pam->renderer,
                        name.data,      name.len,
                        posx,           posy);
    }
    am->update = false;
    // Filesystem_Texture_Dump("ActionMenu.png", renderer, am->texture, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetRenderTarget(pam->renderer, pam->render_target);
}
