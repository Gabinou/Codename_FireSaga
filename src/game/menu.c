#include "game/menu.h"

/* --- All Menus --- */
bool Game_Menus_Init(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool success = true;
    if (sota->menu_stack != NULL)
        DARR_FREE(sota->menu_stack);
    sota->menu_stack = DARR_INIT(sota->menu_stack, tnecs_entity_t, MENU_PLAYER_SELECT_END);
    memset(sota->menu_stack, 0, MENU_PLAYER_SELECT_END * sizeof(*sota->menu_stack));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (success);
}

void Game_Switch_toCandidates(struct Game *sota, tnecs_entity_t *candidates) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    sota->candidates = candidates;
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_CANDIDATES, sota->reason);

    /* - set cursor position on tilemap - */
    Game_cursorFocus_onMap(sota);

    /* - moving cursor moves to next entity-> go to its position on tilemap - */
    struct Position *candidate_pos, *cursor_pos;
    int index = (sota->previous_candidate > -1) ? sota->previous_candidate : 0;
    tnecs_entity_t candidate = sota->candidates[index];

    candidate_pos = TNECS_GET_COMPONENT(sota->world, candidate, Position);
    SDL_assert(candidate_pos != NULL);
    cursor_pos = TNECS_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    SDL_assert(cursor_pos != NULL);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;

    /* - Hover on first candidate - */
    *data2_entity = sota->candidates[index];
    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Menu stack --- */
void Game_menuStack_Free(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    struct StatsMenu *stats_menu;
    struct PlayerSelectMenu *psm_menu;
    SDL_assert(sota->menu_stack != NULL);
    while (DARR_NUM(sota->menu_stack) > 0) {
        tnecs_entity_t entity = sota->menu_stack[DARR_NUM(sota->menu_stack) - 1];
        struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, entity, MenuComponent);

        if (mc->data == NULL) {
            tnecs_entity_t menu = DARR_POP(sota->menu_stack);
            continue;
        }

        switch (mc->type) {
            case MENU_TYPE_STATS:
                stats_menu = mc->data;
                StatsMenu_Free(stats_menu);
                mc->data = NULL;
                break;
            case MENU_TYPE_PLAYER_SELECT:
                psm_menu = mc->data;
                PlayerSelectMenu_Free(psm_menu, mc);
                mc->data = NULL;
                break;
        }
        mc->data = NULL;
        tnecs_entity_t menu = DARR_POP(sota->menu_stack);
    }

    DARR_FREE(sota->menu_stack);
    sota->menu_stack = NULL;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_menuStack_Push(struct Game *sota, tnecs_entity_t in_menu_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%ld", in_menu_entity);
    SDL_assert(in_menu_entity > 0);
    DARR_PUT(sota->menu_stack, in_menu_entity);
    SOTA_Log_Debug("DARR_NUM %ld", DARR_NUM(sota->menu_stack));
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

tnecs_entity_t Game_menuStack_Pop(struct Game *sota, bool destroy) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    tnecs_entity_t menu_stack_top_entity = DARR_POP(sota->menu_stack);
    SDL_assert(menu_stack_top_entity > 0);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world,
                                                   menu_stack_top_entity,
                                                   MenuComponent);
    mc->visible = false;
    // mc->elem = 0;
    if (destroy) {
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (menu_stack_top_entity);
}

