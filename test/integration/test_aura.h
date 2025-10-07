
#include "game/game.h"
#include "map/ontile.h"
#include "fsm.h"
#include "globals.h"
#include "macros.h"
#include "map/map.h"
#include "game/map.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    erwin_eq[eq] = seteqentity;

void test_aura_apply(int argc, char *argv[]) {
    /* -- Startup -- */
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_0);
    Game_Map_Reinforcements_Load(sota);
    Map *map = Game_Map(sota);
    SDL_assert(map != NULL );
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Load Standard */
    SDL_assert(gl_weapons_dtab != NULL);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */

    i32 id;
    /* Place Standard bearer inside */
    struct Point pos = {4, 4};
    tnecs_E ent = Game_Party_Entity_Create(sota);
    struct Unit *erwin = IES_GET_C(gl_world, ent, Unit);
    SDL_assert(erwin->arms.num > 0);
    SDL_assert(erwin != NULL);
    tnecs_E *erwin_eq = Unit_Equipment(erwin);
    struct Position *erwin_pos = IES_GET_C(gl_world, ent, Position);
    erwin_pos->tilemap_pos = pos;
    Unit_id_set(erwin, id = UNIT_ID_ERWIN);
    Unit_Class_set(erwin, UNIT_CLASS_STANDARD_BEARER);
    SDL_assert(erwin->arms.num > 0);
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(map, pos.x, pos.y, ent);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, ent, filename);
    erwin = IES_GET_C(gl_world, ent, Unit);

    /* Give standard to standard bearer */
    TEST_SET_EQUIPMENT(gl_world, ITEM_ID_IMPERIAL_STANDARD, UNIT_HAND_RIGHT);
    SDL_assert(erwin->arms.num > 0);
    Unit_Item_Drop(     erwin, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   erwin, seteqentity, UNIT_HAND_RIGHT);
    SDL_assert(Unit_Id_Equipment(erwin, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);
    Unit_Equip(erwin, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(erwin, UNIT_HAND_RIGHT));

    /* Place Friendly 1 inside */
    pos.x = 3;
    pos.y = 3;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *silou = IES_GET_C(gl_world, ent, Unit);
    Position *silou_pos = IES_GET_C(gl_world, ent, Position);
    silou_pos->tilemap_pos = pos;
    Unit_id_set(silou, id = UNIT_ID_SILOU);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    /* Place Friendly 2 inside */
    pos.x = 3;
    pos.y = 2;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *kiara = IES_GET_C(gl_world, ent, Unit);
    Position *kiara_pos = IES_GET_C(gl_world, ent, Position);
    kiara_pos->tilemap_pos = pos;
    Unit_id_set(kiara, id = UNIT_ID_KIARA);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    /* Place Friendly 3 outside */
    pos.x = 2;
    pos.y = 2;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *servil = IES_GET_C(gl_world, ent, Unit);
    Position *servil_pos = IES_GET_C(gl_world, ent, Position);
    servil_pos->tilemap_pos = pos;
    Unit_id_set(servil, id = UNIT_ID_SERVIL);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    /* Check Aura in bonus stack */
    Map_Bonus_Standard_Apply(map, ARMY_FRIENDLY);
    Map_Bonus_Standard_Apply(map, ARMY_FRIENDLY);
    struct Weapon *standardwpn          = DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item.aura.unit_stats;

    /* Check effective stats */
    silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    SDL_assert(silou != NULL);
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->stats.current.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->stats.current.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->stats.current.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->stats.current.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->stats.current.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->stats.current.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->stats.current.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->stats.current.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->stats.current.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->stats.current.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->stats.current.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->stats.current.prof   + aura_bonus.prof));

    kiara = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_KIARA], Unit);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->stats.current.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (kiara->stats.current.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (kiara->stats.current.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (kiara->stats.current.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (kiara->stats.current.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (kiara->stats.current.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (kiara->stats.current.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (kiara->stats.current.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (kiara->stats.current.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (kiara->stats.current.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (kiara->stats.current.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (kiara->stats.current.prof   + aura_bonus.prof));

    servil = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SERVIL],
                       Unit);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->stats.current.hp);
    nourstest_true(effective_stats.str  == servil->stats.current.str);
    nourstest_true(effective_stats.mag  == servil->stats.current.mag);
    nourstest_true(effective_stats.agi  == servil->stats.current.agi);
    nourstest_true(effective_stats.dex  == servil->stats.current.dex);
    nourstest_true(effective_stats.fth  == servil->stats.current.fth);
    nourstest_true(effective_stats.luck == servil->stats.current.luck);
    nourstest_true(effective_stats.def  == servil->stats.current.def);
    nourstest_true(effective_stats.res  == servil->stats.current.res);
    nourstest_true(effective_stats.con  == servil->stats.current.con);
    nourstest_true(effective_stats.move == servil->stats.current.move);
    nourstest_true(effective_stats.prof == servil->stats.current.prof);

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

void test_aura_decay(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_0);
    Game_Map_Reinforcements_Load(sota);

    Map *map = Game_Map(sota);
    SDL_assert(map != NULL );
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);
    if (sota->party.Es[UNIT_ID_ERWIN] != NULL) {
        tnecs_E_destroy(gl_world, sota->party.Es[UNIT_ID_ERWIN]);
        sota->party.Es[UNIT_ID_ERWIN] = TNECS_NULL;
    }
    if (sota->party.Es[UNIT_ID_SILOU] != NULL) {
        tnecs_E_destroy(gl_world, sota->party.Es[UNIT_ID_SILOU]);
        sota->party.Es[UNIT_ID_SILOU] = TNECS_NULL;
    }
    if (sota->party.Es[UNIT_ID_KIARA] != NULL) {
        tnecs_E_destroy(gl_world, sota->party.Es[UNIT_ID_KIARA]);
        sota->party.Es[UNIT_ID_KIARA] = TNECS_NULL;
    }
    if (sota->party.Es[UNIT_ID_SERVIL] != NULL) {
        tnecs_E_destroy(gl_world, sota->party.Es[UNIT_ID_SERVIL]);
        sota->party.Es[UNIT_ID_SERVIL] = TNECS_NULL;
    }

    /* Load Standard */
    SDL_assert(gl_weapons_dtab != NULL);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */
    i32 id;
    /* Place Standard bearer inside */
    struct Point pos = {4, 4};
    tnecs_E ent = Game_Party_Entity_Create(sota);
    SDL_assert(sota->party.Es[UNIT_ID_ERWIN] == TNECS_NULL);
    struct Unit *erwin = IES_GET_C(gl_world, ent, Unit);
    SDL_assert(erwin->arms.num == 2);
    struct Position *erwin_pos = IES_GET_C(gl_world, ent, Position);
    SDL_assert(erwin->arms.num == 2);
    erwin_pos->tilemap_pos = pos;
    SDL_assert(erwin->arms.num == 2);
    Unit_id_set(erwin, id = UNIT_ID_ERWIN);
    SDL_assert(erwin->arms.num == 2);
    Unit_Class_set(erwin, UNIT_CLASS_STANDARD_BEARER);
    SDL_assert(erwin->arms.num == 2);
    SDL_assert(ent > TNECS_NULL);
    SDL_assert(erwin->arms.num == 2);
    Map_Unit_Put(map, pos.x, pos.y, ent);
    SDL_assert(erwin->arms.num == 2);
    SDL_assert(sota->party.Es[UNIT_ID_ERWIN] == NULL);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, ent, filename);
    erwin = IES_GET_C(gl_world, ent, Unit);
    SDL_assert(ent == sota->party.Es[UNIT_ID_ERWIN]);
    SDL_assert(TNECS_E_EXISTS(gl_world, ent));
    SDL_assert(erwin->arms.num == 2);

    /* Give standard to standard bearer */
    SDL_assert(erwin != NULL);

    SDL_assert(erwin->arms.num == 2);
    Unit_Item_Drop(     erwin, UNIT_HAND_RIGHT);
    SDL_assert(erwin->arms.num == 2);

    tnecs_E *erwin_eq = Unit_Equipment(erwin);
    TEST_SET_EQUIPMENT(gl_world, ITEM_ID_IMPERIAL_STANDARD, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   erwin, seteqentity, UNIT_HAND_RIGHT);
    SDL_assert(erwin->arms.num == 2);
    Unit_Equip(erwin, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(erwin, UNIT_HAND_RIGHT) == true);
    SDL_assert(Unit_Id_Equipment(erwin, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);

    /* Place Friendly 1 inside */
    pos.x = 3;
    pos.y = 3;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *silou = IES_GET_C(gl_world, ent, Unit);
    Position *silou_pos = IES_GET_C(gl_world, ent, Position);
    silou_pos->tilemap_pos = pos;
    Unit_id_set(silou, id = UNIT_ID_SILOU);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);
    SDL_assert(sota->party.Es[id] > TNECS_NULL);

    /* Place Friendly 2 inside */
    pos.x = 3;
    pos.y = 2;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *kiara = IES_GET_C(gl_world, ent, Unit);
    Position *kiara_pos = IES_GET_C(gl_world, ent, Position);
    kiara_pos->tilemap_pos = pos;
    Unit_id_set(kiara, id = UNIT_ID_KIARA);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    /* Place Friendly 3 outside */
    pos.x = 2;
    pos.y = 2;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *servil = IES_GET_C(gl_world, ent, Unit);
    Position *servil_pos = IES_GET_C(gl_world, ent, Position);
    servil_pos->tilemap_pos = pos;
    Unit_id_set(servil, id = UNIT_ID_SERVIL);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    /* Check Aura in bonus stack */
    Map_Bonus_Standard_Apply(map, ARMY_FRIENDLY);
    struct Weapon *standardwpn          = DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item.aura.unit_stats;

    /*  Decaying */
    SDL_assert(DARR_NUM(erwin->stats.bonus_stack) == 1);
    SDL_assert(DARR_NUM(silou->stats.bonus_stack) == 1);
    SDL_assert(DARR_NUM(kiara->stats.bonus_stack) == 1);
    SDL_assert(DARR_NUM(servil->stats.bonus_stack) == 0);

    Map_Bonus_Remove_Instant(map, ARMY_FRIENDLY);

    /* Check effective stats */
    silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    nourstest_true(DARR_NUM(silou->stats.bonus_stack) == 0);
    SDL_assert(silou != NULL);
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->stats.current.hp));
    nourstest_true(effective_stats.str  == (silou->stats.current.str));
    nourstest_true(effective_stats.mag  == (silou->stats.current.mag));
    nourstest_true(effective_stats.agi  == (silou->stats.current.agi));
    nourstest_true(effective_stats.dex  == (silou->stats.current.dex));
    nourstest_true(effective_stats.fth  == (silou->stats.current.fth));
    nourstest_true(effective_stats.luck == (silou->stats.current.luck));
    nourstest_true(effective_stats.def  == (silou->stats.current.def));
    nourstest_true(effective_stats.res  == (silou->stats.current.res));
    nourstest_true(effective_stats.con  == (silou->stats.current.con));
    nourstest_true(effective_stats.move == (silou->stats.current.move));
    nourstest_true(effective_stats.prof == (silou->stats.current.prof));

    kiara = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_KIARA], Unit);
    nourstest_true(DARR_NUM(kiara->stats.bonus_stack) == 0);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->stats.current.hp));
    nourstest_true(effective_stats.str  == (kiara->stats.current.str));
    nourstest_true(effective_stats.mag  == (kiara->stats.current.mag));
    nourstest_true(effective_stats.agi  == (kiara->stats.current.agi));
    nourstest_true(effective_stats.dex  == (kiara->stats.current.dex));
    nourstest_true(effective_stats.fth  == (kiara->stats.current.fth));
    nourstest_true(effective_stats.luck == (kiara->stats.current.luck));
    nourstest_true(effective_stats.def  == (kiara->stats.current.def));
    nourstest_true(effective_stats.res  == (kiara->stats.current.res));
    nourstest_true(effective_stats.con  == (kiara->stats.current.con));
    nourstest_true(effective_stats.move == (kiara->stats.current.move));
    nourstest_true(effective_stats.prof == (kiara->stats.current.prof));

    servil = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SERVIL], Unit);
    nourstest_true(DARR_NUM(servil->stats.bonus_stack) == 0);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->stats.current.hp);
    nourstest_true(effective_stats.str  == servil->stats.current.str);
    nourstest_true(effective_stats.mag  == servil->stats.current.mag);
    nourstest_true(effective_stats.agi  == servil->stats.current.agi);
    nourstest_true(effective_stats.dex  == servil->stats.current.dex);
    nourstest_true(effective_stats.fth  == servil->stats.current.fth);
    nourstest_true(effective_stats.luck == servil->stats.current.luck);
    nourstest_true(effective_stats.def  == servil->stats.current.def);
    nourstest_true(effective_stats.res  == servil->stats.current.res);
    nourstest_true(effective_stats.con  == servil->stats.current.con);
    nourstest_true(effective_stats.move == servil->stats.current.move);
    nourstest_true(effective_stats.prof == servil->stats.current.prof);

    Map_Bonus_Standard_Apply(map, ARMY_FRIENDLY);

    /* Check effective stats */
    silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    nourstest_true(DARR_NUM(silou->stats.bonus_stack) == 1);
    SDL_assert(silou != NULL);
    effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->stats.current.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->stats.current.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->stats.current.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->stats.current.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->stats.current.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->stats.current.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->stats.current.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->stats.current.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->stats.current.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->stats.current.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->stats.current.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->stats.current.prof   + aura_bonus.prof));

    kiara = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_KIARA], Unit);
    nourstest_true(DARR_NUM(kiara->stats.bonus_stack) == 1);
    SDL_assert(kiara != NULL);
    effective_stats = Unit_effectiveStats(kiara);

    nourstest_true(effective_stats.hp   == (kiara->stats.current.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (kiara->stats.current.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (kiara->stats.current.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (kiara->stats.current.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (kiara->stats.current.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (kiara->stats.current.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (kiara->stats.current.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (kiara->stats.current.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (kiara->stats.current.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (kiara->stats.current.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (kiara->stats.current.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (kiara->stats.current.prof   + aura_bonus.prof));

    servil = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SERVIL], Unit);
    nourstest_true(DARR_NUM(servil->stats.bonus_stack) == 0);
    SDL_assert(servil != NULL);
    effective_stats = Unit_effectiveStats(servil);

    nourstest_true(effective_stats.hp   == servil->stats.current.hp);
    nourstest_true(effective_stats.str  == servil->stats.current.str);
    nourstest_true(effective_stats.mag  == servil->stats.current.mag);
    nourstest_true(effective_stats.agi  == servil->stats.current.agi);
    nourstest_true(effective_stats.dex  == servil->stats.current.dex);
    nourstest_true(effective_stats.fth  == servil->stats.current.fth);
    nourstest_true(effective_stats.luck == servil->stats.current.luck);
    nourstest_true(effective_stats.def  == servil->stats.current.def);
    nourstest_true(effective_stats.res  == servil->stats.current.res);
    nourstest_true(effective_stats.con  == servil->stats.current.con);
    nourstest_true(effective_stats.move == servil->stats.current.move);
    nourstest_true(effective_stats.prof == servil->stats.current.prof);

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
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    Names_Load_All();
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    struct Game *sota           = Game_New(settings);
    nourstest_true(Game_State_Current(sota)      == GAME_STATE_Title_Screen);
    nourstest_true(Game_Substate_Current(sota)   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_Save_Load(sota, SOTA_SAVE_DEBUG_0);
    Game_Map_Reinforcements_Load(sota);
    Map *map = Game_Map(sota);
    SDL_assert(map != NULL );
    SDL_assert(DARR_NUM(map->units.onfield.arr) > 0);

    /* Load Standard */
    SDL_assert(gl_weapons_dtab != NULL);
    Weapon_Load(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    SDL_assert(DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD) != NULL);
    /* -- Place all friendlies close together -- */
    i32 id;
    tnecs_E ent;

    /* Place Standard bearer */
    struct Point pos = {4, 4};
    ent = Game_Party_Entity_Create(sota);
    struct Unit *erwin = IES_GET_C(gl_world, ent, Unit);
    struct Position *erwin_pos = IES_GET_C(gl_world, ent, Position);
    erwin_pos->tilemap_pos = pos;
    Unit_id_set(erwin, id = UNIT_ID_ERWIN);
    Unit_Class_set(erwin, UNIT_CLASS_STANDARD_BEARER);
    SDL_assert(ent > TNECS_NULL);
    Map_Unit_Put(map, pos.x, pos.y, ent);
    s8 filename = {0};
    Game_Party_Entity_Init(sota, ent, filename);

    SDL_assert(sota->party.Es[UNIT_ID_ERWIN] > TNECS_NULL);

    erwin = IES_GET_C(gl_world, sota->party.Es[id], Unit);
    Unit_Class_set(erwin, UNIT_CLASS_STANDARD_BEARER);
    ent = sota->party.Es[id];
    Map_Unit_Put(map, pos.x, pos.y, ent);
    SDL_assert(sota->party.Es[id] > TNECS_NULL);
    tnecs_E *erwin_eq = Unit_Equipment(erwin);

    /* Give standard to standard bearer */
    Unit_Item_Drop(     erwin, UNIT_HAND_RIGHT);
    TEST_SET_EQUIPMENT(gl_world, ITEM_ID_IMPERIAL_STANDARD, UNIT_HAND_RIGHT);
    Unit_Item_Takeat(   erwin, seteqentity, UNIT_HAND_RIGHT);
    Unit_Equip(         erwin, UNIT_HAND_RIGHT, UNIT_HAND_RIGHT);
    SDL_assert(Unit_isEquipped(erwin, UNIT_HAND_RIGHT) == true);
    SDL_assert(Unit_Id_Equipment(erwin, UNIT_HAND_RIGHT) == ITEM_ID_IMPERIAL_STANDARD);

    /* Move Friendly 1 inside */
    pos.x = 0;
    pos.y = 0;
    ent = Game_Party_Entity_Create(sota);
    SDL_assert(ent > TNECS_NULL);
    Unit *silou = IES_GET_C(gl_world, ent, Unit);
    Position *silou_pos = IES_GET_C(gl_world, ent, Position);
    silou_pos->tilemap_pos = pos;
    Unit_id_set(silou, id = UNIT_ID_SILOU);
    Game_Party_Entity_Init(sota, ent, filename);
    Map_Unit_Put(map, pos.x, pos.y, ent);

    SDL_assert(sota->party.Es[UNIT_ID_SILOU] > TNECS_NULL);

    ent = sota->party.Es[id];
    Map_Unit_Put(map, pos.x, pos.y, ent);
    SDL_assert(sota->party.Es[id] > TNECS_NULL);
    sota->selected.unit_initial_position.x  = 0;
    sota->selected.unit_initial_position.y  = 0;
    struct Position *cursor_pos = IES_GET_C(gl_world, sota->cursor.entity, Position);
    cursor_pos->tilemap_pos.x               = 3;
    cursor_pos->tilemap_pos.y               = 3;

    /* Mocking stuff for fsm_eAcpt_sGmpMap_ssMapUnitMv */
    sota->selected.unit_entity = sota->party.Es[UNIT_ID_SILOU];
    map->armies.current = 0;
    silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    SDL_assert(sota->party.Es[UNIT_ID_SILOU] != sota->party.Es[UNIT_ID_ERWIN]);
    SDL_assert(erwin->stats.bonus_stack != silou->stats.bonus_stack);

    SDL_assert(DARR_NUM(silou->stats.bonus_stack) == 0);
    SDL_assert(DARR_NUM(erwin->stats.bonus_stack) == 0);

    /* Applying bonuses */
    fsm_eAcpt_sGmpMap_ssMapUnitMv(sota, TNECS_NULL);

    /* Checking */
    silou_pos = IES_GET_C(gl_world,
                          sota->party.Es[UNIT_ID_SILOU],
                          Position);
    SDL_assert(silou_pos->tilemap_pos.x == sota->selected.unit_moved_position.x);
    SDL_assert(silou_pos->tilemap_pos.y == sota->selected.unit_moved_position.y);

    /* Check effective stats */
    SDL_assert(silou->stats.bonus_stack != NULL);
    SDL_assert(erwin->stats.bonus_stack != NULL);
    nourstest_true(DARR_NUM(erwin->stats.bonus_stack) == 1);
    nourstest_true(DARR_NUM(silou->stats.bonus_stack) == 1);
    SDL_assert(silou != NULL);
    struct Weapon *standardwpn          = DTAB_GET(gl_weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);
    struct Unit_stats aura_bonus        = standardwpn->item.aura.unit_stats;
    struct Unit_stats effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == (silou->stats.current.hp     + aura_bonus.hp));
    nourstest_true(effective_stats.str  == (silou->stats.current.str    + aura_bonus.str));
    nourstest_true(effective_stats.mag  == (silou->stats.current.mag    + aura_bonus.mag));
    nourstest_true(effective_stats.agi  == (silou->stats.current.agi    + aura_bonus.agi));
    nourstest_true(effective_stats.dex  == (silou->stats.current.dex    + aura_bonus.dex));
    nourstest_true(effective_stats.fth  == (silou->stats.current.fth    + aura_bonus.fth));
    nourstest_true(effective_stats.luck == (silou->stats.current.luck   + aura_bonus.luck));
    nourstest_true(effective_stats.def  == (silou->stats.current.def    + aura_bonus.def));
    nourstest_true(effective_stats.res  == (silou->stats.current.res    + aura_bonus.res));
    nourstest_true(effective_stats.con  == (silou->stats.current.con    + aura_bonus.con));
    nourstest_true(effective_stats.move == (silou->stats.current.move   + aura_bonus.move));
    nourstest_true(effective_stats.prof == (silou->stats.current.prof   + aura_bonus.prof));

    /* Move Friendly 1 outside */
    sota->selected.unit_initial_position.x  = 3;
    sota->selected.unit_initial_position.y  = 3;
    cursor_pos->tilemap_pos.x               = 0;
    cursor_pos->tilemap_pos.y               = 0;
    sota->selected.unit_entity              = sota->party.Es[UNIT_ID_SILOU];

    /* Reset bonuses */
    DARR_NUM(silou->stats.bonus_stack) = 0;
    SDL_assert(DARR_NUM(silou->stats.bonus_stack) == 0);

    /* Apply bonuses */
    fsm_eAcpt_sGmpMap_ssMapUnitMv(sota, TNECS_NULL);

    SDL_assert(erwin != silou);
    SDL_assert(erwin->stats.bonus_stack != silou->stats.bonus_stack);

    nourstest_true(DARR_NUM(silou->stats.bonus_stack) == 0);
    silou_pos = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Position);
    SDL_assert(silou_pos->tilemap_pos.x == sota->selected.unit_moved_position.x);
    SDL_assert(silou_pos->tilemap_pos.y == sota->selected.unit_moved_position.y);

    effective_stats   = Unit_effectiveStats(silou);

    nourstest_true(effective_stats.hp   == silou->stats.current.hp);
    nourstest_true(effective_stats.str  == silou->stats.current.str);
    nourstest_true(effective_stats.mag  == silou->stats.current.mag);
    nourstest_true(effective_stats.agi  == silou->stats.current.agi);
    nourstest_true(effective_stats.dex  == silou->stats.current.dex);
    nourstest_true(effective_stats.fth  == silou->stats.current.fth);
    nourstest_true(effective_stats.luck == silou->stats.current.luck);
    nourstest_true(effective_stats.def  == silou->stats.current.def);
    nourstest_true(effective_stats.res  == silou->stats.current.res);
    nourstest_true(effective_stats.con  == silou->stats.current.con);
    nourstest_true(effective_stats.move == silou->stats.current.move);
    nourstest_true(effective_stats.prof == silou->stats.current.prof);

    /* Quit game */
    Game_Free(sota);
    Names_Free();
    nourstest_true(true);
}

#undef TEST_SET_EQUIPMENT
