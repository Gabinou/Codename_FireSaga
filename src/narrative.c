#include "narrative.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

struct Scene Scene_default =  {
    .json_element   = JSON_SCENE,
    .id             = 0,
    // .line_conds =    NULL,
    // .lines =         NULL,
    .lines_raw      = NULL,
    // .replace =       NULL,
    // .with =          NULL,
    .speakers       = NULL,
    // .actors =        NULL,
    // .line_num =      0,
    // .line_len =      DEFAULT_BUFFER_SIZE,
    .actors_num     = 0,
};

struct Conditions Conditions_default = {
    .alive      = {0},
    .recruited  = {0},
};

struct RawLine RawLine_default = {
    .conditions = {0},
    .speaker    = {0},
    .rawline    = {0},
};

struct Line Line_default = {
    .line    = {0},
    .speaker = {0},
};

void Scene_Free(struct Scene *scene) {
    SDL_assert(scene != NULL);
    if (scene->lines_raw != NULL) {
        for (size_t i = 0; i < scene->lines_raw_num; i++) {
            s8_free(&scene->lines_raw[i].rawline);
        }
        SDL_free(scene->lines_raw);
        scene->lines_raw = NULL;
    }

    if (scene->lines != NULL) {
        SDL_free(scene->lines);
        scene->lines = NULL;
    }
}

b32 Conditions_Compare(struct Conditions *conds1, struct Conditions *conds2) {
    size_t len_alive    = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_rec      = BITFIELD_LEN(UNIT_ORDER_PC_END);

    /* --  Are all conditions met? -- */
    b32 isalive = Bitfield_isIn(conds1->alive, conds2->alive, len_alive);
    b32 isrec   = Bitfield_isIn(conds1->recruited, conds2->recruited, len_rec);
    return (isalive && isrec);
}

/* --- Replace --- */
void Scene_Replace_Add(struct Scene *scene, s8 replace, s8 with) {

}

void Scene_Replace(struct Scene *scene) {

}

/* --- Rendering --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene) {
    scene->lines = SDL_calloc(scene->lines_raw_num, sizeof(*scene->lines));
    int rdr = 0;
    /* Check lines conditions */
    for (size_t i = 0; i < scene->lines_raw_num; i++) {
        if (Conditions_Compare(&scene->game_cond, &scene->lines_raw[i].conditions)) {
            scene->lines[rdr].line = scene->lines_raw[i].rawline;
            scene->lines[rdr].speaker = scene->lines_raw[i].speaker;
            rdr++;
        }

    }

    // Replace all tokens
}

/* --- I/O --- */
void Scene_readJSON(void *input, cJSON *jscene) {
    SDL_assert(jscene != NULL);
    struct Scene *scene = (struct Scene *)input;
    Scene_Free(scene);
    cJSON *jscene_conds = cJSON_GetObjectItem(jscene, "Conditions");
    cJSON *jrawlines    = cJSON_GetObjectItem(jscene, "Lines");
    SDL_assert(jrawlines != NULL);

    /* -- Read Scene conditions -- */
    if (jscene_conds)
        Conditions_readJSON(&scene->cond, jscene_conds);

    /* -- Read raw lines -- */
    scene->lines_raw_num = cJSON_GetArraySize(jrawlines);
    scene->lines_raw = SDL_calloc(scene->lines_raw_num, sizeof(*scene->lines_raw));
    for (size_t i = 0; i < scene->lines_raw_num; i++) {
        cJSON *jrawline         = cJSON_GetArrayItem(jrawlines, i);

        cJSON *jspeaker         = cJSON_GetObjectItem(jrawline, "Speaker");
        char  *speaker          = cJSON_GetStringValue(jspeaker);
        scene->lines_raw[i].speaker    = s8_var(speaker);

        cJSON *jline            = cJSON_GetObjectItem(jrawline, "Line");
        char  *line             = cJSON_GetStringValue(jline);
        scene->lines_raw[i].rawline    = s8_mut(line);

        cJSON *jline_cond       = cJSON_GetObjectItem(jrawline, "Conditions");
        Conditions_readJSON(&scene->lines_raw[i].conditions, jline_cond);
    }
}

void Scene_writeJSON(void *scene, cJSON *jscene) {

}

void Conditions_readJSON( void *input, cJSON *jconds) {
    struct Conditions *conds = (struct Conditions *)input;

}

void Conditions_writeJSON(void *input, cJSON *jconds) {
    struct Conditions *conds = (struct Conditions *)input;
    SDL_assert(conds    != NULL);
    SDL_assert(jconds   != NULL);
    SDL_assert(conds->alive     != NULL);
    SDL_assert(conds->recruited != NULL);

    cJSON *jalive       = cJSON_GetObjectItem(jconds, "Alive");
    cJSON *jrecruited   = cJSON_GetObjectItem(jconds, "Recruited");
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
        b32 add_scene = true;
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

/* --- Print --- */
void Scene_Raw_Print(struct Scene *scene) {
    SDL_Log("Raw scene: ");
    /* Find max length of name to print cols */
    struct RawLine *raws = scene->lines_raw;
    int max_name = 0;
    for (size_t i = 0; i < scene->lines_raw_num; i++) {
        if (max_name < raws[i].speaker.num)
            max_name = raws[i].speaker.num;
    }

    /* Print text lines in columns */
    for (size_t i = 0; i < scene->lines_raw_num; i++) {
        struct RawLine raw = raws[i];
        SDL_Log("%-*s : %s", max_name, raw.speaker.data, raw.rawline.data);
    }
}

void Scene_Render_Print(struct Scene *scene) {
    SDL_Log("Rendered Scene: ");
    struct Line *lines = scene->lines;
    int max_name = 0;
    for (size_t i = 0; i < scene->lines_num; i++) {
        if (max_name < lines[i].speaker.num)
            max_name = lines[i].speaker.num;
    }

    /* Print text lines in columns */
    for (size_t i = 0; i < scene->lines_raw_num; i++) {
        struct Line line = lines[i];
        SDL_Log("%-*s : %s", max_name, line.speaker.data, line.line.data);
    }
}

