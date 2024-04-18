
#include "map/conditions.h"

struct Map_condition Map_condition_default = {0};

struct Map_condition Map_condition_main_char_loss = {
    .army    =   -1,
    .boss    =   false,
    .all     =   false,
    .unit    =   UNIT_ID_ERWIN,
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .gold    =    0,
    .item    =   ITEM_NULL,
    .scene   =    0,
    .win     =   false,
    .lose    =   true,
};

struct Map_condition Map_condition_debug_map_loss = {
    .army    =   -1,
    .boss    =   false,
    .all     =   false,
    .unit    =   UNIT_ID_SILOU,
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .gold    =    0,
    .item    =   ITEM_NULL,
    .scene   =    0,
    .win     =   false,
    .lose    =   true,
};

struct Map_condition Map_condition_boss_win = {
    .army    =   ARMY_ENEMY,
    .boss    =   true,
    .all     =   false,
    .unit    =   false,
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .gold    =    0,
    .item    =   ITEM_NULL,
    .scene   =    0,
    .win     =   true,
    .lose    =   false,
};

void Map_Conditions_Check_Death(struct Map_condition *conditions_darr,
                                struct Map           *map,
                                struct Unit          *victim,
                                struct Boss          *boss) {
    int conds = DARR_NUM(conditions_darr);
    for (int i = 0; i < conds; i++) {
        struct Map_condition *condition = conditions_darr + i;

        if (!Map_Condition_Check_Death(condition, map, victim, boss))
            continue;

        /* Condition satisfied, doing it */
        DARR_DEL(conditions_darr, i);
        Map_Condition_Trigger(condition);
    }
}

b32 Map_Condition_Check_Death(struct Map_condition *condition,
                              struct Map           *map,
                              struct Unit          *unit,
                              struct Boss          *boss) {
    SDL_Log("Map_Condition: Check_Death");
    SDL_Log("Unit id Silou:%d Died:%d condition:%d", UNIT_ID_SILOU, unit->_id, condition->unit);

    /* Checking for matching army */
    b32 match_army = true;
    if ((condition->army > ARMY_START) && (condition->army < ARMY_NUM))
        match_army = (unit->army == condition->army);

    /* -- No match: army -- */
    if (!match_army) {
        SDL_Log("No match: army");
        return (false);
    }

    /* Checking for matching unit ID */
    b32 match_unit = false;
    if ((condition->unit > UNIT_ID_NULL) && (condition->unit < UNIT_ID_NUM))
        match_unit = (unit->_id == condition->unit);

    /* -- No match: unit -- */
    if (match_unit) {
        SDL_Log("Match: unit");
        return (true);
    }

    /* Match: Rout */
    if (!condition->boss && condition->all && (DARR_NUM(map->enemies_onfield) == 0)) {
        SDL_Log("Match: Rout");
        return (true);
    }

    /* Checking for boss death */
    b32 died_boss = (boss != NULL);

    /* Match: Any boss */
    if (condition->boss && !condition->all && died_boss) {
        SDL_Log("Match: Any boss");
        return (true);
    }

    /* Match: All boss */
    if (condition->boss && condition->all && died_boss && !Map_Boss_Alive(map, ARMY_ENEMY)) {
        SDL_Log("Match: All bosses");
        return (true);
    }

    /* -- No match -- */
    SDL_Log("No match");
    return (false);
}

void Map_Condition_Trigger(struct Map_condition *condition) {
    /* XOR win and lose */
    // SDL_assert(!(condition->win && condition->lose));

    if (condition->win && !condition->lose) {
        Event_Emit(__func__, SDL_USEREVENT, event_Map_Win, NULL, NULL);
    } else if (condition->lose && !condition->win) {
        Event_Emit(__func__, SDL_USEREVENT, event_Map_Lose, NULL, NULL);
    } else if (!condition->lose && !condition->win) {
        /* Neither win nor loss. Triggers something to happen */ 
    } else {
        /* Win and Loss condition both */
        // Error? 
    }

}

/* --- I/O --- */
void Map_Condition_saveJSON(void *input, cJSON *jmap_cond) {

}

void Map_Condition_loadJSON(void *input, cJSON *jmap_cond) {

}
