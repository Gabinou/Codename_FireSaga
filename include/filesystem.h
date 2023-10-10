#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"
#include "globals.h"
#include "enums.h"
#include "structs.h"
#include "physfs.h"
#include "platform.h"
#include "cJSON.h"
#include "nstr.h"
#include "utilities.h"
#include "nmath.h"
#include "jsonio.h"
#include "tile.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATINOS --- */
struct Breakable;
struct Door;
struct Chest;
struct Mobj_Link;

/* --- PATH --- */
/******************* HACKY DISTRIBUTION FOR VARIADIC MACROS ******************/

// XXX_VAR_EACH_ARGN(__VA_ARGS__) counts the number of args
//      -> up to 63, if elements in XXX_VARMACRO_ARGN and XXXX_VARMACRO_VARG_SEQ exist
#define PATH_JOIN_VAR_EACH_ARGN_(...) PATH_JOIN_VAR_ARGN(__VA_ARGS__)
#define PATH_JOIN_VAR_EACH_ARGN(...) PATH_JOIN_VAR_EACH_ARGN_(__VA_ARGS__, PATH_JOIN_VAR_VARG_SEQ())
#define PATH_JOIN_VAR_ARGN(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define PATH_JOIN_VAR_VARG_SEQ() 8, 7, 6, 5, 4, 3, 2, 1, 0

// XXXX_VARMACRO_FOREACH_XXXX(foo, __VA_ARGS__) applies foo to each __VA_ARGS__, PLUS
//      up to 63 args if all FOREACH_XXXX_N exist
#define PATH_JOIN_FOREACH_1(x) x
#define PATH_JOIN_FOREACH_2(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_1(__VA_ARGS__)
#define PATH_JOIN_FOREACH_3(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_2(__VA_ARGS__)
#define PATH_JOIN_FOREACH_4(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_3(__VA_ARGS__)
#define PATH_JOIN_FOREACH_5(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_4(__VA_ARGS__)
#define PATH_JOIN_FOREACH_6(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_5(__VA_ARGS__)
#define PATH_JOIN_FOREACH_7(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_6(__VA_ARGS__)
#define PATH_JOIN_FOREACH_8(x, ...) x PHYSFS_SEPARATOR PATH_JOIN_FOREACH_7(__VA_ARGS__)

#define PATH_JOIN_(N, ...) SOTA_CONCATENATE(PATH_JOIN_FOREACH_, N)(__VA_ARGS__)
#define PATH_JOIN(...) PATH_JOIN_(PATH_JOIN_VAR_EACH_ARGN(__VA_ARGS__), __VA_ARGS__)

/* --- INIT --- */
extern int Filesystem_Init(char *argv0);
extern int Filesystem_Free();

/* --- LOG --- */
extern void Filesystem_Log2file(void *d, i32 c, SDL_LogPriority p, const char *m);

/* --- MISC. --- */
extern bool Filesystem_fequal(const char *filename1, const char *filename2);
extern void Filesystem_Load_Bytes(const char *name, uf8 **mem, size_t *len);

/* --- SURFACES&TEXTURES --- */
extern SDL_Surface *Filesystem_TexturetoSurface(SDL_Renderer *, SDL_Texture *, u32, SDL_Texture *);
extern void Filesystem_Texture_Dump(const char *f, SDL_Renderer *, SDL_Texture *,
                                    u32 fo, SDL_Texture *rt);
extern void Filesystem_Surface_Dump(const char *f, SDL_Surface *);

/* -- Loading -- */
extern SDL_Surface *Filesystem_Surface_Load(const char *f, u32 in_format);
extern SDL_Texture *Filesystem_Texture_Load(struct SDL_Renderer *, const char *f, u32);

/* -- Palettes & index shaders -- */
extern SDL_Surface *Filesystem_indexedSurface_Init(size_t w, size_t h);
extern SDL_Surface *Filesystem_Surface_Palette_Swap(SDL_Surface *s, SDL_Palette *p);
extern void Filesystem_Surface_Pixels2Indices(SDL_Surface *r, SDL_Surface *i);

/* --- JSON I/O --- */
extern void Filesystem_readJSON_Shop(        const char *f, struct Shop         *s);
extern void Filesystem_readJSON_Palette(     const char *f, struct SDL_Palette  *p);
extern void Filesystem_readJSON_Promotion(   const char *f, struct Promotion    *p);
extern void Filesystem_readJSON_PaletteTable(const char *f, uf8                 *p);

extern void Filesystem_readJSON_RNseed(   const struct cJSON *const j);

extern void Filesystem_readJSON_Array(    const struct cJSON *const j, i32 *arr);
extern void Filesystem_readJSON_2DArray(  const struct cJSON *const j, i32 *, uf8 rl, uf8 cl);

extern void Filesystem_readJSON_Door(     const struct cJSON *const j, struct Door  *d);
extern void Filesystem_readJSON_Chest(    const struct cJSON *const j, struct Chest *c);
extern void Filesystem_readJSON_Item(     const struct cJSON *const j, struct Inventory_item *i);
extern void Filesystem_readJSON_mvtcost(  const struct cJSON *const j, struct fMovement_cost *c);
extern void Filesystem_readJSON_Wpnstats( const struct cJSON *const j, struct Weapon_stats   *s);
extern void Filesystem_readJSON_Position( const struct cJSON *const j, struct Point          *p);
extern void Filesystem_readJSON_Tilestats(const struct cJSON *const j, struct Tile_stats     *s);
extern void Filesystem_readJSON_Unitstats(const struct cJSON *const j, struct Unit_stats     *s);
extern void Filesystem_readJSON_MObj_Link(const struct cJSON *const j, struct Mobj_Link      *b);
extern void Filesystem_readJSON_Breakable(const struct cJSON *const j, struct Breakable      *b);
extern void Filesystem_readJSON_Itemstats(const struct cJSON *const j, struct Item_stats     *s);
extern void Filesystem_readJSON_Reinforce(const struct cJSON *const j, struct Reinforcement  *a);

extern void Filesystem_writeJSON_RNseed(   struct cJSON *j);

extern void Filesystem_writeJSON_Array(    struct cJSON *j, const i32 *arr, size_t l);
extern void Filesystem_writeJSON_2DArray(  struct cJSON *j, const i32 *arr, uf8 rl, uf8 cl);

extern void Filesystem_writeJSON_item(     struct cJSON *j, const struct Inventory_item *i);
extern void Filesystem_writeJSON_arrival(  struct cJSON *j, const struct Reinforcement  *a);
extern void Filesystem_writeJSON_mvtcost(  struct cJSON *j, const struct fMovement_cost *c);
extern void Filesystem_writeJSON_Wpnstats( struct cJSON *j, const struct Weapon_stats   *s);
extern void Filesystem_writeJSON_Itemstats(struct cJSON *j, const struct Item_stats     *t);
extern void Filesystem_writeJSON_Unitstats(struct cJSON *j, const struct Unit_stats     *t);
extern void Filesystem_writeJSON_Tilestats(struct cJSON *j, const struct Tile_stats     *t);

extern void Filesystem_printJSON(PHYSFS_file *fp, const struct cJSON *json);

/* --- SCENE --- */
extern void Filesystem_Scene_Chapter(char *f, int chapter);

#endif /* FILESYSTEM_H */
