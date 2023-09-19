#include "popup/popup.h"

struct PopUp PopUp_default = {
    .data =            NULL,
    .visible =         false,
    .type =            SOTA_NULL,
    .target_pos =      {0},
    .update_wait_ns =  POPUP_SLIDEWAIT_ns,
    .timer_ns =        0,
    .slidetype =       SLIDETYPE_GEOMETRIC,
    .draw =            NULL,
    .slidefactor =     2.0f,
};
