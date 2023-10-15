
#include "utilities.h"
#include "stb_sprintf.h"

#ifndef __clang__

/* --- matrix --- */
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
            printf("%2.6f ", array[row * col_len + col]);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    DARR_NUM(list) = 0;
    for (size_t col = 0; col < col_len; col++) {
        for (size_t row = 0; row < row_len; row++) {
            if (matrix[row * col_len + col] <= 0)
                continue;
            DARR_PUT(list, col);
            DARR_PUT(list, row);
        }
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (list);
}

i32 *matrix2list(i32 *matrix, size_t row_len, size_t col_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 *list = DARR_INIT(list, i32, row_len * col_len * 2);
    list = matrix2list_noM(matrix, list, row_len, col_len);
    size_t newsize = (DARR_NUM(list) < SOTA_MINLEN) ? SOTA_MINLEN : DARR_NUM(list);
    list = DARR_REALLOC(list, newsize);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (list);
}

i32 *list2matrix_noM(i32 *out, i32 *list, size_t row_len, size_t col_len, size_t list_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    for (size_t elem = 0; elem < list_len; elem++) {
        out[list[2 * elem + 1] * col_len + list[2 * elem + 0]] = 1;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

i32 *list2matrix(i32 *list, size_t row_len, size_t col_len, size_t list_len) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 *out = calloc(row_len * col_len, sizeof(i32));
    list2matrix_noM(out, list, row_len, col_len, list_len);

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}


/* --- Debug --- */
void Utilities_stacktrace() {
    /* Depends on glibc. Need '-rdynamic' linker options. */
    void *array[10];
    size_t size = backtrace(array, 10);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
}
#endif  /* __clang__ */

/* --- Array --- */
int int_inbounds(int pos, int boundmin, int boundmax) {
    int out = 0;
    out = pos < boundmin ? boundmin : pos;
    out = out > boundmax ? boundmax : out;
    return (out);
}

bool if8_all_equal(if8 *arr1, if8 *arr2, size_t len) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool arrequal = true;
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            arrequal = false;
            break;
        }
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (arrequal);
}

size_t Util_SDL_Surface_Index(SDL_Surface *surf, int x, int y) {
    /* Note: Pitch is the width of the memory buffer in bytes.      */
    /* Pitch [bytes] = width [pixels] * BytesPerPixel [bytes/pixel] */
    return (y * surf->pitch + x * surf->format->BytesPerPixel);
}


bool Utilities_charArr_Equal(char *arr1, char *arr2, size_t len) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    bool arrequal = true;
    for (int i = 0; i < len; i++) {
        if (arr1[i] != arr2[i]) {
            arrequal = false;
            break;
        }
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (arrequal);
}

/* --- Cursor --- */
void Cursor_Box_Offset(struct Point *pos) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    pos->x -= 2 * SOTA_TILESIZE;
    pos->y -= 2 * SOTA_TILESIZE;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* -- Directions -- */
int Utilities_Loop(int direction, int flip) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* Get sprite loop from direction and flip */
    int loop;
    if (direction == SOTA_DIRECTION_BOTTOM)
        loop = MAP_UNIT_SPRITE_LOOP_MOVEB;
    else if (direction == SOTA_DIRECTION_TOP)
        loop = MAP_UNIT_SPRITE_LOOP_MOVET;
    else if (direction == SOTA_DIRECTION_LEFT) {
        bool isflip = (flip == SDL_FLIP_HORIZONTAL);
        loop = isflip * MAP_UNIT_SPRITE_LOOP_MOVER + (1 - isflip) * MAP_UNIT_SPRITE_LOOP_MOVEL;
    } else if (direction == SOTA_DIRECTION_RIGHT) {
        bool isflip = (flip == SDL_FLIP_HORIZONTAL);
        loop = isflip * MAP_UNIT_SPRITE_LOOP_MOVEL + (1 - isflip) * MAP_UNIT_SPRITE_LOOP_MOVER;
    }

    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (loop);
}


/* --- Maths --- */
int Utilities_Mirror(int room_diameter, int pos, int object_width) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(pos !=    (room_diameter / 2)); // object is not on mirror
    SDL_assert(object_width < (room_diameter / 2)); // object is not bigger than room
    // Shifting object left/right, depends on which side of the mirror pos is
    int shift = pos > room_diameter ? - object_width : object_width;

    int mirrored_pos = room_diameter - pos + shift;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (mirrored_pos);
}

void Utilities_DrawColor_Reset(SDL_Renderer *renderer) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

