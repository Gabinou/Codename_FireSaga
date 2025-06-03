
#include "AI.h"
#include "cJSON.h"
#include "names.h"
#include "nmath.h"
#include "pathfinding.h"
#include "map/find.h"
#include "map/map.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/conditions.h"
#include "map/find.h"
#include "game/unit.h"
#include "game/map.h"
#include "game/combat.h"
#include "events.h"
#include "log.h"
#include "position.h"
#include "jsonio.h"
#include "unit/range.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/anim.h"
#include "map/path.h"

const struct Unit_AI Unit_AI_default = {
    .jsonio_header = {
        .json_element   = JSON_AI,
        .json_filename  = {0},
    },
    .priority_master  = AI_PRIORITY_START,
    .priority_slave   = AI_PRIORITY_START,
    .move             = AI_MOVE_START,
};

const struct Game_AI Game_AI_default = {
    .npcs       = NULL,  /* DARR, list of npcs to control */
    .npc_i      = -1,    /* index of latext entity */
    .decided    = false, /* Did AI decide for latest entity*/
    .move_anim  = false, /* Was move animation done for latest entity */
    .act_anim   = false, /* Was act  animation done for latest entity */
    .turn_over  = false, /* Is turn over? */
    .action = {
        .target_move     = {-1, -1},
        .target_action   = {-1, -1},
        .action          = AI_ACTION_START,
    },
};

const struct AI_Action AI_Action_default =  {
    .target_move     = {-1, -1},
    .target_action   = {-1, -1},
    .action          = AI_ACTION_START,
};

/* --- Doer FSM --- */
void _AI_Doer_Wait(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* --- AI Unit Waits --- */
    Game_Unit_Wait(sota, npc_ent);
}

void _AI_Doer_Attack(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* --- AI Unit Attacks the selected defendant --- */
    /* -- Set aggressor for combat -- */
    sota->combat.aggressor = npc_ent;

    /* -- Set defendant for combat -- */
    struct Point pos = action->target_action;
    Map *map = Game_Map(sota);
    tnecs_entity friendly = Map_Unit_Get(map, pos.x, pos.y);
    SDL_assert(friendly != TNECS_NULL);
    sota->combat.defendant = friendly;

    /* -- Check: enemy is really in range of unit -- */
    struct Position *agg_pos    = IES_GET_COMPONENT(sota->ecs.world, npc_ent,  Position);
    struct Position *dft_pos    = IES_GET_COMPONENT(sota->ecs.world, friendly, Position);
    struct Unit     *aggressor  = IES_GET_COMPONENT(sota->ecs.world, npc_ent,  Unit);
    if (!Unit_inRange_Loadout(aggressor, agg_pos, dft_pos, ITEM_ARCHETYPE_WEAPON)) {
        /* Skipping attack, not actually in range */
        Game_Unit_Wait(sota, npc_ent);
        return;
    }

    /* -- Set npc_ent for waiting after combat -- */
    sota->selected.unit_entity = npc_ent;

    /* -- Compute Combat outcome -- */
    Game_Combat_Outcome(sota);

    Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
}

/* -- Decider Move FSM -- */
static b32 _AI_Decider_Move_Never(struct Game *sota, tnecs_entity npc_ent) {
    SDL_LogDebug(SOTA_LOG_AI, "AI Move Decider: AI_MOVE_NEVER set, skipping");
    return (false);
}

static b32 _AI_Decider_Move_Always(struct Game *sota, tnecs_entity npc_ent) {
    SDL_LogDebug(SOTA_LOG_AI, "AI Move Decider: AI_MOVE_ALWAYS set, moving");
    return (true);
}

