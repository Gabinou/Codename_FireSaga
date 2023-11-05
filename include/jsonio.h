#ifndef JSONIO_H
#define JSONIO_H

#include "tile.h"
#include "weapon.h"
#include "item.h"
#include "macros.h"
#include "convoy.h"
#include "physfs.h"
#include "camp.h"
#include "sprite.h"
#include "unit.h"
#include "narrative.h"
#include "utilities.h"
#include "supports.h"
#include "names.h"
#include "SDL.h"

/* --- TYPEDEFS --- */
typedef void (*json_read_t)(       void *, const cJSON *const);
typedef void (*json_write_t)(const void *,       cJSON *);

/* --- READERS & WRITERS --- */
extern json_read_t  json_read_funcs [JSON_END];
extern json_write_t json_write_funcs[JSON_END];

/* --- API --- */
void          jsonio_readJSON( s8 f,       void *ptr);
void          jsonio_writeJSON(s8 f, const void *ptr, bool a);
struct cJSON *jsonio_parseJSON(s8 f);

#endif /* Filesystem_H */
