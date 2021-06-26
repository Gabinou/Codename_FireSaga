#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "enums.h"
#include "weapon.h"
#include "narrative.h"
#include "map.h"
#include "tnecs.h"
#include "macros.h"
#include "utilities.h"
#include "jsonio.h"
#include "convoy.h"
#include "camp.h"
#include "pathfinding.h"
#include "tile.h"
#include "text.h"
#include "unit.h"
#include "linalg.h"
#include "probability.h"
#include "position.h"
#include "sprite.h"
#include "menu.h"
#include "controllerKeyboard.h"
#include "controllerGamepad.h"
#include "controllerMouse.h"
#include "systemRender.h"
#include "systemControl.h"
#include "controllerTouchpad.h"

//forward declaration
struct Item;
struct MenuOption;
void Events_Data_Malloc();
void Events_Data_Free();
void Events_Names_Declare();
void Events_Receivers_Declare();

extern TTF_Font * Game_font;
extern SDL_Renderer * Game_renderer;

struct Entity_hash {
    uint8_t key;
    tnecs_entity_t value;
};

struct uint8_t_hash {
    uint8_t key;
    uint8_t * value;
};

struct Game {
    struct Point cursor_lastpos;
    struct Point mouse_lastpos;

    float mouse_idletime;
    uint32_t mouse_previous;

    struct uint8_t_hash * menu_options;
    struct uint8_t_hash * defaultstates;

    struct Narrative_Conditions narrative;

    struct Weapons_hash * weapons;
    tnecs_world_t * world;

    struct Tiles_hash * tiles_loaded;
    struct Units_hash * units_loaded;

    struct KeyboardInputMap keyboardInputMap;
    struct GamepadInputMap gamepadInputMap;
    struct MouseInputMap mouseInputMap;

    TTF_Font * menu_font;
    struct Map * map_ptr;

    tnecs_entity_t * menu_stack;
    tnecs_entity_t * menu_loaded;

    char filename_menu[DEFAULT_BUFFER_SIZE];
    char reason[DEFAULT_BUFFER_SIZE];

    tnecs_entity_t entity_cursor;
    tnecs_entity_t entity_mouse;
    tnecs_entity_t entity_transition;
    tnecs_entity_t entity_highlighted;
    tnecs_entity_t entity_shadowed;
    tnecs_entity_t entity_fps;
    tnecs_entity_t selected_unit_entity;
    tnecs_entity_t entity_unit_hovered;

    struct Point selected_unit_initial_position;
    struct Point selected_unit_moved_position;

    struct Unit party[MAX_PARTY_SIZE];

    tnecs_entity_t * ent_unit_loaded;
    void (**menuContentMakers)(struct Game * in_game, void * data_1, void * data_2);

    struct Combat_Outcome_Attack outcome_combat[COMBAT_MAX_ATTACKS];
    struct Combat_Forecast combat_forecast;
    struct Combat_Forecast * AI_forecasts;

    uint8_t Combat_Outcome_Attacks_num;
    bool ismouse;
    bool iscursor;
    bool isrunning;
    int8_t chapter;
    int8_t state_previous;
    int8_t state;
    int8_t substate;

    SDL_Window * window;
    struct Settings settings;
    struct TINYMT32_T tinymt32;
    struct Convoy convoy;
    // struct Camp camp;

    tnecs_entity_t attacker_ent;
    tnecs_entity_t * defenders_ent;
    tnecs_entity_t * spectators_ent;
    tnecs_entity_t * patients_ent;
    tnecs_entity_t * talkers_ent;
    tnecs_entity_t * traders_ent;
    tnecs_entity_t * rescuees_ent;

    uint8_t num_defenders;
    uint8_t num_patients;
    uint8_t num_spectators;
    uint8_t num_talkers;
    uint8_t num_traders;
    uint8_t num_rescuees;
    uint8_t menu_stack_num;
    uint8_t party_size;
} extern Game_default;

extern void Game_clean(struct Game * in_game);
extern void Game_init(struct Game * in_game);
extern void Game_startup(struct Game * in_game, struct Input_Arguments in_args);

extern void Game_FPS_Create(struct Game * in_game, float in_update_time);
extern void Game_FPS_Destroy(struct Game * in_game);

extern void Game_Cursor_Create(struct Game * in_game);
extern void Game_Cursor_Destroy(struct Game * in_game);
extern void Game_Cursor_Enable(struct Game * in_game);
extern void Game_Cursor_Disable(struct Game * in_game);
extern void Game_cursorFocus_onMap(struct Game * in_game);
extern void Game_cursorFocus_onMenu(struct Game * in_game);

