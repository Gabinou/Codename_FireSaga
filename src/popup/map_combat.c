
#include "popup/map_combat.h"
#include "popup/popup.h"
#include "bars/topoff.h"
#include "bars/circle.h"
#include "unit/unit.h"
#include "macros.h"
#include "platform.h"
#include "pixelfonts.h"
#include "palette.h"
#include "utilities.h"
#include "filesystem.h"
#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static void _PopUp_Map_Combat_Draw_HP(        struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *r);
static void _PopUp_Map_Combat_Draw_Names(     struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *r);
static void _PopUp_Map_Combat_Draw_Stats(     struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *r);
static void _PopUp_Map_Combat_Draw_Headers(   struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *r);
static void _PopUp_Map_Combat_Draw_CircleBars(struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *r);

struct PopUp_Map_Combat PopUp_Map_Combat_default = {
    .update                 = true,
};

/* --- Constructors/Destructors --- */
void PopUp_Map_Combat_Free(struct PopUp_Map_Combat *pmc) {

    if (pmc->texture_header_blue != NULL) {
        SDL_DestroyTexture(pmc->texture_header_blue);
        pmc->texture_header_blue = NULL;
    }

    if (pmc->texture_header_red != NULL) {
        SDL_DestroyTexture(pmc->texture_header_red);
        pmc->texture_header_red = NULL;
    }

    if (pmc->texture_n9patch_blue != NULL) {
        SDL_DestroyTexture(pmc->texture_n9patch_blue);
        pmc->texture_n9patch_blue = NULL;
    }

    if (pmc->texture_n9patch_red != NULL) {
        SDL_DestroyTexture(pmc->texture_n9patch_red);
        pmc->texture_n9patch_red = NULL;
    }

    if (pmc->texture != NULL) {
        SDL_DestroyTexture(pmc->texture);
        pmc->texture = NULL;
    }

}

void PopUp_Map_Combat_Load(struct PopUp_Map_Combat *pmc, SDL_Renderer *renderer,
                           struct n9Patch *n9patch) {

    n9patch->size_pixels.x    = POPUP_MAP_COMBAT_PATCH_SIZE_X * 2;
    n9patch->patch_pixels.y   = POPUP_MAP_COMBAT_PATCH_SIZE_Y;
    n9patch->scale.x  = PMC_N9PATCH_SCALE_X;
    n9patch->scale.y  = PMC_N9PATCH_SCALE_X;

    /* -- TopOffBar -- */
    /* - bar textures - */
    char *path = PATH_JOIN("..", "assets", "GUI", "Popup", "PopUp_Map_Combat_Header_Blue.png");
    pmc->texture_header_blue = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path = PATH_JOIN("..", "assets", "GUI", "Popup", "PopUp_Map_Combat_Header_Red.png");
    pmc->texture_header_red = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "n9patch_popup_map_combat_red_8px.png");
    pmc->texture_n9patch_red = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "n9patch_popup_map_combat_blue_8px.png");
    pmc->texture_n9patch_blue = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    /* - bar colors - */
    pmc->palette = palette_SOTA;
    pmc->topoff_aggressor.BG_dark   = pmc->palette->colors[TOPOFF_BAR_BG_DARK];
    pmc->topoff_aggressor.BG_light  = pmc->palette->colors[TOPOFF_BAR_BG_LIGHT];
    pmc->topoff_aggressor.FG_dark   = pmc->palette->colors[TOPOFF_BAR_FG_DARK];
    pmc->topoff_aggressor.FG_light  = pmc->palette->colors[TOPOFF_BAR_FG_LIGHT];
    pmc->topoff_defendant.BG_dark   = pmc->palette->colors[TOPOFF_BAR_BG_DARK];
    pmc->topoff_defendant.BG_light  = pmc->palette->colors[TOPOFF_BAR_BG_LIGHT];
    pmc->topoff_defendant.FG_dark   = pmc->palette->colors[TOPOFF_BAR_FG_DARK];
    pmc->topoff_defendant.FG_light  = pmc->palette->colors[TOPOFF_BAR_FG_LIGHT];

    /* - bar ants - */
    pmc->topoff_aggressor.len   = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_LEN;
    pmc->topoff_defendant.len   = POPUP_MAP_COMBAT_RED_TOPOFF_BAR_LEN;
    pmc->topoff_aggressor.pos.x = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_X;
    pmc->topoff_aggressor.pos.y = POPUP_MAP_COMBAT_BLUE_TOPOFF_BAR_Y;
    pmc->topoff_defendant.pos.x = POPUP_MAP_COMBAT_RED_TOPOFF_BAR_X;
    pmc->topoff_defendant.pos.y = POPUP_MAP_COMBAT_RED_TOPOFF_BAR_Y;
    pmc->topoff_defendant.flip  = SDL_FLIP_HORIZONTAL;

    /* -- CircleBar -- */
    pmc->circlebar_agg.pos.y    = POPUP_MAP_COMBAT_CIRCLE_BAR_Y;
    pmc->circlebar_dft.pos.y    = POPUP_MAP_COMBAT_CIRCLE_BAR_Y;
    pmc->circlebar_agg.flip     = SDL_FLIP_HORIZONTAL;

}

