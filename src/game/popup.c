
#include "game/popup.h"
#include "popup/popup.h"
#include "popup/loadout_stats.h"
#include "popup/unit.h"
#include "popup/tile.h"
#include "popup/map_combat.h"
#include "map/map.h"
#include "debug.h"
#include "slider.h"
#include "position.h"
#include "utilities.h"
#include "macros.h"
#include "popup/pre_combat.h"
#include "structs.h"

/* --- Constructors/Destructors --- */
void Game_PopUps_Free(struct Game *sota) {
    // /* - Popup Loadout Stats - */
    // int popup_id = POPUP_TYPE_HUD_LOADOUT_STATS;
    // struct PopUp * popup
    // popup = IES_GET_COMPONENT(sota->world, sota->popups[popup_id], PopUp);
    // SDL_assert(popup != NULL);
    // SDL_free(popup->data);
    // tnecs_entity_destroy(sota->world, sota->popups[popup_id]);
    // sota->popups[popup_id] = TNECS_NULL;

    // /* - Popup Tile - */
    // int popup_id = POPUP_TYPE_HUD_LOADOUT_STATS;
    // struct PopUp * popup = IES_GET_COMPONENT(sota->world,
    //                            sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS],
    //                            PopUp);
    // SDL_assert(popup != NULL);
    // SDL_free(popup->data);
    // tnecs_entity_destroy(sota->world, sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS]);
    // sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] = TNECS_NULL;

}

/* --- POPUP_LOADOUT_STATS --- */
void Game_PopUp_Loadout_Stats_Create(struct Game *sota) {
    /* -- Preliminaries -- */
    if (sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] != TNECS_NULL) {
        SDL_Log("Popup POPUP_TYPE_HUD_LOADOUT_STATS is already loaded");
        return;
    }

    tnecs_entity popup_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position_ID, PopUp_ID);
    sota->popups[POPUP_TYPE_HUD_LOADOUT_STATS] = popup_ent;
    struct Position *position = IES_GET_COMPONENT(sota->world, popup_ent, Position);
    SDL_assert(position != NULL);
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, popup_ent, PopUp);

    /* -- Popup -- */
    *popup              = PopUp_default;
    popup->type         = POPUP_TYPE_HUD_LOADOUT_STATS;
    popup->draw         = &PopUp_Loadout_Stats_Draw;

    popup->visible      = true;

    /* - Popup_Loadout_Stats - */
    popup->data = PopUp_Loadout_Stats_Alloc();
    struct PopUp_Loadout_Stats *pls = popup->data;
    SDL_assert(sota->pixelnours != NULL);
    pls->pixelnours         = sota->pixelnours;
    SDL_assert(sota->pixelnours_big != NULL);
    pls->pixelnours_big     = sota->pixelnours_big;
    pls->tophand_stronghand = true;

    /* setting popup_loadout_stats position */
    position->pixel_pos.x   = sota->settings.res.x / 3;
    position->pixel_pos.y   = sota->settings.res.y / 10;

    PopUp_Loadout_Stats_Load(pls, sota->renderer, sota->world, &popup->n9patch);

}
void Game_PopUp_Loadout_Stats_Hide(struct Game *sota) {
    int popup_ind = POPUP_TYPE_HUD_LOADOUT_STATS;
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, sota->popups[popup_ind], PopUp);
    popup->visible = false;
}

