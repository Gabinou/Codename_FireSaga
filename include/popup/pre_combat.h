#ifndef PRE_COMBAT_POPUP_H
#define PRE_COMBAT_POPUP_H

#include "enums.h"
#include "pixelfonts.h"
#include "unit/unit.h"
#include "nmath.h"
#include "popup/popup.h"
#include "n9patch.h"
#include "events.h"
#include "nstr.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;

// A:Aggressor
// D:Defendant
enum PRE_COMBAT_POPUP_SIMPLE {
    PCP_PATCH_X_SIZE    = 30,
    PCP_PATCH_Y_SIZE    = 11,
    PCP_N9PATCH_SCALE_X = 3,
    PCP_N9PATCH_SCALE_Y = 3,
    PCP_ICONS_ROWLEN    = 8,
    PCP_PATCH_PIXELS    = 8,

    PCP_COLOR_WPN_TYPE = 26,

    /* Computed stats */
    PCP_SIMPLE_HP_X         = 114,
    PCP_SIMPLE_HP_Y         = 16,
    PCP_SIMPLE_HP_DSTAT_X   = PCP_SIMPLE_HP_X - 32,
    PCP_SIMPLE_HP_DSTAT_Y   = PCP_SIMPLE_HP_Y - 1,
    PCP_SIMPLE_HP_ASTAT_X   = PCP_SIMPLE_HP_X + 16,
    PCP_SIMPLE_HP_ASTAT_Y   = PCP_SIMPLE_HP_Y - 1,

    PCP_MATH_HP_X           = 114,
    PCP_MATH_HP_Y           = 10,
    PCP_MATH_HP_DSTAT_X     = PCP_MATH_HP_X - 32,
    PCP_MATH_HP_DSTAT_Y     = PCP_MATH_HP_Y - 1,
    PCP_MATH_HP_ASTAT_X     = PCP_MATH_HP_X + 16,
    PCP_MATH_HP_ASTAT_Y     = PCP_MATH_HP_Y - 1,

    PCP_SIMPLE_DMG_X        = PCP_SIMPLE_HP_X  -  3,
    PCP_SIMPLE_DMG_Y        = PCP_SIMPLE_HP_Y  + 18,
    PCP_SIMPLE_DMG_DSTAT_X  = PCP_SIMPLE_DMG_X - 22,
    PCP_SIMPLE_DMG_DSTAT_Y  = PCP_SIMPLE_DMG_Y -  1,
    PCP_SIMPLE_DMG_ASTAT_X  = PCP_SIMPLE_DMG_X + 27,
    PCP_SIMPLE_DMG_ASTAT_Y  = PCP_SIMPLE_DMG_Y -  1,

    PCP_SIMPLE_SPLIT_DMG_X          = PCP_SIMPLE_HP_X  -  3,
    PCP_SIMPLE_SPLIT_DMG_Y          = PCP_SIMPLE_HP_Y  + 18,
    PCP_SIMPLE_SPLIT_DMG_DSTAT_X    = PCP_SIMPLE_DMG_X - 17,
    PCP_SIMPLE_SPLIT_DMG_DSTAT_Y    = PCP_SIMPLE_DMG_Y -  1,
    PCP_SIMPLE_SPLIT_DMG_ASTAT_X    = PCP_SIMPLE_DMG_X + 32,
    PCP_SIMPLE_SPLIT_DMG_ASTAT_Y    = PCP_SIMPLE_DMG_Y -  1,

    PCP_MATH_ATK_X          = PCP_MATH_HP_X  -  4,
    PCP_MATH_ATK_Y          = PCP_MATH_HP_Y  + 13,
    PCP_MATH_ATK_DSTAT_X    = PCP_MATH_ATK_X - 17,
    PCP_MATH_ATK_DSTAT_Y    = PCP_MATH_ATK_Y -  1,
    PCP_MATH_ATK_ASTAT_X    = PCP_MATH_ATK_X + 32,
    PCP_MATH_ATK_ASTAT_Y    = PCP_MATH_ATK_Y -  1,

