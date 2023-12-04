
#include "AI.h"

struct AI AI_default = {
    .json_element   = JSON_AI,
    .json_filename  = {0},

    .priority_master  = AI_PRIORITY_START,
    .priority_slave   = AI_PRIORITY_START,
    .move             = AI_MOVE_START,
};

struct AI_State AI_State_default = {
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

struct AI_Action AI_Action_default =  {
    .target_move     = {-1, -1},
    .target_action   = {-1, -1},
    .action          = AI_ACTION_START,
};

AI_Decider AI_Decider_master[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ &_AI_Decider_Action_Kill,
    /* AI_PRIORITY_PATROL       */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ &_AI_Decider_Action_Staff,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ &_AI_Decider_Action_Nothing,
    /* AI_PRIORITY_MOVE_TO      */ &_AI_Decider_Action_Move_To,
};

AI_Decider AI_Decider_slave[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ NULL,
    /* AI_PRIORITY_PATROL       */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ NULL,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ NULL,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
};

/* AfterMove is slave to AI_PRIORITY_MOVE_TO master priority */
AI_Decider AI_Decider_AfterMove[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ &_AI_Decider_Kill_AfterMove,
    /* AI_PRIORITY_PATROL       */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ &_AI_Decider_Staff_AfterMove,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ NULL,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
};

AI_Doer AI_Act_action[AI_ACTION_NUM] = {
    /* ITEMS            */ NULL,
    /* TALK             */ NULL,
    /* STAFF            */ NULL,
    /* DANCE            */ NULL,
    /* RESCUE           */ NULL,
    /* SEIZE            */ NULL,
    /* ESCAPE           */ NULL,
    /* ATTACK           */ &_AI_Doer_Attack,
    /* VILLAGE          */ NULL,
    /* TRADE            */ NULL,
    /* MAP              */ NULL,
    /* WAIT             */ &_AI_Doer_Wait,
    /* OPEN             */ NULL,
    /* QUIT             */ NULL,
    /* END_TURN         */ NULL,
    /* UNITS            */ NULL,
    /* CONVOY           */ NULL,
    /* GLOBAL_RANGE     */ NULL,
    /* NEW_GAME         */ NULL,
    /* LOAD             */ NULL,
    /* ERASE            */ NULL,
    /* COPY             */ NULL,
    /* OPTIONS          */ NULL,
    /* EXTRAS           */ NULL,
    /* DEBUG_MAP        */ NULL,
};

AI_Decider_Move AI_Decider_move[AI_MOVE_NUM] = {
    /* AI_MOVE_ALWAYS       */ _AI_Decider_Move_Always,
    /* AI_MOVE_ONCHAPTER    */ _AI_Decider_Move_onChapter,
    /* AI_MOVE_INRANGE      */ _AI_Decider_Move_inRange,
    /* AI_MOVE_TRIGGER      */ _AI_Decider_Move_Trigger,
    /* AI_MOVE_NEVER        */ _AI_Decider_Move_Never,

};

b32 _AI_Decider_Move_Never(struct Game *sota, tnecs_entity npc_ent) {
    return (false);
}

b32 _AI_Decider_Move_Always(struct Game *sota, tnecs_entity npc_ent) {
    return (true);
}

b32 _AI_Decider_Move_inRange(  struct Game *sota, tnecs_entity npc_ent) {
    return (false);
}

b32 _AI_Decider_Move_Trigger(  struct Game *sota, tnecs_entity npc_ent) {
    // TODO: input trigger condition
    return (false);
}

b32 _AI_Decider_Move_onChapter(struct Game *sota, tnecs_entity npc_ent) {
    // TODO: input number of chapters
    return (false);
}


void _AI_Decider_Action_Nothing(struct Game *sota, tnecs_entity npc_ent,
                                struct AI_Action *action) {
    action->action = AI_ACTION_WAIT;
}

void _AI_Decider_Action_Move_To(struct Game *sota, tnecs_entity npc_ent,
                                struct AI_Action *action) {
    /* Set target move to ultimate move_to target*/
    struct AI   *ai     = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);
    action->target_action = ai->target_move;
}