void PopUp_Map_Combat_Units(struct PopUp_Map_Combat *pmc, struct Game *sota,
                            tnecs_entity aggressor, tnecs_entity defendant) {

    /* -- Preliminaries -- */
    // tnecs_entity popup_ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    // SDL_assert(popup_ent != TNECS_NULL);
    // struct PopUp *popup = TNECS_GET_COMPONENT(sota->world, popup_ent, PopUp);
    // struct PopUp_Map_Combat *pmc = popup->data;
    // struct Unit *aggressor = TNECS_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    // struct Unit *defendant = TNECS_GET_COMPONENT(sota->world, sota->defendant, Unit);
    // SDL_assert(popup != NULL);
    SDL_assert(pmc          != NULL);
    SDL_assert(aggressor    > TNECS_NULL);
    SDL_assert(defendant    > TNECS_NULL);

    /* -- Update -- */
    pmc->aggressor      = aggressor;
    pmc->defendant      = defendant;
    pmc->world          = sota->world;
    pmc->forecast       = &sota->combat_forecast;
    pmc->phases         = sota->combat_outcome.phases;
    pmc->update         = true;
}

/* --- Drawing --- */
void PopUp_Map_Combat_Draw(struct PopUp *popup, struct Point pos,
                           SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PopUp_Map_Combat *pmc = (struct PopUp_Map_Combat *)popup->data;
    struct n9Patch *n9patch = &popup->n9patch;

    if (pmc->update) {
        PopUp_Map_Combat_Update(pmc, n9patch, render_target, renderer);
        pmc->update = false;
    }
    /* red + blue size_pixels added and put into n9patch->size_pixels by PopUp_Map_Combat_Update*/
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y,
    };
    SDL_assert(dstrect.w > 0);
    SDL_assert(dstrect.x > 0);
    SDL_assert(pmc->texture != NULL);
    SDL_RenderCopy(renderer, pmc->texture, NULL, &dstrect);
}

static void _PopUp_Map_Combat_Draw_HP(struct PopUp_Map_Combat *pmc, SDL_Renderer *renderer) {
    /* --- HP --- */
    char numbuff[10];
    int width;
    struct Point temp_pos;
    SDL_assert(pmc->aggressor > TNECS_NULL);
    SDL_assert(pmc->defendant > TNECS_NULL);
    struct Unit *agg_unit = TNECS_GET_COMPONENT(pmc->world, pmc->aggressor, Unit);
    struct Unit *dft_unit = TNECS_GET_COMPONENT(pmc->world, pmc->defendant, Unit);
    SDL_assert(agg_unit != NULL);
    SDL_assert(dft_unit != NULL);

    struct Unit_stats effective_stats_agg    = Unit_effectiveStats(agg_unit);
    struct Unit_stats effective_stats_dft    = Unit_effectiveStats(dft_unit);

    /* -- HP number -- */
    int toprint = int_inbounds(agg_unit->current_hp, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_BLUE_HP_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_BLUE_HP_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff), temp_pos.x, temp_pos.y);

    toprint = int_inbounds(dft_unit->current_hp, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_RED_HP_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_RED_HP_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    /* -- TopoffBars -- */
    // TODO: update health before/after EACH combat attack animation
    pmc->topoff_aggressor.fill = ((float)agg_unit->current_hp) / ((float)effective_stats_agg.hp);
    pmc->topoff_defendant.fill = ((float)dft_unit->current_hp) / ((float)effective_stats_dft.hp);
    TopoffBar_Draw(&pmc->topoff_aggressor, renderer);
    TopoffBar_Draw(&pmc->topoff_defendant, renderer);

}

