#include "nourstest.h"
#include "nstr.h"

#undef ITERATIONS
#define ITERATIONS 100000

void test_nstr() {
    SDL_Log("Testing nstr");
    s8 mac = s8_literal("Mac OS X");
    nourstest_true(mac.num == 8);
    s8 lizard = s8_literal("The lizard is a wizard, by joving rove. I say living hell.");
    nourstest_true(lizard.num == 58);

    /* --- s8_var, s8_literal, s8_mut --- */
    s8 windows1 = s8_var("Windows");
    s8 windows2 = s8_literal("Windows");
    s8 windows3 = s8_mut("Windows");
    nourstest_true(windows1.num == windows2.num);
    nourstest_true(windows2.num == windows3.num);
    nourstest_true(windows1.num == windows3.num);
    nourstest_true(windows1.num == 7);
    nourstest_true(windows2.num == 7);
    nourstest_true(windows3.num == 7);

    nourstest_true(s8equal(windows1, windows2));
    nourstest_true(s8equal(windows2, windows3));
    nourstest_true(s8equal(windows1, windows3));

    s8 windows4 = s8_var(windows3.data);
    s8 windows5 = s8_var(windows3.data);
    nourstest_true(windows4.num == windows5.num);
    nourstest_true(windows1.num == windows4.num);
    nourstest_true(windows1.num == windows5.num);

    /* --- s8cat --- */
    s8 lizard_mut       = s8_mut("The lizard is a wizard, by joving rove.");
    lizard_mut          = s8cat(lizard_mut, windows4);
    s8 lizard_result    = s8_mut("The lizard is a wizard, by joving rove.Windows");
    nourstest_true(s8equal(lizard_mut, lizard_result));

    /* --- s8_Path_Remove_Top --- */
    s8 path_mut = s8_mut("/home/gabinours/Sync/Firesaga/build");
    // SDL_Log("path_mut '%s' %d %d", path_mut.data, path_mut.len, path_mut.num);
    path_mut = s8_Path_Remove_Top(path_mut, '/');
    // SDL_Log("path_mut '%s' %d %d", path_mut.data, path_mut.len, path_mut.num);
    s8 temp = s8_literal("/home/gabinours/Sync/Firesaga");
    nourstest_true(path_mut.num == temp.num);
    nourstest_true(s8equal(path_mut, s8_literal("/home/gabinours/Sync/Firesaga")));

    s8_free(&windows3);
    s8_free(&windows3);
    s8_free(&lizard_mut);
    s8_free(&lizard_result);
    s8_free(&path_mut);
}