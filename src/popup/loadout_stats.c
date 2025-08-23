
#include "popup/loadout_stats.h"
#include "names.h"
#include "platform.h"
#include "weapon.h"
#include "utilities.h"
#include "filesystem.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/loadout.h"
#include "unit/equipment.h"
#include "macros.h"
#include "globals.h"
#include "nmath.h"
#include "popup/popup.h"
#include "pixelfonts.h"
#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static void _PopUp_Loadout_Stats_Draw_Arrows(struct PopUp_Loadout_Stats *pls,
                                             SDL_Renderer *renderer);
static void _PopUp_Loadout_Stats_Draw_Stats(   struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer);
static void _PopUp_Loadout_Stats_Draw_Hands(struct PopUp_Loadout_Stats *pls,
                                            SDL_Renderer *renderer);
static void _PopUp_Loadout_Stats_Draw_WpnIcons(struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer);
static void _PopUp_Loadout_Stats_Draw_Equip(struct PopUp_Loadout_Stats *pls,
                                            SDL_Renderer *renderer);
static void _PopUp_Loadout_Stats_Draw_Weapons( struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer);

const struct PopUp_Loadout_Stats PopUp_Loadout_Stats_default = {
    .distance                = -1,
    .tophand_stronghand      = true, /* Tophand should basically always be stronghand */
};

/* --- STATIC FUNCTIONS --- */
/* -- Drawing elements -- */
static void _PopUp_Loadout_Stats_Draw_Arrows(struct PopUp_Loadout_Stats *pls,
                                             SDL_Renderer *renderer) {
    /* - ARROWS - */
    int arrow_index;
    SDL_Rect dstrect, srcrect;
    srcrect.y = 0;
    srcrect.h = PLS_ARROW_H;
    srcrect.w = PLS_ARROW_W;
    dstrect.h = srcrect.h;
    dstrect.w = srcrect.w;

    /* - PHYSICAL ATK ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.attack.physical > pls->initial_cs.attack.physical)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.attack.physical <
             pls->initial_cs.attack.physical)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_ATK_LARROW_X;
    dstrect.y = PLS_ATK_LARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - MAGICAL ATK ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.attack.magical > pls->initial_cs.attack.magical)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.attack.magical <
             pls->initial_cs.attack.magical)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_ATK_RARROW_X;
    dstrect.y = PLS_ATK_RARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - HIT ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.hit > pls->initial_cs.hit)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.hit < pls->initial_cs.hit)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_HIT_LARROW_X;
    dstrect.y = PLS_HIT_LARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - DODGE ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.dodge > pls->initial_cs.dodge)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.dodge < pls->initial_cs.dodge)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_HIT_RARROW_X;
    dstrect.y = PLS_HIT_RARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - CRIT ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.crit > pls->initial_cs.crit)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.crit < pls->initial_cs.crit)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_CRIT_LARROW_X;
    dstrect.y = PLS_CRIT_LARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - FAVOR ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.favor > pls->initial_cs.favor)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.favor < pls->initial_cs.favor)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_CRIT_RARROW_X;
    dstrect.y = PLS_CRIT_RARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - PHYSICAL PROT ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.protection.physical >
        pls->initial_cs.protection.physical)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.protection.physical <
             pls->initial_cs.protection.physical)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_PROT_LARROW_X;
    dstrect.y = PLS_PROT_LARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - MAGICAL PROT ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.protection.magical >
        pls->initial_cs.protection.magical)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.protection.magical <
             pls->initial_cs.protection.magical)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_PROT_RARROW_X;
    dstrect.y = PLS_PROT_RARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - RANGE MIN ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.range_loadout.min > pls->initial_cs.range_loadout.min)
        arrow_index = STAT_ARROW_DOWN;
    else if (pls->selected_cs.range_loadout.min < pls->initial_cs.range_loadout.min)
        arrow_index = STAT_ARROW_UP;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_RANGE_LARROW_X;
    dstrect.y = PLS_RANGE_LARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* - RANGE MAX ARROW - */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.range_loadout.max > pls->initial_cs.range_loadout.max)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.range_loadout.max < pls->initial_cs.range_loadout.max)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_RANGE_RARROW_X;
    dstrect.y = PLS_RANGE_RARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

    /* -- SPEED ARROW -- */
    arrow_index = STAT_ARROW_NULL;
    if (pls->selected_cs.speed > pls->initial_cs.speed)
        arrow_index = STAT_ARROW_UP;
    else if (pls->selected_cs.speed < pls->initial_cs.speed)
        arrow_index = STAT_ARROW_DOWN;

