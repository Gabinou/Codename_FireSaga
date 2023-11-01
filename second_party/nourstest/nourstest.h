#ifndef __NOURSTEST_H__
#define __NOURSTEST_H__
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static int test_num = 0, fail_num = 0;

static void nourstest_results() {
    char message[5] = "FAIL\0";
    if (fail_num == 0)
        strncpy(message, "PASS", 5);
    printf("%s (%d/%d)\n", message, test_num - fail_num, test_num);
}
#define nourstest_true(test) do {\
    if ((test_num++, !(test))) \
        printf("\n %s:%d error #%d", __FILE__, __LINE__, ++fail_num); \
} while (0)

static void nourstest_run(char * name, void (*test)()) {
    const int ts = test_num, fs = fail_num;
    const clock_t t0 = clock();
    printf("\t%-14s", name), test();
    printf("\t pass: %5d \t fail: %2d \t %4dms\n",
           (test_num - ts) - (fail_num - fs), fail_num - fs,
           (int)((clock() - t0) * 1000 / CLOCKS_PER_SEC));
}
#endif /* __NOURSTEST_H__ */
