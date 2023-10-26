#ifndef STATS_MENU_H
#define STATS_MENU_H

#include "enums.h"
#include "globals.h"
#include "bars/simple.h"
#include "bars/stat.h"
#include "pixelfonts.h"
#include "menu.h"
#include "unit.h"
#include "nmath.h"
#include "n9patch.h"
#include "events.h"
#include "nstr.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;

/* --- CONSTANTS --- */
extern const i8 stats_menu_cycle[STATS_MENU_CYCLE_NUM];
extern const i8 stats_menu_cycle_inv[MENU_TYPE_END];

#define REGISTER_ENUM(x) SM_ELEM_##x,
enum STATS_MENU_ELEMENTS {
    SM_ELEM_NULL  = -1,
    SM_ELEM_START = -1,
#include "names/menu/stats.h"
    SM_ELEM_NUM,
    SM_ELEM_END   = SM_ELEM_NUM,
};
#undef REGISTER_ENUM

enum SM_SEX { /* offset by 1 for NULL == 0 */
    SM_SEX_TILESIZE   = 16,
    SM_SEX_ICON_SIZEX = 10, /* for actual contents */
    SM_SEX_ICON_SIZEY = 10, /* for actual contents */
    SM_SEX_NONE       =  0,
    SM_SEX_FEMALE     =  1,
    SM_SEX_MALE       =  2,
    SM_SEX_UNKNOWN    =  3,
};

enum SM_MOUNTS { /* offset by 1 for NULL == 0 */
    SM_MOUNTS_TILESIZE    = 32,
};

enum SM_STATUSES { /* offset by 1 for NULL == 0 */
    SM_STATUSES_TILESIZE  = 16,
};

enum SM_WEAPONS { /* offset by 1 for NULL == 0 */
    SM_WEAPONS_TILESIZE   = 16,
    SM_WEAPONS_DIST       =  2,
    SM_WEAPONS_NONE       =  0,
    SM_WEAPONS_SWORD      =  1,
    SM_WEAPONS_LANCE      =  2,
    SM_WEAPONS_AXE        =  3,
    SM_WEAPONS_BOW        =  4,
    SM_WEAPONS_TRINKET    =  5,
    SM_WEAPONS_OFFHAND    =  6,
    SM_WEAPONS_MAGIC      =  7,
    SM_WEAPONS_ELEMENTAL  =  7,
    SM_WEAPONS_DEMONIC    =  8,
    SM_WEAPONS_ANGELIC    =  9,
    SM_WEAPONS_SHIELD     = 10,
    SM_WEAPONS_STAFF      = 11,
    SM_WEAPONS_CLAW       = 12,
    SM_WEAPON_TYPES_MAX   =  4,

};

enum STATS_MENU {
    STATS_MENU_PATCH_X_SIZE     = 31,
    STATS_MENU_PATCH_Y_SIZE     = 23,
    STATS_MENU_N9PATCH_SCALE_X  =  3,
    STATS_MENU_N9PATCH_SCALE_Y  =  3,
    SM_ROWLEN     =  8,
    MENU_PATCH_PIXELS           =  8,
    STATS_MENU_LINESPACE        =  8,
    TOP_LINE                    =  5,

    /* Computed stats */
    ATK_X_OFFSET        = 144,
    ATK_Y_OFFSET        =  13,
    ATK_X_OFFSET_STAT1  = ATK_X_OFFSET + 6,
    ATK_Y_OFFSET_STAT1  = ATK_Y_OFFSET + STATS_MENU_LINESPACE,
    ATK_X_OFFSET_STAT2  = ATK_X_OFFSET + 1,

    HIT_X_OFFSET        = ATK_X_OFFSET + 39,
    HIT_Y_OFFSET        = ATK_Y_OFFSET,
    HIT_X_OFFSET_STAT   = HIT_X_OFFSET + 5,
    HIT_Y_OFFSET_STAT   = ATK_Y_OFFSET_STAT1,

    RANGE_X_OFFSET      = HIT_X_OFFSET + 32,
    RANGE_Y_OFFSET      = HIT_Y_OFFSET,
    RANGE_X_OFFSET_STAT = RANGE_X_OFFSET + 3,
    RANGE_Y_OFFSET_STAT = HIT_Y_OFFSET_STAT,

