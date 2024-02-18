#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <stdio.h>
#include "unit/unit.h"
#include "index_shader.h"
#include "narrative.h"
#include "enums.h"
#include "structs.h"
#include "settings.h"
#include "bitfields.h"
#include "input.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "controller/fsm.h"
#include "weapon.h"
#include "slider.h"
#include "tnecs.h"
#include "events.h"
#include "nmath.h"
#include "music.h"
#include "macros.h"
#include "hover.h"
#include "utilities.h"
#include "jsonio.h"
#include "pixelfonts.h"
#include "convoy.h"
#include "camp.h"
#include "debug.h"
#include "tile.h"
#include "text.h"
#include "AI.h"
#include "nmath.h"
#include "RNG.h"
#include "position.h"
#include "sprite.h"
#include "title_screen.h"
#include "systems/render.h"
#include "systems/slide.h"
#include "combat.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Convoy;
struct Game;

void Events_Data_Malloc(void);
void Events_Data_Free(void);
void Events_Names_Strings(void);
void Events_Names_Declare(void);
void Events_Receivers_Declare(void);

/* --- Game Object --- */
struct Game {
    SDL_Renderer    *renderer;
    SDL_Texture     *render_target;
    #ifdef SOTA_OPENGL
    GLuint         gl_programid;
    SDL_GLContext  gl_context;
    SDL_Window     *gl_window;
    #endif /* SOTA_OPENGL */
    tnecs_world  *world;

    tnecs_component           timer_typeflag;
    struct Point  cursor_lastpos;

    struct dtab *menu_options_dtab;
    struct dtab *defaultstates_dtab;

    struct dtab *items_dtab;
    struct dtab *weapons_dtab;
    struct dtab *tiles_loaded_dtab;
    struct dtab *units_loaded_dtab;
    tnecs_entity *units_loaded;

    tnecs_entity ai_timer;
    tnecs_entity reinf_timer;

    struct KeyboardInputMap  keyboardInputMap;
    struct GamepadInputMap   gamepadInputMap;
    // struct MouseInputMap  mouseInputMap;

    struct Camera camera;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_tight;
    struct PixelFont *pixelnours_big;
    struct PixelFont *menu_pixelfont;
    struct Map *map;

    /* gameplay state bitfields, narrative conditions */
    struct Conditions *conditions;

    tnecs_entity *menu_stack;
    tnecs_entity player_select_menus[MENU_PLAYER_SELECT_NUM]; /* [PLAYER_SELECT_MENU_...] */
    tnecs_entity popups[POPUP_TYPE_NUM]; /* [POPUP_TYPE_...] */
    tnecs_entity item_select_menu;
    tnecs_entity trade_menu;
    tnecs_entity staff_select_menu;
    tnecs_entity weapon_select_menu;
    tnecs_entity stats_menu;
    tnecs_entity PRE_COMBAT_POPUP;
    tnecs_entity first_menu;
    tnecs_entity title;
    tnecs_entity GM_menu;

    s8 filename_menu;
    char reason[DEFAULT_BUFFER_SIZE];

    tnecs_entity entity_cursor;
    tnecs_entity entity_mouse;
    tnecs_entity entity_transition;
    tnecs_entity entity_highlighted;
    tnecs_entity entity_shadowed;
    tnecs_entity entity_fps;
    tnecs_entity selected_unit_entity;
    tnecs_entity hovered_unit_entity;

    i8 moved_direction;
    i8 selected_menu_option;

    struct Point selected_unit_initial_position;
    struct Point selected_unit_moved_position;

    tnecs_entity *map_enemies;
    struct Unit party[SOTA_MAX_PARTY_SIZE];
    i16 party_id_stack[SOTA_MAX_PARTY_SIZE];
    u8  party_size;

    tnecs_entity *ent_unit_loaded;

    i32  cursor_moved_time_ms;

    struct Combat_Outcome    combat_outcome;
    struct Combat_Forecast   combat_forecast;
    struct Combat_Flow       combat_flow;
    struct Combat_Forecast  *AI_forecasts;

