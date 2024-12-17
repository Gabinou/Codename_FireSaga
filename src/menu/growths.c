
#include "menu/growths.h"
#include "menu/stats.h"
#include "menu/menu.h"
#include "unit/bonus.h"
#include "graph.h"
#include "macros.h"
#include "n9patch.h"
#include "filesystem.h"
#include "platform.h"
#include "pixelfonts.h"
#include "unit/unit.h"
#include "SDL.h"
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

struct MenuElemDirections gm_links[GM_ELEM_NUM] = {
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

    gm->graph              = Graph_default;
    gm->graph.rect.x       = GM_OFFSET_GRAPH_X;
    gm->graph.rect.y       = GM_OFFSET_GRAPH_Y;
    gm->graph.rect.w       = GM_OFFSET_GRAPH_W;
    gm->graph.rect.h       = GM_OFFSET_GRAPH_H;
    gm->graph.header       = GM_OFFSET_GRAPH_HEADER;
    gm->graph.footer       = GM_OFFSET_GRAPH_FOOTER;
    gm->graph.margin_left  = GM_OFFSET_GRAPH_MARGINL;
    gm->graph.margin_right = GM_OFFSET_GRAPH_MARGINR;
    return (gm);
}

void GrowthsMenu_Free(struct GrowthsMenu *gm) {
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
    if (gm != NULL) {
        SDL_free(gm);
        gm = NULL;
    }
}

void GrowthsMenu_Unit_Set(struct GrowthsMenu *gm, struct Unit *unit) {
    SDL_assert(unit != NULL);
    gm->unit   = unit;
    gm->update = true;
}
void GrowthsMenu_Load(struct GrowthsMenu *gm, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    n9Patch_Free(n9patch);
    n9patch->patch_pixels.x = MENU_PATCH_PIXELS;
    n9patch->patch_pixels.y = MENU_PATCH_PIXELS;
    n9patch->size_patches.x = GM_PATCH_X_SIZE;
    n9patch->size_patches.y = GM_PATCH_Y_SIZE;
    n9patch->scale.x        = GM_N9PATCH_SCALE_X;
    n9patch->scale.y        = GM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x  = (MENU_PATCH_PIXELS * GM_PATCH_X_SIZE);
    n9patch->size_pixels.y  = (MENU_PATCH_PIXELS * GM_PATCH_Y_SIZE);
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
    dstrect.w = n9patch->size_pixels.x * n9patch->scale.x;
    dstrect.h = n9patch->size_pixels.y * n9patch->scale.y;
    dstrect.x = gm->pos.x;
    dstrect.y = gm->pos.y;
    SDL_RenderCopy(renderer, gm->texture, NULL, &dstrect);
}

static void _GrowthsMenu_Draw_Talk(struct GrowthsMenu *gm, SDL_Renderer *renderer) {
    /* -- TALK -- */
    SDL_Rect facerect;

    int x = GM_TALK_X_OFFSET, y = GM_TALK_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "TALK", 4, x, y);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    facerect.x = GM_TALK_FACE_X_OFFSET;
    facerect.y = GM_TALK_FACE_Y_OFFSET;
    facerect.w = GM_TALK_FACE_W;
    facerect.h = GM_TALK_FACE_H;
    SDL_RenderFillRect(renderer, &facerect);
}

static void _GrowthsMenu_Draw_Graph(struct GrowthsMenu *gm, struct n9Patch *n9patch,
                                    SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* -- Graph -- */
    SDL_Rect dstrect;
    Graph_Draw(&gm->graph, n9patch, gm->pixelnours_big, renderer, render_target);
    SDL_assert(gm->texture != NULL);

    dstrect.x = GM_OFFSET_GRAPH_X;
    dstrect.y = GM_OFFSET_GRAPH_Y;
    dstrect.h = GM_OFFSET_GRAPH_H;
    dstrect.w = GM_OFFSET_GRAPH_W;
    SDL_SetRenderTarget(renderer, gm->texture);
    SDL_RenderCopy(renderer, gm->graph.texture, NULL, &dstrect);
    SDL_assert(gm->texture);
}

static void _GrowthsMenu_Draw_Growths( struct GrowthsMenu *gm, SDL_Renderer *renderer) {
    /* -- STATS GROWTHS -- */
    int width;
    char numbuff[10];
    struct Unit_stats effective_growths  = gm->unit->effective_growths;

    int x = GM_STATS_X_OFFSET, y = GM_STATS_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "STATS GROWTHS", 13, x, y);

    /* -- GROWN STATS GRAPH -- */
    /* -- ACTUAL GROWTHS -- */
    /* - str - */
    x = GM_STR_X_OFFSET, y = GM_STR_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "STR", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.str);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_STR_STAT_X_OFFSET - width / 2, y = GM_STR_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - mag - */
    x = GM_MAG_X_OFFSET, y = GM_MAG_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "MAG", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.mag);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_MAG_STAT_X_OFFSET - width / 2, y = GM_MAG_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - dex - */
    x = GM_DEX_X_OFFSET, y = GM_DEX_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "DEX", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.dex);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_DEX_STAT_X_OFFSET - width / 2, y = GM_DEX_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - agi - */
    x = GM_AGI_X_OFFSET, y = GM_AGI_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "AGI", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.agi);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_AGI_STAT_X_OFFSET - width / 2, y = GM_AGI_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - luck - */
    x = GM_LUCK_X_OFFSET, y = GM_LUCK_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "LUCK", 4, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.luck);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_LUCK_STAT_X_OFFSET - width / 2, y = GM_LUCK_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - faith - */
    x = GM_FTH_X_OFFSET, y = GM_FTH_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "FTH", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.fth);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_FTH_STAT_X_OFFSET - width / 2, y = GM_FTH_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - def - */
    x = GM_DEF_X_OFFSET, y = GM_DEF_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "DEF", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.def);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_DEF_STAT_X_OFFSET - width / 2, y = GM_DEF_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - res - */
    x = GM_RES_X_OFFSET, y = GM_RES_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "RES", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.res);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_RES_STAT_X_OFFSET - width / 2, y = GM_RES_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - con - */
    x = GM_CON_X_OFFSET, y = GM_CON_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "CON", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.con);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_CON_STAT_X_OFFSET - width / 2, y = GM_CON_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - prof - */
    x = GM_PROF_X_OFFSET, y = GM_PROF_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "PROF", 4, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.prof);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_PROF_STAT_X_OFFSET - width / 2, y = GM_PROF_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - mv - */
    x = GM_MOVE_X_OFFSET, y = GM_MOVE_Y_OFFSET;
    PixelFont_Write(gm->pixelnours_big, renderer, "Mv", 2, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", effective_growths.move);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = GM_MOVE_STAT_X_OFFSET - width / 2, y = GM_MOVE_STAT_Y_OFFSET;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);
}

