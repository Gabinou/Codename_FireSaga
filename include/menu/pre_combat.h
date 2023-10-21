#ifndef PRE_COMBAT_MENU_H
#define PRE_COMBAT_MENU_H

#include "enums.h"
#include "globals.h"
#include "game/game.h"
#include "pixelfonts.h"
#include "menu.h"
#include "unit.h"
#include "nmath.h"
#include "popup/popup.h"
#include "n9patch.h"
#include "events.h"
#include "nstr.h"
#include "stb_sprintf.h"

// A:Aggressor
// D:Defendant
enum PRE_COMBAT_MENU_SIMPLE {
    PCM_PATCH_X_SIZE    = 30,
    PCM_PATCH_Y_SIZE    = 11,
    PCM_N9PATCH_SCALE_X = 3,
    PCM_N9PATCH_SCALE_Y = 3,
    PCM_ICONS_ROWLEN    = 8,
    PCM_PATCH_PIXELS    = 8,

    PCM_COLOR_WPN_TYPE = 26,

    /* Computed stats */
    PCM_SIMPLE_HP_X         = 114,
    PCM_SIMPLE_HP_Y         = 16,
    PCM_SIMPLE_HP_DSTAT_X   = PCM_SIMPLE_HP_X - 32,
    PCM_SIMPLE_HP_DSTAT_Y   = PCM_SIMPLE_HP_Y - 1,
    PCM_SIMPLE_HP_ASTAT_X   = PCM_SIMPLE_HP_X + 16,
    PCM_SIMPLE_HP_ASTAT_Y   = PCM_SIMPLE_HP_Y - 1,

    PCM_MATH_HP_X           = 114,
    PCM_MATH_HP_Y           = 10,
    PCM_MATH_HP_DSTAT_X     = PCM_MATH_HP_X - 32,
    PCM_MATH_HP_DSTAT_Y     = PCM_MATH_HP_Y - 1,
    PCM_MATH_HP_ASTAT_X     = PCM_MATH_HP_X + 16,
    PCM_MATH_HP_ASTAT_Y     = PCM_MATH_HP_Y - 1,

    PCM_SIMPLE_DMG_X        = PCM_SIMPLE_HP_X  -  3,
    PCM_SIMPLE_DMG_Y        = PCM_SIMPLE_HP_Y  + 18,
    PCM_SIMPLE_DMG_DSTAT_X  = PCM_SIMPLE_DMG_X - 22,
    PCM_SIMPLE_DMG_DSTAT_Y  = PCM_SIMPLE_DMG_Y -  1,
    PCM_SIMPLE_DMG_ASTAT_X  = PCM_SIMPLE_DMG_X + 27,
    PCM_SIMPLE_DMG_ASTAT_Y  = PCM_SIMPLE_DMG_Y -  1,

    PCM_SIMPLE_SPLIT_DMG_X          = PCM_SIMPLE_HP_X  -  3,
    PCM_SIMPLE_SPLIT_DMG_Y          = PCM_SIMPLE_HP_Y  + 18,
    PCM_SIMPLE_SPLIT_DMG_DSTAT_X    = PCM_SIMPLE_DMG_X - 17,
    PCM_SIMPLE_SPLIT_DMG_DSTAT_Y    = PCM_SIMPLE_DMG_Y -  1,
    PCM_SIMPLE_SPLIT_DMG_ASTAT_X    = PCM_SIMPLE_DMG_X + 32,
    PCM_SIMPLE_SPLIT_DMG_ASTAT_Y    = PCM_SIMPLE_DMG_Y -  1,

    PCM_MATH_ATK_X          = PCM_MATH_HP_X  -  4,
    PCM_MATH_ATK_Y          = PCM_MATH_HP_Y  + 13,
    PCM_MATH_ATK_DSTAT_X    = PCM_MATH_ATK_X - 17,
    PCM_MATH_ATK_DSTAT_Y    = PCM_MATH_ATK_Y -  1,
    PCM_MATH_ATK_ASTAT_X    = PCM_MATH_ATK_X + 32,
    PCM_MATH_ATK_ASTAT_Y    = PCM_MATH_ATK_Y -  1,

