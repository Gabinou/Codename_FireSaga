
#include "names.h"
#include "nmath.h"
#include "sprite.h"
#include "n9patch.h"
#include "palette.h"
#include "position.h"
#include "platform.h"
#include "utilities.h"

#include "names/write_folders.h"
#include "names/zip_archive.h"

#include "stb_sprintf.h"

#ifndef __clang__


Point Point_Sign(Point vec) {
    Point sign = {
        .x = ((vec.x > 0) - (vec.x < 0)),
        .y = ((vec.y > 0) - (vec.y < 0))
    };
    SDL_assert((sign.x == 1) || (sign.x == -1) || (sign.x == 0));
    SDL_assert((sign.y == 1) || (sign.y == -1) || (sign.y == 0));
    return (sign);
}

/* --- matrix --- */
size_t *matrix_where(i32 *array, i32 to_find, size_t arr_len) {
    size_t *found_list = DARR_INIT(found_list, size_t, arr_len);
    for (size_t i = 0; i < arr_len; i++) {
        if (array[i] == to_find) {
            DARR_PUT(found_list, i);
            break;
        }
    }
    DARR_LEN(found_list) = DARR_NUM(found_list);
    size_t newl = (DARR_NUM(found_list) < SOTA_MINLEN ? SOTA_MINLEN : DARR_NUM(found_list));
    found_list = DARR_REALLOC(found_list, newl);
    return (found_list);
}

b32 list_isIn_2D(i32 *list_2D, size_t list_len, i32 x, i32 y) {
    b32 found = false;
    for (size_t i = 0; i < list_len; i++) {
        if ((x == list_2D[i * NMATH_TWO_D + 0]) &&
            (y == list_2D[i * NMATH_TWO_D + 1])
           ) {
            found = true;
            break;
        }
    }
    return (found);
}

i32 *matrix_sgreater_noM(i32 *out, i32 *matrix1, i32 tocompare, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] > tocompare);
    return (out);
}

i32 *matrix_sgreater(i32 *matrix1, i32 tocompare, size_t arr_len) {
    i32 *out = SDL_calloc(arr_len, sizeof(*out));
    return (matrix_sgreater_noM(out, matrix1, tocompare, arr_len));
}

i32 *matrix_ssmaller_noM(i32 *out, i32 *matrix1, i32 tocompare, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] < tocompare);
    return (out);
}

i32 *matrix_ssmaller(i32 *matrix1, i32 tocompare, size_t arr_len) {
    i32 *out = SDL_calloc(arr_len, sizeof(*out));
    return (matrix_ssmaller_noM(out, matrix1, tocompare, arr_len));
}

void matrix_print(  i32 *array,
                    size_t row_len,
                    size_t col_len) {
    printf("\n");
    for (size_t i = 0; i < row_len * col_len; i++) {
        printf("%d,", array[i]);
        if (sota_ss_x(i, col_len) == (col_len - 1)) {
            printf("\n");
        }
    }
}

void entity_print(  tnecs_E *array,
                    size_t row_len, size_t col_len) {
    for (size_t i = 0; i < row_len * col_len; i++) {
        printf("%2lld ", array[i]);
        if (sota_ss_x(i, col_len) == (col_len - 1)) {
            printf("\n");
        }
    }
}

i32 *matrix_and_noM(i32 *out, i32 *matrix1,
                    i32 *matrix2, size_t arr_len) {
    SDL_assert(out      != NULL);
    SDL_assert(matrix1  != NULL);
    SDL_assert(matrix2  != NULL);
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] && matrix2[i]);
    return (out);
}

i32 *matrix_and(i32 *matrix1, i32 *matrix2, size_t arr_len) {
    i32 *out = SDL_calloc(arr_len, sizeof(*out));
    return (matrix_and_noM(out, matrix1, matrix2, arr_len));
}

i32 *matrix_sub_noM(i32 *matrix1, i32 *matrix2, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        matrix1[i] -= matrix2[i];
    return (matrix1);
}

