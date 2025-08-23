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
** Player Select Menu:
**  - All menus with dynamic, text only options
**
*/

#include "item.h"
#include "nmath.h"
#include "names.h"
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
#include "menu/player_select.h"

/* --- PUT PLAYER_SELECT MENU CONTENT MAKERS INTO FSM --- */
const psm_maker_t menuContentMakers[MENU_PLAYER_SELECT_END] = {
    /* NULL         */ NULL,
    /* UNIT_ACTION  */ makeContent_PSM_UNIT_ACTION,
    /* MAP_ACTION   */ makeContent_PSM_MAP_ACTION,
    /* TRADE        */ makeContent_PSM_TRADE,
    /* STAFF        */ makeContent_PSM_STAFF,
    /* ITEM_ACTION  */ makeContent_PSM_ITEM_ACTION,
};

const struct PlayerSelectMenu PlayerSelectMenu_default = {
    .row_height     = ASCII_GLYPH_HEIGHT,
    .menu_padding   = {PSM_PADDING_RIGHT, PSM_PADDING_TOP, PSM_PADDING_LEFT, PSM_PADDING_BOTTOM},
    .update         = true,
    .icon_width     = 32,
};

struct PlayerSelectMenu *PlayerSelectMenu_Alloc(void) {
    struct PlayerSelectMenu *psm = SDL_malloc(sizeof(struct PlayerSelectMenu));
    *psm = PlayerSelectMenu_default;
    SDL_assert(psm);

    if (psm->options == NULL) {
        psm->options = DARR_INIT(psm->options, PSM_Option, 8);
    }

    return (psm);
}

void PlayerSelectMenu_Free(struct PlayerSelectMenu *psm, struct Menu *mc) {
    SDL_assert(psm);
    SDL_assert(mc);
    Menu_Free(mc);
    if (psm->options != NULL) {
        DARR_FREE(psm->options);
        psm->options = NULL;
    }
    if (psm->texture != NULL) {
        SDL_DestroyTexture(psm->texture);
        psm->texture = NULL;
    }
    SDL_free(psm);
}

void FirstMenu_Load(struct PlayerSelectMenu *psm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);
    *n9patch                 = n9Patch_default;
    n9patch->px.x  = MENU_PATCH_PIXELS;
    n9patch->px.y  = MENU_PATCH_PIXELS;
    n9patch->scale.x         = SOTA_FIRST_MENU_N9PATCH_SCALE_X;
    n9patch->scale.y         = SOTA_FIRST_MENU_N9PATCH_SCALE_Y;
    n9patch->num.x  = SOTA_FIRST_MENU_PATCH_X_SIZE;
    n9patch->num.y  = SOTA_FIRST_MENU_PATCH_X_SIZE;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);


    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);

}

void PlayerSelectMenu_Load(struct PlayerSelectMenu *psm, SDL_Renderer *renderer,
                           struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);

    *n9patch                 = n9Patch_default;
    n9patch->px.x  = MENU_PATCH_PIXELS;
    n9patch->px.y  = MENU_PATCH_PIXELS;
    n9patch->scale.x         = PSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = PSM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    /* n9patch->size_pixels.x   = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE); */
    /* n9patch->size_pixels.y   = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE); */
    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    psm->row_height = ASCII_GLYPH_HEIGHT + 2; /* pixel fonts have python8 pixels*/
    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);

}

/* --- Elem Move --- */
i32 PlayerSelectMenu_Elem_Move(struct Menu *mc, i32 direction) {
    return (Periodic_Elem_Move(mc, direction, 0, mc->elem_num));
}

void PlayerSelectMenu_Options_Reset(PlayerSelectMenu *psm) {
    DARR_NUM(psm->options) = 0;
}

int PlayerSelectMenu_Option_Index(  PlayerSelectMenu *psm,
                                    u32 option) {
    int out = -1;
    i32 num = PSM_Options_Num(psm);
    SDL_assert(num > 0);
    for (i32 i = 0; i < num; i++) {
        if (psm->options[i].id == option) {
            out = i;
            break;
        }
    }
    return (out);
}

