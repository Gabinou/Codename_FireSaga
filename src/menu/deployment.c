
#include "menu/deployment.h"

/* --- ELEMENTS --- */
struct MenuElemDirections dm_links[DM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* DM_ELEM_UNIT1 */ {DM_ELEM_NULL, DM_ELEM_NULL,  DM_ELEM_NULL, DM_ELEM_UNIT2},
    /* DM_ELEM_UNIT2 */ {DM_ELEM_NULL, DM_ELEM_UNIT1, DM_ELEM_NULL, DM_ELEM_UNIT3},
    /* DM_ELEM_UNIT3 */ {DM_ELEM_NULL, DM_ELEM_UNIT2, DM_ELEM_NULL, DM_ELEM_UNIT4},
    /* DM_ELEM_UNIT4 */ {DM_ELEM_NULL, DM_ELEM_UNIT3, DM_ELEM_NULL, DM_ELEM_UNIT5},
    /* DM_ELEM_UNIT5 */ {DM_ELEM_NULL, DM_ELEM_UNIT4, DM_ELEM_NULL, DM_ELEM_UNIT6},
    /* DM_ELEM_UNIT6 */ {DM_ELEM_NULL, DM_ELEM_UNIT5, DM_ELEM_NULL, DM_ELEM_UNIT7},
    /* DM_ELEM_UNIT7 */ {DM_ELEM_NULL, DM_ELEM_UNIT6, DM_ELEM_NULL, DM_ELEM_UNIT8},
    /* DM_ELEM_UNIT8 */ {DM_ELEM_NULL, DM_ELEM_UNIT7, DM_ELEM_NULL, DM_ELEM_NULL},
};

struct Point dm_elem_pos[DM_ELEM_NUM] = {
    /* DM_ELEM_UNIT1 */  {8,  21},
    /* DM_ELEM_UNIT2 */  {8,  39},
    /* DM_ELEM_UNIT3 */  {8,  57},
    /* DM_ELEM_UNIT4 */  {8,  75},
    /* DM_ELEM_UNIT5 */  {8,  93},
    /* DM_ELEM_UNIT6 */  {8, 111},
    /* DM_ELEM_UNIT7 */  {8, 129},
    /* DM_ELEM_UNIT8 */  {8, 147},
};

struct Point dm_elem_box[DM_ELEM_NUM] = {
    /* DM_ELEM_UNIT1 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT2 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT3 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT4 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT5 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT6 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT7 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* DM_ELEM_UNIT8 */  {SOTA_TILESIZE, SOTA_TILESIZE},
};


/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* --- Frame transforms --- */
static struct Point _Unit_Frame(i32 x, i32 y);
static struct Point _Page_Frame(i32 x, i32 y);


/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(struct DeploymentMenu *dm, SDL_Renderer   *r);

/* --- Drawing --- */
typedef void (*fsm_DeploymentMenu_Draw)(struct DeploymentMenu *, SDL_Renderer *);

