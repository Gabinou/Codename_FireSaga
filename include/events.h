#ifndef EVENTS_H
#define EVENTS_H

#include "stdint.h"
#include "game.h"
#include "stb_ds.h"
#include "flecs.h"
#include "unit.h"
#include "narrative.h"
#include "enums.h"
#include "linalg.h"
#include "map.h"
#include "SDL2/SDL.h"

struct Item;
struct Game;

extern ecs_entity_t * data1_entity;
extern ecs_entity_t * data2_entity;

extern tnecs_entity_t * tnecs_data1_entity;
extern tnecs_entity_t * tnecs_data2_entity;

extern void Events_Data_Malloc();
extern void Events_Data_Free();

#define NO_EVENT 0
#define REGISTER_ENUM(x) extern uint32_t event_##x;
#include "names/events.h"
#undef REGISTER_ENUM

extern void Event_Emit(uint32_t in_event_type, int32_t in_event_code, void * in_data1, void * in_data2);
extern void Events_Names_Declare();
extern void Events_Receivers_Declare();
extern ecs_entity_t Events_Controllers_Check(struct Game * in_game, int32_t in_code);
extern tnecs_entity_t Events_Controllers_Check_tnecs(struct Game * in_game, int32_t in_code);


// SDL_events are all > 0x0FF (255 )
// Firesaga Events are all < 100
extern void (**receivers_arr)(struct Game *, SDL_Event *);
typedef void (*receiver_t)(struct Game *, SDL_Event *);
struct Receivers_hash {
    uint32_t key;
    void (*value)(struct Game *, SDL_Event *); //receiver function
};

extern void Events_Manage(struct Game * in_game);

#define REGISTER_ENUM(x) extern void receive_##x(struct Game * in_game, SDL_Event * in_##x);
#include "names/events.h"
#undef REGISTER_ENUM

extern void receive_SDL_MOUSEBUTTON(struct Game * in_game, SDL_Event * in_SDL_MOUSEBUTTON); // received even if there is no motion.
extern void receive_SDL_AUDIODEVICEADDED(struct Game * in_game, SDL_Event * in_SDL_AUDIODEVICEADDED);
extern void receive_SDL_AUDIODEVICEREMOVED(struct Game * in_game, SDL_Event * in_SDL_AUDIODEVICEREMOVED);
extern void receive_SDL_CONTROLLERDEVICEADDED(struct Game * in_game, SDL_Event * in_SDL_CONTROLLERDEVICEADDED);
extern void receive_SDL_QUIT(struct Game * in_game, SDL_Event * in_event);
extern void receive_SDL_WINDOWEVENT(struct Game * in_game, SDL_Event * in_event);
extern void receive_SDL_WINDOWEVENT_CLOSE(struct Game * in_game, SDL_Event * in_event);
extern void receive_SDL_CONTROLLERDEVICEREMOVED(struct Game * in_game, SDL_Event * in_SDL_CONTROLLERDEVICEREMOVED);
extern void receive_SDL_KEYDOWN(struct Game * in_game, SDL_Event * in_SDL_KEYDOWN); //This event REPEATS every frame pressed.
extern void receive_SDL_CONTROLLERBUTTONDOWN(struct Game * in_game, SDL_Event * in_SDL_CONTROLLERBUTTONDOWN); //This event DOES NOT repeat ever frame pressed
extern void receive_SDL_QUIT(struct Game * in_game, SDL_Event * in_SDL_QUIT);

#endif /* EVENTS_H */