/* --- GrowthsMenu --- */
void Game_GrowthsMenu_Enable(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->GM_menu == 0)
        Game_GrowthsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->GM_menu);
    SDL_assert(sota->GM_menu > 0);
    Game_GrowthsMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->reason, "GM_MENU was created", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->stats_menu, NULL);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_GrowthsMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->GM_menu == TNECS_NULL)
        sota->GM_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->GM_menu, MenuComponent);
    SDL_assert(mc != NULL);
    mc->type = MENU_TYPE_GROWTHS;
    mc->draw = &GrowthsMenu_Draw;

    struct GrowthsMenu *GM = GrowthsMenu_Alloc();
    SDL_assert(GM != NULL);
    mc->data = GM;

    // FOR DEBUG
    // TODO: GET STAT FROM USER
    i16 stat_toplot = 1;

    Graph_Stat_Add(&GM->graph, &test_base_stats, test_grown_stats, 40, 31, stat_toplot);
    SDL_assert(GM->graph.graph_stats[stat_toplot].stat_id >= 0);
    GM->pos.x       = sota->settings.res.x / 3;
    GM->pos.y       = sota->settings.res.y / 3;
    mc->visible     = true;
    mc->elem_links  = GM_links;
    mc->elem_pos    = GM_elem_pos;
    mc->elem_box    = GM_elem_box;
    mc->elem_num    = GM_ELEM_NUM;

    SDL_assert(mc->elem_pos != NULL);
    GrowthsMenu_Load(GM, sota->renderer, &mc->n9patch);

    SDL_assert(sota->pixelnours != NULL);
    GM->pixelnours = sota->pixelnours;
    SDL_assert(sota->pixelnours_big != NULL);
    GM->pixelnours_big = sota->pixelnours_big;
    /* scaling elem_pos: put it last cause dependences */
    GrowthsMenu_Elem_Pos(GM, mc);
    MenuComponent_Elem_Boxes_Check(mc);
    mc->elem_box[GM_GROWTHS].x *= 2;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_GrowthsMenu_Update(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->GM_menu, MenuComponent);
    mc->visible = true;
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    struct GrowthsMenu *GM_menu = (struct GrowthsMenu *)mc->data;
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    GrowthsMenu_Unit_Set(GM_menu, unit_ontile);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- StatsMenu --- */
void Game_StatsMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    // creates statsmenu entity, sets sizes, fonts
    if (sota->stats_menu == TNECS_NULL)
        sota->stats_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, MenuComponent);
    mc->type = MENU_TYPE_STATS;
    mc->draw = &StatsMenu_Draw;

    /* stats_menu struct init */
    struct StatsMenu *stats_menu = StatsMenu_Alloc();
    stats_menu->pos.x = sota->settings.res.x / 3;
    stats_menu->pos.y = sota->settings.res.y / 3;
    mc->data          = stats_menu;
    mc->visible       = true;
    mc->elem_links    = stats_menu_links;
    mc->elem_pos      = sm_elem_pos;
    mc->elem_box      = stats_menu_elem_box;
    mc->elem_num      = STATS_MENU_ELEMS_NUM;

    // TODO: copy descriptions
    SDL_assert(sota->pixelnours     != NULL);
    SDL_assert(sota->pixelnours_big != NULL);
    stats_menu->pixelnours      = sota->pixelnours;
    stats_menu->pixelnours_big  = sota->pixelnours_big;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_StatsMenu_Update(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    /* - Get unit on tile - */
    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(unit_ontile->weapons_dtab != NULL);

    /* - Update stats menu - */
    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->stats_menu, MenuComponent);
    mc->visible = true;
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    struct StatsMenu *stats_menu = (struct StatsMenu *)mc->data;
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    StatsMenu_Load(stats_menu, unit_ontile, sota->renderer, &mc->n9patch);
    // Scaling elem_pos: put it last cause dependencies
    StatsMenu_Elem_Pos(stats_menu, mc);
    MenuComponent_Elem_Boxes_Check(mc);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_StatsMenu_Enable(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->stats_menu == 0)
        Game_StatsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->stats_menu);
    SDL_assert(sota->stats_menu > 0);
    Game_StatsMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->reason, "STATS_MENU was created", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->stats_menu, NULL);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
