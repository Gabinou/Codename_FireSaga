#ifndef MAP_CONDITIONS_H
#define MAP_CONDITIONS_H

#include "enums.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Boss;
struct Settings;
struct Unit;
struct Game;
struct cJSON;

/* Conditions */
// When condition is satisfied, something happens:
//  - Win map
//  - Lose map
//  - Get item
//  - Play scene

// Conditions need to be checked at different times
//  - Enemy dies
//  - Turn ends
//  - Unit Waits
//      - Check if condition is met
//  - Unit Seizes
//      -> Instant win. No need for condition struct
// -> For evey time to check, array of conditions.
//    check, if satisfied, trigger

// If condition is met call scene, then function
//   - Each condition needs to refer to scenes, action to take.
//      - Scene:    plays before coming back to gameplay, doing someting else
//      - Function: Recruit unit, Spawn unit, gives player an item, etc.
//

/* --- STRUCTS --- */
struct Map_condition {
    /* --- CRITERIA --- */
    /* -- WHO -- */
    // i8     lvl;          /* -1 for anyone    */
    // i8     sex;          /* -1 for anyone    */
    i32    army;            /* -1 for anyone    */
    b32    boss;            /* true for boss    */
    b32    all;             /* true for rout    */
    i32    unit;            /* -1 for anyone    */
    // i8     unit_class;   /* -1 for anyone    */
    /* -- WHERE -- */
    // i32    col_min;    /* -1 for anywhere  */
    // i32    col_max;    /* -1 for anywhere  */
    // i32    row_min;    /* -1 for anywhere  */
    // i32    row_max;    /* -1 for anywhere  */
    /* -- WHAT -- */
    // i32    equippedL;  /* -1 for anything  */
    // i32    equippedR;  /* -1 for anything  */
    /* -- WHEN --  */
    i32    min;        /* -1 for anytime   */
    i32     at;        /* -1 for never   */
    i32    max;        /* -1 for anytime   */

    /* --- REWARDS --- */
    i32    gold;
    i32    item;
    i32    scene;

    /* --- EFFECTS --- */
    b32    win;
    b32    lose;
};
extern const struct Map_condition Map_condition_default;
extern const struct Map_condition Map_condition_boss_win;
extern const struct Map_condition Map_condition_main_char_loss;
extern const struct Map_condition Map_condition_debug_map_loss;

void Map_Conditions_Check_Death(struct Map_condition *cdarr,
                                struct Map           *map,
                                struct Unit          *victim,
                                struct Boss          *boss,
                                struct Game          *game);
b32  Map_Condition_Check_Death( struct Map_condition *condition,
                                struct Map           *map,
                                struct Unit          *unit,
                                struct Boss          *boss);

void Map_Condition_Trigger(struct Map_condition *condition, struct Game *game);

/* --- I/O --- */
void Map_Condition_readJSON(void *input, cJSON *jmap_cond);
void Map_Condition_writeJSON(void *input, cJSON *jmap_cond);

#endif /* MAP_H */
