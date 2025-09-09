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

void pACtionMenu_Load(const pActionMenu *pAM, n9Patch *n9) {
    if (n9->texture == NULL) {
        char *path = PATH_JOIN( "..", "assets", "GUI",
                                "n9Patch", "menu8px.png");
        n9->texture = Filesystem_Texture_Load(  pAM->renderer, path,
                                                SDL_PIXELFORMAT_INDEX8);
    }
    IES_assert(n9->texture != NULL);
}
