#include "events.h"

u32 event_InfiniteRecursive;
u32 event_Recursive;
u32 event_NonRecursive;
void event_InfiniteRecursive(struct Game *sota, SDL_Event *userevent) {
    Event_Emit(__func__, SDL_USEREVENT, event_InfiniteRecursive, NULL, NULL);
}

void event_Recursive(struct Game *sota, SDL_Event *userevent) {
    Event_Emit(__func__, SDL_USEREVENT, event_NonRecursive, NULL, NULL);
}

void event_NonRecursive(struct Game *sota, SDL_Event *userevent) {

}

void test_events_register (void) {
    u32 event_Recursive     = SDL_RegisterEvents(1);
    u32 event_NonRecursive  = SDL_RegisterEvents(1);

    DTAB_ADD(receivers_dtab, &receive_event_Recursive, event_Recursive);
    DTAB_ADD(receivers_dtab, &receive_event_NonRecursive, event_NonRecursive);
#include "names/events.h"
#undef REGISTER_ENUM


}


void test_events_recursive(void) {

}

