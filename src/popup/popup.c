#include "popup/popup.h"

const struct PopUp PopUp_default = {
    .update_wait_ns = POPUP_SLIDEWAIT_ns,
    .slidetype      = SLIDETYPE_GEOMETRIC,
    .slidefactor    = 2.0f,
};
