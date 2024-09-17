
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

canEquip canEquip_default = {
    // Try to equip:
    /* 1. Weapon in equipment[eq] slot */
    .eq         = -1,

    /* 2. In right hand */
    .hand       = UNIT_HAND_RIGHT,

    /* 3. Ignoring current loadout */
    .loadout    = {
        ITEM_UNEQUIPPED,
        ITEM_UNEQUIPPED,
        ITEM_UNEQUIPPED,
        ITEM_UNEQUIPPED
    },

    /* 4. Of any archetype */
    .archetype  = ITEM_ARCHETYPE_NULL,
};

struct Damage Damage_default = {{0, 0}, {0, 0}};

struct HP HP_default = {0, 0, 0, 0};

struct Computed_Stats Computed_Stats_default = {0};

struct Timer Timer_default = {
    .time_ns      = 0,
    .frame_count  = 0,
    .reset        = false,
    .paused       = false,
    .limit_ns     = SOTA_ns,
};

struct Range Range_default = {.min = SOTA_MAX_RANGE, .max = SOTA_MIN_RANGE};


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
    .infusion    = SOTA_NON_INFUSEABLE,
    .highlighted = true,
};

struct Inventory_item Inventory_item_broken = {
    .id          = ITEM_ID_BROKEN,
    .used        = 0,
    .infusion    = SOTA_NON_INFUSEABLE,
    .highlighted = true,
};

struct Bonus_Stats Bonus_Stats_default = {0};
struct Aura Aura_default = {0};

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

void Computed_Stats_Print(struct Computed_Stats *stats) {
    SDL_Log("atk_P:   %3d\n", stats->attack[DAMAGE_TYPE_PHYSICAL]);
    SDL_Log("atk_M:   %3d\n", stats->attack[DAMAGE_TYPE_MAGICAL]);
    SDL_Log("atk_T:   %3d\n", stats->attack[DAMAGE_TYPE_TRUE]);
    SDL_Log("prot_P:  %3d\n", stats->protection[DAMAGE_TYPE_PHYSICAL]);
    SDL_Log("prot_M:  %3d\n", stats->protection[DAMAGE_TYPE_MAGICAL]);
    SDL_Log("hit:     %3d\n", stats->hit);
    SDL_Log("dodge:   %3d\n", stats->dodge);
    SDL_Log("crit:    %3d\n", stats->crit);
    SDL_Log("favor:   %3d\n", stats->favor);
    SDL_Log("move:    %3d\n", stats->move);
    SDL_Log("speed:   %3d\n", stats->speed);
    SDL_Log("agony:   %3d\n", stats->agony);
    SDL_Log("range_equipment.min: %3d\n", stats->range_equipment.min);
    SDL_Log("range_equipment.max: %3d\n", stats->range_equipment.max);
    SDL_Log("range_loadout.min:  %3d\n", stats->range_loadout.min);
    SDL_Log("range_loadout.max:  %3d\n", stats->range_loadout.max);
}

void Computed_Stats_Compare(struct Computed_Stats *stats1, struct Computed_Stats *stats2) {
    SDL_Log("atk_P:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_PHYSICAL],
            stats2->attack[DAMAGE_TYPE_PHYSICAL]);
    SDL_Log("atk_M:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_MAGICAL],
            stats2->attack[DAMAGE_TYPE_MAGICAL]);
    SDL_Log("atk_T:   %3d, %3d\n", stats1->attack[DAMAGE_TYPE_TRUE],
            stats2->attack[DAMAGE_TYPE_TRUE]);
    SDL_Log("prot_P:  %3d, %3d\n", stats1->protection[DAMAGE_TYPE_PHYSICAL],
            stats2->protection[DAMAGE_TYPE_PHYSICAL]);
    SDL_Log("prot_M:  %3d, %3d\n", stats1->protection[DAMAGE_TYPE_MAGICAL],
            stats2->protection[DAMAGE_TYPE_MAGICAL]);
    SDL_Log("hit:     %3d, %3d\n", stats1->hit, stats2->hit);
    SDL_Log("dodge:   %3d, %3d\n", stats1->dodge, stats2->dodge);
    SDL_Log("crit:    %3d, %3d\n", stats1->crit, stats2->crit);
    SDL_Log("favor:   %3d, %3d\n", stats1->favor, stats2->favor);
    SDL_Log("move:    %3d, %3d\n", stats1->move, stats2->move);
    SDL_Log("speed:   %3d, %3d\n", stats1->speed, stats2->speed);
    SDL_Log("agony:   %3d, %3d\n", stats1->agony, stats2->agony);
    SDL_Log("range_equipment.min: %3d\n", stats1->range_equipment.min, stats2->range_equipment.min);
    SDL_Log("range_equipment.max: %3d\n", stats1->range_equipment.max, stats2->range_equipment.max);
    SDL_Log("range_loadout.min:  %3d\n", stats1->range_loadout.min, stats2->range_loadout.min);
    SDL_Log("range_loadout.max:  %3d\n", stats1->range_loadout.max, stats2->range_loadout.max);
}

