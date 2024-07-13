#include "nourstest.h"
#include "scene.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_scene() {
    SDL_Log("test_scene");
    s8 path = s8_literal(PATH_JOIN("scenes", "example.json"));

    /* --- Read scene file --- */
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_readJSON(&scene, jscene);


    /* -- Scene 1: nothing -- */
    // Scene_Render(&scene);

    // /* -- Scene 2: Otto Recruited -- */
    // Conditions_Recruited_Order(&(scene.game_cond), UNIT_ORDER_OTTO); /* Line 4 */

    // // nourstest_true(!Conditions_Compare(&line_cond, &scene.game_cond));
    // nourstest_true(Conditions_Compare(&(scene.lines_raw[4].conditions), &scene.game_cond));
    // nourstest_true(Bitfield_Get(scene.lines_raw[4].conditions.recruited, UNIT_ORDER_OTTO));
    // nourstest_true(Bitfield_Get(scene.lines_raw[4].conditions.recruited, UNIT_ORDER_OTTO));
    // nourstest_true(Bitfield_Get(scene.game_cond.recruited, UNIT_ORDER_OTTO) > 0);
    // nourstest_true(!Bitfield_Get(scene.game_cond.recruited, UNIT_ORDER_SILOU));

    // Scene_Render(&scene);
    // nourstest_true(Conditions_Compare(&(scene.lines_raw[4].conditions), &scene.game_cond));
    // nourstest_true(scene.lines_raw_num == 6);
    // nourstest_true(scene.lines_num == 4);
    // nourstest_true(scene.rendered[0] == 1);
    // nourstest_true(scene.rendered[1] == 2);
    // nourstest_true(scene.rendered[2] == 4);
    // nourstest_true(scene.rendered[3] == 5);

    /* -- Scene 3: Silou Recruited, Hamilcar Dead -- */
    // scene.game_cond = Conditions_Game_start;
    // Conditions_Recruited_Order(&(scene.game_cond), UNIT_ORDER_SILOU);
    // Conditions_Dead_Order(&(scene.game_cond), UNIT_ORDER_HAMILCAR);

    // Scene_Render(&scene);
    // nourstest_true(scene.lines_num == 4);
    // nourstest_true(scene.rendered[0] == 1);
    // nourstest_true(scene.rendered[1] == 2);
    // nourstest_true(scene.rendered[2] == 3);
    // nourstest_true(scene.rendered[3] == 5);

    /* -- Scene 4: Silou Recruited, Hamilcar Dead, Zidine Dead -- */
    // scene.game_cond = Conditions_Game_start;
    // Conditions_Recruited_Order(&(scene.game_cond), UNIT_ORDER_SILOU);
    // Conditions_Dead_Order(&(scene.game_cond), UNIT_ORDER_HAMILCAR);
    // Conditions_Dead_Order(&(scene.game_cond), UNIT_ORDER_ZIDINE);
    // Scene_Render(&scene);
    // nourstest_true(scene.lines_num == 5);
    // nourstest_true(scene.rendered[0] == 0);
    // nourstest_true(scene.rendered[1] == 1);
    // nourstest_true(scene.rendered[2] == 2);
    // nourstest_true(scene.rendered[3] == 3);
    // nourstest_true(scene.rendered[4] == 5);

    /* -- Scene 5: All -- */
    // scene.game_cond = Conditions_Game_start;
    // Conditions_Recruited_Order(&(scene.game_cond), UNIT_ORDER_SILOU);
    // Conditions_Recruited_Order(&(scene.game_cond), UNIT_ORDER_OTTO);
    // Conditions_Dead_Order(&(scene.game_cond), UNIT_ORDER_HAMILCAR);
    // Conditions_Dead_Order(&(scene.game_cond), UNIT_ORDER_ZIDINE);
    // Scene_Render(&scene);
    // nourstest_true(scene.lines_num == 6);
    // nourstest_true(scene.rendered[0] == 0);
    // nourstest_true(scene.rendered[1] == 1);
    // nourstest_true(scene.rendered[2] == 2);
    // nourstest_true(scene.rendered[3] == 3);
    // nourstest_true(scene.rendered[4] == 4);
    // nourstest_true(scene.rendered[5] == 5);

    /* -- Output replace -- */
    // Scene_Render_Print(&scene);

    /* Clean the jfile */
    Scene_Free(&scene);
    if (jfile != NULL)
        cJSON_Delete(jfile);
}