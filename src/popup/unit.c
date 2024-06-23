#include "popup/unit.h"

struct PopUp_Unit PopUp_Unit_default = {
    .unit             = NULL,
    .texture          = NULL,
    .texture_header   = NULL,
    .pixelnours       = NULL,
    .pixelnours_big   = NULL,
    .update           = true,
    .corner           = SOTA_DIRECTION_TOPLEFT,
    .offset           = {0, 0},
    .cursor_limit_min = {-1, -1},
    .cursor_limit_max = {-1, -1},
    .distance         = -1,
};

void PopUp_Unit_Free(struct PopUp_Unit *pu) {
    if (pu->texture != NULL) {
        SDL_DestroyTexture(pu->texture);
        pu->texture = NULL;
    }

    if (pu->texture_weapons != NULL) {
        SDL_DestroyTexture(pu->texture_weapons);
        pu->texture_weapons = NULL;
    }

}

void PopUp_Unit_Load(struct PopUp_Unit *pu, SDL_Renderer *renderer, struct n9Patch *n9patch) {
    PopUp_Unit_Free(pu);
    n9Patch_Free(n9patch);

    *n9patch                    = n9Patch_default;
    n9patch->patch_pixels.x     = PU_PATCH_PIXELS;
    n9patch->patch_pixels.y     = PU_PATCH_PIXELS;
    n9patch->scale.x            = PU_N9PATCH_SCALE_X;
    n9patch->scale.y            = PU_N9PATCH_SCALE_Y;
    n9patch->size_pixels.x      = (PU_PATCH_PIXELS * PU_PATCH_X_SIZE);
    n9patch->size_pixels.y      = (PU_PATCH_PIXELS * PU_PATCH_Y_SIZE);
    n9patch->size_patches.x     = PU_PATCH_X_SIZE;
    n9patch->size_patches.y     = PU_PATCH_Y_SIZE;
    n9patch->pos.x = 0;
    n9patch->pos.y = 0;
    SDL_assert(n9patch->patch_pixels.x > 0);
    SDL_assert(n9patch->patch_pixels.y > 0);

    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "tilepopup8px.png");
    n9patch->texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    pu->texture_weapons = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    path = PATH_JOIN("..", "assets", "GUI", "Popup", "PopUp_Tile_Header.png");
    pu->texture_header = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

}

/* --- Setters --- */
void PopUp_Unit_Set(struct PopUp_Unit *pu, struct Game *sota) {
    SDL_assert(pu != NULL);
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, sota->hovered_unit_entity, Unit);
    _PopUp_Unit_Set(pu, unit);
}

void _PopUp_Unit_Set(struct PopUp_Unit *pu, struct Unit *unit) {
    SDL_assert(pu != NULL);
    pu->unit   = unit;
    pu->update = true;
    SDL_assert(pu->unit->name.data != NULL);
}

/* --- Positioning --- */
struct Point PopUp_Unit_Offset(struct PopUp_Unit *pu, struct Settings *settings) {
    pu->offset.x = settings->res.x / PU_FACTOR_X;
    pu->offset.y = settings->res.y / PU_FACTOR_Y;
    return (pu->offset);
}

void PopUp_Unit_Limits(struct PopUp *popup, struct PopUp_Unit *pu, struct Settings *settings) {
    pu->cursor_limit_min.x = 0.4f * settings->res.x;
    pu->cursor_limit_max.x = 0.6f * settings->res.x;
}

struct Point PopUp_Unit_Center_Name(struct PopUp_Unit *pu, struct n9Patch *n9patch,
                                    char *numbuff, size_t str_len) {
    SDL_assert(pu->pixelnours_big != NULL);
    struct Point out = {PU_ID_X, PU_ID_Y};
    size_t len = 0;
    for (size_t i = 0; i < str_len; i++)
        len += pu->pixelnours_big->glyph_bbox_width[numbuff[i]];
    SDL_assert(len < n9patch->size_pixels.x);
    out.x = (n9patch->size_pixels.x - len) / 2 - PU_ID_X;
    return (out);
}

