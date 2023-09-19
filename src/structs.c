
#include "structs.h"

struct nmath_hexpoint_int32_t Cube_Direction_xp =   { 0,  1, -1};
struct nmath_hexpoint_int32_t Cube_Direction_xm =   { 0, -1,  1};
struct nmath_hexpoint_int32_t Cube_Direction_yp =   { 1,  0, -1};
struct nmath_hexpoint_int32_t Cube_Direction_ym =   {-1,  0,  2};
struct nmath_hexpoint_int32_t Cube_Direction_zp =   { 1, -1,  0};
struct nmath_hexpoint_int32_t Cube_Direction_zm =   {-1,  1,  0};
struct nmath_hexpoint_int32_t Cube_Diagonal_xp =    { 1,  1, -2};
struct nmath_hexpoint_int32_t Cube_Diagonal_xm =    {-1, -1,  2};
struct nmath_hexpoint_int32_t Cube_Diagonal_yp =    {-2,  1,  1};
struct nmath_hexpoint_int32_t Cube_Diagonal_ym =    { 2, -1, -1};
struct nmath_hexpoint_int32_t Cube_Diagonal_zp =    { 1, -2,  1};
struct nmath_hexpoint_int32_t Cube_Diagonal_zm =    {-1,  2, -1};

struct Damage Damage_default = {{0, 0}, {0, 0}};
struct CombatAnimation CombatAnimation_default = {
    .attack_ind =          0,
    .pause_before_ms =    10,
    .pause_after_ms =   1000,
};

struct HP HP_default = {0, 0, 0, 0};

struct Timer Timer_default = {
    .time_ns      = 0,
    .frame_count  = 0,
    .reset        = false,
    .paused       = false,
};

struct Rendered Rendered_default = {
    .graphics.texture   = NULL,
    .flip               = SDL_FLIP_NONE,
    .angle              = 0,
};

struct Tile_stats Tile_stats_default = {
    .dodge = -1,
    .Pprot = -1,
    .Mprot = -1,
    .heal  = -1
};

struct Inventory_item Inventory_item_default = {
    .id          = ITEM_NULL,
    .used        = 0,
    .infusion    = SOTA_INFUSEABLE,
    .highlighted = true,
};

struct Inventory_item Inventory_item_broken = {
    .id          = ITEM_ID_BROKEN,
    .used        = UINT8_MAX,
    .infusion    = SOTA_NON_INFUSEABLE,
    .highlighted = true,
};

struct Shop Shop_default = {
    .items = NULL,
    .qty   = NULL,
};

struct Padding Padding_default = {
    .right   = 0,
    .top     = 0,
    .left    = 0,
    .bottom  = 0,
};

struct Item_stats Item_stats_default = {
    .price   = 0,
    .uses    = 0,
    .AP      = 0, /* Ability Power for heal (%), blowHorn, gainStats, gainSkill */
};

struct Weapon_stats Weapon_stats_default = {
    .attack      = {0},
    .protection  = {0},
    .hit         =  0,
    .dodge       =  0, /* can be negative */
    .crit        =  0,
    .favor       =  0,
    .range       = {0}, /* [0]: min, [1]: max */
    .wgt         =  0, /* weight */
    .prof        =  0, /* proficiency */
};

struct Movement_cost Movement_cost_default = {
    .foot_slow    = 0,
    .foot_fast    = 0,
    .mages        = 0,
    .riders_fast  = 0,
    .riders_slow  = 0,
    .fliers       = 0,
    .armors       = 0,
    .pirates      = 0,
    .bandits      = 0
};

struct fMovement_cost fMovement_cost_default = {
    .foot_slow   = 0.0f,
    .foot_fast   = 0.0f,
    .mages       = 0.0f,
    .riders_fast = 0.0f,
    .riders_slow = 0.0f,
    .fliers      = 0.0f,
    .armors      = 0.0f,
    .pirates     = 0.0f,
    .bandits     = 0.0f
};

struct Condition Condition_default = {
    .unitid      = -1,
    .dead        = false,
    .recruited   = true,
};

struct Reinforcement Reinforcement_default = {
    .turn        =  0,
    .levelups    =  0,
    .army        = -1,
    .id          = -1,
    .position    = {0, 0},
};

struct Fps Fps_default = {
    .pos         = {FPS_DEFAULT_POSX,        FPS_DEFAULT_POSY},
    .show        =  FPS_DEFAULT_SHOW,
    .sizefactor  = {FPS_DEFAULT_SIZEFACTORX, FPS_DEFAULT_SIZEFACTORY},
    .cap         =  FPS_DEFAULT_CAP,
    .textcolor   = {FPS_DEFAULT_COLORR,      FPS_DEFAULT_COLORG, FPS_DEFAULT_COLORB}
};

struct Mouse Mouse_default = {
    .onhold = MOUSE_DEFAULT_ONHOLD,
    .move   = MOUSE_DEFAULT_MOVE
};

struct Cursor Cursor_default = {DEFAULT_CURSOR_FRAMES, DEFAULT_CURSOR_SPEED};

struct Settings Settings_default = {
    .title = "Codename:Firesaga",