static b32 _AI_Decider_Move_inRange(struct Game *sota, tnecs_entity npc_ent) {
    /* --- Move to enemy in range --- */
    /* --- Get list of defendants in range --- */
    Map *map = Game_Map(sota);

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    map_to.move         = true;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = npc_ent;
    Map_Act_To(map, map_to);

    tnecs_entity *dfts = DARR_INIT(dfts, tnecs_entity, 1);

    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.attacktolist;
    mapfind.found      = dfts;
    mapfind.seeker     = npc_ent;
    mapfind.fastquit   = true;
    mapfind.eq_type    = LOADOUT_EQUIPPED;

    dfts = Map_Find_Defendants(map, mapfind);

    /* --- Move if enemy in range --- */
    b32 out = (DARR_NUM(dfts) > 0);
    SDL_LogDebug(SOTA_LOG_AI, "AI Move Decider: AI_MOVE_INRANGE set, %zu enemies", DARR_NUM(dfts));
    DARR_FREE(dfts);
    return (out);
}

static b32 _AI_Decider_Move_Trigger(  struct Game *sota, tnecs_entity npc_ent) {
    // TODO: input trigger condition
    return (false);
}

static b32 _AI_Decider_Move_onChapter(struct Game *sota, tnecs_entity npc_ent) {
    /* --- Move only after turn_move turns elapsed --- */
    struct Unit_AI *ai = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit_AI);
    SDL_assert(ai != NULL);
    Map *map = Game_Map(sota);
    SDL_LogDebug(SOTA_LOG_AI, "AI Move Decider: AI_MOVE_onChapter set, (%d > %d)",
                 map->turn, ai->turn_move);
    return (map->turn > ai->turn_move);
}

/* -- Master Deciders -- */
static void _AI_Decider_Master_Kill(struct Game *sota, tnecs_entity npc_ent,
                                    struct AI_Action *action) {
    /* --- AI Unit tries to kill enemy --- */
    /* -- Get list of defendants in range -- */
    /* - MapAct settings for attacktolist - */
    Map *map = Game_Map(sota);
    MapAct map_to = MapAct_default;

    map_to.move         = true;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = npc_ent;
    Map_Act_To(map, map_to);

    tnecs_entity *defendants = DARR_INIT(defendants, tnecs_entity, 4);

    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.attacktolist;
    mapfind.found      = defendants;
    mapfind.seeker     = npc_ent;
    mapfind.fastquit   = false;
    mapfind.eq_type    = LOADOUT_EQUIPMENT;

    defendants = Map_Find_Defendants(map, mapfind);

    /* - TODO: Decide which loadout to attack with - */

    if (DARR_NUM(defendants) < 1) {
        /* -- BRANCH 1- No enemies in range -- */
        SDL_LogDebug(SOTA_LOG_AI, "AI Decider Master Kill: No enemies in range.");

        /* - Find closest enemy - */
        struct Position *posc = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Position);
        SDL_assert(posc != NULL);
        struct Point pos = posc->tilemap_pos;
        tnecs_entity closest =  Map_Find_Friendly_Closest(map, pos.x, pos.y);
        SDL_assert(closest != TNECS_NULL);
        struct Position *pos_closest = IES_GET_COMPONENT(sota->ecs.world, closest, Position);
        SDL_assert(pos_closest != NULL);

        /* - Set target_move to closest enemy position - */
        action->target_action.x = pos_closest->tilemap_pos.x;
        action->target_action.y = pos_closest->tilemap_pos.y;
        action->action = AI_ACTION_WAIT;
        DARR_FREE(defendants);
        return;
    }

    /* -- BRANCH 2- Enemies in range -- */
    SDL_LogDebug(SOTA_LOG_AI, "AI Decider master Kill: Enemies in range.");

    /* -- TODO: Find easiest enemy to kill -- */
    tnecs_entity defendant = defendants[0];

    /* - Set target_move to unoccupied tile in range (attackfrom) - */
    // Map_Attackfrommap_Compute(map, npc_ent, defendant, true, true);
    map_to.move         = true;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPPED;
    map_to.output_type  = ARRAY_LIST;
    map_to.aggressor    = npc_ent;
    map_to.defendant    = defendant;
    i32 *attackfromlist = Map_Act_To(map, map_to);

    /* Should be at least   on tile to attack from. */
    SDL_assert(DARR_NUM(attackfromlist) > 0);

    // TODO: find good tile to attack from
    action->target_move.x = -1;
    action->target_move.y = -1;
    // action->target_move.x = attackfromlist[0];
    // action->target_move.y = attackfromlist[1];

    // int index = action->target_move.y * Map_col_len(map) + action->target_move.x;
    // SDL_assert(map->darrs.unitmap[index] == TNECS_NULL);

    /* - Set target_action to enemy-occupied tile - */
    struct Position *pos = IES_GET_COMPONENT(sota->ecs.world, defendant, Position);
    SDL_assert(pos);
    action->target_action = pos->tilemap_pos;

    action->action = AI_ACTION_ATTACK;
    DARR_FREE(defendants);
}