#ifdef DEBUG_LOADOUT_STATS_SHOW_ARROWS
    if (arrow_index == STAT_ARROW_NULL)
        arrow_index = STAT_ARROW_UP;
#endif
    srcrect.x = arrow_index * PLS_ARROW_W;
    dstrect.x = PLS_SPEED_ARROW_X;
    dstrect.y = PLS_SPEED_ARROW_Y;
    SDL_RenderCopy(renderer, pls->texture_arrows, &srcrect, &dstrect);

}

static void _PopUp_Loadout_Stats_Draw_Stats(   struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer) {
    /* -- COMPUTED STATS -- */
    int width;
    char numbuff[10];
    /* - ATK - */
    PixelFont_Write(pls->pixelnours, renderer, "ATK", 3, PLS_ATK_X, PLS_ATK_Y);
    if (pls->selected_cs.attack.True > 0) {
        stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.attack.physical);

        stbsp_sprintf(numbuff, "%d/%d/%d\0",
                      pls->selected_cs.attack.physical,
                      pls->selected_cs.attack.magical,
                      pls->selected_cs.attack.True);
        width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
        PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        PLS_ATK_X_STAT - width / 2 + 2, PLS_ATK_Y_STAT);
    } else {
        stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.attack.physical);
        width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
        stbsp_sprintf(numbuff, "%d/%d\0\0\0\0",
                      pls->selected_cs.attack.physical, pls->selected_cs.attack.magical);
        PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                        PLS_ATK_X_STAT - width, PLS_ATK_Y_STAT);
    }
    /* - PROT - */
    PixelFont_Write(pls->pixelnours, renderer, "DEF", 3, PLS_PROT_X, PLS_PROT_Y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.protection.physical);
    width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
    stbsp_sprintf(numbuff, "%d/%d\0\0\0\0", pls->selected_cs.protection.physical,
                  pls->selected_cs.protection.magical);
    PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                    PLS_PROT_X_STAT - width, PLS_PROT_Y_STAT);
    /* - HIT - */
    PixelFont_Write(pls->pixelnours, renderer, "HIT", 3,
                    PLS_HIT_X,
                    PLS_HIT_Y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.hit);
    width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
    stbsp_sprintf(numbuff, "%d/%d\0\0\0\0", pls->selected_cs.hit, pls->selected_cs.dodge);
    PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                    PLS_HIT_X_STAT - width, PLS_HIT_Y_STAT);
    /* - CRIT - */
    PixelFont_Write(pls->pixelnours, renderer, "CRIT", 4,
                    PLS_CRIT_X, PLS_CRIT_Y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.crit);
    width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
    stbsp_sprintf(numbuff, "%d/%d\0\0\0\0", pls->selected_cs.crit, pls->selected_cs.favor);
    PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                    PLS_CRIT_X_STAT - width, PLS_CRIT_Y_STAT);
    /* - RANGE - */
    PixelFont_Write(pls->pixelnours, renderer, "RANGE", 5,
                    PLS_RANGE_X, PLS_RANGE_Y);
    stbsp_sprintf(numbuff, "%d - %d\0", pls->selected_cs.range_loadout.min,
                  pls->selected_cs.range_loadout.max);
    PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                    PLS_RANGE_X_STAT, PLS_RANGE_Y_STAT);
    /* - SPEED - */
    PixelFont_Write(pls->pixelnours, renderer, "SPEED", 5,
                    PLS_SPEED_X, PLS_SPEED_Y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", pls->selected_cs.speed);
    width = PixelFont_Width(pls->pixelnours_big, numbuff, strlen(numbuff));
    PixelFont_Write(pls->pixelnours_big, renderer, numbuff, strlen(numbuff),
                    PLS_SPEED_X_STAT - width / 2,  PLS_SPEED_Y_STAT);

}