static void _DeploymentMenu_Draw_Unit(      struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Names(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Icons(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Mount(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_PageNum(   struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Scroll_Bar(struct DeploymentMenu *dm, SDL_Renderer *r);

static void _DeploymentMenu_Draw_P1(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P2(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P3(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P4(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P1(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P2(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P3(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P4(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P1(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P2(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P3(struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P4(struct DeploymentMenu *dm, SDL_Renderer *r);

fsm_DeploymentMenu_Draw fsm_DeploymentMenu_Draw_Pages[DM_PAGE_NUM] = {
    &_DeploymentMenu_Draw_P1,
    &_DeploymentMenu_Draw_P2,
    &_DeploymentMenu_Draw_P3,
    &_DeploymentMenu_Draw_P4
};

struct DeploymentMenu DeploymentMenu_default = {
    .update             = true,
    .pos                = {-1, -1},

    ._party_size        =   0,
    ._selected_num      =   0,
    .select_max         =   0,
    .top_unit           =   0,   /* Up   - Down  scrolling [0, _party_size] */
    .page               =   0,   /* Left - Right scrolling [0, DM_PAGE_NUM]*/

    .party              = NULL,
    ._party_id_stack    = NULL,
    ._selected          = NULL,
    .map                = NULL,
    .font_wpns          = NULL,
    .pixelnours         = NULL,
    .pixelnours_16      = NULL,
    .pixelnours_big     = NULL,

    .texture            = NULL,
    .texture_dude       = NULL,
    .texture_mount      = NULL,
};

/* --- STATIC FUNCTIONS --- */
/* --- Frame transforms --- */
static struct Point _Unit_Frame(i32 x_menu, i32 y_menu) {
    /* Relative to menu frame */
    struct Point out;
    out.x = x_menu + DM_UNIT_FRAME_X;
    out.y = y_menu + DM_UNIT_FRAME_Y;

    return (out);
}

static struct Point _Page_Frame(i32 x_unit, i32 y_unit) {
    /* Relative to menu frame */
    struct Point out;
    out.x = x_unit + DM_UNIT_FRAME_X + DM_PAGE_FRAME_X;
    out.y = y_unit + DM_UNIT_FRAME_Y + DM_PAGE_FRAME_Y;

    return (out);
}

/* --- Constructors/Destructors --- */
static void _DeploymentMenu_Free_Icons(struct DeploymentMenu *dm) {

}

/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    // Load map units to put as side-icons

}
/* --- Utility --- */
void _DeploymentMenu_Swap_Unit(struct DeploymentMenu *dm, SDL_Renderer *renderer,
                               i16 unit) {
    if (dm->_selected[unit]) {
        _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
    } else {
        _DeploymentMenu_Swap(dm, renderer, NES_DARK_GRAY, NES_BLACK);
    }
}

void _DeploymentMenu_Swap(struct DeploymentMenu *dm, SDL_Renderer *renderer,
                          i8 white, i8 black) {
    PixelFont_Swap_Palette(dm->pixelnours,      renderer, white, black);
    PixelFont_Swap_Palette(dm->pixelnours_16,   renderer, white, black);
    PixelFont_Swap_Palette(dm->pixelnours_big,  renderer, white, black);
}

i32 _DeploymentMenu_Num(struct DeploymentMenu *dm) {
    SDL_Log("%d %d", dm->top_unit, dm->_party_size);
    SDL_assert(dm->top_unit < dm->_party_size);
    i32 num_to_draw  = dm->_party_size - dm->top_unit;
    return (num_to_draw < DM_UNIT_SHOWN_NUM ? num_to_draw : DM_UNIT_SHOWN_NUM);
}

/* --- Drawing --- */
static void _DeploymentMenu_Draw_P1(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P1(dm, renderer);
    _DeploymentMenu_Draw_Stats_P1(dm, renderer);
}

static void _DeploymentMenu_Draw_P2(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P2(dm, renderer);
    _DeploymentMenu_Draw_Stats_P2(dm, renderer);
}

static void _DeploymentMenu_Draw_P3(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P3(dm, renderer);
    _DeploymentMenu_Draw_Stats_P3(dm, renderer);
}

static void _DeploymentMenu_Draw_P4(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P4(dm, renderer);
    _DeploymentMenu_Draw_Stats_P4(dm, renderer);
    _DeploymentMenu_Draw_Mount(dm, renderer);
}

/* -- Headers -- */
static void _DeploymentMenu_Draw_PageNum(struct DeploymentMenu *dm,
                                         SDL_Renderer *renderer) {
    char array[2] = {0};
    int x = DM_PAGE_NUM_NUMER_X, y = DM_PAGE_NUM_NUMER_Y;
    stbsp_snprintf(array, 2, "%d\0", dm->page + 1);
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 1, x, y);

    x = DM_PAGE_NUM_SLASH_X, y = DM_PAGE_NUM_SLASH_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "/", 1, x, y);

    x = DM_PAGE_NUM_DENOM_X, y = DM_PAGE_NUM_DENOM_Y;
    stbsp_snprintf(array, 2, "%d\0", DM_PAGE_NUM);
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 1, x, y);
}

static void _DeploymentMenu_Draw_Headers_P1(struct DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {
    int x = DM_NAME_X, y = DM_NAME_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Name", 4, x, y);

    x = DM_CLASS_X, y = DM_CLASS_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Class", 5, x, y);

    x = DM_LVL_X, y = DM_LVL_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "Lvl", 3, x, y);

    x = DM_EXP_X, y = DM_EXP_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "EXP", 3, x, y);

    x = DM_HP_X, y = DM_HP_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "HP", 2, x, y);

    x = DM_MOVE_X, y = DM_MOVE_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "Mv", 2, x, y);
}

static void _DeploymentMenu_Draw_Headers_P2(struct DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {
    int x = DM_NAME_X, y = DM_NAME_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Name", 4, x, y);

    x = DM_STR_X, y = DM_STR_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "STR", 3, x, y);

    x = DM_MAG_X, y = DM_MAG_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "MAG", 3, x, y);

    x = DM_DEX_X, y = DM_DEX_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "DEX", 3, x, y);

    x = DM_AGI_X, y = DM_AGI_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "AGI", 3, x, y);

    x = DM_CON_X, y = DM_CON_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "CON", 3, x, y);
}

static void _DeploymentMenu_Draw_Headers_P3(struct DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {
    int x = DM_NAME_X, y = DM_NAME_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Name", 4, x, y);

    x = DM_DEF_X, y = DM_DEF_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "DEF", 3, x, y);

    x = DM_RES_X, y = DM_RES_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "RES", 3, x, y);

    x = DM_FTH_X, y = DM_FTH_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "FTH", 3, x, y);

    x = DM_LUCK_X, y = DM_LUCK_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "LUCK", 4, x, y);

    x = DM_PROF_X, y = DM_PROF_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "PROF", 4, x, y);
}

static void _DeploymentMenu_Draw_Headers_P4(struct DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {
    int x = DM_NAME_X, y = DM_NAME_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Name", 4, x, y);

    x = DM_WPN_TYPE_X, y = DM_WPN_TYPE_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "WPN TYPE", 8, x, y);

    x = DM_REGRETS_X, y = DM_REGRETS_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "REGRETS", 7, x, y);

    x = DM_MOUNT_X, y = DM_MOUNT_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "MOUNT", 5, x, y);
}

