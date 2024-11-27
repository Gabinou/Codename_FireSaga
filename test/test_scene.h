#include "nourstest.h"
#include "scene.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_scene() {
    s8 path = s8_literal(PATH_JOIN("scenes", "scene00001.json"));

    /* --- Read scene file --- */
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_readJSON(&scene, jscene);

    /* --- Check statements num ---  */
    nourstest_true(DARR_NUM(scene.statements) == 4);

    /* --- Check each statement --- */
    /* -- Statement 0 -- */
    SceneHeader scene_header = scene.statements[0].header;
    SDL_Log("scene_header.statement_type %d", scene_header.statement_type);
    nourstest_true(scene_header.statement_type == SCENE_STATEMENT_LINE);

    /* -- Statement 1 -- */
    scene_header = scene.statements[1].header;
    nourstest_true(scene_header.statement_type == SCENE_STATEMENT_LINE);

    /* -- Statement 2 -- */
    scene_header = scene.statements[2].header;
    nourstest_true(scene_header.statement_type == SCENE_STATEMENT_DIDASCALIE);
    nourstest_true(scene_header.didascalie_type == SCENE_DIDASCALIE_SLIDE);

    /* -- Statement 3 -- */
    scene_header = scene.statements[3].header;
    nourstest_true(scene_header.statement_type == SCENE_STATEMENT_LINE);

    /* Clean the jfile */
    Scene_Free(&scene);
    if (jfile != NULL)
        cJSON_Delete(jfile);
}