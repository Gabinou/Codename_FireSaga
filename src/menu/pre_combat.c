#include "menu/pre_combat.h"

struct Point pcm_elem_pos[PRE_COMBAT_MENU_ELEM_NUM] = {
    /* PRE_COMBAT_MENU_ELEM */ {0, 0},
};

struct Point pcm_elem_box[PRE_COMBAT_MENU_ELEM_NUM] = {
    /* PRE_COMBAT_MENU_ELEM */ {1, 1},
};

struct MenuElemDirections pcm_links[PRE_COMBAT_MENU_ELEM_NUM] = {
    /* PRE_COMBAT_MENU_ELEM */ {PRE_COMBAT_MENU_ELEM_NULL, PRE_COMBAT_MENU_ELEM_NULL},
};

struct PreCombatMenu PreCombatMenu_default =  {
    .pos = {0},
    .texture                = NULL,
    .texture_weapons        = NULL,
    .texture_face_aggressor = NULL,
    .texture_face_defendant = NULL,
    .defendant              = TNECS_NULL,
    .aggressor              = TNECS_NULL,
    .dft_unit               = NULL,
    .agg_unit               = NULL,
    .dft_pos                = NULL,
    .agg_pos                = NULL,
    .sota                   = NULL,
    .forecast               = NULL,
    .update                 = false,
    .mode                   = PCM_MODE_TOTAL,
    .pixelnours             = NULL,
    .pixelnours_big         = NULL,
};

struct PreCombatMenu *PreCombatMenu_Alloc() {
    struct PreCombatMenu *pcm = malloc(sizeof(struct PreCombatMenu));
    SDL_assert(pcm);
    *pcm = PreCombatMenu_default;
    SDL_assert(pcm->texture == NULL);
    return (pcm);
}

void PreCombatMenu_Free(struct PreCombatMenu *pcm) {
    SDL_assert(pcm != NULL);
    if (pcm->texture != NULL) {
        SDL_DestroyTexture(pcm->texture);
        pcm->texture = NULL;
    }
    if (pcm->texture_doubling != NULL) {
        SDL_DestroyTexture(pcm->texture_doubling);
        pcm->texture_doubling = NULL;
    }
    PreCombatMenu_Free_Faces(pcm);
    PreCombatMenu_Free_Icons(pcm);
    if (pcm != NULL) {
        free(pcm);
        pcm = NULL;
    }
}

void PreCombatMenu_Load(struct PreCombatMenu *pcm, tnecs_entity_t aggressor,
                        tnecs_entity_t defendant, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(pcm       != NULL);
    SDL_assert(pcm->sota != NULL);
    SDL_assert(aggressor > TNECS_NULL);
    SDL_assert(defendant > TNECS_NULL);

    /* - n9patch - */
    *n9patch                = n9Patch_default;
    n9patch->patch_pixels.x = PCM_PATCH_PIXELS;
    n9patch->patch_pixels.y = PCM_PATCH_PIXELS;
    n9patch->scale.x        = PCM_N9PATCH_SCALE_X;
    n9patch->scale.y        = PCM_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x  = (PCM_PATCH_PIXELS * PCM_PATCH_X_SIZE);
    n9patch->size_pixels.y  = (PCM_PATCH_PIXELS * PCM_PATCH_Y_SIZE);
    n9patch->size_patches.x = PCM_PATCH_X_SIZE;
    n9patch->size_patches.y = PCM_PATCH_Y_SIZE;
    n9patch->pos.x          = 0;
    n9patch->pos.y          = 0;
    SDL_assert(n9patch->patch_pixels.x > 0);
    SDL_assert(n9patch->patch_pixels.y > 0);
    if (n9patch->texture == NULL) {
        n9patch->texture = Filesystem_Texture_Load(renderer, pcm->sota->filename_menu.data,
                                                   SDL_PIXELFORMAT_INDEX8);
    }

    pcm->defendant = defendant;
    pcm->aggressor = aggressor;
    struct Unit *agg_unit    = TNECS_GET_COMPONENT(pcm->sota->world, pcm->aggressor, Unit);
    struct Unit *dft_unit    = TNECS_GET_COMPONENT(pcm->sota->world, pcm->defendant, Unit);
    /* Getting unit positions */
    struct Position *agg_pos = TNECS_GET_COMPONENT(pcm->sota->world, pcm->aggressor, Position);
    struct Position *dft_pos = TNECS_GET_COMPONENT(pcm->sota->world, pcm->defendant, Position);
    _PreCombatMenu_Load(pcm, agg_unit, dft_unit, agg_pos, dft_pos, renderer);
}

