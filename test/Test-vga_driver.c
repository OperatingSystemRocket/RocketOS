#include "unity.h"
#include "vga_driver.h"

void test_strlen(void) {
    TEST_ASSERT_EQUAL_INT(strlen("hello"), 5);
    TEST_ASSERT_EQUAL_INT(strlen(""), 0);
    TEST_ASSERT_EQUAL_INT(strlen("foo"), 3);
    TEST_ASSERT_EQUAL_INT(strlen("4"), 1);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_strlen);
    return UNITY_END();
}
