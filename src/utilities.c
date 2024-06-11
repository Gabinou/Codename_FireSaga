
#include "utilities.h"
#include "stb_sprintf.h"

#ifndef __clang__

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
        if ((x == list_2D[i * NMATH_TWO_D + 0]) && (y == list_2D[i * NMATH_TWO_D + 1])) {
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
    i32 *out = calloc(arr_len, sizeof(*out));
    return (matrix_sgreater_noM(out, matrix1, tocompare, arr_len));
}

i32 *matrix_ssmaller_noM(i32 *out, i32 *matrix1, i32 tocompare, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] < tocompare);
    return (out);
}

i32 *matrix_ssmaller(i32 *matrix1, i32 tocompare, size_t arr_len) {
    i32 *out = calloc(arr_len, sizeof(*out));
    return (matrix_ssmaller_noM(out, matrix1, tocompare, arr_len));
}

void matrix_print(i32 *array, size_t row_len, size_t col_len) {
    for (size_t row = 0; row < row_len; row++) {
        for (size_t col = 0; col < col_len; col++)
            printf("%2d ", array[row * col_len + col]);
        printf("\n");
    }
}

i32 *matrix_and_noM(i32 *out, i32 *matrix1, i32 *matrix2, size_t arr_len) {
    for (size_t i = 0; i < arr_len; i++)
        out[i] = (matrix1[i] && matrix2[i]);
    return (out);
}

i32 *matrix_and(i32 *matrix1, i32 *matrix2, size_t arr_len) {
    i32 *out = calloc(arr_len, sizeof(*out));
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
    i32 *out = calloc(arr_len, sizeof(*out));
    return (matrix_plus_noM(matrix1, matrix2, arr_len));
}

i32 *matrix2list_noM(i32 *matrix, i32 *list, size_t row_len, size_t col_len) {
    DARR_NUM(list) = 0;
    for (size_t col = 0; col < col_len; col++) {
        for (size_t row = 0; row < row_len; row++) {
            if (matrix[row * col_len + col] <= 0)
                continue;
            DARR_PUT(list, col);
            DARR_PUT(list, row);
        }
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
    i32 *out = calloc(row_len * col_len, sizeof(i32));
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
    int loop;
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

int Utilities_Mirror(int room_diameter, int pos, int object_width) {
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
                  stats.attack[DMG_TYPE_PHYSICAL], stats.attack[DMG_TYPE_MAGICAL],
                  stats.attack[DMG_TYPE_TRUE], stats.protection[DMG_TYPE_PHYSICAL],
                  stats.protection[DMG_TYPE_MAGICAL], stats.range.min, stats.range.max,
                  stats.hit, stats.dodge, stats.crit, stats.favor,
                  stats.wgt, stats.prof);
    return (out);
}

char *Utilities_Print_itemStats(struct Item_stats stats) {
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d", stats.price, stats.uses, stats.AP);
    return (out);
}

i32 army_alignment[ARMY_END] = {
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

b32 army_isPC[ARMY_END] = {
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
    Names_Print_All("");
    Palettes_Load();
    Palette_Tables_Load();
}

void Utilities_Free(void) {
    Palettes_Free();
    Names_Free();
    // Hashes_Free();
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

/* --- Weapons and items --- */
struct WpnorItem Utilities_WpnorItem(i16 id, struct dtab *weapons_dtab, struct dtab *items_dtab) {
    /* --- PRELIMINARIES --- */
    SDL_assert(weapons_dtab != NULL);
    SDL_assert(items_dtab   != NULL);

    struct WpnorItem out = WpnorItem_default;
    out.dtab = dtabs_id[Item_Archetype(id)];
    SDL_assert(out.dtab > SOTA_DTAB_NULL);

    if (out.dtab == SOTA_DTAB_ITEMS) {
        Item_Load(items_dtab, id);
        out.item = DTAB_GET(items_dtab,  id);
    } else {
        Weapon_Load(weapons_dtab, id);
        out.wpn = DTAB_GET(weapons_dtab, id);
    }

    return (out);
}