    PROT_X_OFFSET       = ATK_X_OFFSET + 1,
    PROT_Y_OFFSET       = ATK_Y_OFFSET + STATS_MENU_LINESPACE * 2 + 3,
    PROT_X_OFFSET_STAT1 = ATK_X_OFFSET_STAT1,
    PROT_Y_OFFSET_STAT1 = PROT_Y_OFFSET + STATS_MENU_LINESPACE,

    CRIT_X_OFFSET       = HIT_X_OFFSET - 3,
    CRIT_Y_OFFSET       = HIT_Y_OFFSET + STATS_MENU_LINESPACE * 2 + 3,
    CRIT_X_OFFSET_STAT  = HIT_X_OFFSET_STAT,
    CRIT_Y_OFFSET_STAT  = CRIT_Y_OFFSET + STATS_MENU_LINESPACE,

    SPEED_X_OFFSET      = RANGE_X_OFFSET,
    SPEED_Y_OFFSET      = CRIT_Y_OFFSET,
    SPEED_X_OFFSET_STAT = SPEED_X_OFFSET + 7,
    SPEED_Y_OFFSET_STAT = SPEED_Y_OFFSET + STATS_MENU_LINESPACE,

    /* stats */
    SEX_ICON_X_OFFSET   = 10,
    SEX_ICON_Y_OFFSET   = TOP_LINE,
    NAME_X_OFFSET       = SEX_ICON_X_OFFSET + SM_SEX_ICON_SIZEX + 1,
    NAME_Y_OFFSET       = SEX_ICON_Y_OFFSET,
    TITLE_X_OFFSET      = SEX_ICON_X_OFFSET - 2,
    TITLE_Y_OFFSET      = SEX_ICON_Y_OFFSET + STATS_MENU_LINESPACE + 3,
    CLASS_X_OFFSET      = TITLE_X_OFFSET,
    CLASS_Y_OFFSET      = TITLE_Y_OFFSET + STATS_MENU_LINESPACE + 1,
    HP_X_OFFSET         = CLASS_X_OFFSET,
    HP_Y_OFFSET         = CLASS_Y_OFFSET + STATS_MENU_LINESPACE + 1,
    HP_STAT_X_OFFSET    = HP_X_OFFSET + 14,
    HP_STAT_Y_OFFSET    = HP_Y_OFFSET,
    EXP_X_OFFSET        = HP_X_OFFSET,
    EXP_Y_OFFSET        = HP_Y_OFFSET + STATS_MENU_LINESPACE + 3,
    EXP_STAT_X_OFFSET   = EXP_X_OFFSET + 18,
    EXP_STAT_Y_OFFSET   = EXP_Y_OFFSET,
    LV_X_OFFSET         = EXP_X_OFFSET + 32,
    LV_Y_OFFSET         = EXP_Y_OFFSET,
    LV_STAT_X_OFFSET    = LV_X_OFFSET + 14,
    LV_STAT_Y_OFFSET    = LV_Y_OFFSET,
    MOVE_X_OFFSET       = LV_X_OFFSET + 23,
    MOVE_Y_OFFSET       = LV_Y_OFFSET,
    MOVE_STAT_X_OFFSET  = MOVE_X_OFFSET + 13,
    MOVE_STAT_Y_OFFSET  = MOVE_Y_OFFSET,

    REGRETS_X_OFFSET    = EXP_X_OFFSET + 2,
    REGRETS_Y_OFFSET    = EXP_Y_OFFSET + 6 + 8,

    REGRETS_X_STAT      = REGRETS_X_OFFSET + 17,
    REGRETS_Y_STAT      = REGRETS_Y_OFFSET + 11,
    // REGRETS_NONE_X_OFFSET = STATUSES_X_OFFSET + 14,
    // REGRETS_NONE_Y_OFFSET = STATUSES_Y_OFFSET + 13,

