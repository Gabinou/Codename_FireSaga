/*
**  Copyright 2023 Gabriel Taillon
**  Licensed under the GPLv3
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
**  Testing infusion component
**
*/
#include "game/game.h"
#include "systems/control.h"
#include "systems/map.h"
#include "systems/slide.h"
#include "systems/render.h"
#include "systems/time_system.h"
#include "systems/turn_end.h"
#include "structs.h"

void test_infusion() {
    /* -- Startup -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    gl_world = world;

    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    /* -- Registering -- */
#include "register/components.h"
#include "register/pipelines.h"
#include "register/phases.h"
#include "register/systems.h"

    /* -- Creating weapon -- */
    tnecs_entity inv_item = TNECS_ENTITY_CREATE_wCOMPONENTS(gl_world, Inventory_item_ID, Infusion_ID);
    Inventory_item  *item   = IES_GET_COMPONENT(gl_world, inv_item, Inventory_item);
    item->id = ITEM_ID_GLADIUS;
    Infusion        *infusion   = IES_GET_COMPONENT(gl_world, inv_item, Infusion);
    nourstest_true(infusion->physical == 0);
    nourstest_true(infusion->magical == 0);

    Weapon_Load(gl_weapons_dtab, item->id);
    Weapon *weapon = DTAB_GET(gl_weapons_dtab, item->id);
    /* -- Infusing weapon -- */
    infusion->physical  = 3;
    infusion->magical   = 4;

    /* -- Computing stats weapon -- */
    /* - Attacking weapon - */
    WeaponStatGet get = {
        .distance   = DISTANCE_INVALID,
        .hand       = WEAPON_HAND_ONE,
        .infuse     = 1,
        .infusion   = infusion
    };

    SDL_assert(infusion->magical > 0);
    SDL_assert(infusion->magical > 0);

    get.stat = WEAPON_STAT_pATTACK;
    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.attack.physical + infusion->physical));
    get.stat = WEAPON_STAT_mATTACK;
    nourstest_true(_Weapon_Stat_Hand(weapon, get) == weapon->stats.attack.magical);
    nourstest_true(_Weapon_Infusion(weapon, get) == infusion->magical);
    nourstest_true(_Weapon_inRange(weapon, get));

    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.attack.magical + infusion->magical));

    get.stat = WEAPON_STAT_pPROTECTION;
    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.protection.physical));
    get.stat = WEAPON_STAT_mPROTECTION;
    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.protection.magical));

    /* - Shield - */
    item->id = ITEM_ID_WOODEN_SHIELD;
    Weapon_Load(gl_weapons_dtab, item->id);
    weapon = DTAB_GET(gl_weapons_dtab, item->id);

    SDL_assert(infusion->magical > 0);
    SDL_assert(infusion->magical > 0);

    get.stat = WEAPON_STAT_pATTACK;
    SDL_Log("%d %d", Weapon_Stat(weapon, get), (weapon->stats.attack.physical));
    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.attack.physical));
    get.stat = WEAPON_STAT_mATTACK;
    nourstest_true(_Weapon_Stat_Hand(weapon, get) == weapon->stats.attack.magical);
    nourstest_true(_Weapon_Infusion(weapon, get) == infusion->magical);
    nourstest_true(_Weapon_inRange(weapon, get));

    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.attack.magical));

    get.stat = WEAPON_STAT_pPROTECTION;
    nourstest_true(Weapon_Stat(weapon,
                               get) == (weapon->stats.protection.physical + infusion->physical));
    get.stat = WEAPON_STAT_mPROTECTION;
    nourstest_true(Weapon_Stat(weapon, get) == (weapon->stats.protection.magical + infusion->magical));


    /* -- Free -- */
    tnecs_world_destroy(&world);
    gl_world = NULL;
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
}