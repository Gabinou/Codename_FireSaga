#ifndef GM_H
#define GM_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Menu;
struct Game;
struct Graph;
struct n9Patch;
struct n4Directions;

enum GM_ELEMS {
    GM_ELEM_NULL = -1,
    GM_TALK      =  0,
    GM_SUPPORTS,
    GM_SUPPORT,
    GM_SPEED,
    GM_ATK,
    GM_PROT,
    GM_HIT,
    GM_CRIT,
    GM_GRAPH,
    GM_GROWTHS,
    GM_MOVE,
    GM_STR,
    GM_DEX,
    GM_DEF,
    GM_FTH,
    GM_CON,
    GM_MAG,
    GM_AGI,
    GM_RES,
    GM_LUCK,
    GM_PROF,
    GM_ELEM_NUM,
};

enum growths_menu {
    GM_ELEM_START       = -1,

    GM_PATCH_X_SIZE     = 31,
    GM_PATCH_Y_SIZE     = 22,
    GM_N9PATCH_SCALE_X  =  2,
    GM_N9PATCH_SCALE_Y  =  2,

    GM_OFFSET_STATS_GM_X = 85,
    GM_OFFSET_STATS_GM_Y = 74,
    GM_OFFSET_GRAPH_X    = GM_OFFSET_STATS_GM_X + 35,
    GM_OFFSET_GRAPH_Y    = GM_OFFSET_STATS_GM_Y +  9,

    GM_OFFSET_GRAPH_W       = 130,
    GM_OFFSET_GRAPH_H       = 87,
    GM_OFFSET_GRAPH_HEADER  =  9,
    GM_OFFSET_GRAPH_FOOTER  = 12,
    GM_OFFSET_GRAPH_MARGINL = 14,
    GM_OFFSET_GRAPH_MARGINR =  1,

    GM_STATS_X_OFFSET = 84,
    GM_STATS_Y_OFFSET = 75,

    GM_TALK_X_OFFSET  = 30,
    GM_TALK_Y_OFFSET  =  8,

    GM_TALK_FACE_X_OFFSET = GM_TALK_X_OFFSET - 19,
    GM_TALK_FACE_Y_OFFSET = GM_TALK_Y_OFFSET + 14,
    GM_TALK_FACE_W        = 60,
    GM_TALK_FACE_H        = 45,

    GM_SUPPORTS_X = 90,
    GM_SUPPORTS_Y = GM_TALK_Y_OFFSET,

    GM_SUPPORTS_FACE_X_OFFSET = GM_SUPPORTS_X - 11,
    GM_SUPPORTS_FACE_Y_OFFSET = GM_SUPPORTS_Y + 14,
    GM_SUPPORTS_FACE_W        = 60,
    GM_SUPPORTS_FACE_H        = 45,

    GM_STATBAR_LEN          = 57,
    GM_STAT_LINESPACE       = 12,
    GM_COMPSTAT_LINESPACE   = 9,

    GM_BONUS_X              = 160,
    GM_BONUS_Y              = GM_SUPPORTS_Y,

    GM_ATK_X_OFFSET         = 153,
    GM_ATK_Y_OFFSET         = GM_BONUS_Y      + 18,
    GM_ATK_X_OFFSET_STAT1   = GM_ATK_X_OFFSET +  6,
    GM_ATK_Y_OFFSET_STAT1   = GM_ATK_Y_OFFSET + GM_COMPSTAT_LINESPACE,

    GM_PROT_X_OFFSET        = GM_ATK_X_OFFSET  + 35,
    GM_PROT_Y_OFFSET        = GM_ATK_Y_OFFSET,
    GM_PROT_X_OFFSET_STAT1  = GM_PROT_X_OFFSET + 5,
    GM_PROT_Y_OFFSET_STAT1  = GM_ATK_Y_OFFSET_STAT1,