/* --- POPUP_PRE_COMBAT --- */
void Game_PopUp_Pre_Combat_Create(struct Game *sota) {
    if (sota->pre_combat_popup == TNECS_NULL)
        sota->pre_combat_popup = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Menu_ID);
    else {
        return;
    }

    tnecs_entity ent;
    ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position_ID, PopUp_ID);
    sota->popups[POPUP_TYPE_PRE_COMBAT] = ent;

    struct PopUp    *popup       = IES_GET_COMPONENT(sota->world, ent, PopUp);
    SDL_assert(popup            != NULL);
    struct Position *position    = IES_GET_COMPONENT(sota->world, ent, Position);
    SDL_assert(position            != NULL);

    /* -- Popup -- */
    *popup              = PopUp_default;
    popup->type         = POPUP_TYPE_PRE_COMBAT;
    popup->draw         = &PreCombatPopup_Draw;

    popup->visible      = true;

    /* - PreCombatPopup - */
    popup->data = SDL_malloc(sizeof(struct PreCombatPopup));
    struct PreCombatPopup *pcp = popup->data;
    *pcp = PreCombatPopup_default;
    SDL_assert(sota->pixelnours != NULL);
    pcp->pixelnours         = sota->pixelnours;
    SDL_assert(sota->pixelnours_big != NULL);
    pcp->pixelnours_big     = sota->pixelnours_big;
    pcp->update = true;

    /* setting popup_loadout_stats position */
    position->pixel_pos.x      = sota->settings.res.x / 3;
    position->pixel_pos.y      = sota->settings.res.y / 3;

}

void Game_PopUp_Pre_Combat_Enable(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_PRE_COMBAT] == TNECS_NULL)
        Game_PopUp_Pre_Combat_Create(sota);
    tnecs_entity ent = sota->popups[POPUP_TYPE_PRE_COMBAT];
    SDL_assert(ent > TNECS_NULL);

    struct PopUp *popup = IES_GET_COMPONENT(sota->world, ent, PopUp);

    struct PreCombatPopup *pcp = (struct PreCombatPopup *)popup->data;

    PreCombatPopup_Set(pcp, sota);
    SDL_assert(sota->aggressor > TNECS_NULL);
    SDL_assert(sota->defendant > TNECS_NULL);
    PreCombatPopup_Load(pcp, sota->aggressor, sota->defendant, sota->renderer, &popup->n9patch);
    popup->visible = true;
}

void Game_PopUp_Pre_Combat_Hide(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_PRE_COMBAT] == TNECS_NULL)
        return;

    tnecs_entity ent = sota->popups[POPUP_TYPE_PRE_COMBAT];
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, ent, PopUp);
    popup->visible = false;
}

void Game_PopUp_Pre_Combat_Free(struct Game *sota) {

}

/* --- POPUP_UNIT --- */
void Game_PopUp_Unit_Free(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_HUD_UNIT] > TNECS_NULL) {
        tnecs_entity         popup_ent      = sota->popups[POPUP_TYPE_HUD_UNIT];
        struct PopUp        *popup          = IES_GET_COMPONENT(sota->world, popup_ent, PopUp);
        struct PopUp_Unit   *pu             = popup->data;

        PopUp_Unit_Free(pu);
        SDL_free(popup->data);
        tnecs_entity_destroy(sota->world, popup_ent);
        sota->popups[POPUP_TYPE_HUD_UNIT]   = TNECS_NULL;
    }
}