static void _AI_Decider_Master_Staff(struct Game *sota, tnecs_entity npc_ent,
                                     struct AI_Action *action) {
    /* --- AI Unit tries to use staff --- */

}

static void _AI_Decider_Master_Nothing(struct Game *sota, tnecs_entity npc_ent,
                                       struct AI_Action *action) {
    SDL_LogDebug(SOTA_LOG_AI, "AI Decider: AI_PRIORITY_DO_NOTHING set.");
    action->action = AI_ACTION_WAIT;
}

static void _AI_Decider_Master_Move_To(struct Game *sota, tnecs_entity npc_ent,
                                       struct AI_Action *action) {
    /* --- Set target move to ultimate move_to target --- */
    struct Unit_AI *ai = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit_AI);
    action->target_action = ai->target_move;

    /* -- Set target_move to closest tile on way to target_action -- */
    _AI_Decide_Move(sota, npc_ent, action);

    SDL_assert(ai->priority_slave >= AI_PRIORITY_START);
    SDL_assert(ai->priority_slave < AI_PRIORITY_NUM);
    if (AI_Decider_slave[ai->priority_slave] != NULL)
        AI_Decider_slave[ai->priority_slave](sota, npc_ent, action);
}

/* -- Slave Deciders -- */
static void _AI_Decider_Slave_Kill(struct Game *sota, tnecs_entity npc_ent,
                                   struct AI_Action *action) {
    /* --- AI unit tries to kill enemy on way to primary target --- */
    Map *map = Game_Map(sota);
    SDL_assert((action->target_move.x >= 0) && (action->target_move.x < Map_col_len(map)));
    SDL_assert((action->target_move.y >= 0) && (action->target_move.y < Map_row_len(map)));

    /* -- Find targets to attack after target_move was set -- */
    struct Position *pos = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Position);
    struct Point oldpos  = pos->tilemap_pos;
    struct Point newpos  = action->target_move;

    /* -- Find defendants at target_move -- */
    pos->tilemap_pos = newpos;

    /* - MapAct settings for attacktolist - */
    MapAct map_to = MapAct_default;

    map_to.move         = false;
    map_to.archetype    = ITEM_ARCHETYPE_WEAPON;
    map_to.eq_type      = LOADOUT_EQUIPMENT;
    map_to.output_type  = ARRAY_MATRIX;
    map_to.aggressor    = npc_ent;
    Map_Act_To(map, map_to);

    tnecs_entity *defendants = DARR_INIT(defendants, tnecs_entity, 4);

    MapFind mapfind = MapFind_default;

    mapfind.list       = map->darrs.attacktolist;
    mapfind.found      = defendants;
    mapfind.seeker     = npc_ent;
    mapfind.fastquit   = false;
    mapfind.eq_type    = LOADOUT_EQUIPMENT;

    defendants = Map_Find_Defendants(map, mapfind);

    /* -- Revert position to previous -- */
    pos->tilemap_pos = oldpos;

    /* No attackable units tiles */
    if (DARR_NUM(defendants) < 1) {
        SDL_LogDebug(SOTA_LOG_AI, "AI Decider Slave Kill: No tiles to attack enemy from.");
        action->action = AI_ACTION_WAIT;
        DARR_FREE(defendants);
        return;
    }

    /* - TODO: Find easiest unit to kill - */
    action->patient = defendants[0];
    action->action  = AI_ACTION_ATTACK;
    DARR_FREE(defendants);
}