static void _DeploymentMenu_Draw_Unit_Num(struct DeploymentMenu *dm,
                                          SDL_Renderer *renderer) {


    int x = DM_UNIT_NUM_X, y = DM_UNIT_NUM_Y;
    char array[6] = {0};
    SDL_assert(dm->_selected_num >= 0);
    SDL_assert(dm->_selected_num < 99);
    SDL_assert(dm->select_max > 0);
    SDL_assert(dm->select_max < 99);
    stbsp_snprintf(array, 6, "%02d/%02d\0", dm->_selected_num, dm->select_max);
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, array, 5, x, y);
    int width = PixelFont_Width(dm->pixelnours_16,  array, 5);

    SDL_Rect dstrect = {
        .x = DM_UNIT_NUM_X + width / 2 + 1,
        .y = DM_DUDE_Y,
        .w = DM_DUDE_W,
        .h = DM_DUDE_H,
    };

    SDL_RenderCopy(renderer, dm->texture_dude, NULL, &dstrect);
}

static void _DeploymentMenu_Draw_Names(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    int x = DM_NAME_X, y = DM_NAME_CONTENT_Y;
    struct Point point = _Unit_Frame(x, y);

    for (i32 i = 0; i < num_to_draw; i++) {
        y = i * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->_party_id_stack[i + dm->top_unit];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        PixelFont_Write_Centered(dm->pixelnours_big, renderer, unit->name.data,
                                 unit->name.num, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
}

/* -- Page 1 -- */
static void _DeploymentMenu_Draw_Stats_P1(struct DeploymentMenu *dm,
                                          SDL_Renderer *renderer) {
    char array[3] = {0};
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    struct Point point;
    int unit_id, x, y;
    struct Unit *unit;
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        unit_id = dm->_party_id_stack[i + dm->top_unit];
        unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* - HP - */
        x = DM_HP_X, y = DM_HP_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%02d\0", unit->current_stats.hp);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Move - */
        x = DM_MOVE_X, y = DM_MOVE_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 2, "%01d\0", unit->current_stats.move);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Lvl - */
        x = DM_LVL_X, y = DM_LVL_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        i16 lvl = Unit_getLvl(unit);
        stbsp_snprintf(array, 3, "%02d\0", lvl);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - EXP - */
        x = DM_EXP_X, y = DM_EXP_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%02d\0", (unit->exp % SOTA_EXP_PER_LEVEL));
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Class - */
        x = DM_CLASS_X, y = DM_CLASS_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        s8 class = classNames[unit->class];
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, class.data,
                                 class.num, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
}

/* -- Page 2 -- */
static void _DeploymentMenu_Draw_Stats_P2(struct DeploymentMenu *dm,
                                          SDL_Renderer *renderer) {
    char array[3] = {0};
    int x = 0, y = 0;
    struct Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->_party_id_stack[i + dm->top_unit];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);


        /* STR */
        x = DM_STR_X;
        y = DM_STR_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.str);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* MAG */
        x = DM_MAG_X;
        y = DM_MAG_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.mag);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* DEX */
        x = DM_DEX_X;
        y = DM_DEX_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.dex);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* AGI */
        x = DM_AGI_X;
        y = DM_AGI_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.agi);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* CON */
        x = DM_CON_X;
        y = DM_CON_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.con);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
}