/* --- PlayerSelectMenu --- */
void Game_PlayerSelectMenus_Free(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    for (size_t i = 0; i < MENU_PLAYER_SELECT_NUM; i++) {
        if (sota->player_select_menus[i] == TNECS_NULL)
            continue;

        tnecs_entity_t ent = sota->player_select_menus[i];
        struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, ent, MenuComponent);

        if (mc->data == NULL)
            continue;

        struct PlayerSelectMenu *psm_menu = mc->data;
        PlayerSelectMenu_Free(psm_menu, mc);
        mc->data = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_postLoadout_Defendants(struct Game *sota, tnecs_entity_t actor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Finding possible defendants with equipped weapons -- */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, actor, Unit);
    DARR_NUM(sota->defendants) = 0;

    /* - Compute attacktolist - */
    Map_Attacktomap_Compute(sota->map, sota->world, actor, false, true);
    Map_Attacktolist_Compute(sota->map);

    /* Find all Defendants */
    sota->defendants = Map_Find_Defendants(sota->map, sota->map->attacktolist,
                                           sota->defendants, actor, false);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_postLoadout_Patients(struct Game *sota, tnecs_entity_t actor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, actor, Unit);
    assert(Unit_canStaff(unit));
    DARR_NUM(sota->patients) = 0;

    /* - Compute healtolist - */
    Map_Healtomap_Compute(sota->map, sota->world, actor, false, true);
    Map_Healtolist_Compute(sota->map);

    /* Find all Patients if any */
    sota->patients = Map_Find_Patients(sota->map, sota->weapons_dtab, sota->map->healtolist,
                                       sota->patients, actor, false);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Finding if any staff equipment has a patient -- */
void Game_preLoadout_Patients(struct Game *sota, tnecs_entity_t actor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    DARR_NUM(sota->defendants) = 0;
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, actor, Unit);

    /* Combine ranges for all weapons in equipment */
    struct Range *range = Unit_Range_Combine_Staves(unit, false);

    /* Compute healtolist */
    sota->map->healtomap = _Map_tomap_Compute(sota->map->healtomap, sota->map->movemap,
                                              sota->map->row_len,   sota->map->col_len,
                                              true, range, NMATH_MOVETILE_INCLUDE);

    Map_Healtolist_Compute(sota->map);

    /* Find Patients if any */
    sota->patients = Map_Find_Patients(sota->map, sota->weapons_dtab, sota->map->healtolist,
                                       sota->patients, actor, true);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Finding if any weapon in equipment has a defendant -- */
void Game_preLoadout_Defendants(struct Game *sota, tnecs_entity_t actor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    DARR_NUM(sota->defendants) = 0;
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, actor, Unit);

    /* Combine all weapon ranges */
    struct Range *range = Unit_Range_Combine_Weapons(unit, false);

    /* Compute attacktolist */
    sota->map->attacktomap = _Map_tomap_Compute(sota->map->attacktomap, sota->map->movemap,
                                                sota->map->row_len,     sota->map->col_len,
                                                true, range, NMATH_MOVETILE_INCLUDE);
    Map_Attacktolist_Compute(sota->map);

    /* Find Defendants if any */
    sota->defendants = Map_Find_Defendants(sota->map, sota->map->attacktolist,
                                           sota->defendants, actor, true);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Decides which option are in UnitAction menu -- */
/* CONTEXT: Player just chose where unit moves.
    - Defendants (breakables), patients
        - Only check if ANY exist
        - Recomputation necessary AFTER player makes another choice later
    - Doors, auditors, victims, chests
        - Can be computed right now:
*/
void Game_preUnitAction_Targets(struct Game *sota, tnecs_entity_t actor) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Resetting lists -- */
    DARR_NUM(sota->victims)     = 0;
    DARR_NUM(sota->passives)    = 0;
    DARR_NUM(sota->auditors)    = 0;
    DARR_NUM(sota->openables)   = 0;
    DARR_NUM(sota->spectators)  = 0;

    /* -- Getters -- */
    struct Unit     *unit = TNECS_GET_COMPONENT(sota->world, actor, Unit);
    struct Position *pos  = TNECS_GET_COMPONENT(sota->world, actor, Position);

    /* -- if dancer -- */
    int x = pos->tilemap_pos.x, y = pos->tilemap_pos.y;
    if (Unit_canDance(unit))
        sota->spectators = Map_Find_Spectators(sota->map, sota->spectators, x, y);

    /* -- For everyone -- */
    Game_preLoadout_Patients(sota,   actor);
    Game_preLoadout_Defendants(sota, actor);
    sota->victims   = Map_Find_Victims(sota->map,   sota->victims,   x, y, actor);
    sota->passives  = Map_Find_Traders(sota->map,   sota->passives,  x, y);
    sota->auditors  = Map_Find_Auditors(sota->map,  sota->auditors,  x, y);
    sota->openables = Map_Find_Doors(sota->map,     sota->openables, x, y);
    sota->openables = Map_Find_Chests(sota->map,    sota->openables, x, y);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_PlayerSelectMenu_Create(struct Game *sota, i8 in_menu) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    SDL_assert((in_menu > MENU_PLAYER_SELECT_START) & (in_menu < MENU_PLAYER_SELECT_END));
    if (sota->player_select_menus[in_menu] != TNECS_NULL) {
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    tnecs_entity_t ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    sota->player_select_menus[in_menu] = ent;
    SDL_assert(ent > TNECS_NULL);
    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, ent, MenuComponent);
    mc->type                    = MENU_TYPE_PLAYER_SELECT;
    mc->draw                    = &PlayerSelectMenu_Draw;
    mc->visible                 = true;

    struct PlayerSelectMenu *psm = PlayerSelectMenu_Alloc();
    mc->data = psm;
    PlayerSelectMenu_Load(psm, sota->renderer, &mc->n9patch);

    SDL_assert(sota->pixelnours != NULL);
    psm->pixelnours = sota->pixelnours;
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    psm->id = in_menu;
    psm->pos.x = sota->settings.res.x / 3;
    psm->pos.y = sota->settings.res.y / 3;
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);

    SDL_assert(sota->player_select_menus[in_menu] > TNECS_NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_PlayerSelectMenu_Update(struct Game *sota, i8 in_playerselect_menu) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SOTA_Log_Debug("%d", in_playerselect_menu);
    SDL_assert((in_playerselect_menu > MENU_PLAYER_SELECT_START) & (in_playerselect_menu <
               MENU_PLAYER_SELECT_END));
    tnecs_entity_t ent = sota->player_select_menus[in_playerselect_menu];
    if (ent == TNECS_NULL) {
        SOTA_Log_Debug("menu %d is not loaded", in_playerselect_menu);
        exit(ERROR_NotLoaded);
    }
    void *data_1 = NULL;
    void *data_2 = NULL;
    switch (in_playerselect_menu) {
        default:
            break;
        case MENU_PLAYER_SELECT_UNIT_ACTION:
            // 1-
            break;
    }
    SDL_assert(ent > TNECS_NULL);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, ent, MenuComponent);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    SDL_assert(mc != NULL);
    struct PlayerSelectMenu *menu = mc->data;
    SDL_assert(menu != NULL);

    /* - Computing new menu_options - */
    SDL_assert(menuContentMakers[in_playerselect_menu] != NULL);
    menuContentMakers[in_playerselect_menu](sota, data_1, data_2);
    mc->elem_num = menu->option_num;
    PlayerSelectMenu_Elem_Links(menu, mc);
    PlayerSelectMenu_Elem_Boxes(menu, mc);
    PlayerSelectMenu_Elem_Pos(menu, mc);
    MenuComponent_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    menu->update = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
