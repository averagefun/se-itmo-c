#include <stdio.h>

#include "tests.h"

int main() {
    if (run_tests()) {
        printf("RESULT: ALL TESTS PASSED\n");
    } else {
        printf("RESULT: SOME TESTS FAILED\n");
    }

    return 0;
}