static void _GrowthsMenu_Draw_Supports(struct GrowthsMenu *gm, SDL_Renderer *renderer) {
    /* -- SUPPORTS -- */
    SDL_Rect facerect;
    char numbuff[10];
    /* TODO: */
    // struct Computed_Stats support_stats  = gm->unit->support_bonus;
    struct Computed_Stats support_stats  = Computed_Stats_default;

    int x = GM_SUPPORTS_X, y = GM_SUPPORTS_Y;
    PixelFont_Write(gm->pixelnours, renderer, "SUPPORTS", 8, x, y);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    facerect.x =  GM_SUPPORTS_FACE_X_OFFSET;
    facerect.y =  GM_SUPPORTS_FACE_Y_OFFSET;
    facerect.w =  GM_SUPPORTS_FACE_W;
    facerect.h =  GM_SUPPORTS_FACE_H;
    SDL_RenderFillRect(renderer, &facerect);

    /* -- SUPPORT BONUS TOTAL -- */
    x = GM_BONUS_X, y = GM_BONUS_Y;
    PixelFont_Write(gm->pixelnours, renderer, "SUPPORT BONUS", 13, x, y);

    /* - ATK - */
    x = GM_ATK_X_OFFSET,  y = GM_ATK_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "ATK", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.attack[DMG_TYPE_PHYSICAL]);
    int width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", support_stats.attack[DMG_TYPE_PHYSICAL],
                  support_stats.attack[DMG_TYPE_MAGICAL]);
    x = (GM_ATK_X_OFFSET_STAT1 - width),  y = GM_ATK_Y_OFFSET_STAT1;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - PROT - */
    x = GM_PROT_X_OFFSET,  y = GM_PROT_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "DEF", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.protection[DMG_TYPE_PHYSICAL]);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", support_stats.protection[DMG_TYPE_PHYSICAL],
                  support_stats.protection[DMG_TYPE_MAGICAL]);
    x = (GM_PROT_X_OFFSET_STAT1 - width),  y = GM_PROT_Y_OFFSET_STAT1;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - HIT - */
    x = GM_HIT_X_OFFSET,  y = GM_HIT_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "HIT", 3, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.hit);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", support_stats.hit, support_stats.dodge);
    x = (GM_HIT_X_OFFSET_STAT - width),  y = GM_HIT_Y_OFFSET_STAT;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - CRIT - */
    x = GM_CRIT_X_OFFSET,  y = GM_CRIT_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "CRIT", 4, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.crit);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    stbsp_sprintf(numbuff, "%d/%d\0\0", support_stats.crit, support_stats.favor);
    x = (GM_CRIT_X_OFFSET_STAT - width),  y = GM_CRIT_Y_OFFSET_STAT;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);

    /* - SPEED - */
    x = GM_SPEED_X_OFFSET,  y = GM_SPEED_Y_OFFSET;
    PixelFont_Write(gm->pixelnours, renderer, "SPEED", 5, x, y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", support_stats.speed);
    width = PixelFont_Width_Len(gm->pixelnours_big, numbuff);
    x = (GM_SPEED_X_OFFSET_STAT - width / 2),  y = GM_SPEED_Y_OFFSET_STAT;
    PixelFont_Write_Len(gm->pixelnours_big, renderer, numbuff, x, y);
}

void GrowthsMenu_Update(struct GrowthsMenu *gm, struct n9Patch *n9patch,
                        SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    SDL_assert(n9patch  != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert(n9patch->texture  != NULL);
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);

    /* - get unit stats - */
    SDL_assert(gm->unit != NULL);
    if (gm->update_stats) {
        Unit_effectiveGrowths(gm->unit);
        Unit_supportBonus(gm->unit);
    }

    /* - create texture - */
    if (gm->texture == NULL) {
        int x = n9patch->size_pixels.x, y = n9patch->size_pixels.y;
        gm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, x, y);
        SDL_assert(gm->texture != NULL);
        SDL_SetTextureBlendMode(gm->texture, SDL_BLENDMODE_BLEND);
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
    _GrowthsMenu_Draw_Graph(     gm, n9patch, render_target, renderer);

    /* -- Finish -- */
    SDL_SetRenderTarget(renderer, render_target);
}