void _AI_Decider_Action_Kill(struct Game *sota, tnecs_entity npc_ent,
                             struct AI_Action *action) {
    /* -- Get list of defendants in range -- */
    Map_Attacktomap_Compute(sota->map, sota->world, npc_ent, true, false);
    Map_Attacktolist_Compute(sota->map);
    tnecs_entity *defendants = DARR_INIT(defendants, tnecs_entity, 4);
    defendants = Map_Find_Defendants(sota->map, sota->map->attacktolist, defendants, npc_ent, false);

    /* - TODO: Decide which loadout to attack with - */

    if (DARR_NUM(defendants) < 1) {
        /* -- BRANCH 1- No enemies in range -- */
        /* - Find closest enemy - */
        struct Position *posc = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);
        SDL_assert(posc != NULL);
        struct Point pos = posc->tilemap_pos;
        tnecs_entity closest =  Map_Find_Friendly_Closest(sota->map, pos.x, pos.y);
        SDL_assert(closest != TNECS_NULL);
        struct Position *pos_closest = TNECS_GET_COMPONENT(sota->world, closest, Position);
        SDL_assert(pos_closest != NULL);

        /* - Set target_move to closest enemy position - */
        action->target_action.x = pos_closest->tilemap_pos.x;
        action->target_action.y = pos_closest->tilemap_pos.y;
        action->action = AI_ACTION_WAIT;
        DARR_FREE(defendants);
        return;
    }

    /* -- BRANCH 2- Enemies in range -- */
    /* -- TODO: Find easiest enemy to kill -- */
    tnecs_entity defendant = defendants[0];

    /* - Set target_move to unoccupied tile in range (attackfrom) - */
    Map_Attackfrommap_Compute(sota->map, sota->world, npc_ent, defendant, true, true);
    // SDL_Log("ATTACKFROMMAP");
    // matrix_print(sota->map->attackfrommap, sota->map->row_len, sota->map->col_len);

    i32 *attackfromlist = Map_Attackfromlist_Compute(sota->map);
    /* Should be at least on tile to attack from. */
    SDL_assert(DARR_NUM(attackfromlist) > 0);

    // TODO: find good tile to attack from
    // TODO: move to decider move?
    action->target_move.x = attackfromlist[0];
    action->target_move.y = attackfromlist[1];

    /* - Set target_action to enemy-occupied tile - */
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, defendant, Position);
    SDL_assert(pos);
    action->target_action = pos->tilemap_pos;

    action->action = AI_ACTION_ATTACK;
    DARR_FREE(defendants);
}

void _AI_Decider_Action_Staff(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {

}

/* Tries to staff unit after moving to target tile */
void _AI_Decider_Staff_AfterMove(struct Game *sota, tnecs_entity npc_ent,
                                 struct AI_Action *action) {
    // TODO
    action->action  = AI_ACTION_WAIT;
}

/* Tries to attack unit after moving to target tile */
void _AI_Decider_Kill_AfterMove(struct Game *sota, tnecs_entity npc_ent,
                                struct AI_Action *action) {
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);
    struct Point oldpos = pos->tilemap_pos;
    struct Point newpos;
    /* - Compute attacktolist - */

    /* -- If no movement, use current position as newpos -- */
    b32 null_x   = (action->target_move.x < 0);
    null_x      |= (action->target_move.x >= sota->map->col_len);
    b32 null_y   = (action->target_move.y < 0);
    null_y      |= (action->target_move.y >= sota->map->row_len);
    if (null_x || null_y)
        newpos = pos->tilemap_pos;
    else
        newpos = action->target_move;

    /* -- Find defendants at target_move -- */
    pos->tilemap_pos = newpos;
    Map_Attacktomap_Compute(sota->map, sota->world, npc_ent, false, false);
    Map_Attacktolist_Compute(sota->map);
    tnecs_entity *defendants = DARR_INIT(defendants, tnecs_entity, 4);
    defendants = Map_Find_Defendants(sota->map, sota->map->attacktolist, defendants, npc_ent, false);

    /* -- Revert position to previous -- */
    pos->tilemap_pos = oldpos;

    /* No attackable units tiles */
    if (DARR_NUM(defendants) < 1) {
        action->action = AI_ACTION_WAIT;
        DARR_FREE(defendants);
        return;
    }

    /* - TODO: Find easiest unit to kill - */
    action->patient = defendants[0];
    action->action  = AI_ACTION_ATTACK;
    DARR_FREE(defendants);
}

