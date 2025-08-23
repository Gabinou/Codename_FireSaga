
#include "popup/pre_combat.h"
#include "game/game.h"
#include "popup/popup.h"
#include "position.h"
#include "pixelfonts.h"
#include "platform.h"
#include "palette.h"
#include "weapon.h"
#include "macros.h"
#include "nmath.h"
#include "structs.h"
#include "globals.h"
#include "names.h"
#include "utilities.h"
#include "filesystem.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/stats.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
/* -- Drawing Utilities -- */
void static _PreCombatPopup_Draw_Names(   struct PreCombatPopup *pcp, SDL_Renderer *r);
void static _PreCombatPopup_Draw_Stats(   struct PreCombatPopup *pcp, SDL_Renderer *r);
void static _PreCombatPopup_Draw_Faces(   struct PreCombatPopup *pcp, SDL_Renderer *r);
void static _PreCombatPopup_Draw_WpnIcons(struct PreCombatPopup *pcp, SDL_Renderer *r);
void static _PreCombatPopup_Draw_Doubling(struct PreCombatPopup *pcp, SDL_Renderer *r);

/* -- Drawing Stats mode FSM -- */
static void _PreCombatPopup_Draw_Stats_Math(  struct PreCombatPopup *pcp, SDL_Renderer *r);
static void _PreCombatPopup_Draw_Stats_Total( struct PreCombatPopup *pcp, SDL_Renderer *r);
static void _PreCombatPopup_Draw_Stats_Simple(struct PreCombatPopup *pcp, SDL_Renderer *r);

const struct PreCombatPopup PreCombatPopup_default =  {
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
    .mode                   = PCP_MODE_TOTAL,
    .pixelnours             = NULL,
    .pixelnours_big         = NULL,
};

/* --- STATIC FUNCTIONS --- */
/* --- Loading --- */
static void _PreCombatPopup_Load_Faces(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    SDL_assert(pcp);
    SDL_assert(pcp->agg_unit > TNECS_NULL);
    SDL_assert(pcp->dft_unit > TNECS_NULL);
    // TODO: find face file from unit name
}

static void _PreCombatPopup_Load_Icons(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    SDL_assert(pcp);
    SDL_assert(renderer != NULL);
    char *path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    pcp->texture_weapons = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pcp->texture_weapons);
}

static void _PreCombatPopup_Draw_Names(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    /* - Name - */
    const s8 name = Unit_Name(pcp->agg_unit);
    int x = PCP_SIMPLE_ANAME_X, y = PCP_SIMPLE_ANAME_Y;
    PixelFont_Write_Len(pcp->pixelnours_big, renderer, name.data, x, y);
    const s8 dft_name = Unit_Name(pcp->dft_unit);
    x = PCP_SIMPLE_DNAME_X,     y = PCP_SIMPLE_DNAME_Y;
    PixelFont_Write_Len(pcp->pixelnours_big, renderer, dft_name.data, x, y);
}

static void _PreCombatPopup_Draw_Faces(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    /* - Portrait/Face - */
    SDL_Rect srcrect;
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.x = PCP_SIMPLE_DFACEL_X;
    srcrect.y = PCP_SIMPLE_DFACEL_Y;
    srcrect.w = PCP_SIMPLE_DFACEL_W;
    srcrect.h = PCP_SIMPLE_DFACEL_H;
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = PCP_SIMPLE_AFACEL_X;
    srcrect.y = PCP_SIMPLE_AFACEL_Y;
    srcrect.w = PCP_SIMPLE_AFACEL_W;
    srcrect.h = PCP_SIMPLE_AFACEL_H;
    SDL_RenderFillRect(renderer, &srcrect);
    Utilities_DrawColor_Reset(renderer);
}

