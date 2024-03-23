#ifndef UNIT_PARTY_H
#define UNIT_PARTY_H

#include "types.h"
#include "unit.h"
#include "nstr.h"
#include "jsonio.h"

/* --- Party --- */
/* Just for Json loading */
struct Party {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    s8   folder;
    s8  *unit_names;
    s8  *filenames;
    i16 *ids;
    struct Unit *party;
};
extern struct Party Party_default;

/* --- Constructor/Destructors --- */
void Party_Free(struct Party *party);

/* --- Utilities --- */
void Party_Folder(          struct Party *ps, char *folder);
void Party_Ids2Filenames(   struct Party *ps);
void Party_Names2Filenames( struct Party *ps);

/* --- JSONIO --- */
/* -- Load party units from party structs -- */
void Party_Load_Units(  struct Party *ps,    struct Unit *pu,
                        struct dtab  *wdtab, struct dtab *idtab);
void _Party_Load_Units( struct Party *party, struct Unit *pu);

/* -- Load party struct -- */
void Party_readJSON(void *input, cJSON *jparty);

/* -- Write file with list of units -- */
void Party_writeJSON(void *input, cJSON *jparty);

/* -- Each individual unit as a separate json file -- */
void Party_Units_writeJSON(void *input, cJSON *jparty);

#endif /* UNIT_PARTY_H */