    GM_HIT_X_OFFSET         = GM_ATK_X_OFFSET,
    GM_HIT_Y_OFFSET         = GM_ATK_Y_OFFSET + GM_COMPSTAT_LINESPACE * 2 + 3,
    GM_HIT_X_OFFSET_STAT    = GM_ATK_X_OFFSET_STAT1,
    GM_HIT_Y_OFFSET_STAT    = GM_HIT_Y_OFFSET + GM_COMPSTAT_LINESPACE,

    GM_CRIT_X_OFFSET        = GM_HIT_X_OFFSET + 33,
    GM_CRIT_Y_OFFSET        = GM_HIT_Y_OFFSET,
    GM_CRIT_X_OFFSET_STAT   = GM_PROT_X_OFFSET_STAT1,
    GM_CRIT_Y_OFFSET_STAT   = GM_CRIT_Y_OFFSET + GM_COMPSTAT_LINESPACE,

    GM_SPEED_X_OFFSET       = GM_CRIT_X_OFFSET  + 30,
    GM_SPEED_Y_OFFSET       = GM_ATK_Y_OFFSET   + 12,
    GM_SPEED_X_OFFSET_STAT  = GM_SPEED_X_OFFSET + 12,
    GM_SPEED_Y_OFFSET_STAT  = GM_SPEED_Y_OFFSET + GM_COMPSTAT_LINESPACE,

    GM_STR_X_OFFSET         = 7,
    GM_STR_Y_OFFSET         = 115,
    GM_STR_STAT_X_OFFSET    = GM_STR_X_OFFSET + 24,
    GM_STR_STAT_Y_OFFSET    = GM_STR_Y_OFFSET,
    GM_MAG_X_OFFSET         = GM_STR_X_OFFSET + GM_STATBAR_LEN + 3,
    GM_MAG_Y_OFFSET         = GM_STR_Y_OFFSET,
    GM_MAG_STAT_X_OFFSET    = GM_MAG_X_OFFSET + 27,
    GM_MAG_STAT_Y_OFFSET    = GM_MAG_Y_OFFSET,
    GM_DEX_X_OFFSET         = GM_STR_X_OFFSET,
    GM_DEX_Y_OFFSET         = GM_STR_Y_OFFSET + GM_STAT_LINESPACE,
    GM_DEX_STAT_X_OFFSET    = GM_DEX_X_OFFSET + 24,
    GM_DEX_STAT_Y_OFFSET    = GM_DEX_Y_OFFSET,
    GM_AGI_X_OFFSET         = GM_MAG_X_OFFSET,
    GM_AGI_Y_OFFSET         = GM_DEX_Y_OFFSET,
    GM_AGI_STAT_X_OFFSET    = GM_AGI_X_OFFSET + 27,
    GM_AGI_STAT_Y_OFFSET    = GM_AGI_Y_OFFSET,
    GM_DEF_X_OFFSET         = GM_STR_X_OFFSET,
    GM_DEF_Y_OFFSET         = GM_DEX_Y_OFFSET + GM_STAT_LINESPACE,
    GM_DEF_STAT_X_OFFSET    = GM_DEF_X_OFFSET + 24,
    GM_DEF_STAT_Y_OFFSET    = GM_DEF_Y_OFFSET,
    GM_RES_X_OFFSET         = GM_MAG_X_OFFSET,
    GM_RES_Y_OFFSET         = GM_DEF_Y_OFFSET,
    GM_RES_STAT_X_OFFSET    = GM_RES_X_OFFSET + 27,
    GM_RES_STAT_Y_OFFSET    = GM_RES_Y_OFFSET,
    GM_FTH_X_OFFSET         = GM_STR_X_OFFSET,
    GM_FTH_Y_OFFSET         = GM_DEF_Y_OFFSET + GM_STAT_LINESPACE,
    GM_FTH_STAT_X_OFFSET    = GM_FTH_X_OFFSET + 24,
    GM_FTH_STAT_Y_OFFSET    = GM_FTH_Y_OFFSET,
    GM_LUCK_X_OFFSET        = GM_MAG_X_OFFSET,
    GM_LUCK_Y_OFFSET        = GM_FTH_Y_OFFSET,
    GM_LUCK_STAT_X_OFFSET   = GM_LUCK_X_OFFSET + 27,
    GM_LUCK_STAT_Y_OFFSET   = GM_LUCK_Y_OFFSET,
    GM_CON_X_OFFSET         = GM_STR_X_OFFSET,
    GM_CON_Y_OFFSET         = GM_FTH_Y_OFFSET + GM_STAT_LINESPACE,
    GM_CON_STAT_X_OFFSET    = GM_CON_X_OFFSET + 24,
    GM_CON_STAT_Y_OFFSET    = GM_CON_Y_OFFSET,
    GM_PROF_X_OFFSET        = GM_MAG_X_OFFSET,
    GM_PROF_Y_OFFSET        = GM_CON_Y_OFFSET,
    GM_PROF_STAT_X_OFFSET   = GM_PROF_X_OFFSET + 27,
    GM_PROF_STAT_Y_OFFSET   = GM_PROF_Y_OFFSET,

