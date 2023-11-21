
#include "AI.h"

struct AI AI_default = {
    .priority_master  = AI_PRIORITY_START,
    .priority_slave   = AI_PRIORITY_START,
    .move             = AI_MOVE_START,
};

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
    /* AI_PRIORITY_DO_NOTHING   */ &AI_Decider_Do_Nothing,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
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
    /* AI_PRIORITY_DO_NOTHING   */ &AI_Decider_Do_Nothing,
    /* AI_PRIORITY_MOVE_TO      */ NULL,
    /* AI_PRIORITY_PATROL       */ NULL,
};

void AI_Decider_Do_Nothing(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    struct Position *pos    = TNECS_GET_COMPONENT(sota->world, npc_ent, Position);
    action->target_action.x = pos->tilemap_pos.x;
    action->target_action.y = pos->tilemap_pos.y;
    action->action          = AI_ACTION_WAIT;
}

void AI_Decide_Action(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    *action = AI_Action_default;
    struct Unit *npc    = TNECS_GET_COMPONENT(sota->world, npc_ent, Unit);
    struct AI   *ai     = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);
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
}

void AI_Move(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- Skip no movement -- */
    struct AI   *ai     = TNECS_GET_COMPONENT(sota->world, npc_ent, AI);

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
    /* ITEMS          */ NULL,
    /* TALK           */ NULL,
    /* STAFF          */ NULL,
    /* DANCE          */ NULL,
    /* RESCUE         */ NULL,
    /* SEIZE          */ NULL,
    /* ESCAPE         */ NULL,
    /* ATTACK         */ NULL,
    /* VILLAGE        */ NULL,
    /* TRADE          */ NULL,
    /* MAP            */ NULL,
    /* WAIT           */ &AI_Doer_Wait,
    /* OPEN           */ NULL,
    /* QUIT           */ NULL,
    /* END_TURN       */ NULL,
    /* UNITS          */ NULL,
    /* CONVOY         */ NULL,
    /* GLOBAL_RANGE   */ NULL,
    /* NEW_GAME       */ NULL,
    /* LOAD           */ NULL,
    /* ERASE          */ NULL,
    /* COPY           */ NULL,
    /* OPTIONS        */ NULL,
    /* EXTRAS         */ NULL,
    /* DEBUG_MAP      */ NULL,
};


