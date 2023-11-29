
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
extern struct AI_State AI_State_default;


struct AI_Action AI_Action_default =  {
    .target_move     = {-1, -1},
    .target_action   = {-1, -1},
    .action          = AI_ACTION_START,
};

AI_Decider AI_Decider_master[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ NULL,
    /* AI_PRIORITY_SEIZE        */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ NULL,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ &_AI_Decider_Do_Nothing,
    /* AI_PRIORITY_MOVE_TO      */ &_AI_Decider_Do_Move_To,
    /* AI_PRIORITY_PATROL       */ NULL,
};

AI_Decider AI_Decider_slave[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ NULL,
    /* AI_PRIORITY_SEIZE        */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ NULL,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ NULL,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
    /* AI_PRIORITY_PATROL       */ NULL,
};

AI_Decider AI_Decider_AfterMove[AI_PRIORITY_NUM] = {
    /* AI_PRIORITY_KILL         */ _AI_Decider_Kill_AfterMove,
    /* AI_PRIORITY_SEIZE        */ NULL,
    /* AI_PRIORITY_LOOT         */ NULL,
    /* AI_PRIORITY_STAFF        */ _AI_Decider_Staff_AfterMove,
    /* AI_PRIORITY_SURVIVE      */ NULL,
    /* AI_PRIORITY_FLEE         */ NULL,
    /* AI_PRIORITY_SKILL        */ NULL,
    /* AI_PRIORITY_DO_NOTHING   */ NULL,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
    /* AI_PRIORITY_PATROL       */ NULL,
};


void _AI_Decider_Do_Nothing(struct Game *sota, tnecs_entity npc_ent,
                            struct AI_Action *action) {
    action->action = AI_ACTION_WAIT;
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

    /* - Compute attacktolist - */
    Map_Attacktomap_Compute(sota->map, sota->world, npc_ent, false, false);
    Map_Attacktolist_Compute(sota->map);
    tnecs_entity *defendants = DARR_INIT(defendants, tnecs_entity, 4);
    defendants = Map_Find_Defendants(sota->map, sota->map->attacktolist, defendants, npc_ent, false);

    /* No attackable units tiles */
    if (DARR_NUM(defendants) < 1) {
        action->action = AI_ACTION_WAIT;
        DARR_FREE(defendants);
        return;
    }

    /* - TODO: Find easiest unit to kill - */
    action->patient = defendants[0];
    action->action  = AI_ACTION_ATTACK;
}

void _AI_Decider_Do_Move_To(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    //  - Prioritizes moving to target, if possible AfterMove
    //      - Much simpler to implement
    //      - AI is "Dumb" and doesn't go for obvious attacks on way to target
    /* --- PRELIMINARIES --- */
    struct Unit     *npc = TNECS_GET_COMPONENT(sota->world, npc_ent, Unit);
    struct Position *pos = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);
    struct AI       *ai  = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);
    SDL_assert(ai  != NULL);

    /* Skip: Unit is on target tile */
    struct Point target = ai->target_move;
    struct Point start  = pos->tilemap_pos;
    if ((target.x == start.x) && (target.y == start.y))
        return;

    /* -- Compute traversemap for pathfinding -- */
    Map_Traversemap_Movement_Compute(sota->map, sota->world, npc_ent);
    i32 *traversemap        = sota->map->traversemap;
    tnecs_entity *unitmap   = sota->map->unitmap;
    int move                = Unit_computeMove(npc);
    SDL_assert(traversemap != NULL);
    SDL_assert((target.x >= 0) && (target.x < sota->map->col_len));
    SDL_assert((target.y >= 0) && (target.y < sota->map->row_len));
    SDL_assert((start.x >= 0) && (start.x < sota->map->col_len));
    SDL_assert((start.y >= 0) && (start.y < sota->map->row_len));

    i16 row_len     = sota->map->col_len;
    i16 col_len     = sota->map->col_len;

    /* -- Pathfinding --  */
    int *path_list  = DARR_INIT(path_list, int, 16);
    // i32 *Pathfinding_Astar_plus(i32 *path_list, i32 *traversemap, i32* occupymap,
    //                         size_t row_len, size_t col_len, int move,
    //                         struct Point start, struct Point end, b32 forward)
    path_list       = Pathfinding_Astar_plus(path_list, traversemap, unitmap,
                                             row_len, col_len, move,
                                             start, target, true);
    int point_num   = DARR_NUM(path_list) / TWO_D;

    /* -- target_move is furthest point along path unit can move to -- */
    SDL_assert(move > 0);
    int minimum     = (point_num - 1) < move ? (point_num - 1) : move;
    if (minimum > 1) {
        action->target_move.x = path_list[minimum * TWO_D];
        action->target_move.y = path_list[minimum * TWO_D + 1];
    }

    DARR_FREE(path_list);

    /* AfterMove action according to slave priority */
    if (AI_Decider_AfterMove[ai->priority_slave] != NULL) {
        AI_Decider_AfterMove[ai->priority_slave](sota, npc_ent, action);
    } else {
        action->action = AI_ACTION_WAIT;
    }
}

entity AI_Decide_Next(struct Game *sota) {
    struct AI_State *ai_state = &sota->ai_state;
    // TODO: better function for next unit
    ai_state->npc_i = 0;
    return (ai_state->npcs[ai_state->npc_i]);
}

void AI_Decide_Action(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
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
    b32 same_x = (action->target_action.x == pos->tilemap_pos.x);
    b32 same_y = (action->target_action.y == pos->tilemap_pos.y);
    if (same_x && same_y)
        return;

    /* -- Skip movement if target adjacent to current position -- */
    b32 adjacent_x   = (action->target_action.x == pos->tilemap_pos.x - 1);
    adjacent_x      |= (action->target_action.x == pos->tilemap_pos.x + 1);
    b32 adjacent_y   = (action->target_action.y == pos->tilemap_pos.y - 1);
    adjacent_y      |= (action->target_action.y == pos->tilemap_pos.y + 1);
    if (adjacent_y && adjacent_y)
        return;

    /* -- If target is in movement range, go there -- */

    /* -- If target is in movement not in range, go to tile on the way -- */
    // Note: AI_Decide_Action should do the job of taking small actions
    // on the way to a higher objective

}

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
    if (null_x && null_y)
        return;

    struct Position *pos = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);

    // TODO: Movement Animation
    struct Point old = pos->tilemap_pos;
    struct Point new = action->target_move;
    Map_Unit_Move(sota->map, old.x, old.y, new.x, new.y);
}

AI_Doer AI_Act_action[AI_ACTION_NUM] = {
    /* ITEMS            */ NULL,
    /* TALK             */ NULL,
    /* STAFF            */ NULL,
    /* DANCE            */ NULL,
    /* RESCUE           */ NULL,
    /* SEIZE            */ NULL,
    /* ESCAPE           */ NULL,
    /* ATTACK           */ NULL,
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


void _AI_Doer_Wait(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    Game_Unit_Wait(sota, npc_ent);
    sota->reinf_timer   = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Timer);
    struct Timer *timer = TNECS_GET_COMPONENT(sota->world, sota->reinf_timer, Timer);
    *timer = Timer_default;

    // TNECS_ADD_COMPONENT(sota->world, npc_ent, UnitMoveAnimation);
    // struct UnitMoveAnimation *anim;
    // anim = TNECS_GET_COMPONENT(sota->world, npc_ent, UnitMoveAnimation);
    // SDL_assert(anim != NULL);
    // *anim = UnitMoveAnimation_default;
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
