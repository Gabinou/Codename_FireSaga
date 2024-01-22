
#include "map/conditions.h"

struct Map_condition Map_condition_default = {
};
struct Map_condition Map_condition_main_char_loss = {
    army    =   -1,
    boss    =   false,
    all     =   false,
    unit    =   UNIT_ID_ERWIN,
    min     =   -1,
    at      =   -1,
    max     =   -1,
    gold    =    0,
    item    =   ITEM_NULL,
    scene   =    0,
    win     =   false,
    lose    =   true,
}

struct Map_condition Map_condition_main_char_loss = {
    army    =   UNIT_ARMY_ENEMY,
    boss    =   true,
    all     =   false,
    unit    =   false,
    min     =   -1,
    at      =   -1,
    max     =   -1,
    gold    =    0,
    item    =   ITEM_NULL,
    scene   =    0,
    win     =   true,
    lose    =   false,
}

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

void Map_Condition_Trigger(struct Map_condition *condition) {
    /* XOR win and lose */
    SDL_assert(!(condition->win && condition->lose));

    if (condition->win)
        Event_Emit(__func__, SDL_USEREVENT, event_Map_Win, NULL, NULL);

    if (condition->lose)
        Event_Emit(__func__, SDL_USEREVENT, event_Map_Lose, NULL, NULL);
}
