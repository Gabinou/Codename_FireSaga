
#include "SDL.h"
#include "graph.h"
#include "macros.h"
#include "n9patch.h"
#include "platform.h"
#include "filesystem.h"
#include "pixelfonts.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/growths.h"

#include "unit/unit.h"
#include "unit/bonus.h"

#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static void _GrowthsMenu_Draw_Talk(    struct GrowthsMenu *g, SDL_Renderer *r);
static void _GrowthsMenu_Draw_Graph(   struct GrowthsMenu *g, struct n9Patch *n9patch,
                                       SDL_Texture *render_target, SDL_Renderer *r);
static void _GrowthsMenu_Draw_Growths( struct GrowthsMenu *g, SDL_Renderer *r);
static void _GrowthsMenu_Draw_Supports(struct GrowthsMenu *g, SDL_Renderer *r);

const struct GrowthsMenu GrowthsMenu_default = {
    .update         = true,
    .update_stats   = true,
    // only record menu pos. other pos are computed.
    .pos            = {-1, -1},
};

const char GM_description[GM_ELEM_NUM][GM_BUFFER_LEN] = {
    /* GM_TALK */     {"a"},
    /* GM_SUPPORTS */ {"a"},
    /* GM_SUPPORT */  {"a"},
    /* GM_ATK */      {"a"},
    /* GM_PROT */     {"a"},
    /* GM_HIT */      {"a"},
    /* GM_CRIT */     {"a"},
    /* GM_GRAPH */    {"a"},
    /* GM_GROWTHS */  {"a"},
    /* GM_MOVE */     {"a"},
    /* GM_STR */      {"a"},
    /* GM_DEX */      {"a"},
    /* GM_DEF */      {"a"},
    /* GM_FTH */      {"a"},
    /* GM_CON */      {"a"},
    /* GM_MAG */      {"a"},
    /* GM_AGI */      {"a"},
    /* GM_RES */      {"a"},
    /* GM_LUCK */     {"a"},
    /* GM_PROF */     {"a"},
};

const struct Point GM_cursor_box[GM_ELEM_NUM] = {
    /* GM_TALK */     {GM_TALK_X_OFFSET,    GM_TALK_Y_OFFSET},
    /* GM_SUPPORTS */ {GM_SUPPORTS_X,       GM_SUPPORTS_Y},
    /* GM_SUPPORT */  {GM_BONUS_X,          GM_BONUS_Y},
    /* GM_SPEED */    {SOTA_TILESIZE,       SOTA_TILESIZE},
    /* GM_ATK */      {GM_ATK_X_OFFSET,     GM_ATK_Y_OFFSET},
    /* GM_PROT */     {GM_DEF_X_OFFSET,     GM_PROT_Y_OFFSET},
    /* GM_HIT */      {GM_HIT_X_OFFSET,     GM_HIT_Y_OFFSET},
    /* GM_CRIT */     {GM_CRIT_X_OFFSET,    GM_CRIT_Y_OFFSET},
    /* GM_GRAPH */    {GM_OFFSET_GRAPH_X,   GM_OFFSET_GRAPH_X},
    /* GM_GROWTHS */  {GM_MOVE_X_OFFSET,    GM_MOVE_Y_OFFSET},
    /* GM_MOVE */     {GM_MOVE_X_OFFSET,    GM_MOVE_Y_OFFSET},
    /* GM_STR */      {GM_STR_X_OFFSET,     GM_STR_Y_OFFSET},
    /* GM_DEX */      {GM_DEX_X_OFFSET,     GM_DEX_Y_OFFSET},
    /* GM_DEF */      {GM_DEF_X_OFFSET,     GM_DEF_Y_OFFSET},
    /* GM_FTH */      {GM_FTH_X_OFFSET,     GM_FTH_Y_OFFSET},
    /* GM_CON */      {GM_CON_X_OFFSET,     GM_CON_Y_OFFSET},
    /* GM_MAG */      {GM_MAG_X_OFFSET,     GM_MAG_Y_OFFSET},
    /* GM_AGI */      {GM_AGI_X_OFFSET,     GM_AGI_Y_OFFSET},
    /* GM_RES */      {GM_RES_X_OFFSET,     GM_RES_Y_OFFSET},
    /* GM_LUCK */     {GM_LUCK_X_OFFSET,    GM_LUCK_Y_OFFSET},
    /* GM_PROF */     {GM_PROF_X_OFFSET,    GM_PROF_Y_OFFSET},
};

