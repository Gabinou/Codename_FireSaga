
#include "systems/time_system.h"

void Time_Synchronize(tnecs_system_input *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->user_data == NULL);
    /* -- Get components arrays -- */
    struct Timer *updatetimer_arr = TNECS_COMPONENTS_LIST(input, Timer);
    SDL_assert(updatetimer_arr != NULL);

    for (int order = 0; order < input->num_entities; order++) {
        struct Timer *ut = (updatetimer_arr + order);

        if (ut->paused)
            continue;

        /* If reset is true, set timers to 0. If not, increase. */
        ut->time_ns     = (!ut->reset) * (ut->time_ns     + input->deltat);
        ut->frame_count = (!ut->reset) * (ut->frame_count + 1);
        ut->reset       = false;
    }
}