tnecs_entity AI_Decide_Next(struct Game *sota) {
    /* --- AI finds next unit to act --- */
    struct Game_AI *game_ai = &sota->ai;
    // TODO: better function for next unit
    //  - How does AI decide Who goes next??
    //      - Go through all units with certain priorities, for a priority order
    //      - Go through units always in same order in enemies_onmap
    //      - Go through units in random order
    //      - Use Unit master/slave AI_Priority to decide

    game_ai->npc_i = 0;
    return (game_ai->npcs[game_ai->npc_i]);
}

/* --- FSM --- */
const AI_Decider AI_Decider_master[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ &_AI_Decider_Master_Kill,
    /* AI_PRIORITY_PATROL       */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ &_AI_Decider_Master_Staff,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ &_AI_Decider_Master_Nothing,
    /* AI_PRIORITY_MOVE_TO      */ &_AI_Decider_Master_Move_To,
};

const AI_Decider AI_Decider_slave[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ &_AI_Decider_Slave_Kill,
    /* AI_PRIORITY_PATROL       */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ NULL,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ NULL,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
};

const AI_Doer AI_Act_action[AI_ACTION_NUM] = {
    /* ITEMS            */ NULL, // TODO
    /* TALK             */ NULL, // TODO
    /* STAFF            */ NULL, // TODO
    /* DANCE            */ NULL, // TODO
    /* RESCUE           */ NULL, // TODO
    /* SEIZE            */ NULL, // TODO
    /* ESCAPE           */ NULL, // TODO
    /* ATTACK           */ &_AI_Doer_Attack,
    /* VILLAGE          */ NULL, // TODO
    /* TRADE            */ NULL, // TODO
    /* MAP              */ NULL, /* PC only */
    /* WAIT             */ &_AI_Doer_Wait,
    /* OPEN             */ NULL, // TODO
    /* QUIT             */ NULL, /* PC only */
    /* END_TURN         */ NULL, /* PC only */
    /* UNITS            */ NULL, /* PC only */
    /* CONVOY           */ NULL, /* PC only */
    /* GLOBAL_RANGE     */ NULL, /* PC only */
    /* NEW_GAME         */ NULL, /* PC only */
    /* LOAD             */ NULL, /* PC only */
    /* ERASE            */ NULL, /* PC only */
    /* COPY             */ NULL, /* PC only */
    /* OPTIONS          */ NULL, /* PC only */
    /* EXTRAS           */ NULL, /* PC only */
    /* DEBUG_MAP        */ NULL, /* PC only */
};

const AI_Decider_Move AI_Decider_move[AI_MOVE_NUM] = {
    /* AI_MOVE_ALWAYS       */ _AI_Decider_Move_Always,
    /* AI_MOVE_ONCHAPTER    */ _AI_Decider_Move_onChapter,
    /* AI_MOVE_INRANGE      */ _AI_Decider_Move_inRange,
    /* AI_MOVE_TRIGGER      */ _AI_Decider_Move_Trigger,
    /* AI_MOVE_NEVER        */ _AI_Decider_Move_Never,

};

/* --- GLOBAL FUNCTIONS --- */
void AI_Decide_Action(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* --- AI decides which action to take with current unit --- */
    /* --- PRELIMINARIES --- */
    *action = AI_Action_default;
    struct Unit     *npc    = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit);
    struct Unit_AI  *ai     = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit_AI);
    SDL_assert(npc != NULL);
    SDL_assert(ai  != NULL);

    SDL_assert(ai->priority_master > AI_PRIORITY_START);
    SDL_assert(ai->priority_master < AI_PRIORITY_NUM);

    if (AI_Decider_master[ai->priority_master] != NULL)
        AI_Decider_master[ai->priority_master](sota, npc_ent, action);
}