void Game_PopUp_Unit_Create(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_HUD_UNIT] != TNECS_NULL) {
        if (sota->hovered_unit_entity != TNECS_NULL) {
            tnecs_entity ent = sota->popups[POPUP_TYPE_HUD_UNIT];
            struct PopUp *popup = IES_GET_COMPONENT(sota->world, ent, PopUp);
            struct PopUp_Unit *popup_unit = popup->data;
            PopUp_Unit_Set(popup_unit, sota);
            SDL_assert(popup_unit->unit != NULL);
        }
        return;
    }

    tnecs_entity ent;
    ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position_ID, PopUp_ID, Slider_ID, SliderOffscreen_ID);
    sota->popups[POPUP_TYPE_HUD_UNIT] = ent;
    struct PopUp           *popup     = IES_GET_COMPONENT(sota->world, ent, PopUp);
    struct Slider          *slider    = IES_GET_COMPONENT(sota->world, ent, Slider);
    struct Position        *position  = IES_GET_COMPONENT(sota->world, ent, Position);
    struct SliderOffscreen *offscreen = IES_GET_COMPONENT(sota->world, ent, SliderOffscreen);
    SDL_assert(popup     != NULL);
    SDL_assert(slider    != NULL);
    SDL_assert(position  != NULL);
    SDL_assert(offscreen != NULL);

    /* - Popup - */
    *popup = PopUp_default;
    popup->type         = POPUP_TYPE_HUD_UNIT;
    popup->draw         = &PopUp_Unit_Draw;
    popup->visible = true;

    /* - n9Patch - */
    if (popup->data == NULL)
        popup->data = SDL_malloc(sizeof(struct PopUp_Unit));
    struct PopUp_Unit *popup_unit = popup->data;
    *popup_unit = PopUp_Unit_default;
    PopUp_Unit_Load(popup_unit, sota->renderer, &popup->n9patch);

    /* - Slider - */
    SDL_assert(popup_unit->unit == NULL);
    *slider = Slider_default;
    slider->slidetype = SLIDETYPE_EASYINEASYOUT;

    *offscreen = SliderOffscreen_default;
    offscreen->reverse = false;
    offscreen->settings = &sota->settings;
    SDL_assert(sota->pixelnours != NULL);

    /* - Popup Pixelfonts - */
    popup_unit->pixelnours      = sota->pixelnours;
    popup_unit->pixelnours_big  = sota->pixelnours_big;

    /* - Popup position - */
    PopUp_Unit_Offset(popup_unit, &sota->settings);
    PopUp_Unit_Limits(popup, popup_unit, &sota->settings);
    position->pixel_pos.x = SOTA_TILEMAP2PIXEL(position->tilemap_pos.x,
                                               sota->map->tilesize[0], sota->camera.offset.x, sota->camera.zoom);
    position->pixel_pos.y = SOTA_TILEMAP2PIXEL(position->tilemap_pos.x,
                                               sota->map->tilesize[1], sota->camera.offset.y, sota->camera.zoom);

    SDL_assert(popup_unit->unit == NULL);
    position->pixel_pos = PopUp_Unit_Position(popup, popup_unit, &popup->n9patch,
                                              &sota->settings, &position->pixel_pos);

    /* - Setting popup - */
    if (sota->hovered_unit_entity != TNECS_NULL) {
        PopUp_Unit_Set(popup_unit, sota);
        SDL_assert(popup_unit->unit != NULL);
    }
}

void Game_PopUp_Unit_Hide(struct Game *sota) {
    SDL_assert(sota->popups[POPUP_TYPE_HUD_UNIT] != TNECS_NULL);
    struct PopUp *popup;
    popup = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_UNIT], PopUp);
    popup->visible = false;
}

void Game_PopUp_Unit_Place(struct Game *sota, struct Point cursor_pos) {
    /* - Does Popup entity exist - */
    if (sota->popups[POPUP_TYPE_HUD_UNIT] == TNECS_NULL) {
        return;
    }

    /* - Is popup is visible - */
    struct PopUp            *popup;
    struct Position         *position;
    struct Slider           *slider;
    struct SliderOffscreen  *offscreen;
    popup     = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_UNIT], PopUp);
    position  = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_UNIT], Position);
    slider    = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_UNIT], Slider);
    offscreen = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_UNIT], SliderOffscreen);
    SDL_assert(popup);

    if (!popup->visible) {
        return;
    }

    /* - Has popup target position changed - */
    struct PopUp_Unit *popup_unit = popup->data;
    SDL_assert(popup_unit != NULL);
    struct Point pixel_pos;
    pixel_pos.x = SOTA_TILEMAP2PIXEL(cursor_pos.x, sota->map->tilesize[0],
                                     sota->camera.offset.x, sota->camera.zoom);
    pixel_pos.y = SOTA_TILEMAP2PIXEL(cursor_pos.y, sota->map->tilesize[1],
                                     sota->camera.offset.y, sota->camera.zoom);
    struct Point new_target;
    new_target = PopUp_Unit_Position(popup, popup_unit, &popup->n9patch, &sota->settings, &pixel_pos);

    b32 target_changed = (new_target.x != slider->target.x) || (new_target.y != slider->target.y);

    if (!target_changed) {
        return;
    }

    /* - Update the slider target - */
    struct Point popup_pos = position->pixel_pos;
    slider->target = new_target;

    SDL_Rect rect = Utilities_Rect(position, &popup->n9patch);
    rect.x = new_target.x;
    rect.y = new_target.y;

    Slider_Target_Offscreen_Far(slider, offscreen, &rect);
    Slider_Init(slider, &position->pixel_pos, &slider->target);
    Slider_Offscreen_Midpoint(slider, offscreen);

    /* - If no unit is hovered -> teleport to the other side - */
    if (sota->hovered_unit_entity == TNECS_NULL) {
        position->pixel_pos.x   = offscreen->target.x;
        position->pixel_pos.y   = offscreen->target.y;
        slider->target.x        = offscreen->target.x;
        slider->target.y        = offscreen->target.y;
        offscreen->reverse = false;
    }

    // #ifdef DEBUG_POPUP_UNIT_OFFSCREEN
    //     Slider_Init(slider, &position->pixel_pos, &offscreen->target);
    // #else
    //     Slider_Init(slider, &position->pixel_pos, &slider->target);
    // #endif
}

