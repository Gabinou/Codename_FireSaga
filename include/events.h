#ifndef EVENTS_H
#define EVENTS_H

#include <errno.h>
#include "bars/map_hp.h"
#include "game/game.h"
#include "game/cursor.h"
#include "game/popup.h"
#include "game/combat.h"
#include "game/menu.h"
#include "game/map.h"
#include "game/cursor.h"
#include "fsm.h"
#include "tnecs.h"
#include "unit.h"
#include "narrative.h"
#include "enums.h"
#include "arrow.h"
#include "macros.h"
#include "nmath.h"
#include "globals.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Game;

/* --- EVENT DATA ENTITIES --- */
extern tnecs_entity_t *data1_entity;
extern tnecs_entity_t *data2_entity;

/* --- CORE --- */
extern void Event_Emit(const char *em, u32 et, i32 ec, void *d1, void *d2);
extern void Events_Manage(struct Game *sota);

/* -- Constructor/Destructor -- */
extern void Events_Data_Malloc();
extern void Events_Data_Free();

/* -- Controller -- */
extern tnecs_entity_t Events_Controllers_Check(struct Game *sota, i32 code);

/* --- EVENT NAMES --- */
/* -- Declaration -- */
#define NO_EVENT 0
#define REGISTER_ENUM(x) extern u32 event_##x;
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x) extern u32 event_Input_##x;
#include "names/input.h"
#undef REGISTER_ENUM

/* -- Constructor/Destructors -- */
extern char **event_names;
extern void Events_Names_Declare();
extern void Events_Names_Alloc();
extern void Events_Names_Free();

/* --- HOMEMADE EVENT RECEIVERS --- */
/* -- Function Declaration -- */
// SDL_events are all > 0xFF (255)
// Firesaga Events are all < 100
// TODO: Define max number of events?
#define REGISTER_ENUM(x) extern void receive_event_##x(struct Game * sota, SDL_Event *);
#include "names/events.h"
#undef REGISTER_ENUM

/* -- Receiver types -- */
typedef void (* receiver_t)(struct Game *, SDL_Event *);
extern struct dtab *receivers_dtab;

/* -- Constructor/Destructors -- */
extern void Events_Receivers_Free();
extern void Events_Receivers_Declare();

/* --- SDL_EVENT RECEIVERS --- */
extern void receive_event_SDL_AUDIODEVICEADDED(       struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_AUDIODEVICEREMOVED(     struct Game *sota, SDL_Event *event);
/* NOTE: receive_event_SDL_CONTROLLERBUTTONDOWN DOES NOT repeat ever frame pressed */
extern void receive_event_SDL_CONTROLLERBUTTONDOWN(   struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_CONTROLLERDEVICEADDED(  struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_JOYDEVICEADDED(         struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_JOYDEVICEREMOVED(       struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_CONTROLLERDEVICEREMOVED(struct Game *sota, SDL_Event *event);
/* NOTE: receive_event_SDL_KEYDOWN REPEATS every frame pressed. */
extern void receive_event_SDL_KEYDOWN(                struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_MOUSEBUTTON(            struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_QUIT(                   struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_WINDOWEVENT(            struct Game *sota, SDL_Event *event);
extern void receive_event_SDL_WINDOWEVENT_CLOSE(      struct Game *sota, SDL_Event *event);

#endif /* EVENTS_H */