const struct Point GM_cursor_pos[GM_ELEM_NUM] = {
    /* GM_TALK */     {GM_TALK_X_OFFSET - 20,  GM_TALK_Y_OFFSET},
    /* GM_SUPPORTS */ {GM_SUPPORTS_X    - 15,  GM_SUPPORTS_Y},
    /* GM_SUPPORT */  {GM_BONUS_X       - 13,  GM_BONUS_Y},
    /* GM_SPEED */    {GM_SPEED_X_OFFSET,      GM_ATK_Y_OFFSET},
    /* GM_ATK */      {GM_ATK_X_OFFSET  - 6,   GM_ATK_Y_OFFSET},
    /* GM_PROT */     {GM_DEF_X_OFFSET  - 3,   GM_PROT_Y_OFFSET},
    /* GM_HIT */      {GM_HIT_X_OFFSET  - 6,   GM_HIT_Y_OFFSET},
    /* GM_CRIT */     {GM_CRIT_X_OFFSET - 4,   GM_CRIT_Y_OFFSET},
    /* GM_GRAPH */    {GM_OFFSET_GRAPH_X,      GM_OFFSET_STATS_GM_Y + 12},
    /* GM_GROWTHS */  {GM_TALK_X_OFFSET - 22,  GM_OFFSET_STATS_GM_Y},
    /* GM_MOVE */     {GM_TALK_X_OFFSET - 22,  GM_MOVE_Y_OFFSET     - 16},
    /* GM_STR */      {GM_STR_X_OFFSET,        GM_STR_Y_OFFSET},
    /* GM_DEX */      {GM_DEX_X_OFFSET,        GM_DEX_Y_OFFSET},
    /* GM_DEF */      {GM_DEF_X_OFFSET,        GM_DEF_Y_OFFSET},
    /* GM_FTH */      {GM_FTH_X_OFFSET,        GM_FTH_Y_OFFSET},
    /* GM_CON */      {GM_CON_X_OFFSET,        GM_CON_Y_OFFSET},
    /* GM_MAG */      {GM_MAG_X_OFFSET,        GM_MAG_Y_OFFSET},
    /* GM_AGI */      {GM_AGI_X_OFFSET,        GM_AGI_Y_OFFSET},
    /* GM_RES */      {GM_RES_X_OFFSET,        GM_RES_Y_OFFSET},
    /* GM_LUCK */     {GM_LUCK_X_OFFSET,       GM_LUCK_Y_OFFSET},
    /* GM_PROF */     {GM_PROF_X_OFFSET,       GM_PROF_Y_OFFSET},
};

struct Point gm_elem_box[GM_ELEM_NUM] = {
    /* GM_TALK */     {GM_TALK_X_OFFSET,    GM_TALK_Y_OFFSET},
    /* GM_SUPPORTS */ {GM_SUPPORTS_X,       GM_SUPPORTS_Y},
    /* GM_SUPPORT */  {GM_BONUS_X,          GM_BONUS_Y},
    /* GM_SPEED */    {SOTA_TILESIZE,       SOTA_TILESIZE},
    /* GM_ATK */      {GM_ATK_X_OFFSET,     GM_ATK_Y_OFFSET},
    /* GM_PROT */     {GM_DEF_X_OFFSET,     GM_PROT_Y_OFFSET},
    /* GM_HIT */      {GM_HIT_X_OFFSET,     GM_HIT_Y_OFFSET},
    /* GM_CRIT */     {GM_CRIT_X_OFFSET,    GM_CRIT_Y_OFFSET},
    /* GM_GRAPH */    {GM_OFFSET_GRAPH_X,   GM_OFFSET_GRAPH_X},
    /* GM_GROWTHS */  {GM_MOVE_X_OFFSET,    GM_MOVE_Y_OFFSET},
    /* GM_MOVE */     {GM_MOVE_X_OFFSET,    GM_MOVE_Y_OFFSET},
    /* GM_STR */      {GM_STR_X_OFFSET,     GM_STR_Y_OFFSET},
    /* GM_DEX */      {GM_DEX_X_OFFSET,     GM_DEX_Y_OFFSET},
    /* GM_DEF */      {GM_DEF_X_OFFSET,     GM_DEF_Y_OFFSET},
    /* GM_FTH */      {GM_FTH_X_OFFSET,     GM_FTH_Y_OFFSET},
    /* GM_CON */      {GM_CON_X_OFFSET,     GM_CON_Y_OFFSET},
    /* GM_MAG */      {GM_MAG_X_OFFSET,     GM_MAG_Y_OFFSET},
    /* GM_AGI */      {GM_AGI_X_OFFSET,     GM_AGI_Y_OFFSET},
    /* GM_RES */      {GM_RES_X_OFFSET,     GM_RES_Y_OFFSET},
    /* GM_LUCK */     {GM_LUCK_X_OFFSET,    GM_LUCK_Y_OFFSET},
    /* GM_PROF */     {GM_PROF_X_OFFSET,    GM_PROF_Y_OFFSET},
};