i32 *matrix_plus_noM(i32 *matrix1, i32 *matrix2, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        matrix1[i] += matrix2[i];
    return (matrix1);
}

i32 *matrix_plus(i32 *matrix1, i32 *matrix2, size_t arr_len) {
    i32 *out = SDL_calloc(arr_len, sizeof(*out));
    return (matrix_plus_noM(out, matrix2, arr_len));
}

i32 *matrix2list_noM(   i32 *matrix,
                        i32 *list,
                        size_t row_len,
                        size_t col_len) {
    DARR_NUM(list) = 0;
    /* Visited tile order: x/col first. */
    for (size_t i = 0; i < row_len * col_len; i++) {
        if (matrix[i] <= 0)
            continue;
        i32 col = sota_ss_x(i, col_len);
        i32 row = sota_ss_y(i, col_len);
        DARR_PUT(list, col);
        DARR_PUT(list, row);
    }
    return (list);
}

i32 *matrix2list(i32 *matrix, size_t row_len, size_t col_len) {
    i32 *list = DARR_INIT(list, i32, row_len * col_len * 2);
    list = matrix2list_noM(matrix, list, row_len, col_len);
    size_t newsize = (DARR_NUM(list) < SOTA_MINLEN) ? SOTA_MINLEN : DARR_NUM(list);
    list = DARR_REALLOC(list, newsize);

    return (list);
}

i32 *list2matrix_noM(i32 *out, i32 *list, size_t row_len, size_t col_len, size_t list_len) {
    for (size_t elem = 0; elem < list_len; elem++) {
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;
    }

    return (out);
}

i32 *list2matrix(i32 *list, size_t row_len, size_t col_len, size_t list_len) {
    i32 *out = SDL_calloc(row_len * col_len, sizeof(i32));
    list2matrix_noM(out, list, row_len, col_len, list_len);

    return (out);
}

#endif  /* __clang__ */

/* --- Array --- */
int int_inbounds(int pos, int boundmin, int boundmax) {
    int out = 0;
    out = pos < boundmin ? boundmin : pos;
    out = out > boundmax ? boundmax : out;
    return (out);
}

b32 i8_all_equal(i8 *arr1, i8 *arr2, size_t len) {
    b32 arrequal = true;
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            arrequal = false;
            break;
        }
    }
    return (arrequal);
}

size_t Util_SDL_Surface_Index(SDL_Surface *surf, int x, int y) {
    /* Note: Pitch is the width of the memory buffer in bytes.      */
    /* Pitch [bytes] = width [pixels] * BytesPerPixel [bytes/pixel] */
    return (y * surf->pitch + x * surf->format->BytesPerPixel);
}

b32 Utilities_charArr_Equal(char *arr1, char *arr2, size_t len) {
    b32 arrequal = true;
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            arrequal = false;
            break;
        }
    }
    return (arrequal);
}

/* --- Cursor --- */
void Cursor_Box_Offset(struct Point *pos) {
    pos->x -= 2 * SOTA_TILESIZE;
    pos->y -= 2 * SOTA_TILESIZE;
}

/* -- Directions -- */
int Utilities_Loop(int direction, int flip) {
    /* Get sprite loop from direction and flip */
    int loop = LOOPING_INVALID;
    if (direction == SOTA_DIRECTION_BOTTOM)
        loop = MAP_UNIT_LOOP_MOVEB;
    else if (direction == SOTA_DIRECTION_TOP)
        loop = MAP_UNIT_LOOP_MOVET;
    else if (direction == SOTA_DIRECTION_LEFT) {
        b32 isflip = (flip == SDL_FLIP_HORIZONTAL);
        loop = isflip * MAP_UNIT_LOOP_MOVER + (1 - isflip) * MAP_UNIT_LOOP_MOVEL;
    } else if (direction == SOTA_DIRECTION_RIGHT) {
        b32 isflip = (flip == SDL_FLIP_HORIZONTAL);
        loop = isflip * MAP_UNIT_LOOP_MOVEL + (1 - isflip) * MAP_UNIT_LOOP_MOVER;
    }

    return (loop);
}