    MOUNT_X_OFFSET      = REGRETS_X_OFFSET + 151,
    MOUNT_Y_OFFSET      = REGRETS_Y_OFFSET,
    MOUNT_NONE_X_OFFSET = MOUNT_X_OFFSET + 11,
    MOUNT_NONE_Y_OFFSET = MOUNT_Y_OFFSET + 11,
    MOUNT_ICON_X_OFFSET = MOUNT_X_OFFSET,
    MOUNT_ICON_Y_OFFSET = MOUNT_Y_OFFSET + 11,

    WEAPONS_X_OFFSET        = REGRETS_X_OFFSET + 51,
    WEAPONS_Y_OFFSET        = REGRETS_Y_OFFSET,
    WEAPONS_NONE_X_OFFSET   = WEAPONS_X_OFFSET + 16,
    WEAPONS_NONE_Y_OFFSET   = WEAPONS_Y_OFFSET + 11,
    WEAPONS_ICON_X_OFFSET   = WEAPONS_NONE_X_OFFSET + 2,
    WEAPONS_ICON_Y_OFFSET   = WEAPONS_Y_OFFSET + 7,

    RESCUE_X_OFFSET         = REGRETS_X_OFFSET + 105,
    RESCUE_Y_OFFSET         = REGRETS_Y_OFFSET,
    RESCUEE_X_OFFSET        = RESCUE_X_OFFSET + 14,
    RESCUEE_Y_OFFSET        = RESCUE_Y_OFFSET + 11,
    RESCUE_NONE_X_OFFSET    = RESCUE_X_OFFSET + 13,
    RESCUE_NONE_Y_OFFSET    = RESCUE_Y_OFFSET + 11,

    STATUSES_X_OFFSET       = REGRETS_X_OFFSET + 198,
    STATUSES_Y_OFFSET       = REGRETS_Y_OFFSET,
    STATUSES_NONE_X_OFFSET  = STATUSES_X_OFFSET + 14,
    STATUSES_NONE_Y_OFFSET  = STATUSES_Y_OFFSET + 13,
    STATUSES_ICON_X_OFFSET  = STATUSES_X_OFFSET + 12,
    STATUSES_ICON_Y_OFFSET  = STATUSES_Y_OFFSET + 11,

    SKILLS_X_OFFSET = 50,
    SKILLS_Y_OFFSET = 85,
    SKILL_ICON_W    = 16,
    SKILL_ICON_H    = 16,
    SKILL1_X_OFFSET = SKILLS_X_OFFSET - 30,
    SKILL1_Y_OFFSET = SKILLS_Y_OFFSET +  8,
    SKILL2_X_OFFSET = SKILL1_X_OFFSET + SKILL_ICON_W + 2,
    SKILL3_X_OFFSET = SKILL2_X_OFFSET + SKILL_ICON_W + 2,
    SKILL4_X_OFFSET = SKILL3_X_OFFSET + SKILL_ICON_W + 2,
    SKILL5_X_OFFSET = SKILL4_X_OFFSET + SKILL_ICON_W + 2,
    SKILL2_Y_OFFSET = SKILL1_Y_OFFSET,
    SKILL3_Y_OFFSET = SKILL2_Y_OFFSET,
    SKILL4_Y_OFFSET = SKILL3_Y_OFFSET,
    SKILL5_Y_OFFSET = SKILL4_Y_OFFSET,

    PORTRAIT_X_OFFSET   = 85,
    PORTRAIT_Y_OFFSET   =  5,
    PORTRAIT_W          = 48,
    PORTRAIT_H          = 48,

    ITEM_ICON_W         =  12,
    ITEM_ICON_H         =  12,
    ITEM1_X_OFFSET      = 132,
    ITEM1_Y_OFFSET      =  90,
    SM_ITEML_X          = 132,
    SM_ITEMR_X          = SM_ITEML_X + 88,
    SM_ITEM_Y           =  90,

    ITEM1_DURA_X_OFFSET = ITEM1_X_OFFSET + ITEM_ICON_W + 6,
    ITEM1_DURA_Y_OFFSET = ITEM1_Y_OFFSET + 2,

    ITEM_HANDS_INDEX       =  2,
    ITEM_ICON_SPACE        =  3,
    SM_LINE                = ITEM_ICON_H + ITEM_ICON_SPACE,

