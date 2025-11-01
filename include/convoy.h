/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Convoy to store InvItem (all items)
**
*/

#ifndef CONVOY_H
#define CONVOY_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"

typedef struct Convoy {
    jsonIO_Header jsonio_header;

    /*  One 2D array with ALL items
    **      1. Each row is for a weapon type
    **      2. In each row:
    **          1. Sort by ID   (small first)
    **          2. Sort by used (big first)     */
    InvItem  items[ITEM_TYPE_NUM]
    [CONVOY_SIZE_MAX];

    i32             num_items[ITEM_TYPE_NUM];

    /* Increases maximum convoy size,
    **  up to CONVOY_SIZE_MAX */
    i32             num_wagons;

    /* No negative money/debt */
    i32             bank; /* [sesterces] */
} Convoy;
extern const struct Convoy Convoy_default;

/* --- Constructors/Destructors --- */
void Convoy_Free(   Convoy *c);
void Convoy_Clear(  Convoy *c);

/* --- Items --- */
/* Sorting happens on deposit/withdraw */
i32            Convoy_Deposit(      Convoy *c,
                                    InvItem i);
InvItem Convoy_Withdraw(     Convoy *c, i32 i);
InvItem Convoy_Item(const    Convoy *c, i32 i);

/* --- Utils --- */
i32 _Convoy_Index2Type( i32 i);
i32 _Convoy_Index2Order(i32 i);

i32 _Convoy_Size(   const Convoy *c);
b32 Convoy_isFull(  const Convoy *c);

i32 _Convoy_Num_Items(  const Convoy *c);
i32 _Convoy_Num_Wagons( const Convoy *c);

/* --- Money --- */
i32 Convoy_Bank(    const   Convoy *c);
i32 Convoy_Earn(            Convoy *c, i32 in);
i32 Convoy_Spend(           Convoy *c, i32 out);

/* --- I/O --- */
void Convoy_readJSON(           void    *input,
                                const   cJSON   *json);
void Convoy_writeJSON(  const   void    *input,
                        cJSON   *json);

#endif /* CONVOY_H */