    .res                = {DEFAULT_RESOLUTION_X, DEFAULT_RESOLUTION_Y},
    .pos                = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
    .fontsize           = 28,
    .fullscreen         = false,
    .tophand_stronghand = true, // TODO: REMOVE
    .tilesize           = {SOTA_TILESIZE, SOTA_TILESIZE},

    .FPS = {
        .pos            = {.x = FPS_DEFAULT_POSX, .y = FPS_DEFAULT_POSY},
        .textcolor      = {
            FPS_DEFAULT_COLORR, FPS_DEFAULT_COLORG,
            FPS_DEFAULT_COLORB, FPS_DEFAULT_COLORA
        },
        .sizefactor     = {FPS_DEFAULT_SIZEFACTORX, FPS_DEFAULT_SIZEFACTORY},
        .show           = FPS_DEFAULT_SHOW,
        .cap            = FPS_DEFAULT_CAP,
    },
    .map_settings = {
        .overlay_mode   = MAP_OVERLAY_MODE_PALETTESWAP,
        .color_grid     = {0, 0, 0, SDL_ALPHA_OPAQUE},
        .grid_show      = true,
        .stack_mode     = MAP_SETTING_STACK_DANGERMAP
    },
    .mouse              = {MOUSE_DEFAULT_ONHOLD, MOUSE_DEFAULT_MOVE},
    .cursor             = {DEFAULT_CURSOR_FRAMES, DEFAULT_CURSOR_SPEED},
};

void Computed_Stats_Print(struct Computed_Stats *stats) {
    SOTA_Log_Debug("atk_P:   %3d\n", stats->attack[DAMAGE_TYPE_PHYSICAL]);
    SOTA_Log_Debug("atk_M:   %3d\n", stats->attack[DAMAGE_TYPE_MAGICAL]);
    SOTA_Log_Debug("atk_T:   %3d\n", stats->attack[DAMAGE_TYPE_TRUE]);
    SOTA_Log_Debug("prot_P:  %3d\n", stats->protection[DAMAGE_TYPE_PHYSICAL]);
    SOTA_Log_Debug("prot_M:  %3d\n", stats->protection[DAMAGE_TYPE_MAGICAL]);
    SOTA_Log_Debug("hit:     %3d\n", stats->hit);
    SOTA_Log_Debug("dodge:   %3d\n", stats->dodge);
    SOTA_Log_Debug("crit:    %3d\n", stats->crit);
    SOTA_Log_Debug("favor:   %3d\n", stats->favor);
    SOTA_Log_Debug("move:    %3d\n", stats->move);
    SOTA_Log_Debug("speed:   %3d\n", stats->speed);
    SOTA_Log_Debug("agony:   %3d\n", stats->agony);
    SOTA_Log_Debug("range_combined.min: %3d\n", stats->range_combined.min);
    SOTA_Log_Debug("range_combined.max: %3d\n", stats->range_combined.max);
    SOTA_Log_Debug("range_loadout.min:  %3d\n", stats->range_loadout.min);
    SOTA_Log_Debug("range_loadout.max:  %3d\n", stats->range_loadout.max);
}

void Computed_Stats_Compare(struct Computed_Stats *stats1, struct Computed_Stats *stats2) {
    SOTA_Log_Debug("atk_P:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_PHYSICAL],
                   stats2->attack[DAMAGE_TYPE_PHYSICAL]);
    SOTA_Log_Debug("atk_M:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_MAGICAL],
                   stats2->attack[DAMAGE_TYPE_MAGICAL]);
    SOTA_Log_Debug("atk_T:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_TRUE],
                   stats2->attack[DAMAGE_TYPE_TRUE]);
    SOTA_Log_Debug("prot_P:  %3d, %3d\n", stats1->protection[DAMAGE_TYPE_PHYSICAL],
                   stats2->protection[DAMAGE_TYPE_PHYSICAL]);
    SOTA_Log_Debug("prot_M:  %3d, %3d\n", stats1->protection[DAMAGE_TYPE_MAGICAL],
                   stats2->protection[DAMAGE_TYPE_MAGICAL]);
    SOTA_Log_Debug("hit:     %3d, %3d\n", stats1->hit, stats2->hit);
    SOTA_Log_Debug("dodge:   %3d, %3d\n", stats1->dodge, stats2->dodge);
    SOTA_Log_Debug("crit:    %3d, %3d\n", stats1->crit, stats2->crit);
    SOTA_Log_Debug("favor:   %3d, %3d\n", stats1->favor, stats2->favor);
    SOTA_Log_Debug("move:    %3d, %3d\n", stats1->move, stats2->move);
    SOTA_Log_Debug("speed:   %3d, %3d\n", stats1->speed, stats2->speed);
    SOTA_Log_Debug("agony:   %3d, %3d\n", stats1->agony, stats2->agony);
    SOTA_Log_Debug("range_combined.min: %3d\n", stats1->range_combined.min, stats2->range_combined.min);
    SOTA_Log_Debug("range_combined.max: %3d\n", stats1->range_combined.max, stats2->range_combined.max);
    SOTA_Log_Debug("range_loadout.min:  %3d\n", stats1->range_loadout.min, stats2->range_loadout.min);
    SOTA_Log_Debug("range_loadout.max:  %3d\n", stats1->range_loadout.max, stats2->range_loadout.max);
}

