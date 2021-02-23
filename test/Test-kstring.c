#include "unity.h"
#include "kstring.h"

#include <string.h>
#include <stdint.h>

#include <stdio.h>

uint16_t buffer[2048];

void setUp(void) {
    terminal_initialize_test(&buffer[0]); //so that printing doesn't seg fault
}
void tearDown(void) {}


void kint_to_string_util(const char *const expected_text, const int32_t number) {
    char str[128u];
    kint_to_string(number, str, 128u);
    const size_t size_of_str = strlen(expected_text);
    TEST_ASSERT_EQUAL_MEMORY(expected_text, str, size_of_str);
}


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

void test_kint_to_string(void) {
    kint_to_string_util("-12735", -12735);
    kint_to_string_util("784032", 784032);
    kint_to_string_util("0", 0);

    //negative zero should be the same as positive zero since this is integers.
    //contrast this with floats where 0.0f != -0.0f
    kint_to_string_util("0", -0);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cstrlen);
    RUN_TEST(test_cstrcmp);
    RUN_TEST(test_kint_to_string);
    UNITY_END();
    return 0;
}
