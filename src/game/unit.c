
#include "game/unit.h"

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
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, ent, Unit);
    SDL_assert(unit != NULL);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, ent, Sprite);
#ifndef DEBUG_UNIT_MOVEAFTERWAIT
    /* unit waits */
    Unit_wait(unit);
    /* set palette to dark */
    Sprite_Darken(sprite, sota->renderer);
#endif /*DEBUG_UNIT_MOVEAFTERWAIT*/

    /* stop animation */
    Sprite_Animation_Restart(sprite, MAP_UNIT_LOOP_IDLE);
    if (TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Timer))
        TNECS_REMOVE_COMPONENTS(sota->world, ent, Timer);
    Sprite_Draw(sprite, sota->renderer);
}

void Game_Unit_Refresh(struct Game *sota, tnecs_entity ent) {
    /* --- Refresh unit on map --- */
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, ent, Unit);
    SDL_assert(unit != NULL);
    /* --- Skip if unit is not waiting --- */
    if (!unit->waits)
        return;

    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, ent, Sprite);
    SDL_assert(sprite != NULL);

    Unit_refresh(unit);
    /* -- set palette back to nes --  */
    Sprite_Unveil(sprite, sota->renderer);

    /* restart animation */
    Sprite_Animation_Restart(sprite, MAP_UNIT_LOOP_IDLE);
    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Timer))
        TNECS_ADD_COMPONENT(sota->world, ent, Timer);
    struct Timer *timer = TNECS_GET_COMPONENT(sota->world, ent, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;
    Sprite_Draw(sprite, sota->renderer);
}

/* --- Party utilities --- */
void Game_Party_Clear(struct Game *sota) {
    for (size_t i = 0; i < SOTA_MAX_PARTY_SIZE; i++)
        sota->party[i] = Unit_default;
}

void Game_Party_Unload(struct Game *sota, i16 *to_unload_ids, size_t unload_num) {
    for (size_t i = 0; i < unload_num; i++) {
        for (size_t j = 0; j < SOTA_MAX_PARTY_SIZE; j++) {
            if (sota->party[j]._id == to_unload_ids[i])
                sota->party[j] = Unit_default;
        }
    }
}

void Game_Party_Free(struct Unit *party) {
    for (size_t j = 0; j < SOTA_MAX_PARTY_SIZE; j++) {
        Unit_Free(&party[j]);
        party[j] = Unit_default;
    }
}