    PCP_MATH_PROT_X         = PCP_MATH_ATK_X,
    PCP_MATH_PROT_Y         = PCP_MATH_ATK_Y  + 13,
    PCP_MATH_PROT_DSTAT_X   = PCP_MATH_PROT_X - 24,
    PCP_MATH_PROT_DSTAT_Y   = PCP_MATH_PROT_Y -  1,
    PCP_MATH_PROT_ASTAT_X   = PCP_MATH_PROT_X + 25,
    PCP_MATH_PROT_ASTAT_Y   = PCP_MATH_PROT_Y -  1,

    PCP_SIMPLE_HIT_X        = PCP_SIMPLE_DMG_X +  0,
    PCP_SIMPLE_HIT_Y        = PCP_SIMPLE_DMG_Y + 18,
    PCP_SIMPLE_HIT_DSTAT_X  = PCP_SIMPLE_HIT_X - 24,
    PCP_SIMPLE_HIT_DSTAT_Y  = PCP_SIMPLE_HIT_Y -  1,
    PCP_SIMPLE_HIT_ASTAT_X  = PCP_SIMPLE_HIT_X + 25,
    PCP_SIMPLE_HIT_ASTAT_Y  = PCP_SIMPLE_HIT_Y -  1,

    PCP_MATH_HIT_X          = PCP_MATH_HP_X   -  4,
    PCP_MATH_HIT_Y          = PCP_MATH_PROT_Y + 13,
    PCP_MATH_HIT_DSTAT_X    = PCP_MATH_HIT_X  - 24,
    PCP_MATH_HIT_DSTAT_Y    = PCP_MATH_HIT_Y  -  1,
    PCP_MATH_HIT_ASTAT_X    = PCP_MATH_HIT_X  + 24,
    PCP_MATH_HIT_ASTAT_Y    = PCP_MATH_HIT_Y  -  1,

    PCP_SIMPLE_CRIT_X       = PCP_SIMPLE_HIT_X  -  2,
    PCP_SIMPLE_CRIT_Y       = PCP_SIMPLE_HIT_Y  + 16,
    PCP_SIMPLE_CRIT_DSTAT_X = PCP_SIMPLE_CRIT_X - 22,
    PCP_SIMPLE_CRIT_DSTAT_Y = PCP_SIMPLE_CRIT_Y -  1,
    PCP_SIMPLE_CRIT_ASTAT_X = PCP_SIMPLE_CRIT_X + 27,
    PCP_SIMPLE_CRIT_ASTAT_Y = PCP_SIMPLE_CRIT_Y -  1,

    PCP_MATH_CRIT_X         = PCP_MATH_HP_X   -  6,
    PCP_MATH_CRIT_Y         = PCP_MATH_HIT_Y  + 12,
    PCP_MATH_CRIT_DSTAT_X   = PCP_MATH_CRIT_X - 22,
    PCP_MATH_CRIT_DSTAT_Y   = PCP_MATH_CRIT_Y -  1,
    PCP_MATH_CRIT_ASTAT_X   = PCP_MATH_CRIT_X + 26,
    PCP_MATH_CRIT_ASTAT_Y   = PCP_MATH_CRIT_Y -  1,

    PCP_MATH_SPEED_X        = PCP_MATH_HP_X    -  9,
    PCP_MATH_SPEED_Y        = PCP_MATH_CRIT_Y  + 12,
    PCP_MATH_SPEED_DSTAT_X  = PCP_MATH_SPEED_X - 17,
    PCP_MATH_SPEED_DSTAT_Y  = PCP_MATH_SPEED_Y -  1,
    PCP_MATH_SPEED_ASTAT_X  = PCP_MATH_SPEED_X + 31,
    PCP_MATH_SPEED_ASTAT_Y  = PCP_MATH_SPEED_Y -  1,

    /* Faces */
    PCP_TRUE_DMG_OFFSET_X    = 3,
    PCP_TRUE_DMG_OFFSET_10_X = 4,