void AI_Decide_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    Map *map = Game_Map(sota);

    /* --- AI decides where to move unit depending on action to take --- */
    /* --- Skip depending on movement priority --- */
    struct Unit_AI  *ai  = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit_AI);
    SDL_assert(ai  != NULL);

    /* AI_Decider_move function decides if AI unit moves or not */
    if (!AI_Decider_move[ai->move](sota, npc_ent)) {
        // SDL_Log("Don't move cause decider");
        return;
    }

    b32 set_x   = (action->target_move.x >= 0);
    set_x      &= (action->target_move.x < Map_col_len(map));
    b32 set_y   = (action->target_move.y >= 0);
    set_y      &= (action->target_move.y < Map_row_len(map));

    /* Skip if target_move was set previously by action decider */
    if (set_y && set_x) {
        // SDL_Log("AI Move Decider: target_move set, skipping");
        return;
    }

    /* -- Set target_move to closest tile on way to target_action -- */
    _AI_Decide_Move(sota, npc_ent, action);
}

void _AI_Decide_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- Set target_move to closest tile on way to target_action -- */

    /* -- Skip movement if target is at current position -- */
    struct Unit     *npc    = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit);
    struct Position *pos    = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Position);
    struct Point target = action->target_action;
    struct Point start  = pos->tilemap_pos;
    if ((target.x == start.x) && (target.y == start.y)) {
        // SDL_Log("AI Move Decider: target is current position. Skipping.");
        return;
    }

    /* -- Compute costmap for pathfinding -- */
    Map *map = Game_Map(sota);
    map->world = sota->ecs.world;
    Map_Costmap_Movement_Compute(map, npc_ent);
    i32 *costmap            = map->darrs.costmap;
    tnecs_entity *unitmap   = map->darrs.unitmap;
    int effective_move = 0;
    Unit_computeMove(npc, &effective_move);
    effective_move *= Map_Cost_Multiplier(map);
    SDL_assert(costmap != NULL);
    SDL_assert((target.x >= 0) && (target.x < Map_col_len(map)));
    SDL_assert((target.y >= 0) && (target.y < Map_row_len(map)));
    SDL_assert((start.x  >= 0) && (start.x  < Map_col_len(map)));
    SDL_assert((start.y  >= 0) && (start.y  < Map_row_len(map)));

    i16 row_len     = Map_row_len(map);
    i16 col_len     = Map_col_len(map);

    /* -- Pathfinding --  */
    int *path_list  = DARR_INIT(path_list, int, 16);

    // printf("costmap\n\n");
    // matrix_print(costmap, row_len, col_len);
    // printf("unitmap\n\n");
    // entity_print(unitmap, row_len, col_len);

    path_list       = Pathfinding_Astar_plus(path_list, costmap, unitmap,
                                             row_len, col_len, effective_move,
                                             start, target, true);

    int point_num       = DARR_NUM(path_list) / TWO_D;
    // int *computed_path  = list2matrix(path_list, row_len, col_len, point_num);
    // printf("computed_path\n\n");
    // matrix_print(computed_path, row_len, col_len);

    int index   = point_num - 1;
    action->target_move.x = path_list[index * TWO_D];
    action->target_move.y = path_list[index * TWO_D + 1];

    DARR_FREE(path_list);
}

