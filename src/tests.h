#ifndef TESTS_H
#define TESTS_H

#include <stdbool.h>

typedef bool (*predicate)();

extern predicate global_tests[];

#endif