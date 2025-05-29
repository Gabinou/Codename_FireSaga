
#include "game/map.h"
#include "game/menu.h"
#include "game/cursor.h"
#include "game/game.h"
#include "platform.h"
#include "nmath.h"
#include "map/path.h"
#include "map/find.h"
#include "map/map.h"
#include "macros.h"
#include "filesystem.h"
#include "pixelfonts.h"
#include "menu/deployment.h"
#include "menu/stats.h"
#include "menu/trade.h"
#include "menu/staff_select.h"
#include "menu/growths.h"
#include "menu/player_select.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "text.h"
#include "position.h"
#include "events.h"
#include "structs.h"
#include "graph.h"
#include "globals.h"

/* --- All Menus --- */
void Game_Menus_Init(struct Game *sota) {
    if (sota->menus.stack != NULL)
        DARR_FREE(sota->menus.stack);
    sota->menus.stack = DARR_INIT(sota->menus.stack, tnecs_entity, MENU_PLAYER_SELECT_END);
    memset(sota->menus.stack, 0, MENU_PLAYER_SELECT_END * sizeof(*sota->menus.stack));
}

void Game_Switch_toCandidates(struct Game *sota, tnecs_entity *candidates) {
    sota->targets.candidates = candidates;
    Game_subState_Set(sota, GAME_SUBSTATE_MAP_CANDIDATES, sota->debug.reason);

    /* - set cursor position on tilemap - */
    Game_cursorFocus_onMap(sota);

    /* - moving cursor moves to next entity-> go to its position on tilemap - */
    struct Position *candidate_pos, *cursor_pos;
    int index = (sota->targets.previous_order > -1) ? sota->targets.previous_order : 0;
    tnecs_entity candidate = sota->targets.candidates[index];

    candidate_pos = IES_GET_COMPONENT(sota->ecs.world, candidate, Position);
    SDL_assert(candidate_pos != NULL);
    cursor_pos = IES_GET_COMPONENT(sota->ecs.world, sota->cursor.entity, Position);
    SDL_assert(cursor_pos != NULL);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;

    /* - Hover on first candidate - */
    *data2_entity = sota->targets.candidates[index];
    Event_Emit(__func__, SDL_USEREVENT, event_Cursor_Hovers_Unit, NULL, data2_entity);
}

/* --- Menu stack --- */
void Game_DeploymentMenu_Free(struct Game *sota) {
    if (sota->menus.deployment == TNECS_NULL)
        return;

    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.deployment, Menu);
    SDL_assert(mc);

    struct DeploymentMenu *dm = mc->data;
    DeploymentMenu_Free(dm);
    mc->data = NULL;
    sota->menus.deployment = TNECS_NULL;
}

void Game_menuStack_Free(struct Game *sota) {
    struct StatsMenu        *stats_menu;
    struct PlayerSelectMenu *psm_menu;
    SDL_assert(sota->menus.stack != NULL);
    while (DARR_NUM(sota->menus.stack) > 0) {
        tnecs_entity entity = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
        SDL_assert(TNECS_ENTITY_EXISTS(sota->ecs.world, entity));
        struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, entity, Menu);

        if (mc->data == NULL) {
            DARR_POP(sota->menus.stack);
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
        DARR_POP(sota->menus.stack);
    }

    DARR_FREE(sota->menus.stack);
    sota->menus.stack = NULL;
}

void Game_menuStack_Push(struct Game *sota, tnecs_entity in_menu_entity) {
    SDL_assert(in_menu_entity > 0);
    DARR_PUT(sota->menus.stack, in_menu_entity);
}

tnecs_entity Game_menuStack_Pop(struct Game *sota, b32 destroy) {
    tnecs_entity menu_stack_top_entity = DARR_POP(sota->menus.stack);
    SDL_assert(menu_stack_top_entity > 0);
    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, menu_stack_top_entity, Menu);
    mc->visible = false;

    if (destroy) {
    }
    return (menu_stack_top_entity);
}

/* --- Deployment --- */
void Game_DeploymentMenu_Create(struct Game *sota) {
    if (sota->menus.deployment == TNECS_NULL)
        sota->menus.deployment = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.deployment, Menu);
    SDL_assert(mc != NULL);
    mc->type        = MENU_TYPE_DEPLOYMENT;
    mc->draw        = &DeploymentMenu_Draw;

    struct DeploymentMenu *dm = DeploymentMenu_Alloc();
    SDL_assert(dm != NULL);
    mc->data = dm;

    dm->pos.x       = sota->settings.res.x / 10;
    dm->pos.y       = sota->settings.res.y / 4;
    dm->world       = sota->ecs.world;
    mc->visible     = true;
    mc->elem_links  = dm_links;
    mc->elem_pos    = dm_elem_pos;
    mc->elem_box    = dm_elem_box;
    mc->elem_num    = DM_ELEM_NUM;

    SDL_assert(mc->elem_pos != NULL);
    DeploymentMenu_Load(dm, sota->render.er, &mc->n9patch);
    DeploymentMenu_Elem_Pos(dm, mc);


    /* Set position of cursor to go back to after cancelling */
    Map *map = Game_Map(sota);
    SDL_assert(map                  != NULL);
    SDL_assert(map->start_pos.arr   != NULL);
    SDL_assert(DARR_NUM(map->start_pos.arr) > 0);

    sota->cursor.lastpos.x = map->start_pos.arr[0].x;
    sota->cursor.lastpos.y = map->start_pos.arr[0].y;
}

