
#include "map/conditions.h"
#include "unit/unit.h"
#include "map/map.h"
#include "events.h"
#include "nmath.h"
#include "jsonio.h"

struct Map_condition Map_condition_default = {
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
};

struct Map_condition Map_condition_main_char_loss = {
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .unit    =   UNIT_ID_ERWIN,
    .lose    =   true,
};

struct Map_condition Map_condition_rout_loss = {
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .all     =   true,
    .lose    =   true,
};

struct Map_condition Map_condition_debug_map_loss = {
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .unit    =   UNIT_ID_SILOU,
    .lose    =   true,
};

struct Map_condition Map_condition_boss_win = {
    .min     =   -1,
    .at      =   -1,
    .max     =   -1,
    .army    =   ARMY_ENEMY,
    .boss    =   true,
    .win     =   true,
};

void Map_Conditions_Check_Death(struct Map_condition *conditions_darr,
                                struct Map           *map,
                                struct Unit          *victim,
                                struct Boss          *boss,
                                struct Game          *sota) {
    int conds = DARR_NUM(conditions_darr);
    for (int i = 0; i < conds; i++) {
        struct Map_condition *condition = conditions_darr + i;

        if (!Map_Condition_Check_Death(condition, map, victim, boss))
            continue;

        /* Condition satisfied, doing it */
        DARR_DEL(conditions_darr, i);
        Map_Condition_Trigger(condition, sota);
    }
}

b32 Map_Condition_Check_Death(struct Map_condition *condition,
                              struct Map           *map,
                              struct Unit          *unit,
                              struct Boss          *boss) {
    // SDL_Log("Map_Condition: Check_Death");
    // SDL_Log("Unit id Silou:%d Died:%d condition:%d", UNIT_ID_SILOU, unit->_id, condition->unit);

    /* Checking for matching army */
    b32 match_army = true;
    if ((condition->army > ARMY_START) && (condition->army <= ARMY_NUM))
        match_army = (unit->army == condition->army);

    /* -- No match: army -- */
    // SDL_Log("armies: %d %d", condition->army, unit->army);
    if (!match_army) {
        // SDL_Log("No match: army");
        return (false);
    }

    /* Checking for matching unit ID */
    b32 match_unit = false;
    if ((condition->unit > UNIT_ID_NULL) && (condition->unit < UNIT_ID_NUM))
        match_unit = (unit->_id == condition->unit);

    /* -- No match: unit -- */
    if (match_unit) {
        // SDL_Log("Match: unit");
        return (true);
    }

    tnecs_entity *army_onfield = NULL;
    if (army_alignment[condition->army] == ALIGNMENT_FRIENDLY) {
        army_onfield = map->friendlies_onfield;
    } else {
        army_onfield = map->enemies_onfield;
    }
    SDL_assert(army_onfield != NULL);

    /* Match: Rout */
    if (!condition->boss && condition->all && (DARR_NUM(army_onfield) == 0)) {
        // SDL_Log("Match: Rout");
        return (true);
    }

    /* Checking for boss death */
    b32 died_boss = (boss != NULL);

    /* Match: Any boss */
    // SDL_Log("boss %d %d %d", condition->boss, condition->all, died_boss);
    if (condition->boss && !condition->all && died_boss) {
        // SDL_Log("Match: Any boss");
        return (true);
    }

    /* Match: All boss */
    if (condition->boss && condition->all && died_boss && !Map_Boss_Alive(map, ARMY_ENEMY)) {
        // SDL_Log("Match: All bosses");
        return (true);
    }

    /* -- No match -- */
    // SDL_Log("No match");
    return (false);
}

void Map_Condition_Trigger(struct Map_condition *condition, struct Game *sota) {
    /* XOR win and lose */
    // SDL_assert(!(condition->win && condition->lose));

    if (condition->win && !condition->lose) {
        /* Call Receive to not wait to SDL_PollEvent on next frame */
        receive_event_Map_Win(sota, NULL);
    } else if (condition->lose && !condition->win) {
        /* Call Receive to not wait to SDL_PollEvent on next frame */
        receive_event_Map_Lose(sota, NULL);
    } else if (!condition->lose && !condition->win) {
        /* Neither win nor loss. Triggers something to happen */
    } else {
        /* Win and Loss condition both */
        // Error?
    }
}