void PlayerSelectMenu_Option_Add(   PlayerSelectMenu *psm,
                                    u32 opt_id,
                                    b32 enabled) {
    SDL_assert(psm);
    SDL_assert(psm->options);

    /* - adding option - */
    PSM_Option option = {0};

    /* - Computing option width, check if increase menu width - */
    SDL_assert(opt_id > MENU_OPTION_START);
    SDL_assert(opt_id < MENU_OPTION_END);
    option.id   = opt_id;
    option.name = menuOptionnames[opt_id];
    SDL_assert(option.name.data != NULL);
    int text_width  = PixelFont_Width(  psm->pixelnours,
                                        option.name.data,
                                        option.name.num);
    int padding = psm->menu_padding.left + psm->menu_padding.right;
    if ((text_width + padding) > psm->text_width) {
        psm->text_width = text_width + padding;
    }

    option.enabled = enabled;

    DARR_PUT(psm->options, option);

}

void PlayerSelectMenu_Compute_Size( PlayerSelectMenu    *psm,
                                    n9Patch             *n9patch) {
    /* - Compute patch sizes from text - */
    struct Padding mp = psm->menu_padding;
    i32 num = PSM_Options_Num(psm);
    int text_height = mp.top + mp.bottom + psm->row_height * num;
    Point content = {psm->text_width, text_height};
    n9Patch_Fit(n9patch, content);

    /* - Destroy texture because it does not fit new size - */
    SDL_DestroyTexture(psm->texture);
    psm->texture = NULL;
}

void PlayerSelectMenu_Elem_Links(struct PlayerSelectMenu *psm, struct Menu *mc) {
    if (mc->elem_links != NULL)
        SDL_free(mc->elem_links);
    i32 num = PSM_Options_Num(psm);
    SDL_assert(mc->elem_num == num);
    mc->elem_links = SDL_malloc(num * sizeof(*mc->elem_links));
    for (i32 i = 0; i < num; i++) {
        mc->elem_links[i] = MenuElemDirections_default;
        if (i < (num - 1))
            mc->elem_links[i].bottom = i + 1;
        if (i > 0)
            mc->elem_links[i].top = i - 1;
    }
}

void PlayerSelectMenu_Cursor_Boxes(struct PlayerSelectMenu *m, struct Menu *mc) {

}

void PlayerSelectMenu_Cursor_Pos(struct PlayerSelectMenu *m, struct Menu *mc) {

}

void PlayerSelectMenu_Elem_Boxes(struct PlayerSelectMenu *psm, struct Menu *mc) {
    if (mc->elem_box != NULL)
        SDL_free(mc->elem_box);
    SDL_assert(mc->elem_num > 0);
    mc->elem_box = SDL_malloc(mc->elem_num * sizeof(*mc->elem_box));
    for (i32 i = 0; i < mc->elem_num; i++) {
        mc->elem_box[i].x = SOTA_TILESIZE;
        mc->elem_box[i].y = SOTA_TILESIZE;
    }
}