struct Point PopUp_Unit_Position(struct PopUp *popup, struct PopUp_Unit *pu,
                                 struct n9Patch *n9patch, struct Settings *settings,
                                 struct Point *pixel_pos) {
    /* error checking: PopUp_Unit should NOT be on bottom*/
    switch (pu->corner) {
        case SOTA_DIRECTION_BOTRIGHT:
            pu->corner = SOTA_DIRECTION_TOPRIGHT;
            break;
        case SOTA_DIRECTION_BOTLEFT:
            pu->corner = SOTA_DIRECTION_TOPLEFT;
            break;
    }
    /* Check if popup should move corner */
    b32 x_lower   = pixel_pos->x <= pu->cursor_limit_min.x;
    b32 x_greater = pixel_pos->x >= pu->cursor_limit_max.x;
    if ((x_greater) && (pu->corner == SOTA_DIRECTION_TOPRIGHT))
        pu->corner = SOTA_DIRECTION_TOPLEFT;
    if ((x_lower) && (pu->corner == SOTA_DIRECTION_TOPLEFT))
        pu->corner = SOTA_DIRECTION_TOPRIGHT;
    struct Point out = {-1, 0};
    struct Point sign = {1, 1};
    if (pu->unit == NULL) {
        /* - PopUpUnit goes offscreen and waits for another unit - */
        switch (pu->corner) {
            case SOTA_DIRECTION_TOPLEFT:
                out.x  = -(n9patch->size_pixels.x + pu->offset.x) * n9patch->scale.x;
                break;
            case SOTA_DIRECTION_TOPRIGHT:
                out.x  = settings->res.x + pu->offset.x * n9patch->scale.x;
                sign.x = -1;
                break;
            default:
                SDL_Log("Invalid PopUp_Unit corner value");
        }
    } else {
        /* - PopUpUnit shows itself - */
        switch (pu->corner) {
            case SOTA_DIRECTION_TOPLEFT:
                out.x  = -2 * n9patch->scale.x;
                break;
            case SOTA_DIRECTION_TOPRIGHT:
                out.x  = settings->res.x - (n9patch->size_pixels.x - 1) * n9patch->scale.x;
                sign.x = -1;
                break;
            default:
                SDL_Log("Invalid PopUp_Unit corner value");
        }
    }
    out.x += sign.x * pu->offset.x * n9patch->scale.x;
    out.y += sign.y * pu->offset.y * n9patch->scale.y;
    // if popup->pos is set with this output, sliding is skipped
    return (out);
}

void PopUp_Unit_Draw(struct PopUp *popup, struct Point pos,
                     SDL_Texture *render_target, SDL_Renderer *renderer) {
    struct PopUp_Unit *pu   = (struct PopUp_Unit *)popup->data;

    if (pu->unit == NULL) {
        return;
    }

    struct n9Patch *n9patch = &popup->n9patch;

    if (pu->update) {
        PopUp_Unit_Update(pu, n9patch, render_target, renderer);
        pu->update = false;
    }
    SDL_Rect dstrect = {
        .w = n9patch->size_pixels.x * n9patch->scale.x,
        .h = n9patch->size_pixels.y * n9patch->scale.y,
        .x = pos.x,
        .y = pos.y
    };
    SDL_assert(pu->texture != NULL);
    SDL_RenderCopy(renderer, pu->texture, NULL, &dstrect);
}

