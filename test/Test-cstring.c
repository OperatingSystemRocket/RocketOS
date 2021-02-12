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

void test_cstrcmp(void) {
    TEST_ASSERT_EQUAL_INT(0, cstrcmp("a", "a"));
    TEST_ASSERT_LESS_THAN_INT32(0, cstrcmp("a", "b"));
    TEST_ASSERT_GREATER_THAN_INT32(0, cstrcmp("b", "a"));
    TEST_ASSERT_EQUAL_INT(0, cstrcmp("b", "b"));

    TEST_ASSERT_EQUAL_INT(0, cstrcmp("sdfgs", "sdfgs"));
    TEST_ASSERT_LESS_THAN_INT32(0, cstrcmp("lkjjio", "lkjkio"));
    TEST_ASSERT_GREATER_THAN_INT32(0, cstrcmp("sbgsd", "sbgjd"));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    RUN_TEST(test_cstrcmp);
    return UNITY_END();
}
