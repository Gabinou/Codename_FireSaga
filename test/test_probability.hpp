#include "minctest.h"
#include "probability.hpp"

void test_probability() {
	// Actually testing the box-muller transform, the tinyMT is a little more involved.
	// NOt done here because FUCK THAT.
	tinymt32_t tinyMT1;
	tinymt32_t tinyMT2;

	init_tinyMT(tinyMT1);
	init_tinyMT(tinyMT2);
	lok(single_roll(10, 90));
	lok(!single_roll(99, 90));
	lok(!single_roll(90, 90));
	lok(double_roll(10, 100, 90));
	lok(!double_roll(100, 100, 90));
	lok(!double_roll(80, 100, 90));
	lok(getURN(tinyMT1) == getURN(tinyMT2));
	for (short unsigned int i = 0; i < 10000; i++) {
		lok((getURN(tinyMT1) < 100) && (getURN(tinyMT1) >= 0));
	}
}