void Game_PopUp_Tile_Place(struct Game *sota, struct Point cursor_pos) {
    /* - Does Popup entity exist - */
    tnecs_entity ent = sota->popups[POPUP_TYPE_HUD_TILE];
    if (ent == TNECS_NULL) {
        return;
    }

    /* - Is popup is visible - */
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, ent, PopUp);
    SDL_assert(popup);
    if (!popup->visible) {
        return;
    }

    /* - Has popup target position changed - */
    struct Slider *slider               = IES_GET_COMPONENT(sota->world, ent, Slider);
    struct Position *position           = IES_GET_COMPONENT(sota->world, ent, Position);
    struct SliderOffscreen *offscreen   = IES_GET_COMPONENT(sota->world, ent, SliderOffscreen);
    struct PopUp_Tile *popup_tile       = popup->data;
    SDL_assert(popup_tile != NULL);
    struct Point pixel_pos;
    pixel_pos.x = SOTA_TILEMAP2PIXEL(cursor_pos.x, sota->map->tilesize[0],
                                     sota->camera.offset.x, sota->camera.zoom);
    pixel_pos.y = SOTA_TILEMAP2PIXEL(cursor_pos.y, sota->map->tilesize[1],
                                     sota->camera.offset.y, sota->camera.zoom);
    struct Point new_target;
    new_target = PopUp_Tile_Position(popup, popup_tile, &popup->n9patch, &sota->settings,
                                     &pixel_pos, sota->moved_direction);

    b32 target_changed = (new_target.x != slider->target.x) || (new_target.y != slider->target.y);
    if (!target_changed) {
        return;
    }

    /* - Update the slider target - */
    slider->target = new_target;
    // #ifdef DEBUG_POPUP_TILE_OFFSCREEN
    SDL_Rect rect = Utilities_Rect(position, &popup->n9patch);

    Slider_Target_Offscreen_Far(slider, offscreen, &rect);
    Slider_Init(slider, &position->pixel_pos, &slider->target);
    Slider_Offscreen_Midpoint(slider, offscreen);
    // #else
    //     Slider_Init(slider, &position->pixel_pos, &slider->target);
    // #endif
}

/* --- PopUp_Map_Combat --- */
void Game_PopUp_Map_Combat_Create(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_MAP_COMBAT] > TNECS_NULL) {
        SDL_Log("Popup POPUP_TYPE_MAP_COMBAT is already loaded");
        return;
    }

    tnecs_entity ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position_ID, PopUp_ID);
    sota->popups[POPUP_TYPE_MAP_COMBAT] = ent;
    struct PopUp    *popup      = IES_GET_COMPONENT(sota->world, ent, PopUp);
    popup->draw                 = &PopUp_Map_Combat_Draw;
    SDL_assert(popup    != NULL);

    /* - Making PopUp_Map_Combat - */
    if (popup->data != NULL)
        SDL_free(popup->data);
    popup->data = SDL_malloc(sizeof(struct PopUp_Map_Combat));
}