void Game_DeploymentMenu_Update(struct Game *sota) {
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.deployment, Menu);
    mc->visible = true;
    struct DeploymentMenu *dm = mc->data;
    DeploymentMenu_Party_Set(dm, &sota->party);
    DeploymentMenu_Elem_Links(dm, mc);
    dm->update = true;
    Map *map = Game_Map(sota);
    DeploymentMenu_Map_Set(dm, map);
}

void Game_DeploymentMenu_Enable(struct Game *sota) {
    if (sota->menus.deployment == 0)
        Game_DeploymentMenu_Create(sota);
    SDL_assert(sota->menus.deployment > 0);
    Game_menuStack_Push(sota, sota->menus.deployment);
    Game_DeploymentMenu_Update(sota);
    strncpy(sota->debug.reason, "deployment_menu was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.stats, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- GrowthsMenu --- */
void Game_GrowthsMenu_Enable(struct Game *sota, tnecs_entity unit_entity_ontile) {
    if (sota->menus.growths == 0)
        Game_GrowthsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.growths);
    SDL_assert(sota->menus.growths > 0);
    Game_GrowthsMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->debug.reason, "growths_menu was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.stats, NULL);
    Game_cursorFocus_onMenu(sota);
}

void Game_GrowthsMenu_Create(struct Game *sota) {
    if (sota->menus.growths == TNECS_NULL)
        sota->menus.growths = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.growths, Menu);
    SDL_assert(mc != NULL);
    mc->type        = MENU_TYPE_GROWTHS;
    mc->draw        = &GrowthsMenu_Draw;

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
    mc->elem_links  = gm_links;
    mc->elem_pos    = gm_elem_pos;
    mc->elem_box    = gm_elem_box;
    mc->elem_num    = GM_ELEM_NUM;

    SDL_assert(mc->elem_pos != NULL);
    GrowthsMenu_Load(GM, sota->render.er, &mc->n9patch);

    SDL_assert(sota->fonts.pixelnours != NULL);
    GM->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    GM->pixelnours_big = sota->fonts.pixelnours_big;
    /* scaling elem_pos: put it last cause dependences */
    GrowthsMenu_Elem_Pos(GM, mc);
    Menu_Elem_Boxes_Check(mc);
    mc->elem_box[GM_GROWTHS].x *= 2;
}

void Game_GrowthsMenu_Update(struct Game *sota, tnecs_entity unit_entity_ontile) {
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.growths, Menu);
    mc->visible = true;
    SDL_assert(gl_weapons_dtab != NULL);
    struct GrowthsMenu *growths_menu = (struct GrowthsMenu *)mc->data;
    SDL_assert(gl_weapons_dtab != NULL);
    GrowthsMenu_Unit_Set(growths_menu, unit_ontile);
}

/* --- StatsMenu --- */
void Game_StatsMenu_Create(struct Game *sota) {

    // creates statsmenu entity, sets sizes, fonts
    if (sota->menus.stats == TNECS_NULL)
        sota->menus.stats = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.stats, Menu);
    mc->type        = MENU_TYPE_STATS;
    mc->draw        = &StatsMenu_Draw;

    /* stats_menu struct init */
    struct StatsMenu *stats_menu = StatsMenu_Alloc();
    stats_menu->pos.x = sota->settings.res.x / 4;
    stats_menu->pos.y = sota->settings.res.y / 5;
    mc->data          = stats_menu;
    mc->visible       = true;
    mc->elem_links    = sm_links;
    mc->elem_pos      = sm_elem_pos;
    mc->elem_box      = stats_menu_elem_box;
    mc->elem_num      = STATS_MENU_ELEMS_NUM;

    // TODO: copy descriptions
    SDL_assert(sota->fonts.pixelnours     != NULL);
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    stats_menu->pixelnours      = sota->fonts.pixelnours;
    stats_menu->pixelnours_big  = sota->fonts.pixelnours_big;
}

