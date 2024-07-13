#ifndef SCENE_H
#define SCENE_H

#include <string.h>
#include "events.h"
#include "types.h"
#include "utilities.h"
#include "filesystem.h"
#include "jsonio.h"
#include "enums.h"
#include "macros.h"
#include "debug.h"
#include "nmath.h"
#include "cJSON.h"
#include "bitfields.h"
#include "SDL2/SDL.h"

/* ---------------------------------- Scene --------------------------------- */  
//  A visual novel
//      - Multiple characters talk to each other
//          - Player clicks to move to next line
//          - Text bubbles pointing to each character?
//          - Text box on screen bottom?
//      - Characters move around, emote, lip sync, blink...
//      - Sound & music
//      - Transition effects
//      - Interactive script
//          1- Scenes   with dynamic conditions -> play if conditions are met
//           -> Yes
//          2- Lines    with dynamic conditions -> play if conditions are met
//           -> Maybe
//          3- Lines    with dynamic content    -> replaced at runtime
//           -> mmmMaybe?
//      - Branching decisions during scene
//          - Ex: Give away weapons OR Refuse to give away weapons

// Is narrated map a Scene or a cutscene?
//  - Show map (difference kind of background)
//      - Units move around on map
//  - Text box on screen bottom
//  - Player clicks to move to next line
//   ANSWER -> A Scene

/* --- FORWARD DECLARATIONS --- */
struct Boss;
struct Game;
struct Settings;

enum SCENE {
    SCENE_MAX_ACTORS = 8,
};

enum SCENE_bOFFSET {
    SCENE_STATEMENT_bOFFSET = 0,
};

// Statements: Renpy terminology
//      Anything a Scene can do.
//      Scene pauses only on line. Otherwise play statements.
enum SCENE_STATEMENTS {
    SCENE_STATEMENT_LINE        = 0,
    SCENE_STATEMENT_DIDASCALIE  = 1,
    SCENE_STATEMENT_BACKGROUND  = 2,
    SCENE_STATEMENT_MUSIC       = 3,
    SCENE_STATEMENT_NUM         = 4,
};

// Scene Statement FSM

typedef void (*fsm_scene_statement_t)(void *);
extern fsm_scene_statement_t scene_statement_play[SCENE_STATEMENT_NUM];


/* -- Didascalie (theater vocabulary) -- */
// - Note to actors of a scene about what to *do* during a scene
// Only for actors -> Sprites
//  Sprites:
//  - Transform (animated or not)
//    - Move to position (Sliding)
//    - Flip
//    - Rotate
//  - Animate
//      - Fade in, fade out
struct SceneDidascalie {
    i32 type;  /* SCENE_STATEMENT_bOFFSET = 0  (+ 4) */

};
extern struct SceneDidascalie SceneDidascalie_default;
extern struct SceneDidascalie SceneDidascalie_FadeAll;

struct SceneLine {
    i32 type;  /* SCENE_STATEMENT_bOFFSET = 0  (+ 4) */

    s8 speaker;
    s8 line;
};
extern struct SceneLine SceneLine_default;

struct SceneMusic {
    i32 type;  /* SCENE_STATEMENT_bOFFSET = 0  (+ 4) */

};
extern struct SceneBackground SceneMusic_default;
extern struct SceneBackground SceneMusic_Stop;

//  Background:
//  - Transition effects
//    - Slide
//    - Fade (to black)
struct SceneBackground {
    i32 type;  /* SCENE_STATEMENT_bOFFSET = 0  (+ 4) */

};
extern struct SceneBackground SceneBackground_default;
extern struct SceneBackground SceneBackground_FadeToBlack;
//  Screen:
//  - Shake screen


/* -- Game Narrative Conditions -- */
// 1- To load and play Scenes,
// 2- To choose between lines in a Scene dynamically
// NOTE: Conditions are naturally AND
struct Conditions {
    u32 dead     [BITFIELD_LEN(UNIT_NUM)];
    u32 alive    [BITFIELD_LEN(UNIT_NUM)];
    u32 recruited[BITFIELD_LEN(UNIT_ORDER_PC_END)];
};
// NOTE: Alive and dead are separate for those conditions:
//      - line REQUIRES unit to be alive        -> alive bit 1
//      - line does not care if unit is alive   -> alive bit 0
//      - line REQUIRES unit to be dead         -> dead bit  1
//      - line does not care if unit is dead    -> dead bit  0
extern struct Conditions Conditions_Game_default;
extern struct Conditions Conditions_Line_default;