static void _PopUp_Map_Combat_Draw_Names(struct PopUp_Map_Combat *pmc, SDL_Renderer *renderer) {
    /* --- Names --- */
    struct Point temp_pos;

    struct Unit *agg_unit = TNECS_GET_COMPONENT(pmc->world, pmc->aggressor, Unit);
    s8 name = agg_unit->name;
    int width = PixelFont_Width(pmc->pixelnours_tight, name.data, name.num);

    temp_pos.x = POPUP_MAP_COMBAT_PATCH_BLUE_NAME_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_PATCH_BLUE_NAME_Y;
    PixelFont_Write(pmc->pixelnours_big, renderer, name.data, name.num,
                    temp_pos.x, temp_pos.y);

    temp_pos.x = POPUP_MAP_COMBAT_PATCH_RED_NAME_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_PATCH_RED_NAME_Y;

    struct Unit *dft_unit = TNECS_GET_COMPONENT(pmc->world, pmc->defendant, Unit);
    name = dft_unit->name;
    PixelFont_Write(pmc->pixelnours_big, renderer, name.data, name.num,
                    temp_pos.x, temp_pos.y);
}

static void _PopUp_Map_Combat_Draw_Stats(struct PopUp_Map_Combat *pmc, SDL_Renderer *renderer) {
    /* --- DMGs --- */
    struct Damage damage_a      = pmc->forecast->stats.agg_damage;
    struct Damage damage_d      = pmc->forecast->stats.dft_damage;
    struct Combat_Rates rates_a = pmc->forecast->stats.agg_rates;
    struct Combat_Rates rates_d = pmc->forecast->stats.dft_rates;
    char numbuff[10];
    struct Point temp_pos;

    PixelFont_Write(pmc->pixelnours_tight, renderer, "DMG", 3,
                    POPUP_MAP_COMBAT_BLUE_DMG_X, POPUP_MAP_COMBAT_BLUE_DMG_Y);
    PixelFont_Write(pmc->pixelnours_tight, renderer, "DMG", 3,
                    POPUP_MAP_COMBAT_RED_DMG_X, POPUP_MAP_COMBAT_RED_DMG_Y);

    int toprint = int_inbounds(damage_a.dmg[DMG_TYPE_TOTAL], 0, 99);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    int width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_BLUE_DMG_STAT_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_BLUE_DMG_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    toprint = int_inbounds(damage_d.dmg[DMG_TYPE_TOTAL], 0, 99);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_RED_DMG_STAT_X - width / 2;
    temp_pos.y = POPUP_MAP_COMBAT_RED_DMG_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    /* --- HITs --- */
    PixelFont_Write(pmc->pixelnours_tight, renderer, "HIT", 3,
                    POPUP_MAP_COMBAT_BLUE_HIT_X, POPUP_MAP_COMBAT_BLUE_HIT_Y);
    PixelFont_Write(pmc->pixelnours_tight, renderer, "HIT", 3,
                    POPUP_MAP_COMBAT_RED_HIT_X, POPUP_MAP_COMBAT_RED_HIT_Y);

    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_BLUE_HIT_STAT_X - width; // right-align
    temp_pos.y = POPUP_MAP_COMBAT_BLUE_HIT_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_RED_HIT_STAT_X - width;  // right-align
    temp_pos.y = POPUP_MAP_COMBAT_RED_HIT_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    /* --- CRITs --- */
    PixelFont_Write(pmc->pixelnours_tight, renderer, "CRIT", 4,
                    POPUP_MAP_COMBAT_RED_CRIT_X, POPUP_MAP_COMBAT_RED_CRIT_Y);
    PixelFont_Write(pmc->pixelnours_tight, renderer, "CRIT", 4,
                    POPUP_MAP_COMBAT_BLUE_CRIT_X, POPUP_MAP_COMBAT_BLUE_CRIT_Y);

    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_BLUE_CRIT_STAT_X - width;  // right-align
    temp_pos.y = POPUP_MAP_COMBAT_BLUE_CRIT_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);

    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", toprint);
    width = PixelFont_Width(pmc->pixelnours_tight, numbuff, strlen(numbuff));
    temp_pos.x = POPUP_MAP_COMBAT_RED_CRIT_STAT_X - width;
    temp_pos.y = POPUP_MAP_COMBAT_RED_CRIT_Y;

    PixelFont_Write(pmc->pixelnours_tight, renderer, numbuff, strlen(numbuff),
                    temp_pos.x, temp_pos.y);
}