/* --- WeaponSelectMenu --- */
void Game_WeaponSelectMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    if (sota->weapon_select_menu == TNECS_NULL)
        sota->weapon_select_menu =  TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, MenuComponent);
    mc->type = MENU_TYPE_WEAPON_SELECT;
    mc->draw = &LoadoutSelectMenu_Draw;

    // /* n9patch init */
    // mc->n9patch.patch_pixels.x  = MENU_PATCH_PIXELS;
    // mc->n9patch.patch_pixels.y  = MENU_PATCH_PIXELS;
    // mc->n9patch.size_patches.x  = WSM_PATCH_X_SIZE;
    // mc->n9patch.size_patches.y  = WSM_PATCH_X_SIZE;
    // mc->n9patch.scale.x         = WSM_N9PATCH_SCALE_X;
    // mc->n9patch.scale.y         = WSM_N9PATCH_SCALE_Y;
    // mc->n9patch.size_pixels.x   = MENU_PATCH_PIXELS * WSM_PATCH_X_SIZE;
    // mc->n9patch.size_pixels.y   = MENU_PATCH_PIXELS * WSM_PATCH_Y_SIZE;

    /* stats_menu struct init */
    struct LoadoutSelectMenu *wsm   = LoadoutSelectMenu_Alloc();
    wsm->pos.x                      = sota->settings.res.x / 2;
    wsm->pos.y                      = sota->settings.res.y / 2;
    wsm->archetype_stronghand       = ITEM_ARCHETYPE_WEAPON;
    mc->data                        = wsm;
    mc->visible                     = true;
    mc->elem_links                  = wsm_links;
    mc->elem_pos                    = wsm_elem_pos;
    mc->elem_box                    = wsm_elem_box;
    mc->elem_num                    = WSM_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->pixelnours     != NULL);
    SDL_assert(sota->pixelnours_big != NULL);
    wsm->pixelnours             = sota->pixelnours;
    wsm->pixelnours_big         = sota->pixelnours_big;
    LoadoutSelectMenu_Elem_Pos(wsm, mc);
    MenuComponent_Elem_Boxes_Check(mc);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_WeaponSelectMenu_Update(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(unit_ontile->weapons_dtab != NULL);

    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->weapon_select_menu, MenuComponent);
    mc->visible = true;

    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(mc->elem_pos == wsm_elem_pos);
    // LoadoutSelectMenu_Load(wsm, unit_ontile, sota->renderer);
    WeaponSelectMenu_Load(wsm, sota->map, sota->world, unit_entity_ontile, sota->renderer,
                          &mc->n9patch);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    LoadoutSelectMenu_Elem_Reset(wsm, mc);
    LoadoutSelectMenu_Elem_Pos(  wsm, mc);
    MenuComponent_Elem_Boxes_Check(mc);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_WeaponSelectMenu_Enable(struct Game *sota, tnecs_entity_t uent_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->weapon_select_menu == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->weapon_select_menu);
    SDL_assert(sota->weapon_select_menu > 0);
    Game_WeaponSelectMenu_Update(sota, uent_ontile);
    strncpy(sota->reason, "WeaponSelectMenu was created", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->weapon_select_menu, NULL);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- TradeMenu --- */
