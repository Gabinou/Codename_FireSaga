
#include "nourstest.h"
#include "slider.h"


void test_slider_easyineasyout() {
    Slider slider           = Slider_default;
    slider.slidetype        = SLIDETYPE_EASYINEASYOUT;
    Slider_Ratio_Set(&slider, FPS_DEFAULT_CAP / 2, FPS_DEFAULT_CAP / 2);

    /* --- Positive direction --- */
    struct Point start = {0, 0};
    struct Point pos = start;
    struct Point target = {100, 100};
    Slider_Start(&slider, &pos, &target);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 4);
    nourstest_true(pos.y == 4);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 8);
    nourstest_true(pos.y == 8);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 12);
    nourstest_true(pos.y == 12);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 18);
    nourstest_true(pos.y == 18);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 27);
    nourstest_true(pos.y == 27);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 40);
    nourstest_true(pos.y == 40);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 60);
    nourstest_true(pos.y == 60);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 80);
    nourstest_true(pos.y == 80);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 90);
    nourstest_true(pos.y == 90);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 95);
    nourstest_true(pos.y == 95);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 97);
    nourstest_true(pos.y == 97);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 98);
    nourstest_true(pos.y == 98);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 99);
    nourstest_true(pos.y == 99);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 100);
    nourstest_true(pos.y == 100);
    Slider_Compute_Next(&slider, &pos, &target, false);
    nourstest_true(pos.x == 100);
    nourstest_true(pos.y == 100);
    Slider_Compute_Next(&slider, &pos, &target, false);
    // SDL_Log("%d %d", pos.x, pos.y);
    nourstest_true(pos.x == target.x);
    nourstest_true(pos.y == target.x);
}

void test_slider_geometric() {
    Slider slider           = Slider_default;
    slider.slidetype        = SLIDETYPE_GEOMETRIC;
    Slider_Ratio_Set(&slider, FPS_DEFAULT_CAP / 2, FPS_DEFAULT_CAP / 2);

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
    SDL_Rect rect = {0};
    rect.x       = settings.res.x / 4;
    rect.y       = settings.res.y / 4;

    Slider_Target_Offscreen(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == -rect.w);
    nourstest_true(offscreen.target.y == -rect.h);

    rect.x       = 3 * settings.res.x / 4;
    rect.y       = 3 * settings.res.y / 4;
    Slider_Target_Offscreen(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == settings.res.x);
    nourstest_true(offscreen.target.y == settings.res.y);

    rect.w       = 20;
    rect.h       = 20;
    rect.x       = settings.res.x / 4;
    rect.y       = settings.res.y / 4;

    Slider_Target_Offscreen(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == -rect.w);
    nourstest_true(offscreen.target.y == -rect.h);

    rect.x       = 3 * settings.res.x / 4;
    rect.y       = 3 * settings.res.y / 4;
    Slider_Target_Offscreen(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == settings.res.x);
    nourstest_true(offscreen.target.y == settings.res.y);

    /* -- Slider_Target_Offscreen_Far -- */
    rect.w      = 20;
    rect.h      = 20;
    rect.x      = settings.res.x / 4;
    rect.y      = settings.res.y / 4;

    Slider_Target_Offscreen_Far(&slider, &offscreen, &rect);

    nourstest_true(offscreen.target.x == (rect.x - settings.res.x / 3 - rect.w));
    nourstest_true(offscreen.target.y == (rect.y - settings.res.y / 3 - rect.h));

    rect.x       = 3 * settings.res.x / 4;
    rect.y       = 3 * settings.res.y / 4;
    Slider_Target_Offscreen_Far(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == (rect.x + res.x / 3));
    nourstest_true(offscreen.target.y == (rect.y + res.y / 3));

    rect.w      = 0;
    rect.h      = 0;
    rect.x      = settings.res.x / 4;
    rect.y      = settings.res.y / 4;

    Slider_Target_Offscreen_Far(&slider, &offscreen, &rect);

    nourstest_true(offscreen.target.x == (rect.x - settings.res.x / 3 - rect.w));
    nourstest_true(offscreen.target.y == (rect.y - settings.res.y / 3 - rect.h));

    rect.x       = 3 * settings.res.x / 4;
    rect.y       = 3 * settings.res.y / 4;
    Slider_Target_Offscreen_Far(&slider, &offscreen, &rect);
    nourstest_true(offscreen.target.x == (rect.x + res.x / 3));
    nourstest_true(offscreen.target.y == (rect.y + res.y / 3));

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

    i32 rate = FPS_DEFAULT_CAP / slider.ufactors.ratio[0];
    nourstest_true(pos.x == ((int)(res.x * SLIDER_PERIODIC_XN) + slider.target.x) / rate);
    rate = FPS_DEFAULT_CAP / slider.ufactors.ratio[1];
    nourstest_true(pos.y == ((int)(-res.y * SLIDER_PERIODIC_YP) + slider.target.y) / rate);

    offscreen.go_offscreen = true;
    pos.x       =  res.x * 2;
    pos.y       = -res.y;
    offscreen.target.x =  res.x * 3;
    offscreen.target.y = -res.y;
    SliderOffscreen_Compute_Next(&slider, &offscreen, &pos);

    rate = FPS_DEFAULT_CAP / slider.ufactors.ratio[1];
    nourstest_true(pos.x == ((int)(-res.x * SLIDER_PERIODIC_XP) + slider.target.x) / rate);
    nourstest_true(pos.y == ((int)(res.y * SLIDER_PERIODIC_YN) + slider.target.y) / rate);
}

void test_Slider_Speed() {

    {
        Slider slider           = Slider_default;
        slider.slidetype        = SLIDETYPE_VELOCITY;
        i32 px_vel = 100;
        Slider_Speed_Set(&slider,
                         FPS_DEFAULT_CAP * px_vel,
                         FPS_DEFAULT_CAP * px_vel);
        /* --- Positive direction --- */
        struct Point start = {0, 0};
        struct Point pos = start;
        struct Point target = {
            px_vel * 3 + px_vel / 2,
            px_vel * 3 + px_vel / 2
        };
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == px_vel);
        nourstest_true(pos.y == px_vel);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == px_vel * 2);
        nourstest_true(pos.y == px_vel * 2);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == px_vel * 3);
        nourstest_true(pos.y == px_vel * 3);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == target.x);
        nourstest_true(pos.y == target.y);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == target.x);
        nourstest_true(pos.y == target.y);
    }

    {
        Slider slider           = Slider_default;
        slider.slidetype        = SLIDETYPE_VELOCITY;
        i32 px_vel = 100;
        Slider_Speed_Set(&slider,
                         FPS_DEFAULT_CAP * px_vel,
                         FPS_DEFAULT_CAP * px_vel);
        /* --- Positive direction --- */
        struct Point start = {0, 0};
        struct Point pos = start;
        struct Point target = {
            -px_vel * 3 - px_vel / 2,
                -px_vel * 3 - px_vel / 2
            };
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == -px_vel);
        nourstest_true(pos.y == -px_vel);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == -px_vel * 2);
        nourstest_true(pos.y == -px_vel * 2);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == -px_vel * 3);
        nourstest_true(pos.y == -px_vel * 3);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == target.x);
        nourstest_true(pos.y == target.y);
        Slider_Compute_Next(&slider, &pos, &target, false);
        nourstest_true(pos.x == target.x);
        nourstest_true(pos.y == target.y);
    }
}

void test_slider() {
    test_slider_geometric();
    test_Slider_Speed();
    test_slider_easyineasyout();
}