    GM_MOVE_X_OFFSET        = GM_MAG_X_OFFSET - 26,
    GM_MOVE_Y_OFFSET        = GM_STR_STAT_Y_OFFSET - GM_STAT_LINESPACE,
    GM_MOVE_STAT_X_OFFSET   = GM_MOVE_X_OFFSET + 19,
    GM_MOVE_STAT_Y_OFFSET   = GM_MOVE_Y_OFFSET,
};

#define GM_BUFFER_LEN 128

extern struct n4Directions gm_links[GM_ELEM_NUM];
extern const char GM_description[GM_ELEM_NUM][GM_BUFFER_LEN];
// TODO: reduce memory usage.
extern struct Point gm_elem_pos[GM_ELEM_NUM];
extern struct Point GM_elem_pos_[GM_ELEM_NUM];
extern struct Point gm_elem_box[GM_ELEM_NUM];
extern const struct Point GM_cursor_pos_[GM_ELEM_NUM];
extern const struct Point GM_cursor_box[GM_ELEM_NUM];

#define GROWTHSMENU_POS(xory, offset) (offset * n9patch->scale.xory)

typedef struct GrowthsMenu {
    struct Point         pos; /* [pixels] */
    SDL_Texture         *texture;
    /* Scaled for element with highest scale: graph */
    SDL_Texture         *texture_scaled;
    struct Unit         *unit;
    struct PixelFont    *pixelnours;
    struct PixelFont    *pixelnours_big;
    b32 update;
    b32 update_stats; /* only false in tests */

    /* TODO: decide: can player show 1+ stats in graph
    **  If yes: implement legend */
    Graph graph;
    SDL_Rect graph_rect;
} GrowthsMenu;
extern const GrowthsMenu GrowthsMenu_default;

/* --- Constructors/Destructors --- */
struct GrowthsMenu *GrowthsMenu_Alloc(void);
void GrowthsMenu_Free(GrowthsMenu *gm);

/* --- Getters --- */
SDL_Texture *GrowthsMenu_Texture(GrowthsMenu *gm);

/* --- Loading --- */
void GrowthsMenu_Load(      GrowthsMenu *gm, SDL_Renderer *r,
                            struct n9Patch *n9);
void GrowthsMenu_Unit_Set(  GrowthsMenu *gm, struct Unit *u);

/* --- Positioning --- */
void GrowthsMenu_Elem_Pos(  GrowthsMenu *gm, struct Menu *mc);

/* --- Drawing --- */
void GrowthsMenu_Draw(  struct Menu *mc, SDL_Texture *rt,
                        SDL_Renderer *r);
void GrowthsMenu_Update(GrowthsMenu *g, struct n9Patch *n9,
                        SDL_Texture *rt, SDL_Renderer *r);

#endif /* GM_H */
