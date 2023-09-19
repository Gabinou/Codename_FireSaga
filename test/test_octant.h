#include "nourstest.h"
#include "octant.h"

void test_octant() {
    SOTA_Log_Func("%s " STRINGIZE(__LINE__), __func__);
    /* -- Ternary_Direction_Index -- */
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_EQUAL, SOTA_TERNARY_EQUAL) == 0);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_BELOW, SOTA_TERNARY_EQUAL) == 1);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_ABOVE, SOTA_TERNARY_EQUAL) == 2);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_EQUAL, SOTA_TERNARY_BELOW) == 3);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_BELOW, SOTA_TERNARY_BELOW) == 4);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_ABOVE, SOTA_TERNARY_BELOW) == 5);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_EQUAL, SOTA_TERNARY_ABOVE) == 6);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_BELOW, SOTA_TERNARY_ABOVE) == 7);
    nourstest_true(Ternary_Direction_Index(SOTA_TERNARY_ABOVE, SOTA_TERNARY_ABOVE) == 8);

    /* -- Ternary_Direction_Unbalanced -- */
    struct Point move       = {-1, -1};
    struct Point ternary    = Ternary_Direction_Unbalanced(move);
    nourstest_true(ternary.x == SOTA_TERNARY_BELOW);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    move.x = SOTA_TERNARY_EQUAL;
    ternary    = Ternary_Direction_Unbalanced(move);
    nourstest_true(ternary.x == SOTA_TERNARY_EQUAL);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    move.x = SOTA_TERNARY_ABOVE;
    ternary    = Ternary_Direction_Unbalanced(move);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    move.y = SOTA_TERNARY_EQUAL;
    ternary    = Ternary_Direction_Unbalanced(move);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_EQUAL);

    move.y = SOTA_TERNARY_ABOVE;
    ternary    = Ternary_Direction_Unbalanced(move);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_BELOW);

    /* -- Ternary_Direction_Ternary -- */
    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_NULL);
    nourstest_true(ternary.x == SOTA_TERNARY_EQUAL);
    nourstest_true(ternary.y == SOTA_TERNARY_EQUAL);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_LEFT);
    nourstest_true(ternary.x == SOTA_TERNARY_BELOW);
    nourstest_true(ternary.y == SOTA_TERNARY_EQUAL);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_RIGHT);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_EQUAL);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_TOP);
    nourstest_true(ternary.x == SOTA_TERNARY_EQUAL);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_DIAGONAL_TL);
    nourstest_true(ternary.x == SOTA_TERNARY_BELOW);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_DIAGONAL_TR);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_ABOVE);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_BOTTOM);
    nourstest_true(ternary.x == SOTA_TERNARY_EQUAL);
    nourstest_true(ternary.y == SOTA_TERNARY_BELOW);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_DIAGONAL_BL);
    nourstest_true(ternary.x == SOTA_TERNARY_BELOW);
    nourstest_true(ternary.y == SOTA_TERNARY_BELOW);

    ternary = Ternary_Direction_Ternary(SOTA_DIRECTION_DIAGONAL_BR);
    nourstest_true(ternary.x == SOTA_TERNARY_ABOVE);
    nourstest_true(ternary.y == SOTA_TERNARY_BELOW);

    /* -- Ternary_Moved -- */
    ternary = Ternary_Moved(SOTA_DIRECTION_NULL);
    nourstest_true(ternary.x ==  0);
    nourstest_true(ternary.y ==  0);

    ternary = Ternary_Moved(SOTA_DIRECTION_LEFT);
    nourstest_true(ternary.x == -1);
    nourstest_true(ternary.y ==  0);

    ternary = Ternary_Moved(SOTA_DIRECTION_RIGHT);
    nourstest_true(ternary.x ==  1);
    nourstest_true(ternary.y ==  0);

    ternary = Ternary_Moved(SOTA_DIRECTION_TOP);
    nourstest_true(ternary.x ==  0);
    nourstest_true(ternary.y == -1);

    ternary = Ternary_Moved(SOTA_DIRECTION_DIAGONAL_TL);
    nourstest_true(ternary.x == -1);
    nourstest_true(ternary.y == -1);

    ternary = Ternary_Moved(SOTA_DIRECTION_DIAGONAL_TR);
    nourstest_true(ternary.x ==  1);
    nourstest_true(ternary.y == -1);

    ternary = Ternary_Moved(SOTA_DIRECTION_BOTTOM);
    nourstest_true(ternary.x ==  0);
    nourstest_true(ternary.y ==  1);

    ternary = Ternary_Moved(SOTA_DIRECTION_DIAGONAL_BL);
    nourstest_true(ternary.x == -1);
    nourstest_true(ternary.y ==  1);

    ternary = Ternary_Moved(SOTA_DIRECTION_DIAGONAL_BR);
    nourstest_true(ternary.x ==  1);
    nourstest_true(ternary.y ==  1);

    /* -- Ternary_Direction -- */
    move.x = -1;
    move.y = -1;
    int direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_DIAGONAL_TL);

    move.y = 0;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_LEFT);

    move.y = 1;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_DIAGONAL_BL);

    move.x = 0;
    move.y = -1;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_TOP);

    move.y = 0;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_NULL);

    move.y = 1;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_BOTTOM);

    move.x = 1;
    move.y = -1;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_DIAGONAL_TR);

    move.y = 0;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_RIGHT);

    move.y = 1;
    direction = Ternary_Direction(move);
    nourstest_true(direction == SOTA_DIRECTION_DIAGONAL_BR);
}
