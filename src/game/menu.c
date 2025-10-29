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
** game/menu: Top level menu management
**
*/

#include "item.h"
#include "text.h"
#include "nmath.h"
#include "graph.h"
#include "events.h"
#include "macros.h"
#include "structs.h"
#include "globals.h"
#include "position.h"
#include "platform.h"
#include "filesystem.h"
#include "pixelfonts.h"

#include "game/map.h"
#include "game/menu.h"
#include "game/game.h"
#include "game/cursor.h"

#include "map/map.h"
#include "map/path.h"
#include "map/find.h"

#include "menu/first.h"
#include "menu/stats.h"
#include "menu/trade.h"
#include "menu/growths.h"
#include "menu/item_drop.h"
#include "menu/deployment.h"
#include "menu/map_action.h"
#include "menu/which_hand.h"
#include "menu/item_action.h"
#include "menu/unit_action.h"
#include "menu/item_select.h"
#include "menu/staff_select.h"

#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

/* --- All Menus --- */
void Game_Menus_Init(struct Game *sota) {
    if (sota->menus.stack != NULL)
        DARR_FREE(sota->menus.stack);
    sota->menus.stack = DARR_INIT(sota->menus.stack, tnecs_E, 4);
    memset(sota->menus.stack, 0, 4 * sizeof(*sota->menus.stack));
}

void Game_Switch_toCandidates(  Game        *sota,
                                tnecs_E     *candidates,
                                const char  *reason) {
    sota->targets.candidates = candidates;
    Game_subState_Set(  sota,
                        GAME_SUBSTATE_MAP_CANDIDATES,
                        reason);

    /* - set cursor position on tilemap - */
    Game_cursorFocus_onMap(sota);

    /* - moving cursor moves to next entity-> go to its position on tilemap - */
    struct Position *candidate_pos, *cursor_pos;
    int index = (sota->targets.previous_order > -1) ? sota->targets.previous_order : 0;
    tnecs_E candidate = sota->targets.candidates[index];

    candidate_pos = IES_GET_C(gl_world, candidate, Position);
    SDL_assert(candidate_pos != NULL);
    cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    SDL_assert(cursor_pos != NULL);
    cursor_pos->tilemap_pos.x = candidate_pos->tilemap_pos.x;
    cursor_pos->tilemap_pos.y = candidate_pos->tilemap_pos.y;

    /* - Hover on first candidate - */
    tnecs_E *data2 = IES_calloc(1, sizeof(*data2));
    *data2 = sota->targets.candidates[index];

    Event_Emit( __func__, SDL_USEREVENT,
                event_Cursor_Hovers_Unit,
                NULL, data2);
}

/* --- Menu stack --- */
void Game_DeploymentMenu_Free(struct Game *sota) {
    if (sota->menus.deployment == TNECS_NULL)
        return;

    struct Menu *mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    SDL_assert(mc);

    struct DeploymentMenu *dm = mc->data;
    DeploymentMenu_Free(dm);
    mc->data = NULL;
    sota->menus.deployment = TNECS_NULL;
}

void Game_menuStack_Free(struct Game *sota) {
    struct StatsMenu        *stats_menu;
    SDL_assert(sota->menus.stack != NULL);
    while (DARR_NUM(sota->menus.stack) > 0) {
        tnecs_E entity = sota->menus.stack[DARR_NUM(sota->menus.stack) - 1];
        SDL_assert(TNECS_E_EXISTS(gl_world, entity));
        struct Menu *mc = IES_GET_C(gl_world, entity, Menu);

        if (mc->data == NULL) {
            DARR_POP(sota->menus.stack);
            continue;
        }

        /* TODO: free func in Menu  */
        switch (mc->type) {
            case MENU_TYPE_STATS:
                stats_menu = mc->data;
                StatsMenu_Free(stats_menu);
                mc->data = NULL;
                break;
        }
        mc->data = NULL;
        DARR_POP(sota->menus.stack);
    }

    DARR_FREE(sota->menus.stack);
    sota->menus.stack = NULL;
}

void Game_menuStack_Push(struct Game *sota, tnecs_E in_menu_entity) {
    SDL_assert(in_menu_entity > 0);
    DARR_PUT(sota->menus.stack, in_menu_entity);
}

tnecs_E Game_menuStack_Pop(Game *IES, b32 destroy) {
    tnecs_E top_E = DARR_POP(IES->menus.stack);
    SDL_assert(top_E > 0);
    struct Menu *mc = IES_GET_C(gl_world, top_E, Menu);
    mc->visible = false;

    if (destroy) {
        // TODO
    }

    /* -- Sending event after all menus popped -- */
    if ((DARR_NUM(IES->menus.stack) == 0) &&
        (IES->menus.allpopped_event != 0)) {
        Event_Emit( __func__, SDL_USEREVENT,
                    IES->menus.allpopped_event, NULL, NULL);
        IES->menus.allpopped_event = 0;
    }

    return (top_E);
}

