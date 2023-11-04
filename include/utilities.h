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
#include "types.h"
#include "octant.h"
#include "unit.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "item.h"
#include "weapon.h"
#include "filesystem.h"
#include "structs.h"
#include "nmath.h"
#include "nstr.h"
#include "SDL.h"

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

#define SOTA_MINLEN 12

/* --- matrix --- */
bool list_isIn_2D(i32 *list_2D, size_t list_len, i32 x, i32 y);
i32 *matrix2list_noM(i32 *matrix, i32 *list, size_t row_len, size_t col_len);
i32 *matrix2list(    i32 *matrix,            size_t row_len, size_t col_len);
i32 *list2matrix(i32 *list, size_t row_len, size_t col_len, size_t list_len);
i32 *list2matrix_noM(i32 *out, i32 *list, size_t row_len, size_t col_len, size_t list_len);

void matrix_print(i32 *array, size_t row_len, size_t col_len);

size_t *matrix_where(i32 *array, i32 to_find, size_t arr_len);

i32 *matrix_plus_noM(i32 *matrix1, i32 *matrix2, size_t arr_len);
i32 *matrix_sgreater_noM(i32 *out, i32 *matrix1, i32 tocompare, size_t arr_len);
i32 *matrix_sgreater(i32 *matrix1, i32 tocompare, size_t arr_len);
i32 *matrix_ssmaller_noM(i32 *out, i32 *matrix1, i32 tocompare, size_t arr_len);
i32 *matrix_ssmaller(i32 *matrix1, i32 tocompare, size_t arr_len);
i32 *matrix_and_noM(i32 *out, i32 *matrix1, i32 *matrix2, size_t arr_len);
i32 *matrix_and(i32 *matrix1, i32 *matrix2, size_t arr_len);
i32 *matrix_sub_noM(i32 *matrix1, i32 *matrix2, size_t arr_len);
i32 *matrix_plus(i32 *matrix1, i32 *matrix2, size_t arr_len);

/* --- Debug --- */
void Utilities_stacktrace();

/* --- Array --- */
int int_inbounds(int pos, int boundmin, int boundmax);
bool i8_all_equal(i8 *arr1, i8 *arr2, size_t len);
bool Utilities_charArr_Equal(char *arr1, char *arr2, size_t len);

/* --- Weapons and items --- */
struct WpnorItem Utilities_WpnorItem(i16 id, struct dtab *wt, struct dtab *it);

/* --- Cursor --- */
void Cursor_Box_Offset(struct Point *pos);

/* --- Maths --- */
float sota_slowpow(float base, int exponent);
/* Computed mirrored object position in a room */
//      mirror
// |-a-----|------a'|
// |________________|
//    room diameter
int Utilities_Mirror(int room_diameter, int pos, int object_width);

/* -- Directions -- */
int Utilities_Loop(int direction, int flip);

/* -- Printing/Debug -- */
char *Utilities_Print_wpnStats( struct Weapon_stats stats);
char *Utilities_Print_unitStats(struct Unit_stats   stats);
char *Utilities_Print_itemStats(struct Item_stats   stats);

/* -- SDL stuff -- */
size_t Util_SDL_Surface_Index(SDL_Surface *surf, int x, int y);

/* -- Loading -- */
void Utilities_Load();
void Utilities_Free();

void SOTA_Free_Surfaces(SDL_Surface **surfaces, int num);
void SOTA_Free_Textures(SDL_Texture **textures, int num);

void Utilities_DrawColor_Reset(SDL_Renderer *renderer);

#endif /* UTILITIES_H */