/* --- I/O --- */
void Map_Condition_writeJSON(void *input, cJSON *jmap_cond) {
    struct Map_condition *map_cond = (struct Map_condition *)input;

    cJSON *jarmy    = cJSON_CreateNumber(map_cond->army);
    cJSON *jboss    = cJSON_CreateBool(map_cond->boss);
    cJSON *jall     = cJSON_CreateBool(map_cond->all);
    cJSON *junit    = cJSON_CreateNumber(map_cond->unit);
    cJSON *jmin     = cJSON_CreateNumber(map_cond->min);
    cJSON *jmax     = cJSON_CreateNumber(map_cond->max);
    cJSON *jat      = cJSON_CreateNumber(map_cond->at);
    cJSON *jgold    = cJSON_CreateNumber(map_cond->gold);
    cJSON *jitem    = cJSON_CreateNumber(map_cond->item);
    cJSON *jscene   = cJSON_CreateNumber(map_cond->scene);
    cJSON *jwin     = cJSON_CreateBool(map_cond->win);
    cJSON *jlose    = cJSON_CreateBool(map_cond->lose);

    cJSON_AddItemToObject(jmap_cond, "army", jarmy);
    cJSON_AddItemToObject(jmap_cond, "boss", jboss);
    cJSON_AddItemToObject(jmap_cond, "all", jall);
    cJSON_AddItemToObject(jmap_cond, "unit", junit);
    cJSON_AddItemToObject(jmap_cond, "min", jmin);
    cJSON_AddItemToObject(jmap_cond, "max", jmax);
    cJSON_AddItemToObject(jmap_cond, "at", jat);
    cJSON_AddItemToObject(jmap_cond, "gold", jgold);
    cJSON_AddItemToObject(jmap_cond, "item", jitem);
    cJSON_AddItemToObject(jmap_cond, "scene", jscene);
    cJSON_AddItemToObject(jmap_cond, "win", jwin);
    cJSON_AddItemToObject(jmap_cond, "lose", jlose);
}

void Map_Condition_readJSON(void *input, cJSON *jmap_cond) {
    struct Map_condition *map_cond = (struct Map_condition *)input;

    cJSON *jarmy    = cJSON_GetObjectItem(jmap_cond, "army");
    cJSON *jboss    = cJSON_GetObjectItem(jmap_cond, "boss");
    cJSON *jall     = cJSON_GetObjectItem(jmap_cond, "all");
    cJSON *junit    = cJSON_GetObjectItem(jmap_cond, "unit");
    cJSON *jmin     = cJSON_GetObjectItem(jmap_cond, "min");
    cJSON *jmax     = cJSON_GetObjectItem(jmap_cond, "max");
    cJSON *jat      = cJSON_GetObjectItem(jmap_cond, "at");
    cJSON *jgold    = cJSON_GetObjectItem(jmap_cond, "gold");
    cJSON *jitem    = cJSON_GetObjectItem(jmap_cond, "item");
    cJSON *jscene   = cJSON_GetObjectItem(jmap_cond, "scene");
    cJSON *jwin     = cJSON_GetObjectItem(jmap_cond, "win");
    cJSON *jlose    = cJSON_GetObjectItem(jmap_cond, "lose");

    if (jarmy   != NULL)
        map_cond->army  = cJSON_GetNumberValue(jarmy);
    if (jboss   != NULL)
        map_cond->boss  = cJSON_IsTrue(jboss);
    if (jall    != NULL)
        map_cond->all   = cJSON_IsTrue(jall);
    if (junit   != NULL)
        map_cond->unit  = cJSON_GetNumberValue(junit);
    if (jmin    != NULL)
        map_cond->min   = cJSON_GetNumberValue(jmin);
    if (jmax    != NULL)
        map_cond->max   = cJSON_GetNumberValue(jmax);
    if (jat     != NULL)
        map_cond->at    = cJSON_GetNumberValue(jat);
    if (jgold   != NULL)
        map_cond->gold  = cJSON_GetNumberValue(jgold);
    if (jitem   != NULL)
        map_cond->item  = cJSON_GetNumberValue(jitem);
    if (jscene  != NULL)
        map_cond->scene = cJSON_GetNumberValue(jscene);
    if (jwin    != NULL)
        map_cond->win   = cJSON_IsTrue(jwin);
    if (jlose   != NULL)
        map_cond->lose  = cJSON_IsTrue(jlose);
}