void Game_StatsMenu_Update(struct Game *sota, tnecs_entity unit_entity_ontile) {
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    /* - Get unit on tile - */
    struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    /* - Update stats menu - */
    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.stats, Menu);
    mc->visible = true;
    SDL_assert(gl_weapons_dtab != NULL);
    struct StatsMenu *stats_menu = (struct StatsMenu *)mc->data;
    SDL_assert(gl_weapons_dtab != NULL);
    StatsMenu_Load(stats_menu, unit_ontile, sota->render.er, &mc->n9patch);
    // Scaling elem_pos: put it last cause dependencies
    StatsMenu_Elem_Pos(stats_menu, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_StatsMenu_Enable(struct Game *sota, tnecs_entity unit_entity_ontile) {
    if (sota->menus.stats == 0)
        Game_StatsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.stats);
    SDL_assert(sota->menus.stats > 0);
    Game_StatsMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->debug.reason, "STATS_MENU was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.stats, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- PlayerSelectMenu --- */
void Game_PlayerSelectMenus_Free(struct Game *sota) {
    for (size_t i = 0; i < MENU_PLAYER_SELECT_NUM; i++) {
        if (sota->menus.player_select[i] == TNECS_NULL)
            continue;

        tnecs_entity ent = sota->menus.player_select[i];
        struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, ent, Menu);

        if (mc->data == NULL)
            continue;

        struct PlayerSelectMenu *psm_menu = mc->data;
        PlayerSelectMenu_Free(psm_menu, mc);
        mc->data = NULL;
    }
}

void Game_postLoadout_Defendants(struct Game *sota, tnecs_entity actor) {
    /* -- Finding possible defendants with equipped weapons -- */
    DARR_NUM(sota->targets.defendants) = 0;

    /* - Compute attacktolist - */
    SDL_assert(sota->menus.weapon_select > TNECS_NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.weapon_select, Menu);
    SDL_assert(mc != NULL);
    struct LoadoutSelectMenu *lsm = mc->data;
    SDL_assert(lsm != NULL);

    SDL_assert(Loadout_isEquipped(&lsm->selected, UNIT_HAND_LEFT));
    SDL_assert(Loadout_isEquipped(&lsm->selected, UNIT_HAND_RIGHT));

    /* - MapAct settings for attacktolist - */
    MapAct map_to       = MapAct_default;
    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = actor;

    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);

    /* Find all Defendants */
    // matrix_print(map->darrs.attacktomap, Map_row_len(map), Map_col_len(map));

    MapFind mapfind     = MapFind_default;

    mapfind.list        = map->darrs.attacktolist;
    mapfind.found       = sota->targets.defendants;
    mapfind.seeker      = actor;
    mapfind.fastquit    = false;
    mapfind.eq_type     = LOADOUT_EQUIPPED;

    sota->targets.defendants = Map_Find_Defendants(map, mapfind);
}

void Game_postLoadout_Patients(struct Game *sota, tnecs_entity actor) {
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    struct Unit *unit = IES_GET_COMPONENT(sota->ecs.world, actor, Unit);
    SDL_assert(Unit_canStaff(unit));
    DARR_NUM(sota->targets.patients) = 0;

    SDL_assert(sota->menus.staff_select > TNECS_NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.staff_select, Menu);
    SDL_assert(mc != NULL);
    struct LoadoutSelectMenu *lsm = mc->data;
    SDL_assert(lsm != NULL);

    /* - Compute healtolist - */
    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;
    Loadout *loadout = (Loadout *)&map_to;
    Loadout_Set(loadout, UNIT_HAND_LEFT,   Loadout_Eq(&lsm->selected, UNIT_HAND_LEFT));
    Loadout_Set(loadout, UNIT_HAND_RIGHT,  Loadout_Eq(&lsm->selected, UNIT_HAND_RIGHT));

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_LOADOUT;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = actor;

    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);

    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);

    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.attacktolist;
    mapfind.found      = sota->targets.defendants;
    mapfind.seeker     = actor;
    mapfind.fastquit   = false;
    mapfind.eq_type    = LOADOUT_EQUIPPED;

    /* Find Defendants if any */
    sota->targets.defendants = Map_Find_Defendants(map, mapfind);
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    mapfind = MapFind_default;

    mapfind.list       = map->darrs.healtolist;
    mapfind.found      = sota->targets.patients;
    mapfind.seeker     = actor;
    mapfind.fastquit   = false;
    mapfind.eq_type    = LOADOUT_EQUIPPED;


    /* Find all Patients if any */
    sota->targets.patients = Map_Find_Patients(map, mapfind);
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

}

/* -- Finding if any staff equipment has a patient -- */
void Game_preLoadout_Patients(struct Game *sota, tnecs_entity actor) {
    DARR_NUM(sota->targets.patients) = 0;

    /* --- Compute healtolist --- */
    /* -- MapAct settings for healtolist -- */
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_STAFF;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = actor;

    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);
    // SDL_Log("map->darrs.healtolist %d", DARR_NUM(map->darrs.healtolist) / TWO_D);

    // matrix_print(map->darrs.healtomap, Map_row_len(map), Map_col_len(map));

    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.healtolist;
    mapfind.found      = sota->targets.patients;
    mapfind.seeker     = actor;
    mapfind.fastquit   = true;
    mapfind.eq_type    = LOADOUT_EQUIPMENT;

    sota->targets.patients = Map_Find_Patients(map, mapfind);
    SDL_assert(sota->targets.patients != sota->targets.defendants);
    // SDL_Log("sota->targets.patients %d", DARR_NUM(sota->targets.patients));
}

