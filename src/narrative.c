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
    Scene_Free_Read(scene);
    if (scene->replace != NULL) {
        for (size_t i = 0; i < scene->replace_num; i++) {
            if (scene->replace[i] != NULL) {
                free(scene->replace[i]);
                scene->replace[i] = NULL;
            }
        }
        SDL_free(scene->replace);
        scene->replace = NULL;
    }
    if (scene->with != NULL) {
        for (size_t i = 0; i < scene->replace_num; i++) {
            if (scene->with[i] != NULL) {
                free(scene->with[i]);
                scene->with[i] = NULL;
            }
        }
        SDL_free(scene->with);
        scene->with = NULL;
    }
    scene->replace_num = 0;
}

void Scene_Free_Read(struct Scene *scene) {
    if (scene->lines != NULL) {
        for (size_t i = 0; i < scene->line_num; i++) {
            if (scene->lines[i] != NULL) {
                free(scene->lines[i]);
                scene->lines[i] = NULL;
            }
        }
        SDL_free(scene->lines);
        scene->lines = NULL;
    }
    if (scene->speakers != NULL) {
        SDL_free(scene->speakers);
        scene->speakers = NULL;
    }
    if (scene->actors != NULL) {
        SDL_free(scene->actors);
        scene->actors = NULL;
    }
    scene->actors_num = scene->line_num = scene->id = 0;
}

bool Condition_Read(u32 *bitfield, size_t bits, cJSON *jcondition) {
    SDL_assert(jcondition != NULL);
    bool out;
    cJSON *jelem;
    if (cJSON_IsObject(jcondition)) {
        jelem = jcondition->child;
        out = false;
        while (jelem != NULL) {
            // Recursive, for "or" conditions.
            // one level deep, goes into the else.
            out |= Condition_Read(bitfield, bits, jelem);
            jelem = jelem->next;
        }
    } else {
        // Non-recursive, runs after max one recursive call
        u32 *cond = BITFIELD_INIT_BITS(bits);
        if (cJSON_IsArray(jcondition)) {
            cJSON_ArrayForEach(jelem, jcondition) {
                char *name = cJSON_GetStringValue(jelem);
                i16 id = Hashes_unitName2ID(name);
                if (id > 0)
                    Bitfield_On(cond, id - 1);
            }
        } else if (cJSON_IsString(jcondition)) {
            char *name = cJSON_GetStringValue(jcondition);
            i16 id = Hashes_unitName2ID(name);
            if (id > 0)
                Bitfield_On(cond, id - 1);
        }
        out = Bitfield_isIn(cond, bitfield, bits / BITFIELD_BITSPERLEN);
        SDL_free(cond);
    }
    return (out);
}

void Scene_Prune(struct Scene *scene) {
    /* Realloc strings to size */
    SDL_assert(scene->replace_num > 0);
    for (i16 j = 0; j < scene->line_num; j++) {
        scene->lines[j] = SDL_realloc(scene->lines[j], strlen(scene->lines[j]) + 1);
    }
}

void Scene_Replace(struct Scene *scene) {
    /* Replaces strings in replace by with */
    SDL_assert(scene->replace_num > 0);
    for (i16 i = 0; i < scene->replace_num; i++) {
        for (i16 j = 0; j < scene->line_num; j++) {
            nstr_Replace(scene->lines[j], scene->replace[i], scene->with[i]);
        }
    }
}

bool Conditions_Read(struct Conditions *conds, cJSON *jconds) {
    SDL_assert(jconds != NULL);
    SDL_assert(conds != NULL);
    SDL_assert(conds->alive != NULL);
    SDL_assert(conds->dead != NULL);
    SDL_assert(conds->recruited != NULL);
    SDL_assert(conds->bits > 0);
    cJSON *jalive = cJSON_GetObjectItem(jconds, "Alive");
    cJSON *jdead = cJSON_GetObjectItem(jconds, "Dead");
    cJSON *jrecruited = cJSON_GetObjectItem(jconds, "Recruited");
    bool out = true;
    if (jalive)
        out &= Condition_Read(conds->alive, conds->bits, jalive);
    if (jdead)
        out &= Condition_Read(conds->dead, conds->bits, jdead);
    if (jrecruited)
        out &= Condition_Read(conds->recruited, conds->bits, jrecruited);
    return (out);
}

struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_conds,
                          i16 chapter, u16 scene_time) {
    /* scenes darr */
    if (sdarr != NULL)
        DARR_FREE(sdarr);
    sdarr = DARR_INIT(sdarr, struct Scene, 4);
    /* Creating base filename */
    char base[DEFAULT_BUFFER_SIZE] = {0}, filename[DEFAULT_BUFFER_SIZE] = {0};
    char extension[8] = {0};
    strcpy(base, "scenes"DIR_SEPARATOR);
    Filesystem_Scene_Chapter(base, chapter);
    strcat(base, sceneTimes[scene_time]);

    /* Reading scene files */
    struct cJSON *jscene, *jfile;
    for (i16 i = 1; i < DEFAULT_BUFFER_SIZE; i++) {
        stbsp_sprintf(extension, "%d.json", i);
        strcpy(filename, base);
        strcat(filename, extension);
        if (!PHYSFS_exists(filename))
            break;
        jfile = jsonio_parseJSON(filename);
        SDL_assert(jfile);
        jscene = cJSON_GetObjectItem(jfile, "Scene");
        SDL_assert(jscene);
        bool add_scene = true;
        cJSON *jconditions = cJSON_GetObjectItem(jscene, "Conditions");
        if (jconditions != NULL)
            add_scene &= Conditions_Read(scene_conds, jconditions);
        if (add_scene) {
            struct Scene temp = Scene_default;
            Scene_readJSON(&temp, jscene);
            DARR_PUT(sdarr, temp);
        }
        if (jfile != NULL)
            cJSON_Delete(jfile);
    }
    return (sdarr);
}

void Scene_readJSON(void *input, const struct cJSON *const jscene) {
    struct Scene *scene = (struct Scene *) input;
    Scene_Free_Read(scene);
    cJSON *jlines = cJSON_GetObjectItem(jscene, "Lines");
    i16 raw_line_num = cJSON_GetArraySize(jlines);
    scene->line_num = scene->actors_num = 0;
    scene->lines = SDL_malloc(raw_line_num * sizeof(*scene->lines));
    scene->actors = SDL_malloc(raw_line_num * sizeof(*scene->actors));
    scene->speakers = SDL_malloc(raw_line_num * sizeof(*scene->speakers));
    for (size_t i = 0; i < raw_line_num; i++) {
        cJSON *jline = cJSON_GetArrayItem(jlines, i);
        /* - speaker - */
        size_t len = strlen(jline->child->string);
        char *temp = calloc(len + 1, 1);
        strncpy(temp, jline->child->string, len);
        i16 id = Hashes_unitHash2ID(tnecs_hash_djb2(nstr_toLower(temp)));
        SDL_free(temp);
        /* - line - */
        temp = cJSON_GetStringValue(jline->child);
        len = strlen(temp);
        /* - conditions - */
        cJSON *jconditions = cJSON_GetObjectItem(jline, "Conditions");
        bool add_line = true;
        if (jconditions != NULL)
            add_line &= Conditions_Read(scene->line_conds, jconditions);
        if (add_line) {
            if (len >= scene->line_len)
                scene->line_len *= 2;
            scene->lines[scene->line_num] = SDL_calloc(scene->line_len, 1);
            strncpy(scene->lines[scene->line_num], temp, len);
            scene->lines[scene->line_num][len] = '\0';
            scene->speakers[scene->line_num++] = id;
            /* - participant -*/
            bool found = false;
            for (size_t i = 0; i < scene->actors_num; i++)
                found |= scene->actors[i] == id;
            if (!found)
                scene->actors[scene->actors_num++] = id;
        }
    }
    scene->lines = SDL_realloc(scene->lines, scene->line_num * sizeof(*scene->lines));
    scene->speakers = SDL_realloc(scene->speakers,
                                  scene->line_num * sizeof(*scene->speakers));
    scene->actors = SDL_realloc(scene->actors, scene->actors_num * sizeof(*scene->actors));
}