entity AI_Decide_Next(struct Game *sota) {
    struct AI_State *ai_state = &sota->ai_state;
    // TODO: better function for next unit
    ai_state->npc_i = 0;
    return (ai_state->npcs[ai_state->npc_i]);
}

void AI_Decide_Action_PreMove(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* --- PRELIMINARIES --- */
    *action = AI_Action_default;
    struct Unit *npc    = TNECS_GET_COMPONENT(sota->world, npc_ent, Unit);
    struct AI   *ai     = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);
    SDL_assert(npc != NULL);
    SDL_assert(ai  != NULL);

    SDL_assert(ai->priority_master > AI_PRIORITY_START);
    SDL_assert(ai->priority_master < AI_PRIORITY_NUM);
    if (AI_Decider_master[ai->priority_master] != NULL)
        AI_Decider_master[ai->priority_master](sota, npc_ent, action);
}

void AI_Decide_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    struct Position *pos    = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);

    /* -- Skip movement if target is at current position -- */
    struct Point target = action->target_action;
    struct Point start  = pos->tilemap_pos;
    if ((target.x == start.x) && (target.y == start.y))
        return;

    /* -- Skip movement if target adjacent to current position -- */
    b32 adjacent_x   = (action->target_action.x == pos->tilemap_pos.x - 1);
    adjacent_x      |= (action->target_action.x == pos->tilemap_pos.x + 1);
    b32 adjacent_y   = (action->target_action.y == pos->tilemap_pos.y - 1);
    adjacent_y      |= (action->target_action.y == pos->tilemap_pos.y + 1);
    if (adjacent_x && adjacent_y)
        return;


    /* --- Skip depending on movement priority --- */
    struct Unit     *npc = TNECS_GET_COMPONENT(sota->world, npc_ent, Unit);
    struct AI       *ai  = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);
    SDL_assert(ai  != NULL);

    // if (AI_Decider_move[ai->move] != NULL)
    // AI_Decider_move[ai->move](sota, npc_ent) ? 0 : return;
    if (!AI_Decider_move[ai->move](sota, npc_ent))
        return;

    /* -- Move to closest tile on way to target_action -- */
    /* -- Compute costmap for pathfinding -- */
    Map_Costmap_Movement_Compute(sota->map, sota->world, npc_ent);
    i32 *costmap        = sota->map->costmap;
    tnecs_entity *unitmap   = sota->map->unitmap;
    int move                = Unit_computeMove(npc);
    SDL_assert(costmap != NULL);
    SDL_assert((target.x >= 0) && (target.x < sota->map->col_len));
    SDL_assert((target.y >= 0) && (target.y < sota->map->row_len));
    SDL_assert((start.x  >= 0) && (start.x  < sota->map->col_len));
    SDL_assert((start.y  >= 0) && (start.y  < sota->map->row_len));

    i16 row_len     = sota->map->col_len;
    i16 col_len     = sota->map->col_len;

    /* -- Pathfinding --  */
    int *path_list  = DARR_INIT(path_list, int, 16);
    path_list       = Pathfinding_Astar_plus(path_list, costmap, unitmap,
                                             row_len, col_len, move,
                                             start, target, true);
    int point_num   = DARR_NUM(path_list) / TWO_D;

    /* -- target_move is furthest point along path unit can move to -- */
    SDL_assert((move > 0) && (move < SOTA_MAX_MOVEMENT));
    int minimum     = (point_num - 1) < move ? (point_num - 1) : move;
    if (minimum > 1) {
        action->target_move.x = path_list[minimum * TWO_D];
        action->target_move.y = path_list[minimum * TWO_D + 1];
    }

    DARR_FREE(path_list);
}

