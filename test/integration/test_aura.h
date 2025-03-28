
#include "game/game.h"
#include "map/ontile.h"
#include "fsm.h"
#include "macros.h"
#include "map/map.h"
#include "game/map.h"

void test_aura_apply(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_NES1);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(sota->map != NULL );
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);
    sota->map->world = sota->world;

    /* Load Standard */
    SDL_assert(sota->weapons_dtab != NULL);
    Weapon_Load(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */

    i32 id;
    /* Place Standard bearer inside */
    struct Point pos = {4, 4};
    Game_Party_Entity_Create(sota, id = UNIT_ID_ERWIN, pos, &Unit_default);
    tnecs_entity ent = sota->party.entities[UNIT_ID_ERWIN];
    struct Unit *erwin = IES_GET_COMPONENT(sota->world, ent, Unit);

    erwin->class = UNIT_CLASS_STANDARD_BEARER;
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);


    /* Give standard to standard bearer */
    struct Unit *bearer = IES_GET_COMPONENT(sota->world, ent, Unit);
    SDL_assert(bearer != NULL);

    struct Inventory_item standard = Inventory_item_default;
    standard.id = ITEM_ID_IMPERIAL_STANDARD;
    Unit_Item_Drop(     bearer, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   bearer, standard, UNIT_HAND_RIGHT);
    Unit_Equip(bearer, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(bearer, UNIT_HAND_RIGHT));
    SDL_assert(Unit_Id_Equipment(bearer, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);

    /* Place Friendly 1 inside */
    pos.x = 3;
    pos.y = 3;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SILOU, pos, &Unit_default);
    ent = sota->party.entities[id];
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);

    /* Place Friendly 2 inside */
    pos.x = 3;
    pos.y = 2;
    Game_Party_Entity_Create(sota, id = UNIT_ID_KIARA, pos, &Unit_default);
    ent = sota->party.entities[id];
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);

    /* Place Friendly 3 outside */
    pos.x = 2;
    pos.y = 2;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SERVIL, pos, &Unit_default);
    ent = sota->party.entities[id];
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);

    /* Check Aura in bonus stack */
    Map_Bonus_Standard_Apply(sota->map, ARMY_FRIENDLY);
    Map_Bonus_Standard_Apply(sota->map, ARMY_FRIENDLY);
    struct Weapon *standardwpn          = DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item->aura.unit_stats;

    /* Check effective stats */
    struct Unit *silou = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SILOU], Unit);
    SDL_assert(silou != NULL);
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->current_stats.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->current_stats.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->current_stats.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->current_stats.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->current_stats.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->current_stats.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->current_stats.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->current_stats.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->current_stats.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->current_stats.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->current_stats.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->current_stats.prof   + aura_bonus.prof));

    struct Unit *kiara = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_KIARA], Unit);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->current_stats.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (kiara->current_stats.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (kiara->current_stats.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (kiara->current_stats.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (kiara->current_stats.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (kiara->current_stats.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (kiara->current_stats.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (kiara->current_stats.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (kiara->current_stats.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (kiara->current_stats.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (kiara->current_stats.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (kiara->current_stats.prof   + aura_bonus.prof));

    struct Unit *servil = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SERVIL], Unit);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->current_stats.hp);
    nourstest_true(effective_stats.str  == servil->current_stats.str);
    nourstest_true(effective_stats.mag  == servil->current_stats.mag);
    nourstest_true(effective_stats.agi  == servil->current_stats.agi);
    nourstest_true(effective_stats.dex  == servil->current_stats.dex);
    nourstest_true(effective_stats.fth  == servil->current_stats.fth);
    nourstest_true(effective_stats.luck == servil->current_stats.luck);
    nourstest_true(effective_stats.def  == servil->current_stats.def);
    nourstest_true(effective_stats.res  == servil->current_stats.res);
    nourstest_true(effective_stats.con  == servil->current_stats.con);
    nourstest_true(effective_stats.move == servil->current_stats.move);
    nourstest_true(effective_stats.prof == servil->current_stats.prof);

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

void test_aura_decay(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_NES1);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(sota->map != NULL );
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);
    sota->map->world = sota->world;

    /* Load Standard */
    SDL_assert(sota->weapons_dtab != NULL);
    Weapon_Load(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */
    i32 id;
    tnecs_entity ent;
    /* Place Standard bearer inside */
    struct Point pos = {4, 4};
    Game_Party_Entity_Create(sota, id = UNIT_ID_ERWIN, pos, &Unit_default);
    struct Unit *erwin = IES_GET_COMPONENT(sota->world, sota->party.entities[id], Unit);
    erwin->class = UNIT_CLASS_STANDARD_BEARER;
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);

    /* Give standard to standard bearer */
    struct Unit *bearer = IES_GET_COMPONENT(sota->world, ent, Unit);
    SDL_assert(bearer != NULL);

    struct Inventory_item standard = Inventory_item_default;
    standard.id = ITEM_ID_IMPERIAL_STANDARD;
    Unit_Item_Drop(     bearer, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   bearer, standard, UNIT_HAND_RIGHT);
    Unit_Equip(bearer, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(bearer, UNIT_HAND_RIGHT) == true);
    SDL_assert(Unit_Id_Equipment(bearer, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);

    /* Place Friendly 1 inside */
    pos.x = 3;
    pos.y = 3;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SILOU, pos, &Unit_default);
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);

    /* Place Friendly 2 inside */
    pos.x = 3;
    pos.y = 2;
    Game_Party_Entity_Create(sota, id = UNIT_ID_KIARA, pos, &Unit_default);
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);

    /* Place Friendly 3 outside */
    pos.x = 2;
    pos.y = 2;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SERVIL, pos, &Unit_default);
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);

    /* Check Aura in bonus stack */
    Map_Bonus_Standard_Apply(sota->map, ARMY_FRIENDLY);
    struct Weapon *standardwpn          = DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item->aura.unit_stats;

    /*  Decaying */
    Map_Bonus_Remove_Instant(sota->map, ARMY_FRIENDLY);

    /* Check effective stats */
    struct Unit *silou = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SILOU], Unit);
    nourstest_true(DARR_NUM(silou->bonus_stack) == 0);
    SDL_assert(silou != NULL);
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->current_stats.hp));
    nourstest_true(effective_stats.str  == (silou->current_stats.str));
    nourstest_true(effective_stats.mag  == (silou->current_stats.mag));
    nourstest_true(effective_stats.agi  == (silou->current_stats.agi));
    nourstest_true(effective_stats.dex  == (silou->current_stats.dex));
    nourstest_true(effective_stats.fth  == (silou->current_stats.fth));
    nourstest_true(effective_stats.luck == (silou->current_stats.luck));
    nourstest_true(effective_stats.def  == (silou->current_stats.def));
    nourstest_true(effective_stats.res  == (silou->current_stats.res));
    nourstest_true(effective_stats.con  == (silou->current_stats.con));
    nourstest_true(effective_stats.move == (silou->current_stats.move));
    nourstest_true(effective_stats.prof == (silou->current_stats.prof));

    struct Unit *kiara = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_KIARA], Unit);
    nourstest_true(DARR_NUM(kiara->bonus_stack) == 0);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->current_stats.hp));
    nourstest_true(effective_stats.str  == (kiara->current_stats.str));
    nourstest_true(effective_stats.mag  == (kiara->current_stats.mag));
    nourstest_true(effective_stats.agi  == (kiara->current_stats.agi));
    nourstest_true(effective_stats.dex  == (kiara->current_stats.dex));
    nourstest_true(effective_stats.fth  == (kiara->current_stats.fth));
    nourstest_true(effective_stats.luck == (kiara->current_stats.luck));
    nourstest_true(effective_stats.def  == (kiara->current_stats.def));
    nourstest_true(effective_stats.res  == (kiara->current_stats.res));
    nourstest_true(effective_stats.con  == (kiara->current_stats.con));
    nourstest_true(effective_stats.move == (kiara->current_stats.move));
    nourstest_true(effective_stats.prof == (kiara->current_stats.prof));

    struct Unit *servil = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SERVIL], Unit);
    nourstest_true(DARR_NUM(servil->bonus_stack) == 0);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->current_stats.hp);
    nourstest_true(effective_stats.str  == servil->current_stats.str);
    nourstest_true(effective_stats.mag  == servil->current_stats.mag);
    nourstest_true(effective_stats.agi  == servil->current_stats.agi);
    nourstest_true(effective_stats.dex  == servil->current_stats.dex);
    nourstest_true(effective_stats.fth  == servil->current_stats.fth);
    nourstest_true(effective_stats.luck == servil->current_stats.luck);
    nourstest_true(effective_stats.def  == servil->current_stats.def);
    nourstest_true(effective_stats.res  == servil->current_stats.res);
    nourstest_true(effective_stats.con  == servil->current_stats.con);
    nourstest_true(effective_stats.move == servil->current_stats.move);
    nourstest_true(effective_stats.prof == servil->current_stats.prof);

    Map_Bonus_Standard_Apply(sota->map, ARMY_FRIENDLY);

    /* Check effective stats */
    silou = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SILOU], Unit);
    nourstest_true(DARR_NUM(silou->bonus_stack) == 1);
    SDL_assert(silou != NULL);
    effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->current_stats.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->current_stats.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->current_stats.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->current_stats.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->current_stats.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->current_stats.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->current_stats.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->current_stats.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->current_stats.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->current_stats.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->current_stats.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->current_stats.prof   + aura_bonus.prof));

    kiara = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_KIARA], Unit);
    nourstest_true(DARR_NUM(kiara->bonus_stack) == 1);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->current_stats.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (kiara->current_stats.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (kiara->current_stats.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (kiara->current_stats.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (kiara->current_stats.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (kiara->current_stats.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (kiara->current_stats.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (kiara->current_stats.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (kiara->current_stats.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (kiara->current_stats.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (kiara->current_stats.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (kiara->current_stats.prof   + aura_bonus.prof));

    servil = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SERVIL], Unit);
    nourstest_true(DARR_NUM(servil->bonus_stack) == 0);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->current_stats.hp);
    nourstest_true(effective_stats.str  == servil->current_stats.str);
    nourstest_true(effective_stats.mag  == servil->current_stats.mag);
    nourstest_true(effective_stats.agi  == servil->current_stats.agi);
    nourstest_true(effective_stats.dex  == servil->current_stats.dex);
    nourstest_true(effective_stats.fth  == servil->current_stats.fth);
    nourstest_true(effective_stats.luck == servil->current_stats.luck);
    nourstest_true(effective_stats.def  == servil->current_stats.def);
    nourstest_true(effective_stats.res  == servil->current_stats.res);
    nourstest_true(effective_stats.con  == servil->current_stats.con);
    nourstest_true(effective_stats.move == servil->current_stats.move);
    nourstest_true(effective_stats.prof == servil->current_stats.prof);

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

void test_aura_fsm(int argc, char *argv[]) {
    /* Test scenario:
     *    - Unit moves inside range, triggering -> fsm_eAcpt_sGmpMap_ssMapUnitMv
     *        - Bonus active
     *    - Unit moves outside range, triggering -> fsm_eAcpt_sGmpMap_ssMapUnitMv
     *        - Bonus inactive
    */

    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Map_Party_Load(sota, CHAPTER_TEST_NES1);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(sota->map != NULL );
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);
    sota->map->world = sota->world;

    /* Load Standard */
    SDL_assert(sota->weapons_dtab != NULL);
    Weapon_Load(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */
    i32 id;
    tnecs_entity ent;

    /* Place Standard bearer */
    struct Point pos = {4, 4};
    Game_Party_Entity_Create(sota, id = UNIT_ID_ERWIN, pos, &Unit_default);
    struct Unit *erwin = IES_GET_COMPONENT(sota->world, sota->party.entities[id], Unit);
    erwin->class = UNIT_CLASS_STANDARD_BEARER;
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);

    /* Give standard to standard bearer */
    struct Inventory_item standard = Inventory_item_default;
    standard.id = ITEM_ID_IMPERIAL_STANDARD;
    Unit_Item_Drop(     erwin, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   erwin, standard, UNIT_HAND_RIGHT);
    Unit_Equip(         erwin, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(erwin, UNIT_HAND_RIGHT) == true);
    SDL_assert(Unit_Id_Equipment(erwin, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);

    /* Move Friendly 1 inside */
    pos.x = 0;
    pos.y = 0;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SILOU, pos, &Unit_default);
    ent = sota->party.entities[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->party.entities[id] > TNECS_NULL);
    sota->selected_unit_initial_position.x  = 0;
    sota->selected_unit_initial_position.y  = 0;
    struct Position *cursor_pos = IES_GET_COMPONENT(sota->world, sota->entity_cursor, Position);
    cursor_pos->tilemap_pos.x               = 3;
    cursor_pos->tilemap_pos.y               = 3;

    /* Mocking stuff for fsm_eAcpt_sGmpMap_ssMapUnitMv */
    sota->selected_unit_entity = sota->party.entities[UNIT_ID_SILOU];
    sota->map->army_i = 0;
    fsm_eAcpt_sGmpMap_ssMapUnitMv(sota, TNECS_NULL);
    struct Position *silou_pos = IES_GET_COMPONENT(sota->world,
                                                   sota->party.entities[UNIT_ID_SILOU],
                                                   Position);
    SDL_assert(silou_pos->tilemap_pos.x == sota->selected_unit_moved_position.x);
    SDL_assert(silou_pos->tilemap_pos.y == sota->selected_unit_moved_position.y);

    /* Check effective stats */
    struct Unit *silou = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SILOU], Unit);
    nourstest_true(DARR_NUM(silou->bonus_stack) == 1);
    SDL_assert(silou != NULL);
    struct Weapon *standardwpn          = DTAB_GET(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item->aura.unit_stats;
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->current_stats.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->current_stats.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->current_stats.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->current_stats.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->current_stats.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->current_stats.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->current_stats.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->current_stats.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->current_stats.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->current_stats.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->current_stats.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->current_stats.prof   + aura_bonus.prof));

    /* Move Friendly 1 outside */
    sota->selected_unit_initial_position.x  = 3;
    sota->selected_unit_initial_position.y  = 3;
    cursor_pos->tilemap_pos.x               = 0;
    cursor_pos->tilemap_pos.y               = 0;
    sota->selected_unit_entity              = sota->party.entities[UNIT_ID_SILOU];

    fsm_eAcpt_sGmpMap_ssMapUnitMv(sota, TNECS_NULL);
    nourstest_true(DARR_NUM(silou->bonus_stack) == 0);
    silou_pos = IES_GET_COMPONENT(sota->world, sota->party.entities[UNIT_ID_SILOU], Position);
    SDL_assert(silou_pos->tilemap_pos.x == sota->selected_unit_moved_position.x);
    SDL_assert(silou_pos->tilemap_pos.y == sota->selected_unit_moved_position.y);

    effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == silou->current_stats.hp);
    nourstest_true(effective_stats.str  == silou->current_stats.str);
    nourstest_true(effective_stats.mag  == silou->current_stats.mag);
    nourstest_true(effective_stats.agi  == silou->current_stats.agi);
    nourstest_true(effective_stats.dex  == silou->current_stats.dex);
    nourstest_true(effective_stats.fth  == silou->current_stats.fth);
    nourstest_true(effective_stats.luck == silou->current_stats.luck);
    nourstest_true(effective_stats.def  == silou->current_stats.def);
    nourstest_true(effective_stats.res  == silou->current_stats.res);
    nourstest_true(effective_stats.con  == silou->current_stats.con);
    nourstest_true(effective_stats.move == silou->current_stats.move);
    nourstest_true(effective_stats.prof == silou->current_stats.prof);

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}
