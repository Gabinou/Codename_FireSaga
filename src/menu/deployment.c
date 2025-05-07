
#include "menu/deployment.h"
#include "pixelfonts.h"
#include "octant.h"
#include "map/map.h"
#include "game/game.h"
#include "menu/stats.h"
#include "menu/menu.h"
#include "utilities.h"
#include "filesystem.h"
#include "palette.h"
#include "platform.h"
#include "unit/party.h"
#include "unit/flags.h"
#include "unit/mount.h"
#include "unit/equipment.h"
#include "names.h"
#include "macros.h"
#include "stb_sprintf.h"

/* --- ELEMENTS --- */
MenuElemDirections dm_links[DM_ELEM_NUM] = {
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

Point dm_elem_pos[DM_ELEM_NUM] = {
    /* DM_ELEM_UNIT1 */  {8,  21},
    /* DM_ELEM_UNIT2 */  {8,  39},
    /* DM_ELEM_UNIT3 */  {8,  57},
    /* DM_ELEM_UNIT4 */  {8,  75},
    /* DM_ELEM_UNIT5 */  {8,  93},
    /* DM_ELEM_UNIT6 */  {8, 111},
    /* DM_ELEM_UNIT7 */  {8, 129},
    /* DM_ELEM_UNIT8 */  {8, 147},
};

Point dm_elem_box[DM_ELEM_NUM] = {
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
static Point _Unit_Frame(i32 x, i32 y);
static Point _Page_Frame(i32 x, i32 y);

/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(DeploymentMenu *dm, SDL_Renderer   *r);

/* --- Drawing --- */
typedef void (*fsm_DeploymentMenu_Draw)(DeploymentMenu *, SDL_Renderer *);

static void _DeploymentMenu_Draw_Unit(      DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Names(     DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Icons(     DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Mount(     DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_PageNum(   DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Scroll_Bar(DeploymentMenu *dm, SDL_Renderer *r);

static void _DeploymentMenu_Draw_P1(        DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P2(        DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P3(        DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_P4(        DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P1(  DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P2(  DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P3(  DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Stats_P4(  DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P1(DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P2(DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P3(DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Headers_P4(DeploymentMenu *dm, SDL_Renderer *r);

const fsm_DeploymentMenu_Draw fsm_DeploymentMenu_Draw_Pages[DM_PAGE_NUM] = {
    &_DeploymentMenu_Draw_P1,
    &_DeploymentMenu_Draw_P2,
    &_DeploymentMenu_Draw_P3,
    &_DeploymentMenu_Draw_P4
};

const DeploymentMenu DeploymentMenu_default = {
    .update             = true,
    .pos                = {-1, -1},
    .world              = NULL,

    ._party_size        =   0,
    ._selected_num      =   0,
    .select_max         =   0,
    .top_unit           =   0,   /* Up   - Down  scrolling [0, _party_size] */
    .page               =   0,   /* Left - Right scrolling [0, DM_PAGE_NUM]*/

    .party              = NULL,
    ._start_pos_i       = NULL,
    ._selected          = NULL,
    .map                = NULL,
    .font_wpns          = NULL,
    .pixelnours         = NULL,
    .pixelnours_16      = NULL,
    .pixelnours_big     = NULL,

    .texture            = NULL,
    .texture_dude       = NULL,
    .texture_mount      = NULL,
    .surface_mount      = NULL,

    .black              = SOTA_BLACK,
    .white              = SOTA_WHITE,
    .dark_gray          = SOTA_DARK_GRAY,
    .palette            = NULL,
    .start_pos_order1   = -1,
    .start_pos_order2   = -1,
};

/* --- STATIC FUNCTIONS --- */
/* --- Frame transforms --- */
static Point _Unit_Frame(i32 x_menu, i32 y_menu) {
    /* Relative to menu frame */
    Point out;
    out.x = x_menu + DM_UNIT_FRAME_X;
    out.y = y_menu + DM_UNIT_FRAME_Y;
    return (out);
}

static Point _Page_Frame(i32 x_unit, i32 y_unit) {
    /* Relative to menu frame */
    Point out;
    out.x = x_unit + DM_UNIT_FRAME_X + DM_PAGE_FRAME_X;
    out.y = y_unit + DM_UNIT_FRAME_Y + DM_PAGE_FRAME_Y;

    return (out);
}

/* --- Constructors/Destructors --- */
static void _DeploymentMenu_Free_Icons(DeploymentMenu *dm) {

}

/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    // Load map units to put as side-icons

}
/* --- Utility --- */
void _DeploymentMenu_Selected_Num(DeploymentMenu *dm) {
    dm->_selected_num = 0;
    for (i32 i = 0; i < dm->_party_size; i++) {
        dm->_selected_num += (dm->_selected[i] >= 0);
    }
}

i32 _DeploymentMenu_Unselected(DeploymentMenu *dm) {
    i32 out = DM_UNSELECTED, i = 0;
    while (i < dm->_party_size) {
        if (dm->_selected[i] == out) {
            out++;
            i = 0;
        } else {
            i++;
        }
    }

    return (out);
}

void _DeploymentMenu_Swap_Unit(DeploymentMenu *dm, SDL_Renderer *renderer,
                               i16 unit) {
    if (dm->_selected[unit] > DM_UNSELECTED) {
        _DeploymentMenu_Swap(dm, renderer, dm->white,       dm->black);
    } else {
        _DeploymentMenu_Swap(dm, renderer, dm->dark_gray,   dm->black);
    }
}

void _DeploymentMenu_Swap(DeploymentMenu *dm, SDL_Renderer *renderer,
                          i8 white, i8 black) {
    PixelFont_Swap_Palette(dm->pixelnours,      renderer, white, black);
    PixelFont_Swap_Palette(dm->pixelnours_16,   renderer, white, black);
    PixelFont_Swap_Palette(dm->pixelnours_big,  renderer, white, black);
    PixelFont_Swap_Palette(dm->font_wpns,       renderer, white, black);
    if (white == dm->dark_gray) {
        Palette_Colors_Swap(dm->palette,        renderer,
                            &dm->surface_mount, &dm->texture_mount,
                            dm->white,          dm->black,
                            dm->dark_gray,      dm->black);
    } else if (white == dm->white) {
        Palette_Colors_Swap(dm->palette,        renderer,
                            &dm->surface_mount, &dm->texture_mount,
                            dm->dark_gray,      dm->black,
                            dm->white,          dm->black);
    }
}

i32 _DeploymentMenu_Num(DeploymentMenu *dm) {
    SDL_assert(dm->_party_size > 0);
    SDL_assert(dm->top_unit < dm->_party_size);
    i32 out = dm->_party_size - dm->top_unit;
    return (out < DM_UNIT_SHOWN_NUM ? out : DM_UNIT_SHOWN_NUM);
}

/* --- Drawing --- */
static void _DeploymentMenu_Draw_P1(DeploymentMenu *dm, SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P1(dm, renderer);
    _DeploymentMenu_Draw_Stats_P1(dm, renderer);
}

static void _DeploymentMenu_Draw_P2(DeploymentMenu *dm, SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P2(dm, renderer);
    _DeploymentMenu_Draw_Stats_P2(dm, renderer);
}

static void _DeploymentMenu_Draw_P3(DeploymentMenu *dm, SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P3(dm, renderer);
    _DeploymentMenu_Draw_Stats_P3(dm, renderer);
}

static void _DeploymentMenu_Draw_P4(DeploymentMenu *dm, SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P4(dm, renderer);
    _DeploymentMenu_Draw_Stats_P4(dm, renderer);
    _DeploymentMenu_Draw_Mount(dm, renderer);
}

/* -- Headers -- */
static void _DeploymentMenu_Draw_PageNum(DeploymentMenu *dm, SDL_Renderer *renderer) {
    char array[8] = {0};
    int x = DM_PAGE_NUM_NUMER_X, y = DM_PAGE_NUM_NUMER_Y;
    stbsp_snprintf(array, 2, "%d\0\0\0\0", dm->page + 1);
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 1, x, y);

    x = DM_PAGE_NUM_SLASH_X, y = DM_PAGE_NUM_SLASH_Y;
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, "/", 1, x, y);

    x = DM_PAGE_NUM_DENOM_X, y = DM_PAGE_NUM_DENOM_Y;
    stbsp_snprintf(array, 2, "%d\0\0\0\0", DM_PAGE_NUM);
    PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 1, x, y);
}

static void _DeploymentMenu_Draw_Headers_P1(DeploymentMenu *dm, SDL_Renderer *renderer) {
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

static void _DeploymentMenu_Draw_Headers_P2(DeploymentMenu *dm, SDL_Renderer *renderer) {
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

static void _DeploymentMenu_Draw_Headers_P3(DeploymentMenu *dm,
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

static void _DeploymentMenu_Draw_Headers_P4(DeploymentMenu *dm, SDL_Renderer *renderer) {
    int x = DM_NAME_X, y = DM_NAME_Y;
    PixelFont_Write_Centered(dm->pixelnours_16, renderer, "Name", 4, x, y);

    x = DM_WPN_TYPE_X, y = DM_WPN_TYPE_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "WPN TYPE", 8, x, y);

    x = DM_REGRETS_X, y = DM_REGRETS_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "REGRETS", 7, x, y);

    x = DM_MOUNT_X, y = DM_MOUNT_Y;
    PixelFont_Write_Centered(dm->pixelnours, renderer, "MOUNT", 5, x, y);
}

static void _DeploymentMenu_Draw_Unit_Num(DeploymentMenu *dm, SDL_Renderer *renderer) {

    int x = DM_UNIT_NUM_X, y = DM_UNIT_NUM_Y;
    char array[8] = {0};
    SDL_assert(dm->_selected_num >= 0);
    SDL_assert(dm->_selected_num < 99);
    SDL_assert(dm->select_max > 0);
    SDL_assert(dm->select_max < 99);
    stbsp_snprintf(array, 6, "%02d/%02d\0\0\0\0", dm->_selected_num, dm->select_max);
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

static void _DeploymentMenu_Draw_Names(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    int x = DM_NAME_X, y = DM_NAME_CONTENT_Y;
    Point point = _Unit_Frame(x, y);

    for (i32 i = 0; i < num_to_draw; i++) {
        y = i * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party->id_stack[i + dm->top_unit];
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);
        SDL_assert(Unit_id(unit) > UNIT_ID_PC_START);
        SDL_assert(Unit_id(unit) < UNIT_ID_END);
        u64 order = *(u64 *)DTAB_GET(global_unitOrders, Unit_id(unit));
        s8 idname = global_unitNames[order];

        PixelFont_Write_Centered(dm->pixelnours_big, renderer, idname.data,
                                 idname.num, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

/* -- Page 1 -- */
static void _DeploymentMenu_Draw_Stats_P1(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    unsigned char array[8] = {0};
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    Point point;
    int unit_id, x, y;
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        SDL_assert(i + dm->top_unit < DARR_NUM(dm->party->id_stack));
        unit_id = dm->party->id_stack[i + dm->top_unit];
        SDL_assert((unit_id > UNIT_ID_PC_START) && (unit_id < UNIT_ID_PC_END));
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);

        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* - HP - */
        x = DM_HP_X, y = DM_HP_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        memset(array, 0, 8);
        stbsp_snprintf(array, 4, "%02d\0\0\0\0", unit->stats.current.hp);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Move - */
        x = DM_MOVE_X, y = DM_MOVE_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        memset(array, 0, 8);
        stbsp_snprintf(array, 3, "%01d\0\0\0\0", unit->stats.current.move);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Lvl - */
        x = DM_LVL_X, y = DM_LVL_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        i16 lvl = Unit_Level(unit);
        memset(array, 0, 8);
        stbsp_snprintf(array, 4, "%02d\0\0\0\0", lvl);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - EXP - */
        x = DM_EXP_X, y = DM_EXP_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        memset(array, 0, 8);
        stbsp_snprintf(array, 4, "%02d\0\0\0\0", Unit_Experience(unit));
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* - Class - */
        x = DM_CLASS_X, y = DM_CLASS_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        s8 class = classNames[Unit_Class(unit)];
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, class.data,
                                 class.num, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

tnecs_entity DeploymentMenu_Hovered_Entity(DeploymentMenu *dm, i32 elem) {
    int unit_id = dm->party->id_stack[elem + dm->top_unit];
    return (dm->party->entities[unit_id]);
}


/* -- Page 2 -- */
static void _DeploymentMenu_Draw_Stats_P2(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    char array[8] = {0};
    int x = 0, y = 0;
    Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party->id_stack[i + dm->top_unit];
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* STR */
        x = DM_STR_X;
        y = DM_STR_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.str);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* MAG */
        x = DM_MAG_X;
        y = DM_MAG_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.mag);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* DEX */
        x = DM_DEX_X;
        y = DM_DEX_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.dex);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* AGI */
        x = DM_AGI_X;
        y = DM_AGI_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.agi);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* CON */
        x = DM_CON_X;
        y = DM_CON_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.con);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

/* -- Page 3 -- */
static void _DeploymentMenu_Draw_Stats_P3(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    char array[8] = {0};
    int x = 0, y = 0;
    Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party->id_stack[i + dm->top_unit];
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* DEF */
        x = DM_DEF_X;
        y = DM_DEF_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.def);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* RES */
        x = DM_RES_X;
        y = DM_RES_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.res);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* FTH */
        x = DM_FTH_X;
        y = DM_FTH_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.fth);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* LUCK */
        x = DM_LUCK_X;
        y = DM_LUCK_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.luck);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);

        /* PROF */
        x = DM_PROF_X;
        y = DM_PROF_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        stbsp_snprintf(array, 3, "%d\0\0\0\0", unit->stats.current.prof);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

/* -- Page 4 -- */
static void _DeploymentMenu_Draw_Stats_P4(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    char array[8] = {0};
    int x = 0, y = 0;
    Point point;

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    for (i32 i = 0; i < num_to_draw; i++) {
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party->id_stack[i + dm->top_unit];
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);
        SDL_assert(unit != NULL);

        /* WPN_TYPES */
        u8 equippables[ITEM_TYPE_EXP_END] = {0};
        u8 equippable_num = Unit_canEquip_allTypes(unit, equippables);
        x = DM_WPN_TYPE_X;
        y = DM_WPN_TYPE_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        PixelFont_Write_Centered(dm->font_wpns, renderer, equippables,
                                 equippable_num, x, y);
        SDL_assert(unit != NULL);

        /* REGRETS */
        x = DM_REGRETS_X;
        y = DM_REGRETS_CONTENT_Y;
        point = _Page_Frame(x, y);
        y = i * DM_LINE_H + point.y;
        SDL_assert(unit != NULL);
        SDL_assert(Unit_Current_Regrets(unit) < REGRET_MAX);
        stbsp_snprintf(array, 4, "%d\0\0\0\0", Unit_Current_Regrets(unit));
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 3, x, y);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

static void _DeploymentMenu_Draw_Icons(DeploymentMenu *dm, SDL_Renderer *renderer) {
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    int x = DM_ICON_X, y = DM_ICON_Y;
    Point point = _Unit_Frame(x, y);
    SDL_Rect rect = {.x = 0, .y = 0, .w = DM_ICON_W, .h = DM_ICON_H};
    rect.x = point.x;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    for (i32 i = 0; i < num_to_draw; i++) {
        rect.y = i * DM_LINE_H + point.y;
        SDL_RenderFillRect(renderer, &rect);
    }
}

static void _DeploymentMenu_Draw_Unit(DeploymentMenu *dm,
                                      SDL_Renderer *renderer) {
    /* Draw map unit as side icon */
}

static void _DeploymentMenu_Draw_Mount(DeploymentMenu *dm, SDL_Renderer *renderer) {
    SDL_assert(dm->world != NULL);
    /* - preliminaries - */
    i32 num_to_draw = _DeploymentMenu_Num(dm);
    int x = DM_MOUNT_X - SM_MOUNTS_TILESIZE / 2, y = DM_MOUNT_CONTENT_Y;
    Point point = _Page_Frame(x, y);

    for (i32 i = 0; i < num_to_draw; i++) {
        y = i * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party->id_stack[i + dm->top_unit];
        tnecs_entity ent = dm->party->entities[unit_id];
        SDL_assert(ent > TNECS_NULL);
        struct Unit *unit = IES_GET_COMPONENT(dm->world, ent, Unit);
        SDL_assert(unit != NULL);
        _DeploymentMenu_Swap_Unit(dm, renderer, i + dm->top_unit);

        /* - TODO: Get mount type - */
        // if (unit->mount.ptr == NULL)
        // continue;
        // i8 mount_type = unit->mount.ptr->type;

        // switch (mount_type) {
        //     case MOUNT_HORSE:
        //     case MOUNT_SALAMANDER:
        //         mount_offset_x = 6;
        //         mount_offset_y = 2;
        //         break;
        //     case MOUNT_PEGASUS:
        //     case MOUNT_EAGLE:
        //         mount_offset_x =  0;
        //         mount_offset_y = -4;
        //         break;
        // }

        // srcrect.w = SM_MOUNTS_TILESIZE;
        // srcrect.h = SM_MOUNTS_TILESIZE;
        // srcrect.x = mount_type % SOTA_COL_LEN * srcrect.w;
        // srcrect.y = mount_type / SOTA_COL_LEN * srcrect.h;
        // dstrect.w = srcrect.w;
        // dstrect.h = srcrect.h;
        // dstrect.x = (x + mount_offset_x);
        // dstrect.y = (y + mount_offset_y);
        // SDL_RenderCopy(renderer, dm->texture_mount, &srcrect, &dstrect);
    }
    _DeploymentMenu_Swap(dm, renderer, dm->white, dm->black);
}

static void _DeploymentMenu_Draw_Scroll_Bar(DeploymentMenu *dm,
                                            SDL_Renderer *renderer) {

}

/* --- EXTERN FUNCTIONS --- */
DeploymentMenu *DeploymentMenu_Alloc(void) {
    DeploymentMenu *dm = SDL_malloc(sizeof(DeploymentMenu));
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

void DeploymentMenu_Free(DeploymentMenu *dm) {
    if (dm == NULL) {
        return;
    }
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

    if (dm->surface_mount != NULL) {
        SDL_FreeSurface(dm->surface_mount);
        dm->surface_mount = NULL;
    }

    if (dm->texture_dude != NULL) {
        SDL_DestroyTexture(dm->texture_dude);
        dm->texture_dude = NULL;
    }

    if (dm->_selected != NULL) {
        SDL_free(dm->_selected);
        dm->_selected = NULL;
    }
    if (dm->_start_pos_i != NULL) {
        SDL_free(dm->_start_pos_i);
        dm->_start_pos_i = NULL;
    }

    SDL_free(dm);
}

void DeploymentMenu_Load(DeploymentMenu *dm, SDL_Renderer *renderer,
                         struct n9Patch *n9patch) {
    dm->palette = palette_SOTA;

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

    path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    dm->font_wpns = TextureFont_Alloc(2, 8);
    PixelFont_Load(dm->font_wpns, renderer, path);

    /* - loading mounts - */
    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Mount.png");
    dm->surface_mount = Filesystem_Surface_Load(path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(dm->surface_mount != NULL);
    dm->texture_mount = SDL_CreateTextureFromSurface(renderer, dm->surface_mount);
    SDL_assert(dm->texture_mount != NULL);

    /* - loading dude - */
    path = PATH_JOIN("..", "assets", "GUI", "Icon", "Icon_Dude.png");
    dm->texture_dude = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(dm->texture_dude);
}

i32 DeploymentMenu_Map_Find_Pos(DeploymentMenu *dm, struct Map *map,
                                u8 col, u8 row) {
    i32 out = -1;
    for (int i = 0; i < dm->select_max; i++) {
        if ((col == map->start_pos.arr[i].x) && (row == map->start_pos.arr[i].y)) {
            out = i;
            break;
        }
    }
    SDL_assert((out >= 0) && (out < dm->select_max));
    return (DeploymentMenu_Map_StartPos(dm, out));
}

void DeploymentMenu_UnitOrder_Reset(DeploymentMenu *dm) {
    _DeploymentMenu_Selected_Num(dm);
    for (i32 start_order1 = 0; start_order1 < dm->_selected_num; start_order1++) {
        /* Check which unit is on start_order1 */
        i16 unit_order1 = dm->_start_pos_i[start_order1];
        /* Skip if both orders match */
        if (unit_order1 == start_order1)
            continue;

        /* unit2 is on order unit1 wants -> unit_order2 == start_order1 */
        i16 unit_order2 = start_order1;
        i32 start_order2 = dm->_selected[unit_order2];

        i16 unit_id1 = dm->party->id_stack[unit_order1];
        i16 unit_id2 = dm->party->id_stack[unit_order2];

        dm->party->id_stack[unit_order2]    = unit_id1;
        dm->party->id_stack[unit_order1]    = unit_id2;
        i32 selected1 = dm->_selected[unit_order1];
        i32 selected2 = dm->_selected[unit_order2];
        dm->_selected[unit_order1]          = selected2;
        dm->_selected[unit_order2]          = selected1;
        dm->_start_pos_i[start_order1]      = unit_order2;
        dm->_start_pos_i[start_order2]      = unit_order1;
    }
    dm->update = true;
}

void DeploymentMenu_Map_Swap(DeploymentMenu *dm) {
    SDL_assert(dm->_start_pos_i != NULL);
    SDL_assert(dm->start_pos_order1 >= 0);
    SDL_assert(dm->start_pos_order2 >= 0);
    if (dm->start_pos_order1 == dm->start_pos_order2) {
        dm->start_pos_order1 = -1;
        dm->start_pos_order2 = -1;
        return;
    }
    /* Actual swapping of the units */
    SDL_assert(dm->start_pos_order1 < dm->select_max);
    SDL_assert(dm->start_pos_order2 < dm->select_max);
    i32 unit_order1     = dm->_start_pos_i[dm->start_pos_order1];
    i32 unit_order2     = dm->_start_pos_i[dm->start_pos_order2];
    dm->_start_pos_i[dm->start_pos_order1] = unit_order2;
    dm->_start_pos_i[dm->start_pos_order2] = unit_order1;

    /* Swap the selected state too. */
    i32 start_order1    = dm->_selected[unit_order1];
    i32 start_order2    = dm->_selected[unit_order2];
    dm->_selected[unit_order1] = start_order1;
    dm->_selected[unit_order2] = start_order2;

    DeploymentMenu_Map_StartPos_Deselect(dm);
}

i32 DeploymentMenu_Map_StartPos(DeploymentMenu *dm, i32 candidate) {
    SDL_assert(dm->_start_pos_i != NULL);
    SDL_assert(candidate < dm->select_max);
    /* candidate in start_pos space */
    return (candidate);
}

void DeploymentMenu_Map_StartPos_Select(DeploymentMenu *dm, i32 candidate) {
    if (dm->start_pos_order1 < 0) {
        dm->start_pos_order1 = candidate;
    } else if (dm->start_pos_order2 < 0) {
        dm->start_pos_order2 = candidate;
    } else {
        SDL_assert(false);
    }
}

void DeploymentMenu_Map_StartPos_Deselect(DeploymentMenu *dm) {
    dm->start_pos_order1 = -1;
    dm->start_pos_order2 = -1;
}

void DeploymentMenu_Map_Set(DeploymentMenu *dm, struct Map *map) {
    SDL_assert(map              != NULL);
    SDL_assert(map->start_pos.arr   != NULL);
    dm->select_max = DARR_NUM(map->start_pos.arr);
    SDL_assert(dm->select_max > 0);

    if (dm->_start_pos_i != NULL)
        SDL_free(dm->_start_pos_i);

    dm->_start_pos_i = SDL_malloc(dm->select_max * sizeof(*dm->_start_pos_i));
    for (int i = 0; i < dm->select_max; i++)
        dm->_start_pos_i[i] = i;
}

void DeploymentMenu_Party_Set(DeploymentMenu *dm, struct Party *party) {
    if (dm->_selected != NULL) {
        SDL_free(dm->_selected);
        dm->_selected = NULL;
    }
    SDL_assert(DARR_NUM(party->entities) > 0);
    dm->party           = party;
    Party_ID_Stack(party);
    dm->_party_size     = Party_Size(party);
    SDL_assert(dm->_party_size > 0);
    SDL_assert(dm->_party_size < SOTA_MAX_PARTY_SIZE);
    dm->_selected_num   = 0;
    dm->top_unit        = 0;
    dm->_selected       = SDL_calloc(dm->_party_size, sizeof(*dm->_selected));
    for (int i = 0; i < dm->_party_size; i++) {
        dm->_selected[i] = DM_UNSELECTED;
    }
}

/* --- Scrolling --- */
void DeploymentMenu_Scroll_Up(   DeploymentMenu *dm) {
    if (dm->top_unit < 0) {
        dm->top_unit = 0;
    }

    if (dm->top_unit > (dm->_party_size - DM_UNIT_SHOWN_NUM)) {
        dm->top_unit = (dm->_party_size - DM_UNIT_SHOWN_NUM);
    }

    if (dm->top_unit > 1) {
        dm->top_unit -= DM_SCROLL_UP_ADD;
    }

}

void DeploymentMenu_Scroll_Down( DeploymentMenu *dm) {
    if (dm->top_unit < 0) {
        dm->top_unit = 0;
    }
    if (dm->top_unit > (dm->_party_size - DM_UNIT_SHOWN_NUM)) {
        dm->top_unit = (dm->_party_size - DM_UNIT_SHOWN_NUM);

    }

    if (dm->top_unit < (dm->_party_size - DM_UNIT_SHOWN_NUM - DM_SCROLL_UP_ADD)) {
        dm->top_unit += DM_SCROLL_UP_ADD;
    }
}

void DeploymentMenu_Scroll_Left( DeploymentMenu *dm) {
    if (dm->page < 0)
        dm->page = 0;
    if (dm->page > DM_PAGE_NUM)
        dm->page = DM_PAGE_NUM - 1;

    if (dm->page > 1)
        dm->page--;
}

void DeploymentMenu_Scroll_Right(DeploymentMenu *dm) {
    if (dm->page < 0)
        dm->page = 0;
    if (dm->page > DM_PAGE_NUM)
        dm->page = DM_PAGE_NUM - 1;

    if (dm->page < (DM_PAGE_NUM - 1))
        dm->page++;
}

/* --- Elements --- */
void DeploymentMenu_Elem_Links(DeploymentMenu *dm, struct Menu *mc) {
    /* Get number of elements for the menu */

    i32 num_to_draw = _DeploymentMenu_Num(dm);
    mc->elem_num    = num_to_draw;
    mc->elem_links  = dm_links;

    for (i32 i = 0; i < num_to_draw; i++) {
        /* Link to elem top */
        if (i > 0)
            mc->elem_links[i].top       = i - 1;
        else
            mc->elem_links[i].top       = DM_ELEM_NULL;
        /* Link to elem bottom */
        if (i < (num_to_draw - 1))
            mc->elem_links[i].bottom    = i + 1;
        else
            mc->elem_links[i].bottom    = DM_ELEM_NULL;
    }
}


void DeploymentMenu_Elem_Pos(DeploymentMenu *dm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */
    SDL_assert(mc->n9patch.scale.x > 0);
    SDL_assert(mc->n9patch.scale.y > 0);

    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME)
        return;

    for (size_t i = 0; i < mc->elem_num; i++) {
        i32 scale_x = mc->n9patch.scale.x;
        i32 scale_y = mc->n9patch.scale.y;
        i32 x       = dm->pos.x + mc->n9patch.pos.x;
        i32 y       = dm->pos.y + mc->n9patch.pos.y;
        i32 elem_x  = mc->elem_pos[i].x;
        i32 elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = x + elem_x * scale_x;
        mc->elem_pos[i].y = y + elem_y * scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;
}

void DeploymentMenu_Elem_Pos_Revert(DeploymentMenu *dm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_MENU_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        i32 scale_x = mc->n9patch.scale.x;
        i32 scale_y = mc->n9patch.scale.y;
        i32 x       = dm->pos.x + mc->n9patch.pos.x;
        i32 y       = dm->pos.y + mc->n9patch.pos.y;
        i32 elem_x  = mc->elem_pos[i].x;
        i32 elem_y  = mc->elem_pos[i].y;
        mc->elem_pos[i].x = (elem_x - x) / scale_x;
        mc->elem_pos[i].y = (elem_y - y) / scale_y;
    }

    mc->elem_pos_frame = ELEM_POS_MENU_FRAME;
}

/* --- Selection --- */
i32 DeploymentMenu_Select(DeploymentMenu *dm, i32 elem) {
    /* Get unit order from elem */
    i32 unit_order = dm->top_unit + elem;

    if (dm->_selected[unit_order] <= DM_UNSELECTED) {
        /* Skip if deployment slots are full */
        _DeploymentMenu_Selected_Num(dm);
        if (dm->_selected_num >= dm->select_max)
            return (DM_UNSELECTED);
        /* Set previously unselected unit to selected */
        i32 unselected = _DeploymentMenu_Unselected(dm);
        SDL_assert(unselected < dm->select_max);
        SDL_assert(unselected >= 0);
        dm->_selected[unit_order]       = unselected;
        dm->_start_pos_i[unselected]    = unit_order;
    } else {
        /* Revert previously selected unit to unselected*/
        dm->_selected[unit_order] = DM_UNSELECTED;
    }
    dm->update = true;

    return (unit_order);
}

/* --- Drawing --- */
void DeploymentMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *renderer) {

    DeploymentMenu   *dm         = (DeploymentMenu *)mc->data;
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
    DeploymentMenu *dm = menu->data;
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
    dm->update = (previous_top_unit != dm->top_unit);

    /* Left:  Previous Page */
    if (direction == SOTA_DIRECTION_LEFT) {
        dm->page = dm->page > 1 ? dm->page - 1 : 0;
        dm->update = true;
    }
    if (direction == SOTA_DIRECTION_RIGHT) {
        dm->page = dm->page < (DM_PAGE_NUM - 1) ? dm->page + 1 : DM_PAGE_NUM - 1;
        dm->update = true;
    }

    /* Right: Next Page */

    /* If top_unit change update menu */

    return (Menu_Elem_Move(menu, direction));
}

void DeploymentMenu_Update(DeploymentMenu *dm, struct n9Patch *n9patch,
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

    _DeploymentMenu_Selected_Num(dm);
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