static void _PreCombatPopup_Draw_WpnIcons(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    struct Inventory_item *item;
    SDL_Rect dstrect, srcrect;
    /* - Weapon icons - */
    /* Fill Background */
    srcrect.w = PCP_SIMPLE_ICONS_W,  srcrect.h = PCP_SIMPLE_ICONS_H;
    srcrect.x = PCP_SIMPLE_DICONL_X, srcrect.y = PCP_SIMPLE_DICONL_Y;
    SDL_Color color = palette_SOTA->colors[PCP_COLOR_WPN_TYPE];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &srcrect);
    srcrect.x = PCP_SIMPLE_AICONL_X;
    srcrect.y = PCP_SIMPLE_AICONL_Y;
    SDL_RenderFillRect(renderer, &srcrect);

    /* Weapon Types */
    srcrect.w = PCP_SIMPLE_ICON_W, srcrect.h = PCP_SIMPLE_ICON_H;
    dstrect.w = srcrect.w,         dstrect.h = srcrect.h;
    /* Defendant */
    dstrect.y = (PCP_SIMPLE_DICONL_Y + PCP_SIMPLE_ICON_OFFSET_Y);
    if (!Unit_istwoHanding(pcp->dft_unit)) {
        /* Unit is NOT two handing, printing both left and right weapons */
        dstrect.x = (PCP_SIMPLE_DICONL_X + PCP_SIMPLE_ICON_OFFSET_X);
        /* left hand */
        item = Unit_Item_Equipped(pcp->dft_unit, UNIT_HAND_LEFT);
        if (Unit_isEquipped(pcp->dft_unit, UNIT_HAND_LEFT) && (item->id > ITEM_NULL)) {
            const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCP_HYPHEN_OFFSET_X, y = dstrect.y + PCP_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcp->pixelnours, renderer, "-", 1, x, y);
        }

        /* right hand */
        dstrect.x = (PCP_SIMPLE_DICONR_X + PCP_SIMPLE_ICON_OFFSET_X);
        item = Unit_Item_Equipped(pcp->dft_unit, UNIT_HAND_RIGHT);
        if (Unit_isEquipped(pcp->dft_unit, UNIT_HAND_RIGHT) && (item->id > ITEM_NULL)) {
            const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCP_HYPHEN_OFFSET_X, y = dstrect.y + PCP_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcp->pixelnours, renderer, "-", 1, x, y);
        }
    } else {
        /* Unit is two handing, printing ONE icon in the center */
        dstrect.x  = (PCP_SIMPLE_DICONL_X + PCP_SIMPLE_ICON_OFFSET_X) / 2;
        dstrect.x += (PCP_SIMPLE_DICONR_X + PCP_SIMPLE_ICON_OFFSET_X) / 2;
        item = Unit_Item_Equipped(pcp->dft_unit, UNIT_HAND_RIGHT);
        SDL_assert(item->id > ITEM_NULL);
        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
        u16 type = Item_Typecode(&weapon->item);
        // TODO: weapon with multiple types
        SDL_assert(type > 0);
        int type_ind = (int)(log(type) / log(2.0f)) + 1;
        SDL_assert(type_ind > 0);
        srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
        srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
        SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
    }

    /* Aggressor */
    dstrect.y = (PCP_SIMPLE_AICONL_Y + PCP_SIMPLE_ICON_OFFSET_Y);
    if (!Unit_istwoHanding(pcp->agg_unit)) {
        /* Unit is NOT two handing, printing both left and right weapons */
        dstrect.x = (PCP_SIMPLE_AICONL_X + PCP_SIMPLE_ICON_OFFSET_X);
        /* left hand */
        item = Unit_Item_Equipped(pcp->agg_unit, UNIT_HAND_LEFT);
        if (Unit_isEquipped(pcp->agg_unit, UNIT_HAND_LEFT) && (item->id > ITEM_NULL)) {
            const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
            u16 type = Item_Typecode(&weapon->item);
            // TODO: weapon with multiple types
            SDL_assert(type > 0);
            int type_ind = (int)(log(type) / log(2.0f)) + 1;
            SDL_assert(type_ind > 0);
            srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
        } else {
            int x = dstrect.x + PCP_HYPHEN_OFFSET_X, y = dstrect.y + PCP_HYPHEN_OFFSET_Y;
            PixelFont_Write(pcp->pixelnours, renderer, "-", 1, x, y);
        }

        /* right hand */
        dstrect.x = (PCP_SIMPLE_AICONR_X + PCP_SIMPLE_ICON_OFFSET_X);
        dstrect.y = (PCP_SIMPLE_AICONR_Y + PCP_SIMPLE_ICON_OFFSET_Y);
        item = Unit_Item_Equipped(pcp->agg_unit, UNIT_HAND_RIGHT);
        if (Unit_isEquipped(pcp->agg_unit, UNIT_HAND_RIGHT) && (item->id > ITEM_NULL)) {
            const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
            SDL_assert(weapon       != NULL);
            u16 type = Item_Typecode(&weapon->item);
            // TODO: weapon with multiple types
            SDL_assert(type > 0);
            int type_ind = (int)(log(type) / log(2.0f)) + 1;
            SDL_assert(type_ind > 0);
            srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
            srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
            SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
        } else {
            PixelFont_Write(pcp->pixelnours, renderer, "-", 1, dstrect.x + PCP_HYPHEN_OFFSET_X,
                            dstrect.y + PCP_HYPHEN_OFFSET_Y);
        }
    } else {
        /* Unit is two handing, printing ONE icon in the center */
        dstrect.x  = (PCP_SIMPLE_AICONL_X + PCP_SIMPLE_ICON_OFFSET_X) / 2;
        dstrect.x += (PCP_SIMPLE_AICONR_X + PCP_SIMPLE_ICON_OFFSET_X) / 2;
        item = Unit_Item_Equipped(pcp->agg_unit, UNIT_HAND_RIGHT);
        SDL_assert(item->id > ITEM_NULL);

        const Weapon *weapon = DTAB_GET_CONST(gl_weapons_dtab, item->id);
        SDL_assert(weapon       != NULL);
        u16 type = Item_Typecode(&weapon->item);
        // TODO: weapon with multiple types
        SDL_assert(type > 0);
        int type_ind = (int)(log2(type) + 1);
        SDL_assert(type_ind > 0);
        srcrect.x = (type_ind % PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_W;
        srcrect.y = (type_ind / PCP_SIMPLE_ICON_ROWLEN) * PCP_SIMPLE_ICON_H;
        SDL_RenderCopy(renderer, pcp->texture_weapons, &srcrect, &dstrect);
    }
    Utilities_DrawColor_Reset(renderer);
}