void _PreCombatMenu_Load(struct PreCombatMenu *pcm, struct Unit *agg_unit, struct Unit *dft_unit,
                         struct Position *agg_pos, struct Position *dft_pos, SDL_Renderer *renderer) {
    pcm->agg_pos  = agg_pos;
    pcm->dft_pos  = dft_pos;
    pcm->agg_unit = agg_unit;
    pcm->dft_unit = dft_unit;

    pcm->update = true;
    _PreCombatMenu_Load_Faces(pcm, renderer);
    _PreCombatMenu_Load_Icons(pcm, renderer);
}
void PreCombatMenu_Free_Faces(struct PreCombatMenu *pcm) {
    if (pcm->texture_face_defendant != NULL) {
        SDL_DestroyTexture(pcm->texture_face_defendant);
        pcm->texture_face_defendant = NULL;
    }
    if (pcm->texture_face_aggressor != NULL) {
        SDL_DestroyTexture(pcm->texture_face_aggressor);
        pcm->texture_face_aggressor = NULL;
    }
}

void _PreCombatMenu_Load_Faces(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    SDL_assert(pcm);
    SDL_assert(pcm->agg_unit > TNECS_NULL);
    SDL_assert(pcm->dft_unit > TNECS_NULL);
    // TODO: find face file from unit name
}

void PreCombatMenu_Free_Icons(struct PreCombatMenu *pcm) {
    if (pcm->texture_weapons != NULL) {
        SDL_DestroyTexture(pcm->texture_weapons);
        pcm->texture_weapons = NULL;
    }
}

void _PreCombatMenu_Load_Icons(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    SDL_assert(pcm);
    SDL_assert(renderer != NULL);
    char *path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Weapons.png");
    pcm->texture_weapons = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pcm->texture_weapons);
}

/* --- Setters --- */
void PreCombatMenu_Set(struct PreCombatMenu *pcm, struct Game *sota) {
    SDL_assert(sota != NULL);
    /* -- give aggressor and defendant units to Pre_Combat -- */
    pcm->sota     = sota;
    pcm->update   = true;
    pcm->forecast = &sota->combat_forecast;

}

/* --- Drawing --- */
/* Draw the doubling/tripling/quadrupling symbol */
void _PreCombatMenu_Draw_Doubling(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    /* - Checking number of phases to draw - */
    int phases_agg = pcm->forecast->flow.aggressor_phases;
    int phases_dft = pcm->forecast->flow.defendant_phases;

    SDL_assert(((phases_agg >= 0) && (phases_agg <= 1)) || ((phases_dft >= 0) && (phases_dft <= 1)));
    int max_phase = phases_agg > phases_dft ? phases_agg : phases_dft;
    SDL_assert(max_phase <= SOTA_UNIT_MAX_COMBAT_PHASES);

    if (max_phase <= 1) {
        SDL_Log("Number of phases too low. Exiting.");
        return;
    }

    /* - Misc. variables - */
    SDL_Rect rect;
    char numbuff[2];
    i16 menu_w = PCM_DOUBLING_SIMPLE_TEXT_W;
    i16 menu_h = PCM_DOUBLING_SIMPLE_TEXT_H;

    /* - create render target texture - */
    if (pcm->texture_doubling == NULL) {
        pcm->texture_doubling = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                                  SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(pcm->texture_doubling != NULL);
        SDL_SetTextureBlendMode(pcm->texture_doubling, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pcm->texture_doubling);

    stbsp_sprintf(numbuff, "%01d", max_phase);

    /* -- draw rects -- */
    /* - ligt rect - */
    SDL_Color color = palette_NES->colors[PCM_DOUBLING_LIGHT];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    rect.x = 0, rect.y = 1;
    rect.w = menu_w, rect.h = menu_h - 2;
    SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, 1,          0, menu_w - 2,          0);
    SDL_RenderDrawLine(renderer, 1, menu_h - 1, menu_w - 2, menu_h - 1);

    /* - dark rect - */
    color = palette_NES->colors[PCM_DOUBLING_DARK];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    rect.x = 1,          rect.y = 1;
    rect.w = menu_w - 2, rect.h = menu_h - 2;
    SDL_RenderFillRect(renderer, &rect);

    /* - Write text - */
    int x, y;

    x = PCM_DOUBLING_SIMPLE_MULT_X, y = PCM_DOUBLING_SIMPLE_MULT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, "*", strlen(numbuff), x, y);

    x = PCM_DOUBLING_SIMPLE_TEXT_X, y = PCM_DOUBLING_SIMPLE_TEXT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* - Finish - */
    SDL_SetRenderTarget(renderer, pcm->texture);
    Utilities_DrawColor_Reset(renderer);
}