struct Point gm_elem_pos[GM_ELEM_NUM] = {
    /* GM_TALK */     {GM_TALK_X_OFFSET - 20,  GM_TALK_Y_OFFSET},
    /* GM_SUPPORTS */ {GM_SUPPORTS_X    - 15,  GM_SUPPORTS_Y},
    /* GM_SUPPORT */  {GM_BONUS_X       - 13,  GM_BONUS_Y},
    /* GM_SPEED */    {GM_SPEED_X_OFFSET,      GM_ATK_Y_OFFSET},
    /* GM_ATK */      {GM_ATK_X_OFFSET  - 6,   GM_ATK_Y_OFFSET},
    /* GM_PROT */     {GM_DEF_X_OFFSET  - 3,   GM_PROT_Y_OFFSET},
    /* GM_HIT */      {GM_HIT_X_OFFSET  - 6,   GM_HIT_Y_OFFSET},
    /* GM_CRIT */     {GM_CRIT_X_OFFSET - 4,   GM_CRIT_Y_OFFSET},
    /* GM_GRAPH */    {GM_OFFSET_GRAPH_X,      GM_OFFSET_STATS_GM_Y + 12},
    /* GM_GROWTHS */  {GM_TALK_X_OFFSET - 22,  GM_OFFSET_STATS_GM_Y},
    /* GM_MOVE */     {GM_TALK_X_OFFSET - 22,  GM_MOVE_Y_OFFSET     - 16},
    /* GM_STR */      {GM_STR_X_OFFSET,        GM_STR_Y_OFFSET},
    /* GM_DEX */      {GM_DEX_X_OFFSET,        GM_DEX_Y_OFFSET},
    /* GM_DEF */      {GM_DEF_X_OFFSET,        GM_DEF_Y_OFFSET},
    /* GM_FTH */      {GM_FTH_X_OFFSET,        GM_FTH_Y_OFFSET},
    /* GM_CON */      {GM_CON_X_OFFSET,        GM_CON_Y_OFFSET},
    /* GM_MAG */      {GM_MAG_X_OFFSET,        GM_MAG_Y_OFFSET},
    /* GM_AGI */      {GM_AGI_X_OFFSET,        GM_AGI_Y_OFFSET},
    /* GM_RES */      {GM_RES_X_OFFSET,        GM_RES_Y_OFFSET},
    /* GM_LUCK */     {GM_LUCK_X_OFFSET,       GM_LUCK_Y_OFFSET},
    /* GM_PROF */     {GM_PROF_X_OFFSET,       GM_PROF_Y_OFFSET},
};

