#ifndef JSONIO_H
#define JSONIO_H

#include "tile.h"
#include "weapon.h"
#include "item.h"
#include "macros.h"
#include "convoy.h"
// #include "camp.h"
#include "map.h"
#include "unit.h"
#include "narrative.h"
#include "utilities.h"

struct Item;
extern SDL_Renderer * Game_renderer;

extern struct cJSON * parseJSON(const char * filename);
extern void writeJSON(const char * filename, const void * struct_ptr, const bool append);
extern void readJSON(const char * filename, const void * struct_ptr);

#endif /* filesystem_H */
