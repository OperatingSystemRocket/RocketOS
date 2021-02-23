#include "unity.h"
#include "kstring.h"

uint16_t buffer[2048];

void setUp(void) {
    terminal_initialize_test(&buffer[0]); //so that printing doesn't seg fault
}
void tearDown(void) {}


void test_cstrlen(void) {
    TEST_ASSERT_EQUAL_INT(5, kstrlen("hello"));
    TEST_ASSERT_EQUAL_INT(0, kstrlen(""));
    TEST_ASSERT_EQUAL_INT(3, kstrlen("foo"));
    TEST_ASSERT_EQUAL_INT(1, kstrlen("4"));

    //testing that the asserts are triggered
    TEST_ASSERT_EQUAL_INT(0, kstrcmp(NULL, NULL));
}

void test_cstrcmp(void) {
    TEST_ASSERT_EQUAL_INT(0, kstrcmp("a", "a"));
    TEST_ASSERT_LESS_THAN_INT32(0, kstrcmp("a", "b"));
    TEST_ASSERT_GREATER_THAN_INT32(0, kstrcmp("b", "a"));
    TEST_ASSERT_EQUAL_INT(0, kstrcmp("b", "b"));

    TEST_ASSERT_EQUAL_INT(0, kstrcmp("sdfgs", "sdfgs"));
    TEST_ASSERT_LESS_THAN_INT32(0, kstrcmp("lkjjio", "lkjkio"));
    TEST_ASSERT_GREATER_THAN_INT32(0, kstrcmp("sbgsd", "sbgjd"));

    //testing that the asserts are triggered
    TEST_ASSERT_EQUAL_INT(0, kstrcmp(NULL, NULL));
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    RUN_TEST(test_cstrcmp);
    UNITY_END();
    return 0;
}