void Game_TradeMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->trade_menu == TNECS_NULL)
        sota->trade_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->trade_menu, MenuComponent);
    mc->type =                 MENU_TYPE_TRADE;
    // mc->draw =                 &TradeMenu_Draw;

    /* n9patch init */
    mc->n9patch.patch_pixels.x =    MENU_PATCH_PIXELS;
    mc->n9patch.patch_pixels.y =    MENU_PATCH_PIXELS;
    mc->n9patch.size_patches.x =    WSM_PATCH_X_SIZE * 2;
    mc->n9patch.size_patches.y =    WSM_PATCH_Y_SIZE;
    mc->n9patch.scale.x =           WSM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y =           WSM_N9PATCH_SCALE_Y;
    mc->n9patch.size_pixels.x =     MENU_PATCH_PIXELS * mc->n9patch.size_patches.x;
    mc->n9patch.size_pixels.y =     MENU_PATCH_PIXELS * mc->n9patch.size_patches.y;
    mc->n9patch.texture =           Filesystem_Texture_Load(sota->renderer, sota->filename_menu,
                                                            SDL_PIXELFORMAT_INDEX8);
    /* stats_menu struct init */
    // struct TradeMenu *tm =          TradeMenu_Alloc();
    // tm->active =                    LoadoutSelectMenu_Alloc();
    // tm->passive =                   LoadoutSelectMenu_Alloc();

    // mc->data =                      tm;
    // mc->visible =                   true;
    // mc->elem_links =                trade_menu_links;
    // mc->elem =                      0;
    // mc->elem_pos =                  malloc(sizeof(*mc->elem_pos) * TRADE_MENU_ELEMS_NUM);
    // mc->elem_box =                  trade_menu_elem_box;
    // mc->elem_num =                  TRADE_MENU_ELEMS_NUM;

    // // TODO: copy descriptions
    // // mc->elem_description = stats_menu_description;
    // SDL_assert(sota->pixelnours     != NULL);
    // SDL_assert(sota->pixelnours_big != NULL);
    // tm->active->tophand_stronghand     = sota->settings.tophand_stronghand;
    // tm->passive->tophand_stronghand     = sota->settings.tophand_stronghand;
    // tm->active->pixelnours =        sota->pixelnours;
    // tm->active->pixelnours_big =    sota->pixelnours_big;
    // tm->passive->pixelnours =       sota->pixelnours;
    // tm->passive->pixelnours_big =   sota->pixelnours_big;

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_TradeMenu_Update(struct Game *sota, tnecs_entity_t selected, tnecs_entity_t candidate) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(selected     > TNECS_NULL);
    SDL_assert(candidate    > TNECS_NULL);
    struct Unit *active     = TNECS_GET_COMPONENT(sota->world, selected, Unit);
    struct Unit *passive    = TNECS_GET_COMPONENT(sota->world, candidate, Unit);
    SDL_assert(active   != NULL);
    SDL_assert(passive  != NULL);
    SDL_assert(active->weapons_dtab     != NULL);
    SDL_assert(passive->weapons_dtab    != NULL);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->trade_menu, MenuComponent);

    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    struct TradeMenu *tm = (struct TradeMenu *)mc->data;
    // ism->update = true;
    // LoadoutSelectMenu_Load(tm->active,  active,  sota->renderer);
    // LoadoutSelectMenu_Load(tm->passive, passive, sota->renderer);
    SDL_assert(tm->active->unit != NULL);
    SDL_assert(tm->passive->unit != NULL);

    tm->active->pos.x   = sota->settings.res.x / 4;
    tm->active->pos.y   = sota->settings.res.y / 3;
    tm->passive->pos.y  = tm->active->pos.y;

    // TradeMenu_Elem_Pos(tm, mc);

    /* scaling elem_pos: put it last cause dependences */
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_TradeMenu_Enable(struct Game *sota, tnecs_entity_t selected, tnecs_entity_t candidate) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // if (sota->trade_menu == TNECS_NULL)
    //     Game_TradeMenu_Create(sota);
    // SDL_assert(sota->trade_menu > TNECS_NULL);

    // Game_menuStack_Push(sota,   sota->trade_menu);
    // Game_TradeMenu_Update(sota, selected,   candidate);
    // strncpy(sota->reason, "TradeMenu was created", sizeof(sota->reason));
    // Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->trade_menu, NULL);
    // Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- ItemSelectMenu --- */