    SM_HANDL_X_OFFSET      = 12,
    SM_HANDL_X             = SM_ITEML_X,
    SM_HAND_STRONG_Y       = SM_ITEM_Y,
    SM_HANDR_X             = SM_ITEMR_X + ITEM_ICON_W + 2,
    SM_HAND_WEAK_Y         = SM_ITEM_Y + SM_LINE,

    SM_ITEM_TWOHANDL_X     = SM_ITEML_X,
    SM_ITEM_TWOHANDR_X     = SM_ITEMR_X,
    SM_ITEM_TWOHAND_Y      = SM_ITEM_Y + SM_LINE / 2,

    ITEM1_NAME_X_OFFSET    = ITEM1_DURA_X_OFFSET + 6,
    ITEM1_NAME_Y_OFFSET    = ITEM1_Y_OFFSET + ITEM_ICON_SPACE,
    ITEM_TWOLINES_OFFSET_Y =  3,
    ITEM1_NAME_W_MAX       = 64,
    ITEM2_NAME_W_MAX       = 64,
    ITEM3_NAME_W_MAX       = 88,
    ITEM4_NAME_W_MAX       = 88,
    ITEM5_NAME_W_MAX       = 88,
    ITEM6_NAME_W_MAX       = 88,

    ITEM2_X_OFFSET = ITEM1_X_OFFSET,
    ITEM3_X_OFFSET = ITEM1_X_OFFSET,
    ITEM4_X_OFFSET = ITEM1_X_OFFSET,
    ITEM5_X_OFFSET = ITEM1_X_OFFSET,
    ITEM6_X_OFFSET = ITEM1_X_OFFSET,
    ITEM2_Y_OFFSET = ITEM1_Y_OFFSET + ITEM_ICON_H + ITEM_ICON_SPACE,
    ITEM3_Y_OFFSET = ITEM2_Y_OFFSET + ITEM_ICON_H + ITEM_ICON_SPACE,
    ITEM4_Y_OFFSET = ITEM3_Y_OFFSET + ITEM_ICON_H + ITEM_ICON_SPACE,
    ITEM5_Y_OFFSET = ITEM4_Y_OFFSET + ITEM_ICON_H + ITEM_ICON_SPACE,
    ITEM6_Y_OFFSET = ITEM5_Y_OFFSET + ITEM_ICON_H + ITEM_ICON_SPACE,

    STATS_MENU_STATBAR_LEN = 57,
    STATBAR_COL1_X_OFFSET  = 18,
    STATBAR_COL1_Y_OFFSET  =  1,
    STATBAR_COL2_X_OFFSET  = 22,
    STATBAR_COL2_Y_OFFSET  =  1,
    HPBAR_X_OFFSET         = 39,
    HPBAR_Y_OFFSET         =  1,