/* -- Finding if any weapon in equipment has a defendant -- */
void Game_preLoadout_Defendants(struct Game *sota, tnecs_entity actor) {
    DARR_NUM(sota->targets.defendants) = 0;
    /* --- Compute attacktolist --- */
    /* -- MapAct settings for attacktolist -- */
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = actor;

    Map *map = Game_Map(sota);
    Map_Act_To(map, map_to);

    /* Find Defendants if any */
    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.attacktolist;
    mapfind.found      = sota->targets.defendants;
    mapfind.seeker     = actor;
    mapfind.fastquit   = true;
    mapfind.eq_type    = LOADOUT_EQUIPMENT;

    sota->targets.defendants = Map_Find_Defendants(map, mapfind);
}

/* -- Decides which option are in UnitAction menu -- */
/* CONTEXT: Player just chose where unit moves.
    - Defendants (breakables), patients
        - Only check if ANY exist
        - Recomputation necessary AFTER player makes another choice later
    - Doors, auditors, victims, chests
        - Can be computed right now:
*/
void Game_preUnitAction_Targets(struct Game *sota, tnecs_entity actor) {
    /* -- Resetting lists -- */
    DARR_NUM(sota->targets.victims)     = 0;
    DARR_NUM(sota->targets.passives)    = 0;
    DARR_NUM(sota->targets.auditors)    = 0;
    DARR_NUM(sota->targets.openables)   = 0;
    DARR_NUM(sota->targets.spectators)  = 0;

    /* -- Getters -- */
    struct Unit     *unit = IES_GET_COMPONENT(sota->ecs.world, actor, Unit);
    struct Position *pos  = IES_GET_COMPONENT(sota->ecs.world, actor, Position);

    /* -- if dancer -- */
    int x = pos->tilemap_pos.x, y = pos->tilemap_pos.y;
    Map *map = Game_Map(sota);
    if (Unit_canDance(unit))
        sota->targets.spectators = Map_Find_Spectators(map, sota->targets.spectators, x, y);

    /* -- For everyone -- */
    Game_preLoadout_Patients(sota,   actor);
    Game_preLoadout_Defendants(sota, actor);
    sota->targets.victims   = Map_Find_Victims(map,   sota->targets.victims,   x, y, actor);
    sota->targets.passives  = Map_Find_Traders(map,   sota->targets.passives,  x, y);
    sota->targets.auditors  = Map_Find_Auditors(map,  sota->targets.auditors,  x, y);
    sota->targets.openables = Map_Find_Doors(map,     sota->targets.openables, x, y);
    sota->targets.openables = Map_Find_Chests(map,    sota->targets.openables, x, y);
}

void Game_PlayerSelectMenu_Create(struct Game *sota, i8 in_menu) {

    SDL_assert((in_menu > MENU_PLAYER_SELECT_START) & (in_menu < MENU_PLAYER_SELECT_END));
    if (sota->menus.player_select[in_menu] != TNECS_NULL) {
        return;
    }

    tnecs_entity ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    sota->menus.player_select[in_menu] = ent;
    SDL_assert(ent > TNECS_NULL);
    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, ent, Menu);
    mc->type                    = MENU_TYPE_PLAYER_SELECT;
    mc->draw                    = &PlayerSelectMenu_Draw;
    mc->visible                 = true;

    struct PlayerSelectMenu *psm = PlayerSelectMenu_Alloc();
    mc->data = psm;
    PlayerSelectMenu_Load(psm, sota->render.er, &mc->n9patch);

    SDL_assert(sota->fonts.pixelnours != NULL);
    psm->pixelnours = sota->fonts.pixelnours;
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    psm->id = in_menu;
    psm->pos.x = sota->settings.res.x / 3;
    psm->pos.y = sota->settings.res.y / 3;
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);

    SDL_assert(sota->menus.player_select[in_menu] > TNECS_NULL);
}

void Game_PlayerSelectMenu_Update(struct Game *sota, i8 in_playerselect_menu) {
    SDL_assert((in_playerselect_menu > MENU_PLAYER_SELECT_START) & (in_playerselect_menu <
               MENU_PLAYER_SELECT_END));
    tnecs_entity ent = sota->menus.player_select[in_playerselect_menu];
    if (ent == TNECS_NULL) {
        SDL_Log("menu %d is not loaded", in_playerselect_menu);
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
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, ent, Menu);
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
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    menu->update = true;
}
/* --- WeaponSelectMenu --- */
void Game_WeaponSelectMenu_Create(struct Game *sota) {

    if (sota->menus.weapon_select == TNECS_NULL)
        sota->menus.weapon_select =  TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.weapon_select, Menu);
    mc->type        = MENU_TYPE_WEAPON_SELECT;
    mc->draw        = &LoadoutSelectMenu_Draw;

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
    mc->elem_num                    = LSM_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->fonts.pixelnours     != NULL);
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    wsm->pixelnours             = sota->fonts.pixelnours;
    wsm->pixelnours_big         = sota->fonts.pixelnours_big;
}

