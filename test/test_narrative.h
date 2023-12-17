#include "nourstest.h"
#include "narrative.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_narrative() {
    s8 path = s8_literal(PATH_JOIN("scenes", "example.json"));
    struct cJSON *jfile = jsonio_parseJSON(path);
    SDL_assert(jfile);
    struct cJSON *jscene = cJSON_GetObjectItem(jfile, "Scene");

    struct Scene scene = Scene_default;
    Scene_readJSON(&scene, jscene);

    /* --- Read scene file --- */

    /* --- Render scene --- */

    /* --- Output Render --- */

}
