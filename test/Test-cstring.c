#include "unity.h"
#include "cstring.h"

void setUp(void) {}
void tearDown(void) {}

void test_cstrlen(void) {
    TEST_ASSERT_EQUAL_INT(5, cstrlen("hello"));
    TEST_ASSERT_EQUAL_INT(0, cstrlen(""));
    TEST_ASSERT_EQUAL_INT(3, cstrlen("foo"));
    TEST_ASSERT_EQUAL_INT(1, cstrlen("4"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    return UNITY_END();
}