static void _PopUp_Loadout_Stats_Draw_Hands(struct PopUp_Loadout_Stats *pls,
                                            SDL_Renderer *renderer) {
    SDL_assert(pls != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    SDL_Rect dstrect, srcrect;

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    int stronghand = Unit_Hand_Strong(unit);

    /* - HAND ICONS - */
    /* Left hand */
    if (Unit_hasHand(unit, UNIT_HAND_LEFT)) {
        srcrect.w = PLS_HANDS_TILESIZE;
        srcrect.h = PLS_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        int index = (stronghand == UNIT_HAND_RIGHT) ? PLS_HANDS_SMALL_L : PLS_HANDS_BIG_L;
        srcrect.x = index * srcrect.w;
        srcrect.y = 0;

        // Moving hand if two handing or small hand
        dstrect.x = PLS_HANDL_X;
        dstrect.y = PLS_HANDL_Y + pls->ly_offset;

        // Moving hand if small
        if (stronghand == UNIT_HAND_RIGHT) {
            dstrect.x += PLS_HAND_SMALLX_OFFSET;
            dstrect.y += PLS_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, pls->texture_hands, &srcrect, &dstrect);
    }

    /* Right hand */
    if (Unit_hasHand(unit, UNIT_HAND_RIGHT)) {
        srcrect.w = PLS_HANDS_TILESIZE;
        srcrect.h = PLS_HANDS_TILESIZE;
        dstrect.w = srcrect.w;
        dstrect.h = srcrect.h;

        int index = (stronghand == UNIT_HAND_LEFT) ? PLS_HANDS_SMALL_R : PLS_HANDS_BIG_R;
        srcrect.x = index * srcrect.w;
        srcrect.y = 0;

        // Moving hand if two handing or small hand
        // if ambidextrous, LEFT hand is strong hand/on top
        dstrect.x = PLS_HANDR_X;
        dstrect.y = PLS_HANDR_Y + pls->ry_offset;

        // Moving hand if small
        if (stronghand == UNIT_HAND_LEFT) {
            dstrect.x += PLS_HAND_SMALLX_OFFSET;
            dstrect.y += PLS_HAND_SMALLY_OFFSET;
        }

        SDL_RenderCopy(renderer, pls->texture_hands, &srcrect, &dstrect);
    }
}

static void _PopUp_Loadout_Stats_Draw_WpnIcons(struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer) {
    SDL_assert(pls != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);

    /* - WEAPON ICONS - */
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    int stronghand = Unit_Hand_Strong(unit);
    SDL_Rect dstrect, srcrect;

    srcrect.w = PLS_WPN_TILESIZE;
    srcrect.h = PLS_WPN_TILESIZE;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    if (Loadout_istwoHanding(&pls->loadout_selected)) {
        dstrect.x = PLS_WPNL_X;
        dstrect.y = PLS_WPNL_Y + PLS_TWOHAND_Y_OFFSET;
        srcrect.x = (pls->type_left % PLS_WPN_COL_LEN)
                    * PLS_WPN_TILESIZE;
        srcrect.y = (pls->type_left / PLS_WPN_COL_LEN) * PLS_WPN_TILESIZE;
        SDL_RenderCopy(renderer, pls->texture_weapon_icons, &srcrect, &dstrect);
    } else {
        /* Left hand */
        dstrect.x = PLS_WPNL_X;
        dstrect.y = PLS_WPNL_Y;
        if ((pls->tophand_stronghand) && (stronghand == UNIT_HAND_RIGHT)) {
            dstrect.y += PLS_WEAKHAND_Y_OFFSET;
        }
        srcrect.x = (pls->type_left % PLS_WPN_COL_LEN) * PLS_WPN_TILESIZE;
        srcrect.y = (pls->type_left / PLS_WPN_COL_LEN) * PLS_WPN_TILESIZE;
        SDL_RenderCopy(renderer, pls->texture_weapon_icons, &srcrect, &dstrect);

        /* Right hand */
        dstrect.x = PLS_WPNR_X;
        dstrect.y = PLS_WPNR_Y;
        if ((!pls->tophand_stronghand) || (stronghand == UNIT_HAND_LEFT)) {
            dstrect.y += PLS_WEAKHAND_Y_OFFSET;
        }
        srcrect.x = (pls->type_right % PLS_WPN_COL_LEN) * PLS_WPN_TILESIZE;
        srcrect.y = (pls->type_right / PLS_WPN_COL_LEN) * PLS_WPN_TILESIZE;
        SDL_RenderCopy(renderer, pls->texture_weapon_icons, &srcrect, &dstrect);
    }
}

static void _PopUp_Loadout_Stats_Draw_Equip(struct PopUp_Loadout_Stats *pls,
                                            SDL_Renderer *renderer) {
    SDL_assert(pls != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);

    /* - EQUIP ICONS - */
    SDL_Rect dstrect, srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = PLS_ICON_EQUIP_W;
    srcrect.h = PLS_ICON_EQUIP_H;
    dstrect.w = srcrect.w;
    dstrect.h = srcrect.h;

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    if (Loadout_istwoHanding(&pls->loadout_selected)) {
        dstrect.x = PLS_ICON_EQUIPL_X;
        dstrect.y = PLS_ICON_EQUIPL_Y + PLS_TWOHAND_Y_OFFSET;
        SDL_RenderCopy(renderer, pls->texture_equip, &srcrect, &dstrect);
    } else {
        /* Left hand */
        canEquip can_equip  = canEquip_default;
        i32 eq = Unit_Eq_Equipped(unit, UNIT_HAND_LEFT);
        if (eq > ITEM_NULL) {
            canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  Unit_Eq_Equipped(unit, UNIT_HAND_LEFT));
        }
        if (Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT) > ITEM_NULL) {
            canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, Unit_Eq_Equipped(unit, UNIT_HAND_RIGHT));
        }

        canEquip_Eq(&can_equip, UNIT_HAND_LEFT);
        can_equip.hand      = UNIT_HAND_LEFT;
        if (Unit_canEquip(unit, can_equip) || pls->l_equip_override) {
            dstrect.x = PLS_ICON_EQUIPL_X;
            dstrect.y = PLS_ICON_EQUIPL_Y + pls->ly_offset;
            SDL_RenderCopy(renderer, pls->texture_equip, &srcrect, &dstrect);
        }

        /* Right hand */
        canEquip_Eq(&can_equip, UNIT_HAND_LEFT);
        can_equip.hand      = UNIT_HAND_RIGHT;
        if (Unit_canEquip(unit, can_equip) || pls->r_equip_override) {
            dstrect.x = PLS_ICON_EQUIPR_X;
            dstrect.y = PLS_ICON_EQUIPR_Y + pls->ry_offset;
            SDL_RenderCopy(renderer, pls->texture_equip, &srcrect, &dstrect);
        }
    }
}

