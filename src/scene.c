#include "scene.h"
// #ifndef STB_SPRINTF_IMPLEMENTATION
// #define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"
// #endif // STB_SPRINTF_IMPLEMENTATION

struct Scene Scene_default =  {
    .json_element   = JSON_SCENE,
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

/* --- I/O --- */
s8 Scene_Filename_Chapter(i32 chapter, i32 subindex) {
    // TODO: checking validit
    return (Scene_Filename(chapter * SCENE_DIVISOR + subindex));
}

s8 Scene_Filename(i32 index) {
    // TODO: checking validity
    s8 filename = s8_mut(PATH_JOIN("..", "scenes/"));
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
   
    /* -- Line array elem -- */
    /* -- Condition array elem -- */
    /* -- Didascalie array elem -- */
    /* -- Music array elem -- */
    /* -- Background array elem -- */
    
}

void Didasdalie_readJSON(void *input, cJSON *jdid) {
  
}

void Condition_readJSON(void *input, cJSON *jcond) {

}

void Didascalie_writeJSON(void *input, cJSON *jdid) {
  
}

void Conditions_writeJSON(void *input, cJSON *jcond) {

}

/* --- Player interaction --- */
void Scene_Finish(struct Scene *scene, struct Game *sota) {
    /* - Finish scene - */
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Scene Animation Finished");
    SDL_assert((scene->event > event_Start) && (scene->event < event_End));
    Event_Emit(__func__, SDL_USEREVENT, scene->event, NULL, NULL);
}

void Scene_Next_Line(struct Scene *scene, struct Game *sota) {
    /* - Get next statement type, play it if its not a - */
    struct SceneLine *next_line = NULL;
    /* TODO: enable when ready to play statements */
#if 0
    u64 statement_num = DARR_NUM(scene->statements);
    while (scene->statement < statement_num) {
        void    *statement  = scene->statements[scene->statement++]
                              i32     * type       = (i32 *)statement;
        SDL_assert((*type) > SCENE_STATEMENT_START);
        SDL_assert((*type) < SCENE_STATEMENT_NUM);
        if ((*type) == SCENE_STATEMENT_LINE) {
            next_line = (struct SceneLine *)statement;
            break;
        } else {
            scene_statement_play[(*type)](statement);
        }
    }
#endif

    /* - Skip current line if in the middle of rendering - */

    /* - No more lines - */
    if (next_line == NULL) {
        Scene_Finish(scene, sota);
    }
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