void PopUp_Unit_Update(struct PopUp_Unit *pu, struct n9Patch *n9patch,
                       SDL_Texture *render_target, SDL_Renderer *renderer) {
    /* --- PRELIMINARIES --- */
    SDL_assert(n9patch  != NULL);
    SDL_assert(pu       != NULL);
    SDL_assert(renderer != NULL);
    SDL_assert(pu->unit != NULL);
    /* -- Variable declaration/ ants definition -- */
    SDL_assert(n9patch->size_pixels.x > 0);
    SDL_assert(n9patch->size_pixels.y > 0);
    SDL_assert(n9patch->scale.x > 0);
    SDL_assert(n9patch->scale.y > 0);
    SDL_Rect dstrect, srcrect;
    char numbuff[10];
    i16 menu_w = n9patch->size_pixels.x;
    i16 menu_h = (n9patch->size_pixels.y + PU_HEADER_Y);
    SDL_assert(menu_w > 0);
    SDL_assert(menu_h > 0);
    struct Computed_Stats comp_s    = Unit_computedStats(pu->unit, pu->distance);
    struct Unit_stats eff_s         = Unit_effectiveStats(pu->unit);
    /* -- Create render target texture -- */
    if (pu->texture == NULL) {
        pu->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_TARGET, menu_w, menu_h);
        SDL_SetTextureBlendMode(pu->texture, SDL_BLENDMODE_BLEND);

    }
    SDL_assert(pu->texture != NULL);
    // if (pu->texture_weapons == NULL) {
    //     char *path = PATH_JOIN("..", "assets", "GUI", "Icon", "StatsMenu_Icons_Weapons.png");
    //     pu->texture_weapons = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    // }
    SDL_assert(pu->texture_weapons != NULL);

    SDL_SetRenderTarget(renderer, pu->texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_assert(pu->texture != NULL);
    /* --- RENDERING PopUp_Unit --- */
    /* -- PATCHES DRAW -- */
    int scale_x = n9patch->scale.x;
    int scale_y = n9patch->scale.y;
    n9patch->scale.x = 1;
    n9patch->scale.y = 1;
    n9patch->pos.x   = 0;
    n9patch->pos.y   = (PU_HEADER_H - 3);
    n9Patch_Draw(n9patch, renderer);
    n9patch->scale.x = scale_x;
    n9patch->scale.y = scale_y;

    /* -- HEADER WRITING -- */
    if (pu->texture_header != NULL) {
        dstrect.x = PU_HEADER_X;
        dstrect.y = PU_HEADER_Y;
        dstrect.w = PU_HEADER_W;
        dstrect.h = PU_HEADER_H;

        SDL_RenderCopy(renderer, pu->texture_header, NULL, &dstrect);
    }
    /* -- NAME -- */
    struct Point pos;
    s8 name = pu->unit->name;
    SDL_assert(name.data != NULL);
    SDL_Log("name.data %s %d", name.data, name.num );
    pos = PopUp_Unit_Center_Name(pu, n9patch, name.data, name.num);
    PixelFont_Write(pu->pixelnours_big, renderer, name.data, name.num, pos.x, pos.y);
    /* -- PORTRAIT/FACE -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    srcrect.w = PU_PORTRAIT_W;
    srcrect.h = PU_PORTRAIT_H;
    srcrect.x = PU_PORTRAIT_X;
    srcrect.y = PU_PORTRAIT_Y;
    SDL_RenderFillRect(renderer, &srcrect);

    /* -- Weapon icons -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    srcrect.w = PU_WPN_ICON_H;
    srcrect.h = PU_WPN_ICON_H;
    srcrect.x = PU_ICONL_X;
    srcrect.y = PU_ICONL_Y;
    SDL_RenderFillRect(renderer, &srcrect);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    srcrect.w = PU_WPN_ICON_W;
    srcrect.h = PU_WPN_ICON_H;
    srcrect.x = PU_ICONR_X;
    srcrect.y = PU_ICONR_Y;
    SDL_RenderFillRect(renderer, &srcrect);

    dstrect.y = PU_ICONL_Y + 1;
    dstrect.h = PU_WPN_ICON_W;
    dstrect.w = PU_WPN_ICON_H;
    srcrect.w = PU_WPN_ICON_H;
    srcrect.h = PU_WPN_ICON_W;
    if (!pu->unit->isTwoHanding) {
        /* Unit is NOT two handing, printing both left and right weapons */
        // DESIGN QUESTION:
        // Face in popup unit may confuse item sides
        // -> Face right is Player left and vice versa
        // - Which is best? Left/Right in player POV of face POV?

        /* Strong hand */
        dstrect.x = PU_ICONL_X + 1;
        int unit_hand_strong    = Unit_Hand_Strong(pu->unit);
        int unit_hand_weak      = 1 - unit_hand_strong;

        Inventory_item *item = Unit_Item_Equipped(pu->unit, unit_hand_strong);
        if (Unit_isEquipped(pu->unit, unit_hand_strong) && (item->id > ITEM_NULL)) {
            Weapon_Load(pu->unit->weapons_dtab, item->id);
            struct Weapon *weapon = DTAB_GET(pu->unit->weapons_dtab, item->id);
            SDL_assert(weapon != NULL);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PU_WPN_ICON_ROWLEN) * PU_WPN_ICON_H;
            srcrect.y = (type_ind / PU_WPN_ICON_ROWLEN) * PU_WPN_ICON_W;
            SDL_RenderCopy(renderer, pu->texture_weapons, &srcrect, &dstrect);
        } else {
            PixelFont_Write(pu->pixelnours, renderer, "-", 1, dstrect.x + PU_HYPHEN_OFFSET_X,
                            dstrect.y + PU_HYPHEN_OFFSET_Y);
        }

        /* right hand */
        dstrect.x = PU_ICONR_X + 1;
        item = Unit_Item_Equipped(pu->unit, unit_hand_weak);
        if (Unit_isEquipped(pu->unit, unit_hand_weak) && (item->id > ITEM_NULL)) {
            Weapon_Load(pu->unit->weapons_dtab, item->id);
            struct Weapon *weapon = DTAB_GET(pu->unit->weapons_dtab, item->id);
            SDL_assert(weapon != NULL);
            u16 type_ind = Weapon_TypeExp(weapon);
            srcrect.x = (type_ind % PU_WPN_ICON_ROWLEN) * PU_WPN_ICON_H;
            srcrect.y = (type_ind / PU_WPN_ICON_ROWLEN) * PU_WPN_ICON_W;
            SDL_RenderCopy(renderer, pu->texture_weapons, &srcrect, &dstrect);
        } else {
            PixelFont_Write(pu->pixelnours, renderer, "-", 1, dstrect.x + PU_HYPHEN_OFFSET_X,
                            dstrect.y + PU_HYPHEN_OFFSET_Y);
        }
    } else {
        /* Unit is two handing, printing ONE icon in the center */
        // dstrect.x = (pu_SIMPLE_DICONL_X + pu_SIMPLE_ICON_OFFSET_X) / 2 + (pu_SIMPLE_DICONR_X +
        //             pu_SIMPLE_ICON_OFFSET_X) / 2;
        // item = pu->unit->equipment[UNIT_HAND_WEAK];
        // SDL_assert(item->id > ITEM_NULL);
        // struct Weapon *weapon = DTAB_GET(pu->unit->weapons_dtab, item->id);
        // u16 type = weapon->item->type;
        // // TODO: weapon with multiple types
        // SDL_assert(weapon->item->type > 0);
        // int type_ind = (int)(log(type) / log(2.0f)) + 1;
        // SDL_assert(type_ind > 0);
        // srcrect.x = (type_ind % pu_SIMPLE_ICON_ROWLEN) * pu_SIMPLE_ICON_W;
        // srcrect.y = (type_ind / pu_SIMPLE_ICON_ROWLEN) * pu_SIMPLE_ICON_H;
        // SDL_RenderCopy(renderer, pu->texture_weapons, &srcrect, &dstrect);
    }

    /* -- EXP/Level -- */
    PixelFont_Write(pu->pixelnours, renderer, "EXP", 3, PU_EXP_X, PU_EXP_Y);
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", (pu->unit->exp % SOTA_100PERCENT));
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_EXP_STAT_X, PU_EXP_STAT_Y);
    PixelFont_Write(pu->pixelnours, renderer, "Lv", 2, PU_LV_X, PU_LV_Y);
    stbsp_sprintf(numbuff, "%d\0\0\0\0", (pu->unit->exp / SOTA_100PERCENT));
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_LV_STAT_X, PU_LV_STAT_Y);
    /* -- HP -- */
    PixelFont_Write(pu->pixelnours, renderer, "HP", 2,
                    PU_HP_X, PU_HP_Y);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", pu->unit->current_hp, eff_s.hp);
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_HP_STAT_X, PU_HP_STAT_Y);
    struct SimpleBar hp_bar = SimpleBar_default;
    hp_bar.scale.x = 1, hp_bar.scale.y = 1;
    StatBar_Colors_NES(&hp_bar, 14, 15, 52, 54);
    StatBar_Init(&hp_bar, pu->unit->current_hp, eff_s.hp, PU_HPBAR_X, PU_HPBAR_Y);
    hp_bar.len = PU_HPBAR_LEN;
    hp_bar.height = 2;
    SimpleBar_Draw(&hp_bar, renderer);
    /* -- COMPUTED STATS -- */
    SDL_assert(pu->pixelnours != NULL);
    SDL_assert(pu->pixelnours_big != NULL);
    PixelFont_Write(pu->pixelnours, renderer, "ATK", 3, PU_ATK_X, PU_ATK_Y);
    if (comp_s.attack[DMG_TYPE_TRUE] > 0) {
        stbsp_sprintf(numbuff, "%02d/%02d/%01d\0\0\0\0", comp_s.attack[DMG_TYPE_PHYSICAL],
                      comp_s.attack[DMG_TYPE_MAGICAL], comp_s.attack[DMG_TYPE_TRUE]);
        PixelFont_Write(pu->pixelnours, renderer, numbuff, 7, PU_ATK_X_STAT1, PU_ATK_Y_STAT1);
    } else {
        stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", comp_s.attack[DMG_TYPE_PHYSICAL],
                      comp_s.attack[DMG_TYPE_MAGICAL], comp_s.attack[DMG_TYPE_TRUE]);
        PixelFont_Write(pu->pixelnours, renderer, numbuff, 5, PU_ATK_X_STAT1, PU_ATK_Y_STAT1);
    }
    PixelFont_Write(pu->pixelnours, renderer, "DEF", 3, PU_PROT_X, PU_PROT_Y);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", comp_s.protection[DMG_TYPE_PHYSICAL],
                  comp_s.protection[DMG_TYPE_MAGICAL]);
    PixelFont_Write(pu->pixelnours, renderer, numbuff, 5, PU_PROT_X_STAT1, PU_PROT_Y_STAT1);
    PixelFont_Write(pu->pixelnours, renderer, "HIT", 3, PU_HIT_X, PU_HIT_Y);
    stbsp_sprintf(numbuff, "%03d/%02d\0\0\0\0", comp_s.hit, comp_s.dodge);
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_HIT_X_STAT, PU_HIT_Y_STAT);
    PixelFont_Write(pu->pixelnours, renderer, "CRIT", 4, PU_CRIT_X, PU_CRIT_Y);
    stbsp_sprintf(numbuff, "%02d/%02d\0\0\0\0", comp_s.crit, comp_s.favor);
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_CRIT_X_STAT, PU_CRIT_Y_STAT);
    PixelFont_Write(pu->pixelnours, renderer, "SPEED", 5, PU_SPEED_X, PU_SPEED_Y);
    stbsp_sprintf(numbuff, "%2d\0\0\0\0", comp_s.speed);
    PixelFont_Write(pu->pixelnours, renderer, numbuff, strlen(numbuff), PU_SPEED_X_STAT,
                    PU_SPEED_Y_STAT);
    /* -- Finish -- */
    SDL_SetRenderTarget(renderer, render_target);
    Utilities_DrawColor_Reset(renderer);

}