void AI_Doer_Wait(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    Game_Unit_Wait(sota, npc_ent);
    TNECS_ADD_COMPONENT(sota->world, npc_ent, Timer);
    struct Timer *timer = TNECS_GET_COMPONENT(sota->world, npc_ent, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    TNECS_ADD_COMPONENT(sota->world, npc_ent, UnitMoveAnimation);
    struct UnitMoveAnimation *anim;
    anim = TNECS_GET_COMPONENT(sota->world, npc_ent, UnitMoveAnimation);
    SDL_assert(anim != NULL);
    *anim = UnitMoveAnimation_default;
}

void AI_Act(struct Game *sota, tnecs_entity npc_ent, struct AI_Action *action) {
    /* -- Skip if no action -- */
    if (AI_Act_action[action->action] != NULL)
        AI_Act_action[action->action](sota, npc_ent, action);
}

// extern int_fast8_t AI_Forecast_Rating(struct Combat_Forecast in_forecast) {
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     rating_buffer = Equation_AI_Rating_hitRate(in_forecast.stats.agg_rates.hit);
//     rating = rating > (INT8_MAX - AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer) ? INT8_MAX :
//              rating +
//              AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer;
//     rating_buffer = Equation_AI_Rating_hitRate(in_forecast.stats.dft_rates.hit);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_critRate(in_forecast.stats.agg_rates.crit);
//     rating = rating > (INT8_MAX - AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer) ? INT8_MAX :
//              rating +
//              AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer;
//     rating_buffer = Equation_AI_Rating_critRate(in_forecast.stats.dft_rates.crit);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_Damage(
//                         in_forecast.stats.dft_damage.dmg[DAMAGE_TYPE_MAGICAL]);
//     rating = rating < (INT8_MAX + rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Damage(
//                         in_forecast.stats.agg_damage.dmg[DAMAGE_TYPE_MAGICAL]);
//     rating = rating < (INT8_MAX + AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer) ? INT8_MAX :
//              rating +
//              AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer;
//     rating_buffer = Equation_AI_Rating_Damage(
//                         in_forecast.stats.dft_damage.dmg[DAMAGE_TYPE_PHYSICAL]);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_Damage(
//                         in_forecast.stats.agg_damage.dmg[DAMAGE_TYPE_PHYSICAL]);
//     rating = rating < (INT8_MIN + AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer) ? INT8_MIN :
//              rating -
//              AI_RATING_AGGRESSOR_MULTIPLIER * rating_buffer;
//     rating_buffer =
//         in_forecast.death.aggressor_possible * AI_RATING_AGGRESSOR_DEATH_POSSIBLE +
//         in_forecast.death.aggressor_certain * AI_RATING_AGGRESSOR_DEATH_CERTAIN;
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating +
//              AI_RATING_AGGRESSOR_MULTIPLIER *
//              rating_buffer;
//     rating_buffer =
//         in_forecast.death.defendant_certain * AI_RATING_DEFENDANT_DEATH_CERTAIN +
//         in_forecast.death.defendant_possible * AI_RATING_DEFENDANT_DEATH_POSSIBLE;
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer =
//         in_forecast.flow.aggressor_doubles * AI_RATING_aggressor_doubles +
//         in_forecast.flow.aggressor_brave * AI_RATING_aggressor_brave;
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating +
//              AI_RATING_AGGRESSOR_MULTIPLIER *
//              rating_buffer;
//     rating_buffer =
//         in_forecast.flow.defendant_doubles * AI_RATING_defendant_doubles +
//         in_forecast.flow.defendant_retaliates * AI_RATING_defendant_retaliates +
//         in_forecast.flow.defendant_brave * AI_RATING_defendant_brave;
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     return (rating);
// }

// extern tnecs_entity AI_Target_Heal(tnecs_world * in_world,
//                                      tnecs_entity in_staffWielder,
//                                      tnecs_entity * in_possiblePatients, u8 num_patients) {
//     SDL_Log("AI_Target_Heal");
//     struct AI * temp_ai_ptr;
//     tnecs_entity out_target = 0;
//     temp_ai_ptr = TNECS_GET_COMPONENT(in_world, in_staffWielder, AI);
//     if (num_patients > 0) {
//         out_target = linalg_isIn_int32_t(in_possiblePatients, temp_ai_ptr->target_protect,
//                                          num_patients);
//         if (out_target == 0) {
//             float missing_hp_prop = 0.0f;
//             float missing_hp_prop_max = 0.0f;
//             struct Unit * temp_unit_ptr;
//             for (u8 i = 0; i < num_patients; i++) {
//                 temp_unit_ptr = TNECS_GET_COMPONENT(in_world, in_possiblePatients[i], Unit);
//                 missing_hp_prop = 1.0f - ((float)temp_unit_ptr->current_hp / (float)
//                                           temp_unit_ptr->effective_stats.hp);
//                 if (missing_hp_prop > missing_hp_prop_max) {
//                     missing_hp_prop_max = missing_hp_prop;
//                     out_target = in_possiblePatients[i];
//                 }
//             }
//         }
//     }
//     return (out_target);
// }

// int_fast8_t AI_Silence_Rating(tnecs_world * in_world, u8 in_hit_rate,
//                               tnecs_entity in_enemy_ent) {
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     struct Unit * enemy_ptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Unit);
//     rating_buffer = Equation_AI_Rating_hitRate(in_hit_rate);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.mag);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.luck);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.res);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     for (u8 i = 0; i < DEFAULT_EQUIPMENT_SIZE; i++) {
//         switch (enemy_ptr->equipment[i].id) {
//             case ITEM_ID_PULL:
//                 rating_buffer = AI_RATING_STAFF_PULL;
//                 rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//                 break;
//         }
//     }
//     return (rating);
// }

