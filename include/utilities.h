#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "platform.h"
#include "palette.h"
#include "names.h"
#include "types.h"
#include "hashes.h"
#include "octant.h"
#include "enums.h"
#include "globals.h"
#include "macros.h"
#include "debug.h"
#include "item.h"
#include "weapon.h"
#include "filesystem.h"
#include "structs.h"
#include "nmath.h"
#include "nstr.h"
#include "SDL2/SDL.h"

/* #if SDL_BYTEORDER == SDL_BIG_ENDIAN */
/* rmask = 0xff000000;                 */
/* gmask = 0x00ff0000;                 */
/* bmask = 0x0000ff00;                 */
/* amask = 0x000000ff;                 */
/* #else                               */
/* rmask = 0x000000ff;                 */
/* gmask = 0x0000ff00;                 */
/* bmask = 0x00ff0000;                 */
/* amask = 0xff000000;                 */
/* #endif                              */

/* --- Debug --- */
void Utilities_stacktrace();

/* --- Array --- */
int int_inbounds(int pos, int boundmin, int boundmax);
bool if8_all_equal(if8 *arr1, if8 *arr2, size_t len);
bool Utilities_charArr_Equal(char *arr1, char *arr2, size_t len);

/* --- Weapons and items --- */
struct WpnorItem Utilities_WpnorItem(if16 id, struct dtab *wt, struct dtab *it);

/* --- Cursor --- */
extern void Cursor_Box_Offset(struct Point *pos);

/* Computed mirrored object position in a room */
//      mirror
// |-a-----|------a'|
// |________________|
//    room diameter
extern int Utilities_Mirror(int room_diameter, int pos, int object_width);

/* -- Directions -- */
extern int Utilities_Loop(int direction, int flip);

/* -- Printing/Debug -- */
extern char *Utilities_Print_wpnStats( struct Weapon_stats stats);
extern char *Utilities_Print_unitStats(struct Unit_stats   stats);
extern char *Utilities_Print_itemStats(struct Item_stats   stats);

/* -- SDL stuff -- */
extern size_t Util_SDL_Surface_Index(SDL_Surface *surf, int x, int y);

/* -- Loading -- */
extern void Utilities_Load();
extern void Utilities_Free();

extern void SOTA_Free_Surfaces(SDL_Surface **surfaces, int num);
extern void SOTA_Free_Textures(SDL_Texture **textures, int num);

extern void Utilities_DrawColor_Reset(SDL_Renderer *renderer);

#endif /* UTILITIES_H */
