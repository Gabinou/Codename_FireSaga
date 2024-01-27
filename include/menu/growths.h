#ifndef GM_H
#define GM_H

#include "enums.h"
#include "pixelfonts.h"
#include "menu.h"
#include "graph.h"
#include "n9patch.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct MenuElemDirections;
struct Menu;
struct Game;

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

enum GM_MENU {
    GM_ELEM_START       = -1,

    GM_PATCH_X_SIZE     = 31,
    GM_PATCH_Y_SIZE     = 22,
    GM_N9PATCH_SCALE_X  =  3,
    GM_N9PATCH_SCALE_Y  =  3,

    GM_OFFSET_STATS_GM_X = 85,
    GM_OFFSET_STATS_GM_Y = 74,
    GM_OFFSET_GRAPH_X    = GM_OFFSET_STATS_GM_X + 35,
    GM_OFFSET_GRAPH_Y    = GM_OFFSET_STATS_GM_Y +  9,

    GM_OFFSET_GRAPH_W       = 110,
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

extern struct MenuElemDirections GM_links[GM_ELEM_NUM];
extern char GM_description[GM_ELEM_NUM][GM_BUFFER_LEN];
// TODO: reduce memory usage.
extern struct Point GM_elem_pos[GM_ELEM_NUM];
extern struct Point GM_elem_pos_[GM_ELEM_NUM];
extern struct Point GM_elem_box[GM_ELEM_NUM];
extern struct Point GM_cursor_pos_[GM_ELEM_NUM];
extern struct Point GM_cursor_box[GM_ELEM_NUM];

#define GROWTHSMENU_POS(xory, offset) (offset * n9patch->scale.xory)

struct GrowthsMenu {
    b32 update;
    struct Point pos; /* [pixels], MENU_POS_bOFFSET = 0 */
    SDL_Texture      *texture;
    struct Unit      *unit;
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    struct Graph graph;
    bool update_stats : 1; /* only false in tests */
};
extern struct GrowthsMenu GrowthsMenu_default;

/* --- Constructors/Destructors --- */
struct GrowthsMenu *GrowthsMenu_Alloc(void);
void GrowthsMenu_Free(struct GrowthsMenu *gm);

/* --- Loading --- */
void GrowthsMenu_Load(struct GrowthsMenu *gm, SDL_Renderer *r, struct n9Patch *n9);
void GrowthsMenu_Unit_Set(struct GrowthsMenu *gm, struct Unit *u);

/* --- Positioning --- */
void GrowthsMenu_Elem_Pos(struct GrowthsMenu *gm, struct Menu *mc);

/* --- Drawing --- */
void GrowthsMenu_Draw(  struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void GrowthsMenu_Update(struct GrowthsMenu *g, struct n9Patch *n9, SDL_Texture *rt,
                        SDL_Renderer *r);


#endif /* GM_H */
