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
    for (size_t i = 0 ; i < (*weapons_dtab)->num; i++) {
        Weapon_Free((cast_dtab + i));
    }

    DTAB_FREE((*weapons_dtab));
    *weapons_dtab = NULL;
}

/* --- Wait/Refresh --- */
void Game_Unit_Wait(struct Game *sota, tnecs_entity_t ent) {
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
    Sprite_Animation_Restart(sprite, MAP_UNIT_SPRITE_LOOP_IDLE);
    SDL_assert(TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Timer));
    TNECS_REMOVE_COMPONENTS(sota->world, ent, Timer);
    Sprite_Draw(sprite, sota->renderer);

}

void Game_Unit_Refresh(struct Game *sota, tnecs_entity_t ent) {
    struct Unit *unit = TNECS_GET_COMPONENT(sota->world, ent, Unit);
    SDL_assert(unit != NULL);
    struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, ent, Sprite);
    SDL_assert(sprite != NULL);
    /* refresh unit */
    if (!unit->waits) {
        return;
    }

    Unit_refresh(unit);
    /* set palette back to nes */
    Sprite_Unveil(sprite, sota->renderer);

    /* restart animation */
    SDL_assert(!TNECS_ENTITY_HASCOMPONENT(sota->world, ent, Timer));
    Sprite_Animation_Restart(sprite, MAP_UNIT_SPRITE_LOOP_IDLE);
    TNECS_ADD_COMPONENT(sota->world, ent, Timer);
    struct Timer *timer = TNECS_GET_COMPONENT(sota->world, ent, Timer);
    SDL_assert(timer != NULL);
    * timer = Timer_default;
    Sprite_Draw(sprite, sota->renderer);

}

/* --- Party utilities --- */
void Game_Party_Load(struct Game *sota, i16 *unit_ids, size_t load_num) {
    struct Unit temp_unit;
    Game_Party_Clear(sota);
    for (size_t i = 0; i < load_num; i++) {
        SDL_assert((unit_ids[i] > 0) && (unit_ids[i] < UNIT_ID_PC_END));
        temp_unit       = Unit_default;
        s8 filename     = s8_mut("units"PHYSFS_SEPARATOR);
        size_t order    = *(u16 *)DTAB_GET(global_unitOrders, unit_ids[i]);
        filename        = s8cat(filename, s8_var(global_unitNames[order]));
        filename        = s8cat(filename, s8_literal(".json"));
        temp_unit.items_dtab   = sota->items_dtab;
        temp_unit.weapons_dtab = sota->weapons_dtab;
        jsonio_readJSON(filename.data, &temp_unit);
        sota->party[unit_ids[i]] = temp_unit;
        s8_free(&filename);
    }
}

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