// tnecs_entity AI_Target_Silence(tnecs_world * in_world, tnecs_entity in_staffWielder,
//                                  u8 * in_hit_rates, tnecs_entity * in_possibleDefenders,
//                                  u8 num_defender) {
//     SDL_Log("AI_Target_Silence");
//     struct AI * temp_ai_ptr;
//     //  struct Unit * temp_unit_ptr;
//     temp_ai_ptr = TNECS_GET_COMPONENT(in_world, in_staffWielder, AI);
//     tnecs_entity out_target = 0;
//     if (num_defender > 0) {
//         out_target = linalg_isIn_int32_t(temp_ai_ptr->target_kill, in_possibleDefenders,
//                                          num_defender);
//         if (out_target == 0) {
//             int_fast8_t temp_rating;
//             int_fast8_t max_rating = INT8_MIN;
//             out_target = in_possibleDefenders[(rand() % num_defender)];
//             for (u8 i = 0; i < num_defender; i++) {
//                 // temp_unit_ptr = TNECS_GET_COMPONENT(in_world, in_possibleDefenders[i], Unit);
//                 temp_rating = AI_Silence_Rating(in_world, in_hit_rates[i], in_possibleDefenders[i]);
//                 if (temp_rating > max_rating) {
//                     out_target = in_possibleDefenders[i];
//                     max_rating = temp_rating;
//                 }
//             }
//         }
//     } else
//         SDL_Log("no defenders");
//     return (out_target);
// }

// tnecs_entity AI_Target_Pull(tnecs_world * in_world, tnecs_entity in_staffWielder,
//                               tnecs_entity * in_friendlies, u8 num_friendly, tnecs_entity * in_enemies,
//                               u8 num_enemy) {
//     SDL_Log("AI_Target_Pull");
//     struct AI * temp_ai_ptr;
//     struct Unit * temp_unit_ptr;
//     temp_ai_ptr = TNECS_GET_COMPONENT(in_world, in_staffWielder, AI);
//     tnecs_entity out_target = 0;
//     float missing_hp_prop = 0.0f;
//     float missing_hp_prop_max = 0.0f;
//     switch (temp_ai_ptr->priority) {
//         case AI_PRIORITY_PROTECT:
//             if (num_friendly > 0) {
//                 out_target = linalg_isIn_int32_t(in_friendlies, temp_ai_ptr->target_protect,
//                                                  num_friendly);
//                 if (out_target > 0) {
//                     temp_unit_ptr = TNECS_GET_COMPONENT(in_world, out_target, Unit);
//                     missing_hp_prop = 1.0f - ((float)temp_unit_ptr->current_hp / (float)
//                                               temp_unit_ptr->effective_stats.hp);
//                     if (missing_hp_prop < AI_PULL_FRIENDLY_MISSINGHP)
//                         out_target = 0;
//                 }
//             }
//             break;
//         case AI_PRIORITY_KILL:
//             if (num_enemy > 0)
//                 out_target = linalg_isIn_int32_t(temp_ai_ptr->target_kill, in_enemies, num_enemy);
//             break;
//     }
//     if (out_target == 0) {
//         switch (temp_ai_ptr->priority) {
//             case AI_PRIORITY_PROTECT:
//                 // Pull most wounded ally.
//                 // Pull ally that just attacked. -> AI makes staff pullers go last
//                 for (u8 i = 0; i < num_friendly; i++) {
//                     temp_unit_ptr = TNECS_GET_COMPONENT(in_world, in_friendlies[i], Unit);
//                     missing_hp_prop = 1.0f - ((float)temp_unit_ptr->current_hp / (float)
//                                               temp_unit_ptr->effective_stats.hp);
//                     if (missing_hp_prop > missing_hp_prop_max) {
//                         out_target = in_friendlies[i];
//                         missing_hp_prop_max = missing_hp_prop;
//                     }
//                 }
//                 break;
//             case AI_PRIORITY_KILL:
//                 // Pull wounded enemy.
//                 // Pull weak enemy.
//                 break;
//         }
//     }
//     return (out_target);
// }