struct n4Directions gm_links[GM_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* GM_TALK */     {GM_SUPPORTS,  GM_ELEM_NULL, GM_ELEM_NULL, GM_GROWTHS},
    /* GM_SUPPORTS */ {GM_SUPPORT,   GM_ELEM_NULL, GM_TALK,      GM_GROWTHS},
    /* GM_SUPPORT */  {GM_ELEM_NULL, GM_ELEM_NULL, GM_SUPPORTS,  GM_ATK},
    /* GM_SPEED */    {GM_ELEM_NULL, GM_SUPPORT,   GM_PROT,      GM_GROWTHS},
    /* GM_ATK */      {GM_PROT,      GM_SUPPORT,   GM_SUPPORTS,  GM_HIT},
    /* GM_PROT */     {GM_SPEED,     GM_SUPPORT,   GM_ATK,       GM_CRIT},
    /* GM_HIT */      {GM_CRIT,      GM_ATK,       GM_SUPPORTS,  GM_GROWTHS},
    /* GM_CRIT */     {GM_SPEED,     GM_PROT,      GM_HIT,       GM_GROWTHS},
    /* GM_GRAPH */    {GM_ELEM_NULL, GM_GROWTHS,   GM_MOVE,      GM_ELEM_NULL},
    /* GM_GROWTHS */  {GM_GRAPH,     GM_TALK,      GM_ELEM_NULL, GM_MOVE},
    /* GM_MOVE */     {GM_GRAPH,     GM_GROWTHS,   GM_ELEM_NULL, GM_STR},
    /* GM_STR */      {GM_MAG,       GM_MOVE,      GM_ELEM_NULL, GM_DEX},
    /* GM_DEX */      {GM_AGI,       GM_STR,       GM_ELEM_NULL, GM_DEF},
    /* GM_DEF */      {GM_RES,       GM_DEX,       GM_ELEM_NULL, GM_FTH},
    /* GM_FTH */      {GM_LUCK,      GM_DEF,       GM_ELEM_NULL, GM_CON},
    /* GM_CON */      {GM_PROF,      GM_FTH,       GM_ELEM_NULL, GM_ELEM_NULL},
    /* GM_MAG */      {GM_GRAPH,     GM_MOVE,      GM_STR,       GM_AGI},
    /* GM_AGI */      {GM_GRAPH,     GM_MAG,       GM_DEX,       GM_RES},
    /* GM_RES */      {GM_GRAPH,     GM_AGI,       GM_DEF,       GM_LUCK},
    /* GM_LUCK */     {GM_GRAPH,     GM_RES,       GM_FTH,       GM_PROF},
    /* GM_PROF */     {GM_GRAPH,     GM_LUCK,      GM_CON,       GM_ELEM_NULL},
};

struct GrowthsMenu *GrowthsMenu_Alloc(void) {
    struct GrowthsMenu *gm = SDL_malloc(sizeof(struct GrowthsMenu));
    SDL_assert(gm);
    *gm = GrowthsMenu_default;

    gm->pixelnours     = PixelFont_Alloc();
    gm->pixelnours_big = PixelFont_Alloc();
    SDL_assert(gm->pixelnours     != NULL);
    SDL_assert(gm->pixelnours_big != NULL);

    gm->graph               = Graph_default;
    gm->graph_rect.x        = GM_OFFSET_GRAPH_X * GRAPH_SCALE;
    gm->graph_rect.y        = GM_OFFSET_GRAPH_Y * GRAPH_SCALE;
    gm->graph_rect.w        = graph_size.x * GRAPH_SCALE;
    gm->graph_rect.h        = graph_size.y * GRAPH_SCALE;
    return (gm);
}

void GrowthsMenu_Free(struct GrowthsMenu *gm) {
    if (gm == NULL) {
        return;
    }
    SDL_assert(gm != NULL);
    if (gm->texture != NULL) {
        SDL_DestroyTexture(gm->texture);
        gm->texture = NULL;
    }
    if (gm->pixelnours != NULL) {
        PixelFont_Free(gm->pixelnours, true);
        gm->pixelnours = NULL;
    }
    if (gm->pixelnours_big != NULL) {
        PixelFont_Free(gm->pixelnours_big, true);
        gm->pixelnours_big = NULL;
    }

    SDL_free(gm);
}

SDL_Texture *GrowthsMenu_Texture(GrowthsMenu *gm) {
    return (gm->texture_scaled);
}

void GrowthsMenu_Unit_Set(struct GrowthsMenu *gm, struct Unit *unit) {
    SDL_assert(unit != NULL);
    gm->unit   = unit;
    gm->update = true;
}
void GrowthsMenu_Load(struct GrowthsMenu *gm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);
    n9patch->px.x       = MENU_PATCH_PIXELS;
    n9patch->px.y       = MENU_PATCH_PIXELS;
    n9patch->num.x      = GM_PATCH_X_SIZE;
    n9patch->num.y      = GM_PATCH_Y_SIZE;
    n9patch->scale.x    = GM_N9PATCH_SCALE_X;
    n9patch->scale.y    = GM_N9PATCH_SCALE_Y;
    Point size = {
        .x  = (MENU_PATCH_PIXELS * GM_PATCH_X_SIZE),
        .y  = (MENU_PATCH_PIXELS * GM_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);
    if (n9patch->texture == NULL) {
        char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
        n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    }
}