tnecs_entity_t Game_Unit_Entity_Create(struct Game *sota, i16 in_unit,
                                       struct Point in_pos) {
    if (sota->units_loaded[in_unit] < 1) {
        tnecs_world_t *world = sota->world;
        SDL_Log("-- create entity for unit %ld --", in_unit);
        char filename[DEFAULT_BUFFER_SIZE];
        SDL_Log("-- create entity --");
        tnecs_entity_t temp_unit_ent = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position, Sprite,
                                       Timer, MapHPBar);
        tnecs_component_t typeflag = TNECS_COMPONENT_NAMES2TYPEFLAG(world, Unit, Position, Sprite,
                                                                    Timer, MapHPBar);
        size_t typeflag_id1 = tnecs_typeflagid(world, typeflag);
        size_t typeflag_id2 = tnecs_typeflagid(world, world->entity_typeflags[temp_unit_ent]);
        SDL_Log("temp_unit_ent %ld", temp_unit_ent);
        SDL_Log("typeflag %ld", world->entity_typeflags[temp_unit_ent]);
        SDL_assert(world->entities[temp_unit_ent] == temp_unit_ent);
        SDL_assert(temp_unit_ent);
        SDL_assert(sota->world->entity_typeflags[temp_unit_ent] == typeflag);

        SDL_Log("-- loading unit --");
        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        SDL_assert(unit != NULL);

        memcpy(unit, &Unit_default, sizeof(Unit_default));
        *unit = sota->party[in_unit];
        SDL_assert(unit->_id > 0);
        Unit_Init(unit);
        SDL_assert(unit->status_queue != NULL);
        unit->items_dtab   = sota->items_dtab;
        unit->weapons_dtab = sota->weapons_dtab;
        struct Inventory_item *item = Unit_Item_Strong(unit, UNIT_HAND_WEAK);
        if (item->id > ITEM_NULL)
            Unit_Equip_inHand(unit, UNIT_HAND_WEAK);
        item = Unit_Item_Strong(unit, UNIT_HAND_STRONG);
        if (item->id > ITEM_NULL)
            Unit_Equip_inHand(unit, UNIT_HAND_STRONG);

        // SDL_Log("-- loading slider --");
        // struct Slider * slider = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Slider);
        // SDL_assert(slider != NULL);
        // *slider = Slider_default;
        // slider->slidefactors[DIMENSION_X] = 2.0f;
        // slider->slidefactors[DIMENSION_Y] = 2.0f;
        // slider->update_wait = CURSOR_SLIDEWAIT;
        // slider->slidetype = SLIDETYPE_GEOMETRIC;
        SDL_Log("-- loading map_hp_bar --");
        struct MapHPBar *map_hp_bar = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, MapHPBar);
        *map_hp_bar = MapHPBar_default;
        map_hp_bar->unit_ent = temp_unit_ent;
        map_hp_bar->len = sota->settings.tilesize[0];
        SDL_assert(unit->status_queue != NULL);

        SDL_Log("-- loading position --");
        struct Position *pos;
        pos = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Position);
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
        timer = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Timer);
        SDL_assert(timer != NULL);
        * timer = Timer_default;

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        SDL_assert(sprite != NULL);
        memcpy(sprite, &Sprite_default, sizeof(Sprite_default));
        Sprite_Map_Unit_Load(sprite, unit, sota->renderer);
        sprite->visible = true;
        sprite->flip = SDL_FLIP_HORIZONTAL;

        SDL_Log("name %s", unit->name);
        SDL_assert(sprite->spritesheet != NULL);
        SDL_assert(sprite->spritesheet->current_loop == MAP_UNIT_SPRITE_LOOP_IDLE);
        SDL_assert(sprite->spritesheet->frames != NULL);
        SDL_assert(sprite->spritesheet->frames[0] == 4);

        Sprite_Tilesize_Set(sprite, sota->settings.tilesize);
        Sprite_Rects_Init(sprite);
        Sprite_defaultShaders_Load(sprite);

        SDL_Log("-- checks --");
        sprite->visible = true;
        SDL_assert(typeflag_id1 == typeflag_id2);
        size_t current_num = world->num_entities_bytype[typeflag_id1];
        SDL_assert(world->entities_bytype[typeflag_id1][current_num - 1] == temp_unit_ent);
        sota->units_loaded[in_unit] = temp_unit_ent;
    }
    return (sota->units_loaded[in_unit]);
}
/* --- Unitmap --- */
void Game_UnitsonMap_Free(struct Game *sota) {
    tnecs_entity_t temp_unit_ent;
    if (sota->map == NULL) {
        return;
    }
    for (size_t i = 0; i < sota->map->col_len * sota->map->row_len; i++) {
        temp_unit_ent = sota->map->unitmap[i];
        if (temp_unit_ent == TNECS_NULL)
            continue;

        struct Unit *unit = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Unit);
        if (unit)
            Unit_Free(unit);

        struct Sprite *sprite = TNECS_GET_COMPONENT(sota->world, temp_unit_ent, Sprite);
        if (sprite)
            Sprite_Free(sprite);
    }
}
void Game_putPConMap(struct Game *sota, i16 *unit_ids,
                     struct Point *posarr, size_t load_num) {
    SDL_assert(sota->map != NULL);
    SDL_assert(posarr != NULL);
    SDL_assert(load_num > 0);
    for (i16 i = 0; i < load_num; i++) {
        SDL_assert(Unit_ID_Valid(unit_ids[i]));
        size_t order = *(u16 *)DTAB_GET(global_unitOrders, unit_ids[i]);
        tnecs_entity_t temp_unit_ent = Game_Unit_Entity_Create(sota, unit_ids[i], posarr[i]);
        SDL_assert(temp_unit_ent);
        Map_Unit_Put(sota->map, sota->world, posarr[i].x, posarr[i].y, temp_unit_ent);
    }
}

/* --- Utilities --- */
void Game_Weapons_Rewrite(struct Game *sota) {
    SDL_Log("Rewriting weapons with new data\n");
    Weapons_All_Load(sota->weapons_dtab);
    Weapons_All_Save(sota->weapons_dtab);
    getchar();
}
