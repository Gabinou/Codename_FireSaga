#include "nourstest.h"
#include "conditions.h"
#include "enums.h"
#include "types.h"
#include "jsonio.h"

void test_conditions() {
    struct Conditions game_cond = Conditions_Game_start;
    struct Conditions line_cond = Conditions_default;

    nourstest_true(Conditions_Match(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead_Order(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Match(&line_cond, &game_cond));

    game_cond = Conditions_Game_start;
    line_cond = Conditions_default;

    /* Line should play if Silou is alive, but she is dead: False*/
    Conditions_Alive_Order(&line_cond, UNIT_ORDER_SILOU);
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));

    game_cond = Conditions_Game_start;
    line_cond = Conditions_default;

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead_Order(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));

    /* Line should play if Silou is dead, but she is alive: False*/
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_NICOLE);
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_OTTO);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));

    /* Line should play if Silou is dead, and she is dead: True*/
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Match(&line_cond, &game_cond));

    game_cond = Conditions_Game_start;
    line_cond = Conditions_default;

    /* Line should play if Silou was recruited, but she wasn't: False*/
    Conditions_Recruited_Order(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));

    /* Line should play if Silou was recruited, and she was: True*/
    Conditions_Recruited_Order(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Match(&line_cond, &game_cond));

    /* Line doesn't care if Silou died: True*/
    Conditions_Dead_Order(&game_cond, UNIT_ORDER_SILOU);
    nourstest_true(Conditions_Match(&line_cond, &game_cond));

    /* Line cares if Silou died: False*/
    Conditions_Alive_Order(&line_cond, UNIT_ORDER_SILOU);
    nourstest_true(!Conditions_Match(&line_cond, &game_cond));
}