// tnecs_entity AI_Target_Push(tnecs_world * in_world, tnecs_entity in_staffWielder,
//                               tnecs_entity * in_friendlies, u8 num_friendly, tnecs_entity * in_enemies,
//                               u8 num_enemy) {
//     SDL_Log("AI_Target_Push");
//     struct AI * temp_ai_ptr;
//     struct Unit * temp_unit_ptr;
//     temp_ai_ptr = TNECS_GET_COMPONENT(in_world, in_staffWielder, AI);
//     tnecs_entity out_target = 0;
//     return (out_target);
// }

// tnecs_entity AI_Target_Attack(tnecs_world * in_world, tnecs_entity in_attacker,
//                                 tnecs_entity * in_possibleDefenders, struct Combat_Forecast * in_forecasts,
//                                 u8 num_defender) {
//     struct AI * temp_ai_ptr;
//     temp_ai_ptr = TNECS_GET_COMPONENT(in_world, in_attacker, AI);
//     tnecs_entity out_target = 0;
//     if (num_defender > 0) {
//         out_target = linalg_isIn_int32_t(in_possibleDefenders, temp_ai_ptr->target_kill,
//                                          num_defender);
//         if (out_target == 0) {
//             int_fast8_t temp_rating;
//             int_fast8_t max_rating = INT8_MIN;
//             out_target = in_possibleDefenders[(rand() % num_defender)];
//             for (u8 i = 0; i < num_defender; i++) {
//                 temp_rating = AI_Forecast_Rating(in_forecasts[i]);
//                 if (temp_rating > max_rating) {
//                     out_target = in_possibleDefenders[i];
//                     max_rating = temp_rating;
//                 }
//             }
//         }
//     } else
//         SDL_Log("no defenders");
//     return (out_target);
// }

// struct AI_PushPull_Out AI_PushPull_Friendly_Offensively_Rating(tnecs_world * in_world,
//         struct Map * in_map, tnecs_entity in_friendly_ent) {
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     struct Unit * friendly_ptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Unit);
//     struct Position * position_ptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Position);
//     struct Unit * friendly_mptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Unit);
//     float current_hp_prop = ((float)friendly_ptr->current_hp / (float)
//                              friendly_ptr->effective_stats.hp);
//     rating_buffer = Equation_AI_Rating_HPprop(current_hp_prop);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.res);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.def);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.str);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.hp);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.mag);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.agi);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.dex);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     // UNTIL MAP IS NOT TNECSED
//     // i32 * costmapp_pushpull = Map_Costmap_PushPull_Compute(in_map, in_world, in_friendly_ent);
//     // i32 * costmapp_move = Map_Costmap_Movement_Compute(in_map, in_world, in_friendly_ent);
//     // i32 * movemapp = Map_Pathfinding_Moveto(costmapp_move, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, Unit_computeMove(friendly_mptr), POINTS_MATRIX);
//     // struct Position * temp_pos;
//     // struct Point * enemy_points = NULL;
//     // for (u8 i = 0; i < in_DARR_NUM(map->friendlies_onfield); i++) {
//     //     if (in_map->friendlies_onfield[i] > 0) {
//     //         temp_pos = TNECS_GET_COMPONENT(in_world, in_map->enemies_onfield[i], Position);
//     //         if (temp_pos != NULL) {
//     //             arrput(enemy_points, temp_pos->tilemap_pos);
//     //         }
//     //     }
//     // }
//     // i32 * unit_range = Unit_Range_Combine_Equipment(friendly_mptr);
//     // i32 * gradientmapp_enemy = pathfinding_Map_unitGradient_int32_t(costmapp_pushpull, in_map->row_len, in_map->col_len, enemy_points, in_DARR_NUM(map->friendlies_onfield));
//     // i32 * attacktomapp = pathfinding_Map_Attackto_int32_t(movemapp, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, unit_range, POINTS_MATRIX);
//     // i32 * gradientmapp_enemy_masked = linalg_mask_i16(gradientmapp_enemy, attacktomapp, in_map->row_len, in_map->col_len);
//     // struct AI_PushPull_Out ai_pushpull_out = AI_PushPull_bestPosition(gradientmapp_enemy_masked, in_map->row_len, in_map->col_len, unit_range[1], position_ptr->tilemap_pos, 1);
//     // ai_pushpull_out.rating = ai_pushpull_out.rating > (INT8_MAX - rating) ? INT8_MAX : ai_pushpull_out.rating + rating;
//     // return (ai_pushpull_out);
// }