    PCP_SIMPLE_DFACEL_X = 12,
    PCP_SIMPLE_DFACEL_Y = 26,
    PCP_SIMPLE_DFACEL_W = 54,
    PCP_SIMPLE_DFACEL_H = 54,
    PCP_SIMPLE_DNAME_X  = PCP_SIMPLE_DFACEL_X +  6,
    PCP_SIMPLE_DNAME_Y  = PCP_SIMPLE_DFACEL_Y - 15,

    PCP_SIMPLE_AFACEL_X = 172,
    PCP_SIMPLE_AFACEL_Y =   8,
    PCP_SIMPLE_AFACEL_W =  54,
    PCP_SIMPLE_AFACEL_H =  54,
    PCP_SIMPLE_ANAME_X  = PCP_SIMPLE_AFACEL_X + 14,
    PCP_SIMPLE_ANAME_Y  = PCP_SIMPLE_AFACEL_Y + 60,

    /* Icons */
    PCP_SIMPLE_ICON_H      = 16,
    PCP_SIMPLE_ICON_W      = 16,
    PCP_SIMPLE_ICON_ROWLEN =  8,

    PCP_SIMPLE_ICONS_H  = 16,
    PCP_SIMPLE_ICONS_W  = 16 * 2,

    PCP_SIMPLE_DICONL_X = PCP_SIMPLE_DFACEL_X + PCP_SIMPLE_DFACEL_W - PCP_SIMPLE_ICONS_W,
    PCP_SIMPLE_DICONL_Y = PCP_SIMPLE_DFACEL_Y + PCP_SIMPLE_DFACEL_H - PCP_SIMPLE_ICONS_H,
    PCP_SIMPLE_DICONR_X = PCP_SIMPLE_DICONL_X + 16,
    PCP_SIMPLE_DICONR_Y = PCP_SIMPLE_DICONL_Y,

    PCP_SIMPLE_ICON_OFFSET_X = 3,
    PCP_SIMPLE_ICON_OFFSET_Y = 0,

    PCP_HYPHEN_OFFSET_X = 3,
    PCP_HYPHEN_OFFSET_Y = 4,

    PCP_SIMPLE_AICONL_X = PCP_SIMPLE_AFACEL_X + PCP_SIMPLE_AFACEL_W - PCP_SIMPLE_ICONS_W,
    PCP_SIMPLE_AICONL_Y = PCP_SIMPLE_AFACEL_Y + PCP_SIMPLE_AFACEL_H - PCP_SIMPLE_ICONS_H,
    PCP_SIMPLE_AICONR_X = PCP_SIMPLE_AICONL_X + 16,
    PCP_SIMPLE_AICONR_Y = PCP_SIMPLE_AICONL_Y,

    PCP_DOUBLING_SIMPLE_MULT_X = 2,
    PCP_DOUBLING_SIMPLE_MULT_Y = 2,
    PCP_DOUBLING_SIMPLE_TEXT_X = 8,
    PCP_DOUBLING_SIMPLE_TEXT_Y = 2,

    PCP_DOUBLING_SIMPLE_TEXT_W = 15,
    PCP_DOUBLING_SIMPLE_TEXT_H = 12,
    PCP_DOUBLING_SIMPLE_DFT_X  = PCP_SIMPLE_DMG_DSTAT_X - 26,
    PCP_DOUBLING_SIMPLE_DFT_Y  = PCP_SIMPLE_DMG_DSTAT_Y -  2,
    PCP_DOUBLING_SIMPLE_AGG_X  = PCP_SIMPLE_DMG_ASTAT_X + 22,
    PCP_DOUBLING_SIMPLE_AGG_Y  = PCP_SIMPLE_DMG_ASTAT_Y -  2,

    PCP_DOUBLING_MATH_TEXT_W   = 15,
    PCP_DOUBLING_MATH_TEXT_H   = 12,
    PCP_DOUBLING_SPLIT_DFT_X   = PCP_SIMPLE_DMG_DSTAT_X - 34,
    PCP_DOUBLING_SPLIT_DFT_Y   = PCP_SIMPLE_DMG_DSTAT_Y -  2,
    PCP_DOUBLING_SPLIT_AGG_X   = PCP_SIMPLE_DMG_ASTAT_X + 30,
    PCP_DOUBLING_SPLIT_AGG_Y   = PCP_SIMPLE_DMG_ASTAT_Y -  2,