void Game_PopUp_Map_Combat_Update(   struct Game *sota) {
    tnecs_entity ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct Position *position   = IES_GET_COMPONENT(sota->world, ent, Position);
    struct PopUp    *popup      = IES_GET_COMPONENT(sota->world, ent, PopUp);
    SDL_assert(position != NULL);
    SDL_assert(popup    != NULL);

    popup->visible      = true;

    struct PopUp_Map_Combat *pmc = popup->data;
    *pmc = PopUp_Map_Combat_default;

    struct Unit *aggressor = IES_GET_COMPONENT(sota->world, sota->aggressor, Unit);
    struct Unit *defendant = IES_GET_COMPONENT(sota->world, sota->defendant, Unit);

    struct Position *agg_posc = IES_GET_COMPONENT(sota->world, sota->aggressor, Position);
    struct Position *dft_posc = IES_GET_COMPONENT(sota->world, sota->defendant, Position);
    struct Point *agg_pos = &agg_posc->tilemap_pos;
    struct Point *dft_pos = &dft_posc->tilemap_pos;

    PopUp_Map_Combat_Units(pmc, sota, sota->aggressor, sota->defendant);

    PopUp_Map_Combat_Load(pmc, sota->renderer, &popup->n9patch);

    SDL_assert(sota->pixelnours != NULL);
    pmc->pixelnours_big     = sota->pixelnours_big;
    pmc->pixelnours_tight   = sota->pixelnours_tight;

    /* - position - */
    // TODO: dynamically set position on the corners like popup_tile
    position->pixel_pos.x   = 1;
    position->pixel_pos.y   = 1;
}

void Game_PopUp_Map_Combat_Hide(struct Game *sota) {
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    if (popup_ent == TNECS_NULL)
        return;
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, popup_ent, PopUp);
    popup->visible = false;
}

void Game_PopUp_Map_Combat_Free(struct Game *sota) {
    tnecs_entity popup_ent = sota->popups[POPUP_TYPE_MAP_COMBAT];
    struct PopUp *popup = IES_GET_COMPONENT(sota->world, popup_ent, PopUp);
    struct PopUp_Map_Combat *pmc = popup->data;
    PopUp_Map_Combat_Free(pmc);
    tnecs_entity_destroy(sota->world, popup_ent);
    sota->popups[POPUP_TYPE_MAP_COMBAT] = TNECS_NULL;
}

/* --- POPUP_TILE --- */
// void Game_PopUp_Tile_Set(struct Game *sota) {
// //     struct Position *cursor_position;
//     cursor_position = IES_GET_COMPONENT(sota->world, sota->entity_cursor, Position);

//     /* -- Get popup stuff -- */
//     tnecs_entity popup_ent =  sota->popups[POPUP_TYPE_HUD_TILE];
//     SDL_assert(popup_ent > TNECS_NULL);
//     struct PopUp *popup = IES_GET_COMPONENT(sota->world, popup_ent, PopUp);
//     SDL_assert(popup      != NULL);
//     struct PopUp_Tile *popup_tile = popup->data;
//     SDL_assert(popup_tile != NULL);

//     /* -- Give tile to popup_tile -- */
//     int i = cursor_position->tilemap_pos.y * sota->map->col_len + cursor_position->tilemap_pos.x;
//     i32 tile_ind = sota->map->tilemap[i] / TILE_DIVISOR;
//     SDL_assert(tile_ind > 0);
//     i32 tile_order = Map_Tile_Order(sota->map, tile_ind);
//     popup_tile->tile = &sota->map->tiles[tile_order];
//     popup_tile->update = true;
// // }