    STATS_MENU_STAT_LINESPACE   = 13,
    STR_X_OFFSET                =  8,
    STR_Y_OFFSET                = SKILLS_Y_OFFSET + 32,
    STR_STAT_X_OFFSET           = STR_X_OFFSET + 22,
    STR_STAT_Y_OFFSET           = STR_Y_OFFSET -  1,
    MAG_X_OFFSET                = STR_X_OFFSET + STATS_MENU_STATBAR_LEN + 3,
    MAG_Y_OFFSET                = STR_Y_OFFSET,
    MAG_STAT_X_OFFSET           = MAG_X_OFFSET + 25,
    MAG_STAT_Y_OFFSET           = MAG_Y_OFFSET -  1,
    DEX_X_OFFSET                = STR_X_OFFSET,
    DEX_Y_OFFSET                = STR_Y_OFFSET + STATS_MENU_STAT_LINESPACE,
    DEX_STAT_X_OFFSET           = DEX_X_OFFSET + 22,
    DEX_STAT_Y_OFFSET           = DEX_Y_OFFSET -  1,
    AGI_X_OFFSET                = MAG_X_OFFSET,
    AGI_Y_OFFSET                = DEX_Y_OFFSET,
    AGI_STAT_X_OFFSET           = AGI_X_OFFSET + 25,
    AGI_STAT_Y_OFFSET           = AGI_Y_OFFSET -  1,
    DEF_X_OFFSET                = STR_X_OFFSET,
    DEF_Y_OFFSET                = DEX_Y_OFFSET + STATS_MENU_STAT_LINESPACE,
    DEF_STAT_X_OFFSET           = DEF_X_OFFSET + 22,
    DEF_STAT_Y_OFFSET           = DEF_Y_OFFSET -  1,
    RES_X_OFFSET                = MAG_X_OFFSET,
    RES_Y_OFFSET                = DEF_Y_OFFSET,
    RES_STAT_X_OFFSET           = RES_X_OFFSET + 25,
    RES_STAT_Y_OFFSET           = RES_Y_OFFSET -  1,
    FTH_X_OFFSET                = STR_X_OFFSET,
    FTH_Y_OFFSET                = DEF_Y_OFFSET + STATS_MENU_STAT_LINESPACE,
    FTH_STAT_X_OFFSET           = FTH_X_OFFSET + 22,
    FTH_STAT_Y_OFFSET           = FTH_Y_OFFSET -  1,
    LUCK_X_OFFSET               = MAG_X_OFFSET,
    LUCK_Y_OFFSET               = FTH_Y_OFFSET,
    LUCK_STAT_X_OFFSET          = LUCK_X_OFFSET + 25,
    LUCK_STAT_Y_OFFSET          = LUCK_Y_OFFSET -  1,
    CON_X_OFFSET                = STR_X_OFFSET,
    CON_Y_OFFSET                = FTH_Y_OFFSET + STATS_MENU_STAT_LINESPACE,
    CON_STAT_X_OFFSET           = CON_X_OFFSET + 22,
    CON_STAT_Y_OFFSET           = CON_Y_OFFSET -  1,
    PROF_X_OFFSET               = MAG_X_OFFSET,
    PROF_Y_OFFSET               = CON_Y_OFFSET,
    PROF_STAT_X_OFFSET          = PROF_X_OFFSET + 25,
    PROF_STAT_Y_OFFSET          = PROF_Y_OFFSET -  1,
};

enum SM_HANDS { /* offset by 1 for NULL == 0 */
    SM_HANDS_TILESIZE         = 16,
    SM_HANDS_NONE             = -1,
    SM_HANDS_BIG_L            =  0,
    SM_HANDS_SMALL_L          =  1,
    SM_HANDS_SMALL_R          =  2,
    SM_HANDS_BIG_R            =  3,

    SM_HAND_SMALLX_OFFSET     =  2,
    SM_HAND_SMALLY_OFFSET     =  2,

    SM_TWOHAND_Y_OFFSET       =  6,
    SM_WEAKHAND_Y_OFFSET      =  13,
};

enum STATS_MENU_ELEMS {
    STATS_MENU_ELEMS_NULL     = -1,
    STATS_MENU_ELEMS_SEX      =  0,
    STATS_MENU_ELEMS_NAME,
    STATS_MENU_ELEMS_CLASS,
    STATS_MENU_ELEMS_PORTRAIT,
    STATS_MENU_ELEMS_TITLE,
    STATS_MENU_ELEMS_HP,
    STATS_MENU_ELEMS_EXP,
    STATS_MENU_ELEMS_LVL,
    STATS_MENU_ELEMS_MOVE,
    STATS_MENU_ELEMS_MOUNT,
    STATS_MENU_ELEMS_WEAPONS,
    STATS_MENU_ELEMS_RESCUE,
    STATS_MENU_ELEMS_REGRETS,
    STATS_MENU_ELEMS_STATUS,
    STATS_MENU_ELEMS_SKILLS,
    STATS_MENU_ELEMS_SKILL1,
    STATS_MENU_ELEMS_SKILL2,
    STATS_MENU_ELEMS_SKILL3,
    STATS_MENU_ELEMS_SKILL4,
    STATS_MENU_ELEMS_SKILL5,
    STATS_MENU_ELEMS_STR,
    STATS_MENU_ELEMS_DEX,
    STATS_MENU_ELEMS_DEF,
    STATS_MENU_ELEMS_FTH,
    STATS_MENU_ELEMS_CON,
    STATS_MENU_ELEMS_MAG,
    STATS_MENU_ELEMS_AGI,
    STATS_MENU_ELEMS_RES,
    STATS_MENU_ELEMS_LUCK,
    STATS_MENU_ELEMS_PROF,
    STATS_MENU_ELEMS_ITEM1,
    STATS_MENU_ELEMS_ITEM2,
    STATS_MENU_ELEMS_ITEM3,
    STATS_MENU_ELEMS_ITEM4,
    STATS_MENU_ELEMS_ITEM5,
    STATS_MENU_ELEMS_ITEM6,
    STATS_MENU_ELEMS_HAND1,
    STATS_MENU_ELEMS_HAND2,
    STATS_MENU_ELEMS_ATK,
    STATS_MENU_ELEMS_PROT,
    STATS_MENU_ELEMS_RANGE,
    STATS_MENU_ELEMS_HIT,
    STATS_MENU_ELEMS_CRIT,
    STATS_MENU_ELEMS_SPEED,
    STATS_MENU_ELEMS_NUM,
};

