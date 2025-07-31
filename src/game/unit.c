/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Top level unit handling
**  - Ex: Putting PC unit entities on map
*/

#include "game/unit.h"
#include "game/game.h"
#include "game/map.h"
#include "map/ontile.h"
#include "unit/equipment.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/range.h"
#include "unit/loadout.h"
#include "nmath.h"
#include "jsonio.h"
#include "map/map.h"
#include "bars/map_hp.h"
#include "weapon.h"
#include "filesystem.h"
#include "position.h"
#include "names.h"
#include "sprite.h"
#include "globals.h"

/* --- Constructors/Destructors --- */
void Game_Items_Free(struct dtab **items_dtab) {
    if (*items_dtab == NULL) {
        return;
    }

    struct Item *cast_dtab = (*items_dtab)->values;
    SDL_assert(cast_dtab != NULL);
    for (size_t i = 0 ; i < (*items_dtab)->num; i++) {
        Item_Free((cast_dtab + i));
    }

    DTAB_FREE((*items_dtab));
    *items_dtab = NULL;
}

void Game_Weapons_Free(struct dtab **weapons_dtab) {
    if ((*weapons_dtab) == NULL) {
        return;
    }

    struct Weapon *cast_dtab = (*weapons_dtab)->values;
    SDL_assert(cast_dtab != NULL);
    for (size_t i = 0; i < (*weapons_dtab)->num; i++) {
        Weapon_Free((cast_dtab + i));
    }

    DTAB_FREE((*weapons_dtab));
    *weapons_dtab = NULL;
}

/* --- Wait/Refresh --- */
void Game_Unit_Wait(struct Game *sota, tnecs_entity ent) {
    struct Unit *unit = IES_GET_COMPONENT(gl_world, ent, Unit);
    SDL_assert(unit != NULL);
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, ent, Sprite);
#ifndef DEBUG_UNIT_MOVEAFTERWAIT
    /* unit waits */
    Unit_wait(unit);
    SDL_assert(Unit_isWaiting(unit));
    /* set palette to dark */
    Sprite_Darken(sprite, sota->render.er);
#endif /*DEBUG_UNIT_MOVEAFTERWAIT*/

    /* stop animation */
    Sprite_Animation_Restart(sprite, MAP_UNIT_LOOP_IDLE);
    SDL_assert(IES_ENTITY_HASCOMPONENT(gl_world, ent, Timer));
    struct Timer *timer = IES_GET_COMPONENT(gl_world, ent, Timer);
    timer->paused = true;
    // if (IES_ENTITY_HASCOMPONENT(gl_world, ent, Timer))

    // TNECS_REMOVE_COMPONENTS(gl_world, ent, Timer);
    // Sprite_Draw(sprite, sota->render.er);
}