static void _PreCombatPopup_Draw_Stats(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    SDL_assert(pcp->mode >= PCP_MODE_NULL && pcp->mode < PCP_MODE_NUM);

    if (pcp_draw_stats[pcp->mode] != NULL)
        pcp_draw_stats[pcp->mode](pcp, renderer);
}

static void _PreCombatPopup_Draw_Stats_Math(  struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    SDL_Rect dstrect;

    char numbuff[10];
    struct Point *agg_pos = &pcp->agg_pos->tilemap_pos;
    struct Point *dft_pos = &pcp->dft_pos->tilemap_pos;

    /* Distance between units */
    u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Unit_stats ES_A = Unit_effectiveStats(pcp->agg_unit);
    struct Unit_stats ES_D = Unit_effectiveStats(pcp->dft_unit);
    struct Computed_Stats CS_A = Unit_computedStats(pcp->agg_unit, distance, ES_A);
    struct Computed_Stats CS_D = Unit_computedStats(pcp->dft_unit, distance, ES_D);
    // struct Combat_Damage damage_a                 = pcp->forecast->stats.agg_damage;
    // struct Combat_Damage damage_d                 = pcp->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcp->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcp->forecast->stats.dft_rates;

    int x = PCP_MATH_HP_X,  y = PCP_MATH_HP_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HP",    2, x, y);
    x = PCP_MATH_ATK_X,     y = PCP_MATH_ATK_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "ATK",   3, x, y);
    x = PCP_MATH_PROT_X,    y = PCP_MATH_PROT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "DEF",   3, x, y);
    x = PCP_MATH_HIT_X,     y = PCP_MATH_HIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HIT",   3, x, y);
    x = PCP_MATH_CRIT_X,    y = PCP_MATH_CRIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "CRIT",  4, x, y);
    x = PCP_MATH_SPEED_X,   y = PCP_MATH_SPEED_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "SPEED", 5, x, y);

    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    i32 current_hp = Unit_Current_HP(pcp->dft_unit);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", current_hp, ES_D.hp);
    x = PCP_MATH_HP_DSTAT_X,     y = PCP_MATH_HP_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* atk */
    int toprint  = int_inbounds(CS_D.attack.physical, 0, 0xFF);
    int toprint2 = int_inbounds(CS_D.attack.magical,  0, 0xFF);
    int toprint3 = int_inbounds(CS_D.attack.True,     0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d\0\0", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d\0\0",    toprint, toprint2);
    int width = PixelFont_Width(pcp->pixelnours_big, numbuff, strlen(numbuff));
    width += (toprint3 > 0) * PCP_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCP_TRUE_DMG_OFFSET_10_X * 2;

    x = PCP_MATH_ATK_DSTAT_X - width / 2,   y = PCP_MATH_ATK_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* prot */
    toprint = int_inbounds(CS_D.protection.physical, 0, 0xFF);
    toprint2 = int_inbounds(CS_D.protection.magical, 0, 0xFF);
    stbsp_sprintf(numbuff, "%d/%d\0\0", toprint, toprint2);
    x = PCP_MATH_PROT_DSTAT_X,   y = PCP_MATH_PROT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_MATH_HIT_DSTAT_X,   y = PCP_MATH_HIT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_MATH_CRIT_DSTAT_X,   y = PCP_MATH_CRIT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);
    /* speed */
    toprint = int_inbounds(CS_D.speed, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%2d\0\0\0", toprint);
    x = PCP_MATH_SPEED_DSTAT_X,   y = PCP_MATH_SPEED_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    current_hp = Unit_Current_HP(pcp->agg_unit);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", current_hp, ES_A.hp);
    x = PCP_MATH_HP_ASTAT_X,   y = PCP_MATH_HP_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* atk */
    toprint  = int_inbounds(CS_A.attack.physical,   0, 0xFF);
    toprint2 = int_inbounds(CS_A.attack.magical,    0, 0xFF);
    toprint3 = int_inbounds(CS_A.attack.True,       0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d\0\0", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d\0\0\0", toprint, toprint2);
    width = PixelFont_Width(pcp->pixelnours_big, numbuff, strlen(numbuff));
    width -= (toprint3 > 0) * PCP_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCP_TRUE_DMG_OFFSET_10_X * 2;
    x = PCP_MATH_ATK_ASTAT_X - width / 2,   y = PCP_MATH_ATK_ASTAT_Y;
    PixelFont_Write_Len(pcp->pixelnours_big, renderer, numbuff, x, y);

    /* prot */
    toprint  = int_inbounds(CS_A.protection.physical, 0, 0xFF);
    toprint2 = int_inbounds(CS_A.protection.magical,  0, 0xFF);
    stbsp_sprintf(numbuff, "%d/%d\0\0\0", toprint, toprint2);
    x = PCP_MATH_PROT_ASTAT_X,   y = PCP_MATH_PROT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_MATH_HIT_ASTAT_X,   y = PCP_MATH_HIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_MATH_CRIT_ASTAT_X,   y = PCP_MATH_CRIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* speed */
    toprint = int_inbounds(CS_A.speed, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%2d\0\0\0", toprint);
    x = PCP_MATH_SPEED_ASTAT_X,   y = PCP_MATH_SPEED_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcp->forecast->flow.aggressor_phases;
    int phases_dft = pcp->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1) {
            dstrect.x = PCP_DOUBLING_MATH_DFT_X;
            dstrect.y = PCP_DOUBLING_MATH_DFT_Y;
        }
        if (phases_agg > 1) {
            dstrect.x = PCP_DOUBLING_MATH_AGG_X;
            dstrect.y = PCP_DOUBLING_MATH_AGG_Y;
        }
        _PreCombatPopup_Draw_Doubling(pcp, renderer);
        dstrect.w = PCP_DOUBLING_MATH_TEXT_W, dstrect.h = PCP_DOUBLING_MATH_TEXT_H;
        SDL_RenderCopy(renderer, pcp->texture_doubling, NULL, &dstrect);
    }
}

