#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Game object: high level API to everything
**
*/

#include <math.h>
#include <stdio.h>
#include "enums.h"
#include "convoy.h"
#include "structs.h"
#ifdef SOTA_OPENGL
    #include <GL/glew.h>
    #include <SDL_opengl.h>
    #include <GL/glu.h>
#endif

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Convoy;
struct Menu;
struct Scene;

/* --- Game structs --- */
typedef struct Game_State_Times {
    i32 current;
    i32 previous;
} Game_State_Times;

typedef struct Game_State {
    struct Game_State_Times top; /* aka state */
    struct Game_State_Times sub;
    i8 animation_attack;
    i8 chapter;
} Game_State;

typedef struct Game_Render {
    SDL_Renderer    *er;
    SDL_Texture     *target;
    SDL_Window      *window;
#ifdef SOTA_OPENGL
    GLuint           gl_programid;
    SDL_GLContext    gl_context;
#endif /* SOTA_OPENGL */
} Game_Render;

typedef struct Game_ECS {
    tnecs_C timer_typeflag;
} Game_ECS;

typedef struct Game_Timers {
    tnecs_E ai;
    tnecs_E reinf;
    u64    runtime_ns; /* -> millions of years */
} Game_Timers;

typedef struct Game_Flags {
    b32   ismouse;
    b32   iscursor;
    b32   isrunning;
    b32   isShadow;
    b32   fast_forward;
} Game_Flags;

typedef struct Game_Fonts {
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_tight;
    struct PixelFont *pixelnours_big;
    struct PixelFont *menu;
} Game_Fonts;

typedef struct Game_FPS {
    /* frames/time after fps_text->update_time_ns */
    f32 instant;
    /* rolling average of fps */
    // float rolling;
    tnecs_E entity;
} Game_FPS;

typedef struct Game_Audio {
    /* The music that will be played. */
    Mix_Music *music;
    Mix_Chunk *soundfx_cursor;
    Mix_Chunk *soundfx_next_turn;
} Game_Audio;

typedef struct Game_RNG {
    u64 s_xoshiro256ss[4];
} Game_RNG;

typedef struct Game_Inputs {
    struct KeyboardInputMap  keyboardInputMap;
    struct GamepadInputMap   gamepadInputMap;
    // struct MouseInputMap  mouseInputMap;
    i32   controller_type;
    /* Button interpreted as which input */
    u32 arr[SOTA_BUTTON_END];
} Game_Inputs;

typedef struct Game_Narrative {
    /* gameplay state bitfields, narrative conditions */
    struct Conditions *conditions;
    tnecs_E cutscene;
    tnecs_E scene;
} Game_Narrative;

typedef struct Game_Cursor {
    tnecs_E    entity;
    /* move direction in current frame.
    **  Polled at start of frame. */
    Point           move;
    Point           lastpos;
    b32             frame_moved;
    b32             diagonal;
    i32             moved_time_ms;
    i8              moved_direction;
} Game_Cursor;

typedef struct Game_Mouse {
    tnecs_E entity;
} Game_Mouse;

typedef struct Game_Turn_End {
    i32 army;
} Game_Turn_End;

typedef struct Game_Selected {
    tnecs_E unit_entity;
    Point   unit_initial_position;
    Point   unit_moved_position;
    tnecs_E item;
    i32     menu_option;
} Game_Selected;

typedef struct Game_Popups {
    tnecs_E arr[POPUP_TYPE_NUM];
    tnecs_E pre_combat;
} Game_Popups;

typedef struct Game_Hovered {
    tnecs_E unit_entity;
} Game_Hovered;

typedef struct Game_Menus {
    tnecs_E *stack;

    /* Event to send after all menus popped */
    u32 allpopped_event;

    tnecs_E map_action;
    tnecs_E unit_action;
    tnecs_E item_action;
    tnecs_E item_drop;
    tnecs_E item_select;
    tnecs_E trade;
    tnecs_E staff_select;
    tnecs_E weapon_select;
    tnecs_E stats;
    tnecs_E growths;
    tnecs_E deployment;
    tnecs_E which_hand;

    s8 filename;
} Game_Menus;

typedef struct Game_Title_Screen {
    tnecs_E menu; /* i.e. first_menu */
    tnecs_E title;
} Game_Title_Screen;

typedef struct Game_Targets {
    /* -- Chosen by player -- */
    /* Order of target in any candidates array */
    int order;
    int previous_order;

    /* ptr to other target list, used by choosecandidates */
    tnecs_E *candidates;       /* [order] */
    /* --- on attackmap --- */
    tnecs_E *defendants;       /* combat */
    tnecs_E *patients;         /* staff, items */
    /* --- on neighbouring tiles --- */
    tnecs_E *victims;          /* rescue */
    tnecs_E *spectators;       /* dance */
    tnecs_E *auditors;         /* talk */
    tnecs_E *passives;         /* trade */
    tnecs_E *openables;        /* doors and chests */
    tnecs_E *deployed;         /* deployment positions */
} Game_Targets;