void PlayerSelectMenu_Elem_Pos(struct PlayerSelectMenu *psm, struct Menu *mc) {
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

void PlayerSelectMenu_Draw(struct Menu *mc, SDL_Texture *render_target,
                           SDL_Renderer *renderer) {
    struct PlayerSelectMenu *psm = (struct PlayerSelectMenu *)mc->data;
    struct n9Patch *n9patch = &mc->n9patch;

    SDL_assert(psm != NULL);
    SDL_assert(n9patch->pos.x == 0);
    SDL_assert(n9patch->pos.y == 0);
    if (psm->update) {
        PlayerSelectMenu_Update(psm, n9patch, render_target, renderer);
        psm->update = false;
    }
    SDL_assert(n9patch->pos.x == 0);
    SDL_assert(n9patch->pos.y == 0);

    /* TODO: set position of player_select_menu */
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

void PlayerSelectMenu_Update(   PlayerSelectMenu *psm,
                                n9Patch *n9patch,
                                SDL_Texture *render_target,
                                SDL_Renderer *renderer) {
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

    i32 num = PSM_Options_Num(psm);
    for (i32 i = 0; i < num; i++) {
        posy = n9patch->pos.y + psm->menu_padding.top + (i * psm->row_height);
        PixelFont_Write(psm->pixelnours, renderer,
                        psm->options[i].name.data,
                        psm->options[i].name.len,
                        posx, posy);
    }
    psm->update = false;
    // Filesystem_Texture_Dump("PlayerSelectMenu.png", renderer, psm->texture, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetRenderTarget(renderer, render_target);
}

/* Only for interactive MENU_TYPE_PLAYER_SELECT */
void makeContent_PSM_UNIT_ACTION(struct Game *sota, void *data1, void *data2) {
    Map *map = Game_Map(sota);
    SDL_assert(map                  != NULL);
    SDL_assert(map->darrs.tilemap   != NULL);
    tnecs_E menu_entity = sota->menus.player_select[MENU_PLAYER_SELECT_UNIT_ACTION];
    SDL_assert(menu_entity > 0);
    SDL_assert(sota->selected.unit_entity > 0);
    struct Menu *mc = IES_GET_C(gl_world, menu_entity, Menu);
    struct PlayerSelectMenu *psm = mc->data;
    SDL_assert(psm != NULL);
    PlayerSelectMenu_Options_Reset(psm);

    /* Items Option is always first. */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_ITEMS, 1);

    /* Trade option IF unit to trade with */
    // TODO: Reinsert trade menu when implemented.
    // if (DARR_NUM(sota->targets.passives) > 0)
    // PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TRADE, 1));

    /* --- Check if unit can SEIZE --- */
    /* -- Seizable: Check if tile is a throne --  */
    tnecs_E unit_ent = sota->selected.unit_entity;
    struct Position *pos = IES_GET_C(gl_world, unit_ent, Position);
    SDL_assert(pos != NULL);
    i32 index = sota_2D_index(pos->tilemap_pos.x, pos->tilemap_pos.y, Map_col_len(map));
    i32 tile_ind = map->darrs.tilemap[index] / TILE_DIVISOR;
    b32 isthrone = (tile_ind == TILE_THRONE);

    /* -- Seizable: Check if unit is a main character --  */
    struct Unit *unit = IES_GET_C(gl_world, unit_ent, Unit);
    SDL_assert(unit != NULL);
#ifdef DEBUG_SEIZE_ANYONE
    b32 ismainchar = true;
#else
    b32 ismainchar = (Unit_id(unit) == UNIT_ID_ERWIN);
#endif /*DEBUG_SEIZE_ANYONE*/

    if (isthrone && ismainchar)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_SEIZE, 1);
    if (DARR_NUM(sota->targets.auditors) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TALK, 1);
    if (DARR_NUM(sota->targets.defendants) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_ATTACK, 1);
    if (DARR_NUM(sota->targets.patients) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_STAFF, 1);
    if (DARR_NUM(sota->targets.spectators) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_DANCE, 1);
    if (DARR_NUM(sota->targets.victims) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_RESCUE, 1);
    if (DARR_NUM(sota->targets.openables) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_OPEN, 1);

    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_WAIT, 1);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);
    Game_Menu_LocationfromUnit(sota, menu_entity, sota->selected.unit_entity);
}