// struct AI_PushPull_Out AI_PushPull_Friendly_Defensively_Rating(tnecs_world * in_world,
//         struct Map * in_map, tnecs_entity in_friendly_ent) {
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     struct Unit * friendly_ptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Unit);
//     struct Position * position_ptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Position);
//     struct Unit * friendly_mptr = TNECS_GET_COMPONENT(in_world, in_friendly_ent, Unit);
//     float missing_hp_prop = 1.0f - ((float)friendly_ptr->current_hp / (float)
//                                     friendly_ptr->effective_stats.hp);
//     rating_buffer = Equation_AI_Rating_HPprop(missing_hp_prop);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.res);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(friendly_ptr->effective_stats.def);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     // i32 * costmapp_pushpull = Map_Costmap_PushPull_Compute(in_map, in_world, in_friendly_ent);
//     // i32 * costmapp_move = Map_Costmap_Movement_Compute(in_map, in_world, in_friendly_ent);
//     // i32 * movemapp = Map_Pathfinding_Moveto(costmapp_move, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, Unit_computeMove(friendly_mptr), POINTS_MATRIX);
//     // struct Position * temp_pos;
//     // struct Point * friendly_points = NULL;
//     // for (u8 i = 0; i < in_DARR_NUM(map->friendlies_onfield); i++) {
//     //     if (in_map->friendlies_onfield[i] > 0) {
//     //         temp_pos = TNECS_GET_COMPONENT(in_world, in_map->friendlies_onfield[i], Position);
//     //         if (temp_pos != NULL) {
//     //             arrput(friendly_points, temp_pos->tilemap_pos);
//     //         }
//     //     }
//     // }
//     // i32 * unit_range = Unit_Range_Combine_Equipment(friendly_mptr);
//     // i32 * gradientmapp_friendly = pathfinding_Map_unitGradient_int32_t(costmapp_pushpull, in_map->row_len, in_map->col_len, friendly_points, in_DARR_NUM(map->friendlies_onfield));
//     // i32 * attacktomapp = pathfinding_Map_Attackto_int32_t(movemapp, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, unit_range, POINTS_MATRIX);
//     // i32 * gradientmapp_friendly_masked = linalg_mask_i16(gradientmapp_friendly, attacktomapp, in_map->row_len, in_map->col_len);
//     // struct AI_PushPull_Out ai_pushpull_out = AI_PushPull_bestPosition(gradientmapp_friendly_masked, in_map->row_len, in_map->col_len, unit_range[1], position_ptr->tilemap_pos, -1);
//     // ai_pushpull_out.rating = ai_pushpull_out.rating > (INT8_MAX - rating) ? INT8_MAX : ai_pushpull_out.rating + rating;
//     // return (ai_pushpull_out);
// }

