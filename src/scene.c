#include "scene.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION


struct Conditions Conditions_Line_default = {
    .alive      = {0},
    .dead       = {0},
    .recruited  = {0},
};

struct Conditions Conditions_Game_default = {
    .alive      = {0xFFFFFFFF}, /* Everyone alive at start      */
    .dead       = {0},          /* No one dead at start         */
    .recruited  = {0},          /* No one recruited at start    */
};

struct Scene Scene_default =  {
    //     .json_element   = JSON_SCENE,
    //     .lines_raw      = NULL,
    //     .with           = NULL,
    //     .speakers       = NULL,
    //     .actors_num     = 0,
};


void Scene_Init(struct Scene *scene) {
    SDL_assert(scene != NULL);
    scene->statements   = DARR_INIT(scene->statements, void *, 16);
    scene->actors       = DARR_INIT(scene->statements, u16, 16);
}

void Scene_Free(struct Scene *scene) {
    if (scene == NULL)
        return;

    if (scene->statements != NULL) {
        DARR_FREE(scene->statements);
        scene->statements = NULL;
    }
    if (scene->actors != NULL) {
        DARR_FREE(scene->actors);
        scene->actors = NULL;
    }
}

void Conditions_Dead_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Dead(cond, order);
}

void Conditions_Alive_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Alive(cond, order);
}

void Conditions_Recruited_char(struct Conditions *cond, char *name) {
    s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    int order  = Unit_Name2Order(namestr);
    s8_free(&namestr);
    SDL_assert(order >= 0);
    Conditions_Recruited(cond, order);
}

void Conditions_Dead(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->dead, unit_order);
    Bitfield_Off(cond->alive, unit_order);
}

void Conditions_Alive(struct Conditions *cond, size_t unit_order) {
    Bitfield_On( cond->alive, unit_order);
    Bitfield_Off(cond->dead, unit_order);
}

void Conditions_Recruited(struct Conditions *cond, size_t unit_order) {
    Bitfield_On(cond->recruited, unit_order);
}

/* --  Are all conditions in conds1 satisfied in conds2? -- */
b32 Conditions_Compare(struct Conditions *line_cond, struct Conditions *game_cond) {
    size_t len_alive    = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_dead     = BITFIELD_LEN(UNIT_ORDER_NPC_END);
    size_t len_rec      = BITFIELD_LEN(UNIT_ORDER_PC_END);

    /* --  Are all conditions met? -- */
    b32 isalive = Bitfield_isIn(line_cond->alive,     game_cond->alive,     len_alive);
    b32 isdead  = Bitfield_isIn(line_cond->dead,      game_cond->dead,      len_dead);
    b32 isrec   = Bitfield_isIn(line_cond->recruited, game_cond->recruited, len_rec);
    return (isdead && isalive && isrec);
}

// /* --- Replace --- */
// void Scene_Replace_Add(struct Scene *scene, s8 with) {
//     if (scene->with == NULL) {
//         scene->with = DARR_INIT(scene->with, s8, 4);
//     }
//     DARR_PUT(scene->with, with);
// }

// void Scene_Replace(struct Scene *scene) {

// }

// /* --- Rendering --- */
// /* Read game condition and render text lines */
// void Scene_Render(struct Scene *scene) {
//     if (scene->lines != NULL) {
//         SDL_free(scene->lines);
//         scene->lines = NULL;
//     }
//     if (scene->rendered != NULL) {
//         SDL_free(scene->rendered);
//         scene->rendered = NULL;
//     }
//     if (scene->rendered != NULL) {
//         SDL_free(scene->rendered);
//         scene->rendered = NULL;
//     }
//     if (scene->speakers != NULL) {
//         SDL_free(scene->speakers);
//         scene->speakers = NULL;
//     }

