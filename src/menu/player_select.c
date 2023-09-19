#include "menu/player_select.h"

/* --- PUT PLAYER_SELECT MENU CONTENT MAKERS INTO FSM --- */
// #include "names/menu/player_select.h"
psm_maker_t menuContentMakers[MENU_PLAYER_SELECT_END] = {
    /* NULL */        NULL,
    /* UNIT_ACTION */ makeContent_PSM_UNIT_ACTION,
    /* MAP_ACTION */  makeContent_PSM_MAP_ACTION,
    /* TRADE */       makeContent_PSM_TRADE,
    /* STAFF */       makeContent_PSM_STAFF,
};

struct PlayerSelectMenu PlayerSelectMenu_default = {
    .pos            = {0},

    .id             = 0,
    .option_num     = 0,
    .row_height     = ASCII_GLYPH_HEIGHT,
    .menu_padding   = {PSM_PADDING_RIGHT, PSM_PADDING_TOP, PSM_PADDING_LEFT, PSM_PADDING_BOTTOM},
    .option_names   = NULL,
    .options        = NULL,

    .update         = true,
    .texture        = NULL,

    .text_width     = 0,
    .icon_width     = 32,

    .pixelnours     = NULL,
};

struct PlayerSelectMenu *PlayerSelectMenu_Alloc() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct PlayerSelectMenu *psm = SDL_malloc(sizeof(struct PlayerSelectMenu));
    *psm = PlayerSelectMenu_default;
    SDL_assert(psm);

    if (psm->options == NULL) {
        psm->options = DARR_INIT(psm->options, uf32, 8);
    }
    if (psm->option_names == NULL) {
        psm->option_names = DARR_INIT(psm->option_names, char *, 8);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (psm);
}

void PlayerSelectMenu_Free(struct PlayerSelectMenu *psm, struct MenuComponent *mc) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(mc);
    SDL_assert(psm);
    if (mc->elem_box != NULL) {
        free(mc->elem_box);
        mc->elem_box = NULL;
    }
    if (mc->elem_pos != NULL) {
        free(mc->elem_pos);
        mc->elem_pos = NULL;
    }
    if (mc->elem_links != NULL) {
        free(mc->elem_links);
        mc->elem_links = NULL;
    }
    if (psm->options != NULL)
        DARR_FREE(psm->options);
    if (psm->option_names != NULL)
        DARR_FREE(psm->option_names);
    if (psm->texture != NULL) {
        SDL_DestroyTexture(psm->texture);
        psm->texture = NULL;
    }
    free(psm);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void FirstMenu_Load(struct PlayerSelectMenu *psm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    n9Patch_Free(n9patch);
    *n9patch                 = n9Patch_default;
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->scale.x         = SOTA_FIRST_MENU_N9PATCH_SCALE_X;
    n9patch->scale.y         = SOTA_FIRST_MENU_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE);
    n9patch->size_pixels.y   = (MENU_PATCH_PIXELS * SOTA_FIRST_MENU_PATCH_X_SIZE);
    n9patch->size_patches.x  = SOTA_FIRST_MENU_PATCH_X_SIZE;
    n9patch->size_patches.y  = SOTA_FIRST_MENU_PATCH_X_SIZE;
    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);


    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Load(struct PlayerSelectMenu *psm, SDL_Renderer *renderer,
                           struct n9Patch *n9patch) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    n9Patch_Free(n9patch);

    *n9patch                 = n9Patch_default;
    n9patch->patch_pixels.x  = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y  = MENU_PATCH_PIXELS;
    n9patch->scale.x         = PSM_N9PATCH_SCALE_X;
    n9patch->scale.y         = PSM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x   = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_X_SIZE);
    n9patch->size_pixels.y   = (MENU_PATCH_PIXELS * STATS_MENU_PATCH_Y_SIZE);
    n9patch->pos.x           = 0;
    n9patch->pos.y           = 0;

    psm->row_height = ASCII_GLYPH_HEIGHT + 2; /* pixel fonts have python8 pixels*/
    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
    SDL_assert(n9patch->texture != NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}


void PlayerSelectMenu_Options_Reset(struct PlayerSelectMenu *psm) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    DARR_NUM(psm->options) = 0;
    DARR_NUM(psm->option_names) = 0;
    psm->option_num = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