/* --- Deployment --- */
void Game_DeploymentMenu_Create(struct Game *sota) {
    if (sota->menus.deployment == TNECS_NULL)
        sota->menus.deployment = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
    SDL_assert(mc != NULL);
    mc->type        = MENU_TYPE_DEPLOYMENT;
    mc->draw        = &DeploymentMenu_Draw;

    struct DeploymentMenu *dm = DeploymentMenu_Alloc();
    SDL_assert(dm != NULL);
    mc->data = dm;

    dm->pos.x       = sota->settings.res.x / 10;
    dm->pos.y       = sota->settings.res.y / 4;
    dm->world       = gl_world;
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
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.deployment, Menu);
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

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.stats;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- GrowthsMenu --- */
void Game_GrowthsMenu_Enable(struct Game *sota, tnecs_E ent_ontile) {
    if (sota->menus.growths == 0)
        Game_GrowthsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.growths);
    SDL_assert(sota->menus.growths > 0);
    Game_GrowthsMenu_Update(sota, ent_ontile);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.stats;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

void Game_GrowthsMenu_Create(struct Game *sota) {
    if (sota->menus.growths == TNECS_NULL)
        sota->menus.growths = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.growths, Menu);
    SDL_assert(mc != NULL);
    mc->type        = MENU_TYPE_GROWTHS;
    mc->draw        = &GrowthsMenu_Draw;

    struct GrowthsMenu *GM = GrowthsMenu_Alloc();
    SDL_assert(GM != NULL);
    mc->data = GM;

    // FOR DEBUG
    // TODO: GET STAT FROM USER
    i16 stat_toplot = 1;

    _Graph_Stat_Add(&GM->graph, &test_base_stats,
                    test_grown_stats, 40, 31, stat_toplot);
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

void Game_GrowthsMenu_Update(struct Game *sota, tnecs_E ent_ontile) {
    SDL_assert(ent_ontile > TNECS_NULL);
    struct Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.growths, Menu);
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
        sota->menus.stats = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.stats, Menu);
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