// struct AI_PushPull_Out AI_PushPull_Enemy_Defensively_Rating(tnecs_world * in_world,
//         struct Map * in_map, tnecs_entity in_enemy_ent) {
//     SDL_Log("AI_PushPull_Enemy_Defensively_Rating");
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     struct Unit * enemy_ptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Unit);
//     struct Position * position_ptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Position);
//     struct Unit * enemy_mptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Unit);
//     float current_hp_prop = 1.0f - ((float)enemy_ptr->current_hp / (float)
//                                     enemy_ptr->effective_stats.hp);
//     rating_buffer = Equation_AI_Rating_HPprop(current_hp_prop);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.str);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.mag);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     // i32 * costmapp_pushpull = Map_Costmap_PushPull_Compute(in_map, in_world, in_enemy_ent);
//     // i32 * costmapp_move = Map_Costmap_Movement_Compute(in_map, in_world, in_enemy_ent);
//     // i32 * movemapp = Map_Pathfinding_Moveto(costmapp_move, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, Unit_computeMove(enemy_mptr), POINTS_MATRIX);
//     // struct Position * temp_pos;
//     // struct Point * enemy_points = NULL;
//     // for (u8 i = 0; i < in_DARR_NUM(map->friendlies_onfield); i++) {
//     //     if (in_map->friendlies_onfield[i] > 0) {
//     //         temp_pos = TNECS_GET_COMPONENT(in_world, in_map->enemies_onfield[i], Position);
//     //         if (temp_pos != NULL) {
//     //             arrput(enemy_points, temp_pos->tilemap_pos);
//     //         }
//     //     }
//     // }
//     // i32 * unit_range = Unit_Range_Combine_Equipment(enemy_mptr);
//     // i32 * gradientmapp_enemy = pathfinding_Map_unitGradient_int32_t(costmapp_pushpull, in_map->row_len, in_map->col_len, enemy_points, in_DARR_NUM(map->friendlies_onfield));
//     // i32 * attacktomapp = pathfinding_Map_Attackto_int32_t(movemapp, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, unit_range, POINTS_MATRIX);
//     // i32 * gradientmapp_enemy_masked = linalg_mask_i16(gradientmapp_enemy, attacktomapp, in_map->row_len, in_map->col_len);
//     // struct AI_PushPull_Out ai_pushpull_out = AI_PushPull_bestPosition(gradientmapp_enemy_masked, in_map->row_len, in_map->col_len, unit_range[1], position_ptr->tilemap_pos, 1);
//     // ai_pushpull_out.rating = ai_pushpull_out.rating > (INT8_MAX - rating) ? INT8_MAX : ai_pushpull_out.rating + rating;
//     // return (ai_pushpull_out);
// }