void AI_Decide_Action_AfterMove(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    struct AI *ai = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);

    /* If no action decided yet, wait */
    if (action->action == AI_ACTION_START)
        action->action = AI_ACTION_WAIT;

    /* If priority is AI_PRIORITY_MOVE_TO, need to check for AfterMove action */
    if (ai->priority_master != AI_PRIORITY_MOVE_TO)
        return;

    /* AfterMove action according to slave priority */
    if (AI_Decider_AfterMove[ai->priority_slave] != NULL)
        AI_Decider_AfterMove[ai->priority_slave](sota, npc_ent, action);
}

/* --- Move unit to target_move --- */
void AI_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    struct AI   *ai     = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);

    /* -- Skip no movement -- */
    if (ai->move == AI_MOVE_NEVER)
        return;

    /* -- Skip no movement -- */
    b32 null_x   = (action->target_move.x < 0);
    null_x      |= (action->target_move.x >= sota->map->col_len);
    b32 null_y   = (action->target_move.y < 0);
    null_y      |= (action->target_move.y >= sota->map->row_len);
    if (null_x || null_y)
        return;

    struct Position *pos    = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);
    struct Sprite   *sprite = TNECS_GET_COMPONENT(sota->world, npc_ent, Sprite);
    SDL_assert(pos      != NULL);
    SDL_assert(sprite   != NULL);

    // TODO: Movement Animation
    struct Point old = pos->tilemap_pos;
    struct Point new = action->target_move;
    Map_Unit_Move(sota->map, old.x, old.y, new.x, new.y);
}

void _AI_Doer_Wait(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    Game_Unit_Wait(sota, npc_ent);
}

void _AI_Doer_Attack(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- Set aggressor for combat -- */
    sota->aggressor = npc_ent;

    /* -- Set defendant for combat -- */
    struct Point pos = action->target_action;
    tnecs_entity friendly = Map_Unit_Get(sota->map, pos.x, pos.y);
    SDL_assert(friendly != TNECS_NULL);
    sota->defendant = friendly;

    /* -- Set npc_ent for waiting after combat -- */
    sota->selected_unit_entity = npc_ent;

    Event_Emit(__func__, SDL_USEREVENT, event_Combat_Start, data1_entity, data2_entity);
}

void AI_Act(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- Skip if no action -- */
    if (AI_Act_action[action->action] != NULL)
        AI_Act_action[action->action](sota, npc_ent, action);
}

/* --- AI_State --- */
void AI_State_Init(struct AI_State *ai_state, tnecs_world *world, struct Map *map) {
    /* -- Init ai_state->npc --  */
    if (ai_state->npcs == NULL)
        ai_state->npcs  = DARR_INIT(ai_state->npcs, entity, 16);

    ai_state->init = true;

    /* -- Find all units in current army -- */
    i8 army = map->army_onfield[map->army_i];
    for (int i = 0; i < DARR_NUM(map->units_onfield); i++) {
        entity npc_ent = map->units_onfield[i];
        struct Unit *unit = TNECS_GET_COMPONENT(world, npc_ent, Unit);
        /* Skip if unit is waiting e.g. a reinforcement */
        if (unit->waits)
            continue;

        if (unit->army == army)
            DARR_PUT(ai_state->npcs, npc_ent);
    }
    SDL_LogDebug(SOTA_LOG_AI, "NPC num: %d %d", DARR_NUM(ai_state->npcs),
                 DARR_NUM(map->units_onfield));
}

