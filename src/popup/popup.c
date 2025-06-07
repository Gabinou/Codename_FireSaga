#include "popup/popup.h"

const struct PopUp PopUp_default = {
    .update_wait_ns = POPUP_SLIDEWAIT_ns,
    .slidetype      = SLIDETYPE_GEOMETRIC,
    .slidefactor    = 2.0f,
};

void PopUp_Init_tnecs(void *voidpopup) {
    PopUp_Init(voidpopup);
}

void PopUp_Init(struct PopUp *popup) {
    *popup = PopUp_default;
}
