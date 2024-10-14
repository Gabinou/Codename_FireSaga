#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "types.h"
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
#include "SDL.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATINOS --- */
struct Breakable;
struct RNG_Sequence;
struct Door;
struct Unit;
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
int Filesystem_Init(char *argv0);
int Filesystem_Free(void);

/* --- MOUNT --- */
void Filesystem_Mount(s8 folder);

/* --- MISC. --- */
b32 Filesystem_fequal(char *filename1,  char *filename2);
void Filesystem_Load_Bytes(char *name, u8 **mem, size_t *len);

/* --- SURFACES&TEXTURES --- */
SDL_Surface *Filesystem_TexturetoSurface(SDL_Renderer *, SDL_Texture *, u32, SDL_Texture *);
void Filesystem_Texture_Dump(char *f, SDL_Renderer *, SDL_Texture *, u32 fo, SDL_Texture *rt);
void Filesystem_Surface_Dump(char *f, SDL_Surface *);

/* -- Loading -- */
SDL_Surface *Filesystem_Surface_Load(char *f, u32 format);
SDL_Texture *Filesystem_Texture_Load(struct SDL_Renderer *,  char *f, u32);

/* -- Palettes & index shaders -- */
SDL_Surface *Filesystem_indexedSurface_Init(size_t w, size_t h);
SDL_Surface *Filesystem_Surface_Palette_Swap(SDL_Surface *s, SDL_Palette *p);
void Filesystem_Surface_Pixels2Indices(SDL_Surface *r, SDL_Surface *i);

/* --- SCENE --- */
s8 Filesystem_Scene_Chapter(s8 filename, int chapter);

#endif /* FILESYSTEM_H */
