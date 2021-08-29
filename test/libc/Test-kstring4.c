#include "test_kernel_early.h"

#include "unity.h"

#include "kstring.h"


void setUp(void) {}
void tearDown(void) {}


void  test_kint_to_char_normal_3(void) {
    const char c = kint_to_char(3);

    TEST_ASSERT_EQUAL_HEX8('3', c);
}

void test_kint_to_char_normal_7(void) {
    const char c = kint_to_char(7);

    TEST_ASSERT_EQUAL_HEX8('7', c);
}

void test_kint_to_char_normal_0(void) {
    const char c = kint_to_char(0);

    TEST_ASSERT_EQUAL_HEX8('0', c);
}

void test_kint_to_char_normal_9(void) {
    const char c = kint_to_char(9);

    TEST_ASSERT_EQUAL_HEX8('9', c);
}

void test_kint_to_char_out_of_range_minus_1(void) {
    const char c = kint_to_char(-1);

    TEST_ASSERT_EQUAL_HEX8('\0', c);
}

void test_kint_to_char_out_of_range_10(void) {
    const char c = kint_to_char(10);

    TEST_ASSERT_EQUAL_HEX8('\0', c);
}


void kernel_main(void) {
    serial_init();

    UNITY_BEGIN();


    RUN_TEST(test_kint_to_char_normal_3);
    RUN_TEST(test_kint_to_char_normal_7);
    RUN_TEST(test_kint_to_char_normal_0);
    RUN_TEST(test_kint_to_char_normal_9);
    RUN_TEST(test_kint_to_char_out_of_range_minus_1);
    RUN_TEST(test_kint_to_char_out_of_range_10);


    UNITY_END();

    outb(0xf4, 0x10);
}