/* -- Page 3 -- */
static void _DeploymentMenu_Draw_Stats_P3(struct DeploymentMenu *dm,
                                          SDL_Renderer *renderer) {
    char array[3] = {0};
    int x = 0, y = 0;
    struct Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->_party_id_stack[i + dm->top_unit];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* DEF */
        x = DM_DEF_X;
        y = DM_DEF_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.def);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* RES */
        x = DM_RES_X;
        y = DM_RES_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.res);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* FTH */
        x = DM_FTH_X;
        y = DM_FTH_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.fth);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* LUCK */
        x = DM_LUCK_X;
        y = DM_LUCK_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.luck);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* PROF */
        x = DM_PROF_X;
        y = DM_PROF_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0", unit->current_stats.prof);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
}

/* -- Page 4 -- */
static void _DeploymentMenu_Draw_Stats_P4(struct DeploymentMenu *dm,
                                          SDL_Renderer *renderer) {
    char array[4] = {0};
    int x = 0, y = 0;
    struct Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->_party_id_stack[i + dm->top_unit];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* WPN_TYPES */
        u8 equippables[SM_WEAPON_TYPES_MAX] = {0};
        u8 equippable_num = Unit_Equippables(unit, equippables);
        x = DM_WPN_TYPE_X;
        y = DM_WPN_TYPE_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        PixelFont_Write_Centered(dm->font_wpns, renderer, equippables,
                                 equippable_num, x, y);

        /* REGRETS */
        x = DM_REGRETS_X;
        y = DM_REGRETS_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 4, "%d\0", unit->regrets);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 3, x, y);

        /* MOUNTS */
    }
    _DeploymentMenu_Swap(dm, renderer, NES_WHITE, NES_BLACK);
}