/* --- Move unit to target_move --- */
void AI_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    Map *map = Game_Map(sota);

    struct Unit_AI *ai     = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Unit_AI);
    struct Position *pos    = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Position);
    // TODO: wait until previous combat is finished before moving
    /* -- AI moves, after taking the decision -- */

    /* -- Skip no movement -- */
    if (ai->move == AI_MOVE_NEVER) {
        // SDL_Log("AI Move: AI_MOVE_NEVER set. Skipping.");
        return;
    }

    /* -- Skip no movement -- */
    b32 null_x   = (action->target_move.x < 0);
    null_x      |= (action->target_move.x >= Map_col_len(map));
    b32 null_y   = (action->target_move.y < 0);
    null_y      |= (action->target_move.y >= Map_row_len(map));
    if (null_x || null_y) {
        SDL_LogWarn(SOTA_LOG_AI, "AI Move: target_move is outside bounds. Skipping");
        return;
    }

    struct Sprite   *sprite = IES_GET_COMPONENT(sota->ecs.world, npc_ent, Sprite);
    SDL_assert(pos      != NULL);
    SDL_assert(sprite   != NULL);

    // TODO: Movement Animation
    struct Point old = pos->tilemap_pos;
    struct Point new = action->target_move;
    int old_index = old.y * Map_col_len(map) + old.x;
    int new_index = new.y * Map_col_len(map) + new.x;
    if ((new.x == old.x) && (new.y == old.y)) {
        SDL_LogWarn(SOTA_LOG_AI, "AI Move: target_move is current position. Skipping");
        return;
    }

    // entity_print(map->darrs.unitmap, Map_row_len(map), Map_col_len(map));
    // entity_print(map->darrs.unitmap, Map_row_len(map), Map_col_len(map));
    SDL_assert(map->darrs.unitmap[old_index] == npc_ent);
    SDL_assert(map->darrs.unitmap[new_index] == TNECS_NULL);

    SDL_assert(old.y > 0);
    SDL_assert(old.y < Map_row_len(map));
    SDL_assert(old.x > 0);
    SDL_assert(old.x < Map_col_len(map));
    SDL_assert(new.y > 0);
    SDL_assert(new.y < Map_row_len(map));
    SDL_assert(new.x > 0);
    SDL_assert(new.x < Map_col_len(map));

    SDL_assert(map->darrs.unitmap[old_index] > TNECS_NULL);

    tnecs_entity ent = map->darrs.unitmap[old_index];
    Map_Unit_Move(map, old.x, old.y, new.x, new.y);

    SDL_assert(map->darrs.unitmap[new_index] == npc_ent);
    pos = IES_GET_COMPONENT(map->world, npc_ent, Position);
    SDL_assert(pos->tilemap_pos.x == new.x);
    SDL_assert(pos->tilemap_pos.y == new.y);
    SDL_assert(map->darrs.unitmap[new_index] == ent);
}

void AI_Act(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- AI acts, after taking the decision -- */
    if (AI_Act_action[action->action] != NULL)
        /* -- Skip if no action -- */
        AI_Act_action[action->action](sota, npc_ent, action);
}

/* --- AI_State --- */
void Game_AI_Free(struct Game_AI *game_ai) {
    if (game_ai->npcs != NULL) {
        DARR_FREE(game_ai->npcs);
        game_ai->npcs = NULL;
    }
}
void Game_AI_Init(struct Game_AI *game_ai, tnecs_world *world, struct Map *map) {
    /* -- Init game_ai->npc --  */
    if (game_ai->npcs == NULL) {
        game_ai->npcs  = DARR_INIT(game_ai->npcs, tnecs_entity, 16);
    }

    game_ai->init = true;

    /* -- Find all units in current army -- */
    i8 army = map->units.onfield.arr[map->armies.current];
    for (int i = 0; i < DARR_NUM(map->units.onfield.arr); i++) {
        tnecs_entity npc_ent = map->units.onfield.arr[i];
        struct Unit *unit = IES_GET_COMPONENT(world, npc_ent, Unit);
        /* Skip if unit is waiting e.g. a reinforcement */
        if (Unit_isWaiting(unit))
            continue;

        if (Unit_Army(unit) == army)
            DARR_PUT(game_ai->npcs, npc_ent);
    }
}

void Game_AI_Pop(struct Game_AI *game_ai, tnecs_world *world) {
    // tnecs_entity npc_ent = game_ai->npcs[game_ai->npc_i];
    DARR_DEL(game_ai->npcs, game_ai->npc_i);
    game_ai->decided       = false;
    game_ai->move_anim     = false;
    game_ai->act_anim      = false;
    game_ai->npc_i         = -1;
}

void Game_AI_Turn_Start( struct Game_AI *game_ai) {
    game_ai->turn_over     = false;
    // game_ai->init          = false;
    game_ai->decided       = false;
    game_ai->move_anim     = false;
    game_ai->act_anim      = false;
    game_ai->npc_i         = -1;
}