void Game_WeaponSelectMenu_Update(struct Game *sota, tnecs_entity unit_entity_ontile) {
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    /* Find new canEquip */
    canEquip can_equip          = canEquip_default;
    can_equip.archetype         = ITEM_ARCHETYPE_WEAPON;
    can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;

    Map *map = Game_Map(sota);
    Map_canEquip(map, unit_entity_ontile, can_equip);
    SDL_assert(unit_ontile->can_equip.num > 0);

    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.weapon_select, Menu);
    mc->visible = true;

    struct LoadoutSelectMenu *wsm = mc->data;
    SDL_assert(mc->elem_pos == wsm_elem_pos);
    // LoadoutSelectMenu_Load(wsm, unit_ontile, sota->render.er);
    LoadoutSelectMenu_Select_Reset(wsm);
    WeaponSelectMenu_Load(wsm, map, sota->ecs.world, sota->render.er, &mc->n9patch);
    LoadoutSelectMenu_Unit(wsm, unit_entity_ontile);
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
    LoadoutSelectMenu_Elem_Reset(wsm, mc);
    LoadoutSelectMenu_Elem_Pos(  wsm, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_WeaponSelectMenu_Enable(struct Game *sota, tnecs_entity uent_ontile) {
    if (sota->menus.weapon_select == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.weapon_select);
    SDL_assert(sota->menus.weapon_select > 0);
    Game_WeaponSelectMenu_Update(sota, uent_ontile);
    strncpy(sota->debug.reason, "WeaponSelectMenu was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.weapon_select, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- TradeMenu --- */
void Game_TradeMenu_Create(struct Game *sota) {
    if (sota->menus.trade == TNECS_NULL)
        sota->menus.trade = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.trade, Menu);
    mc->type            = MENU_TYPE_TRADE;
    // mc->draw         = &TradeMenu_Draw;

    /* n9patch init */
    mc->n9patch.patch_pixels.x =    MENU_PATCH_PIXELS;
    mc->n9patch.patch_pixels.y =    MENU_PATCH_PIXELS;
    mc->n9patch.size_patches.x =    LSM_PATCH_X_SIZE * 2;
    mc->n9patch.size_patches.y =    LSM_PATCH_Y_SIZE;
    mc->n9patch.scale.x =           LSM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y =           LSM_N9PATCH_SCALE_Y;
    mc->n9patch.size_pixels.x =     MENU_PATCH_PIXELS * mc->n9patch.size_patches.x;
    mc->n9patch.size_pixels.y =     MENU_PATCH_PIXELS * mc->n9patch.size_patches.y;
    mc->n9patch.texture =           Filesystem_Texture_Load(sota->render.er,
                                                            sota->menus.filename.data,
                                                            SDL_PIXELFORMAT_INDEX8);
    /* stats_menu struct init */
    // struct TradeMenu *tm =          TradeMenu_Alloc();
    // tm->active =                    LoadoutSelectMenu_Alloc();
    // tm->passive =                   LoadoutSelectMenu_Alloc();

    // mc->data =                      tm;
    // mc->visible =                   true;
    // mc->elem_links =                trade_menu_links;
    // mc->elem =                      0;
    // mc->elem_pos =                  SDL_malloc(sizeof(*mc->elem_pos) * TRADE_MENU_ELEMS_NUM);
    // mc->elem_box =                  trade_menu_elem_box;
    // mc->elem_num =                  TRADE_MENU_ELEMS_NUM;

    // // TODO: copy descriptions
    // // mc->elem_description = stats_menu_description;
    // SDL_assert(sota->fonts.pixelnours     != NULL);
    // SDL_assert(sota->fonts.pixelnours_big != NULL);
    // tm->active->tophand_stronghand     = sota->settings.tophand_stronghand;
    // tm->passive->tophand_stronghand     = sota->settings.tophand_stronghand;
    // tm->active->pixelnours =        sota->fonts.pixelnours;
    // tm->active->pixelnours_big =    sota->fonts.pixelnours_big;
    // tm->passive->pixelnours =       sota->fonts.pixelnours;
    // tm->passive->pixelnours_big =   sota->fonts.pixelnours_big;

}

void Game_TradeMenu_Update(struct Game *sota, tnecs_entity selected, tnecs_entity candidate) {
    SDL_assert(selected     > TNECS_NULL);
    SDL_assert(candidate    > TNECS_NULL);
    struct Unit *active     = IES_GET_COMPONENT(sota->ecs.world, selected, Unit);
    struct Unit *passive    = IES_GET_COMPONENT(sota->ecs.world, candidate, Unit);
    SDL_assert(active   != NULL);
    SDL_assert(passive  != NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.trade, Menu);

    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    struct TradeMenu *tm = (struct TradeMenu *)mc->data;
    // ism->update = true;
    // LoadoutSelectMenu_Load(tm->active,  active,  sota->render.er);
    // LoadoutSelectMenu_Load(tm->passive, passive, sota->render.er);
    SDL_assert(tm->active->unit     > TNECS_NULL);
    SDL_assert(tm->passive->unit    > TNECS_NULL);

    tm->active->pos.x   = sota->settings.res.x / 4;
    tm->active->pos.y   = sota->settings.res.y / 3;
    tm->passive->pos.y  = tm->active->pos.y;

    // TradeMenu_Elem_Pos(tm, mc);

    /* scaling elem_pos: put it last cause dependences */
}

void Game_TradeMenu_Enable(struct Game *sota, tnecs_entity selected, tnecs_entity candidate) {
    // if (sota->menus.trade == TNECS_NULL)
    //     Game_TradeMenu_Create(sota);
    // SDL_assert(sota->menus.trade > TNECS_NULL);

    // Game_menuStack_Push(sota,   sota->menus.trade);
    // Game_TradeMenu_Update(sota, selected,   candidate);
    // strncpy(sota->debug.reason, "TradeMenu was created", sizeof(sota->debug.reason));
    // Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.trade, NULL);
    // Game_cursorFocus_onMenu(sota);
}

/* --- ItemSelectMenu --- */
void Game_ItemSelectMenu_Create(struct Game *sota) {

    if (sota->menus.item_select == TNECS_NULL)
        sota->menus.item_select = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.item_select, Menu);
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
    mc->n9patch.texture             = Filesystem_Texture_Load(sota->render.er,
                                                              sota->menus.filename.data,
                                                              SDL_PIXELFORMAT_INDEX8);

    /* stats_menu struct init */
    struct LoadoutSelectMenu *ism   = LoadoutSelectMenu_Alloc();
    ism->pos.x                      = sota->settings.res.x / 2;
    ism->pos.y                      = sota->settings.res.y / 2;
    mc->data                        = ism;
    mc->visible                     = true;
    // mc->elem_links =                item_select_menu_links;
    // mc->elem_pos =                  item_select_menu_elem_pos;
    // mc->elem_box =                  item_select_menu_elem_box;
    // mc->elem_num =                  ISM_MENU_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->fonts.pixelnours     != NULL);
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    ism->pixelnours             = sota->fonts.pixelnours;
    ism->pixelnours_big         = sota->fonts.pixelnours_big;

    Menu_Elem_Boxes_Check(mc);
    LoadoutSelectMenu_Elem_Pos(ism, mc);

}

void Game_ItemSelectMenu_Update(struct Game *sota, tnecs_entity unit_entity_ontile) {
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.item_select, Menu);

    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    mc->visible = true;
    // struct LoadoutSelectMenu *ism = mc->data;
    // ism->update = true;
    // SDL_assert(mc->elem_pos == item_select_menu_elem_pos);
    // LoadoutSelectMenu_Load(ism, unit_ontile, sota->render.er);

    /* scaling elem_pos: put it last cause dependences */
}