void PreCombatMenu_Draw(struct Menu *mc, SDL_Texture *render_target,
                        SDL_Renderer *renderer) {
    struct PreCombatMenu *pcm = (struct PreCombatMenu *)mc->data;
    struct n9Patch *n9patch   = &mc->n9patch;

    SDL_assert(pcm != NULL);
    if (pcm->update) {
        PreCombatMenu_Update(pcm, n9patch, render_target, renderer);
        pcm->update = false;
    }
    /* - Unit equipement decided by player with LoadoutSelectMenu - */

    /* TODO: set position of statsmenu */
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pcm->pos.x,
        .y = pcm->pos.y,
    };
    SDL_assert(pcm->texture != NULL);
    SDL_RenderCopy(renderer, pcm->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void _PreCombatMenu_Draw_Names(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    /* - Name - */
    char *name = pcm->agg_unit->name;
    int x = PCM_SIMPLE_ANAME_X, y = PCM_SIMPLE_ANAME_Y;
    PixelFont_Write_Len(pcm->pixelnours_big, renderer, name, x, y);
    name = pcm->dft_unit->name;
    x = PCM_SIMPLE_DNAME_X,     y = PCM_SIMPLE_DNAME_Y;
    PixelFont_Write_Len(pcm->pixelnours_big, renderer, name, x, y);
}

void _PreCombatMenu_Draw_Faces(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    /* - Portrait/Face - */
    SDL_Rect srcrect;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.x = PCM_SIMPLE_DFACEL_X;
    srcrect.y = PCM_SIMPLE_DFACEL_Y;
    srcrect.w = PCM_SIMPLE_DFACEL_W;
    srcrect.h = PCM_SIMPLE_DFACEL_H;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = PCM_SIMPLE_AFACEL_X;
    srcrect.y = PCM_SIMPLE_AFACEL_Y;
    srcrect.w = PCM_SIMPLE_AFACEL_W;
    srcrect.h = PCM_SIMPLE_AFACEL_H;
    SDL_RenderFillRect(renderer, &srcrect);
}

void _PreCombatMenu_Draw_WpnIcons(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    struct Inventory_item *item;
    SDL_Rect dstrect, srcrect;
    /* - Weapon icons - */
    /* Fill Background */
    srcrect.w = PCM_SIMPLE_ICONS_W,  srcrect.h = PCM_SIMPLE_ICONS_H;
    srcrect.x = PCM_SIMPLE_DICONL_X, srcrect.y = PCM_SIMPLE_DICONL_Y;
    SDL_Color color = palette_NES->colors[PCM_COLOR_WPN_TYPE];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = PCM_SIMPLE_AICONL_X;
    srcrect.y = PCM_SIMPLE_AICONL_Y;
    SDL_RenderFillRect(renderer, &srcrect);

    /* Weapon Types */
    srcrect.w = PCM_SIMPLE_ICON_W, srcrect.h = PCM_SIMPLE_ICON_H;
    dstrect.w = srcrect.w,         dstrect.h = srcrect.h;
    /* Defendant */
    dstrect.y = (PCM_SIMPLE_DICONL_Y + PCM_SIMPLE_ICON_OFFSET_Y);
    if (!pcm->dft_unit->isTwoHanding) {
        /* Unit is NOT two handing, printing both left and right weapons */
        dstrect.x = (PCM_SIMPLE_DICONL_X + PCM_SIMPLE_ICON_OFFSET_X);
        /* left hand */
        item = Unit_Item_Side(pcm->dft_unit, UNIT_HAND_LEFT);
        if ((pcm->dft_unit->equipped[UNIT_HAND_LEFT]) && (item->id > ITEM_NULL)) {
            struct Weapon *weapon = DTAB_GET(pcm->dft_unit->weapons_dtab, item->id);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCM_HYPHEN_OFFSET_X, y = dstrect.y + PCM_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcm->pixelnours, renderer, "-", 1, x, y);
        }

        /* right hand */
        dstrect.x = (PCM_SIMPLE_DICONR_X + PCM_SIMPLE_ICON_OFFSET_X);
        item = Unit_Item_Side(pcm->dft_unit, UNIT_HAND_RIGHT);
        if ((pcm->dft_unit->equipped[UNIT_HAND_RIGHT]) && (item->id > ITEM_NULL)) {
            struct Weapon *weapon = DTAB_GET(pcm->dft_unit->weapons_dtab, item->id);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCM_HYPHEN_OFFSET_X, y = dstrect.y + PCM_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcm->pixelnours, renderer, "-", 1, x, y);
        }
    } else {
        /* Unit is two handing, printing ONE icon in the center */
        dstrect.x  = (PCM_SIMPLE_DICONL_X + PCM_SIMPLE_ICON_OFFSET_X) / 2;
        dstrect.x += (PCM_SIMPLE_DICONR_X + PCM_SIMPLE_ICON_OFFSET_X) / 2;
        item = Unit_Item_Side(pcm->dft_unit, UNIT_HAND_RIGHT);
        SDL_assert(item->id > ITEM_NULL);
        struct Weapon *weapon = DTAB_GET(pcm->dft_unit->weapons_dtab, item->id);
        u16 type = weapon->item->type;
        // TODO: weapon with multiple types
        SDL_assert(weapon->item->type > 0);
        int type_ind = (int)(log(type) / log(2.0f)) + 1;
        SDL_assert(type_ind > 0);
        srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
        srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
        SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
    }

    /* Aggressor */
    dstrect.y = (PCM_SIMPLE_AICONL_Y + PCM_SIMPLE_ICON_OFFSET_Y);
    if (!pcm->agg_unit->isTwoHanding) {
        /* Unit is NOT two handing, printing both left and right weapons */
        dstrect.x = (PCM_SIMPLE_AICONL_X + PCM_SIMPLE_ICON_OFFSET_X);
        /* left hand */
        item = Unit_Item_Side(pcm->agg_unit, UNIT_HAND_LEFT);
        if ((pcm->agg_unit->equipped[UNIT_HAND_LEFT]) && (item->id > ITEM_NULL)) {
            struct Weapon *weapon = DTAB_GET(pcm->agg_unit->weapons_dtab, item->id);
            u16 type = weapon->item->type;
            // TODO: weapon with multiple types
            SDL_assert(weapon->item->type > 0);
            int type_ind = (int)(log(type) / log(2.0f)) + 1;
            SDL_assert(type_ind > 0);
            srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCM_HYPHEN_OFFSET_X, y = dstrect.y + PCM_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcm->pixelnours, renderer, "-", 1, x, y);
        }

        /* right hand */
        dstrect.x = (PCM_SIMPLE_AICONR_X + PCM_SIMPLE_ICON_OFFSET_X);
        dstrect.y = (PCM_SIMPLE_AICONR_Y + PCM_SIMPLE_ICON_OFFSET_Y);
        item = Unit_Item_Side(pcm->agg_unit, UNIT_HAND_RIGHT);
        if ((pcm->dft_unit->equipped[UNIT_HAND_RIGHT]) && (item->id > ITEM_NULL)) {
            struct Weapon *weapon = DTAB_GET(pcm->dft_unit->weapons_dtab, item->id);
            u16 type = weapon->item->type;
            // TODO: weapon with multiple types
            SDL_assert(weapon->item->type > 1);
            int type_ind = (int)(log(type) / log(2.0f)) + 1;
            SDL_assert(type_ind > 0);
            srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
        } else {
            PixelFont_Write(pcm->pixelnours, renderer, "-", 1, dstrect.x + PCM_HYPHEN_OFFSET_X,
                            dstrect.y + PCM_HYPHEN_OFFSET_Y);
        }
    } else {
        /* Unit is two handing, printing ONE icon in the center */
        dstrect.x  = (PCM_SIMPLE_AICONL_X + PCM_SIMPLE_ICON_OFFSET_X) / 2;
        dstrect.x += (PCM_SIMPLE_AICONR_X + PCM_SIMPLE_ICON_OFFSET_X) / 2;
        item = Unit_Item_Side(pcm->agg_unit, UNIT_HAND_RIGHT);
        SDL_assert(item->id > ITEM_NULL);
        struct Weapon *weapon = DTAB_GET(pcm->agg_unit->weapons_dtab, item->id);
        u16 type = weapon->item->type;
        // TODO: weapon with multiple types
        SDL_assert(weapon->item->type > 0);
        int type_ind = (int)(log(type) / log(2.0f)) + 1;
        SDL_assert(type_ind > 0);
        srcrect.x = (type_ind % PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_W;
        srcrect.y = (type_ind / PCM_SIMPLE_ICON_ROWLEN) * PCM_SIMPLE_ICON_H;
        SDL_RenderCopy(renderer, pcm->texture_weapons, &srcrect, &dstrect);
    }
}

