
#include "nourstest.h"
#include "slider.h"


void test_slider_easyineasyout() {

}

void test_slider_geometric() {
    Slider slider = Slider_default;
    slider.update_wait_ns  = POPUP_TILE_SLIDEWAIT_ns;
    slider.slidetype       = SLIDETYPE_GEOMETRIC;
    Slider_Rate_Set(&slider, 2.0f, 2.0f);

    /* --- Positive direction --- */
    struct Point start = {0, 0};
    struct Point pos = start;
    struct Point target = {100, 100};
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 50);
    nourstest_true(pos.y == 50);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 75);
    nourstest_true(pos.y == 75);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 87);
    nourstest_true(pos.y == 87);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 93);
    nourstest_true(pos.y == 93);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 96);
    nourstest_true(pos.y == 96);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 98);
    nourstest_true(pos.y == 98);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 99);
    nourstest_true(pos.y == 99);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 100);
    nourstest_true(pos.y == 100);

    /* --- Negative direction --- */
    start.x     = 0;
    start.y     = 0;
    pos.x       = start.x;
    pos.y       = start.y;
    target.x    = -100;
    target.y    = -100;
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -50);
    nourstest_true(pos.y == -50);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -75);
    nourstest_true(pos.y == -75);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -87);
    nourstest_true(pos.y == -87);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -93);
    nourstest_true(pos.y == -93);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -96);
    nourstest_true(pos.y == -96);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -98);
    nourstest_true(pos.y == -98);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -99);
    nourstest_true(pos.y == -99);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == -100);
    nourstest_true(pos.y == -100);

    /* --- Offscreen --- */
    /* -- Slider_Target_Offscreen -- */
    struct Settings settings = Settings_default;
    struct Point res = settings.res;

    struct SliderOffscreen offscreen = SliderOffscreen_default;
    offscreen.settings = &settings;

    pos.x       = settings.res.x / 4;
    pos.y       = settings.res.y / 4;
    Slider_Target_Offscreen(&slider, &offscreen, &pos);
    nourstest_true(offscreen.target.x == pos.x - settings.res.x);
    nourstest_true(offscreen.target.y == pos.y - settings.res.y);

    pos.x       = 3 * settings.res.x / 4;
    pos.y       = 3 * settings.res.y / 4;
    Slider_Target_Offscreen(&slider, &offscreen, &pos);
    nourstest_true(offscreen.target.x == pos.x + settings.res.x);
    nourstest_true(offscreen.target.y == pos.y + settings.res.y);

    /* -- SliderOffscreen_Compute_Next -- */
    /* - go_offscreen false - */
    // SliderOffscreen_Compute_Next === Slider_Compute_Next
    offscreen.go_offscreen = false;
    pos.x       = start.x;
    pos.y       = start.y;
    slider.target.x    = -100;
    slider.target.y    = -100;
    SliderOffscreen_Compute_Next(&slider, &offscreen, &pos);
    nourstest_true(pos.x == -50);
    nourstest_true(pos.y == -50);
    SliderOffscreen_Compute_Next(&slider, &offscreen, &pos);
    nourstest_true(pos.x == -75);
    nourstest_true(pos.y == -75);

    /* - go_offscreen true - */
    offscreen.go_offscreen = true;
    pos.x       = -res.x;
    pos.y       =  res.y * 2;
    offscreen.target.x = -res.x;
    offscreen.target.y =  res.y * 3;
    SliderOffscreen_Compute_Next(&slider, &offscreen, &pos);

    nourstest_true(pos.x == ((int)(res.x * SLIDER_PERIODIC_XN) + slider.target.x) /
                   slider.ufactors.rate[0]);
    nourstest_true(pos.y == ((int)(-res.y * SLIDER_PERIODIC_YP) + slider.target.y) /
                   slider.ufactors.rate[1]);

    offscreen.go_offscreen = true;
    pos.x       =  res.x * 2;
    pos.y       = -res.y;
    offscreen.target.x =  res.x * 3;
    offscreen.target.y = -res.y;
    SliderOffscreen_Compute_Next(&slider, &offscreen, &pos);

    nourstest_true(pos.x == ((int)(-res.x * SLIDER_PERIODIC_XP) + slider.target.x) /
                   slider.ufactors.rate[0]);
    nourstest_true(pos.y == ((int)(res.y * SLIDER_PERIODIC_YN) + slider.target.y) /
                   slider.ufactors.rate[1]);
}

void test_slider_linear() {

}

void test_slider() {
    test_slider_geometric();
    test_slider_linear();
    test_slider_easyineasyout();
}
