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

/* --- Destructors --- */
void Convoy_Free(   Convoy *conv);
void Convoy_Clear(  Convoy *conv);

/* --- Items --- */
// Sorting every item input/output
void           Convoy_Deposit(  Convoy *c,
                                Inventory_item i);
Inventory_item Convoy_Withdraw( Convoy *c, i32 i);

/* -- Wagon -- */
// Add item to wagon
// Remove item from wagon
// Move item from wagon to wagon
// Question: GUI for that?
// - How to easily switch items in wagons?
//  1. Wagon view, Items view
//      - "Items view" is basically GBA FE convoy. All wagons
//      - "Wagon view" is only one wagon
//          - Can't exchange items between wagons 
//  

/* --- Utils --- */
i32 _Convoy_Index2Type( Convoy *c, i32 i);
i32 _Convoy_Index2Order(Convoy *c, i32 i);

/* --- Money --- */
i32 Convoy_Earn(    Convoy *c, i32 in);
i32 Convoy_Spend(   Convoy *c, i32 out);

b32 Convoy_isFull(  Convoy *c);

/* --- I/O --- */
void Convoy_readJSON(           void    *input,  
                        const   cJSON   *json);
void Convoy_writeJSON(  const   void    *input,
                                cJSON   *json);

/* --- Debug --- */
void Convoy_Stats_Print(Convoy      *c,
                        i32          wpnType,
                        i32          stattype);
void Convoy_AllStats_Print( Convoy  *c,
                            i32      wpnType);

#endif /* CONVOY_H */