void AI_State_Pop(struct AI_State *ai_state, tnecs_world *world) {
    entity npc_ent = ai_state->npcs[ai_state->npc_i];
    DARR_DEL(ai_state->npcs, ai_state->npc_i);
    ai_state->decided       = false;
    ai_state->move_anim     = false;
    ai_state->act_anim      = false;
    ai_state->npc_i         = -1;
}

void AI_State_Turn_Start( struct AI_State *ai_state) {
    ai_state->turn_over     = false;
    // ai_state->init          = false;
    ai_state->decided       = false;
    ai_state->move_anim     = false;
    ai_state->act_anim      = false;
    ai_state->npc_i         = -1;
}

void AI_State_Turn_Finish(struct AI_State *ai_state) {
    ai_state->turn_over     = true;
    ai_state->init          = false;
    ai_state->decided       = false;
    ai_state->move_anim     = false;
    ai_state->act_anim      = false;
    ai_state->npc_i         = -1;
}

/* --- I/O --- */
void AI_readJSON(void *input,  cJSON *jai) {
    struct AI *ai = (struct AI *)input;
    SDL_assert(ai);
    cJSON *jpriority_master     = cJSON_GetObjectItem(jai, "priority_master");
    cJSON *jpriority_slave      = cJSON_GetObjectItem(jai, "priority_slave");
    cJSON *jmove                = cJSON_GetObjectItem(jai, "move");
    cJSON *jtarget_move         = cJSON_GetObjectItem(jai, "target_move");
    if (jtarget_move != NULL) {
        SDL_assert(cJSON_IsArray(jtarget_move));
        SDL_assert(cJSON_GetArraySize(jtarget_move) == 2);
        cJSON *jtarget_move_x       = cJSON_GetArrayItem(jtarget_move, 0);
        cJSON *jtarget_move_y       = cJSON_GetArrayItem(jtarget_move, 1);
        ai->target_move.x   = cJSON_GetNumberValue(jtarget_move_x);
        ai->target_move.y   = cJSON_GetNumberValue(jtarget_move_y);
    }

    ai->priority_master = cJSON_GetNumberValue(jpriority_master);
    ai->priority_slave  = cJSON_GetNumberValue(jpriority_slave);
    ai->move            = cJSON_GetNumberValue(jmove);
}

void AI_writeJSON(void *input,  cJSON *jai) {
    struct AI *ai = (struct AI *)input;
    SDL_assert(ai);

    cJSON *jpriority_master = cJSON_CreateNumber(ai->priority_master);
    cJSON *jpriority_slave  = cJSON_CreateNumber(ai->priority_slave);
    cJSON *jmove            = cJSON_CreateNumber(ai->move);
    cJSON *jtarget_move     = cJSON_CreateArray();
    cJSON *jtarget_move_x   = cJSON_CreateNumber(ai->target_move.x);
    cJSON *jtarget_move_y   = cJSON_CreateNumber(ai->target_move.y);

    cJSON_AddItemToArray(jtarget_move, jtarget_move_x);
    cJSON_AddItemToArray(jtarget_move, jtarget_move_y);
    cJSON_AddItemToObject(jai, "priority_master",   jpriority_master);
    cJSON_AddItemToObject(jai, "priority_slave",    jpriority_slave);
    cJSON_AddItemToObject(jai, "move",              jmove);
    cJSON_AddItemToObject(jai, "target_move",       jtarget_move);
}

void Unit_Move_onMap_Animate(struct Game  *sota,  tnecs_entity entity,
                             struct Timer *timer, struct UnitMoveAnimation *anim) {
    SDL_Log("%d %d", timer->time_ns / 1000000, anim->time_ns / 1000000);
    if (timer->time_ns >= anim->time_ns) {
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Unit Animation Finished");
        TNECS_REMOVE_COMPONENTS(sota->world, entity, UnitMoveAnimation);
        return;
    }
}
