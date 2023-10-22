#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <stdio.h>
#include "index_shader.h"
#include "narrative.h"
#include "globals.h"
#include "enums.h"
#include "structs.h"
#include "bitfields.h"
#include "input.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "weapon.h"
#include "slider.h"
#include "tnecs.h"
#include "events.h"
#include "nmath.h"
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
#include "unit.h"
#include "nmath.h"
#include "RNG.h"
#include "position.h"
#include "sprite.h"
#include "title_screen.h"
#include "systems/render.h"
#include "systems/slide.h"
#include "combat.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

void Events_Data_Malloc();
void Events_Data_Free();
void Events_Names_Strings();
void Events_Names_Declare();
void Events_Receivers_Declare();

/* --- Game Object --- */
struct Game {
    SDL_Renderer    *renderer;
    SDL_Texture     *render_target;
    #ifdef SOTA_OPENGL
    GLuint         gl_programid;
    SDL_GLContext  gl_context;
    SDL_Window     *gl_window;
    #endif /* SOTA_OPENGL */
    tnecs_world_t  *world;

    tnecs_component_t           timer_typeflag;
    struct Point  cursor_lastpos;

    struct dtab *menu_options_dtab;
    struct dtab *defaultstates_dtab;

    struct dtab *items_dtab;
    struct dtab *weapons_dtab;
    struct dtab *tiles_loaded_dtab;
    struct dtab *units_loaded_dtab;
    tnecs_entity_t *units_loaded;

    // TTF_Font *font;

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

    tnecs_entity_t *menu_stack;
    tnecs_entity_t player_select_menus[MENU_PLAYER_SELECT_NUM]; /* [PLAYER_SELECT_MENU_...] */
    tnecs_entity_t popups[POPUP_TYPE_NUM]; /* [POPUP_TYPE_...] */
    tnecs_entity_t item_select_menu;
    tnecs_entity_t trade_menu;
    tnecs_entity_t staff_select_menu;
    tnecs_entity_t weapon_select_menu;
    tnecs_entity_t stats_menu;
    tnecs_entity_t pre_combat_menu;
    tnecs_entity_t first_menu;
    tnecs_entity_t title;
    tnecs_entity_t GM_menu;
    tnecs_entity_t map_animation;

    char filename_menu[DEFAULT_BUFFER_SIZE];
    char reason[DEFAULT_BUFFER_SIZE];

    tnecs_entity_t entity_cursor;
    tnecs_entity_t entity_mouse;
    tnecs_entity_t entity_transition;
    tnecs_entity_t entity_highlighted;
    tnecs_entity_t entity_shadowed;
    tnecs_entity_t entity_fps;
    tnecs_entity_t selected_unit_entity;
    tnecs_entity_t hovered_unit_entity;

    i8 moved_direction;
    i8 selected_menu_option;

    struct Point selected_unit_initial_position;
    struct Point selected_unit_moved_position;

    tnecs_entity_t *map_enemies;
    struct Unit party[SOTA_MAX_PARTY_SIZE];

    tnecs_entity_t *ent_unit_loaded;

    i32  cursor_moved_time_ms;

    struct Combat_Phase      combat_phases[SOTA_COMBAT_MAX_PHASES];
    struct Combat_Attack    *combat_attacks;
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

    int  window_w;
    int  window_h;

    struct Settings    settings;
    struct TINYMT32_T  tinymt32;
    struct Convoy      convoy;
    // struct Camp camp;

    struct Point cursor_move;
    bool cursor_frame_moved;
    bool cursor_diagonal;

    /* --- COMBAT --- */
    int candidate;          // potential candidate
    int previous_candidate; // previously selected candidate
    /* -- Chosen by player -- */
    // Also use for non-combat: staff, item use...
    tnecs_entity_t aggressor; // combat -> player unit
    tnecs_entity_t defendant; // combat -> player chose

    /* -- Choices list for player -- */

    /* --- UNIT ACTION CANDIDATES --- */
    // copy of one other psm list, used by choosecandidates
    tnecs_entity_t *candidates;
    /* on attackmap */
    tnecs_entity_t *defendants;     // combat
    tnecs_entity_t *patients;       // staff
    /* on neighbouring tiles */
    tnecs_entity_t *victims;        // rescue
    tnecs_entity_t *spectators;     // dance
    tnecs_entity_t *auditors;       // talk
    tnecs_entity_t *passives;       // trade
    tnecs_entity_t *openables;      // doors and chests

    i32   controller_code;

    /* Button interpreted as which input?  */
    u32 inputs[SOTA_BUTTON_END];

    u8    party_size;
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
struct Game *Game_Init();
void Game_Free(   struct Game *sota);
void Game_Startup(struct Game *sota, struct Input_Arguments in_args);


/* --- I/O --- */
void Game_saveJSON(struct Game *sota, i16 save_ind);
void Game_loadJSON(struct Game *sota, i16 save_ind);
void Game_Save_Delete(i16 save_ind);
void Game_Save_Copy(i16 from, i16 to);

/* --- State --- */
void Game_State_Set(   struct Game *sota, i8 ns, const char *r);
void Game_subState_Set(struct Game *sota, i8 ns, const char *r);

/* --- Camera --- */
void Game_Camera_Scroll(struct Game *sota);

/* --- Time --- */
void Game_Delay(struct Game *sota, i64 d, u64 c, u64 e);

/* --- FPS --- */
i64 Game_FPS_Delay(  struct Game *sota, u64 e);
void Game_FPS_Free(  struct Game *sota);
void Game_FPS_Create(struct Game *sota, i64 ut);

#endif /* GAME_STRUCT_H */