pcm_draw_stats_t pcm_draw_stats[PCM_MODE_NUM] = {
    /* NULL     */ NULL,
    /* TOTAL    */ _PreCombatMenu_Draw_Stats_Total,
    /* SIMPLE   */ _PreCombatMenu_Draw_Stats_Simple,
    /* MATH     */ _PreCombatMenu_Draw_Stats_Math,
};

void _PreCombatMenu_Draw_Stats_Math(  struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    SDL_Rect dstrect, srcrect;

    char numbuff[10];
    const struct Point *agg_pos = &pcm->agg_pos->tilemap_pos;
    const struct Point *dft_pos = &pcm->dft_pos->tilemap_pos;

    /* Distance between units */
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Computed_Stats computed_stats_a = Unit_computedStats(pcm->agg_unit, distance);
    struct Computed_Stats computed_stats_d = Unit_computedStats(pcm->dft_unit, distance);
    struct Damage damage_a                 = pcm->forecast->stats.agg_damage;
    struct Damage damage_d                 = pcm->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcm->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcm->forecast->stats.dft_rates;
    struct Unit_stats effective_stats_a    = pcm->agg_unit->effective_stats;
    struct Unit_stats effective_stats_d    = pcm->dft_unit->effective_stats;

    int x = PCM_MATH_HP_X,  y = PCM_MATH_HP_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HP",    2, x, y);
    x = PCM_MATH_ATK_X,     y = PCM_MATH_ATK_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "ATK",   3, x, y);
    x = PCM_MATH_PROT_X,    y = PCM_MATH_PROT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "DEF",   3, x, y);
    x = PCM_MATH_HIT_X,     y = PCM_MATH_HIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HIT",   3, x, y);
    x = PCM_MATH_CRIT_X,    y = PCM_MATH_CRIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "CRIT",  4, x, y);
    x = PCM_MATH_SPEED_X,   y = PCM_MATH_SPEED_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "SPEED", 5, x, y);

    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->dft_unit->current_hp, effective_stats_d.hp);
    x = PCM_MATH_HP_DSTAT_X,     y = PCM_MATH_HP_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* atk */
    int toprint  = int_inbounds(computed_stats_d.attack[DMG_TYPE_PHYSICAL], 0, 0xFF);
    int toprint2 = int_inbounds(computed_stats_d.attack[DMG_TYPE_MAGICAL],  0, 0xFF);
    int toprint3 = int_inbounds(computed_stats_d.attack[DMG_TYPE_TRUE],     0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d",    toprint, toprint2);
    int width = PixelFont_Width(pcm->pixelnours_big, numbuff, strlen(numbuff));
    width += (toprint3 > 0) * PCM_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCM_TRUE_DMG_OFFSET_10_X * 2;

    x = PCM_MATH_ATK_DSTAT_X - width / 2,   y = PCM_MATH_ATK_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* prot */
    toprint = int_inbounds(computed_stats_d.protection[DMG_TYPE_PHYSICAL], 0, 0xFF);
    toprint2 = int_inbounds(computed_stats_d.protection[DMG_TYPE_MAGICAL], 0, 0xFF);
    stbsp_sprintf(numbuff, "%d/%d", toprint, toprint2);
    x = PCM_MATH_PROT_DSTAT_X,   y = PCM_MATH_PROT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_MATH_HIT_DSTAT_X,   y = PCM_MATH_HIT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_MATH_CRIT_DSTAT_X,   y = PCM_MATH_CRIT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);
    /* speed */
    toprint = int_inbounds(computed_stats_d.speed, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%2d", toprint);
    x = PCM_MATH_SPEED_DSTAT_X,   y = PCM_MATH_SPEED_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->agg_unit->current_hp, effective_stats_a.hp);
    x = PCM_MATH_HP_ASTAT_X,   y = PCM_MATH_HP_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* atk */
    toprint  = int_inbounds(computed_stats_a.attack[DMG_TYPE_PHYSICAL],   0, 0xFF);
    toprint2 = int_inbounds(computed_stats_a.attack[DMG_TYPE_MAGICAL],    0, 0xFF);
    toprint3 = int_inbounds(computed_stats_a.attack[DMG_TYPE_TRUE],       0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d", toprint, toprint2);
    width = PixelFont_Width(pcm->pixelnours_big, numbuff, strlen(numbuff));
    width -= (toprint3 > 0) * PCM_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCM_TRUE_DMG_OFFSET_10_X * 2;
    x = PCM_MATH_ATK_ASTAT_X - width / 2,   y = PCM_MATH_ATK_ASTAT_Y;
    PixelFont_Write_Len(pcm->pixelnours_big, renderer, numbuff, x, y);

    /* prot */
    toprint  = int_inbounds(computed_stats_a.protection[DMG_TYPE_PHYSICAL], 0, 0xFF);
    toprint2 = int_inbounds(computed_stats_a.protection[DMG_TYPE_MAGICAL],  0, 0xFF);
    stbsp_sprintf(numbuff, "%d/%d", toprint, toprint2);
    x = PCM_MATH_PROT_ASTAT_X,   y = PCM_MATH_PROT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_MATH_HIT_ASTAT_X,   y = PCM_MATH_HIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_MATH_CRIT_ASTAT_X,   y = PCM_MATH_CRIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* speed */
    toprint = int_inbounds(computed_stats_a.speed, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%2d", toprint);
    x = PCM_MATH_SPEED_ASTAT_X,   y = PCM_MATH_SPEED_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcm->forecast->flow.aggressor_phases;
    int phases_dft = pcm->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1) {
            dstrect.x = PCM_DOUBLING_MATH_DFT_X;
            dstrect.y = PCM_DOUBLING_MATH_DFT_Y;
        }
        if (phases_agg > 1) {
            dstrect.x = PCM_DOUBLING_MATH_AGG_X;
            dstrect.y = PCM_DOUBLING_MATH_AGG_Y;
        }
        _PreCombatMenu_Draw_Doubling(pcm, renderer);
        dstrect.w = PCM_DOUBLING_MATH_TEXT_W, dstrect.h = PCM_DOUBLING_MATH_TEXT_H;
        SDL_RenderCopy(renderer, pcm->texture_doubling, NULL, &dstrect);
    }
}

