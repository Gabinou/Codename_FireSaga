#ifndef UNIT_PARTY_H
#define UNIT_PARTY_H

#include "types.h"
#include "nmath.h"
#include "tnecs.h"
#include "nstr.h"
#include "cJSON.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Game;
struct Party;
struct cJSON;

/* --- Constructor/Destructors --- */
void Party_Init( struct Party *party);
void Party_Free( struct Party *party);

void Party_Members_Alloc( struct Party *party);

/* Remove all filenames in party json_filenames */
void Party_Reset(struct Party *party);

/* --- Utilities --- */
void Party_Folder(struct Party *ps, char *folder);

void Party_Ids2Filenames(   struct Party *ps);
void Party_Names2Filenames( struct Party *ps);

/* Put all unit ids with valid entities on a stack */
void Party_ID_Stack(struct Party *party);

i32  Party_Size(struct Party *ps);
i32 _Party_Size(struct Unit *party, i16 *id);

/* --- JSONIO --- */
void Party_readJSON( void *input, const cJSON *jparty);
void Party_writeJSON(const void *input, cJSON *jparty);

#endif /* UNIT_PARTY_H */