//     scene->lines    = SDL_calloc(scene->lines_raw_num, sizeof(*scene->lines));
//     scene->rendered = SDL_calloc(scene->lines_raw_num, sizeof(*scene->rendered));
//     scene->speakers = SDL_calloc(scene->lines_raw_num, sizeof(*scene->rendered));
//     scene->lines_num = 0;
//     /* Check lines conditions */
//     for (size_t i = 0; i < scene->lines_raw_num; i++) {
//         if (Conditions_Compare(&scene->lines_raw[i].conditions, &scene->game_cond)) {
//             scene->lines[scene->lines_num].line     = scene->lines_raw[i].rawline;
//             scene->lines[scene->lines_num].speaker  = scene->lines_raw[i].speaker;
//             Scene_Speaker_Add(scene, scene->lines_raw[i].speaker_order);

//             scene->rendered[scene->lines_num]       = i;
//             scene->lines_num++;
//         }
//     }
//     /* Replace all tokens */
//     if (scene->with == NULL)
//         return;

//     char replace[DEFAULT_BUFFER_SIZE];
//     struct Line *lines = scene->lines;
//     for (size_t i = 0; i < scene->lines_num; i++) {
//         for (size_t j = 0; j < DARR_NUM(scene->with); j++) {
//             memset(replace, 0, DEFAULT_BUFFER_SIZE);
//             stbsp_snprintf(replace,   DEFAULT_BUFFER_SIZE, "REPLACE_TOKEN_%d", j);
//             lines[i].line = s8_Replace(lines[i].line, replace, scene->with[j].data);
//         }
//     }
// }

// /* --- I/O --- */
void Scene_readJSON(void *input, cJSON *jscene) {
    //     SDL_assert(jscene != NULL);
    //     struct Scene *scene = (struct Scene *)input;
    //     Scene_Free(scene);
    //     cJSON *jscene_conds = cJSON_GetObjectItem(jscene, "Conditions");
    //     cJSON *jrawlines    = cJSON_GetObjectItem(jscene, "Lines");
    //     SDL_assert(jrawlines != NULL);
    //     if (!cJSON_IsArray(jrawlines)) {
    //         SDL_Log("Lines object should be a JSON array.");
    //         exit(ERROR_Generic);
    //     }

    //     /* -- Read Scene conditions -- */
    //     if (jscene_conds)
    //         Conditions_readJSON(&scene->cond, jscene_conds);

    //     /* -- Read raw lines -- */
    //     scene->lines_raw_num = cJSON_GetArraySize(jrawlines);
    //     scene->lines_raw = SDL_calloc(scene->lines_raw_num, sizeof(*scene->lines_raw));
    //     for (size_t i = 0; i < scene->lines_raw_num; i++) {
    //         cJSON *jrawline         = cJSON_GetArrayItem(jrawlines, i);

    //         cJSON *jspeaker         = cJSON_GetObjectItem(jrawline, "Speaker");
    //         char  *speaker          = cJSON_GetStringValue(jspeaker);
    //         scene->lines_raw[i].speaker    = s8_var(speaker);

    //         cJSON *jline            = cJSON_GetObjectItem(jrawline, "Line");
    //         char  *line             = cJSON_GetStringValue(jline);
    //         scene->lines_raw[i].rawline    = s8_mut(line);

    //         cJSON *jline_cond       = cJSON_GetObjectItem(jrawline, "Conditions");
    //         Conditions_readJSON(&scene->lines_raw[i].conditions, jline_cond);

    //         /* -- Get speaker unit order -- */
    //         s8 speakerstr = s8_camelCase(s8_toLower(s8_mut(speaker)), ' ', 2);
    //         u16 order = Unit_Name2Order(speakerstr);
    //         SDL_assert(order >= 0);
    //         scene->lines_raw[i].speaker_order = order;
    //         s8_free(&speakerstr);

    //         /* -- Speaker should always be alive -- */
    //         Bitfield_On(scene->lines_raw[i].conditions.alive, order);
    //     }
    // }

    // void Scene_writeJSON(void *scene, cJSON *jscene) {
    //     // If necessary...
    // }

    // void Condition_Array_Read(u32 *array, cJSON *jarray) {
    //     size_t num = cJSON_GetArraySize(jarray);

    //     for (int i = 0; i < num; i++) {
    //         cJSON *jname = cJSON_GetArrayItem(jarray, i);
    //         char  *name  = cJSON_GetStringValue(jname);
    //         s8 namestr = s8_camelCase(s8_toLower(s8_mut(name)), ' ', 2);
    //         int order  = Unit_Name2Order(namestr);
    //         s8_free(&namestr);
    //         SDL_assert(order >= 0);
    //         Bitfield_On(array, order);
    //     }
}

