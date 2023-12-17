#include "nourstest.h"
#include "narrative.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_narrative() {
    s8 path = s8_literal(PATH_JOIN("scenes", "example.json"));

    /* --- Read scene file --- */
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_readJSON(&scene, jscene);
    nourstest_true(scene.lines_raw_num == 6);

    for (size_t i = 0; i < scene.lines_raw_num; i++) {
        SDL_Log("%s: %s", scene.lines_raw[i].speaker.data, scene.lines_raw[i].rawline.data);
    }

    getchar();



    /* --- Render scene --- */

    /* --- Output Render --- */

}
