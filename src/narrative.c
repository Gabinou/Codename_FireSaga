#include "narrative.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

struct Scene Scene_default =  {
    .json_element =  JSON_SCENE,
    .id =            0,
    .line_conds =    NULL,
    .lines =         NULL,
    .replace =       NULL,
    .with =          NULL,
    .speakers =      NULL,
    .actors =        NULL,
    .line_num =      0,
    .line_len =      DEFAULT_BUFFER_SIZE,
    .actors_num =    0,
};

void Scene_Free(struct Scene *scene) {
}

bool Conditions_Compare(struct Conditions *conds1, struct Conditions *conds2) {

}

void Conditions_Read(struct Conditions *conds, cJSON *jconds) {
    SDL_assert(conds    != NULL);
    SDL_assert(jconds   != NULL);
    SDL_assert(conds->dead      != NULL);
    SDL_assert(conds->alive     != NULL);
    SDL_assert(conds->recruited != NULL);

    cJSON *jalive       = cJSON_GetObjectItem(jconds, "Alive");
    cJSON *jdead        = cJSON_GetObjectItem(jconds, "Dead");
    cJSON *jrecruited   = cJSON_GetObjectItem(jconds, "Recruited");
}

/* --- Replace --- */
void Scene_Replace_Add(struct Scene *scene, s8 replace, s8 with) {

}

void Scene_Replace(struct Scene *scene) {

}

/* --- I/O --- */
void Scene_readJSON(void *scene, cJSON *jscene) {

}

void Scene_writeJSON(void *u, cJSON *junit) {

}

struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_conds,
                          i16 chapter, u16 scene_time) {
    /* scenes darr */
    if (sdarr != NULL)
        DARR_FREE(sdarr);
    sdarr = DARR_INIT(sdarr, struct Scene, 4);
    /* Creating base filename */
    s8 filename     = s8_mut("");
    char extension[8] = {0};
    s8 base         = s8_mut("scenes"DIR_SEPARATOR);
    base            = Filesystem_Scene_Chapter(base, chapter);
    base            = s8cat(base, s8_var(sceneTimes[scene_time].data));

    /* Reading scene files */
    struct cJSON *jscene, *jfile;
    for (i16 i = 1; i < DEFAULT_BUFFER_SIZE; i++) {
        stbsp_sprintf(extension, "%d.json", i);
        s8cpy(filename, base);
        s8cat(filename, s8_var(extension));
        if (!PHYSFS_exists(filename.data))
            break;
        jfile = jsonio_parseJSON(filename);
        SDL_assert(jfile);
        jscene = cJSON_GetObjectItem(jfile, "Scene");
        SDL_assert(jscene);
        bool add_scene = true;
        cJSON *jconditions = cJSON_GetObjectItem(jscene, "Conditions");

        // if (jconditions != NULL)
        //     add_scene &= Conditions_Read(scene_conds, jconditions);
        // if (add_scene) {
        //     struct Scene temp = Scene_default;
        //     Scene_readJSON(&temp, jscene);
        //     DARR_PUT(sdarr, temp);
        // }
        // if (jfile != NULL)
        //     cJSON_Delete(jfile);
    }

    s8_free(&filename);
    s8_free(&base);
    return (sdarr);
}
