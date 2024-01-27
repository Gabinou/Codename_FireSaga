#ifndef PLS_H
#define PLS_H

#include "enums.h"
#include "n9patch.h"
#include "popup/popup.h"
#include "slider.h"
#include "unit/unit.h"
#include "tnecs.h"
#include "pixelfonts.h"
#include "SDL2/SDL.h"
#include "menu/loadout_select.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct cJSON;
struct Settings;
struct Menu;
struct LoadoutSelectMenu;

/* --- ENUMS --- */
enum STAT_ARROWS {
    STAT_ARROW_NULL = -1,
    STAT_ARROW_UP   =  0,
    STAT_ARROW_DOWN =  1,
};

enum PLS_ICON_EQUIP {
    PLS_ICON_EQUIP_W        =   7,
    PLS_ICON_EQUIP_H        =  10,
};

enum PLS_HANDS {
    PLS_HANDS_TILESIZE      =  16,
    PLS_HANDS_NONE          =  -1,
    PLS_HANDS_BIG_L         =   0,
    PLS_HANDS_SMALL_L       =   1,
    PLS_HANDS_SMALL_R       =   2,
    PLS_HANDS_BIG_R         =   3,

    PLS_HAND_SMALLX_OFFSET  =   2,
    PLS_HAND_SMALLY_OFFSET  =   3,

    PLS_TWOHAND_Y_OFFSET    =   6,
    PLS_WEAKHAND_Y_OFFSET   =  11,
};

enum POPUP_LOADOUT_STATS {
    PLS_NULL            = -1,

    PLS_TEXT_SCALE_X    =  1,
    PLS_TEXT_SCALE_Y    =  1,
    PLS_N9PATCH_SCALE_X =  4,
    PLS_N9PATCH_SCALE_Y =  4,
    PLS_PATCH_X_SIZE    = 22,
    PLS_PATCH_Y_SIZE    = 10,
    PLS_PATCH_PIXELS    =  8,

    PLS_HANDL_X         =  6,
    PLS_HANDL_Y         =  9,
    PLS_HANDR_X         =  PLS_PATCH_X_SIZE * PLS_PATCH_PIXELS - PLS_HANDS_TILESIZE - 1,
    PLS_HANDR_Y         =  9,

    PLS_DEST_X          =  0,
    PLS_DEST_Y          =  2,

    PLS_HEADER_Y        =  0,
    PLS_HEADER_W        = 24,
    PLS_HEADER_H        =  5,
    PLS_HEADER_X        = PLS_PATCH_X_SIZE * PLS_PATCH_PIXELS / 2 - PLS_HEADER_W / 2,

    /* WEAPON ICONS */
    PLS_WPN_TILESIZE    =  16,
    PLS_WPN_COL_LEN     =   8,
    PLS_WPNL_X          = PLS_HANDL_X +  9,
    PLS_WPNL_Y          = PLS_HANDL_Y -  1,
    PLS_WPNR_X          = PLS_HANDR_X - PLS_WPN_TILESIZE - 2,
    PLS_WPNR_Y          = PLS_HANDR_Y -  1,
    PLS_WPN_W           = 16,
    PLS_WPN_H           = 16,

    /* EQUIP ICONS */
    PLS_ICON_EQUIPL_X   = PLS_WPNL_X + PLS_WPN_TILESIZE + 1,
    PLS_ICON_EQUIPL_Y   = PLS_WPNL_Y + 3,
    PLS_ICON_EQUIPR_X   = PLS_WPNR_X - PLS_ICON_EQUIP_W - 2,
    PLS_ICON_EQUIPR_Y   = PLS_WPNR_Y + 3,

    /* WEAPON NAMES */
    PLS_NAMEL_X         = PLS_ICON_EQUIPL_X + PLS_ICON_EQUIP_W + 4,
    PLS_NAMEL_Y         = PLS_WPNL_Y + 4,

    PLS_NAMER_X         = PLS_ICON_EQUIPR_X - PLS_ICON_EQUIP_W + 4,
    PLS_NAMER_Y         = PLS_WPNR_Y + 4,

    /* ARROW ICON */
    PLS_ARROW_W         =  8,
    PLS_ARROW_H         = 16,

    /* Computed stats */
    PLS_ATK_X           = 22,
    PLS_ATK_Y           = 37,
    PLS_ATK_X_STAT      = PLS_ATK_X         +  6,
    PLS_ATK_Y_STAT      = PLS_ATK_Y         +  9,
    PLS_ATK_LARROW_X    = PLS_ATK_X_STAT    - 20,
    PLS_ATK_LARROW_Y    = PLS_ATK_Y_STAT    -  1,
    PLS_ATK_RARROW_X    = PLS_ATK_X_STAT    + 16,
    PLS_ATK_RARROW_Y    = PLS_ATK_Y_STAT    -  1,

    PLS_PROT_X          = PLS_ATK_X         + 1,
    PLS_PROT_Y          = PLS_ATK_Y         + 21,
    PLS_PROT_X_STAT     = PLS_PROT_X        +  5,
    PLS_PROT_Y_STAT     = PLS_PROT_Y        +  9,
    PLS_PROT_LARROW_X   = PLS_PROT_X_STAT   - 20,
    PLS_PROT_LARROW_Y   = PLS_PROT_Y_STAT   -  1,
    PLS_PROT_RARROW_X   = PLS_PROT_X_STAT   + 16,
    PLS_PROT_RARROW_Y   = PLS_PROT_Y_STAT   -  1,

