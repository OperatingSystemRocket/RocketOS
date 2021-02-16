#include "unity.h"


void setUp(void) {
    terminal_initialize_test(&buffer[0]); //so that printing doesn't seg fault
}
void tearDown(void) {}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    RUN_TEST(test_cstrcmp);
    UNITY_END();
    return 0;
}