int PlayerSelectMenu_Option_Index(struct PlayerSelectMenu *psm, uf32 option) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(psm->option_num > 0);
    int out = -1;
    for (if32 i = 0; i < psm->option_num; i++) {
        if (psm->options[i] == option) {
            out = i;
            break;
        }
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void PlayerSelectMenu_Option_Add(struct PlayerSelectMenu *psm, uf32 option) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(psm);
    SDL_assert(psm->options);
    SDL_assert(psm->option_names);

    /* - adding option - */
    psm->option_num++;
    DARR_PUT(psm->options, option);

    /* - Computing option width, check if increase menu width - */
    char *text = menuOptionnames[option];
    int text_width = PixelFont_Width(psm->pixelnours, text, strlen(text));
    int padding = psm->menu_padding.left + psm->menu_padding.right;
    if ((text_width + padding) > psm->text_width) {
        psm->text_width = text_width + padding;
    }

    DARR_PUT(psm->option_names, menuOptionnames[option]);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Compute_Size(struct PlayerSelectMenu *psm, struct n9Patch *n9patch) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Padding mp = psm->menu_padding;
    /* - Compute patch sizes from text - */
    n9patch->size_patches.x = (int)round((float)psm->text_width / n9patch->patch_pixels.x);
    int temp_y = mp.top + mp.bottom + psm->row_height * psm->option_num;
    n9patch->size_patches.y = temp_y / n9patch->patch_pixels.y;

    /* - Compute pixel size from previous patch sizes for tight fit - */
    n9patch->size_pixels.x = n9patch->size_patches.x * n9patch->patch_pixels.x;
    n9patch->size_pixels.y = n9patch->size_patches.y * n9patch->patch_pixels.y;

    /* - Destroy texture because it does not fit new size - */
    SDL_DestroyTexture(psm->texture);
    psm->texture = NULL;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Elem_Links(struct PlayerSelectMenu *psm, struct MenuComponent *mc) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (mc->elem_links != NULL)
        free(mc->elem_links);
    SDL_assert(mc->elem_num == psm->option_num);
    mc->elem_links = malloc(psm->option_num * sizeof(*mc->elem_links));
    for (if32 i = 0; i < psm->option_num; i++) {
        mc->elem_links[i] = MenuElemDirections_default;
        if (i < (psm->option_num - 1))
            mc->elem_links[i].bottom = i + 1;
        if (i > 0)
            mc->elem_links[i].top = i - 1;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Cursor_Boxes(struct PlayerSelectMenu *m, struct MenuComponent *mc) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Cursor_Pos(struct PlayerSelectMenu *m, struct MenuComponent *mc) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Elem_Boxes(struct PlayerSelectMenu *psm, struct MenuComponent *mc) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (mc->elem_box != NULL)
        free(mc->elem_box);
    SDL_assert(mc->elem_num > 0);
    mc->elem_box = malloc(mc->elem_num * sizeof(*mc->elem_box));
    for (if32 i = 0; i < mc->elem_num; i++) {
        mc->elem_box[i].x = SOTA_TILESIZE;
        mc->elem_box[i].y = SOTA_TILESIZE;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Elem_Pos(struct PlayerSelectMenu *psm, struct MenuComponent *mc) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (mc->elem_pos != NULL)
        free(mc->elem_pos);
    struct Padding mp = psm->menu_padding;
    struct nmath_point_int32_t pos9 = mc->n9patch.pos, scale = mc->n9patch.scale;

    mc->elem_pos = calloc(mc->elem_num, sizeof(*mc->elem_pos));
    for (if32 i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = psm->pos.x + pos9.x + mp.left * scale.x;
        mc->elem_pos[i].y = psm->pos.y + (pos9.y + ((i * psm->row_height + mp.top))) * scale.y;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Draw(struct MenuComponent *mc, SDL_Texture *render_target,
                           SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = psm->pos.x,
        .y = psm->pos.y,
    };
    SDL_assert(psm->texture != NULL);
    SDL_RenderCopy(renderer, psm->texture, NULL, &dstrect);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void PlayerSelectMenu_Update(struct PlayerSelectMenu *psm, struct n9Patch *n9patch,
                             SDL_Texture *render_target, SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    SDL_assert(psm != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert(psm->pixelnours != NULL);
    SDL_assert(psm->option_names != NULL);

    /* - variable declaration/constants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);

    int_fast16_t menu_w = n9patch->size_pixels.x;
    int_fast16_t menu_h = n9patch->size_pixels.y;
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);

    /* - create render target texture - */
    if (psm->texture == NULL) {
        psm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
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

    if32 posx = n9patch->pos.x + psm->menu_padding.left, posy;
    int total_text_height = psm->option_num * psm->row_height +  n9patch->pos.y + psm->menu_padding.top;
    int shift_y = (n9patch->size_patches.y * n9patch->patch_pixels.y) - total_text_height;
    shift_y /= 2;

    for (if32 i = 0; i < psm->option_num; i++) {
        posy = shift_y + n9patch->pos.y + psm->menu_padding.top + (i * psm->row_height);
        PixelFont_Write(psm->pixelnours, renderer, psm->option_names[i],
                        strlen(psm->option_names[i]), posx, posy);
    }
    psm->update = false;
    // Filesystem_Texture_Dump("PlayerSelectMenu.png", renderer, psm->texture, SDL_PIXELFORMAT_ARGB8888);
    SDL_SetRenderTarget(renderer, render_target);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* Only for interactive MENU_TYPE_PLAYER_SELECT */
void makeContent_PSM_UNIT_ACTION(struct Game *sota, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_entity = sota->player_select_menus[MENU_PLAYER_SELECT_UNIT_ACTION];
    SDL_assert(menu_entity > 0);
    SDL_assert(sota->selected_unit_entity > 0);
    struct MenuComponent *mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    struct PlayerSelectMenu *psm = mc_ptr->data;
    SDL_assert(psm != NULL);
    PlayerSelectMenu_Options_Reset(psm);

    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_ITEMS);
    if (DARR_NUM(sota->passives) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TRADE);
    bool seizeable = false;
    if (seizeable)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_SEIZE);
    if (DARR_NUM(sota->auditors) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TALK);
    if (DARR_NUM(sota->defendants) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_ATTACK);
    if (DARR_NUM(sota->patients) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_STAFF);
    if (DARR_NUM(sota->spectators) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_DANCE);
    if (DARR_NUM(sota->victims) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_RESCUE);
    if (DARR_NUM(sota->openables) > 0)
        PlayerSelectMenu_Option_Add(psm, MENU_OPTION_OPEN);

    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_WAIT);
    PlayerSelectMenu_Compute_Size(psm, &mc_ptr->n9patch);
    Game_Menu_LocationfromUnit(sota, menu_entity, sota->selected_unit_entity);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void makeContent_PSM_STAFF(struct Game *sota, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // tnecs_entity_t menu_entity = sota->player_select_menus[MENU_PLAYER_SELECT_STAFF];
    // struct MenuComponent * mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    // struct PlayerSelectMenu * psm = mc_ptr->data;
    // for (uint32_t i = 0; i < sota->num_patients; i++) {
    // }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void makeContent_PSM_CONVOY(struct Game *sota, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // tnecs_entity_t menu_entity = sota->player_select_menus[MENU_PLAYER_SELECT_CONVOY];
    // struct MenuComponent * mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    // struct PlayerSelectMenu * psm = mc_ptr->data;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void makeContent_PSM_MAP_ACTION(struct Game *sota, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_entity = sota->player_select_menus[MENU_PLAYER_SELECT_MAP_ACTION];
    SDL_assert(menu_entity > TNECS_NULL);
    struct MenuComponent *mc_ptr;
    mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    struct PlayerSelectMenu *psm = mc_ptr->data;
    SDL_assert(psm != NULL);
    psm->option_num = 0;
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_UNITS);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_CONVOY);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_QUIT);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_END_TURN);
    PlayerSelectMenu_Compute_Size(psm, &mc_ptr->n9patch);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void makeContent_FirstMenu(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_entity = sota->first_menu;
    SDL_assert(menu_entity > 0);
    struct MenuComponent *mc_ptr;
    mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    SDL_assert(mc_ptr != NULL);
    struct PlayerSelectMenu *psm = mc_ptr->data;
    SDL_assert(psm != NULL);
    psm->option_num = 0;
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_DEBUG_MAP);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_NEW_GAME);
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_OPTIONS);
    PlayerSelectMenu_Compute_Size(psm, &mc_ptr->n9patch);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void makeContent_PSM_TRADE(struct Game *sota, void *data1, void *data2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_entity = sota->player_select_menus[MENU_PLAYER_SELECT_TRADE];
    struct MenuComponent *mc_ptr = TNECS_GET_COMPONENT(sota->world, menu_entity, MenuComponent);
    struct PlayerSelectMenu *psm = mc_ptr->data;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