// void Conditions_readJSON( void *input, cJSON *jconds) {
//     struct Conditions *conds = (struct Conditions *)input;

//     cJSON *jdead        = cJSON_GetObjectItem(jconds, "Dead");
//     cJSON *jalive       = cJSON_GetObjectItem(jconds, "Alive");
//     cJSON *jrecruited   = cJSON_GetObjectItem(jconds, "Recruited");

//     if (jdead != NULL) {
//         if (!cJSON_IsArray(jdead)) {
//             SDL_Log("Dead object should be a JSON array.");
//             exit(ERROR_Generic);
//         }
//         Condition_Array_Read(conds->dead, jdead);
//     }

//     if (jalive != NULL) {
//         if (!cJSON_IsArray(jalive)) {
//             SDL_Log("Alive object should be a JSON array.");
//             exit(ERROR_Generic);
//         }
//         Condition_Array_Read(conds->alive, jalive);
//     }

//     if (jrecruited != NULL) {
//         if (!cJSON_IsArray(jrecruited)) {
//             SDL_Log("Recruited object should be a JSON array.");
//             exit(ERROR_Generic);
//         }
//         Condition_Array_Read(conds->recruited, jrecruited);
//     }
// }

// void Conditions_writeJSON(void *input, cJSON *jconds) {
//     struct Conditions *conds = (struct Conditions *)input;
//     SDL_assert(conds    != NULL);
//     SDL_assert(jconds   != NULL);
//     SDL_assert(conds->alive     != NULL);
//     SDL_assert(conds->recruited != NULL);

// }

// struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_conds,
//                           i16 chapter, u16 scene_time) {
//     /* Scenes darr */
//     if (sdarr != NULL)
//         DARR_FREE(sdarr);
//     sdarr = DARR_INIT(sdarr, struct Scene, 4);
//     /* Creating base filename */
//     s8 filename     = s8_mut("");
//     char extension[8] = {0};
//     s8 base         = s8_mut("scenes"DIR_SEPARATOR);
//     base            = Filesystem_Scene_Chapter(base, chapter);
//     base            = s8cat(base, s8_var(sceneTimes[scene_time].data));

//     /* Reading scene files */
//     struct cJSON *jscene, *jfile;
//     for (i16 i = 1; i < DEFAULT_BUFFER_SIZE; i++) {
//         // TODO: remake batch scene function
//         stbsp_sprintf(extension, "%d.json", i);
//         s8cpy(filename, base);
//         s8cat(filename, s8_var(extension));
//         if (!PHYSFS_exists(filename.data))
//             break;
//         jfile = jsonio_parseJSON(filename);
//         SDL_assert(jfile);
//         jscene = cJSON_GetObjectItem(jfile, "Scene");
//         SDL_assert(jscene);
//         struct Scene scene;
//         Scene_readJSON(&scene,  jscene);
//         DARR_PUT(sdarr, scene);
//     }

//     s8_free(&filename);
//     s8_free(&base);
//     return (sdarr);
// }

// /* --- Speakers --- */
// void Scene_Speaker_Add(struct Scene *scene, u16 order) {
//     int index = 0;
//     for (int i = 0; i < scene->lines_raw_num; i++) {
//         /* Skip if order found in speakers */
//         if (order == scene->speakers[i])
//             return;

//         /* Set index to greatest free spot in speakers array */
//         if ((scene->speakers[i] > 0) && (index < 1))
//             index = i;
//     }
//     SDL_assert(index >= 0);
//     SDL_assert(index < scene->lines_raw_num);
//     scene->speakers[index] = order;
// }

