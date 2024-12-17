#include "cutscene.h"
#include "events.h"

const struct Cutscene Cutscene_default = {
    .time_ns    = 4UL * SOTA_ns,
};

void Cutscene_Free(Cutscene *cutscene) {

}

void Cutscene_Init(Cutscene *cutscene) {

}

// /* --- Player interaction --- */
void Cutscene_Finish(Cutscene *cutscene, Game *sota) {
    /* - Finish scene - */
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Cutscene Animation Finished");
    SDL_assert((cutscene->event > event_Start));
    SDL_assert((cutscene->event < event_End));
    Event_Emit(__func__, SDL_USEREVENT, cutscene->event, NULL, NULL);
}
