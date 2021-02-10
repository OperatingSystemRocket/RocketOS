#include "unity.h"
#include "cstring.h"

void setUp(void) {}
void tearDown(void) {}

void test_cstrlen(void) {
    TEST_ASSERT_EQUAL_INT(cstrlen("hello"), 4);
    TEST_ASSERT_EQUAL_INT(cstrlen(""), 0);
    TEST_ASSERT_EQUAL_INT(cstrlen("foo"), 3);
    TEST_ASSERT_EQUAL_INT(cstrlen("4"), 1);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    return UNITY_END();
}
