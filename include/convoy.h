#ifndef CONVOY_H
#define CONVOY_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "cJSON.h"

/* --- FORWARD DECLARATINOS --- */
struct Game;
struct Unit;
struct Inventory_item;
struct cJSON;

/* --- NEW API --- */

/* --- Constructors/Destructors --- */
void Convoy_Free(   Convoy *c);
void Convoy_Clear(  Convoy *c);

/* --- Items --- */
/* Sorting happens on deposit/withdraw */
i32            Convoy_Deposit(  Convoy *c,
                                Inventory_item i);
Inventory_item Convoy_Withdraw( Convoy *c, i32 i);


/* --- Utils --- */
i32 _Convoy_Index2Type( i32 i);
i32 _Convoy_Index2Order(i32 i);

i32 _Convoy_Size(   const Convoy *c);
b32 Convoy_isFull(  const Convoy *c);

i32 _Convoy_Num_Items(  const Convoy *c);
i32 _Convoy_Num_Wagons( const Convoy *c);

/* --- Money --- */
i32 Convoy_Earn(    Convoy *c, i32 in);
i32 Convoy_Spend(   Convoy *c, i32 out);

/* --- I/O --- */
void Convoy_readJSON(           void    *input,
                                const   cJSON   *json);
void Convoy_writeJSON(  const   void    *input,
                        cJSON   *json);

/* --- Debug --- */
void Convoy_Stats_Print(const Convoy      *c,
                        i32          wpnType,
                        i32          stattype);
void Convoy_AllStats_Print( const Convoy  *c,
                            i32      wpnType);

#endif /* CONVOY_H */