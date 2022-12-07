#include <stdio.h>

#include "tests.h"

bool run_tests(predicate tests[], int n) {
    bool result = true;
    for (size_t i = 0; i < n; i++) {
        printf("===== ↓ Test %zu ↓ =====\n", i + 1);
        if (!tests[i]()) {
            printf("===== ↑ FAILED ↑ =====\n");
            result = false;
        } else {
            printf("===== ↑ PASSED ↑ =====\n");
        }
        printf("\n");
    }
    return result;
}

int main() {
    if (run_tests(global_tests, 5)) {
        printf("RESULT: ALL TESTS PASSED\n");
    } else {
        printf("RESULT: SOME TESTS FAILED\n");
    }

    return 0;
}