// /* --- Print --- */
// void Scene_Raw_Print(struct Scene *scene) {
//     SDL_Log("Raw scene: ");
//     /* Find max length of name to print cols */
//     struct RawLine *raws = scene->lines_raw;
//     int max_name = 0;
//     for (size_t i = 0; i < scene->lines_raw_num; i++) {
//         if (max_name < raws[i].speaker.num)
//             max_name = raws[i].speaker.num;
//     }

//     /* Print text lines in columns */
//     for (size_t i = 0; i < scene->lines_raw_num; i++) {
//         struct RawLine raw = raws[i];
//         SDL_Log("%-*s : %s", max_name, raw.speaker.data, raw.rawline.data);
//     }
// }

// void Scene_Render_Print(struct Scene *scene) {
//     SDL_Log("Rendered Scene: ");
//     struct Line *lines = scene->lines;
//     int max_name = 0;
//     for (size_t i = 0; i < scene->lines_num; i++) {
//         if (max_name < lines[i].speaker.num)
//             max_name = lines[i].speaker.num;
//     }

//     /* Print text lines in columns */
//     for (size_t i = 0; i < scene->lines_num; i++) {
//         struct Line line = lines[i];
//         SDL_Log("%-*s : %s", max_name, line.speaker.data, line.line.data);
//     }
// }

// /* --- Player interaction --- */
void Scene_Finish(struct Scene *scene, struct Game *sota) {
    /* - Finish scene - */
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Scene Animation Finished");
    SDL_assert((scene->event > event_Start) && (scene->event < event_End));
    Event_Emit(__func__, SDL_USEREVENT, scene->event, NULL, NULL);
}

void Scene_Next_Line(struct Scene *scene, struct Game *sota) {
    // TODO: get next line if more to go
    /* - Skip current line if in the middle fo rendering - */

    /* - No more lines - */
    Scene_Finish(scene, sota);
}

// /* --- Animate --- */
// void _Scene_Animate_Actors(        struct Scene *scene) {

// }
// void _Scene_Animate_Background(    struct Scene *scene) {

// }
// void _Scene_Animate_Text_Bubbles(  struct Scene *scene) {

// }

void Scene_Animate(struct Game  *sota, tnecs_entity entity,
                   struct Scene *scene, struct Timer *timer) {
    //     /* - Change frame to be drawn each frame - */

    //     _Scene_Animate_Background(scene);
    //     _Scene_Animate_Actors(scene);
    //     _Scene_Animate_Text_Bubbles(scene);
}

void Scene_Actor_Add(Scene *scene, u16 actor) {
    SDL_assert(scene != NULL);
    /* Do not add new actor if found */
    if (Scene_Actor_Find(scene, actor) >= 0) {
        return;
    }

    /* Do not add new actor if too many actors already */
    if (DARR_NUM(scene->actors) >= SCENE_MAX_ACTORS) {
        return;
    }

    /* Add new actor */
    DARR_PUT(scene->actors, actor);
}

i32 Scene_Actor_Find(Scene *scene, u16 actor) {
    SDL_assert(scene != NULL);
    i32 found = -1;
    for (i32 i = 0; i < DARR_NUM(scene->actors); i++) {
        if (scene->actors[i] == actor) {
            found = i;
            break;
        }
    }
    return (found);
}

// /* --- Draw --- */
// void _Scene_Draw_Actors(        struct Scene *scene, SDL_Renderer *renderer) {

// }
// void _Scene_Draw_Background(    struct Scene *scene, SDL_Renderer *renderer) {

// }
// void _Scene_Draw_Text_Bubbles(  struct Scene *scene, SDL_Renderer *renderer) {

// }

void Scene_Draw(struct Scene *scene, struct Settings *settings, struct SDL_Texture *rt,
                SDL_Renderer *renderer) {
    //     /* - Draw the scene frame - */

    //     _Scene_Draw_Background(scene, renderer);
    //     _Scene_Draw_Actors(scene, renderer);
    //     _Scene_Draw_Text_Bubbles(scene, renderer);
}
