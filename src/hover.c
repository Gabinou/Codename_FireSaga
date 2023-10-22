
#include "hover.h"

struct Hover Hover_default = {
    .width           = SOTA_HOVER_WIDTH_DEFAULT,
    .step            = SOTA_HOVER_STEP_DEFAULT, /* every step, move step [pixels] < width */
    .current         = SOTA_HOVER_WIDTH_DEFAULT,
    .wait_factor     = SOTA_HOVER_WAIT_FACTOR,
    .update_wait_ns  = HOVER_WAIT,
    .timer_ns        = 0,
};

void Hover_Compute_Next(struct Hover *hover, struct Point *pos) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    i32 upper =   hover->width / hover->step + 1;
    i32 lower = - hover->width / hover->step;
    i32 current_diff = q_sequence_pingpong_int32_t(hover->current++, upper, lower);
    i32 next_diff    = q_sequence_pingpong_int32_t(hover->current, upper, lower);
    pos->y += next_diff * hover->step - current_diff * hover->step;
    if ((current_diff == lower) || (current_diff == -lower))
        hover->update_wait_ns /= hover->wait_factor;
    else if ((next_diff == lower) || (next_diff == -lower))
        hover->update_wait_ns *= hover->wait_factor;
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
