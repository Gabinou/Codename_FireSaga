
#include "nourstest.h"
#include "scene.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"


void test_scene() {
    s8 path = s8_literal(PATH_JOIN("scenes", "scene00001.json"));

    /* --- Init world --- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
#include "register_components.h"

    /* --- Read scene file --- */
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_Init(&scene, world);
    Scene_readJSON(&scene, jscene);

    /* --- Check statements num ---  */
    nourstest_true(DARR_NUM(scene.statements) == 5);
    // SDL_Log("DARR_NUM(scene.statements) %d", DARR_NUM(scene.statements));
    // getchar();

    /* --- Check each statement --- */
    /* -- Statement 0 -- */
    SceneStatement statement = scene.statements[0];
    SceneLine scene_line = scene.statements[0]._union.line;
    SceneDidascalie didascalie = scene.statements[0]._union.didascalie;
    nourstest_true(statement.type == SCENE_STATEMENT_DIDASCALIE);
    nourstest_true(didascalie.type == SCENE_DIDASCALIE_APPEAR);
    // nourstest_true(s8equal(scene_line.actor, s8_literal("Erwin")));
    // nourstest_true(s8equal(scene_line.line, s8_literal("Hello.")));

    /* -- Statement 1 -- */
    statement   = scene.statements[1];
    scene_line  = scene.statements[1]._union.line;
    nourstest_true(statement.type == SCENE_STATEMENT_LINE);
    nourstest_true(statement.actor_unit_id == UNIT_ID_ERWIN);
    nourstest_true(s8equal(scene_line.line, s8_literal("Hello.")));

    /* -- Statement 2 -- */
    statement   = scene.statements[2];
    scene_line  = scene.statements[2]._union.line;
    nourstest_true(statement.type == SCENE_STATEMENT_LINE);
    nourstest_true(statement.actor_unit_id == UNIT_ID_ERWIN);
    nourstest_true(s8equal(scene_line.line, s8_literal("Silou is actually alive!")));

    /* -- Statement 3 -- */
    statement   = scene.statements[3];
    didascalie  = scene.statements[3]._union.didascalie;
    nourstest_true(statement.type  == SCENE_STATEMENT_DIDASCALIE);
    nourstest_true(didascalie.type == SCENE_DIDASCALIE_SLIDE);

    /* -- Statement 4 -- */
    statement   = scene.statements[4];
    scene_line      = scene.statements[4]._union.line;
    nourstest_true(statement.type == SCENE_STATEMENT_LINE);
    nourstest_true(statement.actor_unit_id == UNIT_ID_SILOU);
    nourstest_true(s8equal(scene_line.line, s8_literal("I slid.")));

    /* Clean the jfile */
    Scene_Free(&scene);
    if (jfile != NULL)
        cJSON_Delete(jfile);
}