void Game_Unit_Refresh(struct Game *sota, tnecs_entity ent) {
    SDL_assert(sota != NULL);
    if (TNECS_NULL == ent) {
        return;
    }
    /* --- Refresh unit on map --- */
    SDL_assert(TNECS_ENTITY_EXISTS(gl_world, ent));
    struct Unit *unit = IES_GET_COMPONENT(gl_world, ent, Unit);
    SDL_assert(unit != NULL);
    /* --- Skip if unit is not waiting --- */
    if (!Unit_isWaiting(unit))
        return;
    struct Sprite *sprite = IES_GET_COMPONENT(gl_world, ent, Sprite);
    SDL_assert(sprite != NULL);

    Unit_refresh(unit);
    SDL_assert(!Unit_isWaiting(unit));
    /* -- set palette back to nes --  */
    Sprite_Unveil(sprite, sota->render.er);

    /* restart animation */
    Sprite_Animation_Restart(sprite, MAP_UNIT_LOOP_IDLE);
    // if (!IES_ENTITY_HASCOMPONENT(gl_world, ent, Timer))
    // TNECS_ADD_COMPONENT(gl_world, ent, Timer);
    struct Timer *timer = IES_GET_COMPONENT(gl_world, ent, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;
    Sprite_Draw(sprite, sota->render.er);
}

/* --- Party utilities --- */
void Game_Loaded_Units_Free(struct Game *sota) {
    /* -- SDL_free entities in units_loaded array -- */
    // TODO
}

void Game_Party_Free(struct Game *sota) {
    /* -- SDL_free unit struct read from JSON files in party array -- */
    tnecs_entity    *entities   = sota->party.entities;
    // SDL_assert(sota->party.entities[unit_ids[i]] >= TNECS_NULL);

    for (size_t j = UNIT_ID_START + 1; j < UNIT_ID_END; j++) {
        // Skip if party unit was never read
        // -| Party unit did not become component for entity in units_loaded
        tnecs_entity ent = entities[j];
        if (ent <= TNECS_NULL) {
            return;
        }
        // SDL_free loaded unit component from and clear party entry
        Unit *unit = IES_GET_COMPONENT(gl_world, ent, Unit);
        SDL_assert(unit != NULL);
        SDL_assert(j == unit->id.self);

        Sprite *sprite = IES_GET_COMPONENT(gl_world, ent, Sprite);

        SDL_assert(sprite != NULL);

        // SDL_Log("sprite->texture %p", sprite->texture);
        // SDL_Log("unit->statuses.queue %p", unit->statuses.queue);
        // // SDL_assert(sprite->texture != unit->statuses.queue);

        // Sprite_Free(sprite);
        // SDL_Log("sprite->texture %p", sprite->texture);
        // SDL_Log("unit->statuses.queue %p", unit->statuses.queue);
        // SDL_assert(sprite->texture != unit->statuses.queue);

        tnecs_entity_destroy(gl_world, ent);
        entities[j] = TNECS_NULL;
    }
}

tnecs_entity Game_Party_Entity_Create(struct Game *sota) {
    /* Pre-json read unit entity creation */
    tnecs_world *world = gl_world;

    tnecs_component archetype = TNECS_COMPONENT_IDS2ARCHETYPE(Unit_ID,
                                                              Position_ID,
                                                              Sprite_ID,
                                                              Timer_ID,
                                                              MapHPBar_ID);
    tnecs_entity unit_ent;
    unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(world,
                                               Unit_ID,
                                               Position_ID,
                                               Sprite_ID,
                                               Timer_ID,
                                               MapHPBar_ID);

    // SDL_Log("-- loading unit --");
    struct Unit *unit = IES_GET_COMPONENT(world, unit_ent, Unit);
    SDL_assert(unit != NULL);
    SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);
    SDL_assert((Unit_Handedness(unit) > UNIT_HAND_NULL) && (Unit_Handedness(unit) < UNIT_HAND_END));

    // Unit_id_set(unit, unit_id);
    SDL_assert(Unit_Movement(unit) > UNIT_MVT_START);

    /* Equipping equipment in hand if any */
    canEquip can_equip  = canEquip_default;
    can_equip.hand      = UNIT_HAND_LEFT;
    canEquip_Eq(&can_equip, UNIT_HAND_LEFT);

    if (Unit_canEquip(unit, can_equip)) {
        Unit_Equip(unit, UNIT_HAND_LEFT, UNIT_HAND_LEFT);
    }

    can_equip.hand      = UNIT_HAND_RIGHT;
    canEquip_Eq(&can_equip, UNIT_HAND_RIGHT);
    if (Unit_canEquip(unit, can_equip)) {
        Unit_Equip(unit, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    }

    if ((Unit_Class(unit) == UNIT_CLASS_VESTAL) || (Unit_Class(unit) == UNIT_CLASS_PRIEST)) {
        Unit_Rangemap_set(unit, RANGEMAP_HEALMAP);
    }

    // SDL_Log("-- loading slider --");
    // struct Slider * slider = IES_GET_COMPONENT(world, unit_ent, Slider);
    // SDL_assert(slider != NULL);
    // *slider = Slider_default;
    // slider->slidefactors[DIMENSION_X] = 2.0f;
    // slider->slidefactors[DIMENSION_Y] = 2.0f;
    // slider->update_wait = CURSOR_SLIDEWAIT;
    // slider->slidetype = SLIDETYPE_GEOMETRIC;

    // SDL_Log("-- loading map_hp_bar --");
    struct MapHPBar *map_hp_bar = IES_GET_COMPONENT(world, unit_ent, MapHPBar);
    *map_hp_bar             = MapHPBar_default;
    map_hp_bar->unit_ent    = unit_ent;
    map_hp_bar->len         = sota->settings.tilesize[0];
    map_hp_bar->update      = false;
    map_hp_bar->visible     = false;

    // SDL_Log("-- loading position --");
    struct Position *pos;
    pos = IES_GET_COMPONENT(world, unit_ent, Position);
    SDL_assert(pos != NULL);
    memcpy(pos, &Position_default, sizeof(Position_default));
    pos->onTilemap = true;

    Map *map = Game_Map(sota);
    if (map != NULL) {
        Position_Bounds_Set(pos, 0, Map_col_len(map), 0, Map_row_len(map));
    }
    pos->scale[0]       = (float)sota->settings.tilesize[0];
    pos->scale[1]       = (float)sota->settings.tilesize[1];
    pos->pixel_pos.x    = (i32)lround(pos->tilemap_pos.x * pos->scale[0]);
    pos->pixel_pos.y    = (i32)lround(pos->tilemap_pos.y * pos->scale[1]);

    // SDL_Log("-- loading sprite --");
    struct Timer *timer;
    timer = IES_GET_COMPONENT(world, unit_ent, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    struct Sprite *sprite = IES_GET_COMPONENT(world, unit_ent, Sprite);
    SDL_assert(sprite != NULL);
    *sprite = Sprite_default;
    Sprite_Map_Unit_Load(sprite, unit, sota->render.er);
    sprite->visible = false;
    sprite->flip = SDL_FLIP_HORIZONTAL;

    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);
    SDL_assert(sprite->spritesheet->frames != NULL);

    Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
    Sprite_Rects_Init(sprite);
    Sprite_defaultShaders_Load(sprite);

    // SDL_Log("-- checks --");
    SDL_assert(TNECS_ENTITY_EXISTS(world, unit_ent));

    return (unit_ent);
}

void Game_Party_Load(struct Game *sota) {
    SDL_assert(sota != NULL);
    s8 *filenames = sota->party.json_filenames;
    SDL_assert(filenames != NULL);
    SDL_assert(DARR_NUM(filenames) > 0);

    size_t load_num = DARR_NUM(filenames);

    for (size_t i = 0; i < load_num; i++) {
        /* --- Entity creation --- */
        tnecs_entity unit_ent = Game_Party_Entity_Create(sota);
        Unit    *unit   = IES_GET_COMPONENT(gl_world, unit_ent, Unit);
        Sprite  *sprite = IES_GET_COMPONENT(gl_world, unit_ent, Sprite);
        SDL_assert(unit     != NULL);
        SDL_assert(sprite   != NULL);
        SDL_assert(unit->jsonio_header.json_filename.data == NULL);
        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);

        /* --- Unit json reading putting in array --- */
        /* Reading party unit json */
        /* Putting unit in entities list */
        s8 filename = filenames[i];
        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);
        Game_Party_Entity_Init(sota, unit_ent, filename);
        sota->party.entities[Unit_id(unit)] = unit_ent;
    }
}