/* --- Maths --- */
float sota_slowpow(float base, int exponent) {
    /* Super fast for -O1/-O2 optimization */
    if (exponent == 0)
        return (1.0f);
    float out = base;
    for (int i = 0; i < (exponent - 1); i++)
        out *= base;
    return (out);
}

int Utilities_Mirror(   int room_diameter,
                        int pos, int object_width) {
    SDL_assert(pos !=    (room_diameter / 2)); // object is not on mirror
    SDL_assert(object_width < (room_diameter / 2)); // object is not bigger than room
    // Shifting object left/right, depends on which side of the mirror pos is
    int shift = pos > room_diameter ? - object_width : object_width;

    int mirrored_pos = room_diameter - pos + shift;
    return (mirrored_pos);
}

/* Reset Drawing color to black */
void Utilities_DrawColor_Reset(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
}

char *Utilities_Print_unitStats(struct Unit_stats stats) {
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d", stats.hp,
                  stats.str, stats.mag, stats.agi, stats.dex, stats.luck,
                  stats.def, stats.res, stats.con, stats.move, stats.prof);
    return (out);
}

char *Utilities_Print_wpnStats(struct Weapon_stats stats) {
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d",
                  stats.attack.physical, stats.attack.magical,
                  stats.attack.True, stats.protection.physical,
                  stats.protection.magical, stats.range.min, stats.range.max,
                  stats.hit, stats.dodge, stats.crit, stats.favor,
                  stats.wgt, stats.prof);
    return (out);
}

char *Utilities_Print_itemStats(struct Item_stats stats) {
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d", stats.price, stats.uses, stats.AP);
    return (out);
}

const i32 army_alignment[ARMY_END] = {
    /* ARMY_START                     */ ALIGNMENT_ENEMY,
    /* FRIENDLY                       */ ALIGNMENT_FRIENDLY,
    /* ENEMY                          */ ALIGNMENT_ENEMY,
    /* ERWIN                          */ ALIGNMENT_FRIENDLY,
    /* FREE_MILITIA                   */ ALIGNMENT_FRIENDLY,
    /* NEUTRAL                        */ ALIGNMENT_FRIENDLY,
    /* IMPERIAL                       */ ALIGNMENT_FRIENDLY,
    /* HAMILCAR                       */ ALIGNMENT_ENEMY,
    /* FIRST_EXPEDITIONARY_FORCE      */ ALIGNMENT_ENEMY,
    /* THEOCRACY                      */ ALIGNMENT_FRIENDLY,
    /* VOLDAN                         */ ALIGNMENT_FRIENDLY,
    /* KEWAC                          */ ALIGNMENT_ENEMY,
    /* KESIRU                         */ ALIGNMENT_FRIENDLY,
    /* BANDITS                        */ ALIGNMENT_ENEMY,
};

const b32 army_isPC[ARMY_END] = {
    /* ARMY_START                     */ false,
    /* FRIENDLY                       */ true,
    /* ENEMY                          */ false,
    /* ERWIN                          */ true,
    /* FREE_MILITIA                   */ true,
    /* NEUTRAL                        */ false,
    /* IMPERIAL                       */ false,
    /* HAMILCAR                       */ false,
    /* FIRST_EXPEDITIONARY_FORCE      */ false,
    /* THEOCRACY                      */ false,
    /* VOLDAN                         */ false,
    /* KEWAC                          */ false,
    /* KESIRU                         */ false,
    /* BANDITS                        */ false,
};

void Utilities_Load(void) {
    Names_Load_All();
#ifdef SOTA_NAMES_PRINT_ALL
    Names_Print_All("");
#endif /* SOTA_NAMES_PRINT_ALL */
    Palettes_Load();
    Palette_Tables_Load();
}

void Utilities_Free(void) {
    Palettes_Free();
    Names_Free();
    // Hashes_Free();
}