// struct AI_PushPull_Out AI_PushPull_Enemy_Offensively_Rating(tnecs_world * in_world,
//         struct Map * in_map, tnecs_entity in_enemy_ent) {
//     int_fast8_t rating = 0;
//     int_fast8_t rating_buffer;
//     struct Unit * enemy_ptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Unit);
//     struct Position * position_ptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Position);
//     struct Unit * enemy_mptr = TNECS_GET_COMPONENT(in_world, in_enemy_ent, Unit);
//     float current_hp_prop = ((float)enemy_ptr->current_hp / (float)
//                              enemy_ptr->effective_stats.hp);
//     rating_buffer = Equation_AI_Rating_HPprop(current_hp_prop);
//     rating = rating < (INT8_MIN + rating_buffer) ? INT8_MIN : rating - rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.res);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     rating_buffer = Equation_AI_Rating_Stats(enemy_ptr->effective_stats.def);
//     rating = rating > (INT8_MAX - rating_buffer) ? INT8_MAX : rating + rating_buffer;
//     // i32 * costmapp_pushpull = Map_Costmap_PushPull_Compute(in_map, in_world, in_enemy_ent);
//     // i32 * costmapp_move = Map_Costmap_Movement_Compute(in_map, in_world, in_enemy_ent);
//     // i32 * movemapp = Map_Pathfinding_Moveto(costmapp_move, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, Unit_computeMove(enemy_mptr), POINTS_MATRIX);
//     // struct Position * temp_pos;
//     // struct Point * friendly_points = NULL;
//     // for (u8 i = 0; i < in_DARR_NUM(map->friendlies_onfield); i++) {
//     //     if (in_map->friendlies_onfield[i] > 0) {
//     //         temp_pos = TNECS_GET_COMPONENT(in_world, in_map->friendlies_onfield[i], Position);
//     //         if (temp_pos != NULL) {
//     //             arrput(friendly_points, temp_pos->tilemap_pos);
//     //         }
//     //     }
//     // }
//     // i32 * unit_range = Unit_Range_Combine_Equipment(enemy_mptr);
//     // i32 * gradientmapp_friendly = pathfinding_Map_unitGradient_int32_t(costmapp_pushpull, in_map->row_len, in_map->col_len, friendly_points, in_DARR_NUM(map->friendlies_onfield));
//     // i32 * attacktomapp = pathfinding_Map_Attackto_int32_t(movemapp, in_map->row_len, in_map->col_len, position_ptr->tilemap_pos, unit_range, POINTS_MATRIX);
//     // i32 * gradientmapp_friendly_masked = linalg_mask_i16(gradientmapp_friendly, attacktomapp, in_map->row_len, in_map->col_len);
//     // struct AI_PushPull_Out ai_pushpull_out = AI_PushPull_bestPosition(gradientmapp_friendly_masked, in_map->row_len, in_map->col_len, unit_range[1], position_ptr->tilemap_pos, -1);
//     // ai_pushpull_out.rating = ai_pushpull_out.rating > (INT8_MAX - rating) ? INT8_MAX : ai_pushpull_out.rating + rating;
//     // return (ai_pushpull_out);
// }

// struct AI_PushPull_Out AI_PushPull_bestPosition(i32 * in_gradientmap,
//         size_t row_len,
//         size_t col_len, i32 pushpull_distance, struct Point victim_pos,
//         int_fast8_t sign) {
//     struct AI_PushPull_Out out = {{-1 - 1}, INT8_MIN};
//     struct Point pushpullable;
//     out.rating = INT8_MIN;
//     int_fast8_t temp_rating = 0;
//     for (i32 sq_neighbour = 0; sq_neighbour < SQUARE_NEIGHBOURS; sq_neighbour++) {
//         for (i32 distance = 1; distance < pushpull_distance; distance++) {
//             pushpullable.x = int_inbounds((distance * q_cycle4_pzmz(
//                     sq_neighbour)) + victim_pos.x, 0,
//                                                     col_len - 1);
//             pushpullable.y = int_inbounds((distance * q_cycle4_zmzp(
//                     sq_neighbour)) + victim_pos.y, 0,
//                                                     row_len - 1);
//             if ((pushpullable.x == victim_pos.x) && (pushpullable.y == victim_pos.y))
//                 break;
//             else if (in_gradientmap[pushpullable.y * col_len + pushpullable.x] <
//                      NMATH_GRADIENTMAP_MINDIST)
//                 break;
//             else {
//                 if (sign * Equation_AI_Rating_pushGradient(in_gradientmap[pushpullable.y * col_len +
//                                        pushpullable.x]) > out.rating) {
//                     out.rating = sign * Equation_AI_Rating_pushGradient(in_gradientmap[pushpullable.y *
//                                                 col_len +
//                                                 pushpullable.x]);
//                     out.pos = pushpullable;
//                 }
//             }
//         }
//     }
//     return (out);
// }

void Unit_Move_onMap_Animate(struct Game *sota, tnecs_entity entity,
                             struct Timer *timer, struct UnitMoveAnimation *anim) {
    /* - Animation is complete, begin a turn - */
    if (timer->time_ns >= anim->time_ns) {
        SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Unit Animation Finished");
        TNECS_REMOVE_COMPONENTS(sota->world, entity, UnitMoveAnimation);
        TNECS_REMOVE_COMPONENTS(sota->world, entity, Timer);
        return;
    }

}