void Game_AI_Turn_Finish(struct Game_AI *game_ai) {
    game_ai->turn_over     = true;
    game_ai->init          = false;
    game_ai->decided       = false;
    game_ai->move_anim     = false;
    game_ai->act_anim      = false;
    game_ai->npc_i         = -1;
}

/* --- I/O --- */
void AI_readJSON(void *input, const cJSON *jai) {
    struct Unit_AI *ai = (struct Unit_AI *)input;
    SDL_assert(ai);
    cJSON *jpriority_master     = cJSON_GetObjectItem(jai, "priority_master");
    cJSON *jpriority_slave      = cJSON_GetObjectItem(jai, "priority_slave");
    cJSON *jmove                = cJSON_GetObjectItem(jai, "move");
    cJSON *jturn_move           = cJSON_GetObjectItem(jai, "turn_move");
    cJSON *jtarget_move         = cJSON_GetObjectItem(jai, "target_move");

    if (jturn_move != NULL)
        ai->turn_move = cJSON_GetNumberValue(jturn_move);

    if (jtarget_move != NULL) {
        SDL_assert(cJSON_IsArray(jtarget_move));
        SDL_assert(cJSON_GetArraySize(jtarget_move) == 2);
        cJSON *jtarget_move_x   = cJSON_GetArrayItem(jtarget_move, 0);
        cJSON *jtarget_move_y   = cJSON_GetArrayItem(jtarget_move, 1);
        ai->target_move.x       = cJSON_GetNumberValue(jtarget_move_x);
        ai->target_move.y       = cJSON_GetNumberValue(jtarget_move_y);
    }

    ai->priority_master = cJSON_GetNumberValue(jpriority_master);
    ai->priority_slave  = cJSON_GetNumberValue(jpriority_slave);
    ai->move            = cJSON_GetNumberValue(jmove);
}

i32 AI_ID_isvalid(i32 ai_id) {
    return ((ai_id > AI_NULL) && (ai_id < AI_NUM));
}

void AI_writeJSON(const void *input,  cJSON *jai) {
    struct Unit_AI *ai = (struct Unit_AI *)input;
    SDL_assert(ai);

    cJSON *jpriority_master = cJSON_CreateNumber(ai->priority_master);
    cJSON *jpriority_slave  = cJSON_CreateNumber(ai->priority_slave);
    cJSON *jmove            = cJSON_CreateNumber(ai->move);
    cJSON *jtarget_move     = cJSON_CreateArray();
    cJSON *jtarget_move_x   = cJSON_CreateNumber(ai->target_move.x);
    cJSON *jtarget_move_y   = cJSON_CreateNumber(ai->target_move.y);
    cJSON *jturn_move       = cJSON_CreateNumber(ai->turn_move);

    cJSON_AddItemToArray(jtarget_move, jtarget_move_x);
    cJSON_AddItemToArray(jtarget_move, jtarget_move_y);
    cJSON_AddItemToObject(jai, "priority_master",   jpriority_master);
    cJSON_AddItemToObject(jai, "priority_slave",    jpriority_slave);
    cJSON_AddItemToObject(jai, "move",              jmove);
    cJSON_AddItemToObject(jai, "target_move",       jtarget_move);
    cJSON_AddItemToObject(jai, "turn_move",         jturn_move);
}

void Unit_Move_onMap_Animate(struct Game  *sota,  tnecs_entity entity,
                             struct Timer *timer, struct UnitMoveAnimation *anim) {
    if (timer->time_ns >= anim->time_ns) {
        // SDL_Log("Unit Animation Finished");
        /* Remove component to stop calling __func__ */
        TNECS_REMOVE_COMPONENTS(sota->ecs.world, entity, UnitMoveAnimation_ID);
        return;
    }
}