    i8 animation_attack;
    i8 chapter;
    i8 state;
    i8 substate;
    i8 state_previous;
    i8 substate_previous;

    SDL_Window  *window;

    struct Settings    settings;
    struct Convoy      convoy;
    u64 s_xoshiro256ss[4]; /* Only used to read s from RNG file */

    struct Point cursor_move;
    bool cursor_frame_moved;
    bool cursor_diagonal;

    /* --- FPS --- */
    float instant_fps; /* frames/time after fps_text->update_time_ns */
    float rolling_fps; /* rolling average of fps */

    /* --- COMBAT --- */
    int candidate;          // potential candidate
    int previous_candidate; // previously selected candidate
    /* -- Chosen by player -- */
    // Also use for non-combat: staff, item use...
    tnecs_entity aggressor; // combat -> player unit
    tnecs_entity defendant; // combat -> player chose

    /* -- Choices list for player -- */

    /* --- UNIT ACTION CANDIDATES --- */
    // copy of one other psm list, used by choosecandidates
    tnecs_entity *candidates;
    /* on attackmap */
    tnecs_entity *defendants;     // combat
    tnecs_entity *patients;       // staff
    /* on neighbouring tiles */
    tnecs_entity *victims;        // rescue
    tnecs_entity *spectators;     // dance
    tnecs_entity *auditors;       // talk
    tnecs_entity *passives;       // trade
    tnecs_entity *openables;      // doors and chests

    struct AI_State ai_state;

    i32   controller_code;
    /* Button interpreted as which input?  */
    u32 inputs[SOTA_BUTTON_END];

    /* The music that will be played. */
    Mix_Music *music;
    Mix_Chunk *soundfx_cursor;
    Mix_Chunk *soundfx_next_turn;

    u64    runtime_ns; // -> millions of years
    bool  *shadow_area;  /* pixels */
    bool   ismouse          : 1;
    bool   iscursor         : 1;
    bool   isrunning        : 1;
    bool   isShadow         : 1;
    bool   fast_forward     : 1;
};
extern struct Game Game_default;

/* --- Constructors/Destructors --- */
void Game_Init(   struct Game *sota);
void Game_Free(   struct Game *sota);
void Game_Startup(struct Game *sota, struct Input_Arguments args);
void Game_AI_Free(struct Game *sota);

/* --- I/O --- */
void Game_saveJSON(struct Game *sota, i16 save_ind);
void Game_loadJSON(struct Game *sota, i16 save_ind);

void Game_Save_Copy(  i16 from, i16 to);
void Game_Save_Delete(i16 save);

/* --- State --- */
void Game_State_Set(   struct Game *sota, i8 ns, char *r);
void Game_subState_Set(struct Game *sota, i8 ns, char *r);

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota);

/* --- Time --- */
void Game_Delay(struct Game *sota, i64 d, u64 c, u64 e);

/* --- FPS --- */
void Game_FPS_Free(  struct Game *sota);
i64  Game_FPS_Delay( struct Game *sota, u64 e);
void Game_FPS_Create(struct Game *sota, i64 ut);

/* --- SETTINGS --- */
void Game_Brightness_Set(struct Game *sota, float bright);
float Game_Brightness_Get(struct Game *sota);

/* --- AUDIO --- */
/* -- Music -- */
void Game_Music_Play( struct Game *sota);
void Game_Music_Stop( struct Game *sota);
void Game_Music_Pause(struct Game *sota);

/* -- Volume -- */
void Game_Volume_Set(        struct Game *sota, int volume);
void Game_Volume_Music_Set(  struct Game *sota, int volume);
int  Game_Volume_Music_Get(  struct Game *sota);
void Game_Volume_SoundFX_Set(struct Game *sota, int volume);
int  Game_Volume_SoundFX_Get(struct Game *sota);

#endif /* GAME_STRUCT_H */