/* -- SDL_Rect -- */
SDL_Rect Utilities_Rect(Position *pos, n9Patch *n9patch) {
    SDL_assert(pos      != NULL);
    SDL_assert(n9patch  != NULL);
    Point n9_size = n9Patch_Pixels_Total(n9patch);

    SDL_Rect rect = {
        .x = pos->pixel_pos.x,
        .y = pos->pixel_pos.y,
        .w = n9_size.x,
        .h = n9_size.y,
    };
    return (rect);
}

void SOTA_Free_Surfaces(SDL_Surface **surfaces, int num) {
    for (size_t i = 0; i < num; i++) {
        if (surfaces[i] == NULL)
            continue;

        SDL_FreeSurface(surfaces[i]);
        surfaces[i] = NULL;
    }
}

void SOTA_Free_Textures(SDL_Texture **textures, int num) {
    for (size_t i = 0; i < num; i++) {
        if (textures[i] == NULL)
            continue;

        SDL_DestroyTexture(textures[i]);
        textures[i] = NULL;
    }
}

/* -- Effective fps for framerate independence -- */
// Instant FPS can be used to mitigate frame dependence.
// fps_instant^-1 is a good estimate of time elapsed
// in previous frame.
f32 FPS_Effective(f32 fps_target,
                  f32 fps_instant) {
    /* 1. Game is going fast
    **      - Only possible w/ Fast Forward (FF)
    **      - Everything should go fast, so:
    **      - return fps_target */
    if (fps_instant >= fps_target) {
        /* SDL_Log("1. Game is going fast"); */
        return (fps_target);
    }
    /* 2. fps_instant is uninitialized.
    **      - fps_instant is kept in memory
    **      if timer was reset.
    **      - failsafe & tests  */
    if (fps_instant <= 0.0f) {
        /* SDL_Log("2. fps_instant is uninitialized."); */
        return (fps_target);
    }

    /* 3. Game is going VERY slow
    **      - cap effective estimate, so as not to
    **        go crazy when mitigating for lag
    **      - return fps_target / IES_MAX_LAG_FACTOR
    */
    if (fps_instant < (fps_target / IES_MAX_LAG_FACTOR)) {
        /* SDL_Log("3. Game is going VERY slow"); */
        return (fps_target / IES_MAX_LAG_FACTOR);
    }

    /* 4. Game is going slow
    **      - fps_instant can be used to mitigate lag
    **      - return fps_instant */
    /* SDL_Log("4. Game is going slow"); */
    return (fps_instant);
}

s8 IES_Archive_Name(void) {
    // #define REGISTER_ENUM(x) s8 archive = s8_mut(#x);
    // #include "names/zip_archive.h"
    // #undef REGISTER_ENUM
    s8 archive = s8_mut(STRINGIZE(ZIP_ARCHIVE_NAME));
    return (archive);
}

s8 IES_Path_Install(void) {
    s8 dir = IES_Path();
    dir = s8cat(dir, s8_literal(DIR_SEPARATOR GAME_INSTALL_DIR));
    return (dir);
}

s8 IES_Path_Build(void) {
    s8 dir = IES_Path();
    dir = s8cat(dir, s8_literal(DIR_SEPARATOR GAME_BUILD_DIR));
    return (dir);
}

s8 IES_Path_Saves(void) {
    s8 dir = s8_mut(GAME_SAVE_DIR);
    return (dir);
}

s8 IES_Path(void) {
    /* -- path to firesaga folder -- */
    /*  e.g /home/gabinours/firesaga */
    char *temp_base = SDL_GetBasePath();

    s8 dir       = s8_mut(temp_base);
    if (dir.data[dir.num - 1] == DIR_SEPARATOR[0]) {
        dir = s8_Path_Remove_Top(dir,    DIR_SEPARATOR[0]);
    }
    dir = s8_Path_Remove_Top(dir,    DIR_SEPARATOR[0]);

    return (dir);
}