void Game_Party_Entity_Init(Game *sota,
                            tnecs_entity ent,
                            s8 filename) {
    // Post-json read unit entity init
    Unit *unit = IES_GET_COMPONENT(gl_world, ent, Unit);

    /* --- Reading party unit json --- */
    if (filename.data != NULL) {
        Sprite  *sprite = IES_GET_COMPONENT(gl_world, ent, Sprite);
        jsonio_readJSON(filename, unit);
        SDL_assert(Unit_Name(unit).data != NULL);

        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);
        SDL_assert(unit->flags.handedness > UNIT_HAND_NULL);
        SDL_assert(unit->flags.handedness < UNIT_HAND_END);
        SDL_assert(unit->flags.mvt_type > UNIT_MVT_START);

        SDL_assert(DARR_NUM(unit->stats.bonus_stack) == 0);

        /* --- Load sprite --- */
        Sprite_Map_Unit_Load(sprite, unit, sota->render.er);
    }
    unit->id.army = ARMY_FRIENDLY;
    i16 id = Unit_id(unit);
    SDL_assert(id > UNIT_ID_PC_START);
    SDL_assert(id < UNIT_ID_PC_END);
    SDL_assert(_Unit_Name_id(id).data != NULL);

    /* --- Putting entity in party --- */
    if (sota->party.entities[id] != TNECS_NULL) {
        /* TODO: all components should have free functions */
        tnecs_entity_destroy(gl_world, sota->party.entities[id]);
    }

    sota->party.entities[id] = ent;
    SDL_assert(sota->party.entities[id] != TNECS_NULL);
}

/* --- Unitmap --- */
void Game_putPConMap(struct Game    *sota,   i16    *unit_ids,
                     struct Point   *posarr, size_t  load_num) {
    Map *map = Game_Map(sota);
    SDL_assert(map      != NULL);
    SDL_assert(posarr   != NULL);
    SDL_assert(load_num > 0);
    for (i16 i = 0; i < load_num; i++) {
        SDL_assert(Unit_ID_Valid(unit_ids[i]));
        tnecs_entity unit_ent = sota->party.entities[unit_ids[i]];

        SDL_assert(unit_ent > TNECS_NULL);
        struct Unit *temp = IES_GET_COMPONENT(gl_world, unit_ent, Unit);
        SDL_assert(temp             != NULL);
        SDL_assert(Unit_Name(temp).data != NULL);

        SDL_assert(map->world == gl_world);
        Map_Unit_Put(map, posarr[i].x, posarr[i].y, unit_ent);

        struct Position *pos = IES_GET_COMPONENT(gl_world, unit_ent, Position);
        SDL_assert(pos->tilemap_pos.x == posarr[i].x);
        SDL_assert(pos->tilemap_pos.y == posarr[i].y);
    }
}

/* --- Utilities --- */
void Game_Weapons_Rewrite(struct Game *sota) {
    SDL_Log("Rewriting weapons with new data\n");
    Weapons_All_Load(gl_weapons_dtab);
    Weapons_All_Save(gl_weapons_dtab);
    getchar();
}
