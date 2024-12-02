#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <math.h>
#include <stdio.h>
#include "SDL.h"
#ifdef SOTA_OPENGL
    #include <GL/glew.h>
    #include <SDL_opengl.h>
    #include <GL/glu.h>
#endif
#include "convoy.h"
#include "unit/party.h"
#include "unit/unit.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "tnecs.h"
#include "combat.h"
#include "controller/keyboard.h"
#include "credits.h"
#include "index_shader.h"
#include "index_shader.h"
#include "scene.h"
#include "cutscene.h"
#include "enums.h"
#include "structs.h"
#include "bitfields.h"
#include "input.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "fps_fsm.h"
#include "weapon.h"
#include "slider.h"
#include "game/control.h"
#include "events.h"
#include "nmath.h"
#include "systems/time_system.h"
#include "music.h"
#include "macros.h"
#include "hover.h"
#include "utilities.h"
#include "jsonio.h"
#include "pixelfonts.h"
#include "camp.h"
#include "debug.h"
#include "tile.h"
#include "text.h"
#include "AI.h"
#include "RNG.h"
#include "position.h"
#include "sprite.h"
#include "title_screen.h"
#include "systems/render.h"
#include "systems/slide.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Convoy;
struct Game;
struct Menu;

void Events_Data_Malloc(void);
void Events_Data_Free(void);
void Events_Names_Strings(void);
void Events_Names_Declare(void);
void Events_Receivers_Declare(void);

/* --- Pre game --- */
Input_Arguments IES_Init(int argc, char *argv[]);
void Game_Post_Free(void);

/* --- Constructors/Destructors --- */
struct Game *Game_New(Settings settings);

void Game_Free(   struct Game *sota);
void Game_AI_Free(struct Game *sota);

/* -- Multithreaded constructors -- */
int _Game_New_Tnecs(void *data);
int _Game_New_Alloc(void *data);
int _Game_New_Events(void *data);

/* --- Startup --- */
// Depending on input arguments, IES can play a scene, go directly to a map...
// By default, and for users: Go to title screen.
void Game_Startup_Map(          Game *IES);
void Game_Startup_Scene(        Game *IES);
void Game_Startup_TitleScreen(   Game *IES);

/* --- Stepping --- */
void  Game_Step(            struct Game *sota);
void _Game_Step_Render(     struct Game *sota);
void _Game_Step_Control(    struct Game *sota);
u64  _Game_Step_PreFrame(   struct Game *sota);
void _Game_Step_PostFrame(  struct Game *sota, u64 time);

/* --- I/O --- */
void  Game_saveJSON(struct Game *sota, i16 save_ind);
void  Game_loadJSON(struct Game *sota, i16 save_ind);
void _Game_saveJSON(struct Game *sota, s8  filename);
void _Game_loadJSON(struct Game *sota, s8  filename);

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
void  Game_Brightness_Set(struct Game *sota, float bright);
float Game_Brightness_Get(struct Game *sota);

/* --- DISPLAY --- */
void  Game_Display_Bounds(struct Game *sota);

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

/* -- Battle -- */
void  Game_Battle_Start(struct Game *sota, struct Menu *mc);

#endif /* GAME_STRUCT_H */