    PCM_MATH_PROT_X         = PCM_MATH_ATK_X,
    PCM_MATH_PROT_Y         = PCM_MATH_ATK_Y  + 13,
    PCM_MATH_PROT_DSTAT_X   = PCM_MATH_PROT_X - 24,
    PCM_MATH_PROT_DSTAT_Y   = PCM_MATH_PROT_Y -  1,
    PCM_MATH_PROT_ASTAT_X   = PCM_MATH_PROT_X + 25,
    PCM_MATH_PROT_ASTAT_Y   = PCM_MATH_PROT_Y -  1,

    PCM_SIMPLE_HIT_X        = PCM_SIMPLE_DMG_X +  0,
    PCM_SIMPLE_HIT_Y        = PCM_SIMPLE_DMG_Y + 18,
    PCM_SIMPLE_HIT_DSTAT_X  = PCM_SIMPLE_HIT_X - 24,
    PCM_SIMPLE_HIT_DSTAT_Y  = PCM_SIMPLE_HIT_Y -  1,
    PCM_SIMPLE_HIT_ASTAT_X  = PCM_SIMPLE_HIT_X + 25,
    PCM_SIMPLE_HIT_ASTAT_Y  = PCM_SIMPLE_HIT_Y -  1,

    PCM_MATH_HIT_X          = PCM_MATH_HP_X   -  4,
    PCM_MATH_HIT_Y          = PCM_MATH_PROT_Y + 13,
    PCM_MATH_HIT_DSTAT_X    = PCM_MATH_HIT_X  - 24,
    PCM_MATH_HIT_DSTAT_Y    = PCM_MATH_HIT_Y  -  1,
    PCM_MATH_HIT_ASTAT_X    = PCM_MATH_HIT_X  + 24,
    PCM_MATH_HIT_ASTAT_Y    = PCM_MATH_HIT_Y  -  1,

    PCM_SIMPLE_CRIT_X       = PCM_SIMPLE_HIT_X  -  2,
    PCM_SIMPLE_CRIT_Y       = PCM_SIMPLE_HIT_Y  + 16,
    PCM_SIMPLE_CRIT_DSTAT_X = PCM_SIMPLE_CRIT_X - 22,
    PCM_SIMPLE_CRIT_DSTAT_Y = PCM_SIMPLE_CRIT_Y -  1,
    PCM_SIMPLE_CRIT_ASTAT_X = PCM_SIMPLE_CRIT_X + 27,
    PCM_SIMPLE_CRIT_ASTAT_Y = PCM_SIMPLE_CRIT_Y -  1,

    PCM_MATH_CRIT_X         = PCM_MATH_HP_X   -  6,
    PCM_MATH_CRIT_Y         = PCM_MATH_HIT_Y  + 12,
    PCM_MATH_CRIT_DSTAT_X   = PCM_MATH_CRIT_X - 22,
    PCM_MATH_CRIT_DSTAT_Y   = PCM_MATH_CRIT_Y -  1,
    PCM_MATH_CRIT_ASTAT_X   = PCM_MATH_CRIT_X + 26,
    PCM_MATH_CRIT_ASTAT_Y   = PCM_MATH_CRIT_Y -  1,

    PCM_MATH_SPEED_X        = PCM_MATH_HP_X    -  9,
    PCM_MATH_SPEED_Y        = PCM_MATH_CRIT_Y  + 12,
    PCM_MATH_SPEED_DSTAT_X  = PCM_MATH_SPEED_X - 17,
    PCM_MATH_SPEED_DSTAT_Y  = PCM_MATH_SPEED_Y -  1,
    PCM_MATH_SPEED_ASTAT_X  = PCM_MATH_SPEED_X + 31,
    PCM_MATH_SPEED_ASTAT_Y  = PCM_MATH_SPEED_Y -  1,

    /* Faces */
    PCM_TRUE_DMG_OFFSET_X    = 3,
    PCM_TRUE_DMG_OFFSET_10_X = 4,

    PCM_SIMPLE_DFACEL_X = 12,
    PCM_SIMPLE_DFACEL_Y = 26,
    PCM_SIMPLE_DFACEL_W = 54,
    PCM_SIMPLE_DFACEL_H = 54,
    PCM_SIMPLE_DNAME_X  = PCM_SIMPLE_DFACEL_X +  6,
    PCM_SIMPLE_DNAME_Y  = PCM_SIMPLE_DFACEL_Y - 15,