void _PreCombatMenu_Draw_Stats_Total( struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    char numbuff[10];
    /* -- Preliminaries -- */
    SDL_Rect dstrect, srcrect;

    const struct Point *agg_pos = &pcm->agg_pos->tilemap_pos;
    const struct Point *dft_pos = &pcm->dft_pos->tilemap_pos;

    /* Distance between units */
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Computed_Stats computed_stats_a = Unit_computedStats(pcm->agg_unit, distance);
    struct Computed_Stats computed_stats_d = Unit_computedStats(pcm->dft_unit, distance);
    struct Damage damage_a                 = pcm->forecast->stats.agg_damage;
    struct Damage damage_d                 = pcm->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcm->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcm->forecast->stats.dft_rates;
    struct Unit_stats effective_stats_a    = pcm->agg_unit->effective_stats;
    struct Unit_stats effective_stats_d    = pcm->dft_unit->effective_stats;


    /* - Names - */
    int x = PCM_SIMPLE_HP_X,   y   = PCM_SIMPLE_HP_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HP",    2, x, y);
    x = PCM_SIMPLE_DMG_X,   y  = PCM_SIMPLE_DMG_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "DMG",   3, x, y);
    x = PCM_SIMPLE_HIT_X,   y  = PCM_SIMPLE_HIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HIT",   3, x, y);
    x = PCM_SIMPLE_CRIT_X,   y = PCM_SIMPLE_CRIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "CRIT",  4, x, y);

    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->dft_unit->current_hp, effective_stats_d.hp);
    x = PCM_SIMPLE_HP_DSTAT_X,   y = PCM_SIMPLE_HP_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    int toprint = int_inbounds(damage_d.dmg[DMG_TYPE_TOTAL], 0, 0xFF);
    stbsp_sprintf(numbuff, "%2d", toprint);
    x = PCM_SIMPLE_DMG_DSTAT_X,   y = PCM_SIMPLE_DMG_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_HIT_DSTAT_X,   y = PCM_SIMPLE_HIT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_CRIT_DSTAT_X,   y = PCM_SIMPLE_CRIT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->agg_unit->current_hp, effective_stats_a.hp);
    x = PCM_SIMPLE_HP_ASTAT_X,   y = PCM_SIMPLE_HP_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    toprint = int_inbounds(damage_a.dmg[DMG_TYPE_TOTAL], 0, 0xFF);
    stbsp_sprintf(numbuff, "%2d", toprint);
    x = PCM_SIMPLE_DMG_ASTAT_X,   y = PCM_SIMPLE_DMG_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_HIT_ASTAT_X,   y = PCM_SIMPLE_HIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_CRIT_ASTAT_X,   y = PCM_SIMPLE_CRIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcm->forecast->flow.aggressor_phases;
    int phases_dft = pcm->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1)
            dstrect.x = PCM_DOUBLING_SIMPLE_DFT_X, dstrect.y = PCM_DOUBLING_SIMPLE_DFT_Y;
        _PreCombatMenu_Draw_Doubling(pcm, renderer);
        if (phases_agg > 1)
            dstrect.x = PCM_DOUBLING_SIMPLE_AGG_X, dstrect.y = PCM_DOUBLING_SIMPLE_AGG_Y;
        _PreCombatMenu_Draw_Doubling(pcm, renderer);
        dstrect.w = PCM_DOUBLING_SIMPLE_TEXT_W,    dstrect.h = PCM_DOUBLING_SIMPLE_TEXT_H;
        SDL_RenderCopy(renderer, pcm->texture_doubling, NULL, &dstrect);
    }
}

