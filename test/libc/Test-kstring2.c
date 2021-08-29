#include "test_kernel_early.h"

#include "unity.h"

#include "kstring.h"


void setUp(void) {}
void tearDown(void) {}


void test_kstrchr_normal_success(void) {
    char source[] = "abcdefghijklmnopqrstuvwxyz";
    const char c = 'f';
    const char *const expected = source + 5;

    const char *const result = kstrchr(source, c);

    TEST_ASSERT_EQUAL_PTR(expected, result);
}

void test_kstrchr_normal_duplicate_chars(void) {
    char source[] = "aba";
    const char c = 'a';
    const char *const expected = source;

    const char *const result = kstrchr(source, c);

    TEST_ASSERT_EQUAL_PTR(expected, result);
}

void test_kstrchr_normal_fail(void) {
    char source[] = "abcdefghijklmnopqrstuvwxyz";
    const char c = '2';

    const char *const result = kstrchr(source, c);

    TEST_ASSERT_NULL(result);
}

void test_kstrchr_empty_fail(void) {
    char source[] = "";
    const char c = '2';

    const char *const result = kstrchr(source, c);

    TEST_ASSERT_NULL(result);
}

void test_kstrchr_null(void) {
    const char c = '2';

    const char *const result = kstrchr(NULL, c);

    TEST_ASSERT_NULL(result);
}


void test_kstrrchr_normal_success(void) {
    char source[] = "abcdefghijklmnopqrstuvwxyz";
    const char c = 'f';
    const char *const expected = source + 5;

    const char *const result = kstrrchr(source, c);

    TEST_ASSERT_EQUAL_PTR(expected, result);
}

void test_kstrrchr_normal_duplicate_chars(void) {
    char source[] = "aba";
    const char c = 'a';
    const char *const expected = source + 2;

    const char *const result = kstrrchr(source, c);

    TEST_ASSERT_EQUAL_PTR(expected, result);
}

void test_kstrrchr_normal_fail(void) {
    char source[] = "abcdefghijklmnopqrstuvwxyz";
    const char c = '2';

    const char *const result = kstrrchr(source, c);

    TEST_ASSERT_NULL(result);
}

void test_kstrrchr_empty_fail(void) {
    char source[] = "";
    const char c = '2';

    const char *const result = kstrrchr(source, c);

    TEST_ASSERT_NULL(result);
}

void test_kstrrchr_null(void) {
    const char c = '2';

    const char *const result = kstrrchr(NULL, c);

    TEST_ASSERT_NULL(result);
}


void test_kstrcmp_normal_greater_than(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abd";

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrcmp_normal_less_than(void) {
    const char *const str1 = "abd";
    const char *const str2 = "abc";

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(1, result);
}

void test_kstrcmp_normal_equal(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abc";

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrcmp_null_str1(void) {
    const char *const str1 = NULL;
    const char *const str2 = "abc";

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(-2, result);
}

void test_kstrcmp_null_str2(void) {
    const char *const str1 = "abc";
    const char *const str2 = NULL;

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(-2, result);
}

void test_kstrcmp_null_str1_str2(void) {
    const char *const str1 = NULL;
    const char *const str2 = NULL;

    const int32_t result = kstrcmp(str1, str2);

    TEST_ASSERT_EQUAL(-2, result);
}


void test_kstrncmp_normal_greater_than_same_size(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abd";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrncmp_normal_less_than_same_size(void) {
    const char *const str1 = "abd";
    const char *const str2 = "abc";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(1, result);
}

void test_kstrncmp_normal_equal_same_size(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abc";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrncmp_normal_greater_than_smaller_str_size_equal(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abd";
    const size_t n = 2;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrncmp_normal_less_than_smaller_str_size_equal(void) {
    const char *const str1 = "abd";
    const char *const str2 = "abc";
    const size_t n = 2;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrncmp_normal_equal_smaller_str_size_equal(void) {
    const char *const str1 = "abc";
    const char *const str2 = "abc";
    const size_t n = 2;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrncmp_normal_greater_than_smaller_str_size_greater(void) {
    const char *const str1 = "abc";
    const char *const str2 = "acc";
    const size_t n = 2;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrncmp_normal_smaller_than_smaller_str_size_less(void) {
    const char *const str1 = "acc";
    const char *const str2 = "abc";
    const size_t n = 2;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(1, result);
}

void test_kstrncmp_normal_greater_than_diff_len(void) {
    const char *const str1 = "abcd";
    const char *const str2 = "abd";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrncmp_normal_less_than_diff_len(void) {
    const char *const str1 = "abd";
    const char *const str2 = "abcd";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(1, result);
}

void test_kstrncmp_null_str1(void) {
    const char *const str1 = NULL;
    const char *const str2 = "abc";
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-2, result);
}

void test_kstrncmp_null_str2(void) {
    const char *const str1 = "abc";
    const char *const str2 = NULL;
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-2, result);
}

void test_kstrncmp_null_str1_str2(void) {
    const char *const str1 = NULL;
    const char *const str2 = NULL;
    const size_t n = 3;

    const int32_t result = kstrncmp(str1, str2, n);

    TEST_ASSERT_EQUAL(-2, result);
}


void kernel_main(void) {
    serial_init();

    UNITY_BEGIN();


    RUN_TEST(test_kstrchr_normal_success);
    RUN_TEST(test_kstrchr_normal_duplicate_chars);
    RUN_TEST(test_kstrchr_normal_fail);
    RUN_TEST(test_kstrchr_empty_fail);
    RUN_TEST(test_kstrchr_null);

    RUN_TEST(test_kstrrchr_normal_success);
    RUN_TEST(test_kstrrchr_normal_duplicate_chars);
    RUN_TEST(test_kstrrchr_normal_fail);
    RUN_TEST(test_kstrrchr_empty_fail);
    RUN_TEST(test_kstrrchr_null);

    RUN_TEST(test_kstrcmp_normal_greater_than);
    RUN_TEST(test_kstrcmp_normal_less_than);
    RUN_TEST(test_kstrcmp_normal_equal);
    RUN_TEST(test_kstrcmp_null_str1);
    RUN_TEST(test_kstrcmp_null_str2);
    RUN_TEST(test_kstrcmp_null_str1_str2);

    RUN_TEST(test_kstrncmp_normal_greater_than_same_size);
    RUN_TEST(test_kstrncmp_normal_less_than_same_size);
    RUN_TEST(test_kstrncmp_normal_equal_same_size);
    RUN_TEST(test_kstrncmp_normal_greater_than_smaller_str_size_equal);
    RUN_TEST(test_kstrncmp_normal_less_than_smaller_str_size_equal);
    RUN_TEST(test_kstrncmp_normal_equal_smaller_str_size_equal);
    RUN_TEST(test_kstrncmp_normal_greater_than_smaller_str_size_greater);
    RUN_TEST(test_kstrncmp_normal_smaller_than_smaller_str_size_less);
    RUN_TEST(test_kstrncmp_normal_greater_than_diff_len);
    RUN_TEST(test_kstrncmp_normal_less_than_diff_len);
    RUN_TEST(test_kstrncmp_null_str1);
    RUN_TEST(test_kstrncmp_null_str2);
    RUN_TEST(test_kstrncmp_null_str1_str2);


    UNITY_END();

    outb(0xf4, 0x10);
}
