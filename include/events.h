#ifndef EVENTS_H
#define EVENTS_H
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
** Events:
**  - Everything player does is an event.
**  - Game state changes are events
**
*/

#include <math.h>
#include "enums.h"
#include "tnecs.h"
#include "nstr.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Game;
struct Convoy;

/* --- CORE --- */
void Event_Emit(   const char *em, u32 et, i32 ec, void *d1, void *d2);
void Event_Receiver_Call(struct Game *IES, u32 et, SDL_Event *event);
void Events_Manage(struct Game *sota);
void Event_Free(SDL_Event *event);

/* -- Controller -- */
tnecs_E Events_Controllers_Check(struct Game *sota, i32 code);

/* --- EVENT NAMES --- */
/* -- Declaration -- */
#define NO_EVENT 0
extern u32 event_Start;
#define REGISTER_ENUM(x, y) extern u32 event_##x;
#include "names/events.h"
#undef REGISTER_ENUM
#define REGISTER_ENUM(x, y) extern u32 event_Input_##x;
#include "names/input.h"
#undef REGISTER_ENUM
extern u32 event_End;

/* -- Constructor/Destructors -- */
extern s8 *event_names;
void Events_Names_Declare(void);
void Events_Names_Alloc(void);
void Events_Names_Free(void);

/* --- HOMEMADE EVENT RECEIVERS --- */
/* -- Function Declaration -- */
// SDL_events are all > 0xFF (255)
// Firesaga Events are all < 100
// TODO: Define max number of events?
#define REGISTER_ENUM(x, y) void receive_event_##x(struct Game * sota, SDL_Event *);
#include "names/events.h"
#undef REGISTER_ENUM

/* -- Receiver types -- */
typedef void (* receiver_t)(struct Game *, SDL_Event *);
extern struct dtab *receivers_dtab;

/* -- Constructor/Destructors -- */
void Events_Receivers_Free(void);
void Events_Receivers_Declare(void);

/* --- SDL_EVENT RECEIVERS --- */
void receive_event_SDL_AUDIODEVICEADDED(       struct Game *sota, SDL_Event *event);
void receive_event_SDL_AUDIODEVICEREMOVED(     struct Game *sota, SDL_Event *event);
/* NOTE: receive_event_SDL_CONTROLLERBUTTONDOWN DOES NOT repeat ever frame pressed */
void receive_event_SDL_CONTROLLERBUTTONDOWN(   struct Game *sota, SDL_Event *event);
void receive_event_SDL_CONTROLLERDEVICEADDED(  struct Game *sota, SDL_Event *event);
void receive_event_SDL_JOYDEVICEADDED(         struct Game *sota, SDL_Event *event);
void receive_event_SDL_JOYDEVICEREMOVED(       struct Game *sota, SDL_Event *event);
void receive_event_SDL_CONTROLLERDEVICEREMOVED(struct Game *sota, SDL_Event *event);
/* NOTE: receive_event_SDL_KEYDOWN REPEATS every frame pressed. */
void receive_event_SDL_KEYDOWN(                struct Game *sota, SDL_Event *event);
void receive_event_SDL_MOUSEBUTTON(            struct Game *sota, SDL_Event *event);
void receive_event_SDL_QUIT(                   struct Game *sota, SDL_Event *event);
void receive_event_SDL_WINDOWEVENT(            struct Game *sota, SDL_Event *event);
void receive_event_SDL_WINDOWEVENT_CLOSE(      struct Game *sota, SDL_Event *event);

#endif /* EVENTS_H */
