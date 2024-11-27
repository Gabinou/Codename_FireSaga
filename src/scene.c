#include "scene.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

struct Scene Scene_default =  {
    .jsonio_header.json_element = JSON_SCENE,
    /* Can't put Conditions_Game_start here. */
    .game_cond                  = {.alive = {0xFFFFFFFF}},
    .line_cond                  = {.alive = {0xFFFFFFFF}},
};

struct SceneLine        SceneLine_default       = {0};
struct SceneDidascalie  SceneDidascalie_default = {0};
struct SceneBackground  SceneBackground_default = {0};

static u64 hash_alive     = 210706583606;       /* sota_hash_djb2(s8_literal("alive"));     */
static u64 hash_dead      = 6385147891;         /* sota_hash_djb2(s8_literal("dead"));      */
static u64 hash_recruited = 249904965071548876; /* sota_hash_djb2(s8_literal("recruited")); */

json_func fsm_Scene_Didascalie_readJSON[SCENE_DIDASCALIE_NUM] = {
    Scene_Didascalie_Appear_readJSON,
    Scene_Didascalie_Slide_readJSON
};

json_func fsm_Scene_readJSON[SCENE_STATEMENT_NUM] = {
    Scene_Line_readJSON,
    Scene_Didascalie_readJSON,
    Scene_Condition_readJSON,
    Scene_Background_readJSON,
    Scene_Music_readJSON,
};

json_func fsm_Scene_writeJSON[SCENE_STATEMENT_NUM] = {
    Scene_Line_writeJSON,
    Scene_Didascalie_writeJSON,
    Scene_Condition_writeJSON,
    Scene_Background_writeJSON,
    Scene_Music_writeJSON,
};

