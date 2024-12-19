#ifndef SCENE_H
#define SCENE_H

#include "jsonio.h"
#include "conditions.h"
#include "n9patch.h"
#include "popup/text_box.h"

// SCENE MVP
//  1.
//      - Change state to scene
//      - Write fixed Lines     in box at bottom
//           -> TEXT BUBBLES LATER
//      - Write fixed Speaker name on top of box
//           -> HIGHLIGHT SPEAKER LATER
//      - Button to skip scene
//      - Button to go to next line
//      - Draw Speaker Rectangle somewhere on the scene
//  2.
//      - Load speaker from file
//      - Load Lines from file
//      - highlight speaker
//      - Speaker movement
//      - Speaker movement
//          - Basic slides

// Scene index = chapter * SCENE_DIVISOR + scene_sub_index
//

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

/* --- Scene flow --- */
// 1. Press button
// 2. Go to next line
//  a. All didascalies play

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

enum SCENE_MISC {
    SCENE_TEXT_BOX_PATCH_PIXELS     =  16,
    SCENE_TEXT_BOX_SCALE            =   1,
    SCENE_TEXT_BOX_MAX_LENGTH       = 120,
    SCENE_TEXT_BOX_MAX_LINES        =   2,
    SCENE_TEXT_BOX_PADDING_LEFT     =  15,
    SCENE_TEXT_BOX_PADDING_BOTTOM   =   8,
    SCENE_TEXT_BOX_PADDING_TOP      =   5,

    SCENE_TEXT_BOX_POS_X            = 100,
    SCENE_TEXT_BOX_POS_Y            = 140,
    SCENE_TEXT_BOX_ACTOR_POS_X      = 120,
    SCENE_TEXT_BOX_ACTOR_POS_Y      = 130,
};

enum SCENE_DIDASCALIE {
    SCENE_DIDASCALIE_START  = -1,
    SCENE_DIDASCALIE_APPEAR =  0,
    SCENE_DIDASCALIE_SLIDE  =  1,
    SCENE_DIDASCALIE_NUM    =  2
};

enum SCENE_ACTORS {
    SCENE_ACTOR_POS_X = 80,
    SCENE_ACTOR_POS_Y = 10,
    SCENE_ACTOR_POS_W = 20,
    SCENE_ACTOR_POS_H = 160,
    SCENE_ACTOR_COLOR_OFFSET = 8,

    SCENE_ACTOR_POS_X_DIFF = 100,
};

// Statements: Renpy terminology
//      Anything a Scene can do.
//      Scene pauses only on line. Otherwise play statements.
enum SCENE_STATEMENTS {
    SCENE_STATEMENT_START       = -1,
    SCENE_STATEMENT_LINE        =  0,
    SCENE_STATEMENT_DIDASCALIE  =  1,
    SCENE_STATEMENT_CONDITION   =  2,
    SCENE_STATEMENT_BACKGROUND  =  3,
    SCENE_STATEMENT_MUSIC       =  4,
    SCENE_STATEMENT_NUM         =  5
};

/* --- TYPEDEFS --- */
typedef void (*json_func)(void *, cJSON *);

// Scene FSMs
extern const json_func fsm_Scene_readJSON[SCENE_STATEMENT_NUM];
extern const json_func fsm_Scene_writeJSON[SCENE_STATEMENT_NUM];

// Scene Statement FSM
typedef void (*fsm_scene_statement_t)(void *);
extern const fsm_scene_statement_t scene_statement_play[SCENE_STATEMENT_NUM];

typedef struct SceneHeader {
    i32 statement_type;
    i32 didascalie_type;
} SceneHeader;


/* -- Didascalie (theater vocabulary) -- */
// - Note to actors of a scene about what to *do* during a scene
// Only for actors -> Sprites
//  Sprites:
//  - Transform (animated or not)
//    - Move to position (Sliding)
//    - Flipeureka7349

//    - Rotate
//  - Animate
//      - Fade in, fade out

typedef struct DidascalieSlide {
    Point start;
    Point end;
    int speed;
} DidascalieSlide;
extern const struct DidascalieSlide DidascalieSlide_default;

typedef struct DidascalieAppear {
    Point pos;
} DidascalieAppear;
extern const struct DidascalieAppear DidascalieAppear_default;

typedef union DidascalieUnion {
    DidascalieSlide slide;
} DidascalieUnion;


typedef struct SceneDidascalie {
    s8 actor;
    DidascalieUnion _union;
} SceneDidascalie;
extern const struct SceneDidascalie SceneDidascalie_default;
extern const struct SceneDidascalie SceneDidascalie_FadeAll;

typedef struct SceneLine {
    s8 actor;
    s8 line;
} SceneLine;
extern const struct SceneLine SceneLine_default;

typedef struct SceneMusic {
    int a;
} SceneMusic;

extern const struct SceneBackground SceneMusic_default;
extern const struct SceneBackground SceneMusic_Stop;

//  Background:
//  - Transition effects
//    - Slide
//    - Fade (to black)
typedef struct SceneBackground {
    int a;
} SceneBackground;
extern const struct SceneBackground SceneBackground_default;
extern const struct SceneBackground SceneBackground_FadeToBlack;
//  Screen:
//  - Shake screen