void Game_PopUp_Tile_Destroy(struct Game *sota) {

}

void Game_PopUp_Tile_Hide(struct Game *sota) {
    SDL_assert(sota->popups[POPUP_TYPE_HUD_TILE] != TNECS_NULL);
    struct PopUp *popup;
    popup = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_TILE], PopUp);
    popup->visible = false;
}

void Game_PopUp_Tile_Free(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_HUD_TILE] > TNECS_NULL) {
        struct PopUp *popup;
        popup = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_TILE], PopUp);
        SDL_free(popup->data);
    }
    sota->popups[POPUP_TYPE_HUD_TILE] = TNECS_NULL;
}

void Game_PopUp_Tile_Create(struct Game *sota) {
    if (sota->popups[POPUP_TYPE_HUD_TILE] != TNECS_NULL) {
        SDL_Log("Popup POPUP_TYPE_HUD_TILE is already loaded");
        return;
    }
    tnecs_entity ent;
    ent = TNECS_ENTITY_CREATE_wCOMPONENTS(sota->world, Position_ID, PopUp_ID, Slider_ID, SliderOffscreen_ID);
    sota->popups[POPUP_TYPE_HUD_TILE] = ent;

    struct PopUp           *popup       = IES_GET_COMPONENT(sota->world, ent, PopUp);
    struct Slider          *slider      = IES_GET_COMPONENT(sota->world, ent, Slider);
    struct Position        *position    = IES_GET_COMPONENT(sota->world, ent, Position);
    struct SliderOffscreen *offscreen   = IES_GET_COMPONENT(sota->world, ent, SliderOffscreen);
    SDL_assert(popup     != NULL);
    SDL_assert(slider    != NULL);
    SDL_assert(position  != NULL);
    SDL_assert(offscreen != NULL);
    /* - DEBUG: Making Hover - */
    // struct Hover * hover = IES_GET_COMPONENT(sota->world, sota->popups[POPUP_TYPE_HUD_TILE], Hover);
    // SDL_assert(hover != NULL);
    // *hover = Hover_default;
    /* - Making Popup - */
    *popup = PopUp_default;
    popup->type = POPUP_TYPE_HUD_TILE;
    popup->visible      = true;
    popup->draw         = &PopUp_Tile_Draw;

    /* - Popup - */
    popup->data = SDL_malloc(sizeof(struct PopUp_Tile));
    struct PopUp_Tile *popup_tile = popup->data;
    *popup_tile = PopUp_Tile_default;
    PopUp_Tile_Load(popup_tile, sota->renderer, &popup->n9patch);

    *offscreen = SliderOffscreen_default;
    SDL_assert(sota->pixelnours != NULL);
    popup_tile->pixelnours = sota->pixelnours;
    popup_tile->pixelnours_big = sota->pixelnours_big;

    /* - Making Popup_Tile - */
    *slider = Slider_default;
    slider->slidetype = SLIDETYPE_EASYINEASYOUT;

    offscreen->settings     = &sota->settings;

    PopUp_Tile_Set(popup_tile, sota);
    PopUp_Tile_Offset(popup_tile, &sota->settings);
    PopUp_Tile_Limits(popup, popup_tile, &sota->settings);
    // struct Point pixel_pos;
    position->pixel_pos.x = SOTA_TILEMAP2PIXEL(position->tilemap_pos.x,
                                               sota->map->tilesize[0], sota->camera.offset.x, sota->camera.zoom);
    position->pixel_pos.y = SOTA_TILEMAP2PIXEL(position->tilemap_pos.x,
                                               sota->map->tilesize[1], sota->camera.offset.y, sota->camera.zoom);
    position->pixel_pos = PopUp_Tile_Position(popup, popup_tile,
                                              &popup->n9patch,
                                              &sota->settings, &position->pixel_pos, sota->moved_direction);
    slider->target = position->pixel_pos;
    SDL_assert(popup->n9patch.pos.x == 0);
    SDL_assert(popup->n9patch.pos.y == 0);

}
