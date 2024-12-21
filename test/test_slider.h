
#include "nourstest.h"
#include "slider.h"


void test_slider_easyineasyout() {

}

void test_slider_geometric() {
    Slider slider = Slider_default;
    slider->update_wait_ns  = POPUP_TILE_SLIDEWAIT_ns;
    slider->slidetype       = SLIDETYPE_GEOMETRIC;
    Slider_Rate_Set(slider, 2.0f, 2.0f);

    /* --- Positive direction --- */
    struct Point start = {0, 0};
    struct Point pos = start;
    struct Point target = {100, 100};
    Slider_Compute_Next(&slider, &pos, &target, go_offscreen);

    /* --- Negative direction --- */
}

void test_slider_linear() {

}

void test_slider() {

    test_slider_geometric();
    test_slider_linear();
    test_slider_easyineasyout();

}