void Game_ItemSelectMenu_Enable(struct Game *sota, tnecs_entity uent_ontile) {
    if (sota->menus.item_select == TNECS_NULL)
        Game_ItemSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.item_select);
    SDL_assert(sota->menus.item_select > 0);
    Game_ItemSelectMenu_Update(sota, uent_ontile);
    strncpy(sota->debug.reason, "ItemSelectMenu was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.item_select, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- StaffSelectMenu --- */
void Game_StaffSelectMenu_Create(struct Game *sota) {
    if (sota->menus.staff_select == TNECS_NULL)
        sota->menus.staff_select = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc;
    mc          = IES_GET_COMPONENT(sota->ecs.world, sota->menus.staff_select, Menu);
    mc->type    = MENU_TYPE_STAFF_SELECT;
    mc->draw    = &LoadoutSelectMenu_Draw;

    /* n9patch init */
    mc->n9patch.patch_pixels.x      = MENU_PATCH_PIXELS;
    mc->n9patch.patch_pixels.y      = MENU_PATCH_PIXELS;
    mc->n9patch.size_patches.x      = ISM_PATCH_X_SIZE;
    mc->n9patch.size_patches.y      = ISM_PATCH_Y_SIZE;
    mc->n9patch.scale.x             = ISM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y             = ISM_N9PATCH_SCALE_Y;
    mc->n9patch.size_pixels.x       = MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE;
    mc->n9patch.size_pixels.y       = MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE;
    mc->n9patch.texture             = Filesystem_Texture_Load(sota->render.er,
                                                              sota->menus.filename.data,
                                                              SDL_PIXELFORMAT_INDEX8);

    /* stats_menu struct init */
    struct LoadoutSelectMenu *ssm   = LoadoutSelectMenu_Alloc();
    ssm->pos.x                      = sota->settings.res.x / 2;
    ssm->pos.y                      = sota->settings.res.y / 2;
    ssm->archetype_stronghand       = ITEM_ARCHETYPE_STAFF;
    mc->data                        = ssm;
    mc->visible                     = true;
    mc->elem_links                  = ssm_links;
    mc->elem_pos                    = ssm_elem_pos;
    mc->elem_box                    = ssm_elem_box;
    mc->elem_num                    = SSM_ELEMS_NUM;

    // TODO: copy descriptions
    // mc->elem_description = stats_menu_description;
    SDL_assert(sota->fonts.pixelnours     != NULL);
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    ssm->pixelnours                  = sota->fonts.pixelnours;
    ssm->pixelnours_big              = sota->fonts.pixelnours_big;
    Menu_Elem_Boxes_Check(mc);
    LoadoutSelectMenu_Elem_Pos(ssm, mc);
}

void Game_StaffSelectMenu_Update(struct Game *sota, tnecs_entity unit_entity_ontile) {
    SDL_assert(unit_entity_ontile > TNECS_NULL);
    struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    struct Menu *mc;
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.staff_select, Menu);
    mc->visible = true;

    struct LoadoutSelectMenu *ssm = mc->data;
    Map *map = Game_Map(sota);
    SDL_assert(mc->elem_pos == ssm_elem_pos);
    StaffSelectMenu_Load(ssm, map, sota->ecs.world, sota->render.er, &mc->n9patch);
    LoadoutSelectMenu_Unit(ssm, unit_entity_ontile);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    mc->elem_num = unit_ontile->can_equip.num;
    for (int i = mc->elem_num - 1; i < SOTA_EQUIPMENT_SIZE; i++) {
        mc->elem_links[i].top    = LSM_ELEM_NULL;
        mc->elem_links[i].bottom = LSM_ELEM_NULL;
    }

    Menu_Elem_Boxes_Check(mc);

    // SDL_assert(unit_entity_ontile > TNECS_NULL);
    // struct Unit *unit_ontile = IES_GET_COMPONENT(sota->ecs.world, unit_entity_ontile, Unit);
    // SDL_assert(unit_ontile != NULL);
    // SDL_assert(unit_ontile->gl_weapons_dtab != NULL);
    // struct Menu *mc;
    // mc = IES_GET_COMPONENT(sota->ecs.world, sota->menus.staff_select, Menu);

    // SDL_assert(mc->n9patch.patch_pixels.x > 0);
    // SDL_assert(mc->n9patch.patch_pixels.y > 0);
    // mc->visible = true;
    // struct LoadoutSelectMenu *ssm = mc->data;
    // // ssm->update = true;
    // SDL_assert(mc->elem_pos == ssm_elem_pos);

    // LoadoutSelectMenu_Load(ssm, unit_ontile, sota->render.er);
    // /* scaling elem_pos: put it last cause dependences */

}