void Game_AI_Enemy_Turn(struct Game *sota) {
    SDL_Log(__func__);
    /* --- AI CONTROL --- */
    // TODO: ai_control entity
    // TODO: Don't check for loss every frame
    Map *map = Game_Map(sota);
    if (Map_isLost(map)) {
        // SDL_Log("AI CONTROL -> LOSS");
        Event_Emit(__func__, SDL_USEREVENT, event_Game_Over, NULL, NULL);
        return;
    }

    /* -- Reinforcements timer: pause before moving units -- */
    /* -- TODO: Rename to AI timer? */
    // TODO: Animate reinforcements
    if (sota->timers.reinf != TNECS_NULL) {
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        SDL_assert(timer != NULL);
        // u64 limit = sota->settings.enemy_turn_settings.pause_post_reinforcement;
        if (timer->time_ns <= timer->limit_ns)
            return;

        tnecs_entity_destroy(sota->ecs.world, sota->timers.reinf);
        sota->timers.reinf = TNECS_NULL;
    }

    /* -- Skip if turn is over -- */
    if (sota->ai.turn_over)
        return;

    /* -- Build list of npcs to control -- */
    if (sota->ai.init == false) {
        SDL_LogDebug(SOTA_LOG_AI, "Building NPC list");
        Game_AI_Init(&sota->ai, sota->ecs.world, map);
    }
    SDL_assert(sota->ai.npcs != NULL);

    /* -- If no more NPCs, end NPC turn. -- */
    if (sota->ai.npcs && (DARR_NUM(sota->ai.npcs) < 1)) {
        // SDL_Log("AI Turn Finished");
        Game_AI_Turn_Finish(&sota->ai);
        Event_Emit(__func__, SDL_USEREVENT, event_Turn_End, NULL, NULL);
        return;
    }

    /* -- Decide next NPC to act -- */
    if (sota->ai.npc_i < 0) {
        tnecs_entity debug = AI_Decide_Next(sota);
        SDL_LogDebug(SOTA_LOG_AI, "Next npc entity: %lld", debug);
    }
    tnecs_entity npc_ent = sota->ai.npcs[sota->ai.npc_i];

    SDL_assert(npc_ent != TNECS_NULL);

    /* -- AI decides what to do with unit -- */
    // If not previously decided for npc_ent, decide
    b32 decided     = sota->ai.decided;
    if (!decided) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Decide");
        AI_Decide_Action(sota, npc_ent, &sota->ai.action);
        AI_Decide_Move(  sota, npc_ent, &sota->ai.action);
        sota->ai.decided = true;
    }

    decided         = sota->ai.decided;
    b32 act_anim    = sota->ai.act_anim;
    b32 move_anim   = sota->ai.move_anim;

    /* -- AI moves unit -- */
    // TODO: wait on combat to finish!
    if (decided && !move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Move");
        SDL_assert(!act_anim);
        AI_Move(sota, npc_ent, &sota->ai.action);
        // TODO: Move animation
        sota->ai.move_anim = true;
    }

    /* Check if move_anim updated during frame */
    move_anim   = sota->ai.move_anim;
    act_anim    = sota->ai.act_anim;

    /* -- AI acts unit -- */
    if (decided && move_anim && !act_anim) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Act");
        AI_Act(sota, npc_ent, &sota->ai.action);
        // TODO: Act animation
        sota->ai.act_anim = true;
    }

    /* Check if act_anim updated during frame */
    act_anim    = sota->ai.act_anim;

    /* -- Pop unit from list in AI_State -- */
    if ((act_anim) && ((DARR_NUM(sota->ai.npcs) > 0))) {
        SDL_LogDebug(SOTA_LOG_AI, "AI_Pop");
        Game_AI_Pop(&sota->ai, sota->ecs.world);

        SDL_LogDebug(SOTA_LOG_AI, "AI: Pause AFTER AI_act");
        /* Pause AFTER AI action */
        sota->timers.reinf   = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->ecs.world, Timer_ID);
        struct Timer *timer = IES_GET_COMPONENT(sota->ecs.world, sota->timers.reinf, Timer);
        *timer = Timer_default;
        timer->limit_ns = sota->settings.enemy_turn_settings.pause_post_move;
    }
}