void _PreCombatMenu_Draw_Stats_Simple(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    char numbuff[10];
    /* -- Preliminaries -- */
    SDL_Rect dstrect, srcrect;
    const struct Point *agg_pos = &pcm->agg_pos->tilemap_pos;
    const struct Point *dft_pos = &pcm->dft_pos->tilemap_pos;

    /* Distance between units */
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Computed_Stats computed_stats_a = Unit_computedStats(pcm->agg_unit, distance);
    struct Computed_Stats computed_stats_d = Unit_computedStats(pcm->dft_unit, distance);
    struct Damage damage_a                 = pcm->forecast->stats.agg_damage;
    struct Damage damage_d                 = pcm->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcm->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcm->forecast->stats.dft_rates;
    struct Unit_stats effective_stats_a    = pcm->agg_unit->effective_stats;
    struct Unit_stats effective_stats_d    = pcm->dft_unit->effective_stats;

    /* - Names - */
    int x = PCM_SIMPLE_HP_X,        y = PCM_SIMPLE_HP_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HP",   2, x, y);
    x = PCM_SIMPLE_SPLIT_DMG_X, y = PCM_SIMPLE_SPLIT_DMG_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "DMG",  3, x, y);
    x = PCM_SIMPLE_HIT_X,       y = PCM_SIMPLE_HIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "HIT",  3, x, y);
    x = PCM_SIMPLE_CRIT_X,      y = PCM_SIMPLE_CRIT_Y;
    PixelFont_Write(pcm->pixelnours, renderer, "CRIT", 4, x, y);
    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->dft_unit->current_hp, effective_stats_d.hp);
    x = PCM_SIMPLE_HP_DSTAT_X,      y = PCM_SIMPLE_HP_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    int toprint  = int_inbounds(damage_d.dmg[DMG_TYPE_PHYSICAL],  0, 0xFF);
    int toprint2 = int_inbounds(damage_d.dmg[DMG_TYPE_MAGICAL],   0, 0xFF);
    int toprint3 = int_inbounds(damage_d.dmg[DMG_TYPE_TRUE],      0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d",    toprint, toprint2);
    int width = PixelFont_Width(pcm->pixelnours_big, numbuff, strlen(numbuff));
    width += (toprint3 > 0) * PCM_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCM_TRUE_DMG_OFFSET_10_X * 2;
    x = PCM_SIMPLE_SPLIT_DMG_DSTAT_X - width / 2,      y = PCM_SIMPLE_SPLIT_DMG_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_SPLIT_DMG_DSTAT_X,      y = PCM_SIMPLE_SPLIT_DMG_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_CRIT_DSTAT_X,      y = PCM_SIMPLE_CRIT_DSTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    stbsp_sprintf(numbuff, "%02d/%02d", pcm->agg_unit->current_hp, effective_stats_a.hp);
    x = PCM_SIMPLE_HP_ASTAT_X,      y = PCM_SIMPLE_HP_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    toprint  = int_inbounds(damage_a.dmg[DMG_TYPE_PHYSICAL], 0, 0xFF);
    toprint2 = int_inbounds(damage_a.dmg[DMG_TYPE_MAGICAL],  0, 0xFF);
    toprint3 = int_inbounds(damage_a.dmg[DMG_TYPE_TRUE],     0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d", toprint, toprint2);
    width = PixelFont_Width(pcm->pixelnours_big, numbuff, strlen(numbuff));
    width -= (toprint3 > 0) * PCM_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCM_TRUE_DMG_OFFSET_10_X * 2;
    x = PCM_SIMPLE_SPLIT_DMG_ASTAT_X - width / 2,      y = PCM_SIMPLE_SPLIT_DMG_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_HIT_ASTAT_X,      y = PCM_SIMPLE_HIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d", toprint);
    x = PCM_SIMPLE_CRIT_ASTAT_X,      y = PCM_SIMPLE_CRIT_ASTAT_Y;
    PixelFont_Write(pcm->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcm->forecast->flow.aggressor_phases;
    int phases_dft = pcm->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1) {
            dstrect.x = PCM_DOUBLING_SPLIT_DFT_X;
            dstrect.y = PCM_DOUBLING_SPLIT_DFT_Y;
        }
        if (phases_agg > 1) {
            dstrect.x = PCM_DOUBLING_SPLIT_AGG_X;
            dstrect.y = PCM_DOUBLING_SPLIT_AGG_Y;
        }
        _PreCombatMenu_Draw_Doubling(pcm, renderer);
        dstrect.w = PCM_DOUBLING_SPLIT_TEXT_W, dstrect.h = PCM_DOUBLING_SPLIT_TEXT_H;
        SDL_RenderCopy(renderer, pcm->texture_doubling, NULL, &dstrect);
    }
}