void Game_ItemSelectMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);

    if (sota->item_select_menu == TNECS_NULL)
        sota->item_select_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->item_select_menu, MenuComponent);
    mc->type                        = MENU_TYPE_ITEM_SELECT;
    mc->draw                        = &LoadoutSelectMenu_Draw;

    /* n9patch init */
    mc->n9patch.patch_pixels.x      = MENU_PATCH_PIXELS;
    mc->n9patch.patch_pixels.y      = MENU_PATCH_PIXELS;
    mc->n9patch.size_patches.x      = ISM_PATCH_X_SIZE;
    mc->n9patch.size_patches.y      = ISM_PATCH_Y_SIZE;
    mc->n9patch.scale.x             = ISM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y             = ISM_N9PATCH_SCALE_Y;
    mc->n9patch.size_pixels.x       = MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE;
    mc->n9patch.size_pixels.y       = MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE;
    mc->n9patch.texture             = Filesystem_Texture_Load(sota->renderer, sota->filename_menu,
                                                              SDL_PIXELFORMAT_INDEX8);

    /* stats_menu struct init */
    struct LoadoutSelectMenu *ism =      LoadoutSelectMenu_Alloc();
    ism->pos.x =                    sota->settings.res.x / 2;
    ism->pos.y =                    sota->settings.res.y / 2;
    mc->data =                      ism;
    mc->visible =                   true;
    // mc->elem_links =                item_select_menu_links;
    // mc->elem_pos =                  item_select_menu_elem_pos;
    // mc->elem_box =                  item_select_menu_elem_box;
    // mc->elem_num =                  ISM_MENU_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->pixelnours     != NULL);
    SDL_assert(sota->pixelnours_big != NULL);
    ism->pixelnours             = sota->pixelnours;
    ism->pixelnours_big         = sota->pixelnours_big;

    MenuComponent_Elem_Boxes_Check(mc);
    LoadoutSelectMenu_Elem_Pos(ism, mc);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_ItemSelectMenu_Update(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(unit_ontile->weapons_dtab != NULL);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->item_select_menu, MenuComponent);

    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    struct LoadoutSelectMenu *ism = mc->data;
    // ism->update = true;
    // SDL_assert(mc->elem_pos == item_select_menu_elem_pos);
    // LoadoutSelectMenu_Load(ism, unit_ontile, sota->renderer);

    /* scaling elem_pos: put it last cause dependences */
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_ItemSelectMenu_Enable(struct Game *sota, tnecs_entity_t uent_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->item_select_menu == TNECS_NULL)
        Game_ItemSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->item_select_menu);
    SDL_assert(sota->item_select_menu > 0);
    Game_ItemSelectMenu_Update(sota, uent_ontile);
    strncpy(sota->reason, "ItemSelectMenu was created", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->item_select_menu, NULL);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- StaffSelectMenu --- */
