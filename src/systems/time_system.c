
#include "systems/time_system.h"

void timeSynchronize(tnecs_system_input_t *in_input) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* --- PRELIMINARIES --- */
    SDL_assert(in_input->user_data == NULL);
    /* -- Get components arrays -- */
    struct Timer *updatetimer_arr = TNECS_COMPONENTS_LIST(in_input, Timer);
    SDL_assert(updatetimer_arr != NULL);

    for (int order = 0; order < in_input->num_entities; order++) {
        struct Timer *ut = (updatetimer_arr + order);

        if (ut->paused)
            continue;

        /* If reset is true, set timers to 0. If not, increase. */
        ut->time_ns     = (!ut->reset) * (ut->time_ns     + in_input->deltat);
        ut->frame_count = (!ut->reset) * (ut->frame_count + 1);
        ut->reset       = false;
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