void Game_StaffSelectMenu_Enable(struct Game *sota, tnecs_entity unit_entity_ontile) {
    if (sota->menus.staff_select == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.staff_select);
    SDL_assert(sota->menus.staff_select > 0);
    Game_StaffSelectMenu_Update(sota, unit_entity_ontile);
    strncpy(sota->debug.reason, "STAFF_SELECT_MENU was created", sizeof(sota->debug.reason));
    Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.staff_select, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- Menu location --- */
void Game_Menu_LocationfromUnit(struct Game *sota, tnecs_entity in_menu_entity,
                                tnecs_entity in_unit_entity) {

    struct Menu *mc = NULL;
    // struct Position *unit_pos = NULL;
    SDL_assert(in_unit_entity > 0);
    SDL_assert(in_menu_entity > 0);
    // unit_pos = IES_GET_COMPONENT(sota->ecs.world, in_unit_entity, Position);
    mc = IES_GET_COMPONENT(sota->ecs.world, in_menu_entity, Menu);
    SDL_assert(mc != NULL);
    // if (unit_pos != NULL) {
    //     if (unit_pos->onTilemap) {
    //         menu->n9patch.pos.x = unit_pos->tilemap_pos.x * map->tilesize[0];
    //         menu->n9patch.pos.y = unit_pos->tilemap_pos.y * map->tilesize[0];
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
}

void Game_Menu_LocationfromCursor(struct Game *sota, tnecs_entity in_menu_entity) {
    SDL_assert(sota->cursor.entity);
    SDL_assert(in_menu_entity > 0);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, in_menu_entity,
                                        Menu);
    SDL_assert(mc != NULL);
    // if (cursor_pos != NULL) {
    //     if (cursor_pos->onTilemap) {
    //         menu->n9patch.pos.x = cursor_pos->tilemap_pos.x * map->tilesize[0];
    //         menu->n9patch.pos.y = cursor_pos->tilemap_pos.y * map->tilesize[0];
    //     } else {
    //         menu->n9patch.pos.x = cursor_pos->pixel_pos.x;
    //         menu->n9patch.pos.y = cursor_pos->pixel_pos.y;
    //     }
    // } else {
    mc->n9patch.pos.x = sota->settings.res.x / 2;
    mc->n9patch.pos.y = sota->settings.res.y / 2;
    // }
}

