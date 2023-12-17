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
    struct Conditions game_cond = Conditions_Game_default;
    struct Conditions line_cond = Conditions_Line_default;

    nourstest_true(Conditions_Compare(&game_cond, &line_cond));
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Death(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Death(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    game_cond = Conditions_Game_default;
    line_cond = Conditions_Line_default;

    /* Line should play if Silou is alive, but she is dead: False*/
    Conditions_Death(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    game_cond = Conditions_Game_default;
    line_cond = Conditions_Line_default;

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Death(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Death(&game_cond, UNIT_ORDER_NICOLE);
    Conditions_Death(&game_cond, UNIT_ORDER_OTTO);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Death(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));
}

void test_narrative() {
    test_scene();
    test_conditions();
}