static void _DeploymentMenu_Draw_Icons(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    int x = DM_ICON_X, y = DM_ICON_Y;
    struct Point point = _Unit_Frame(x, y);
    SDL_Rect rect = {.x = 0, .y = 0, .w = DM_ICON_W, .h = DM_ICON_H};
    rect.x = point.x;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    for (i32 i = 0; i < num_to_draw; i++) {
        rect.y = i * DM_LINE_H + point.y;
        SDL_RenderFillRect(renderer, &rect);
    }
}

static void _DeploymentMenu_Draw_Unit(struct DeploymentMenu *dm,
                                      SDL_Renderer *renderer) {
    /* Draw map unit as side icon */
}

static void _DeploymentMenu_Draw_Mount(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    /* - preliminaries - */
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    SDL_Rect dstrect, srcrect;
    int mount_offset_x, mount_offset_y;
    int x = DM_MOUNT_X - SM_MOUNTS_TILESIZE / 2, y = DM_MOUNT_CONTENT_Y;
    struct Point point = _Page_Frame(x, y);

    for (i32 i = 0; i < num_to_draw; i++) {
        y = i * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->_party_id_stack[i + dm->top_unit];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);

        /* - Get mount type - */
        if (unit->mount == NULL)
            continue;
        i8 mount_type = unit->mount->type;
        // SDL_Log("mount_type %d", mount_type);
        // getchar();
        // if (mount_type == MOUNT_TYPE_NULL) {
        //     x = MOUNT_NONE_X_OFFSET, y = MOUNT_NONE_Y_OFFSET;
        //     PixelFont_Write(dm->pixelnours, renderer, "-", 1, x, y);
        //     return;
        // }

        switch (mount_type) {
            case MOUNT_TYPE_HORSE:
            case MOUNT_TYPE_SALAMANDER:
                mount_offset_x = 6;
                mount_offset_y = 2;
                break;
            case MOUNT_TYPE_PEGASUS:
            case MOUNT_TYPE_EAGLE:
                mount_offset_x =  0;
                mount_offset_y = -4;
                break;
        }

        srcrect.w = SM_MOUNTS_TILESIZE;
        srcrect.h = SM_MOUNTS_TILESIZE;
        srcrect.x = mount_type % SOTA_COL_LEN * srcrect.w;
        srcrect.y = mount_type / SOTA_COL_LEN * srcrect.h;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;
        // dstrect.x = 0;
        // dstrect.y = 0;
        dstrect.x = (x + mount_offset_x);
        dstrect.y = (y + mount_offset_y);
        SDL_RenderCopy(renderer, dm->texture_mount, &srcrect, &dstrect);
    }
}