void GrowthsMenu_Elem_Pos(struct GrowthsMenu *gm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_SCREEN_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = gm->pos.x + mc->elem_pos[i].x * mc->n9patch.scale.x;
        mc->elem_pos[i].y = gm->pos.y + mc->elem_pos[i].y * mc->n9patch.scale.y;
    }

    mc->elem_pos_frame = ELEM_POS_SCREEN_FRAME;
}

void GrowthsMenu_Elem_Pos_Revert(struct GrowthsMenu *gm, struct Menu *mc) {
    /* Scales elem_pos to menu size. */
    /* 1. Makes the cursor focus on right place on the Screen       */
    /* 2. Box lined are drawn in menu frame, making thinner lines   */

    /* - Skip if already in screen frame - */
    if (mc->elem_pos_frame == ELEM_POS_MENU_FRAME) {
        return;
    }

    for (size_t i = 0; i < mc->elem_num; i++) {
        mc->elem_pos[i].x = (mc->elem_pos[i].x - gm->pos.x) / mc->n9patch.scale.x;
        mc->elem_pos[i].y = (mc->elem_pos[i].y - gm->pos.y) / mc->n9patch.scale.y;
    }

    mc->elem_pos_frame = ELEM_POS_MENU_FRAME;
}

void GrowthsMenu_Draw(struct Menu *mc, SDL_Texture *render_target,
                      SDL_Renderer *renderer) {
    struct GrowthsMenu *gm  =  mc->data;
    struct n9Patch *n9patch = &mc->n9patch;

    SDL_assert(gm != NULL);
    if (gm->update) {
        GrowthsMenu_Update(gm, n9patch, render_target, renderer);
        gm->update = false;
    }

    SDL_SetRenderTarget(renderer, render_target);
    SDL_Rect dstrect;
    Point size = n9Patch_Pixels_Total(n9patch);
    dstrect.w = size.x * n9patch->scale.x;
    dstrect.h = size.y * n9patch->scale.y;
    dstrect.x = gm->pos.x;
    dstrect.y = gm->pos.y;
    SDL_RenderCopy(renderer, gm->texture, NULL, &dstrect);
}

static void _GrowthsMenu_Draw_Talk( GrowthsMenu     *gm,
                                    SDL_Renderer    *renderer) {
    /* -- TALK -- */
    SDL_Rect facerect;
    PixelFont_In pxin = {
        .renderer   = renderer,
        .text       = "TALK",
        .len        = 4,
        .pos        =  {
            .x = GM_TALK_X_OFFSET,
            .y = GM_TALK_Y_OFFSET
        }
    };
    PixelFont_Write(gm->pixelnours, pxin);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    facerect.x = GM_TALK_FACE_X_OFFSET;
    facerect.y = GM_TALK_FACE_Y_OFFSET;
    facerect.w = GM_TALK_FACE_W;
    facerect.h = GM_TALK_FACE_H;
    SDL_RenderFillRect(renderer, &facerect);
}

static void _GrowthsMenu_Draw_Graph(GrowthsMenu     *gm,
                                    n9Patch         *n9patch,
                                    SDL_Texture     *render_target,
                                    SDL_Renderer    *renderer) {
    /* -- Graph -- */
    Graph_Draw(&gm->graph, n9patch, gm->pixelnours_big, renderer, render_target);
    SDL_assert(gm->texture != NULL);

    SDL_SetRenderTarget(renderer, gm->texture_scaled);
    SDL_RenderCopy( renderer, Graph_Texture(&gm->graph),
                    NULL, &gm->graph_rect);
    SDL_assert(gm->texture);
}