    PLS_HIT_X           = PLS_ATK_X         + 58,
    PLS_HIT_Y           = PLS_ATK_Y         +  0,
    PLS_HIT_X_STAT      = PLS_HIT_X         +  5,
    PLS_HIT_Y_STAT      = PLS_HIT_Y         +  9,
    PLS_HIT_LARROW_X    = PLS_HIT_X_STAT    - 24,
    PLS_HIT_LARROW_Y    = PLS_HIT_Y_STAT    -  1,
    PLS_HIT_RARROW_X    = PLS_HIT_X_STAT    + 20,
    PLS_HIT_RARROW_Y    = PLS_HIT_Y_STAT    -  1,

    PLS_CRIT_X          = PLS_HIT_X         -  3,
    PLS_CRIT_Y          = PLS_HIT_Y         + 21,
    PLS_CRIT_X_STAT     = PLS_CRIT_X        +  8,
    PLS_CRIT_Y_STAT     = PLS_CRIT_Y        +  9,
    PLS_CRIT_LARROW_X   = PLS_CRIT_X_STAT   - 24,
    PLS_CRIT_LARROW_Y   = PLS_CRIT_Y_STAT   -  1,
    PLS_CRIT_RARROW_X   = PLS_CRIT_X_STAT   + 20,
    PLS_CRIT_RARROW_Y   = PLS_CRIT_Y_STAT   -  1,

    PLS_RANGE_X         = PLS_HIT_X         + 52,
    PLS_RANGE_Y         = PLS_HIT_Y         +  0,
    PLS_RANGE_X_STAT    = PLS_RANGE_X       +  3,
    PLS_RANGE_Y_STAT    = PLS_RANGE_Y       +  9,
    PLS_RANGE_LARROW_X  = PLS_RANGE_X_STAT  - 11,
    PLS_RANGE_LARROW_Y  = PLS_RANGE_Y_STAT  -  1,
    PLS_RANGE_RARROW_X  = PLS_RANGE_X_STAT  + 20,
    PLS_RANGE_RARROW_Y  = PLS_RANGE_Y_STAT  -  1,

    PLS_SPEED_X         = PLS_RANGE_X       +  0,
    PLS_SPEED_Y         = PLS_RANGE_Y       + 21,
    PLS_SPEED_X_STAT    = PLS_SPEED_X       + 11,
    PLS_SPEED_Y_STAT    = PLS_SPEED_Y       +  9,
    PLS_SPEED_ARROW_X   = PLS_SPEED_X_STAT  - 16,
    PLS_SPEED_ARROW_Y   = PLS_SPEED_Y_STAT  -  1,
};

enum PLS_ELEMS {
    PLS_ELEM_NULL = -1,
    PLS_ELEM_HANDL,
    PLS_ELEM_HANDR,
    PLS_ELEM_ICONL,
    PLS_ELEM_ICONR,
    PLS_ELEM_ATK,
    PLS_ELEM_PROT,
    PLS_ELEM_RANGE,
    PLS_ELEM_HIT,
    PLS_ELEM_CRIT,
    PLS_ELEM_SPEED,
    PLS_ELEM_NUM,
};

typedef struct PopUp_Loadout_Stats {
    b32 update;
    tnecs_entity unit_ent;
    struct Unit *unit;
    SDL_Texture *texture;
    SDL_Texture *texture_arrows;
    SDL_Texture *texture_hands;
    SDL_Texture *texture_weapon_icons;
    SDL_Texture *texture_equip;
    SDL_Texture *texture_header;
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    struct Computed_Stats previous_cs;
    struct Computed_Stats new_cs;
    int item_left;  /* item side space */
    int item_right; /* item side space */
    i16 type_left;
    i16 type_right;
    int ly_offset;
    int ry_offset;

    int distance;
    bool l_equip_override   : 1;
    bool r_equip_override   : 1;
    bool tophand_stronghand : 1; // If false, tophand is lefthand
    bool twoHanding         : 1; // If false, tophand is lefthand
} PopUp_Loadout_Stats;

extern struct PopUp_Loadout_Stats PopUp_Loadout_Stats_default;

/* --- ructor/Destructor --- */
void PopUp_Loadout_Stats_Free(struct PopUp_Loadout_Stats *pls);
void PopUp_Loadout_Stats_Load(struct PopUp_Loadout_Stats *pls, SDL_Renderer *r,
                              struct n9Patch *n9);

/* --- Setters --- */
void  PopUp_Loadout_Stats_New(     struct PopUp_Loadout_Stats *pls);
void  PopUp_Loadout_Stats_Unit(    struct PopUp_Loadout_Stats *pls, struct Unit *u);
void  PopUp_Loadout_Stats_Previous(struct PopUp_Loadout_Stats *pls);

/* --- Select --- */
void PopUp_Loadout_Stats_Hover(struct PopUp_Loadout_Stats *pls,
                               struct LoadoutSelectMenu *wsm, int elem);

/* --- Internals --- */
void PopUp_Loadout_Stats_ItemTypes(      struct PopUp_Loadout_Stats *pls);
void PopUp_Loadout_Stats_Weakhand_Offset(struct PopUp_Loadout_Stats *pls);

/* --- Rendering --- */
void PopUp_Loadout_Stats_Draw(struct PopUp *p, struct Point pos,
                              SDL_Texture *rt, SDL_Renderer *r);
void PopUp_Loadout_Stats_Update(struct PopUp_Loadout_Stats *pls,
                                struct n9Patch *n9patch,
                                SDL_Texture *rt, SDL_Renderer *r);

#endif /* PLS_H */