static void _PopUp_Loadout_Stats_Draw_Weapons( struct PopUp_Loadout_Stats *pls,
                                               SDL_Renderer *renderer) {
    /* - EQUIPPED WEAPONS NAMES - */
    /* Left hand */

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    do {
        i32 eq = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_LEFT);

        if (!eq_valid(eq))
            break;

        SDL_assert(gl_weapons_dtab != NULL);

        i32 id = Unit_Id_Equipment(unit, eq);
        int x = PLS_NAMEL_X;
        int y = PLS_NAMEL_Y + pls->ly_offset;
        int width;
        if (id == ITEM_NULL) {
            width = PixelFont_Width(pls->pixelnours, "-", 1);
            PixelFont_Write(pls->pixelnours, renderer, "-", 1, x - width, y);
            break;
        }

        const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, id);
        if (wpn == NULL)
            break;

        size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);

        s8 buffer   = s8_mut(global_itemNames[item_order].data);
        buffer      = s8_toUpper(buffer);

        if (Loadout_istwoHanding(&pls->loadout_selected)) {
            width = PixelFont_Width(pls->pixelnours, buffer.data, buffer.num);
            x = PLS_HEADER_X - width / 2 + PLS_HEADER_W / 2;
        }
        PixelFont_Write(pls->pixelnours, renderer, buffer.data, buffer.num, x, y);
        s8_free(&buffer);
    } while (false);

    /* Right hand */
    do {
        if (Loadout_istwoHanding(&pls->loadout_selected))
            break;

        i32 eq = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_RIGHT);

        if (!eq_valid(eq))
            break;

        SDL_assert(gl_weapons_dtab != NULL);

        i32 id = Unit_Id_Equipment(unit, eq);
        int x = PLS_NAMER_X;
        int y = PLS_NAMER_Y + pls->ry_offset;
        int width;
        if (id == ITEM_NULL) {
            width = PixelFont_Width(pls->pixelnours, "-", 1);
            PixelFont_Write(pls->pixelnours, renderer, "-", 1, x - width, y);
            break;
        }

        const Weapon *wpn = DTAB_GET_CONST(gl_weapons_dtab, id);
        if (wpn == NULL)
            break;

        size_t item_order = *(u16 *)DTAB_GET(global_itemOrders, id);

        s8 buffer   = s8_mut(global_itemNames[item_order].data);
        buffer      = s8_toUpper(buffer);

        width = PixelFont_Width(pls->pixelnours, buffer.data, buffer.num);

        PixelFont_Write(pls->pixelnours, renderer, buffer.data, buffer.num, x - width, y);

        s8_free(&buffer);
    } while (false);
}