/* -- Game Object -- */
typedef struct Game {
    tnecs_E                     map;

    struct Party                party;
    struct Combat               combat;
    struct Convoy               convoy;
    struct Settings             settings;

    struct Game_AI             ai;
    struct Game_ECS            ecs;
    struct Game_FPS            fps;
    struct Game_RNG            RNG;
    struct Game_State          state;
    struct Game_Flags          flags;
    struct Game_Fonts          fonts;
    struct Game_Audio          audio;
    struct Game_Mouse          mouse;
    struct Game_Menus          menus;
    struct Game_Render         render;
    struct Game_Popups         popups;
    struct Game_Timers         timers;
    struct Game_Inputs         inputs;
    struct Game_Cursor         cursor;
    struct Game_Targets        targets;
    struct Game_Hovered        hovered;
    struct Game_Selected       selected;
    struct Game_Turn_End       turn_end;
    struct Game_Narrative      narrative;
    struct Game_Title_Screen   title_screen;
} Game;
extern const struct Game Game_default;

void Events_Names_Strings(void);
void Events_Names_Declare(void);
void Events_Receivers_Declare(void);

/* --- Pre game --- */
Input_Arguments IES_Init(int argc, char *argv[]);

/* --- Constructors/Destructors --- */
/* Note: SDL stuff is init/freed here. */
struct Game *Game_New(Settings settings);
void Game_Free(   struct Game *sota);

void IES_Core_Free(void);

/* -- Multithreaded constructors -- */
int _Game_New_Tnecs(    void *data);
int _Game_New_Alloc(    void *data);
int _Game_New_Events(   void *data);

/* --- Startup --- */
// Depending on input arguments, IES can play a scene, go directly to a map...
// By default, and for users: Go to title screen.
void Game_Startup_Map(          Game *IES);
void Game_Startup_Scene(        Game *IES);
void Game_Startup_TitleScreen(  Game *IES);

/* --- Stepping --- */
void  Game_Step(            struct Game *sota);
void _Game_Step_Render(     struct Game *sota);
void _Game_Step_Control(    struct Game *sota);
u64  _Game_Step_PreFrame(   struct Game *sota);
void _Game_Step_PostFrame(  struct Game *sota, u64 time);
void _Game_Step_Pi(         struct Game *sota,
                            void        *data,
                            tnecs_Pi     pi);

/* --- I/O --- */
void  Game_Save(struct Game *sota, i32 save_ind);
void  Game_Save_Load(struct Game *sota, i32 save_ind);
void _Game_saveJSON(struct Game *sota, s8  filename);
void _Game_loadJSON(struct Game *sota, s8  filename);

void Game_Save_Copy(i32 from, i32 to);

/* --- State --- */
void Game_State_Set(    struct Game *sota,
                        i8 ns, const char *r);
void Game_subState_Set( struct Game *sota,
                        i8 ns, const char *r);
i32  Game_State_Current(    const struct Game *sota);
i32  Game_State_Previous(   const struct Game *sota);
i32  Game_Substate_Current( const struct Game *sota);
i32  Game_Substate_Previous(const struct Game *sota);

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota);

/* --- Time --- */
void Game_Delay(struct Game *sota,
                i64 d, u64 c, u64 e);

/* --- FPS --- */
void Game_FPS_Free(     struct Game *sota);
i64  Game_FPS_Delay(    struct Game *sota, u64 e);
void Game_FPS_Create(   struct Game *sota, i64 ut);
void Game_FPS_Instant(  struct Game *IES,
                        u32   frame_count,
                        i64   last_update_ns);

/* --- SETTINGS --- */
void  Game_Brightness_Set(struct Game *sota, float bright);
float Game_Brightness_Get(struct Game *sota);

/* --- DISPLAY --- */
void  Game_Display_Bounds(struct Game *sota);

/* --- AI --- */
tnecs_E    Game_AI_Control_Start(  const struct Game *const g);
void            Game_AI_Control_End(    const struct Game *const g);

i32 Game_inControl(const struct Game *const IES);

/* --- AUDIO --- */
/* -- Music -- */
void Game_Music_Play( struct Game *sota);
void Game_Music_Stop( struct Game *sota);
void Game_Music_Pause(struct Game *sota);

/* -- Volume -- */
void Game_Volume_Set(       struct Game *sota,
                            int volume);
void Game_Volume_Music_Set( struct Game *sota,
                            int volume);
int  Game_Volume_Music_Get(  struct Game *sota);
void Game_Volume_SoundFX_Set(struct Game *sota,
                             int volume);
int  Game_Volume_SoundFX_Get(struct Game *sota);

/* -- Battle -- */
void    Game_Battle_Start(struct Game *sota,
                          struct Menu *mc);

b32     Game_inCombat(struct Game *sota);

i32  Game_Chapter(const struct Game *sota);
s8   Savefile_Path(i32 save_ind);

/* -- Scene -- */
struct Scene *Game_Scene(struct Game *sota);

/* -- Exit -- */
void Game_atexit(Game *input);

#endif /* GAME_STRUCT_H */