void Game_StatsMenu_Update(struct Game *sota, tnecs_E ent_ontile) {
    SDL_assert(ent_ontile > TNECS_NULL);
    /* - Get unit on tile - */
    struct Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    SDL_assert(unit_ontile != NULL);
    SDL_assert(gl_weapons_dtab != NULL);

    /* - Update stats menu - */
    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.stats, Menu);
    mc->visible = true;
    SDL_assert(gl_weapons_dtab != NULL);
    struct StatsMenu *stats_menu = (struct StatsMenu *)mc->data;
    SDL_assert(gl_weapons_dtab != NULL);
    StatsMenu_Load(stats_menu, unit_ontile, sota->render.er, &mc->n9patch);
    // Scaling elem_pos: put it last cause dependencies
    StatsMenu_Elem_Pos(stats_menu, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_StatsMenu_Enable(struct Game *sota, tnecs_E ent_ontile) {
    if (sota->menus.stats == 0)
        Game_StatsMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.stats);
    SDL_assert(sota->menus.stats > 0);
    Game_StatsMenu_Update(sota, ent_ontile);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.stats;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

void Game_postLoadout_Defendants(struct Game *sota, tnecs_E actor) {
    /* -- Finding possible defendants with equipped weapons -- */
    DARR_NUM(sota->targets.defendants) = 0;

    /* - Compute attacktolist - */
    SDL_assert(sota->menus.weapon_select > TNECS_NULL);
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.weapon_select, Menu);
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

void Game_postLoadout_Patients(struct Game *sota, tnecs_E actor) {
    SDL_assert(sota->targets.patients != sota->targets.defendants);

    struct Unit *unit = IES_GET_C(gl_world, actor, Unit);
    SDL_assert(Unit_canStaff(unit));
    DARR_NUM(sota->targets.patients) = 0;

    SDL_assert(sota->menus.staff_select > TNECS_NULL);
    struct Menu *mc = IES_GET_C(gl_world, sota->menus.staff_select, Menu);
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
void Game_preLoadout_Patients(struct Game *sota, tnecs_E actor) {
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
void Game_preLoadout_Defendants(struct Game *sota, tnecs_E actor) {
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
void Game_preUnitAction_Targets(struct Game *sota, tnecs_E actor) {
    /* -- Resetting lists -- */
    DARR_NUM(sota->targets.victims)     = 0;
    DARR_NUM(sota->targets.passives)    = 0;
    DARR_NUM(sota->targets.auditors)    = 0;
    DARR_NUM(sota->targets.openables)   = 0;
    DARR_NUM(sota->targets.spectators)  = 0;

    /* -- Getters -- */
    struct Unit     *unit = IES_GET_C(gl_world, actor, Unit);
    struct Position *pos  = IES_GET_C(gl_world, actor, Position);

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

/* --- WeaponSelectMenu --- */
void Game_WeaponSelectMenu_Create(struct Game *sota) {

    if (sota->menus.weapon_select == TNECS_NULL)
        sota->menus.weapon_select =  IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    Menu *mc    = IES_GET_C(gl_world, sota->menus.weapon_select, Menu);
    mc->type    = MENU_TYPE_WEAPON_SELECT;
    mc->draw    = &LoadoutSelectMenu_Draw;

    /* stats_menu struct init */
    struct LoadoutSelectMenu *wsm   = LoadoutSelectMenu_Alloc();
    wsm->pos.x                      = sota->settings.res.x / 2;
    wsm->pos.y                      = sota->settings.res.y / 2;
    wsm->archetype_stronghand       = ITEM_ARCHETYPE_WEAPON;
    mc->data                        = wsm;
    mc->visible                     = true;
    // TODO: Menu frees, elem_pos, elem_box so make
    // LoadoutSelectMenu functions to alloc
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

void Game_WeaponSelectMenu_Update(struct Game *sota,
                                  tnecs_E ent_ontile) {
    SDL_assert(ent_ontile > TNECS_NULL);
    Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    SDL_assert(unit_ontile      != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    /* Find new canEquip */
    canEquip can_equip          = canEquip_default;
    can_equip.archetype         = ITEM_ARCHETYPE_WEAPON;
    can_equip.two_hands_mode    = TWO_HAND_EQ_MODE_LOOSE;

    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.weapon_select, Menu);
    mc->visible = true;
    struct LoadoutSelectMenu *wsm = mc->data;

    Map *map = Game_Map(sota);
    wsm->equippable = Map_canEquip(map, ent_ontile, can_equip);
    SDL_assert(wsm->equippable.num > 0);

    SDL_assert(mc->elem_pos == wsm_elem_pos);
    // LoadoutSelectMenu_Load(wsm, unit_ontile, sota->render.er);
    LoadoutSelectMenu_Select_Reset(wsm);
    WeaponSelectMenu_Load(wsm, map, sota->render.er, &mc->n9patch);
    LoadoutSelectMenu_Unit(wsm, ent_ontile);
    SDL_assert(mc->n9patch.scale.x          > 0);
    SDL_assert(mc->n9patch.scale.y          > 0);
    SDL_assert(mc->n9patch.px.x   > 0);
    SDL_assert(mc->n9patch.px.y   > 0);

    LoadoutSelectMenu_Elem_Pos_Revert(wsm, mc);
    LoadoutSelectMenu_Elem_Reset(wsm, mc);
    LoadoutSelectMenu_Elem_Pos(  wsm, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_WeaponSelectMenu_Enable(struct Game *sota, tnecs_E uent_ontile) {
    if (sota->menus.weapon_select == TNECS_NULL)
        Game_WeaponSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.weapon_select);
    SDL_assert(sota->menus.weapon_select > 0);
    Game_WeaponSelectMenu_Update(sota, uent_ontile);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.weapon_select;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- TradeMenu --- */
void Game_TradeMenu_Create(struct Game *sota) {
    if (sota->menus.trade == TNECS_NULL)
        sota->menus.trade = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    Menu *mc    = IES_GET_C(gl_world, sota->menus.trade, Menu);
    mc->type    = MENU_TYPE_TRADE;
    // mc->draw         = &TradeMenu_Draw;

    /* n9patch init */
    mc->n9patch.px.x        = MENU_PATCH_PIXELS;
    mc->n9patch.px.y        = MENU_PATCH_PIXELS;
    mc->n9patch.num.x       = LSM_PATCH_X_SIZE * 2;
    mc->n9patch.num.y       = LSM_PATCH_Y_SIZE;
    mc->n9patch.scale.x     = LSM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y     = LSM_N9PATCH_SCALE_Y;

    Point size = {
        .x  = (MENU_PATCH_PIXELS * mc->n9patch.num.x),
        .y  = (MENU_PATCH_PIXELS * mc->n9patch.num.y),
    };
    n9Patch_Pixels_Total_Set(&mc->n9patch, size);

    mc->n9patch.texture =   Filesystem_Texture_Load(sota->render.er,
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

void Game_TradeMenu_Update(struct Game *sota, tnecs_E selected, tnecs_E candidate) {
    SDL_assert(selected     > TNECS_NULL);
    SDL_assert(candidate    > TNECS_NULL);
    Unit *active    = IES_GET_C(gl_world, selected, Unit);
    Unit *passive   = IES_GET_C(gl_world, candidate, Unit);
    SDL_assert(active   != NULL);
    SDL_assert(passive  != NULL);
    Menu *mc = IES_GET_C(gl_world, sota->menus.trade, Menu);

    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    mc->visible = true;
    struct TradeMenu *tm = (struct TradeMenu *)mc->data;
    // ism->update = true;
    // LoadoutSelectMenu_Load(tm->active,  active,  sota->render.er);
    // LoadoutSelectMenu_Load(tm->passive, passive, sota->render.er);
    SDL_assert(tm->active->_unit    > TNECS_NULL);
    SDL_assert(tm->passive->_unit   > TNECS_NULL);

    tm->active->pos.x   = sota->settings.res.x / 4;
    tm->active->pos.y   = sota->settings.res.y / 3;
    tm->passive->pos.y  = tm->active->pos.y;

    // TradeMenu_Elem_Pos(tm, mc);

    /* scaling elem_pos: put it last cause dependences */
}

void Game_TradeMenu_Enable(struct Game *sota, tnecs_E selected, tnecs_E candidate) {
    // if (sota->menus.trade == TNECS_NULL)
    //     Game_TradeMenu_Create(sota);
    // SDL_assert(sota->menus.trade > TNECS_NULL);

    // Game_menuStack_Push(sota,   sota->menus.trade);
    // Game_TradeMenu_Update(sota, selected,   candidate);
    // strncpy(sota->debug.reason, "TradeMenu was created", sizeof(sota->debug.reason));
    // Event_Emit(__func__, SDL_USEREVENT, event_Menu_Created, &sota->menus.trade, NULL);
    // Game_cursorFocus_onMenu(sota);
}

/* --- MapActionMenu --- */
void Game_MapActionMenu_Create(Game *sota) {
    if (sota->menus.map_action != TNECS_NULL) {
        SDL_Log("MapActionMenu is already loaded");
        return;
    }
    sota->menus.map_action = IES_E_CREATE_wC(gl_world, Menu_ID);

    MapActionMenu *mam = UnitActionMenu_Alloc();
    Menu *mc = IES_GET_C(gl_world, sota->menus.map_action, Menu);
    mc->data        = mam;
    mc->type        = MENU_TYPE_MAP_ACTION;
    mc->draw        = &MapActionMenu_Draw;
    mc->visible     = true;

    pActionMenu_Set(mam->platform, NULL, sota->render.er);
    MapActionMenu_Load(mam, &mc->n9patch);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    mam->row_height = sota->fonts.pixelnours->glyph_height + 2; /* pixel fonts have python8 pixels*/
    mam->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours != NULL);
    mam->id = sota->title_screen.menu;
    mam->pos.x = sota->settings.res.x / 3;
    mam->pos.y = sota->settings.res.y / 3;
}

void Game_MapActionMenu_Update(Game *sota) {
    if (sota->menus.map_action == TNECS_NULL) {
        SDL_Log("MapActionMenu is not loaded");
        SDL_assert(false);
        exit(ERROR_Generic);
    }
    SDL_assert(sota->menus.map_action > TNECS_NULL);
    Menu *mc = IES_GET_C(   gl_world, sota->menus.map_action,
                            Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    MapActionMenu *mam = mc->data;
    SDL_assert(mam != NULL);
    MapActionMenu_Dynamic(mam, &mc->n9patch);

    mc->elem_num = MapActionMenu_Options_Num(mam);
    MapActionMenu_Elem_Links(mam, mc);
    MapActionMenu_Elem_Boxes(mam, mc);
    MapActionMenu_Elem_Pos(mam, mc);
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    mc->visible = true;
}

void Game_MapActionMenu_Destroy(Game *sota) {
    if (sota->menus.map_action != TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_C(gl_world, sota->menus.map_action, Menu);
        SDL_DestroyTexture(mc->n9patch.texture);
        if (mc->data != NULL) {
            MapActionMenu_Free(mc->data, mc);
            mc->data = NULL;
            tnecs_E_destroy(gl_world, sota->menus.map_action);
        }
    }
    tnecs_E MAM = DARR_POP(sota->menus.stack);
    SDL_assert(MAM == sota->menus.map_action);

    sota->menus.map_action = TNECS_NULL;
}

/* --- UnitActionMenu --- */
void Game_UnitActionMenu_Create(Game *sota) {
    if (sota->menus.unit_action != TNECS_NULL) {
        SDL_Log("UnitActionMenu is already loaded");
        return;
    }
    sota->menus.unit_action = IES_E_CREATE_wC(gl_world, Menu_ID);

    UnitActionMenu *uam = UnitActionMenu_Alloc();
    Menu *mc = IES_GET_C(gl_world, sota->menus.unit_action, Menu);
    mc->data        = uam;
    mc->type        = MENU_TYPE_UNIT_ACTION;
    mc->draw        = &UnitActionMenu_Draw;
    mc->visible     = true;

    pActionMenu_Set(uam->platform, NULL, sota->render.er);
    UnitActionMenu_Load(uam, &mc->n9patch);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    uam->row_height = sota->fonts.pixelnours->glyph_height + 2; /* pixel fonts have python8 pixels*/
    uam->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours != NULL);
    uam->id = sota->title_screen.menu;
    uam->pos.x = sota->settings.res.x / 3;
    uam->pos.y = sota->settings.res.y / 3;
}

void Game_UnitActionMenu_Update(Game *sota, tnecs_E ent) {
    if (sota->menus.unit_action == TNECS_NULL) {
        SDL_Log("UnitActionMenu is not loaded");
        SDL_assert(false);
        exit(ERROR_Generic);
    }
    SDL_assert(sota->menus.unit_action > TNECS_NULL);
    Menu *mc = IES_GET_C(   gl_world, sota->menus.unit_action,
                            Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    UnitActionMenu *uam = mc->data;
    SDL_assert(uam != NULL);
    UnitActionMenu_Dynamic(uam, &mc->n9patch, ent, sota);

    mc->elem_num = UnitActionMenu_Options_Num(uam);
    UnitActionMenu_Elem_Links(uam, mc);
    UnitActionMenu_Elem_Boxes(uam, mc);
    UnitActionMenu_Elem_Pos(uam, mc);
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    mc->visible = true;
}

void Game_UnitActionMenu_Destroy(Game *sota) {
    if (sota->menus.unit_action != TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_C(gl_world, sota->menus.unit_action, Menu);
        SDL_DestroyTexture(mc->n9patch.texture);
        if (mc->data != NULL) {
            UnitActionMenu_Free(mc->data, mc);
            mc->data = NULL;
            tnecs_E_destroy(gl_world, sota->menus.unit_action);
        }
    }
    tnecs_E unit_action_menu = DARR_POP(sota->menus.stack);
    SDL_assert(unit_action_menu == sota->menus.unit_action);

    sota->menus.unit_action = TNECS_NULL;
}

/* --- ItemActionMenu --- */
void Game_ItemActionMenu_Create(Game *sota) {
    if (sota->menus.item_action != TNECS_NULL) {
        SDL_Log("UnitActionMenu is already loaded");
        return;
    }
    sota->menus.item_action = IES_E_CREATE_wC(gl_world, Menu_ID);

    ItemActionMenu *iam = ItemActionMenu_Alloc();
    Menu *mc = IES_GET_C(gl_world, sota->menus.item_action, Menu);
    mc->data        = iam;
    mc->type        = MENU_TYPE_ITEM_ACTION;
    mc->draw        = &ItemActionMenu_Draw;
    mc->visible     = true;

    pActionMenu_Set(iam->am->platform, NULL, sota->render.er);
    UnitActionMenu_Load(iam->am, &mc->n9patch);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    iam->am->row_height = sota->fonts.pixelnours->glyph_height + 2; /* pixel fonts have python8 pixels*/
    iam->am->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours != NULL);
    iam->am->id = sota->title_screen.menu;
    iam->am->pos.x = sota->settings.res.x / 3;
    iam->am->pos.y = sota->settings.res.y / 3;

    mc->n9patch.scale.x = ISM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y = ISM_N9PATCH_SCALE_Y;
    iam->am->pos.y += 200;
}

void Game_ItemActionMenu_Update(Game *sota, tnecs_E unit_E) {
    if (sota->menus.item_action == TNECS_NULL) {
        SDL_Log("ItemActionMenu is not loaded");
        SDL_assert(false);
        exit(ERROR_Generic);
    }
    SDL_assert(sota->menus.item_action > TNECS_NULL);
    Menu *mc = IES_GET_C(   gl_world,
                            sota->menus.item_action,
                            Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    ItemActionMenu *iam = mc->data;
    SDL_assert(iam != NULL);
    ItemActionMenu_Dynamic( iam, &mc->n9patch,
                            unit_E, sota);

    mc->elem_num = ItemActionMenu_Options_Num(iam);
    ItemActionMenu_Elem_Links(iam, mc);
    ItemActionMenu_Elem_Boxes(iam, mc);
    ItemActionMenu_Elem_Pos(iam, mc);
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
    mc->visible = true;

}

void Game_ItemActionMenu_Enable(Game *sota, tnecs_E unit_E) {
    if (sota->menus.item_action == TNECS_NULL)
        Game_ItemActionMenu_Create(sota);

    SDL_assert(sota->menus.item_action != TNECS_NULL);
    Game_menuStack_Push(sota, sota->menus.item_action);
    SDL_assert(sota->menus.item_action > TNECS_NULL);

    Game_ItemActionMenu_Update(sota, unit_E);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.item_action;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);

    Game_cursorFocus_onMenu(sota);
}

/* --- ItemSelectMenu --- */
void Game_ItemSelectMenu_Create(struct Game *sota) {
    if (sota->menus.item_select == TNECS_NULL)
        sota->menus.item_select = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    Menu *mc = IES_GET_C(gl_world, sota->menus.item_select, Menu);
    mc->type = MENU_TYPE_ITEM_SELECT;
    mc->draw = &ItemSelectMenu_Draw;

    /* n9patch init */
    mc->n9patch.px.x    = MENU_PATCH_PIXELS;
    mc->n9patch.px.y    = MENU_PATCH_PIXELS;
    mc->n9patch.num.x   = ISM_PATCH_X_SIZE;
    mc->n9patch.num.y   = ISM_PATCH_Y_SIZE;
    mc->n9patch.scale.x = ISM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y = ISM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(&mc->n9patch, size);

    mc->n9patch.texture = Filesystem_Texture_Load(
                                  sota->render.er,
                                  sota->menus.filename.data,
                                  SDL_PIXELFORMAT_INDEX8
                          );

    /* stats_menu struct init */
    ItemSelectMenu *ism = ItemSelectMenu_Alloc();
    ism->pos.x          = sota->settings.res.x / 2;
    ism->pos.y          = sota->settings.res.y / 2;

    SDL_assert(sota->fonts.pixelnours     != NULL);
    SDL_assert(sota->fonts.pixelnours_big != NULL);
    ism->pixelnours     = sota->fonts.pixelnours;
    ism->pixelnours_big = sota->fonts.pixelnours_big;

    mc->data            = ism;
    mc->visible         = true;
    mc->elem_links      = wsm_links;
    mc->elem_pos        = wsm_elem_pos;
    mc->elem_box        = wsm_elem_box;
    mc->elem_num        = LSM_ELEMS_NUM;
}

void Game_ItemSelectMenu_Update(Game    *sota,
                                tnecs_E  ent_ontile) {
    SDL_assert(ent_ontile > TNECS_NULL);
    Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    SDL_assert(unit_ontile      != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.item_select, Menu);
    SDL_assert(mc != NULL);
    mc->visible = true;
    ItemSelectMenu *ism = mc->data;

    SDL_assert(mc->elem_pos == wsm_elem_pos);

    ItemSelectMenu_Unit(mc, ent_ontile);
    ItemSelectMenu_Size(ism, &mc->n9patch);

    ItemSelectMenu_Elem_Pos(ism, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_ItemSelectMenu_Enable(struct Game *sota, tnecs_E uent_ontile) {
    if (sota->menus.item_select == TNECS_NULL)
        Game_ItemSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.item_select);
    SDL_assert(sota->menus.item_select > 0);
    Game_ItemSelectMenu_Update(sota, uent_ontile);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.item_select;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- WhichHandMenu --- */
void Game_WHM_Create(Game *sota) {
    if (sota->menus.which_hand == TNECS_NULL)
        sota->menus.which_hand = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    Menu *mc = IES_GET_C(gl_world, sota->menus.which_hand, Menu);
    mc->type = MENU_TYPE_WHICH_HAND;
    mc->draw = &WhichHandMenu_Draw;

    /* stats_menu struct init */
    WhichHandMenu *whm              = WhichHandMenu_Alloc();
    whm->pos.x                      = sota->settings.res.x / 2;
    whm->pos.y                      = sota->settings.res.y / 2;
    mc->data                        = whm;
    mc->visible                     = true;
    mc->elem_links                  = whm_links;
    mc->elem_box                    = whm_elem_box;
    mc->elem_num                    = WHM_ELEM_NUM;

    WhichHandMenu_Load(whm, sota->render.er, &mc->n9patch);
}

void Game_WHM_Update(Game *IES) {
    IES_assert(IES->menus.which_hand > TNECS_NULL);
    Menu *mc    = IES_GET_C(gl_world, IES->menus.which_hand, Menu);
    IES_assert(mc != NULL);

    IES_assert(IES->selected.unit_entity > TNECS_NULL);
    Unit *unit  = IES_GET_C(gl_world, IES->selected.unit_entity, Unit);
    IES_assert(unit != NULL);
    Inventory_item *invitem;
    invitem = IES_GET_C(gl_world, IES->selected.item,
                        Inventory_item);
    Item *item = Item_Get(invitem);
    IES_assert(item != NULL);

    WhichHandMenu_Elements(mc, unit, item);
}

void Game_WHM_Enable(Game *IES) {
    if (IES->menus.which_hand == TNECS_NULL)
        Game_WHM_Create(IES);
    Game_menuStack_Push(IES, IES->menus.which_hand);
    SDL_assert(IES->menus.which_hand > 0);
    Game_WHM_Update(IES);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = IES->menus.which_hand;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(IES);
}

/* --- StaffSelectMenu --- */
void Game_StaffSelectMenu_Create(struct Game *sota) {
    if (sota->menus.staff_select == TNECS_NULL)
        sota->menus.staff_select = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    struct Menu *mc;
    mc          = IES_GET_C(gl_world, sota->menus.staff_select, Menu);
    mc->type    = MENU_TYPE_STAFF_SELECT;
    mc->draw    = &LoadoutSelectMenu_Draw;

    /* n9patch init */
    mc->n9patch.px.x        = MENU_PATCH_PIXELS;
    mc->n9patch.px.y        = MENU_PATCH_PIXELS;
    mc->n9patch.num.x       = ISM_PATCH_X_SIZE;
    mc->n9patch.num.y       = ISM_PATCH_Y_SIZE;
    mc->n9patch.scale.x     = ISM_N9PATCH_SCALE_X;
    mc->n9patch.scale.y     = ISM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * ISM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * ISM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(&mc->n9patch, size);


    mc->n9patch.texture = Filesystem_Texture_Load(
                                  sota->render.er,
                                  sota->menus.filename.data,
                                  SDL_PIXELFORMAT_INDEX8
                          );

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
    LoadoutSelectMenu_Elem_Pos(ssm, mc);
    Menu_Elem_Boxes_Check(mc);
}

void Game_StaffSelectMenu_Update(struct Game *sota,
                                 tnecs_E ent_ontile) {
    SDL_assert(ent_ontile > TNECS_NULL);
    Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    SDL_assert(unit_ontile      != NULL);
    SDL_assert(gl_weapons_dtab  != NULL);

    struct Menu *mc;
    mc = IES_GET_C(gl_world, sota->menus.staff_select, Menu);
    mc->visible = true;

    struct LoadoutSelectMenu *ssm = mc->data;
    Map *map = Game_Map(sota);
    SDL_assert(mc->elem_pos == ssm_elem_pos);
    StaffSelectMenu_Load(ssm, map, sota->render.er, &mc->n9patch);
    LoadoutSelectMenu_Unit(ssm, ent_ontile);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    mc->elem_num = ssm->equippable.num;
    for (int i = mc->elem_num - 1; i < SOTA_EQUIPMENT_SIZE; i++) {
        mc->elem_links[i].top    = LSM_ELEM_NULL;
        mc->elem_links[i].bottom = LSM_ELEM_NULL;
    }

    Menu_Elem_Boxes_Check(mc);

    // SDL_assert(ent_ontile > TNECS_NULL);
    // struct Unit *unit_ontile = IES_GET_C(gl_world, ent_ontile, Unit);
    // SDL_assert(unit_ontile != NULL);
    // SDL_assert(unit_ontile->gl_weapons_dtab != NULL);
    // struct Menu *mc;
    // mc = IES_GET_C(gl_world, sota->menus.staff_select, Menu);

    // SDL_assert(mc->n9patch.px.x > 0);
    // SDL_assert(mc->n9patch.px.y > 0);
    // mc->visible = true;
    // struct LoadoutSelectMenu *ssm = mc->data;
    // // ssm->update = true;
    // SDL_assert(mc->elem_pos == ssm_elem_pos);

    // LoadoutSelectMenu_Load(ssm, unit_ontile, sota->render.er);
    // /* scaling elem_pos: put it last cause dependences */

}

void Game_StaffSelectMenu_Enable(Game *sota, tnecs_E ent_ontile) {
    if (sota->menus.staff_select == TNECS_NULL)
        Game_StaffSelectMenu_Create(sota);
    Game_menuStack_Push(sota, sota->menus.staff_select);
    SDL_assert(sota->menus.staff_select > 0);
    Game_StaffSelectMenu_Update(sota, ent_ontile);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = sota->menus.staff_select;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(sota);
}

/* --- Menu location --- */
void Game_Menu_LocationfromUnit(struct Game *sota, tnecs_E in_menu_entity,
                                tnecs_E in_unit_entity) {

    struct Menu *mc = NULL;
    // struct Position *unit_pos = NULL;
    SDL_assert(in_unit_entity > 0);
    SDL_assert(in_menu_entity > 0);
    // unit_pos = IES_GET_C(gl_world, in_unit_entity, Position);
    mc = IES_GET_C(gl_world, in_menu_entity, Menu);
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
    // SDL_assert(mc->n9patch.px.x > 0);
    // SDL_assert(mc->n9patch.px.y > 0);
}

void Game_Menu_LocationfromCursor(struct Game *sota, tnecs_E in_menu_entity) {
    SDL_assert(sota->cursor.entity);
    SDL_assert(in_menu_entity > 0);
    struct Menu *mc = IES_GET_C(gl_world, in_menu_entity,
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

/* --- ItemDropMenu --- */
void Game_ItemDropMenu_Create(Game *IES) {
    if (IES->menus.item_drop == TNECS_NULL)
        IES->menus.item_drop = IES_E_CREATE_wC(gl_world, Menu_ID);
    else {
        // TODO: destroy menu?
    }
    Menu *mc = IES_GET_C(gl_world, IES->menus.item_drop, Menu);
    mc->type        = MENU_TYPE_ITEM_DROP;
    mc->draw        = &ItemDropMenu_Draw;

    /* stats_menu struct init */
    ItemDropMenu *idm   = ItemDropMenu_Alloc();
    idm->pos.x          = IES->settings.res.x / 2;
    idm->pos.y          = IES->settings.res.y / 2;
    idm->update         = true;
    mc->data            = idm;
    mc->visible         = true;

    ItemDropMenu_Load(idm, IES->render.er, &mc->n9patch);

    // TODO: copy descriptions
    SDL_assert(IES->fonts.pixelnours     != NULL);
    SDL_assert(IES->fonts.pixelnours_big != NULL);
    idm->pixelnours         = IES->fonts.pixelnours;
    idm->pixelnours_big     = IES->fonts.pixelnours_big;
}

void Game_ItemDropMenu_Update(Game *IES) {
    /*  1. unit_E read from Game
    **  2. eq_todrop read from ItemSelectMenu */
    if (IES->menus.item_drop == TNECS_NULL) {
        SDL_Log("ItemDropMenu is not loaded");
        SDL_assert(0);
        return;
    }
    Menu *mc_IDM = IES_GET_C(gl_world, IES->menus.item_drop, Menu);
    SDL_assert(mc_IDM != NULL);
    SDL_assert(mc_IDM->n9patch.px.x > 0);
    SDL_assert(mc_IDM->n9patch.px.y > 0);
    Menu *mc_ISM = IES_GET_C(gl_world, IES->menus.item_select, Menu);
    SDL_assert(mc_ISM != NULL);
    mc_IDM->visible = true;
    ItemDropMenu *idm   = mc_IDM->data;
    SDL_assert(idm != NULL);
    ItemSelectMenu *ism = mc_ISM->data;
    SDL_assert(ism != NULL);

    idm->unit_E     = IES->selected.unit_entity;
    idm->eq_todrop  = ism->selected_eq;

    ItemDropMenu_Elem_Links(idm,    mc_IDM);
    ItemDropMenu_Elem_Boxes(idm,    mc_IDM);
    ItemDropMenu_Elem_Pos(idm,      mc_IDM);
    Menu_Elem_Boxes_Check(mc_IDM);
    SDL_assert(mc_IDM->n9patch.px.x > 0);
    SDL_assert(mc_IDM->n9patch.px.y > 0);
    SDL_assert(mc_IDM->n9patch.pos.x == 0);
    SDL_assert(mc_IDM->n9patch.pos.y == 0);
}

void Game_ItemDropMenu_Enable(Game *IES) {
    if (IES->menus.item_drop == TNECS_NULL) {
        Game_ItemDropMenu_Create(IES);
    }
    Game_menuStack_Push(IES, IES->menus.item_drop);
    SDL_assert(IES->menus.item_drop > TNECS_NULL);
    Game_ItemDropMenu_Update(IES);

    tnecs_E *data1 = IES_calloc(1, sizeof(*data1));
    *data1 = IES->menus.item_drop;

    Event_Emit( __func__, SDL_USEREVENT,
                event_Menu_Created,
                data1, NULL);
    Game_cursorFocus_onMenu(IES);
}

/* --- Title Screen --- */
void Game_FirstMenu_Update(struct Game *sota) {
    if (sota->title_screen.menu == 0) {
        SDL_Log("First menu is not loaded");
        exit(ERROR_Generic);
    }
    SDL_assert(sota->title_screen.menu > TNECS_NULL);
    Menu *mc = IES_GET_C(gl_world, sota->title_screen.menu, Menu);
    SDL_assert(mc != NULL);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    mc->visible = true;
    FirstMenu *fm = mc->data;
    SDL_assert(fm != NULL);
    FirstMenu_Dynamic(fm, &mc->n9patch);

    mc->elem_num = FirstMenu_Options_Num(fm);
    FirstMenu_Elem_Links(fm, mc);
    FirstMenu_Elem_Boxes(fm, mc);
    FirstMenu_Elem_Pos(fm, mc);
    Menu_Elem_Boxes_Check(mc);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
}

void Game_FirstMenu_Create(struct Game *sota) {
    if (sota->title_screen.menu != TNECS_NULL) {
        SDL_Log("FirstMenu is already loaded");
        return;
    }
    sota->title_screen.menu = IES_E_CREATE_wC(gl_world, Menu_ID);
    struct Menu *mc;

    FirstMenu *fm = FirstMenu_Alloc();
    mc = IES_GET_C(gl_world, sota->title_screen.menu, Menu);
    mc->data        = fm;
    mc->type        = MENU_TYPE_FIRST;
    mc->draw        = &FirstMenu_Draw;
    mc->visible     = true;

    pActionMenu_Set(fm->platform, NULL, sota->render.er);
    FirstMenu_Load(fm, &mc->n9patch);
    SDL_assert(mc->n9patch.px.x > 0);
    SDL_assert(mc->n9patch.px.y > 0);

    fm->row_height = sota->fonts.pixelnours->glyph_height + 2; /* pixel fonts have python8 pixels*/
    fm->pixelnours = sota->fonts.pixelnours;
    SDL_assert(sota->fonts.pixelnours != NULL);
    fm->id = sota->title_screen.menu;
    fm->pos.x = sota->settings.res.x / 3;
    fm->pos.y = sota->settings.res.y / 3;
    SDL_assert(mc->n9patch.pos.x == 0);
    SDL_assert(mc->n9patch.pos.y == 0);
}

void Game_FirstMenu_Destroy(struct Game *sota) {
    if (sota->title_screen.menu != TNECS_NULL) {
        struct Menu *mc;
        mc = IES_GET_C(gl_world, sota->title_screen.menu, Menu);
        SDL_DestroyTexture(mc->n9patch.texture);
        if (mc->data != NULL) {
            FirstMenu_Free(mc->data, mc);
            mc->data = NULL;
            tnecs_E_destroy(gl_world, sota->title_screen.menu);
        }
    }
    /* only first_menu should be on the stack */
    SDL_assert(DARR_NUM(sota->menus.stack) == 1);
    tnecs_E first_menu = DARR_POP(sota->menus.stack);
    SDL_assert(first_menu == sota->title_screen.menu);
    SDL_assert(DARR_NUM(sota->menus.stack) == 0);

    sota->title_screen.menu = TNECS_NULL;
    Game_Title_Destroy(sota);
}


void Game_Title_Create(struct Game *sota) {
    SDL_SetRenderDrawColor( sota->render.er,
                            0x00, 0x00, 0x00,
                            SDL_ALPHA_OPAQUE);
    if (sota->title_screen.title != TNECS_NULL) {
        SDL_Log("Title is already loaded");
        return;
    }

    sota->title_screen.title = IES_E_CREATE_wC( gl_world, Text_ID,
                                                Position_ID);

    /* -- Get position -- */
    Position *position      = IES_GET_C(gl_world,
                                        sota->title_screen.title,
                                        Position);
    position->pixel_pos.x   = sota->settings.res.x / 20;
    position->pixel_pos.y   = sota->settings.res.y / 10;
    position->scale[0]      = 10;
    position->scale[1]      = 10;

    /* -- Get text -- */
    struct Text *text = IES_GET_C(  gl_world,
                                    sota->title_screen.title,
                                    Text);
    Text_Init(text);
    P_Text_Init(text, sota->render.er);
    SDL_assert(text->plat != NULL);

    /* - Load pixelfont - */
    text->pixelfont = PixelFont_Alloc();
    text->pixelfont->glyph_width  = 16;
    text->pixelfont->glyph_height = 16;
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_gothic.png");
    PixelFont_Load(text->pixelfont, sota->render.er, path);

    /* - Set title - */
    Text_Set(text, GAME_TITLE, PIXELNOURS_GOTHIC_Y_OFFSET);
    SDL_assert((text->size.x > 0) && (text->size.y > 0));
    SDL_assert(sota->fonts.pixelnours_big != NULL);
}

void Game_Title_Destroy(struct Game *sota) {
    if (sota->title_screen.title == TNECS_NULL) {
        return;
    }

    struct Text *text = IES_GET_C(gl_world, sota->title_screen.title, Text);
    SDL_assert(text != NULL);

    if ((text != NULL) && (text->pixelfont != NULL)) {
        PixelFont_Free(text->pixelfont, true);
    }

    tnecs_E_destroy(gl_world, sota->title_screen.title);
    sota->title_screen.title = TNECS_NULL;
}

void Game_titleScreen_Load(struct Game *sota, struct Input_Arguments in_args) {
    SDL_SetRenderDrawColor(sota->render.er, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    Game_FirstMenu_Create(sota);
    Game_FirstMenu_Update(sota);
    Game_menuStack_Push(sota, sota->title_screen.menu);
    Game_Title_Create(sota);
}