static void _PopUp_Map_Combat_Draw_CircleBars(struct PopUp_Map_Combat *pmc,
                                              SDL_Renderer *renderer) {
    /* --- Attack CircleBars --- */
    int phases_agg = 1, phases_dft = 0;

    SDL_assert(pmc->forecast);
    phases_agg = pmc->forecast->flow.aggressor_phases;
    phases_dft = pmc->forecast->flow.defendant_phases;
    SDL_assert(phases_agg <= SOTA_UNIT_MAX_COMBAT_PHASES);
    SDL_assert(phases_dft <= SOTA_UNIT_MAX_COMBAT_PHASES);

    int phases_num = phases_agg + phases_dft;
    int circle_agg_width = phases_agg * CIRCLE_BAR_WIDTH + (phases_agg - 1) *
                           POPUP_MAP_COMBAT_CIRCLE_DIST;
    int circle_dft_width = phases_dft * CIRCLE_BAR_WIDTH + (phases_dft - 1) *
                           POPUP_MAP_COMBAT_CIRCLE_DIST;

    int total_attack = 0;
    SDL_assert(pmc->phases);
    int agg_p = 0, dft_p = 0;
    for (int i = 0; i < phases_num; i++) {
        struct Combat_Phase phase = pmc->phases[i];
        // First circle bars are closer to the middle, fill outwards
        if (phase.attacker) {
            pmc->circlebar_agg.pos.x = POPUP_MAP_COMBAT_BLUE_CIRCLE_BAR_X + circle_agg_width / 2 - agg_p *
                                       CIRCLE_BAR_WIDTH;

            int attack_i = pmc->current_attack > total_attack ?
                           pmc->current_attack - total_attack : 0;
            attack_i = attack_i > phase.attack_num ? phase.attack_num : attack_i;
            CircleBar_Fill(&pmc->circlebar_agg, attack_i, phase.attack_num);
            CircleBar_Draw(&pmc->circlebar_agg, renderer);
            agg_p++;
        } else {
            pmc->circlebar_dft.pos.x = POPUP_MAP_COMBAT_RED_CIRCLE_BAR_X - circle_dft_width / 2 + dft_p *
                                       CIRCLE_BAR_WIDTH;

            int attack_i = pmc->current_attack > total_attack ?
                           pmc->current_attack - total_attack : 0;
            attack_i = attack_i > phase.attack_num ? phase.attack_num : attack_i;
            CircleBar_Fill(&pmc->circlebar_dft, attack_i, phase.attack_num);
            CircleBar_Draw(&pmc->circlebar_dft, renderer);
            dft_p++;
        }
        total_attack += phase.attack_num;
    }
    SDL_assert(phases_agg == agg_p);
    SDL_assert(phases_dft == dft_p);
}

static void _PopUp_Map_Combat_Draw_Headers(struct PopUp_Map_Combat *pmc, SDL_Renderer *renderer) {
    /* --- Menu Headers --- */
    SDL_Rect dstrect;
    dstrect.x = POPUP_MAP_COMBAT_HEADER_RED_X;
    dstrect.y = POPUP_MAP_COMBAT_HEADER_RED_Y;
    dstrect.w = POPUP_MAP_COMBAT_HEADER_W;
    dstrect.h = POPUP_MAP_COMBAT_HEADER_H;
    SDL_RenderCopy(renderer, pmc->texture_header_red, NULL, &dstrect);
    dstrect.x = POPUP_MAP_COMBAT_HEADER_BLUE_X;
    dstrect.y = POPUP_MAP_COMBAT_HEADER_BLUE_Y;
    dstrect.w = POPUP_MAP_COMBAT_HEADER_W;
    dstrect.h = POPUP_MAP_COMBAT_HEADER_H;
    SDL_RenderCopy(renderer, pmc->texture_header_blue, NULL, &dstrect);
}