/* --- GLOBAL FUNCTIONS --- */
struct PopUp_Loadout_Stats *PopUp_Loadout_Stats_Alloc(void) {
    struct PopUp_Loadout_Stats *pls = SDL_malloc(sizeof(struct PopUp_Loadout_Stats));
    SDL_assert(pls != NULL);
    *pls = PopUp_Loadout_Stats_default;

    return (pls);
}


void PopUp_Loadout_Stats_Free(struct PopUp_Loadout_Stats *pls) {
    if (pls->texture != NULL) {
        SDL_DestroyTexture(pls->texture);
        pls->texture = NULL;
    }
    if (pls->texture_arrows != NULL) {
        SDL_DestroyTexture(pls->texture_arrows);
        pls->texture_arrows = NULL;
    }
    if (pls->texture_hands != NULL) {
        SDL_DestroyTexture(pls->texture_hands);
        pls->texture_hands = NULL;
    }
    if (pls->texture_weapon_icons != NULL) {
        SDL_DestroyTexture(pls->texture_weapon_icons);
        pls->texture_weapon_icons = NULL;
    }

}


void PopUp_Loadout_Stats_Load(struct PopUp_Loadout_Stats *pls, SDL_Renderer *renderer,
                              struct n9Patch *n9patch) {
    SDL_assert(pls      != NULL);
    SDL_assert(gl_world    != NULL);
    SDL_assert(n9patch  != NULL);
    PopUp_Loadout_Stats_Free(pls);
    n9Patch_Free(n9patch);

    pls->update = true;

    /* -- Popup position, scale, n9Patch -- */
    *n9patch                  = n9Patch_default;
    n9patch->px.x   = PLS_PATCH_PIXELS;
    n9patch->px.y   = PLS_PATCH_PIXELS;
    n9patch->scale.x          = PLS_N9PATCH_SCALE_X;
    n9patch->scale.y          = PLS_N9PATCH_SCALE_Y;
    n9patch->num.x   = PLS_PATCH_X_SIZE;
    n9patch->num.y   = PLS_PATCH_Y_SIZE;
    n9patch->pos.x            = 0;
    n9patch->pos.y            = 0;
    SDL_assert(n9patch->px.x > 0);
    SDL_assert(n9patch->px.y > 0);

    Point size = {
        .x  = (PLS_PATCH_PIXELS * PLS_PATCH_X_SIZE),
        .y  = (PLS_PATCH_PIXELS * PLS_PATCH_Y_SIZE),
    };
    n9Patch_Pixels_Total_Set(n9patch, size);

    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px_fancycorner.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path  = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Arrows.png");
    pls->texture_arrows = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pls->texture_arrows);

    path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    pls->texture_weapon_icons = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pls->texture_weapon_icons);

    path = PATH_JOIN("..", "assets", "GUI", "Menu", "StatsMenu_Icons_Hands.png");
    pls->texture_hands = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pls->texture_hands);

    path = PATH_JOIN("..", "assets", "GUI", "Popup", "PopUp_Tile_Header.png");
    pls->texture_header = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pls->texture_header);

    path = PATH_JOIN("..", "assets", "GUI", "Icon", "Icon_Equipped.png");
    pls->texture_equip = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(pls->texture_equip);

}

void PopUp_Loadout_Stats_ItemTypes(struct PopUp_Loadout_Stats *pls) {
    // Get item type of each waepon in hand to draw icons.
    SDL_assert(pls->unit_ent            > TNECS_NULL);

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    SDL_assert(gl_weapons_dtab  != NULL);

    /* Left hand item type */
    int eq = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_LEFT);
    if (eq_valid(eq)) {
        int id = Unit_Id_Equipment(unit, eq);

        if (Weapon_ID_isValid(id)) {
            Weapon_Load(gl_weapons_dtab, id);
            pls->type_left = Weapon_TypeExp(DTAB_GET_CONST(gl_weapons_dtab, id));
        } else {
            pls->type_left = ITEM_TYPE_ITEM;
        }
    }

    /* Right hand item type */
    eq = Loadout_Eq(&pls->loadout_selected, UNIT_HAND_RIGHT);
    if (eq_valid(eq)) {
        int id = Unit_Id_Equipment(unit, eq);

        if (Weapon_ID_isValid(id)) {
            Weapon_Load(gl_weapons_dtab, id);
            pls->type_right = Weapon_TypeExp(DTAB_GET_CONST(gl_weapons_dtab, id));
        } else {
            pls->type_right = ITEM_TYPE_ITEM;
        }
    }
}

