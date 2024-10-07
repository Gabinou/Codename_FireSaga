#include "events.h"

u32 event_InfiniteRecursive;
u32 event_Recursive;
u32 event_NonRecursive;
void Event_Emit_Test( const char *emitter, u32 type, i32 code, void *data1, void *data2) {
    SDL_assert(code > 0);
    SDL_assert(type != ((UINT32_MAX) - 1));
    SDL_Event event;
    event.type          = type;
    event.user.code     = code;
    event.user.data1    = data1;
    event.user.data2    = data2;
    SDL_assert(SDL_PushEvent(&event));
}

void receive_event_InfiniteRecursive(SDL_Event *userevent) {
    Event_Emit_Test(__func__, SDL_USEREVENT, event_InfiniteRecursive, NULL, NULL);
}

void receive_event_Recursive(SDL_Event *userevent) {
    Event_Emit_Test(__func__, SDL_USEREVENT, event_NonRecursive, NULL, NULL);
}

void receive_event_NonRecursive(SDL_Event *userevent) {

}

void test_events_register (void) {
    event_Recursive         = SDL_RegisterEvents(1);
    event_NonRecursive      = SDL_RegisterEvents(1);
    event_InfiniteRecursive = SDL_RegisterEvents(1);
}


void Events_Manage_Test() {
    SDL_Event event;

    /* Note: events emitted during SDL_PollEvent loop don't get polled! */
    while (SDL_PollEvent(&event)) {
    loopagain:
        ;
        /* -- Getting receiver -- */
        u32 receiver_key = (event.type == SDL_USEREVENT) ? event.user.code : event.type;
        if (receiver_key == event_Recursive)
            receive_event_Recursive(&event);
        else if (receiver_key == event_NonRecursive)
            receive_event_NonRecursive(&event);
        else if (receiver_key == event_InfiniteRecursive)
            receive_event_InfiniteRecursive(&event);
        else {
            // SDL_Log("%d %d %d", event_Recursive, event_NonRecursive, event_InfiniteRecursive);
            // SDL_Log("%d", receiver_key);
            // SDL_assert(false);
        }
    }

    /* This goto is because events emitted while(SDL_PollEvent(&event)) don't get polled. */
    // This goto ensures that the events get managed recursively until no more events emit events.
    if (SDL_PollEvent(&event))
        goto loopagain;

}


void test_events(void) {

    test_events_register();
    SDL_Log("event_InfiniteRecursive");
    Event_Emit_Test(__func__, SDL_USEREVENT, event_InfiniteRecursive, NULL, NULL);
    Events_Manage_Test();
    SDL_Log("event_InfiniteRecursive DONE");

}

