
#include "systems/time_system.h"
#include "structs.h"

void Time_Synchronize(tnecs_In *input) {
    /* --- PRELIMINARIES --- */
    SDL_assert(input->data == NULL);
    /* -- Get components arrays -- */
    struct Timer *updatetimer_arr = TNECS_C_ARRAY(input, Timer_ID);
    SDL_assert(updatetimer_arr != NULL);

    for (int o = 0; o < input->num_Es; o++) {
        /* o for order */
        struct Timer *ut = (updatetimer_arr + o);

        if (ut->paused)
            continue;

        /* If reset is true, set timers to 0.
        ** If not, time tick. */
        ut->time_ns     = (!ut->reset) * (ut->time_ns     + input->dt);
        ut->frame_count = (!ut->reset) * (ut->frame_count + 1);
        ut->reset       = false;
    }
}
