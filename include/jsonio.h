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
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

/* --- TYPEDEFS --- */
typedef void (*json_read_t)(       void *, const cJSON *const);
typedef void (*json_write_t)(const void *,       cJSON *);

/* --- READERS & WRITERS --- */
extern json_read_t  json_read_funcs [JSON_END];
extern json_write_t json_write_funcs[JSON_END];

/* --- API --- */
extern void          jsonio_readJSON( const char *f,       void *ptr);
extern void          jsonio_writeJSON(const char *f, const void *ptr, bool a);
extern struct cJSON *jsonio_parseJSON(const char *f);

#endif /* Filesystem_H */