extern void Game_Mouse_Create(struct Game * in_game);
extern void Game_Mouse_Destroy(struct Game * in_game);
extern void Game_Mouse_Enable(struct Game * in_game);
extern void Game_Mouse_Disable(struct Game * in_game);
extern void Game_Mouse_State_Set(struct Game * in_game, const int8_t in_menu);

extern uint8_t * Game_menuOptions_Get(struct Game * in_game, int8_t in_menu);
extern tnecs_entity_t Game_menuOptions_Create(struct Game * in_game, tnecs_entity_t in_entity, char * in_name);
extern void Game_menuText_Create(struct Game * in_game, int8_t in_menu);
extern bool Game_Menu_init(struct Game * in_game);
extern void Game_Menu_Create(struct Game * in_game, int8_t in_menu);
extern void Game_Menu_Update(struct Game * in_game, int8_t in_menu);
extern void Game_Menu_Enable(struct Game * in_game, int8_t in_menu);
extern void Game_Menu_Disable(struct Game * in_game, int8_t in_menu);
extern void Game_Menu_Disable_Entity(struct Game * in_game, tnecs_entity_t in_menu_entity);
extern void Game_Menu_Destroy(struct Game * in_game, int8_t in_menu);
extern tnecs_entity_t Game_menuStack_Pop(struct Game * in_game, bool destroy);
extern void Game_menuStack_Push(struct Game * in_game, tnecs_entity_t in_menu_entity);

extern tnecs_entity_t Game_Menu_Get(struct Game * in_game, int8_t in_menu);

extern void Game_makeTurntransition(struct Game * in_game);
extern void Game_setTurntransitiontext(struct Game * in_game, int8_t in_army);

extern void Game_State_Set(struct Game * in_game, const int8_t new_state, const char * reason);
extern void Game_subState_Set(struct Game * in_game, const int8_t new_substate, const char * reason);

extern void Game_saveJSON(struct Game * in_game, const int16_t save_ind);
extern void Game_loadJSON(struct Game * in_game, const int16_t save_ind);
extern void Game_Save_Delete(const int16_t save_ind);
extern void Game_Save_Copy(const int16_t from_ind, const int16_t to_ind);

extern void Game_Map_Destroy(struct Game * in_game);
extern void Game_Map_Load(struct Game * in_game, const int16_t in_map_index);
extern void Game_mapArrivals_Load(struct Game * in_game);

extern void Game_Party_Clear(struct Game * in_game);
extern void Game_Party_Load(struct Game * in_game, int16_t * to_load_ids, size_t load_num);
extern void Game_Party_Unload(struct Game * in_game, int16_t * to_unload, size_t unload_num);

extern void Game_Units_Unload(struct Game * in_game);
extern void Game_Tiles_Unload(struct Game * in_game);

extern void Game_makeTurntransition(struct Game * in_game);
extern void Game_setTurntransitiontext(struct Game * in_game, int8_t in_army);

extern void Game_putPConMap(struct Game * in_game, int16_t * in_units, struct Point * in_pos_list, size_t load_num);
extern struct Combat_Forecast Game_Combat_Forecast(struct Game * in_game, struct Unit * attacker, struct Point * attacker_pos, struct Unit * defender, struct Point * defender_pos);
extern void Game_Combat_Outcome(struct Game * in_game, struct Unit * attacker, struct Unit * defender);
extern void Game_Combat_Resolves(struct Game * in_game, struct Unit * attacker, struct Unit * defender);
extern void Game_Combat_Animate(struct Game * in_game, struct Unit * attacker, struct Unit * defender);

extern void Game_getDefenders(struct Game * in_game, tnecs_entity_t attacker_ent);
extern void Game_getAudience(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_getTalkers(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_getPatients(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_getTraders(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_getRescuees(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_canSeize(struct Game * in_game, tnecs_entity_t moved_ent);
extern void Game_Menu_LocationfromUnit(struct Game * in_game, tnecs_entity_t in_menu_entity, tnecs_entity_t in_unit_entity);
extern void Game_Menu_LocationfromCursor(struct Game * in_game, tnecs_entity_t in_menu_entity);
extern void Game_Menu_LocationfromMouse(struct Game * in_game, tnecs_entity_t in_menu_entity);

extern void Game_subStateSwitch_onMenupop(struct Game * in_game, int8_t menu_popped);

#define REGISTER_ENUM(x) void makeContent_MENU_##x(struct Game * in_game, void * data_1, void * data_2);
#include "names/menu.h"
#undef REGISTER_ENUM

#endif /* GAME_H */