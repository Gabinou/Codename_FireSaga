#ifndef GAME_STRUCT_H
#define GAME_STRUCT_H

#include <stdio.h>
#include "convoy.h"
#include "settings.h"
#include "unit/party.h"
#include "unit/unit.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include "tnecs.h"
#include "combat.h"
#include "controller/keyboard.h"
#include "credits.h"
#include "index_shader.h"
#include "narrative.h"
#include "enums.h"
#include "structs.h"
#include "bitfields.h"
#include "input.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "fps_fsm.h"
#include "weapon.h"
#include "slider.h"
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
#include "nmath.h"
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

void Events_Data_Malloc(void);
void Events_Data_Free(void);
void Events_Names_Strings(void);
void Events_Names_Declare(void);
void Events_Receivers_Declare(void);

/* --- Pre game --- */
void Pre_Game_Startup(int argc, char *argv[]);
void Pre_Game_Free(void);

/* --- Constructors/Destructors --- */
void Game_Init(   struct Game *sota, int argc, char *argv[]);
void Game_Free(   struct Game *sota);
void Game_AI_Free(struct Game *sota);

/* --- Stepping --- */
void  Game_Step(            struct Game *sota);
void _Game_Step(            struct Game *sota);
void _Game_Step_Render(     struct Game *sota);
u64  _Game_Step_PreFrame(   struct Game *sota);
void _Game_Step_PostFrame(  struct Game *sota, u64 time);

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