void Game_StaffSelectMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->staff_select_menu == TNECS_NULL)
        sota->staff_select_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    else {
        // TODO: destroy menu?
    }
    struct MenuComponent *mc;
    mc          = TNECS_GET_COMPONENT(sota->world, sota->staff_select_menu, MenuComponent);
    mc->type    = MENU_TYPE_STAFF_SELECT;
    mc->draw    = &LoadoutSelectMenu_Draw;

    /* stats_menu struct init */
    struct LoadoutSelectMenu *ssm    = LoadoutSelectMenu_Alloc();
    ssm->pos.x                  = sota->settings.res.x / 2;
    ssm->pos.y                  = sota->settings.res.y / 2;
    ssm->archetype_stronghand   = ITEM_ARCHETYPE_STAFF;
    mc->data                    = ssm;
    mc->visible                 = true;
    mc->elem_links              = ssm_links;
    mc->elem_pos                = ssm_elem_pos;
    mc->elem_box                = ssm_elem_box;
    mc->elem_num                = SSM_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->pixelnours     != NULL);
    SDL_assert(sota->pixelnours_big != NULL);
    ssm->pixelnours                  = sota->pixelnours;
    ssm->pixelnours_big              = sota->pixelnours_big;
    MenuComponent_Elem_Boxes_Check(mc);
    LoadoutSelectMenu_Elem_Pos(ssm, mc);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_StaffSelectMenu_Update(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(unit_ontile->weapons_dtab != NULL);

    struct MenuComponent *mc;
    mc = TNECS_GET_COMPONENT(sota->world, sota->staff_select_menu, MenuComponent);
    mc->visible = true;

    struct LoadoutSelectMenu *ssm = mc->data;
    SDL_assert(mc->elem_pos == ssm_elem_pos);
    StaffSelectMenu_Load(ssm, sota->map, sota->world, unit_entity_ontile, sota->renderer, &mc->n9patch);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    mc->elem_num = ssm->unit->num_usable;
    for (int i = mc->elem_num - 1; i < DEFAULT_EQUIPMENT_SIZE; i++) {
        mc->elem_links[i].top    = WSM_ELEM_NULL;
        mc->elem_links[i].bottom = WSM_ELEM_NULL;
    }

    MenuComponent_Elem_Boxes_Check(mc);

    // SDL_assert(unit_entity_ontile > TNECS_NULL);
    // struct Unit *unit_ontile = TNECS_GET_COMPONENT(sota->world, unit_entity_ontile, Unit);
    // SDL_assert(unit_ontile != NULL);
    // SDL_assert(unit_ontile->weapons_dtab != NULL);
    // struct MenuComponent *mc;
    // mc = TNECS_GET_COMPONENT(sota->world, sota->staff_select_menu, MenuComponent);

    // SDL_assert(mc->n9patch.patch_pixels.x > 0);
    // SDL_assert(mc->n9patch.patch_pixels.y > 0);
    // mc->visible = true;
    // struct LoadoutSelectMenu *ssm = mc->data;
    // // ssm->update = true;
    // SDL_assert(mc->elem_pos == ssm_elem_pos);

    // LoadoutSelectMenu_Load(ssm, unit_ontile, sota->renderer);
    // /* scaling elem_pos: put it last cause dependences */


    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_StaffSelectMenu_Enable(struct Game *sota, tnecs_entity_t unit_entity_ontile) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->staff_select_menu == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->staff_select_menu);
    SDL_assert(sota->staff_select_menu > 0);
    Game_StaffSelectMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->reason, "STAFF_SELECT_MENU was created", sizeof(sota->reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->staff_select_menu, NULL);
    Game_cursorFocus_onMenu(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Menu location --- */
void Game_Menu_LocationfromUnit(struct Game *sota, tnecs_entity_t in_menu_entity,
                                tnecs_entity_t in_unit_entity) {

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct MenuComponent *mc = NULL;
    struct Position *unit_pos = NULL;
    SDL_assert(in_unit_entity > 0);
    SDL_assert(in_menu_entity > 0);
    unit_pos = TNECS_GET_COMPONENT(sota->world, in_unit_entity, Position);
    mc = TNECS_GET_COMPONENT(sota->world, in_menu_entity, MenuComponent);
    SDL_assert(mc != NULL);
    // if (unit_pos != NULL) {
    //     if (unit_pos->onTilemap) {
    //         menu->n9patch.pos.x = unit_pos->tilemap_pos.x * sota->map->tilesize[0];
    //         menu->n9patch.pos.y = unit_pos->tilemap_pos.y * sota->map->tilesize[0];
    //     } else {
    //         menu->n9patch.pos.x = unit_pos->pixel_pos.x;
    //         menu->n9patch.pos.y = unit_pos->pixel_pos.y;
    //     }
    // } else {
    // mc->n9patch.pos.x = sota->settings.res.x / 2;
    // mc->n9patch.pos.y = sota->settings.res.y / 2;
    // }
    // SDL_assert(mc->n9patch.pixel_pos.x > 0);
    // SDL_assert(mc->n9patch.pixel_pos.y > 0);
    // SDL_assert(mc->n9patch.patch_pixels.x > 0);
    // SDL_assert(mc->n9patch.patch_pixels.y > 0);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Menu_LocationfromCursor(struct Game *sota, tnecs_entity_t in_menu_entity) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(sota->entity_cursor);
    SDL_assert(in_menu_entity > 0);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, in_menu_entity,
                                                   MenuComponent);
    SDL_assert(mc != NULL);
    // if (cursor_pos != NULL) {
    //     if (cursor_pos->onTilemap) {
    //         menu->n9patch.pos.x = cursor_pos->tilemap_pos.x * sota->map->tilesize[0];
    //         menu->n9patch.pos.y = cursor_pos->tilemap_pos.y * sota->map->tilesize[0];
    //     } else {
    //         menu->n9patch.pos.x = cursor_pos->pixel_pos.x;
    //         menu->n9patch.pos.y = cursor_pos->pixel_pos.y;
    //     }
    // } else {
    mc->n9patch.pos.x = sota->settings.res.x / 2;
    mc->n9patch.pos.y = sota->settings.res.y / 2;
    // }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Title Screen --- */
void Game_FirstMenu_Update(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->first_menu == 0) {
        SOTA_Log_Debug("First menu is not loaded");
        exit(ERROR_Generic);
    }
    SDL_assert(sota->first_menu > TNECS_NULL);
    struct MenuComponent *mc = TNECS_GET_COMPONENT(sota->world, sota->first_menu, MenuComponent);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    struct PlayerSelectMenu *menu = mc->data;
    SDL_assert(menu != NULL);
    makeContent_FirstMenu(sota);
    mc->elem_num = menu->option_num;
    PlayerSelectMenu_Elem_Links(menu, mc);
    PlayerSelectMenu_Elem_Boxes(menu, mc);
    PlayerSelectMenu_Elem_Pos(menu, mc);
    MenuComponent_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_FirstMenu_Destroy(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->first_menu != TNECS_NULL) {
        struct MenuComponent *mc;
        mc = TNECS_GET_COMPONENT(sota->world, sota->first_menu, MenuComponent);
        SDL_DestroyTexture(mc->n9patch.texture);
        PlayerSelectMenu_Free(mc->data, mc);
        mc->data = NULL;
        tnecs_entity_destroy(sota->world, sota->first_menu);
    }
    /* only first_menu should be on the stack */
    SDL_assert(DARR_NUM(sota->menu_stack) == 1);
    tnecs_entity_t first_menu = DARR_POP(sota->menu_stack);
    SDL_assert(first_menu == sota->first_menu);
    SDL_assert(DARR_NUM(sota->menu_stack) == 0);

    sota->first_menu = TNECS_NULL;
    Game_Title_Destroy(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Title_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_SetRenderDrawColor(sota->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    if (sota->title != TNECS_NULL) {
        SOTA_Log_Debug("Title is already loaded");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }

    sota->title = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position, Text);

    /* -- Get position -- */
    struct Position *position = TNECS_GET_COMPONENT(sota->world, sota->title, Position);
    position->pixel_pos.x = sota->settings.res.x / 20;
    position->pixel_pos.y = sota->settings.res.y / 10;
    position->scale[0] = 10;
    position->scale[1] = 10;

    /* -- Get text -- */
    struct Text *text = TNECS_GET_COMPONENT(sota->world, sota->title, Text);
    *text = Text_default;

    /* - Load pixelfont - */
    text->pixelfont = PixelFont_Alloc();
    text->pixelfont->glyph_width  = 16;
    text->pixelfont->glyph_height = 16;
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_gothic.png");
    PixelFont_Load(text->pixelfont, sota->renderer, path);

    /* - Set title - */
    Text_Set(text, GAME_TITLE);
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));
    SDL_assert(sota->pixelnours_big != NULL);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_Title_Destroy(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->title != TNECS_NULL) {
        struct Text *text = TNECS_GET_COMPONENT(sota->world, sota->title, Text);

        if (text != NULL)
            PixelFont_Free(text->pixelfont, true);

        tnecs_entity_destroy(sota->world, sota->title);
        sota->title = TNECS_NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_FirstMenu_Create(struct Game *sota) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (sota->first_menu != TNECS_NULL) {
        SOTA_Log_Debug("FirstMenu is already loaded");
        SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return;
    }
    sota->first_menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, MenuComponent);
    struct MenuComponent *mc;
    struct PlayerSelectMenu *psm = PlayerSelectMenu_Alloc();
    mc = TNECS_GET_COMPONENT(sota->world, sota->first_menu, MenuComponent);
    mc->data    = psm;
    mc->type    = MENU_TYPE_PLAYER_SELECT;
    mc->draw    = &PlayerSelectMenu_Draw;
    mc->visible = true;

    FirstMenu_Load(psm, sota->renderer, &mc->n9patch);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    psm->row_height = ASCII_GLYPH_HEIGHT + 2; /* pixel fonts have python8 pixels*/
    psm->pixelnours = sota->pixelnours;
    SDL_assert(sota->pixelnours != NULL);
    psm->id = sota->first_menu;
    psm->pos.x = sota->settings.res.x / 3;
    psm->pos.y = sota->settings.res.y / 3;
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Game_titleScreen_Load(struct Game *sota, struct Input_Arguments in_args) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_SetRenderDrawColor(sota->renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    Game_FirstMenu_Create(sota);
    Game_FirstMenu_Update(sota);
    Game_menuStack_Push(sota, sota->first_menu);
    Game_Title_Create(sota);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