static void _GrowthsMenu_Draw_Growths(  GrowthsMenu     *gm,
                                        SDL_Renderer    *renderer) {
    /* -- STATS GROWTHS -- */
    char numbuff[10];
    Unit_stats effective_growths = Unit_effectiveGrowths(gm->unit);

    PixelFont_In pxin = {
        .renderer   = renderer,
        .text       = "STATS GROWTHS",
        .len        = 13,
        .pos        =  {
            .x = GM_STATS_X_OFFSET,
            .y = GM_STATS_Y_OFFSET
        }
    };

    PixelFont_Write(gm->pixelnours, pxin);

    /* -- GROWN STATS GRAPH -- */
    /* -- ACTUAL GROWTHS -- */
    /* - str - */
#define REGISTER_ENUM(NAME) \
    pxin.text       = #NAME;\
    pxin.len        = 0; \
    pxin.centered   = SOTA_TEXT_LEFT; \
    pxin.pos.x      = GM_ ## NAME ## _X_OFFSET;\
    pxin.pos.y      = GM_ ## NAME ## _Y_OFFSET;\
    PixelFont_Write(gm->pixelnours, pxin);

    REGISTER_ENUM(STR)
    REGISTER_ENUM(MAG)
    REGISTER_ENUM(DEX)
    REGISTER_ENUM(AGI)
    REGISTER_ENUM(FTH)
    REGISTER_ENUM(LUCK)
    REGISTER_ENUM(DEF)
    REGISTER_ENUM(RES)
    REGISTER_ENUM(CON)
    // REGISTER_ENUM(MOVE)
    REGISTER_ENUM(PROF)
#undef REGISTER_ENUM

#define REGISTER_ENUM(name, NAME) \
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.name); \
    pxin.text       = numbuff; \
    pxin.len        = 0; \
    pxin.centered   = SOTA_TEXT_CENTER; \
    pxin.pos.x      = GM_ ## NAME ## _STAT_X_OFFSET; \
    pxin.pos.y      = GM_ ## NAME ## _STAT_Y_OFFSET; \
    PixelFont_Write(gm->pixelnours_big, pxin);

    REGISTER_ENUM(str,  STR)
    REGISTER_ENUM(mag,  MAG)
    REGISTER_ENUM(dex,  DEX)
    REGISTER_ENUM(agi,  AGI)
    REGISTER_ENUM(fth,  FTH)
    REGISTER_ENUM(luck, LUCK)
    REGISTER_ENUM(def,  DEF)
    REGISTER_ENUM(res,  RES)
    REGISTER_ENUM(con,  CON)
    REGISTER_ENUM(move, MOVE)
    REGISTER_ENUM(prof, PROF)
#undef REGISTER_ENUM

    pxin.text   = "Mv";
    pxin.len    = 2;
    pxin.pos.x  = GM_MOVE_X_OFFSET;
    pxin.pos.y  = GM_MOVE_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, pxin);
}

static void _GrowthsMenu_Draw_Supports( GrowthsMenu     *gm,
                                        SDL_Renderer    *renderer) {
    /* -- SUPPORTS -- */
    SDL_Rect facerect;
    char numbuff[10];
    /* TODO: */
    // struct Computed_Stats support_stats  = gm->unit->support_bonus;
    struct Computed_Stats support_stats  = Computed_Stats_default;
    PixelFont_In pxin = {.renderer = renderer };

    pxin.text       = "SUPPORTS";
    pxin.len        = 8;
    pxin.pos.x      = GM_SUPPORTS_X;
    pxin.pos.y      = GM_SUPPORTS_Y;
    PixelFont_Write(gm->pixelnours, pxin);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    facerect.x = GM_SUPPORTS_FACE_X_OFFSET;
    facerect.y = GM_SUPPORTS_FACE_Y_OFFSET;
    facerect.w = GM_SUPPORTS_FACE_W;
    facerect.h = GM_SUPPORTS_FACE_H;
    SDL_RenderFillRect(renderer, &facerect);

    /* -- SUPPORT BONUS TOTAL -- */
    pxin.text       = "SUPPORT BONUS";
    pxin.len        = 13;
    pxin.pos.x      = GM_BONUS_X;
    pxin.pos.y      = GM_BONUS_Y;
    PixelFont_Write(gm->pixelnours, pxin);


#define REGISTER_ENUM(NAME) \
    pxin.text       = #NAME; \
    pxin.len        = 0; \
    pxin.centered   = SOTA_TEXT_LEFT; \
    pxin.pos.x      = GM_ ## NAME ## _X_OFFSET; \
    pxin.pos.y      = GM_ ## NAME ## _Y_OFFSET; \
    PixelFont_Write(gm->pixelnours_big, pxin);

    REGISTER_ENUM(ATK)
    REGISTER_ENUM(PROT)
    REGISTER_ENUM(HIT)
    REGISTER_ENUM(CRIT)
    REGISTER_ENUM(SPEED)
#undef REGISTER_ENUM

    /* - ATK - */
    stbsp_sprintf(  numbuff, "%d\0\0\0\0",
                    support_stats.attack.physical);
    stbsp_sprintf(  numbuff, "%d/%d\0\0",
                    support_stats.attack.physical,
                    support_stats.attack.magical);
    pxin.text       = numbuff;
    pxin.centered   = SOTA_TEXT_CENTER;
    pxin.pos.x      = GM_ATK_X_OFFSET_STAT1;
    pxin.pos.y      = GM_ATK_Y_OFFSET_STAT1;
    PixelFont_Write(gm->pixelnours_big, pxin);

    /* - PROT - */
    stbsp_sprintf(  numbuff, "%d\0\0\0\0",
                    support_stats.protection.physical);
    stbsp_sprintf(  numbuff, "%d/%d\0\0",
                    support_stats.protection.physical,
                    support_stats.protection.magical);
    pxin.text       = numbuff;
    pxin.centered   = SOTA_TEXT_CENTER;
    pxin.pos.x      = GM_PROT_X_OFFSET_STAT1;
    pxin.pos.y      = GM_PROT_Y_OFFSET_STAT1;
    PixelFont_Write(gm->pixelnours_big, pxin);

    /* - HIT - */
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.hit);
    stbsp_sprintf(  numbuff, "%d/%d\0\0",
                    support_stats.hit,
                    support_stats.dodge);
    pxin.text       = numbuff;
    pxin.centered   = SOTA_TEXT_CENTER;
    pxin.pos.x      = GM_HIT_X_OFFSET_STAT;
    pxin.pos.y      = GM_HIT_Y_OFFSET_STAT;
    PixelFont_Write(gm->pixelnours_big, pxin);

    /* - CRIT - */
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.crit);
    stbsp_sprintf(numbuff, "%d/%d\0\0", support_stats.crit, support_stats.favor);
    pxin.text       = numbuff;
    pxin.centered   = SOTA_TEXT_CENTER;
    pxin.pos.x      = GM_CRIT_X_OFFSET_STAT;
    pxin.pos.y      = GM_CRIT_Y_OFFSET_STAT;
    PixelFont_Write(gm->pixelnours_big, pxin);

    /* - SPEED - */
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.speed);
    pxin.text       = numbuff;
    pxin.centered   = SOTA_TEXT_CENTER;
    pxin.pos.x      = GM_SPEED_X_OFFSET_STAT;
    pxin.pos.y      = GM_SPEED_Y_OFFSET_STAT;
    PixelFont_Write(gm->pixelnours_big, pxin);
}