struct n9Patch PopUp_Map_Combat_Compute_Patch(struct PopUp_Map_Combat *pmc,
                                              struct n9Patch *n9patch) {
    /* -- Blue patches -- */
    struct n9Patch patch   = n9Patch_default;
    patch.size_patches.x   = POPUP_MAP_COMBAT_PATCH_SIZE_X;
    patch.size_patches.y   = POPUP_MAP_COMBAT_PATCH_SIZE_Y;

    patch.patch_pixels.x   = PMC_PATCH_PIXELS;
    patch.patch_pixels.y   = PMC_PATCH_PIXELS;

    patch.size_pixels.x    = patch.patch_pixels.x * patch.size_patches.x;
    patch.size_pixels.y    = patch.patch_pixels.y * patch.size_patches.y;

    patch.pos.y            = POPUP_MAP_COMBAT_HEADER_PLUS_HEIGHT;

    patch.scale.x          = n9patch->scale.x;

    return (patch);
}

void PopUp_Map_Combat_Update(struct PopUp_Map_Combat *pmc, struct n9Patch *n9patch,
                             SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(n9patch                      != NULL);
    SDL_assert(pmc                          != NULL);
    SDL_assert(renderer                     != NULL);
    SDL_assert(pmc->aggressor               >  TNECS_NULL);
    SDL_assert(pmc->defendant               >  TNECS_NULL);
    SDL_assert(pmc->texture_n9patch_red     != NULL);
    SDL_assert(pmc->texture_n9patch_blue    != NULL);
    SDL_assert(pmc->texture_header_red      != NULL);
    SDL_assert(pmc->texture_header_blue     != NULL);
    SDL_assert(pmc->pixelnours_big          != NULL);
    SDL_assert(pmc->pixelnours_tight        != NULL);
    SDL_assert(pmc->forecast                != NULL);

    /* -- Variable declaration/ ants definition -- */
    struct n9Patch red_patch    = PopUp_Map_Combat_Compute_Patch(pmc, n9patch);
    struct n9Patch blue_patch   = PopUp_Map_Combat_Compute_Patch(pmc, n9patch);

    red_patch.texture           = pmc->texture_n9patch_red;
    blue_patch.texture          = pmc->texture_n9patch_blue;

    SDL_assert(red_patch.scale.x        == blue_patch.scale.x);
    SDL_assert(red_patch.scale.y        == blue_patch.scale.y);
    SDL_assert(red_patch.size_pixels.y  == blue_patch.size_pixels.y);
    SDL_assert(red_patch.size_patches.y == blue_patch.size_patches.y);

    n9patch->size_pixels.x      = blue_patch.size_pixels.x  + red_patch.size_pixels.x;
    n9patch->size_pixels.y      = red_patch.size_pixels.y;
    n9patch->size_patches.x     = blue_patch.size_patches.x + red_patch.size_patches.x;
    n9patch->size_patches.y     = blue_patch.size_patches.y;

    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);

    i16 menu_w = (red_patch.size_pixels.x + blue_patch.size_pixels.x);
    i16 menu_h = (red_patch.size_pixels.y + POPUP_MAP_COMBAT_HEADER_PLUS_HEIGHT);
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);

    /* -- Create render target texture -- */
    if (pmc->texture == NULL) {
        pmc->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(pmc->texture != NULL);
        SDL_SetTextureBlendMode(pmc->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_assert(pmc->texture != NULL);
    SDL_SetRenderTarget(renderer, pmc->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);

    /* --- n9Patches --- */
    int scale_x = blue_patch.scale.x;
    int scale_y = blue_patch.scale.y;
    blue_patch.scale.x  = 1;
    blue_patch.scale.y  = 1;
    n9Patch_Draw(&blue_patch, renderer);
    blue_patch.scale.x  = scale_x;
    blue_patch.scale.y  = scale_y;

    red_patch.pos.x     = blue_patch.size_pixels.x;
    red_patch.scale.x   = 1;
    red_patch.scale.y   = 1;
    n9Patch_Draw(&red_patch, renderer);
    red_patch.scale.x   = scale_x;
    red_patch.scale.y   = scale_y;

    _PopUp_Map_Combat_Draw_Headers(      pmc, renderer);
    _PopUp_Map_Combat_Draw_Names(        pmc, renderer);
    _PopUp_Map_Combat_Draw_Stats(        pmc, renderer);
    _PopUp_Map_Combat_Draw_CircleBars(   pmc, renderer);
    _PopUp_Map_Combat_Draw_HP(           pmc, renderer);

    /* -- Postliminaries -- */
    SDL_SetRenderTarget(renderer, render_target);
    Utilities_DrawColor_Reset(renderer);
    // Filesystem_Texture_Dump("PopupMapCombat.png", renderer, pmc->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

}
