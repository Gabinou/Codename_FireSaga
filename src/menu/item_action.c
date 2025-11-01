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
** ItemActionMenu (IAM): Core
**
*/

#include "item.h"
#include "weapon.h"
#include "globals.h"
#include "position.h"
#include "platform.h"
#include "title_screen.h"

#include "game/map.h"
#include "game/game.h"

#include "map/map.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/item_action.h"

#include "unit/unit.h"
#include "unit/equipment.h"

const ItemActionMenu ItemActionMenu_default = {0};

const i32 IAM_Options[IAM_OPTION_NUM] = {
    MENU_OPTION_EQUIP,
    MENU_OPTION_USE,
    MENU_OPTION_DROP,
    MENU_OPTION_TRADE
};

ItemActionMenu *ItemActionMenu_Alloc(void) {
    ItemActionMenu *iam = IES_malloc(sizeof(ItemActionMenu));
    *iam                = ItemActionMenu_default;
    iam->am             = ActionMenu_Alloc();
    IES_assert(iam                  != NULL);
    IES_assert(iam->am              != NULL);
    IES_assert(iam->am->platform    != NULL);

    return (iam);
}

void ItemActionMenu_Free(ItemActionMenu *iam, Menu *mc) {
    if (iam == NULL) {
        IES_assert(0);
        return;
    }
    if (mc == NULL) {
        IES_assert(0);
        return;
    }

    ActionMenu_Free(iam->am, mc);
    iam->am = NULL;
    IES_free(iam);
}

/* --- Elem Move --- */
i32 ItemActionMenu_Elem_Move(Menu *mc, i32 direction) {
    return (ActionMenu_Elem_Move(mc, direction));
}

void ItemActionMenu_Elem_Pos(   ItemActionMenu  *iam,
                                Menu            *mc) {
    ActionMenu_Elem_Pos(iam->am, mc);
}

void ItemActionMenu_Elem_Links( ItemActionMenu  *iam,
                                Menu            *mc) {
    ActionMenu_Elem_Links(iam->am, mc);
}

void ItemActionMenu_Elem_Boxes( ItemActionMenu  *iam,
                                Menu            *mc) {
    ActionMenu_Elem_Boxes(iam->am, mc);
}

void ItemActionMenu_Dynamic(ItemActionMenu  *iam,
                            n9Patch         *n9,
                            tnecs_E          unit_E,
                            Game            *IES) {
    /* --- Possible actions with selected item --- */
    /* -- Get menu -- */
    if (iam == NULL) {
        IES_assert(0);
        return;
    }
    if (iam->am == NULL) {
        IES_assert(0);
        return;
    }

    /* -- Get selected item & unit -- */
    Unit *unit = IES_GET_C(gl_world, unit_E, Unit);
    SDL_assert(unit != NULL);

    SDL_assert(IES->selected.item != TNECS_NULL);
    InvItem *invitem;
    invitem = IES_GET_C(gl_world, IES->selected.item,
                        InvItem);
    SDL_assert(invitem          != NULL);
    SDL_assert(gl_items_dtab    != NULL);

    Item_Load(invitem->id);
    const Item *item = Item_Get(invitem);

    /* -- 1. Equip  -- */
    ActionMenu_Options_Reset(iam->am);
    /* All items & weapons */
    Menu_Option option = {MENU_OPTION_EQUIP, 1};
    ActionMenu_Option_Add(iam->am, option);

    /* -- 2. Use -- */
    /* Show "Use" option but **greyed** if COULD be used if
    ** criteria is met. Document criteria in UI */
    if (Item_canUse(item)) {
        option.enabled  = _Unit_canUse_Item(unit, item);
        option.id       = MENU_OPTION_USE;
        ActionMenu_Option_Add(iam->am, option);
    }

    /* -- 3. Drop -- */
    /* Drop option should be hardest to acces with cursor */
    option.id       = MENU_OPTION_DROP;
    option.enabled  = true;
    ActionMenu_Option_Add(iam->am, option);

    /* -- 4. Trade -- */
    /* TODO: Only if neighboring friendly unit */
    option.id       = MENU_OPTION_TRADE;
    ActionMenu_Option_Add(iam->am, option);
    ActionMenu_Compute_Size(iam->am, n9);
}

i32 ItemActionMenu_Options_Num(const ItemActionMenu *iam) {
    return (ActionMenu_Options_Num(iam->am));
}

void ItemActionMenu_Load(ItemActionMenu *iam, n9Patch *n9) {
    if (iam == NULL) {
        IES_assert(0);
        return;
    }
    if (iam->am == NULL) {
        IES_assert(0);
        return;
    }
    /* Set n9 size, load its textures */
    n9Patch_Free(n9);
    *n9             = n9Patch_default;
    n9->px.x        = MENU_PATCH_PIXELS;
    n9->px.y        = MENU_PATCH_PIXELS;
    n9->scale.x     = IAM_N9PATCH_SCALE_X;
    n9->scale.y     = IAM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * IAM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * IAM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9, size);

    n9->pos.x           = 0;
    n9->pos.y           = 0;

    pActionMenu_Load(iam->am->platform, n9);
}

i32 ItemActionMenu_Option(  ItemActionMenu  *iam,
                            struct Menu     *mc) {
    i32 menu_option = iam->am->options[mc->elem].id;
    return (menu_option);
}

i32 ItemActionMenu_Option_Order(ItemActionMenu *iam,
                                struct Menu *mc) {
    i32 menu_option = ItemActionMenu_Option(iam, mc);
    /* TODO: check if menu_option is part of UAM_OPTIONS */
    i32 order = _ItemActionMenu_Option_Order(iam, menu_option);
    return (order);
}

i32 _ItemActionMenu_Option_Order(ItemActionMenu *m,
                                 i32 option) {
    for (int i = 0; i < IAM_OPTION_NUM; i++) {
        if (IAM_Options[i] == option) {
            return (i);
        }
    }
    return (-1);
}

void ItemActionMenu_Draw(   Menu            *mc,
                            SDL_Texture     *render_target,
                            SDL_Renderer    *renderer) {
    if (mc == NULL) {
        IES_assert(0);
        return;
    }
    IES_assert(mc->type == MENU_TYPE_ITEM_ACTION);
    ItemActionMenu *iam = mc->data;
    if (iam == NULL) {
        IES_assert(0);
        return;
    }

    _ActionMenu_Draw(   iam->am, &mc->n9patch,
                        render_target, renderer);
}
