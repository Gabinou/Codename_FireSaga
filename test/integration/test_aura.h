#include "game/game.h"
#include "map/ontile.h"

void test_aura(int argc, char *argv[]) {
    /* -- Startup -- */
    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Game *sota       = SDL_malloc(sizeof(struct Game));
    *sota                   = Game_default;
    sota->settings          = Settings_default;
    sota->settings.window   = false;
    Game_Init(sota, argc, argv);
    nourstest_true(sota->state      == GAME_STATE_Title_Screen);
    nourstest_true(sota->substate   == GAME_SUBSTATE_MENU);

    /* Load Save file test/debug_map.json */
    Game_debugMap_Load(sota);
    Game_Map_Reinforcements_Load(sota);
    SDL_assert(DARR_NUM(sota->map->units_onfield) > 0);


    /* Load Standard */
    Weapon_Load(sota->weapons_dtab, ITEM_ID_IMPERIAL_STANDARD);

    /* -- Place all friendlies close together -- */
    i32 id;
    tnecs_entity ent;
    /* Place Standard bearer inside */
    struct Point pos = {4, 4};
    Game_Party_Entity_Create(sota, id = UNIT_ID_ERWIN, pos);
    tnecs_entity ent = sota->units_loaded[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->units_loaded[id] > TNECS_NULL);

    /* Give standard to standard bearer */
    struct Unit *bearer = TNECS_GET_COMPONENT(sota->world, ent, Unit);

    struct Inventory_item standard = Inventory_item_default;
    standard.id = ITEM_ID_IMPERIAL_STANDARD;
    Unit_Item_Take(&bearer, standard);

    /* Place Friendly 1 inside */
    pos.x = 3;
    pos.y = 3;
    Game_Party_Entity_Create(sota, id = UNIT_ID_SILOU, pos);
    tnecs_entity ent = sota->units_loaded[id];
    Map_Unit_Put(sota->map, pos.x, pos.y, ent);
    SDL_assert(sota->units_loaded[id] > TNECS_NULL);

    /* Place Friendly 2 inside */

    /* Place Friendly 3 outside */

    /* Check Aura in bonus stack */
    // nourstest_true(sota->map->win);
    // nourstest_true(!sota->map->loss);

    /* Check effective stats */

    /* Quit game */
    Game_Free(sota);
    nourstest_true(true);
}