#ifndef UNIT_PARTY_H
#define UNIT_PARTY_H

#include "types.h"
#include "unit.h"
#include "jsonio.h"

/* --- Party --- */
/* Just for Json loading */
struct Party {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */
    
    s8   party_folder;
    struct Unit *party
}

void Party_readJSON(void *input, cJSON *jparty);

/* --- Write file with list of units --- */
void Party_writeJSON(void *input, cJSON *jparty);

/* --- Each individual unit as a separate json file --- */
void Party_Units_writeJSON(void *input, cJSON *jparty);

#endif /* UNIT_PARTY_H */