/* --- Title Screen --- */
void Game_FirstMenu_Update(struct Game *sota) {
    if (sota->title_screen.menu == 0) {
        SDL_Log("First menu is not loaded");
        exit(ERROR_Generic);
    }
    SDL_assert(sota->title_screen.menu > TNECS_NULL);
    struct Menu *mc = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.menu, Menu);
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
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
}

void Game_FirstMenu_Destroy(struct Game *sota) {
    if (sota->title_screen.menu != TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.menu, Menu);
        SDL_DestroyTexture(mc->n9patch.texture);
        if (mc->data != NULL) {
            PlayerSelectMenu_Free(mc->data, mc);
            mc->data = NULL;
            tnecs_entity_destroy(sota->ecs.world, sota->title_screen.menu);
        }
    }
    /* only first_menu should be on the stack */
    SDL_assert(DARR_NUM(sota->menus.stack) == 1);
    tnecs_entity first_menu = DARR_POP(sota->menus.stack);
    SDL_assert(first_menu == sota->title_screen.menu);
    SDL_assert(DARR_NUM(sota->menus.stack) == 0);

    sota->title_screen.menu = TNECS_NULL;
    Game_Title_Destroy(sota);
}

void Game_Title_Create(struct Game *sota) {
    SDL_SetRenderDrawColor(sota->render.er, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    if (sota->title_screen.title != TNECS_NULL) {
        SDL_Log("Title is already loaded");
        return;
    }

    sota->title_screen.title = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Text_ID, Position_ID);

    /* -- Get position -- */
    struct Position *position = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.title, Position);
    position->pixel_pos.x = sota->settings.res.x / 20;
    position->pixel_pos.y = sota->settings.res.y / 10;
    position->scale[0] = 10;
    position->scale[1] = 10;

    /* -- Get text -- */
    struct Text *text = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.title, Text);
    *text = Text_default;

    /* - Load pixelfont - */
    text->pixelfont = PixelFont_Alloc();
    text->pixelfont->glyph_width  = 16;
    text->pixelfont->glyph_height = 16;
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_gothic.png");
    PixelFont_Load(text->pixelfont, sota->render.er, path);

    /* - Set title - */
    Text_Set(text, GAME_TITLE, PIXELNOURS_GOTHIC_Y_OFFSET);
    SDL_assert((text->rect.w > 0) && (text->rect.h > 0));
    SDL_assert(sota->fonts.pixelnours_big != NULL);
}

void Game_Title_Destroy(struct Game *sota) {
    if (sota->title_screen.title == TNECS_NULL) {
        return;
    }

    struct Text *text = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.title, Text);
    SDL_assert(text != NULL);

    if ((text != NULL) && (text->pixelfont != NULL)) {
        PixelFont_Free(text->pixelfont, true);
    }

    tnecs_entity_destroy(sota->ecs.world, sota->title_screen.title);
    sota->title_screen.title = TNECS_NULL;
}

void Game_FirstMenu_Create(struct Game *sota) {
    if (sota->title_screen.menu != TNECS_NULL) {
        SDL_Log("FirstMenu is already loaded");
        return;
    }
    sota->title_screen.menu = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Menu_ID);
    struct Menu *mc;
    struct PlayerSelectMenu *psm = PlayerSelectMenu_Alloc();
    mc = IES_GET_COMPONENT(sota->ecs.world, sota->title_screen.menu, Menu);
    mc->data        = psm;
    mc->type        = MENU_TYPE_PLAYER_SELECT;
    mc->draw        = &PlayerSelectMenu_Draw;
    mc->visible = true;

    FirstMenu_Load(psm, sota->render.er, &mc->n9patch);
    SDL_assert(mc->n9patch.patch_pixels.x > 0);
    SDL_assert(mc->n9patch.patch_pixels.y > 0);

    psm->row_height = sota->fonts.pixelnours->glyph_height + 2; /* pixel fonts have python8 pixels*/
    psm->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours != NULL);
    psm->id = sota->title_screen.menu;
    psm->pos.x = sota->settings.res.x / 3;
    psm->pos.y = sota->settings.res.y / 3;
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);

}

void Game_titleScreen_Load(struct Game *sota, struct Input_Arguments in_args) {
    SDL_SetRenderDrawColor(sota->render.er, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    Game_FirstMenu_Create(sota);
    Game_FirstMenu_Update(sota);
    Game_menuStack_Push(sota, sota->title_screen.menu);
    Game_Title_Create(sota);
}