static void _PreCombatPopup_Draw_Stats_Total( struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    char numbuff[10];
    /* -- Preliminaries -- */
    SDL_Rect dstrect;


    /* Distance between units */
    // struct Point *agg_pos = &pcp->agg_pos->tilemap_pos;
    // struct Point *dft_pos = &pcp->dft_pos->tilemap_pos;
    // u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Unit_stats ES_A = Unit_effectiveStats(pcp->agg_unit);
    struct Unit_stats ES_D = Unit_effectiveStats(pcp->dft_unit);
    // struct Computed_Stats CS_A = Unit_computedStats(pcp->agg_unit, distance, ES_A);
    // struct Computed_Stats CS_D = Unit_computedStats(pcp->dft_unit, distance, ES_D);
    struct Combat_Damage damage_a                 = pcp->forecast->stats.agg_damage;
    struct Combat_Damage damage_d                 = pcp->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcp->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcp->forecast->stats.dft_rates;

    /* - Names - */
    int x = PCP_SIMPLE_HP_X,   y   = PCP_SIMPLE_HP_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HP",    2, x, y);
    x = PCP_SIMPLE_DMG_X,   y  = PCP_SIMPLE_DMG_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "DMG",   3, x, y);
    x = PCP_SIMPLE_HIT_X,   y  = PCP_SIMPLE_HIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HIT",   3, x, y);
    x = PCP_SIMPLE_CRIT_X,   y = PCP_SIMPLE_CRIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "CRIT",  4, x, y);

    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    i32 current_hp = Unit_Current_HP(pcp->dft_unit);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", current_hp, ES_D.hp);
    x = PCP_SIMPLE_HP_DSTAT_X,   y = PCP_SIMPLE_HP_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    int toprint = int_inbounds(damage_d.dmg.dealt, 0, 0xFF);
    stbsp_sprintf(numbuff, "%2d\0\0\0", toprint);
    x = PCP_SIMPLE_DMG_DSTAT_X,   y = PCP_SIMPLE_DMG_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_HIT_DSTAT_X,   y = PCP_SIMPLE_HIT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_CRIT_DSTAT_X,   y = PCP_SIMPLE_CRIT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    current_hp = Unit_Current_HP(pcp->agg_unit);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", current_hp, ES_A.hp);
    x = PCP_SIMPLE_HP_ASTAT_X,   y = PCP_SIMPLE_HP_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    toprint = int_inbounds(damage_a.dmg.dealt, 0, 0xFF);
    stbsp_sprintf(numbuff, "%2d\0\0\0", toprint);
    x = PCP_SIMPLE_DMG_ASTAT_X,   y = PCP_SIMPLE_DMG_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_HIT_ASTAT_X,   y = PCP_SIMPLE_HIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_CRIT_ASTAT_X,   y = PCP_SIMPLE_CRIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcp->forecast->flow.aggressor_phases;
    int phases_dft = pcp->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1)
            dstrect.x = PCP_DOUBLING_SIMPLE_DFT_X, dstrect.y = PCP_DOUBLING_SIMPLE_DFT_Y;
        _PreCombatPopup_Draw_Doubling(pcp, renderer);
        if (phases_agg > 1)
            dstrect.x = PCP_DOUBLING_SIMPLE_AGG_X, dstrect.y = PCP_DOUBLING_SIMPLE_AGG_Y;
        _PreCombatPopup_Draw_Doubling(pcp, renderer);
        dstrect.w = PCP_DOUBLING_SIMPLE_TEXT_W,    dstrect.h = PCP_DOUBLING_SIMPLE_TEXT_H;
        SDL_RenderCopy(renderer, pcp->texture_doubling, NULL, &dstrect);
    }
}

