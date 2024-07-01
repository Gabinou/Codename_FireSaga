#ifndef UNIT_PARTY_H
#define UNIT_PARTY_H

#include "types.h"
#include "unit.h"
#include "nstr.h"
#include "jsonio.h"

/* --- Constructor/Destructors --- */
void Party_Free( struct Party *party);
void Party_Init( struct Party *party);
void Party_Reset(struct Party *party);

/* --- Order --- */
// int Party_Unit_Order(struct Party *party, i16 id);

/* --- Utilities --- */
void Party_Folder(          struct Party *ps, char *folder);
void Party_Ids2Filenames(   struct Party *ps);
void Party_Names2Filenames( struct Party *ps);
i32 Party_Size( struct Party *ps);
i32 _Party_Size(struct Unit *party, i16 *id);
void Party_ID_Stack(struct Party *party);

/* --- JSONIO --- */
/* -- Load party units from party structs -- */
void Party_Load(    struct Party    *party,     struct Game *sota,
                    struct dtab     *wdtab,     struct dtab *idtab);
void _Party_Load(   tnecs_entity *entities,     struct Game *sota,
                    struct dtab     *wdtab,     struct dtab *idtab,
                    s8 *farr, size_t load_num);

/* -- Load party struct -- */
void Party_readJSON(void *input, cJSON *jparty);

/* -- Write file with list of units -- */
void Party_writeJSON(void *input, cJSON *jparty);

/* -- Each individual unit as a separate json file -- */
void Party_Units_writeJSON(void *input, cJSON *jparty);

#endif /* UNIT_PARTY_H */
