
#include "menu/deployment.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* --- Frame transforms --- */
static struct Point _Unit_Frame(i32 x, i32 y);
static struct Point _Page_Frame(i32 x, i32 y);

/* --- Loading --- */
static void _DeploymentMenu_Load_Icons(struct DeploymentMenu *dm, SDL_Renderer   *r);

/* --- Drawing --- */
typedef void (*fsm_DeploymentMenu_Draw)(struct DeploymentMenu *, SDL_Renderer *);

static void _DeploymentMenu_Draw_Mv(        struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_HP(        struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_EXP(       struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Lvl(       struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Unit(      struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Names(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Class(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Icons(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Mount(     struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Weapons(   struct DeploymentMenu *dm, SDL_Renderer *r);
static void _DeploymentMenu_Draw_Content(   struct DeploymentMenu *dm, SDL_Renderer *r);
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

fsm_DeploymentMenu_Draw fsm_DeploymentMenu_Draw_Pages[DM_PAGE_NUM] = {
    &_DeploymentMenu_Draw_P1,
    &_DeploymentMenu_Draw_P2,
    &_DeploymentMenu_Draw_P3,
    &_DeploymentMenu_Draw_P4
};

struct DeploymentMenu DeploymentMenu_default = {
    .update         = true,
    .pos            = {-1, -1},

    .party_size     =   0,
    .top_unit       =   0,   /* Up   - Down  scrolling [0, party_size] */
    .page           =   0,   /* Left - Right scrolling [0, DM_PAGE_NUM]*/

    .party          = NULL,
    .map            = NULL,
    .font_wpns      = NULL,
    .pixelnours     = NULL,
    .pixelnours_16  = NULL,
    .pixelnours_big = NULL,

    .texture        = NULL,
    .texture_mount  = NULL,
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

}

/* --- Drawing --- */
static void _DeploymentMenu_Draw_P1(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P1(dm, renderer);
    _DeploymentMenu_Draw_Class(dm, renderer);
    _DeploymentMenu_Draw_Lvl(dm, renderer);
    _DeploymentMenu_Draw_EXP(dm, renderer);
    _DeploymentMenu_Draw_HP(dm, renderer);
    _DeploymentMenu_Draw_Mv(dm, renderer);
}

static void _DeploymentMenu_Draw_P2(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P2(dm, renderer);
}

static void _DeploymentMenu_Draw_P3(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P3(dm, renderer);
}

static void _DeploymentMenu_Draw_P4(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {
    _DeploymentMenu_Draw_Headers_P4(dm, renderer);
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

static void _DeploymentMenu_Draw_Names(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    i32 num_to_draw = dm->party_size - dm->top_unit;
    int x = DM_NAME_X, y = DM_NAME_CONTENT_Y;
    struct Point point = _Unit_Frame(x, y);

    for (i32 i = dm->top_unit; i < num_to_draw; i++) {
        y = (i - dm->top_unit) * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party_stack[i];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);

        PixelFont_Write_Centered(dm->pixelnours_big, renderer, unit->name.data,
                                 unit->name.num, x, y);
    }
}

/* -- Page 1 -- */
static void _DeploymentMenu_Draw_Mv(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {

}

static void _DeploymentMenu_Draw_HP(struct DeploymentMenu *dm,
                                    SDL_Renderer *renderer) {

}

static void _DeploymentMenu_Draw_EXP(struct DeploymentMenu *dm,
                                     SDL_Renderer *renderer) {

}

static void _DeploymentMenu_Draw_Lvl(struct DeploymentMenu *dm,
                                     SDL_Renderer *renderer) {
    char array[3] = {0};
    int x = DM_LVL_X, y = DM_LVL_CONTENT_Y;
    struct Point point = _Page_Frame(x, y);

    i32 num_to_draw = dm->party_size - dm->top_unit;
    for (i32 i = dm->top_unit; i < num_to_draw; i++) {
        y = (i - dm->top_unit) * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party_stack[i];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        i16 lvl = Unit_getLvl(unit);
        stbsp_snprintf(array, 3, "%02d\0", lvl);
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, array, 2, x, y);
    }
}

static void _DeploymentMenu_Draw_Icons(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    i32 num_to_draw = dm->party_size - dm->top_unit;
    int x = DM_ICON_X, y = DM_ICON_Y;
    struct Point point = _Unit_Frame(x, y);
    SDL_Rect rect = {.x = 0, .y = 0, .w = DM_ICON_W, .h = DM_ICON_H};
    rect.x = point.x;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    for (i32 i = dm->top_unit; i < num_to_draw; i++) {
        rect.y = (i - dm->top_unit) * DM_LINE_H + point.y;
        SDL_RenderFillRect(renderer, &rect);
    }
}
static void _DeploymentMenu_Draw_Class(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {
    i32 num_to_draw = dm->party_size - dm->top_unit;
    int x = DM_CLASS_X, y = DM_CLASS_CONTENT_Y;
    struct Point point = _Page_Frame(x, y);

    for (i32 i = dm->top_unit; i < num_to_draw; i++) {
        y = (i - dm->top_unit) * DM_LINE_H + point.y;
        SDL_assert(dm->party != NULL);
        int unit_id = dm->party_stack[i];
        struct Unit *unit = &dm->party[unit_id];
        SDL_assert(unit != NULL);
        s8 class = classNames[unit->class];
        PixelFont_Write_Centered(dm->pixelnours_big, renderer, class.data,
                                 class.num, x, y);
    }
}

static void _DeploymentMenu_Draw_Unit(struct DeploymentMenu *dm,
                                      SDL_Renderer *renderer) {

}

static void _DeploymentMenu_Draw_Mount(struct DeploymentMenu *dm,
                                       SDL_Renderer *renderer) {

}
static void _DeploymentMenu_Draw_Weapons(struct DeploymentMenu *dm,
                                         SDL_Renderer *renderer) {

}
static void _DeploymentMenu_Draw_Content(struct DeploymentMenu *dm,
                                         SDL_Renderer *renderer) {

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
}


/* --- Scrolling --- */
void DeploymentMenu_Scroll_Up(   struct DeploymentMenu *dm) {
    if (dm->top_unit < 0);
    dm->top_unit = 0;
    if (dm->top_unit > (dm->party_size - DM_UNIT_SHOWN_NUM));
    dm->top_unit = (dm->party_size - DM_UNIT_SHOWN_NUM);

    if (dm->top_unit > 1)
        dm->top_unit -= DM_SCROLL_UP_ADD;

}

void DeploymentMenu_Scroll_Down( struct DeploymentMenu *dm) {
    if (dm->top_unit < 0);
    dm->top_unit = 0;
    if (dm->top_unit > (dm->party_size - DM_UNIT_SHOWN_NUM));
    dm->top_unit = (dm->party_size - DM_UNIT_SHOWN_NUM);

    if (dm->top_unit < (dm->party_size - DM_UNIT_SHOWN_NUM - DM_SCROLL_UP_ADD))
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

    _DeploymentMenu_Draw_PageNum(dm, renderer);
    _DeploymentMenu_Draw_Names(dm, renderer);
    _DeploymentMenu_Draw_Icons(dm, renderer);

    Utilities_DrawColor_Reset(renderer);
    /* -- Finish -- */
    SDL_SetRenderTarget(renderer, rt);
    SDL_assert(dm->texture);
}