    PCP_DOUBLING_SPLIT_TEXT_W  = 15,
    PCP_DOUBLING_SPLIT_TEXT_H  = 12,
    PCP_DOUBLING_MATH_DFT_X    = PCP_MATH_ATK_DSTAT_X - 40,
    PCP_DOUBLING_MATH_DFT_Y    = PCP_MATH_ATK_DSTAT_Y -  2,
    PCP_DOUBLING_MATH_AGG_X    = PCP_MATH_ATK_ASTAT_X + 23,
    PCP_DOUBLING_MATH_AGG_Y    = PCP_MATH_ATK_ASTAT_Y -  2,

    PCP_DOUBLING_DARK   = 45,
    PCP_DOUBLING_LIGHT  = 47,
};

enum PRE_COMBAT_POPUP_TYPE {
    PCP_MODE_NULL       = 0,
    PCP_MODE_TOTAL      = 1, /* Show total damage */
    PCP_MODE_SIMPLE     = 2,
    PCP_MODE_MATH       = 3,
    PCP_MODE_NUM        = 4,
};

enum PRE_COMBAT_POPUP_ELEMS {
    PRE_COMBAT_POPUP_ELEM_NULL = -1,
    PRE_COMBAT_POPUP_ELEM      =  0,
    PRE_COMBAT_POPUP_ELEM_NUM  =  1,
};

struct PreCombatPopup {
    b32 update;
    struct Point pos; // [pixels]
    int mode;
    SDL_Texture *texture;
    SDL_Texture *texture_weapons;
    SDL_Texture *texture_face_aggressor;
    SDL_Texture *texture_face_defendant;
    SDL_Texture *texture_doubling;  // actually can render any number of phases

    tnecs_entity defendant;
    tnecs_entity aggressor;

    struct Unit *dft_unit;
    struct Unit *agg_unit;

    struct Position *dft_pos;
    struct Position *agg_pos;

    struct Game *sota;
    struct Combat_Forecast *forecast;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
};
extern struct PreCombatPopup PreCombatPopup_default;

/* --- Constructors/Destructors --- */
struct PreCombatPopup *PreCombatPopup_Alloc();
void PreCombatPopup_Free(      struct PreCombatPopup *pcp);
void PreCombatPopup_Free_Icons(struct PreCombatPopup *pcp);
void PreCombatPopup_Free_Faces(struct PreCombatPopup *pcp);

/* --- Loading --- */
void PreCombatPopup_Load(struct PreCombatPopup *pcp, tnecs_entity a,
                         tnecs_entity d, SDL_Renderer *r, struct n9Patch *n9patch);
void _PreCombatPopup_Load(struct PreCombatPopup *pcp, struct Unit *a, struct Unit *d,
                          struct Position *ap, struct Position *dp, SDL_Renderer *r);

// void _PreCombatPopup_Load_Icons(struct PreCombatPopup *pcp, SDL_Renderer *r);
// void _PreCombatPopup_Load_Faces(struct PreCombatPopup *pcp, SDL_Renderer *r);

/* --- Setters --- */
void PreCombatPopup_Set(struct PreCombatPopup *pcp, struct Game *sota);

/* --- Drawing --- */
void PreCombatPopup_Draw(  struct PopUp *p, struct Point pos, SDL_Texture *rt,
                           SDL_Renderer *r);
void PreCombatPopup_Update(struct PreCombatPopup *pcp, struct n9Patch *n9patch,
                           SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing Stats mode FSM -- */
typedef void (*pcp_draw_stats_t)(struct PreCombatPopup *, SDL_Renderer *r);
extern pcp_draw_stats_t pcp_draw_stats[PCP_MODE_NUM];

#endif /* PRE_COMBAT_POPUP_H */