void _PreCombatMenu_Draw_Stats(struct PreCombatMenu *pcm, SDL_Renderer *renderer) {
    SDL_assert(pcm->mode >= PCM_MODE_NULL && pcm->mode < PCM_MODE_NUM);

    if (pcm_draw_stats[pcm->mode] != NULL)
        pcm_draw_stats[pcm->mode](pcm, renderer);
}

void PreCombatMenu_Update(struct PreCombatMenu *pcm, struct n9Patch *n9patch,
                          SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(pcm                  != NULL);
    SDL_assert(renderer             != NULL);
    SDL_assert(pcm->forecast        != NULL);
    SDL_assert(pcm->agg_unit        != NULL);
    SDL_assert(pcm->dft_unit        != NULL);
    SDL_assert(pcm->agg_pos         != NULL);
    SDL_assert(pcm->dft_pos         != NULL);
    SDL_assert(pcm->pixelnours      != NULL);
    SDL_assert(pcm->pixelnours_big  != NULL);
    /* - variable declaration/constants definition - */
    SDL_assert(n9patch->size_pixels.x   > 0);
    SDL_assert(n9patch->size_pixels.y   > 0);
    SDL_assert(n9patch->scale.x         > 0);
    SDL_assert(n9patch->scale.y         > 0);
    i16 menu_w = n9patch->size_pixels.x;
    i16 menu_h = n9patch->size_pixels.y;
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);

    /* - create render target texture - */
    if (pcm->texture == NULL) {
        pcm->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(pcm->texture != NULL);
        SDL_SetTextureBlendMode(pcm->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pcm->texture);
    SDL_assert(pcm->texture != NULL);

    /* --- RENDERING PRE_COMBAT_MENU --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9patch->scale.x;
    int scale_y = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    _PreCombatMenu_Draw_Names(pcm,    renderer);
    _PreCombatMenu_Draw_Faces(pcm,    renderer);
    _PreCombatMenu_Draw_WpnIcons(pcm, renderer);
    _PreCombatMenu_Draw_Stats(pcm,    renderer);

    /* - Finish - */
    SDL_SetRenderTarget(renderer, render_target);
    Utilities_DrawColor_Reset(renderer);
}