void GrowthsMenu_Update(struct GrowthsMenu *gm, struct n9Patch *n9patch,
                        SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    SDL_assert(n9patch  != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert(n9patch->texture  != NULL);

    /* - get unit stats - */
    SDL_assert(gm->unit != NULL);
    if (gm->update_stats) {
        Unit_effectiveGrowths(gm->unit);
        Unit_supportBonus(gm->unit);
    }

    /* - create texture - */
    if (gm->texture == NULL) {
        Point size = n9Patch_Pixels_Total(n9patch);
        gm->texture = SDL_CreateTexture(renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET,
                                        size.x, size.y);
        SDL_assert(gm->texture != NULL);
        SDL_SetTextureBlendMode(gm->texture, SDL_BLENDMODE_BLEND);
    }

    if (gm->texture_scaled == NULL) {
        Point size = n9Patch_Pixels_Total(n9patch);
        gm->texture_scaled = SDL_CreateTexture(renderer,
                                               SDL_PIXELFORMAT_ARGB8888,
                                               SDL_TEXTUREACCESS_TARGET,
                                               size.x * GRAPH_SCALE, size.y * GRAPH_SCALE);
        SDL_assert(gm->texture_scaled != NULL);
        SDL_SetTextureBlendMode(gm->texture_scaled, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderTarget(renderer, gm->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);
    SDL_assert(gm->texture != NULL);

    /* --- RENDERING STATS-MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x      = n9patch->scale.x;
    int scale_y      = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = 0;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    _GrowthsMenu_Draw_Talk(      gm, renderer);
    _GrowthsMenu_Draw_Supports(  gm, renderer);
    _GrowthsMenu_Draw_Growths(   gm, renderer);

    /* Copy all 1x elements to scaled texture */
    SDL_SetRenderTarget(renderer, gm->texture_scaled);
    SDL_RenderCopy( renderer, gm->texture,
                    NULL, NULL);

    _GrowthsMenu_Draw_Graph(     gm, n9patch, render_target, renderer);

    /* -- Finish -- */
    SDL_SetRenderTarget(renderer, render_target);
}