static void _PreCombatPopup_Draw_Stats_Simple(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    char numbuff[10];
    /* -- Preliminaries -- */
    SDL_Rect dstrect;

    /* Distance between units */
    // struct Point *agg_pos = &pcp->agg_pos->tilemap_pos;
    // struct Point *dft_pos = &pcp->dft_pos->tilemap_pos;
    // u8 distance = abs(dft_pos->x - agg_pos->x) + abs(dft_pos->y - agg_pos->y);

    struct Unit_stats ES_A = Unit_effectiveStats(pcp->agg_unit);
    struct Unit_stats ES_D = Unit_effectiveStats(pcp->dft_unit);
    // struct Computed_Stats CS_A = Unit_computedStats(pcp->agg_unit, distance, ES_A);
    // struct Computed_Stats CS_D = Unit_computedStats(pcp->dft_unit, distance, ES_D);
    struct Combat_Damage damage_a                 = pcp->forecast->stats.agg_damage;
    struct Combat_Damage damage_d                 = pcp->forecast->stats.dft_damage;
    struct Combat_Rates rates_a            = pcp->forecast->stats.agg_rates;
    struct Combat_Rates rates_d            = pcp->forecast->stats.dft_rates;

    /* - Names - */
    int x = PCP_SIMPLE_HP_X,        y = PCP_SIMPLE_HP_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HP",   2, x, y);
    x = PCP_SIMPLE_SPLIT_DMG_X, y = PCP_SIMPLE_SPLIT_DMG_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "DMG",  3, x, y);
    x = PCP_SIMPLE_HIT_X,       y = PCP_SIMPLE_HIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "HIT",  3, x, y);
    x = PCP_SIMPLE_CRIT_X,      y = PCP_SIMPLE_CRIT_Y;
    PixelFont_Write(pcp->pixelnours, renderer, "CRIT", 4, x, y);
    /* - Defendant - */
    // TODO: Text alignment: Centered? -> left align left column, right align right column

    /* HP */
    i32 current_hp = Unit_Current_HP(pcp->dft_unit);
    stbsp_sprintf(numbuff, "%02d/%02d", current_hp, ES_D.hp);
    x = PCP_SIMPLE_HP_DSTAT_X,      y = PCP_SIMPLE_HP_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    int toprint  = int_inbounds(damage_d.dmg.physical,  0, 0xFF);
    int toprint2 = int_inbounds(damage_d.dmg.magical,   0, 0xFF);
    int toprint3 = int_inbounds(damage_d.dmg.True,      0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d\0", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d\0\0\0",    toprint, toprint2);
    int width = PixelFont_Width(pcp->pixelnours_big, numbuff, strlen(numbuff));
    width += (toprint3 > 0) * PCP_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCP_TRUE_DMG_OFFSET_10_X * 2;
    x = PCP_SIMPLE_SPLIT_DMG_DSTAT_X - width / 2,      y = PCP_SIMPLE_SPLIT_DMG_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_d.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_SPLIT_DMG_DSTAT_X,      y = PCP_SIMPLE_SPLIT_DMG_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_d.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_CRIT_DSTAT_X,      y = PCP_SIMPLE_CRIT_DSTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* - Aggressor - */
    /* HP */
    current_hp = Unit_Current_HP(pcp->agg_unit);
    stbsp_sprintf(numbuff, "%02d/%02d", current_hp, ES_A.hp);
    x = PCP_SIMPLE_HP_ASTAT_X,      y = PCP_SIMPLE_HP_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* dmg */
    toprint  = int_inbounds(damage_a.dmg.physical, 0, 0xFF);
    toprint2 = int_inbounds(damage_a.dmg.magical,  0, 0xFF);
    toprint3 = int_inbounds(damage_a.dmg.True,     0, 0xFF);
    if (toprint3 > 0)
        stbsp_sprintf(numbuff, "%d/%d/%d\0", toprint, toprint2, toprint3);
    else
        stbsp_sprintf(numbuff, "%d/%d\0\0\0", toprint, toprint2);
    width = PixelFont_Width(pcp->pixelnours_big, numbuff, strlen(numbuff));
    width -= (toprint3 > 0) * PCP_TRUE_DMG_OFFSET_X    * 2;
    width += (toprint3 > 9) * PCP_TRUE_DMG_OFFSET_10_X * 2;
    x = PCP_SIMPLE_SPLIT_DMG_ASTAT_X - width / 2,      y = PCP_SIMPLE_SPLIT_DMG_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* hit */
    toprint = int_inbounds(rates_a.hit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_HIT_ASTAT_X,      y = PCP_SIMPLE_HIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* crit */
    toprint = int_inbounds(rates_a.crit, 0, SOTA_100PERCENT);
    stbsp_sprintf(numbuff, "%3d\0\0\0", toprint);
    x = PCP_SIMPLE_CRIT_ASTAT_X,      y = PCP_SIMPLE_CRIT_ASTAT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, 3, x, y);

    /* Doubling */
    int phases_agg = pcp->forecast->flow.aggressor_phases;
    int phases_dft = pcp->forecast->flow.defendant_phases;
    if ((phases_agg > 1) || (phases_dft > 1)) {
        if (phases_dft > 1) {
            dstrect.x = PCP_DOUBLING_SPLIT_DFT_X;
            dstrect.y = PCP_DOUBLING_SPLIT_DFT_Y;
        }
        if (phases_agg > 1) {
            dstrect.x = PCP_DOUBLING_SPLIT_AGG_X;
            dstrect.y = PCP_DOUBLING_SPLIT_AGG_Y;
        }
        _PreCombatPopup_Draw_Doubling(pcp, renderer);
        dstrect.w = PCP_DOUBLING_SPLIT_TEXT_W, dstrect.h = PCP_DOUBLING_SPLIT_TEXT_H;
        SDL_RenderCopy(renderer, pcp->texture_doubling, NULL, &dstrect);
    }
}

