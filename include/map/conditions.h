#ifndef MAP_CONDITIONS_H
#define MAP_CONDITIONS_H

/* --- STRUCTS --- */
struct Map_condition {
    i8 condition_type;
    /* --- CRITERIA --- */
    /* -- WHO -- */
    i8     lvl;        /* -1 for anyone    */
    i8     sex;        /* -1 for anyone    */
    i16    army;       /* -1 for anyone    */
    i16    unit;       /* -1 for anyone    */
    i8     unit_class; /* -1 for anyone    */
    /* -- WHERE -- */
    i16    col_min;    /* -1 for anywhere  */
    i16    col_max;    /* -1 for anywhere  */
    i16    row_min;    /* -1 for anywhere  */
    i16    row_max;    /* -1 for anywhere  */
    /* -- WHAT -- */
    i16    equippedL;  /* -1 for anything  */
    i16    equippedR;  /* -1 for anything  */
    /* -- WHEN --  */
    i16    min;        /* -1 for anytime   */
    i16    max;        /* -1 for anytime   */

    /* --- GENERAL PURPOSE COUNTER --- */
    i16 counter;

    /* --- REWARDS --- */
    i16    gold;
    i16    item;
    i16    scene;
    i16    event;
    i8     substate;
};


#endif /* MAP_H */