void Scene_Init(struct Scene *scene) {
    SDL_assert(scene != NULL);
    scene->statements       = DARR_INIT(scene->statements, void *, 16);
    scene->actors           = DARR_INIT(scene->actors, u16, 16);
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

/* --- I/O --- */
s8 Scene_Filename_Chapter(i32 chapter, i32 subindex) {
    // TODO: checking validity
    return (Scene_Filename(chapter * SCENE_DIVISOR + subindex));
}

s8 Scene_Filename(i32 index) {
    // TODO: checking validity
    s8 filename = s8_mut("scenes/");
    char numbuff[8];
    stbsp_sprintf(numbuff, "%05d\0\0\0", index);
    filename = s8cat(filename, s8_literal("scene"));
    filename = s8cat(filename, s8_var(numbuff));
    filename = s8cat(filename, s8_literal(".json"));
    return (filename);
}

void Scene_readJSON(void *input, cJSON *jscene) {
    /* --- Conditions for Scene --- */

    /* --- Scene Array --- */
    i32 statement_num = cJSON_GetArraySize(jscene);
    for (int i = 0; i < statement_num; i++) {
        struct cJSON *jstatement = cJSON_GetArrayItem(jscene, i);
        i32 statement_type = Scene_jsonStatement_Type(jstatement);

        /* -- Skip is statement is invalid -- */
        if ((statement_type <= SCENE_STATEMENT_START) &&
            (statement_type >= SCENE_STATEMENT_NUM)) {
            continue;
        }

        /* -- FSM for array elem -- */
        SDL_assert(fsm_Scene_readJSON[statement_type] != NULL);
        fsm_Scene_readJSON[statement_type](input, jstatement);
    }
}

i32 Scene_Statement_Type(void *statement) {
    SceneHeader *header = statement;
    return (header->statement_type);
}

i32 Scene_Statement_Didascalie_Type(void *statement) {
    SceneHeader *header = statement;
    return (header->didascalie_type);
}

i32 Scene_jsonStatement_Type(cJSON *jstatement) {
    cJSON *jline = cJSON_GetObjectItem(jstatement, "Line");
    if (jline != NULL) {
        // SDL_Log("Is a Line");
        return (SCENE_STATEMENT_LINE);
    }

    cJSON *jcondition = cJSON_GetObjectItem(jstatement, "Condition");
    if (jcondition != NULL) {
        // SDL_Log("Is a Condition");
        return (SCENE_STATEMENT_CONDITION);
    }

    i32 did = Scene_jsonDidascalie_Type(jstatement);
    if ((did > SCENE_DIDASCALIE_START) && (did < SCENE_DIDASCALIE_NUM)) {
        // SDL_Log("Is a Didascalie");
        return (SCENE_STATEMENT_DIDASCALIE);
    }

    // SDL_Log("Unknown Statement");
    return (SCENE_STATEMENT_START);
}

i32 Scene_jsonDidascalie_Type(cJSON *jstatement) {
    cJSON *jappear = cJSON_GetObjectItem(jstatement, "Appear");
    if (jappear != NULL) {
        // SDL_Log("Is an Appear");
        return (SCENE_DIDASCALIE_APPEAR);
    }

    cJSON *jslide = cJSON_GetObjectItem(jstatement, "Slide");
    if (jslide != NULL) {
        // SDL_Log("Is a Slide");
        return (SCENE_DIDASCALIE_SLIDE);
    }

    // SDL_Log("Unknown Didascalie");
    return (SCENE_DIDASCALIE_START);
}

void Scene_Didascalie_readJSON(void *input, cJSON *jdid) {
    Scene *scene = input;

    i32 type = Scene_jsonDidascalie_Type(jdid);
    if ((type > SCENE_DIDASCALIE_START) && (type < SCENE_DIDASCALIE_NUM)) {
        SDL_assert(fsm_Scene_Didascalie_readJSON[type] != NULL);
        fsm_Scene_Didascalie_readJSON[type](input, jdid);
    }
}

void Scene_Didascalie_Appear_readJSON( void *input, cJSON *jdid) {
    Scene *scene = input;

    SceneDidascalie scene_did = SceneDidascalie_default;

    /* Compare conditions */
    if (Conditions_Match(&scene->line_cond, &scene->game_cond)) {
        scene_did.scene_header.statement_type   = SCENE_STATEMENT_DIDASCALIE;
        scene_did.scene_header.didascalie_type  = SCENE_DIDASCALIE_APPEAR;

        // TODO parse jdid, add to scene_did

        Scene_Statement_Add(scene, &scene_did);
    }

}

void Scene_Didascalie_Slide_readJSON( void *input, cJSON *jdid) {
    Scene *scene = input;

    SceneDidascalie scene_did = SceneDidascalie_default;

    /* Compare conditions */
    if (Conditions_Match(&scene->line_cond, &scene->game_cond)) {
        scene_did.scene_header.statement_type   = SCENE_STATEMENT_DIDASCALIE;
        scene_did.scene_header.didascalie_type  = SCENE_DIDASCALIE_SLIDE;

        // TODO parse jdid, add to scene_did

        Scene_Statement_Add(scene, &scene_did);
    }
}

void Scene_Condition_readJSON(void *input, cJSON *jcond) {
    Scene *scene = input;

    cJSON *jcondition = cJSON_GetObjectItem(jcond, "Condition");
    // Was checked before -> assert
    SDL_assert(jcondition != NULL);

    s8 actor        = s8_var(jcondition->child->string);
    i32 unit_order  = Unit_Name2Order(actor);
    if ((unit_order <= UNIT_ORDER_START) && (unit_order >= UNIT_NUM)) {
        SDL_Log("Problem parsing Scene's Condition: Unknown name '%s'.", actor.data);
        exit(1);
    }

    s8 condition    = s8_toLower(s8_mut(cJSON_GetStringValue(jcondition->child)));
    u64 hash_cond   = sota_hash_djb2(condition);

    if (hash_cond == hash_alive) {
        // SDL_Log("alive");
        Conditions_Alive_Order(&scene->line_cond, unit_order);
    } else if (hash_cond == hash_dead) {
        // SDL_Log("dead");
        Conditions_Dead_Order(&scene->line_cond, unit_order);
    } else if (hash_cond == hash_recruited) {
        // SDL_Log("recruited");
        Conditions_Recruited_Order(&scene->line_cond, unit_order);
    } else {
        SDL_Log("Problem parsing Scene's Condition: Unknown condition '%s'.",
                cJSON_GetStringValue(jcondition->child));
        exit(1);
    }

    s8_free(&condition);
}

void Scene_Didascalie_writeJSON(void *input, cJSON *jdid) {
    Scene *scene = input;

}

void Scene_Condition_writeJSON(void *input, cJSON *jcond) {
    Scene *scene = input;

}

void Scene_Background_writeJSON(void *input, cJSON *jc) {

}

void Scene_Background_readJSON( void *input, cJSON *jc) {
    Scene *scene = input;

    // Scene_Statement_Add(scene, statement)
}

void Scene_Music_readJSON( void *input, cJSON *jc) {
    Scene *scene = input;

    // Scene_Statement_Add(scene, statement)
}

void Scene_Music_writeJSON(void *input, cJSON *jc) {

}

void Scene_Line_readJSON(void *input, cJSON *jstatement) {
    Scene *scene = input;

    cJSON *jline = cJSON_GetObjectItem(jstatement, "Line");
    // Was checked before -> assert
    SDL_assert(jline != NULL);

    if (jline->child == NULL) {
        SDL_Log("Problem parsing Scene's Line: No child object.");
        exit(1);
    }

    if (jline->child->string == NULL) {
        SDL_Log("Problem parsing Scene's Line: Child has no name.");
        exit(1);
    }
    if (jline->child->valuestring == NULL) {
        SDL_Log("Problem parsing Scene's Line: Child has no value.");
        exit(1);
    }

    s8 actor    = s8_var(jline->child->string);
    int order   = Unit_Name2Order(actor);

    /* Compare conditions: conditions match and actor is NOT DEAD */
    if (!Conditions_Match(&scene->line_cond, &scene->game_cond)) {
        // SDL_Log("Line: Conditions don't match");
    } else if (Bitfield_Get(scene->game_cond.dead, order)) {
        // SDL_Log("Line: Actor is dead");
    } else {
        SceneLine scene_line = SceneLine_default;

        scene_line.scene_header.statement_type = SCENE_STATEMENT_LINE;

        s8 line     = s8_var(cJSON_GetStringValue(jline->child));

        scene_line.actor    = actor;
        scene_line.line     = line;
        SDL_Log("%s: %s", scene_line.actor.data, scene_line.line.data);

        Scene_Statement_Add(scene, &scene_line);
    }

    scene->line_cond = Conditions_Game_start;
}

void Scene_Line_writeJSON(void *input, cJSON *jc) {

}

/* --- Player interaction --- */
void Scene_Finish(struct Scene *scene, struct Game *sota) {
    /* - Finish scene - */
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Scene Animation Finished");
    SDL_assert((scene->event > event_Start) && (scene->event < event_End));
    Event_Emit(__func__, SDL_USEREVENT, scene->event, NULL, NULL);
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


void Scene_Statement_Add(Scene *scene, void *statement) {
    SDL_assert(scene        != NULL);
    SDL_assert(statement    != NULL);

    if (scene->statements == NULL) {
        scene->statements       = DARR_INIT(scene->statements, void *, 16);
    }

    DARR_PUT(scene->statements, statement);
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
