#ifndef UTILITIES_H
#define UTILITIES_H

#include "types.h"
#include "enums.h"
#include "structs.h"

#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct RNG_Sequence;
struct Position;
struct n9Patch;

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
#define IES_MAX_LAG_FACTOR      10.0f

/* --- matrix --- */

b32 list_isIn_2D(i32 *list_2D, size_t list_len, i32 x, i32 y);
i32 *matrix2list_noM(i32 *matrix, i32 *list, size_t row_len, size_t col_len);
i32 *matrix2list(    i32 *matrix,            size_t row_len, size_t col_len);
i32 *list2matrix(    i32 *list,              size_t row_len, size_t col_len, size_t list_len);
i32 *list2matrix_noM(i32 *out, i32 *list,    size_t row_len, size_t col_len, size_t list_len);

void matrix_print(i32 *array, size_t row_len, size_t col_len);
void entity_print(tnecs_E *array, size_t row_len, size_t col_len);

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

/* --- Array --- */
int int_inbounds(int pos, int boundmin, int boundmax);
b32 i8_all_equal(i8 *arr1, i8 *arr2, size_t len);
b32 Utilities_charArr_Equal(char *arr1, char *arr2, size_t len);

/* --- Cursor --- */
void Cursor_Box_Offset(struct Point *pos);
Point Point_Sign(Point vec);

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

/* -- SDL_Rect -- */
SDL_Rect Utilities_Rect(struct Position *pos, struct n9Patch *n9patch);

/* -- SDL stuff -- */
size_t Util_SDL_Surface_Index(SDL_Surface *surf, int x, int y);

/* -- Loading -- */
void Utilities_Load(void);
void Utilities_Free(void);

void SOTA_Free_Surfaces(SDL_Surface **surfaces, int num);
void SOTA_Free_Textures(SDL_Texture **textures, int num);

void Utilities_DrawColor_Reset(SDL_Renderer *renderer);

/* -- FPS -- */
f32 FPS_Effective(f32 t, f32 i);

/* -- archive -- */
s8 IES_Archive_Name(void);
s8 IES_Path(void);
s8 IES_Path_Saves(void);
s8 IES_Path_Build(void);
s8 IES_Path_Install(void);

#endif /* UTILITIES_H */
