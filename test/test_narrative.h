#include "nourstest.h"
#include "narrative.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"


void test_scene() {
    s8 path = s8_literal(PATH_JOIN("scenes", "example.json"));

    /* --- Read scene file --- */
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_readJSON(&scene, jscene);
    nourstest_true(scene.lines_raw_num == 6);
    Scene_Raw_Print(&scene);
    // getchar();

    /* --- Render scene --- */

    /* --- Output Render --- */

}

void test_conditions() {
    struct Conditions game_cond = Conditions_default;
    struct Conditions line_cond = Conditions_default;

    nourstest_true(Conditions_Compare(&game_cond, &line_cond));
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    Conditions_Death(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));
}

void test_narrative() {
    test_scene();
    test_conditions();
}