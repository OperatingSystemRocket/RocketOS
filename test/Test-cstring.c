#include "unity.h"
#include "cstring.h"

uint16_t buffer[2048];

void setUp(void) {
    terminal_initialize_test(&buffer[0]); //so that printing doesn't seg fault
}
void tearDown(void) {}


void test_cstrlen(void) {
    TEST_ASSERT_EQUAL_INT(5, cstrlen("hello"));
    TEST_ASSERT_EQUAL_INT(0, cstrlen(""));
    TEST_ASSERT_EQUAL_INT(3, cstrlen("foo"));
    TEST_ASSERT_EQUAL_INT(1, cstrlen("4"));

    //testing that the asserts are triggered
    TEST_ASSERT_EQUAL_INT(0, cstrcmp(NULL, NULL));
}

void test_cstrcmp(void) {
    TEST_ASSERT_EQUAL_INT(0, cstrcmp("a", "a"));
    TEST_ASSERT_LESS_THAN_INT32(0, cstrcmp("a", "b"));
    TEST_ASSERT_GREATER_THAN_INT32(0, cstrcmp("b", "a"));
    TEST_ASSERT_EQUAL_INT(0, cstrcmp("b", "b"));

    TEST_ASSERT_EQUAL_INT(0, cstrcmp("sdfgs", "sdfgs"));
    TEST_ASSERT_LESS_THAN_INT32(0, cstrcmp("lkjjio", "lkjkio"));
    TEST_ASSERT_GREATER_THAN_INT32(0, cstrcmp("sbgsd", "sbgjd"));

    //testing that the asserts are triggered
    TEST_ASSERT_EQUAL_INT(0, cstrcmp(NULL, NULL));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    RUN_TEST(test_cstrcmp);
    UNITY_END();
    return 0;
}