tnecs_entity Game_Party_Entity_Create(struct Game *sota, i16 unit_id,
                                      struct Point in_pos) {
    /* Create Unit entity from previously loaded party unit. */
    tnecs_world *world = sota->world;
    tnecs_entity unit_ent;
    if (sota->units_loaded[unit_id] != TNECS_NULL) {
        unit_ent = sota->units_loaded[unit_id];
        SDL_Log("Unit %d (%s, entity %d) is already loaded", unit_id,
                global_unitNames[unit_id].data, unit_ent);
    } else {
        SDL_Log("-- create entity for unit %ld --", unit_id);
        char filename[DEFAULT_BUFFER_SIZE];
        SDL_Log("-- create entity --");
        unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position,
                                                   Sprite, Timer, MapHPBar);
    }

    SDL_assert(TNECS_ENTITY_HASCOMPONENT(sota->world, unit_ent, Unit));
    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, unit_ent, MapHPBar)) {
        TNECS_ADD_COMPONENT(sota->world, unit_ent, MapHPBar);
    }

    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, unit_ent, Position)) {
        TNECS_ADD_COMPONENT(sota->world, unit_ent, Position);
    }

    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, unit_ent, Sprite)) {
        TNECS_ADD_COMPONENT(sota->world, unit_ent, Sprite);
    }

    if (!TNECS_ENTITY_HASCOMPONENT(sota->world, unit_ent, Timer)) {
        TNECS_ADD_COMPONENT(sota->world, unit_ent, Timer);
    }

    tnecs_component typeflag   = TNECS_COMPONENT_NAMES2TYPEFLAG(world, Unit, Position,
                                                                Sprite, Timer, MapHPBar);
    size_t typeflag_id1 = tnecs_typeflagid(world, typeflag);
    size_t typeflag_id2 = tnecs_typeflagid(world, world->entity_typeflags[unit_ent]);
    SDL_assert(world->entities[unit_ent] == unit_ent);
    SDL_assert(unit_ent > TNECS_NULL);
    SDL_assert(sota->world->entity_typeflags[unit_ent] == typeflag);

    SDL_Log("-- loading unit --");
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, unit_ent, Unit);
    SDL_assert(unit != NULL);
    SDL_assert(sota->party[unit_id]._id != 0);

    memcpy(unit, &sota->party[unit_id], sizeof(struct Unit));
    SDL_assert((sota->party[unit_id].handedness > UNIT_HAND_NULL)
               && (sota->party[unit_id].handedness < UNIT_HAND_END));
    SDL_assert((unit->handedness > UNIT_HAND_NULL) && (unit->handedness < UNIT_HAND_END));

    Unit_setid(unit, unit_id);
    SDL_assert(unit->name.data != NULL);

    unit->items_dtab   = sota->items_dtab;
    unit->weapons_dtab = sota->weapons_dtab;
    struct Inventory_item *item = Unit_Item_Strong(unit, UNIT_HAND_WEAK);
    if (item->id > ITEM_NULL)
        Unit_Equip_inHand(unit, UNIT_HAND_WEAK);
    item = Unit_Item_Strong(unit, UNIT_HAND_STRONG);
    if (item->id > ITEM_NULL)
        Unit_Equip_inHand(unit, UNIT_HAND_STRONG);

    // SDL_Log("-- loading slider --");
    // struct Slider * slider = TNECS_GET_COMPONENT(sota->world, unit_ent, Slider);
    // SDL_assert(slider != NULL);
    // *slider = Slider_default;
    // slider->slidefactors[DIMENSION_X] = 2.0f;
    // slider->slidefactors[DIMENSION_Y] = 2.0f;
    // slider->update_wait = CURSOR_SLIDEWAIT;
    // slider->slidetype = SLIDETYPE_GEOMETRIC;
    SDL_Log("-- loading map_hp_bar --");
    struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, unit_ent, MapHPBar);
    *map_hp_bar = MapHPBar_default;
    map_hp_bar->unit_ent = unit_ent;
    map_hp_bar->len = sota->settings.tilesize[0];

    SDL_Log("-- loading position --");
    struct Position *pos;
    pos = TNECS_GET_COMPONENT(sota->world, unit_ent, Position);
    SDL_assert(pos != NULL);
    memcpy(pos, &Position_default, sizeof(Position_default));
    pos->onTilemap = true;
    Map_Bounds_Compute(sota->map);
    Position_Bounds_Set(pos, sota->map->boundsmin.x, sota->map->boundsmax.x,
                        sota->map->boundsmin.y, sota->map->boundsmax.y);
    pos->scale[0] = (float)sota->settings.tilesize[0];
    pos->scale[1] = (float)sota->settings.tilesize[1];
    pos->tilemap_pos.x = in_pos.x;
    pos->tilemap_pos.y = in_pos.y;
    pos->pixel_pos.x = (i32)lround(pos->tilemap_pos.x * pos->scale[0]);
    pos->pixel_pos.y = (i32)lround(pos->tilemap_pos.y * pos->scale[1]);

    SDL_Log("-- loading sprite --");
    struct Timer *timer;
    timer = TNECS_GET_COMPONENT(sota->world, unit_ent, Timer);
    SDL_assert(timer != NULL);
    *timer = Timer_default;

    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, unit_ent, Sprite);
    SDL_assert(sprite != NULL);
    memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
    Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
    sprite->visible = true;
    sprite->flip = SDL_FLIP_HORIZONTAL;

    SDL_assert(sprite->spritesheet != NULL);
    SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_LOOP_IDLE);
    SDL_assert(sprite->spritesheet->frames != NULL);

    Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
    Sprite_Rects_Init(sprite);
    Sprite_defaultShaders_Load(sprite);

    SDL_Log("-- checks --");
    sprite->visible = true;
    SDL_assert(typeflag_id1 == typeflag_id2);
    size_t current_num = world->num_entities_bytype[typeflag_id1];
    SDL_assert(world->entities_bytype[typeflag_id1][current_num - 1] == unit_ent);
    sota->units_loaded[unit_id] = unit_ent;
    SDL_assert(unit->name.data != NULL);

    return (sota->units_loaded[unit_id]);
}
/* --- Unitmap --- */
void Game_putPConMap(struct Game *sota, i16 *unit_ids,
                     struct Point *posarr, size_t load_num) {
    SDL_assert(sota->map != NULL);
    SDL_assert(posarr != NULL);
    SDL_assert(load_num > 0);
    for (i16 i = 0; i < load_num; i++) {
        SDL_assert(Unit_ID_Valid(unit_ids[i]));
        size_t order = *(u16 *)DTAB_GET(global_unitOrders, unit_ids[i]);
        if (sota->units_loaded[unit_ids[i]] <= TNECS_NULL)
            Game_Party_Entity_Create(sota, unit_ids[i], posarr[i]);
        tnecs_entity unit_ent = sota->units_loaded[unit_ids[i]];

        SDL_assert(unit_ent > TNECS_NULL);
        struct Unit *temp = TNECS_GET_COMPONENT(sota->world, unit_ent, Unit);
        SDL_assert(temp             != NULL);
        SDL_assert(temp->name.data  != NULL);

        SDL_assert(sota->map->world == sota->world);
        Map_Unit_Put(sota->map,  posarr[i].x, posarr[i].y, unit_ent);
    }
}

/* --- Utilities --- */
void Game_Weapons_Rewrite(struct Game *sota) {
    SDL_Log("Rewriting weapons with new data\n");
    Weapons_All_Load(sota->weapons_dtab);
    Weapons_All_Save(sota->weapons_dtab);
    getchar();
}