static void _DeploymentMenu_Draw_Scroll_Bar(struct DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {

}

/* --- EXTERN FUNCTIONS --- */
struct DeploymentMenu *DeploymentMenu_Alloc(void) {
    struct DeploymentMenu *dm = SDL_malloc(sizeof(struct DeploymentMenu));
    SDL_assert(dm);
    *dm = DeploymentMenu_default;

    dm->pixelnours     = PixelFont_Alloc();
    dm->pixelnours_16  = PixelFont_Alloc();
    dm->pixelnours_16->glyph_height  = 16;
    dm->pixelnours_16->glyph_width   = 16;

    dm->pixelnours_big = PixelFont_Alloc();
    SDL_assert(dm->pixelnours     != NULL);
    SDL_assert(dm->pixelnours_16  != NULL);
    SDL_assert(dm->pixelnours_big != NULL);
    return (dm);
}

void DeploymentMenu_Free(struct DeploymentMenu *dm) {
    SDL_assert(dm != NULL);
    if (dm->texture != NULL) {
        SDL_DestroyTexture(dm->texture);
        dm->texture = NULL;
    }
    if (dm->pixelnours != NULL) {
        PixelFont_Free(dm->pixelnours, true);
        dm->pixelnours = NULL;
    }
    if (dm->pixelnours_16 != NULL) {
        PixelFont_Free(dm->pixelnours_16, true);
        dm->pixelnours_16 = NULL;
    }

    if (dm->pixelnours_big != NULL) {
        PixelFont_Free(dm->pixelnours_big, true);
        dm->pixelnours_big = NULL;
    }

    if (dm->font_wpns != NULL) {
        PixelFont_Free(dm->font_wpns, true);
        dm->font_wpns = NULL;
    }

    if (dm->texture_mount != NULL) {
        SDL_DestroyTexture(dm->texture_mount);
        dm->texture_mount = NULL;
    }

    if (dm->texture_dude != NULL) {
        SDL_DestroyTexture(dm->texture_dude);
        dm->texture_dude = NULL;
    }

    if (dm->_selected != NULL) {
        SDL_free(dm->_selected);
        dm->_selected = NULL;
    }

    if (dm != NULL) {
        SDL_free(dm);
        dm = NULL;
    }
}

void DeploymentMenu_Load(struct DeploymentMenu *dm, SDL_Renderer *renderer,
                         struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);
    n9patch->patch_pixels.x = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y = MENU_PATCH_PIXELS;
    n9patch->size_patches.x = DM_PATCH_X_SIZE;
    n9patch->size_patches.y = DM_PATCH_Y_SIZE;
    n9patch->scale.x        = DM_N9PATCH_SCALE_X;
    n9patch->scale.y        = DM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x  = (MENU_PATCH_PIXELS * DM_PATCH_X_SIZE);
    n9patch->size_pixels.y  = (MENU_PATCH_PIXELS * DM_PATCH_Y_SIZE);

    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }

    /* - loading fonts - */
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");

    PixelFont_Load(dm->pixelnours, renderer, path);
    dm->pixelnours->y_offset = pixelfont_y_offset;
    SDL_assert(dm->pixelnours);

    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(dm->pixelnours_big, renderer, path);
    dm->pixelnours_big->y_offset = pixelfont_big_y_offset;
    SDL_assert(dm->pixelnours_big);

    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_16_tight.png");
    PixelFont_Load(dm->pixelnours_16, renderer, path);
    SDL_assert(dm->pixelnours_big);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Weapons.png");
    dm->font_wpns = TextureFont_Alloc(2, 8);
    PixelFont_Load(dm->font_wpns, renderer, path);

    /* - loading mounts - */
    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Mount.png");
    dm->texture_mount = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(dm->texture_mount);

    /* - loading dude - */
    path = PATH_JOIN("..", "assets", "GUI", "Icon", "Icon_Dude.png");
    dm->texture_dude = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(dm->texture_dude);
}

void DeploymentMenu_Party_Set(struct DeploymentMenu *dm,
                              struct Unit *party, i16 *party_id_stack) {
    if (dm->_selected != NULL) {
        SDL_free(dm->_selected);
        dm->_selected = NULL;
    }
    dm->party           = party;
    dm->_party_size     = DARR_NUM(party_id_stack);
    dm->_party_id_stack = party_id_stack;
    dm->_selected_num   = 0;
    dm->_selected       = SDL_calloc(dm->_party_size, sizeof(*dm->_selected));
}


/* --- Scrolling --- */
void DeploymentMenu_Scroll_Up(   struct DeploymentMenu *dm) {
    if (dm->top_unit < 0);
    dm->top_unit = 0;
    if (dm->top_unit > (dm->_party_size - DM_UNIT_SHOWN_NUM));
    dm->top_unit = (dm->_party_size - DM_UNIT_SHOWN_NUM);

    if (dm->top_unit > 1)
        dm->top_unit -= DM_SCROLL_UP_ADD;

}

