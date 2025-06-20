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
    tnecs_entity inv_item = Unit_InvItem_Entity(unit, hand);
    /* -- Infusing weapon -- */
 
    /* -- Computing stats weapon -- */
    WeaponStatGet get = {
        .distance   = distance,
        .hand       = WEAPON_HAND_ONE,
        .infuse     = 1
    };
    // TODO: two handing.
    //  dodge should not stack!
    Weapon wpn = Weapon_default;
    
    get.stat = WEAPON_STAT_pATTACK;
    stat = Weapon_Stat(&wpn, get);

    /* -- Free -- */
    tnecs_world_destroy(&world);
    gl_world = NULL;
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
}