/* --- Drawing --- */
/* Draw the doubling/tripling/quadrupling symbol */
static void _PreCombatPopup_Draw_Doubling(struct PreCombatPopup *pcp, SDL_Renderer *renderer) {
    /* -- Preliminaries -- */
    /* - Checking number of phases to draw - */
    int phases_agg = pcp->forecast->flow.aggressor_phases;
    int phases_dft = pcp->forecast->flow.defendant_phases;

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
    i16 menu_w = PCP_DOUBLING_SIMPLE_TEXT_W;
    i16 menu_h = PCP_DOUBLING_SIMPLE_TEXT_H;

    /* - create render target texture - */
    if (pcp->texture_doubling == NULL) {
        pcp->texture_doubling = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                                  SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_assert(pcp->texture_doubling != NULL);
        SDL_SetTextureBlendMode(pcp->texture_doubling, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pcp->texture_doubling);

    stbsp_sprintf(numbuff, "%01d\0\0\0\0", max_phase);

    /* -- draw rects -- */
    /* - ligt rect - */
    SDL_Color color = palette_SOTA->colors[PCP_DOUBLING_LIGHT];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    rect.x = 0, rect.y = 1;
    rect.w = menu_w, rect.h = menu_h - 2;
    SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, 1,          0, menu_w - 2,          0);
    SDL_RenderDrawLine(renderer, 1, menu_h - 1, menu_w - 2, menu_h - 1);

    /* - dark rect - */
    color = palette_SOTA->colors[PCP_DOUBLING_DARK];
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    rect.x = 1,          rect.y = 1;
    rect.w = menu_w - 2, rect.h = menu_h - 2;
    SDL_RenderFillRect(renderer, &rect);

    /* - Write text - */
    int x, y;

    x = PCP_DOUBLING_SIMPLE_MULT_X, y = PCP_DOUBLING_SIMPLE_MULT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, "*", strlen(numbuff), x, y);

    x = PCP_DOUBLING_SIMPLE_TEXT_X, y = PCP_DOUBLING_SIMPLE_TEXT_Y;
    PixelFont_Write(pcp->pixelnours_big, renderer, numbuff, strlen(numbuff), x, y);

    /* - Finish - */
    SDL_SetRenderTarget(renderer, pcp->texture);
    Utilities_DrawColor_Reset(renderer);
}

