#ifndef PU_H
#define PU_H

#include "enums.h"
#include "n9patch.h"
#include "game/game.h"
#include "popup/popup.h"
#include "bars/stat.h"
#include "slider.h"
#include "unit/unit.h"
#include "pixelfonts.h"
#include "SDL.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Settings;
struct SliderOffscreen;
struct cJSON;

enum POPUP_UNIT {
    PU_NULL             = -1,

    PU_TEXT_SCALE_X     =  1,
    PU_TEXT_SCALE_Y     =  1,
    PU_N9PATCH_SCALE_X  =  4,
    PU_N9PATCH_SCALE_Y  =  4,
    PU_PATCH_X_SIZE     = 14,
    PU_PATCH_Y_SIZE     =  8,
    PU_PATCH_PIXELS     =  8,

    PU_LINESPACE        =  8,

    PU_HEADER_Y         =  0,
    PU_HEADER_W         = 24,
    PU_HEADER_H         =  5,
    PU_HEADER_X         = PU_PATCH_X_SIZE * PU_PATCH_PIXELS / 2 - PU_HEADER_W / 2,

    PU_ID_X             = 28,
    PU_ID_Y             =  7,

    /* Computed stats */
    PU_HIT_X        = 57,
    PU_HIT_Y        = PU_ID_Y  + PU_LINESPACE,
    PU_HIT_X_STAT   = PU_HIT_X + 19,
    PU_HIT_Y_STAT   = PU_HIT_Y,

    PU_CRIT_X       = PU_HIT_X  -  5,
    PU_CRIT_Y       = PU_HIT_Y  + PU_LINESPACE,
    PU_CRIT_X_STAT  = PU_CRIT_X + 28,
    PU_CRIT_Y_STAT  = PU_CRIT_Y,

    PU_ATK_X        = PU_HIT_X  -  1,
    PU_ATK_Y        = PU_CRIT_Y + PU_LINESPACE,
    PU_ATK_X_STAT1  = PU_ATK_X  + 24,
    PU_ATK_Y_STAT1  = PU_ATK_Y,

    PU_PROT_X       = PU_ATK_X  + 1,
    PU_PROT_Y       = PU_ATK_Y  + PU_LINESPACE,
    PU_PROT_X_STAT1 = PU_PROT_X + 23,
    PU_PROT_Y_STAT1 = PU_PROT_Y,

    PU_SPEED_X      = PU_HIT_X   - 10,
    PU_SPEED_Y      = PU_PROT_Y  + PU_LINESPACE,
    PU_SPEED_X_STAT = PU_SPEED_X + 40,
    PU_SPEED_Y_STAT = PU_SPEED_Y,

    PU_FACTOR_X     = 200,
    PU_FACTOR_Y     = 200,

    PU_EXP_X        = 57,
    PU_EXP_Y        = PU_ID_Y,
    PU_EXP_STAT_X   = PU_EXP_X + 17,
    PU_EXP_STAT_Y   = PU_EXP_Y,
    PU_LV_X         = PU_EXP_X + 28,
    PU_LV_Y         = PU_EXP_Y,
    PU_LV_STAT_X    = PU_LV_X  + 10,
    PU_LV_STAT_Y    = PU_LV_Y,

    /* Portrait: face + item icons */
    PU_PORTRAIT_X   =  9,
    PU_PORTRAIT_Y   = PU_ID_Y + PU_LINESPACE + 1,
    PU_PORTRAIT_W   = 36,
    PU_PORTRAIT_H   = 36,

    PU_WPN_ICON_ROWLEN  =  8,
    PU_WPN_ICON_H       = 16,
    PU_WPN_ICON_W       = 16,

    /* L/R from face PoV */
    PU_ICONR_X = PU_PORTRAIT_X,
    PU_ICONR_Y = PU_PORTRAIT_Y + PU_PORTRAIT_W - PU_WPN_ICON_H,
    PU_ICONL_X = PU_PORTRAIT_X + PU_PORTRAIT_H - PU_WPN_ICON_W,
    PU_ICONL_Y = PU_PORTRAIT_Y + PU_PORTRAIT_W - PU_WPN_ICON_H,

    PU_HYPHEN_OFFSET_X = 4,
    PU_HYPHEN_OFFSET_Y = 3,

    PU_HP_X         = PU_PORTRAIT_X,
    PU_HP_Y         = PU_PORTRAIT_Y + PU_PORTRAIT_H + PU_LINESPACE - 7,
    PU_HP_STAT_X    = PU_HP_X + 14,
    PU_HP_STAT_Y    = PU_HP_Y,
    PU_HPBAR_X      = PU_HP_X + 38,
    PU_HPBAR_Y      = PU_HP_Y +  2,
    PU_HPBAR_LEN    = 55,
};

#define POPUP_POS(xory, offset) (offset * n9patch->scale.xory)

typedef struct PopUp_Unit {
    b32 update;
    struct Unit *unit;  /* goes offscreen if NULL */
    int distance; /* distance from unit to enemy on tilemap */
    SDL_Texture *texture;
    SDL_Texture *texture_header;
    SDL_Texture *texture_weapons;
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    u32 corner; /* SOTA_DIRECTION_DIAGONAL */
    struct Point offset; /* amount popup moves in one frame?*/
    struct Point cursor_limit_min;
    struct Point cursor_limit_max;
} PopUp_Unit;
extern struct PopUp_Unit PopUp_Unit_default;

/* --- Constructors/Destructors --- */
void PopUp_Unit_Free(struct PopUp_Unit *pu);
void PopUp_Unit_Load(struct PopUp_Unit *pu, SDL_Renderer *r, struct n9Patch *n9);

/* --- Positioning --- */
void PopUp_Unit_Limits(struct PopUp *p, struct PopUp_Unit *pu, struct Settings *s);

struct Point PopUp_Unit_Offset(  struct PopUp_Unit *pu, struct Settings *s);
struct Point PopUp_Unit_Position(struct PopUp *p,       struct PopUp_Unit *pu,
                                 struct n9Patch *n9patch, struct Settings *s,
                                 struct Point *tp);

/* --- Setters --- */
void _PopUp_Unit_Set(struct PopUp_Unit *pu, struct Unit *unit);
void  PopUp_Unit_Set(struct PopUp_Unit *pu, struct Game *sota);

/* --- Drawing --- */
void PopUp_Unit_Draw(struct PopUp *p, struct Point pos,
                     SDL_Texture *rt, SDL_Renderer *r);
void PopUp_Unit_Update(struct PopUp_Unit *pu, struct n9Patch *n9patch,
                       SDL_Texture *rt, SDL_Renderer *r);

#endif /* PU_H */
