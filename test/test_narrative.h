#include "nourstest.h"
#include "narrative.h"
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
    nourstest_true(scene.lines_raw_num == 6);
    // Scene_Raw_Print(&scene);
    // getchar();

    /* - Line 0 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[0].conditions.alive, UNIT_ORDER_ERWIN));
    nourstest_true(!Bitfield_Get(scene.lines_raw[0].conditions.alive, UNIT_ORDER_SILOU));

    nourstest_true(Bitfield_Get(scene.lines_raw[0].conditions.recruited, UNIT_ORDER_SILOU));
    nourstest_true(!Bitfield_Get(scene.lines_raw[0].conditions.recruited, UNIT_ORDER_ERWIN));

    nourstest_true(Bitfield_Get(scene.lines_raw[0].conditions.dead, UNIT_ORDER_HAMILCAR));
    nourstest_true(Bitfield_Get(scene.lines_raw[0].conditions.dead, UNIT_ORDER_ZIDINE));
    nourstest_true(!Bitfield_Get(scene.lines_raw[0].conditions.dead, UNIT_ORDER_ERWIN));
    nourstest_true(!Bitfield_Get(scene.lines_raw[0].conditions.dead, UNIT_ORDER_SILOU));

    /* - Line 1 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[1].conditions.alive, UNIT_ORDER_OTTO));
    nourstest_true(!Bitfield_Get(scene.lines_raw[1].conditions.alive, UNIT_ORDER_SILOU));

    /* - Line 2 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[2].conditions.alive, UNIT_ORDER_SEBASTIAN));
    nourstest_true(!Bitfield_Get(scene.lines_raw[2].conditions.alive, UNIT_ORDER_SILOU));

    /* - Line 3 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[3].conditions.alive, UNIT_ORDER_ERWIN));
    nourstest_true(Bitfield_Get(scene.lines_raw[3].conditions.alive, UNIT_ORDER_SILOU));
    nourstest_true(Bitfield_Get(scene.lines_raw[3].conditions.recruited, UNIT_ORDER_SILOU));
    nourstest_true(Bitfield_Get(scene.lines_raw[3].conditions.dead, UNIT_ORDER_HAMILCAR));

    /* - Line 4 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[4].conditions.alive, UNIT_ORDER_KIARA));
    nourstest_true(Bitfield_Get(scene.lines_raw[4].conditions.alive, UNIT_ORDER_SILOU));
    nourstest_true(Bitfield_Get(scene.lines_raw[4].conditions.recruited, UNIT_ORDER_OTTO));

    /* - Line 5 - */
    nourstest_true(Bitfield_Get(scene.lines_raw[5].conditions.alive, UNIT_ORDER_SEBASTIAN));
    nourstest_true(!Bitfield_Get(scene.lines_raw[5].conditions.alive, UNIT_ORDER_SILOU));

    /* --- Render scene --- */

    /* -- Scene 1: nothing -- */
    scene.game_cond = Conditions_Game_default;
    Scene_Render(&scene);
    nourstest_true(scene.lines_num == 3);
    Scene_Render_Print(&scene);
    getchar();

    /* -- Scene 2 -- */
    Conditions_Recruited(&scene.game_cond, UNIT_ORDER_SILOU); /* Line 0, 3 */

    /* --- Output Render --- */

}

void test_conditions() {
    SDL_Log("test_conditions");
    struct Conditions game_cond = Conditions_Game_default;
    struct Conditions line_cond = Conditions_Line_default;

    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Dead(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    game_cond = Conditions_Game_default;
    line_cond = Conditions_Line_default;

    /* Line should play if Silou is alive, but she is dead: False*/
    Conditions_Alive(&line_cond, UNIT_ORDER_SILOU);
    Conditions_Dead(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    game_cond = Conditions_Game_default;
    line_cond = Conditions_Line_default;

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead(&game_cond, UNIT_ORDER_NICOLE);
    Conditions_Dead(&game_cond, UNIT_ORDER_OTTO);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Dead(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    game_cond = Conditions_Game_default;
    line_cond = Conditions_Line_default;

    /* Line should play if Silou was recruited, but she wasn't: False*/
    Conditions_Recruited(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));

    /* Line should play if Silou was recruited, and she was: True*/
    Conditions_Recruited(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    /* Line doesn't care if Silou died: True*/
    Conditions_Dead(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Compare(&line_cond, &game_cond));

    /* Line cares if Silou died: False*/
    Conditions_Alive(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Compare(&line_cond, &game_cond));
}

void test_narrative() {
    test_scene();
    test_conditions();
}