/* --- CONSTANTS --- */
#define STATSMENU_BUFFER_LEN 128

extern struct MenuElemDirections stats_menu_links[STATS_MENU_ELEMS_NUM];
extern char stats_menu_description[STATS_MENU_ELEMS_NUM][STATSMENU_BUFFER_LEN];

extern struct Point sm_elem_pos[STATS_MENU_ELEMS_NUM];
extern struct Point stats_menu_elem_box[STATS_MENU_ELEMS_NUM];

extern int status_offset_x[UNIT_STATUS_END];
extern int status_offset_y[UNIT_STATUS_END];

struct StatsMenu {
    struct Point pos; /* [pixels], MENU_POS_bOFFSET = 0 */

    SDL_Texture *texture;
    SDL_Texture *texture_face;
    SDL_Texture *texture_sex;
    SDL_Texture *texture_mount;
    SDL_Texture *texture_hands;
    SDL_Texture *texture_statuses;
    struct Unit *unit;

    struct PixelFont *font_wpns; /* wpn icons as font to write them easily */
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;

    bool update             : 1;
    bool update_stats       : 1; /* only false in tests */
    bool tophand_stronghand : 1; /* If false, tophand is lefthand */
};
extern struct StatsMenu StatsMenu_default;

/* --- Constructors/Destructors --- */
struct StatsMenu *StatsMenu_Alloc();
void StatsMenu_Free(      struct StatsMenu *sm);

static void _StatsMenu_Free_Face( struct StatsMenu *sm);
static void _StatsMenu_Free_Icons(struct StatsMenu *sm);

/* --- Loading --- */
void StatsMenu_Load(      struct StatsMenu *sm, struct Unit    *u,
                          SDL_Renderer     *r,  struct n9Patch *n9);
static void _StatsMenu_Load_Face( struct StatsMenu *sm);
static void _StatsMenu_Load_Icons(struct StatsMenu *sm, SDL_Renderer   *r);

/* --- Drawing --- */
void StatsMenu_Draw(  struct Menu *mc, SDL_Texture *rt,
                      SDL_Renderer         *r);

void StatsMenu_Update(struct StatsMenu     *s,  struct n9Patch *n9,
                      SDL_Texture          *rt, SDL_Renderer   *r);
/* --- Positioning --- */
void StatsMenu_Elem_Pos(       struct StatsMenu *sm, struct Menu *mc);
void StatsMenu_Elem_Pos_Revert(struct StatsMenu *sm, struct Menu *mc);

/* -- Drawing elements -- */
static void _StatsMenu_Draw_Item(         struct StatsMenu *sm, SDL_Renderer *r, int i);
static void _StatsMenu_Draw_Name(         struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Mount(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Stats(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Hands(        struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Rescue(       struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Skills(       struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Statuses(     struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_WpnTypes(     struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_Equipment(    struct StatsMenu *sm, SDL_Renderer *r);
static void _StatsMenu_Draw_ComputedStats(struct StatsMenu *sm, SDL_Renderer *r);

#endif /* STATS_MENU_H */