/* --- Setters --- */
void PopUp_Loadout_Stats_Unit(struct PopUp_Loadout_Stats *pls,  tnecs_E unit_ent) {
    SDL_assert(pls  != NULL);
    SDL_assert(unit_ent > TNECS_NULL);
    pls->unit_ent       = unit_ent;
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    SDL_assert(unit != NULL);

    PopUp_Loadout_Stats_Initial_Loadout(pls);
    PopUp_Loadout_Stats_Initial_Stats(pls);
}

void  PopUp_Loadout_Stats_Initial_Loadout(  struct PopUp_Loadout_Stats *pls) {
    SDL_assert(pls          != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    SDL_assert(unit != NULL);

    Unit_Loadout_Export(unit, &pls->loadout_initial);
}

void  PopUp_Loadout_Stats_Selected_Loadout( struct PopUp_Loadout_Stats *pls) {
    SDL_assert(pls          != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    SDL_assert(unit != NULL);

    Unit_Loadout_Export(unit, &pls->loadout_selected);
}

void PopUp_Loadout_Stats_Initial_Stats(struct PopUp_Loadout_Stats *pls) {
    SDL_assert(pls          != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    SDL_assert(unit != NULL);

    pls->initial_cs     = Unit_computedStats_wLoadout(unit, &pls->loadout_initial, pls->distance);
    pls->update         = true;
}

void PopUp_Loadout_Stats_Selected_Stats(struct PopUp_Loadout_Stats *pls) {
    SDL_assert(pls          != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);
    SDL_assert(unit != NULL);

    /* Compute loadout stats of "selected" loadout compared with "initial" loadout */
    PopUp_Loadout_Stats_ItemTypes(pls);

    pls->selected_cs    = Unit_computedStats_wLoadout(unit, &pls->loadout_selected, pls->distance);
    pls->update         = true;
}

/* --- Select --- */
void PopUp_Loadout_Stats_Hover(struct PopUp_Loadout_Stats *pls, struct LoadoutSelectMenu *wsm,
                               int elem) {
    /* Set pls items to weapons hovered in wsm */
    SDL_assert(pls       != NULL);
    SDL_assert(wsm       != NULL);
    SDL_assert(wsm->_unit > TNECS_NULL);
    SDL_assert(pls->unit_ent == wsm->_unit);
    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    /* - Select item - */
    int stronghand  = Unit_Hand_Strong(unit);
    int weakhand    = Unit_Hand_Weak(unit);
    int hand        = Loadout_isEquipped(&wsm->selected, stronghand) ? weakhand : stronghand;

    Loadout_Set(&pls->loadout_selected, hand, wsm->equippable.arr[elem]);
    SDL_assert(Loadout_Eq(&pls->loadout_selected, hand) == wsm->equippable.arr[elem]);
    pls->update = true;
}

void PopUp_Loadout_Stats_Select(struct PopUp_Loadout_Stats *pls, struct LoadoutSelectMenu *wsm) {
    /* Set pls items to weapons selected in wsm */
    SDL_assert(pls       != NULL);
    SDL_assert(wsm       != NULL);
    SDL_assert(wsm->_unit > TNECS_NULL);
    SDL_assert(pls->unit_ent == wsm->_unit);

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    /* - Select item - */
    int stronghand  = Unit_Hand_Strong(unit);
    int weakhand    = Unit_Hand_Weak(unit);

    if (Loadout_isEquipped(&wsm->selected, stronghand))  {
        Loadout_Set(&pls->loadout_selected, stronghand, Loadout_Eq(&wsm->selected, stronghand));
    }

    if (Loadout_isEquipped(&wsm->selected, weakhand))  {
        Loadout_Set(&pls->loadout_selected, weakhand, Loadout_Eq(&wsm->selected, weakhand));
    }
    pls->update = true;
}

/* --- Rendering --- */
void PopUp_Loadout_Stats_Weakhand_Offset(struct PopUp_Loadout_Stats *pls) {
    SDL_assert(pls       != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);

    Unit *unit = IES_GET_C(gl_world, pls->unit_ent, Unit);

    int stronghand = Unit_Hand_Strong(unit);
    pls->ry_offset = 0;
    pls->ly_offset = 0;

    /* Weakhand offsets */
    if (Loadout_istwoHanding(&pls->loadout_selected))
        pls->ly_offset = PLS_TWOHAND_Y_OFFSET;
    else if ((pls->tophand_stronghand) && (stronghand == UNIT_HAND_RIGHT)) {
        pls->ly_offset = PLS_WEAKHAND_Y_OFFSET;
    }
    if (Loadout_istwoHanding(&pls->loadout_selected)) {
        pls->ry_offset = PLS_TWOHAND_Y_OFFSET;
    } else if ((!pls->tophand_stronghand) || (stronghand == UNIT_HAND_LEFT)) {
        pls->ry_offset = PLS_WEAKHAND_Y_OFFSET;
    }
}

void PopUp_Loadout_Stats_Draw(struct PopUp *popup, struct Point pos,
                              SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PopUp_Loadout_Stats *pls     =  popup->data;
    struct n9Patch             *n9patch = &popup->n9patch;

    SDL_assert(pls != NULL);
    if (pls->update) {
        PopUp_Loadout_Stats_Update(pls, n9patch,  render_target, renderer);
        pls->update = false;
    }

    Point size = n9Patch_Pixels_Total(n9patch);
    SDL_Rect dstrect = {
        .w = size.x * n9patch->scale.x,
        .h = size.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y,
    };
    SDL_assert(pls->texture != NULL);
    SDL_RenderCopy(renderer, pls->texture, NULL, &dstrect);
    Utilities_DrawColor_Reset(renderer);
}

void PopUp_Loadout_Stats_Update(struct PopUp_Loadout_Stats *pls, struct n9Patch *n9patch,
                                SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(pls != NULL);
    SDL_assert(pls->unit_ent > TNECS_NULL);
    SDL_assert(renderer != NULL);
    /* - variable declaration/ ants definition - */
    Point size = n9Patch_Pixels_Total(n9patch);

    SDL_assert(size.x > 0);
    SDL_assert(size.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    i16 menu_w = size.x;
    i16 menu_h = size.y + PLS_DEST_Y;
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);

    /* - create render target texture - */
    if (pls->texture == NULL) {
        pls->texture = SDL_CreateTexture(   renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            menu_w, menu_h);
        SDL_assert(pls->texture != NULL);
        SDL_SetTextureBlendMode(pls->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, pls->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_assert(pls->texture != NULL);

    /* -- PATCHES DRAW -- */
    int scale_x         = n9patch->scale.x;
    int scale_y         = n9patch->scale.y;
    n9patch->scale.x    = 1;
    n9patch->scale.y    = 1;
    n9patch->pos.x      = 0;
    n9patch->pos.y      = PLS_DEST_Y;
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x    = scale_x;
    n9patch->scale.y    = scale_y;

    /* -- HEADER -- */
    SDL_Rect dstrect;
    dstrect.x = PLS_HEADER_X;
    dstrect.y = PLS_HEADER_Y;
    dstrect.y = PLS_HEADER_Y;
    dstrect.w = PLS_HEADER_W;
    dstrect.h = PLS_HEADER_H;
    SDL_RenderCopy(renderer, pls->texture_header, NULL, &dstrect);

    /* -- Weakhand offsets -- */
    PopUp_Loadout_Stats_Weakhand_Offset(pls);

    /* --- RENDERING --- */
    _PopUp_Loadout_Stats_Draw_Hands(     pls, renderer);
    _PopUp_Loadout_Stats_Draw_Equip(     pls, renderer);
    _PopUp_Loadout_Stats_Draw_WpnIcons(  pls, renderer);
    _PopUp_Loadout_Stats_Draw_Weapons(   pls, renderer);
    _PopUp_Loadout_Stats_Draw_Stats(     pls, renderer);
    _PopUp_Loadout_Stats_Draw_Arrows(    pls, renderer);
    SDL_SetRenderTarget(renderer, render_target);
    Utilities_DrawColor_Reset(renderer);
}
