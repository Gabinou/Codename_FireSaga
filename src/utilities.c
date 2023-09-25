
#include "utilities.h"
#include "stb_sprintf.h"

#ifndef __clang__
/* --- Debug --- */
void Utilities_stacktrace() {
    /* Depends on glibc. Need '-rdynamic' linker options. */
    void *array[10];
    size_t size = backtrace(array, 10);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
}
#endif  /* __clang__ */

/* --- Array --- */
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
    /* Note: Pitch is the width of the memory buffer,
    it can be different of the surface width. */
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

i64 Utilities_Frame_Delay(u64 elapsedTime_ns, uf8 fps_cap, bool fastforward) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i64 delay = 0;
    /* 0 frame delay if fast-forwarding */
    if (fastforward) {
        SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
        return (delay);
    }

    if (((elapsedTime_ns * fps_cap) < 1e9) || (elapsedTime_ns == 0)) {
        delay = floor(1000.0f / fps_cap - elapsedTime_ns / 1e6);
    }
    SDL_assert(delay >= 0);
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (delay);
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