    PCM_SIMPLE_AFACEL_X = 172,
    PCM_SIMPLE_AFACEL_Y =   8,
    PCM_SIMPLE_AFACEL_W =  54,
    PCM_SIMPLE_AFACEL_H =  54,
    PCM_SIMPLE_ANAME_X  = PCM_SIMPLE_AFACEL_X + 14,
    PCM_SIMPLE_ANAME_Y  = PCM_SIMPLE_AFACEL_Y + 60,

    /* Icons */
    PCM_SIMPLE_ICON_H      = 16,
    PCM_SIMPLE_ICON_W      = 16,
    PCM_SIMPLE_ICON_ROWLEN =  8,

    PCM_SIMPLE_ICONS_H  = 16,
    PCM_SIMPLE_ICONS_W  = 16 * 2,

    PCM_SIMPLE_DICONL_X = PCM_SIMPLE_DFACEL_X + PCM_SIMPLE_DFACEL_W - PCM_SIMPLE_ICONS_W,
    PCM_SIMPLE_DICONL_Y = PCM_SIMPLE_DFACEL_Y + PCM_SIMPLE_DFACEL_H - PCM_SIMPLE_ICONS_H,
    PCM_SIMPLE_DICONR_X = PCM_SIMPLE_DICONL_X + 16,
    PCM_SIMPLE_DICONR_Y = PCM_SIMPLE_DICONL_Y,

    PCM_SIMPLE_ICON_OFFSET_X = 3,
    PCM_SIMPLE_ICON_OFFSET_Y = 0,

    PCM_HYPHEN_OFFSET_X = 3,
    PCM_HYPHEN_OFFSET_Y = 4,

    PCM_SIMPLE_AICONL_X = PCM_SIMPLE_AFACEL_X + PCM_SIMPLE_AFACEL_W - PCM_SIMPLE_ICONS_W,
    PCM_SIMPLE_AICONL_Y = PCM_SIMPLE_AFACEL_Y + PCM_SIMPLE_AFACEL_H - PCM_SIMPLE_ICONS_H,
    PCM_SIMPLE_AICONR_X = PCM_SIMPLE_AICONL_X + 16,
    PCM_SIMPLE_AICONR_Y = PCM_SIMPLE_AICONL_Y,

    PCM_DOUBLING_SIMPLE_MULT_X = 2,
    PCM_DOUBLING_SIMPLE_MULT_Y = 2,
    PCM_DOUBLING_SIMPLE_TEXT_X = 8,
    PCM_DOUBLING_SIMPLE_TEXT_Y = 2,

    PCM_DOUBLING_SIMPLE_TEXT_W = 15,
    PCM_DOUBLING_SIMPLE_TEXT_H = 12,
    PCM_DOUBLING_SIMPLE_DFT_X  = PCM_SIMPLE_DMG_DSTAT_X - 26,
    PCM_DOUBLING_SIMPLE_DFT_Y  = PCM_SIMPLE_DMG_DSTAT_Y -  2,
    PCM_DOUBLING_SIMPLE_AGG_X  = PCM_SIMPLE_DMG_ASTAT_X + 22,
    PCM_DOUBLING_SIMPLE_AGG_Y  = PCM_SIMPLE_DMG_ASTAT_Y -  2,

    PCM_DOUBLING_MATH_TEXT_W   = 15,
    PCM_DOUBLING_MATH_TEXT_H   = 12,
    PCM_DOUBLING_SPLIT_DFT_X   = PCM_SIMPLE_DMG_DSTAT_X - 34,
    PCM_DOUBLING_SPLIT_DFT_Y   = PCM_SIMPLE_DMG_DSTAT_Y -  2,
    PCM_DOUBLING_SPLIT_AGG_X   = PCM_SIMPLE_DMG_ASTAT_X + 30,
    PCM_DOUBLING_SPLIT_AGG_Y   = PCM_SIMPLE_DMG_ASTAT_Y -  2,

    PCM_DOUBLING_SPLIT_TEXT_W  = 15,
    PCM_DOUBLING_SPLIT_TEXT_H  = 12,
    PCM_DOUBLING_MATH_DFT_X    = PCM_MATH_ATK_DSTAT_X - 40,
    PCM_DOUBLING_MATH_DFT_Y    = PCM_MATH_ATK_DSTAT_Y -  2,
    PCM_DOUBLING_MATH_AGG_X    = PCM_MATH_ATK_ASTAT_X + 23,
    PCM_DOUBLING_MATH_AGG_Y    = PCM_MATH_ATK_ASTAT_Y -  2,

