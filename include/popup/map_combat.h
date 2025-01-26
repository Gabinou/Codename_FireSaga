#ifndef POPUP_MAP_COMBAT_H
#define POPUP_MAP_COMBAT_H

#include "enums.h"
#include "structs.h"
#include "tnecs.h"
#include "n9patch.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct PopUp;
struct Settings;
struct cJSON;

enum POPUP_MAP_COMBAT {
    POPUP_MAP_COMBAT_PATCH_SIZE_X  = 8,
    POPUP_MAP_COMBAT_PATCH_SIZE_Y  = 5,

    PMC_PATCH_PIXELS    = 8,

    PMC_N9PATCH_SCALE_X = 5,
    PMC_N9PATCH_SCALE_Y = 5,

    POPUP_MAP_COMBAT_BLUE_DMG_X          = 37,
    POPUP_MAP_COMBAT_BLUE_DMG_Y          = 23,
    POPUP_MAP_COMBAT_BLUE_HIT_X          = 7,
    POPUP_MAP_COMBAT_BLUE_HIT_Y          = POPUP_MAP_COMBAT_BLUE_DMG_Y,
    POPUP_MAP_COMBAT_BLUE_CRIT_X         = 3,
    POPUP_MAP_COMBAT_BLUE_CRIT_Y         = 32,

    POPUP_MAP_COMBAT_BLUE_HIT_STAT_X     = POPUP_MAP_COMBAT_BLUE_HIT_X + 28,
    POPUP_MAP_COMBAT_BLUE_DMG_STAT_X     = POPUP_MAP_COMBAT_BLUE_DMG_X + 19,
    POPUP_MAP_COMBAT_BLUE_CRIT_STAT_X    = POPUP_MAP_COMBAT_BLUE_HIT_STAT_X,

    POPUP_MAP_COMBAT_RED_DMG_X           = 76,
    POPUP_MAP_COMBAT_RED_DMG_Y           = POPUP_MAP_COMBAT_BLUE_DMG_Y,
    POPUP_MAP_COMBAT_RED_HIT_X           = 110,
    POPUP_MAP_COMBAT_RED_HIT_Y           = POPUP_MAP_COMBAT_BLUE_HIT_Y,
    POPUP_MAP_COMBAT_RED_CRIT_X          = POPUP_MAP_COMBAT_RED_HIT_X,
    POPUP_MAP_COMBAT_RED_CRIT_Y          = POPUP_MAP_COMBAT_BLUE_CRIT_Y,

    POPUP_MAP_COMBAT_RED_HIT_STAT_X      = POPUP_MAP_COMBAT_RED_HIT_X - 3,
    POPUP_MAP_COMBAT_RED_DMG_STAT_X      = POPUP_MAP_COMBAT_RED_DMG_X - 6,
    POPUP_MAP_COMBAT_RED_CRIT_STAT_X     = POPUP_MAP_COMBAT_RED_HIT_STAT_X,

    POPUP_MAP_COMBAT_BLUE_CIRCLE_BAR_X   = 39,
    POPUP_MAP_COMBAT_RED_CIRCLE_BAR_X    = 80,
    POPUP_MAP_COMBAT_CIRCLE_BAR_Y        = 30,
    POPUP_MAP_COMBAT_CIRCLE_DIST         = 2,

    POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_X   = 16,
    POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_Y   = 17,
    POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_LEN = 45,

    POPUP_MAP_COMBAT_RED_TOPOFF_BAR_X    = 67,
    POPUP_MAP_COMBAT_RED_TOPOFF_BAR_Y    = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_Y,
    POPUP_MAP_COMBAT_RED_TOPOFF_BAR_LEN  = 45,

    POPUP_MAP_COMBAT_BLUE_HP_X           = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_X -  8,
    POPUP_MAP_COMBAT_BLUE_HP_Y           = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_Y -  1,
    POPUP_MAP_COMBAT_RED_HP_X            = POPUP_MAP_COMBAT_RED_TOPOFF_BAR_X  + 53,
    POPUP_MAP_COMBAT_RED_HP_Y            = POPUP_MAP_COMBAT_RED_TOPOFF_BAR_Y  -  1,

    POPUP_MAP_COMBAT_HEADER_PLUS_HEIGHT = 3,

    POPUP_MAP_COMBAT_HEADER_BLUE_X      = 22,
    POPUP_MAP_COMBAT_HEADER_BLUE_Y      = 0,

    POPUP_MAP_COMBAT_HEADER_RED_X       = 84,
    POPUP_MAP_COMBAT_HEADER_RED_Y       = 0,

    POPUP_MAP_COMBAT_HEADER_W           = 24,
    POPUP_MAP_COMBAT_HEADER_H           = 5,

    POPUP_MAP_COMBAT_PATCH_BLUE_NAME_X  = 31,
    POPUP_MAP_COMBAT_PATCH_BLUE_NAME_Y  = 7,
    POPUP_MAP_COMBAT_PATCH_RED_NAME_X   = 88,
    POPUP_MAP_COMBAT_PATCH_RED_NAME_Y   = 7,
};

typedef struct PopUp_Map_Combat {
    b32 update;

    tnecs_world  *world;

    tnecs_entity aggressor;
    tnecs_entity defendant;
    SDL_Texture *texture;
    struct PixelFont *pixelnours_big;
    struct PixelFont *pixelnours_tight;

    struct SimpleBar topoff_aggressor;
    struct SimpleBar topoff_defendant;
    struct CircleBar circlebar_agg;
    struct CircleBar circlebar_dft;

    SDL_Texture *texture_n9patch_red;
    SDL_Texture *texture_n9patch_blue;
    SDL_Texture *texture_header_red;
    SDL_Texture *texture_header_blue;

    struct Combat_Forecast *forecast;
    struct Combat_Phase    *phases;

    int current_attack;

    SDL_Palette *palette;
} PopUp_Map_Combat;
extern const struct PopUp_Map_Combat PopUp_Map_Combat_default;

/* --- Constructors/Destructors --- */
void PopUp_Map_Combat_Load(struct PopUp_Map_Combat *pmc, SDL_Renderer *r,
                           struct n9Patch *n);
void PopUp_Map_Combat_Free(struct PopUp_Map_Combat *pmc);

/* --- Setters --- */
void PopUp_Map_Combat_Units(struct PopUp_Map_Combat *pmc, struct Game *sota,
                            tnecs_entity aggressor, tnecs_entity defendant);

struct n9Patch PopUp_Map_Combat_Compute_Patch(struct PopUp_Map_Combat *pmc,
                                              struct n9Patch *n9patch);

/* --- Drawing --- */
void PopUp_Map_Combat_Draw(struct PopUp *popup, struct Point pos,
                           SDL_Texture *rt, SDL_Renderer *r);
void PopUp_Map_Combat_Update(struct PopUp_Map_Combat *pmc, struct n9Patch *n9patch,
                             SDL_Texture *rt, SDL_Renderer *r);

#endif /* POPUP_MAP_COMBAT_H */