void makeContent_PSM_ITEM_ACTION(   Game *IES,
                                    void *data1,
                                    void *data2) {
    /* --- Possible actions with selected item --- */
    /* -- Get menu -- */
    tnecs_E menu = IES->menus.player_select[MENU_PLAYER_SELECT_ITEM_ACTION];
    SDL_assert(menu > TNECS_NULL);
    Menu *mc = IES_GET_C(gl_world, menu, Menu);
    PlayerSelectMenu *psm = mc->data;
    SDL_assert(psm != NULL);

    /* -- Get selected item & unit -- */
    SDL_assert(IES->selected.unit_entity != TNECS_NULL);
    Unit *unit = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);
    SDL_assert(unit != NULL);

    SDL_assert(IES->selected.item != TNECS_NULL);
    Inventory_item *invitem = IES_GET_C(gl_world, IES->selected.item, Inventory_item);
    SDL_assert(invitem != NULL);
    SDL_assert(gl_items_dtab != NULL);
    Item_Load(gl_items_dtab, invitem->id);
    const Item *item = DTAB_GET_CONST(gl_items_dtab, invitem->id);

    SDL_assert(item != NULL);

    /* -- 1. Equip  -- */
    PlayerSelectMenu_Options_Reset(psm);
    /* All items & weapons */
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_EQUIP, 1);

    /* -- 2. Use -- */
    /* Show "Use" option but **greyed** if COULD be used if
    ** criteria is met. Document criteria in UI */
    /* TODO: can item be used? */
    if (Item_couldbeUsed(item)) {
        b32 enabled = Unit_canUse_Item(item, unit);
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_USE, enabled);
    }

    /* -- 3. Drop -- */
    /* Drop option should be hardest to acces with cursor */
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_DROP, 1);

    /* -- 4. Trade -- */
    /* TODO: Only if neighboring friendly unit */
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_TRADE, 1);

    mc->elem_num = PSM_Options_Num(psm);
}

void makeContent_PSM_STAFF(Game *sota,
                           void *data1, void *data2) {
    // tnecs_E menu_entity = sota->menus.player_select[MENU_PLAYER_SELECT_STAFF];
    // struct Menu * mc = IES_GET_C(gl_world, menu_entity, Menu);
    // struct PlayerSelectMenu * psm = mc->data;
    // for (uint32_t i = 0; i < sota->num_patients; i++) {
    // }
}

void makeContent_PSM_CONVOY(Game *sota,
                            void *data1, void *data2) {
    // tnecs_E menu_entity = sota->menus.player_select[MENU_PLAYER_SELECT_CONVOY];
    // struct Menu * mc = IES_GET_C(gl_world, menu_entity, Menu);
    // struct PlayerSelectMenu * psm = mc->data;
}

void makeContent_PSM_MAP_ACTION(Game *sota,
                                void *data1, void *data2) {
    tnecs_E menu_entity = sota->menus.player_select[MENU_PLAYER_SELECT_MAP_ACTION];
    SDL_assert(menu_entity > TNECS_NULL);
    struct Menu *mc;
    mc = IES_GET_C(gl_world, menu_entity, Menu);
    struct PlayerSelectMenu *psm = mc->data;
    SDL_assert(psm != NULL);
    PlayerSelectMenu_Options_Reset(psm);
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_UNITS,       1);
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_CONVOY,      1);
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_SETTINGS,    1);
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_QUIT,        1);
    PlayerSelectMenu_Option_Add(  psm, MENU_OPTION_END_TURN,    1);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);
}

void makeContent_FirstMenu(struct Game *sota) {
    tnecs_E menu_entity = sota->title_screen.menu;
    SDL_assert(menu_entity > 0);
    struct Menu *mc;
    mc = IES_GET_C(gl_world, menu_entity, Menu);
    SDL_assert(mc != NULL);
    struct PlayerSelectMenu *psm = mc->data;
    SDL_assert(psm != NULL);
    PlayerSelectMenu_Options_Reset(psm);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_DEBUG_MAP, 1);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_NEW_GAME,  1);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_SETTINGS,  1);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);
}

void makeContent_PSM_TRADE(struct Game *sota, void *data1, void *data2) {
    // tnecs_E menu_entity = sota->menus.player_select[MENU_PLAYER_SELECT_TRADE];
    // struct Menu *mc = IES_GET_C(gl_world, menu_entity, Menu);
    // struct PlayerSelectMenu *psm = mc->data;
}

i32 PSM_Options_Num(const PlayerSelectMenu *psm) {
    if (psm == NULL) {
        return 0;
    }
    if (psm->options == NULL) {
        return 0;
    }

    return (DARR_NUM(psm->options));
}