    PCM_DOUBLING_DARK   = 45,
    PCM_DOUBLING_LIGHT  = 47,
};

enum PRE_COMBAT_MENU_TYPE {
    PCM_MODE_NULL       = 0,
    PCM_MODE_TOTAL      = 1, /* Show total damage */
    PCM_MODE_SIMPLE     = 2,
    PCM_MODE_MATH       = 3,
    PCM_MODE_NUM        = 4,
};

enum PRE_COMBAT_MENU_ELEMS {
    PRE_COMBAT_MENU_ELEM_NULL = -1,
    PRE_COMBAT_MENU_ELEM      =  0,
    PRE_COMBAT_MENU_ELEM_NUM  =  1,
};

extern struct MenuElemDirections pcm_links[PRE_COMBAT_MENU_ELEM_NUM];
extern struct Point pcm_elem_pos[PRE_COMBAT_MENU_ELEM_NUM];
extern struct Point pcm_elem_box[PRE_COMBAT_MENU_ELEM_NUM];

struct PreCombatMenu {
    struct Point pos; // [pixels], MENU_POS_bOFFSET = 0
    int mode;
    SDL_Texture *texture;
    SDL_Texture *texture_weapons;
    SDL_Texture *texture_face_aggressor;
    SDL_Texture *texture_face_defendant;
    SDL_Texture *texture_doubling;  // actually can render any number of phases

    tnecs_entity_t defendant;
    tnecs_entity_t aggressor;

    struct Unit *dft_unit;
    struct Unit *agg_unit;

    struct Position *dft_pos;
    struct Position *agg_pos;

    struct Game *sota;
    struct Combat_Forecast *forecast;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;

    bool update : 1;
};
extern struct PreCombatMenu PreCombatMenu_default;

/* --- Constructors/Destructors --- */
struct PreCombatMenu *PreCombatMenu_Alloc();
void PreCombatMenu_Free(      struct PreCombatMenu *pcm);
void PreCombatMenu_Free_Icons(struct PreCombatMenu *pcm);
void PreCombatMenu_Free_Faces(struct PreCombatMenu *pcm);

/* --- Loading --- */
void PreCombatMenu_Load(struct PreCombatMenu *pcm, tnecs_entity_t a,
                        tnecs_entity_t d, SDL_Renderer *r, struct n9Patch *n9patch);
void _PreCombatMenu_Load(struct PreCombatMenu *pcm, struct Unit *a, struct Unit *d,
                         struct Position *ap, struct Position *dp, SDL_Renderer *r);

static void _PreCombatMenu_Load_Icons(struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Load_Faces(struct PreCombatMenu *pcm, SDL_Renderer *r);

/* --- Setters --- */
void PreCombatMenu_Set(struct PreCombatMenu *pcm, struct Game *sota);

/* --- Positioning --- */
void PreCombatMenu_Elem_Pos(struct PreCombatMenu *pcm, struct MenuComponent *mc);

/* --- Drawing --- */
void PreCombatMenu_Draw(  struct MenuComponent *mc, SDL_Texture *rt, SDL_Renderer *r);
void PreCombatMenu_Update(struct PreCombatMenu *pcm, struct n9Patch *n9patch,
                          SDL_Texture *rt, SDL_Renderer *r);

/* -- Drawing Utilities -- */
static void _PreCombatMenu_Draw_Names(       struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_Stats(       struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_Faces(       struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_WpnIcons(    struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_Doubling(    struct PreCombatMenu *pcm, SDL_Renderer *r);

/* -- Drawing Stats mode FSM -- */
typedef void (*pcm_draw_stats_t)(struct PreCombatMenu *, SDL_Renderer *r);
extern pcm_draw_stats_t pcm_draw_stats[PCM_MODE_NUM];
static void _PreCombatMenu_Draw_Stats_Math(  struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_Stats_Total( struct PreCombatMenu *pcm, SDL_Renderer *r);
static void _PreCombatMenu_Draw_Stats_Simple(struct PreCombatMenu *pcm, SDL_Renderer *r);

#endif /* PRE_COMBAT_MENU_H */