/* A scene is a conversation.
*   - Up to 8 characters sprites on screen at once
*   - Characters can move around screen
*       - Limited animation
*            - Lip sync
*            - Blinking
*            - Sliding, rotating
*   - Text bubbles with scrollling text
*/
typedef struct Scene {
    s8   json_filename; /* JSON_FILENAME_bOFFSET = 0  (+ 24) */
    u8   json_element;  /* JSON_ELEM_bOFFSET     = 24 (+ ALIGNMENT) */

    /* -- Condition to play scene -- */
    struct Conditions    cond;

    u16 *actors;

    /* Statements: meat of the Scene
     *  - SceneLine, SceneDidascalie, SceneMusic, SceneBackground
     */
    void **statements;

    /* -- Post-scene -- */
    // What happens after a scene ends?
    //  - Play another scene
    //          -> event_Scene_Play
    //  - Go back to gameplay, preparation.
    //          -> event?
    //  - Go back to next chapter start.
    //          -> event?
    tnecs_entity event;

} Scene;
extern struct Scene Scene_default;

/* --- Conditions --- */
b32 Conditions_Compare(struct Conditions *line_cond, struct Conditions *game_cond);

void Conditions_Dead_char(     struct Conditions *cond, char *u);
void Conditions_Alive_char(    struct Conditions *cond, char *u);
void Conditions_Recruited_char(struct Conditions *cond, char *u);

void Conditions_Dead(     struct Conditions *cond, size_t uo);
void Conditions_Alive(    struct Conditions *cond, size_t uo);
void Conditions_Recruited(struct Conditions *cond, size_t uo);

/* --- Constructors/Destructors --- */
void Scene_Free(struct Scene *scene);
void Scene_Init(struct Scene *scene);

/* --- Replace --- */
// TODO: replace text dynamically
void Scene_Replace_Add(struct Scene *scene, s8 with);

/* --- Actors --- */
i32     Scene_Actors_Num(Scene *scene);

/// @return Actor order in actors DARR, or -1;
i32     Scene_Actor_Find(Scene *scene, u16 actor);
void    Scene_Actor_Add( Scene *scene, u16 actor);

/* --- I/O --- */
void Scene_readJSON( void *s, cJSON *js);
void Scene_writeJSON(void *s, cJSON *js);

// TODO: Split into non human readable conditions/hashes for game
void Conditions_readJSON( void *c, cJSON *jc);
void Conditions_writeJSON(void *c, cJSON *jc);

/* -- Load all scenes with conditions -- */
struct Scene *Scenes_Load(struct Scene *sdarr, struct Conditions *scene_concs,
                          i16 chapter, u16 scene_time);

/* --- Speakers --- */
void Scene_Speaker_Add(struct Scene *scene, u16 order);

/* --- Rendering --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene);

/* --- Output --- */
void Scene_Raw_Output(   struct Scene *scene, s8 path);
void Scene_Render_Output(struct Scene *scene, s8 path);

/* --- Print --- */
void Scene_Raw_Print(   struct Scene *scene);
void Scene_Render_Print(struct Scene *scene);

/* --- Player Interaction --- */
void Scene_Next_Line(struct Scene *scene, struct Game *sota);
void Scene_Finish(struct Scene *scene, struct Game *sota);

/* --- Play --- */
void _Scene_Animate_Actors(        struct Scene *scene);
void _Scene_Animate_Background(    struct Scene *scene);
void _Scene_Animate_Text_Bubbles(  struct Scene *scene);

void Scene_Animate(struct Game  *sota, tnecs_entity entity,
                   struct Scene *scene, struct Timer *timer);

/* --- Draw --- */
void _Scene_Draw_Actors(        struct Scene *scene, SDL_Renderer *renderer);
void _Scene_Draw_Background(    struct Scene *scene, SDL_Renderer *renderer);
void _Scene_Draw_Text_Bubbles(  struct Scene *scene, SDL_Renderer *renderer);

void Scene_Draw(struct Scene *scene, struct Settings *settings,
                struct SDL_Texture *rt, SDL_Renderer *renderer);

#endif /* SCENE_H */