typedef union SceneStatementUnion {
    SceneLine       line;
    SceneDidascalie didascalie;
    SceneMusic      music;
    SceneBackground background;
} SceneStatementUnion;

typedef struct SceneStatement {
    SceneHeader header;
    SceneStatementUnion _union;
} SceneStatement;

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
    jsonIO_Header jsonio_header;

    /* -- Current game condition -- */
    struct Conditions    game_cond;
    struct Conditions    line_cond; /* Reset everytime a line is read */

    /* Current statement */
    int current_statement;
    b32 update;

    /* Unit order */
    int *actor_order;

    /* Statements: meat of the Scene
     *  - SceneLine, SceneDidascalie, SceneMusic OR SceneBackground
     *      - No Condition
     *  - Note: only statements that satisfy game_cond
     */
    SceneStatement *statements;

    /* -- Rendering -- */
    SDL_Rect texture_rect;
    SDL_Texture *texture;
    struct PixelFont   *pixelnours;
    struct Text_Box     text_box;
    struct n9Patch      n9patch;

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
extern const struct Scene Scene_default;


/* --- Constructors/Destructors --- */
void Scene_Free(struct Scene *scene);
void Scene_Init(struct Scene *scene);
void Scene_Text_Box_Init(struct Scene *scene, SDL_Renderer *renderer);
void Scene_Texture_Create(struct Scene *scene, SDL_Renderer *renderer);

void Scene_Statement_Add(Scene *scene, SceneStatement statement);

/* --- Actors --- */
i32     Scene_Actors_Num(Scene *scene);

/// @return Actor order in actors DARR, or -1;
i32     Scene_Actor_Find(Scene *scene, u16 actor);
void    Scene_Actor_Add( Scene *scene, u16 actor);

/* --- I/O --- */
s8 Scene_Filename_Chapter(i32 chapter, i32 subindex);
s8 Scene_Filename(i32 index);
void Scene_readJSON( void *s, cJSON *js);
void Scene_writeJSON(void *s, cJSON *js);

i32 Scene_jsonDidascalie_Type(cJSON *jstatement);

void Scene_Background_readJSON( void *c, cJSON *jc);
void Scene_Background_writeJSON(void *c, cJSON *jc);
void Scene_Music_readJSON( void *c, cJSON *jc);
void Scene_Music_writeJSON(void *c, cJSON *jc);
void Scene_Line_readJSON( void *c, cJSON *jc);
void Scene_Line_writeJSON(void *c, cJSON *jc);
void Scene_Didascalie_readJSON( void *c, cJSON *jc);
void Scene_Didascalie_writeJSON(void *c, cJSON *jc);
void Scene_Condition_readJSON(  void *c, cJSON *jc);
void Scene_Condition_writeJSON(void *c, cJSON *jc);

void Scene_Didascalie_Appear_readJSON( void *c, cJSON *jc);
void Scene_Didascalie_Slide_readJSON( void *c, cJSON *jc);
/* --- Rendering --- */
/* Read game condition and render text lines */
void Scene_Render(struct Scene *scene);

/* --- Output --- */
void Scene_Raw_Output(   struct Scene *scene, s8 path);
void Scene_Render_Output(struct Scene *scene, s8 path);

/* --- Print --- */
void Scene_Raw_Print(   struct Scene *scene);
void Scene_Render_Print(struct Scene *scene);

/* --- Statement --- */
int Scene_Statement_Next(struct Scene *scene);
int Scene_Line_Next(struct Scene *scene);

/* --- Numbers --- */
void Scene_Line_Num(        struct Scene *scene);
void Scene_Music_Num(       struct Scene *scene);
void Scene_Statement_Num(   struct Scene *scene);
void Scene_Didascalie_Num(  struct Scene *scene);
void Scene_Background_Num(  struct Scene *scene);

/* --- Player Interaction --- */
void Scene_Finish(      struct Scene *scene, struct Game *sota);

i32 Scene_jsonStatement_Type(cJSON *jstatement);

/* --- Play --- */
void _Scene_Animate_Actors(        struct Scene *scene);
void _Scene_Animate_Background(    struct Scene *scene);
void _Scene_Animate_Text_Boxs(  struct Scene *scene);

void Scene_Animate(struct Game  *sota, tnecs_entity entity,
                   struct Scene *scene, struct Timer *timer);

/* --- Draw --- */
void _Scene_Draw_Text(      Scene *scene, SDL_Texture *render_target, SDL_Renderer *renderer);
void _Scene_Draw_Actors(    Scene *scene, SDL_Renderer *renderer);
void _Scene_Draw_Background(Scene *scene, SDL_Renderer *renderer);

void Scene_Draw(struct Scene *scene, struct Settings *settings,
                struct SDL_Texture *rt, SDL_Renderer *renderer);

void Scene_Update(struct Scene *scene, struct Settings *settings,
                  struct SDL_Texture *rt, SDL_Renderer *renderer);

#endif /* SCENE_H */