char *Utilities_Print_unitStats(struct Unit_stats stats) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d", stats.hp,
                  stats.str, stats.mag, stats.agi, stats.dex, stats.luck,
                  stats.def, stats.res, stats.con, stats.move, stats.prof);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

char *Utilities_Print_wpnStats(struct Weapon_stats stats) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d %02d",
                  stats.attack[DMG_TYPE_PHYSICAL], stats.attack[DMG_TYPE_MAGICAL],
                  stats.attack[DMG_TYPE_TRUE], stats.protection[DMG_TYPE_PHYSICAL],
                  stats.protection[DMG_TYPE_MAGICAL], stats.range.min, stats.range.max,
                  stats.hit, stats.dodge, stats.crit, stats.favor,
                  stats.wgt, stats.prof);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

char *Utilities_Print_itemStats(struct Item_stats stats) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    char *out = SDL_malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
    stbsp_sprintf(out, "%02d %02d %02d", stats.price, stats.uses, stats.AP);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

uf8 army_alignment[ARMY_END] = {
    /* ARMY_START                     */ ALIGNMENT_ENEMY,
    /* ARMY_FRIENDLY                  */ ALIGNMENT_FRIENDLY,
    /* ARMY_ERWIN                     */ ALIGNMENT_FRIENDLY,
    /* ARMY_FREE_MILITIA              */ ALIGNMENT_FRIENDLY,
    /* ARMY_NEUTRAL                   */ ALIGNMENT_FRIENDLY,
    /* ARMY_IMPERIAL                  */ ALIGNMENT_FRIENDLY,
    /* ARMY_HAMILCAR                  */ ALIGNMENT_ENEMY,
    /* ARMY_FIRST_EXPEDITIONARY_FORCE */ ALIGNMENT_ENEMY,
    /* ARMY_THEOCRACY                 */ ALIGNMENT_FRIENDLY,
    /* ARMY_VOLDAN                    */ ALIGNMENT_FRIENDLY,
    /* ARMY_KEWAC                     */ ALIGNMENT_ENEMY,
    /* ARMY_KESIRU                    */ ALIGNMENT_FRIENDLY,
    /* ARMY_ENEMY                     */ ALIGNMENT_ENEMY,
    /* ARMY_BANDITS                   */ ALIGNMENT_ENEMY,
};

bool army_isPC[ARMY_END] = {
    /* ARMY_START                     */ false,
    /* ARMY_FRIENDLY                  */ true,
    /* ARMY_ERWIN                     */ true,
    /* ARMY_FREE_MILITIA              */ true,
    /* ARMY_NEUTRAL                   */ false,
    /* ARMY_IMPERIAL                  */ false,
    /* ARMY_HAMILCAR                  */ false,
    /* ARMY_FIRST_EXPEDITIONARY_FORCE */ false,
    /* ARMY_THEOCRACY                 */ false,
    /* ARMY_VOLDAN                    */ false,
    /* ARMY_KEWAC                     */ false,
    /* ARMY_KESIRU                    */ false,
    /* ARMY_ENEMY                     */ false,
    /* ARMY_BANDITS                   */ false,
};

void Utilities_Load() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Names_Load_All();
    Palettes_Load();
    Palette_Tables_Load();
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Utilities_Free() {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    Palettes_Free();
    Names_Free();
    Hashes_Free();
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void SOTA_Free_Surfaces(SDL_Surface **surfaces, int num) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    for (size_t i = 0; i < num; i++) {
        if (surfaces[i] == NULL)
            continue;

        SDL_FreeSurface(surfaces[i]);
        surfaces[i] = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void SOTA_Free_Textures(SDL_Texture **textures, int num) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    for (size_t i = 0; i < num; i++) {
        if (textures[i] == NULL)
            continue;

        SDL_DestroyTexture(textures[i]);
        textures[i] = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Weapons and items --- */
struct WpnorItem Utilities_WpnorItem(if16 id, struct dtab *weapons_dtab, struct dtab *items_dtab) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    SDL_assert(weapons_dtab != NULL);
    SDL_assert(items_dtab   != NULL);

    struct WpnorItem out = WpnorItem_default;
    out.dtab = dtabs_id[Item_Archetype(id)];
    SDL_assert(out.dtab > SOTA_DTAB_NULL);

    if (out.dtab == SOTA_DTAB_ITEMS) {
        Item_Load(items_dtab, id);
        out.item = DTAB_GET(items_dtab,    id);
    } else {
        Weapon_Load(weapons_dtab, id);
        out.wpn = DTAB_GET(weapons_dtab, id);
    }

    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}