void DeploymentMenu_Scroll_Down( struct DeploymentMenu *dm) {
    if (dm->top_unit < 0);
    dm->top_unit = 0;
    if (dm->top_unit > (dm->_party_size - DM_UNIT_SHOWN_NUM));
    dm->top_unit = (dm->_party_size - DM_UNIT_SHOWN_NUM);

    if (dm->top_unit < (dm->_party_size - DM_UNIT_SHOWN_NUM - DM_SCROLL_UP_ADD))
        dm->top_unit += DM_SCROLL_UP_ADD;

}

void DeploymentMenu_Scroll_Left( struct DeploymentMenu *dm) {
    if (dm->page < 0)
        dm->page = 0;
    if (dm->page > DM_PAGE_NUM)
        dm->page = DM_PAGE_NUM - 1;

    if (dm->page > 1)
        dm->page--;
}

void DeploymentMenu_Scroll_Right(struct DeploymentMenu *dm) {
    if (dm->page < 0)
        dm->page = 0;
    if (dm->page > DM_PAGE_NUM)
        dm->page = DM_PAGE_NUM - 1;

    if (dm->page < (DM_PAGE_NUM - 1))
        dm->page++;
}

void DeploymentMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *renderer) {
    struct DeploymentMenu   *dm         = (struct DeploymentMenu *)mc->data;
    struct n9Patch          *n9patch    = &mc->n9patch;

    SDL_assert(dm != NULL);

    if (dm->update) {
        DeploymentMenu_Update(dm, n9patch, rt, renderer);
        dm->update = false;
    }

    /* TODO: set position of DeploymentMenu */
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = dm->pos.x,
        .y = dm->pos.y,
    };
    SDL_assert(dm->texture != NULL);
    SDL_RenderCopy(renderer, dm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

i32 DeploymentMenu_Elem_Move(struct Menu *menu, i32 direction) {
    /* -- Scrolling menu -- */
    direction = Ternary_Direction_Straight(direction);
    struct DeploymentMenu *dm = menu->data;
    int previous_top_unit = dm->top_unit;

    /* Scrolling up:    if unit1 and up */
    if ((menu->elem == DM_ELEM_UNIT1) && (direction == SOTA_DIRECTION_TOP)) {
        if (dm->top_unit > 0)
            dm->top_unit--;
    }
    /* Scrolling down:  if unit8 and down */
    if ((menu->elem == DM_ELEM_UNIT8) && (direction == SOTA_DIRECTION_BOTTOM)) {
        if (dm->top_unit < (dm->_party_size - DM_UNIT_SHOWN_NUM))
            dm->top_unit++;
    }

    /* If top_unit change update menu */
    dm->update = (previous_top_unit != dm->top_unit);

    return (Menu_Elem_Move(menu, direction));
}

void DeploymentMenu_Update(struct DeploymentMenu *dm, struct n9Patch *n9patch,
                           SDL_Texture *rt, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(renderer != NULL);
    SDL_assert(dm       != NULL);
    /* - variable declaration/ ants definition - */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x       > 0);
    SDL_assert(n9patch->scale.y       > 0);

    /* - create render target texture - */
    if (dm->texture == NULL) {
        int x  = n9patch->size_pixels.x, y = n9patch->size_pixels.y;
        dm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, x, y);
        SDL_assert(dm->texture != NULL);
        SDL_SetTextureBlendMode(dm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, dm->texture);
    SDL_assert(dm->texture != NULL);

    /* --- RENDERING DEPLOYMENT-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    fsm_DeploymentMenu_Draw_Pages[dm->page](dm, renderer);

    _DeploymentMenu_Draw_Scroll_Bar(dm, renderer);
    _DeploymentMenu_Draw_PageNum(dm, renderer);
    _DeploymentMenu_Draw_Names(dm, renderer);
    _DeploymentMenu_Draw_Unit_Num(dm, renderer);
    _DeploymentMenu_Draw_Icons(dm, renderer);

    Utilities_DrawColor_Reset(renderer);

    /* -- Finish -- */
    SDL_SetRenderTarget(renderer, rt);
    SDL_assert(dm->texture);
}
