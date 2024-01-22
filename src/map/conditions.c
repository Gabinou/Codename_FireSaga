
#include "map/conditions.h"

struct Map_condition Map_condition_default = {
    // .unit       = -1,
    .army       = -1,
    // .row_min    = -1,
    // .row_max    = -1,
    // .col_min    = -1,
    // .col_max    = -1
};

b32 Map_Condition_Check_Death(struct Map_condition *condition,
                              struct Map           *map,
                              struct Unit          *unit,
                              struct Boss          *boss) {
    /* Checking for matching army */
    b32 match_army = true;
    if ((condition->army > ARMY_START) && (condition->army < ARMY_NUM))  
        match_army = (unit->army == condition->army); 

    /* -- No match: army -- */
    if (!match_army) 
        return(false);

    /* Checking for matching unit ID */
    b32 match_unit = true;
    if ((condition->unit > UNIT_ID_NULL) && (condition->unit < UNIT_ID_NUM))  
        match_unit = (unit->_id == condition->unit); 

    /* -- No match: unit -- */
    if (!match_unit) 
        return(false);

    /* Match: Rout */
    // TODO: check remaining units of enemy army
    if (!condition->boss && condition->all && (DARR_NUM(map->enemies_onfield) == 0)) 
        return(true);

    /* Checking for boss death */
    b32 died_boss = (boss != NULL);

    /* Match: Any boss */
    if (condition->boss && !condition->all && died_boss)
        return(true);

    /* Match: All boss */
    if (condition->boss && condition->all && died_boss && !Map_Boss_Alive(struct Map *map, i16 army))
        return(true);

    /* -- No match -- */
    return(false);
}