const pcp_draw_stats_t pcp_draw_stats[PCP_MODE_NUM] = {
    /* NULL     */ NULL,
    /* TOTAL    */ _PreCombatPopup_Draw_Stats_Total,
    /* SIMPLE   */ _PreCombatPopup_Draw_Stats_Simple,
    /* MATH     */ _PreCombatPopup_Draw_Stats_Math,
};

/* --- GLOBAL FUNCTIONS --- */
/* --- Setters --- */
void PreCombatPopup_Set(struct PreCombatPopup *pcp, struct Game *sota) {
    SDL_assert(sota != NULL);
    /* -- give aggressor and defendant units to Pre_Combat -- */
    pcp->sota     = sota;
    pcp->update   = true;
    pcp->forecast = &sota->combat.forecast;
}

struct PreCombatPopup *PreCombatPopup_Alloc(void) {
    struct PreCombatPopup *pcp = SDL_malloc(sizeof(struct PreCombatPopup));
    SDL_assert(pcp);
    *pcp = PreCombatPopup_default;
    SDL_assert(pcp->texture == NULL);
    return (pcp);
}

void PreCombatPopup_Free(struct PreCombatPopup *pcp) {
    if (pcp == NULL) {
        return;
    }
    if (pcp->texture != NULL) {
        SDL_DestroyTexture(pcp->texture);
        pcp->texture = NULL;
    }
    if (pcp->texture_doubling != NULL) {
        SDL_DestroyTexture(pcp->texture_doubling);
        pcp->texture_doubling = NULL;
    }
    PreCombatPopup_Free_Faces(pcp);
    PreCombatPopup_Free_Icons(pcp);

    SDL_free(pcp);
}

void PreCombatPopup_Load(struct PreCombatPopup *pcp, tnecs_E aggressor,
                         tnecs_E defendant, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    SDL_assert(pcp       != NULL);
    SDL_assert(pcp->sota != NULL);
    SDL_assert(aggressor > TNECS_NULL);
    SDL_assert(defendant > TNECS_NULL);

    /* - n9patch - */
    *n9patch                = n9Patch_default;
    n9patch->px.x = PCP_PATCH_PIXELS;
    n9patch->px.y = PCP_PATCH_PIXELS;
    n9patch->scale.x        = PCP_N9PATCH_SCALE_X;
    n9patch->scale.y        = PCP_N9PATCH_SCALE_Y;
    n9patch->num.x = PCP_PATCH_X_SIZE;
    n9patch->num.y = PCP_PATCH_Y_SIZE;
    n9patch->pos.x          = 0;
    n9patch->pos.y          = 0;
    SDL_assert(n9patch->px.x > 0);
    SDL_assert(n9patch->px.y > 0);

    Point size = {
        .x  = (PCP_PATCH_PIXELS * PCP_PATCH_X_SIZE),
        .y  = (PCP_PATCH_PIXELS * PCP_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    if (n9patch->texture == NULL) {
        n9patch->texture = Filesystem_Texture_Load(renderer, pcp->sota->menus.filename.data,
                                                   SDL_PIXELFORMAT_INDEX8);
    }

    pcp->defendant = defendant;
    pcp->aggressor = aggressor;
    struct Unit *agg_unit    = IES_GET_C(gl_world, pcp->aggressor, Unit);
    struct Unit *dft_unit    = IES_GET_C(gl_world, pcp->defendant, Unit);
    /* Getting unit positions */
    struct Position *agg_pos = IES_GET_C(gl_world, pcp->aggressor, Position);
    struct Position *dft_pos = IES_GET_C(gl_world, pcp->defendant, Position);
    _PreCombatPopup_Load(pcp, agg_unit, dft_unit, agg_pos, dft_pos, renderer);
}

void _PreCombatPopup_Load(struct PreCombatPopup *pcp, struct Unit *agg_unit, struct Unit *dft_unit,
                          struct Position *agg_pos, struct Position *dft_pos, SDL_Renderer *renderer) {
    pcp->agg_pos  = agg_pos;
    pcp->dft_pos  = dft_pos;
    pcp->agg_unit = agg_unit;
    pcp->dft_unit = dft_unit;

    pcp->update = true;
    _PreCombatPopup_Load_Faces(pcp, renderer);
    _PreCombatPopup_Load_Icons(pcp, renderer);
}
void PreCombatPopup_Free_Faces(struct PreCombatPopup *pcp) {
    if (pcp == NULL) {
        return;
    }
    if (pcp->texture_face_defendant != NULL) {
        SDL_DestroyTexture(pcp->texture_face_defendant);
        pcp->texture_face_defendant = NULL;
    }
    if (pcp->texture_face_aggressor != NULL) {
        SDL_DestroyTexture(pcp->texture_face_aggressor);
        pcp->texture_face_aggressor = NULL;
    }
}

void PreCombatPopup_Free_Icons(struct PreCombatPopup *pcp) {
    if (pcp == NULL) {
        return;
    }
    if (pcp->texture_weapons != NULL) {
        SDL_DestroyTexture(pcp->texture_weapons);
        pcp->texture_weapons = NULL;
    }
}

/* --- Drawing --- */
void PreCombatPopup_Draw(  struct PopUp *popup, struct Point pos,
                           SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PreCombatPopup *pcp = (struct PreCombatPopup *)popup->data;
    struct n9Patch *n9patch   = &popup->n9patch;

    SDL_assert(pcp != NULL);
    if (pcp->update) {
        PreCombatPopup_Update(pcp, n9patch, render_target, renderer);
        pcp->update = false;
    }
    /* - Unit equipement decided by player with LoadoutSelectMenu - */

    /* TODO: set position of statsmenu */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y,
    };
    SDL_assert(pcp->texture != NULL);
    SDL_RenderCopy(renderer, pcp->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void PreCombatPopup_Update(struct PreCombatPopup *pcp, struct n9Patch *n9patch,
                           SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(pcp                  != NULL);
    SDL_assert(renderer             != NULL);
    SDL_assert(pcp->forecast        != NULL);
    SDL_assert(pcp->agg_unit        != NULL);
    SDL_assert(pcp->dft_unit        != NULL);
    SDL_assert(pcp->agg_pos         != NULL);
    SDL_assert(pcp->dft_pos         != NULL);
    SDL_assert(pcp->pixelnours      != NULL);
    SDL_assert(pcp->pixelnours_big  != NULL);
    /* - variable declaration/ants definition - */
    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_assert(size.x   > 0);
    SDL_assert(size.y   > 0);
    SDL_assert(n9patch->scale.x         > 0);
    SDL_assert(n9patch->scale.y         > 0);

    /* - create render target texture - */
    if (pcp->texture == NULL) {
        pcp->texture = SDL_CreateTexture(   renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            size.x, size.y);
        SDL_assert(pcp->texture != NULL);
        SDL_SetTextureBlendMode(pcp->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pcp->texture);
    SDL_assert(pcp->texture != NULL);

    /* --- RENDERING PRE_COMBAT_POPUP --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9patch->scale.x;
    int scale_y = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    _PreCombatPopup_Draw_Names(pcp,    renderer);
    _PreCombatPopup_Draw_Faces(pcp,    renderer);
    _PreCombatPopup_Draw_WpnIcons(pcp, renderer);
    _PreCombatPopup_Draw_Stats(pcp,    renderer);

    /* - Finish - */
    SDL_SetRenderTarget(renderer, render_target);
    Utilities_DrawColor_Reset(renderer);
}
