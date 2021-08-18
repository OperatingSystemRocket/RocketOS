#include <multiboot.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "unity.h"

#include "kstring.h"


void setUp(void) {}
void tearDown(void) {}


void test_kstrcpy_empty_string(void) {
    char dest[32];

    kmemset(dest, 0xFF, sizeof(dest));

    const char *const result = kstrcpy(dest, "");

    TEST_ASSERT_EQUAL_CHAR('\0', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('\0', result[0]);
}

void test_kstrcpy_normal(void) {
    char dest[32];
    const char *const src = "Hello";

    char *const result = kstrcpy(dest, src);

    TEST_ASSERT_EQUAL_STRING(src, (const char*)dest);
    TEST_ASSERT_EQUAL_STRING(src, result);
}

void test_kstrcpy_null_dest(void) {
    const char *const src = "Hello";

    const char *const result = kstrcpy(NULL, src);

    TEST_ASSERT_NULL(result);
}

void test_kstrcpy_null_src(void) {
    char dest[32];

    const char *const result = kstrcpy(dest, NULL);

    TEST_ASSERT_NULL(result);
}

void test_kstrcpy_null_dest_src(void) {
    const char *const result = kstrcpy(NULL, NULL);

    TEST_ASSERT_NULL(result);
}


void test_kstrncpy_normal_empty_string(void) {
    char dest[32];

    kmemset(dest, 0xFF, sizeof(dest));

    const char *const result = kstrncpy(dest, "", 32u);

    TEST_ASSERT_EQUAL_CHAR('\0', dest[0]);
    TEST_ASSERT_EQUAL_CHAR('\0', result[0]);
}

void test_kstrncpy_normal(void) {
    char dest[32];
    const char *const src = "Hello";

    const char *const result = kstrncpy(dest, src, 32u);

    TEST_ASSERT_EQUAL_STRING(src, (const char*)dest);
    TEST_ASSERT_EQUAL_STRING(src, result);
}

void test_kstrncpy_zero_buffer_size(void) {
    char dest[32];
    const char *const src = "Hello";

    kmemset(dest, 0xFF, sizeof(dest));

    const char *const result = kstrncpy(dest, src, 0u);

    TEST_ASSERT_EACH_EQUAL_CHAR(0xFF, dest, sizeof(dest));
    TEST_ASSERT_EACH_EQUAL_CHAR(0xFF, result, sizeof(dest));
}

void test_kstrncpy_overflow(void) {
    char dest[32];
    const char *const src = "This is a test string";
    const char *const expected_string = "This is";

    kmemset(dest, 0xFF, sizeof(dest));

    const char *const result = kstrncpy(dest, src, 8u);

    TEST_ASSERT_EQUAL_STRING(expected_string, (const char*)dest);
    TEST_ASSERT_EQUAL_STRING(expected_string, result);
}

void test_kstrncpy_null_dest(void) {
    const char *const src = "Hello";

    const char *const result = kstrncpy(NULL, src, 32u);

    TEST_ASSERT_NULL(result);
}

void test_kstrncpy_null_src(void) {
    char dest[32];

    const char *const result = kstrncpy(dest, NULL, 32u);

    TEST_ASSERT_NULL(result);
}

void test_kstrncpy_null_dest_src(void) {
    const char *const result = kstrncpy(NULL, NULL, 32u);

    TEST_ASSERT_NULL(result);
}


void test_kstrspn_fully_unequal(void) {
    const char *const str1 = "Hello";
    const char *const str2 = "Popcorn";

    const int32_t result = kstrspn(str1, str2);

    TEST_ASSERT_EQUAL(0, result);
}

void test_kstrspn_partially_equal(void) {
    const char *const str1 = "Help";
    const char *const str2 = "Hello";

    const int32_t result = kstrspn(str1, str2);

    TEST_ASSERT_EQUAL(3, result);
}

void test_kstrspn_fully_equal(void) {
    const char *const str = "Popcorn";
    const int32_t expected_length_of_str = (int32_t)kstrlen(str);

    const int32_t result = kstrspn(str, str);

    TEST_ASSERT_EQUAL(expected_length_of_str, result);
}

void test_kstrspn_null_str1(void) {
    const char *const str2 = "Hello";
    const int32_t result = kstrspn(NULL, str2);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrspn_null_str2(void) {
    const char *const str1 = "Hello";
    const int32_t result = kstrspn(str1, NULL);

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kstrspn_null_str1_str2(void) {
    const int32_t result = kstrspn(NULL, NULL);

    TEST_ASSERT_EQUAL(-1, result);
}


void test_kstrlen_empty_string(void) {
    TEST_ASSERT_EQUAL_UINT32(0, kstrlen(""));
}

void test_kstrlen_one_char_string(void) {
    TEST_ASSERT_EQUAL_UINT32(1, kstrlen("f"));
}

void test_kstrlen_multi_char_string(void) {
    TEST_ASSERT_EQUAL_UINT32(5, kstrlen("hello"));
}

void test_kstrlen_null(void) {
    TEST_ASSERT_EQUAL_UINT32(0, kstrlen(NULL));
}


void test_kint_to_string_base_10_normal_zero(void) {
    char buffer[32];
    const char *const expected_str = "0";

    const char *const result = kint_to_string(0, buffer, 32u, 10u, false); //last param doesn't matter here

    TEST_ASSERT_EQUAL_STRING(expected_str, buffer);
    TEST_ASSERT_EQUAL_STRING(expected_str, result);
}




void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    (void) mboot_header; //needed for calling convention reasons, but currently unused

    if(serial_init()) { //fails if serial is faulty
        serial_writestring("Serial driver works\n");
    }

    terminal_context_initialize();

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_context_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_context_writestring("The multiboot structure was loaded properly\n");
    }
}


void kernel_main(void) {
    serial_init();

    UNITY_BEGIN();


    RUN_TEST(test_kstrcpy_empty_string);
    RUN_TEST(test_kstrcpy_normal);
    RUN_TEST(test_kstrcpy_null_dest);
    RUN_TEST(test_kstrcpy_null_src);
    RUN_TEST(test_kstrcpy_null_dest_src);

    RUN_TEST(test_kstrncpy_normal_empty_string);
    RUN_TEST(test_kstrncpy_normal);
    RUN_TEST(test_kstrncpy_zero_buffer_size);
    RUN_TEST(test_kstrncpy_overflow);
    RUN_TEST(test_kstrncpy_null_dest);
    RUN_TEST(test_kstrncpy_null_src);
    RUN_TEST(test_kstrncpy_null_dest_src);

    RUN_TEST(test_kstrspn_fully_unequal);
    RUN_TEST(test_kstrspn_partially_equal);
    RUN_TEST(test_kstrspn_fully_equal);
    RUN_TEST(test_kstrspn_null_str1);
    RUN_TEST(test_kstrspn_null_str2);
    RUN_TEST(test_kstrspn_null_str1_str2);

    RUN_TEST(test_kstrlen_empty_string);
    RUN_TEST(test_kstrlen_one_char_string);
    RUN_TEST(test_kstrlen_multi_char_string);
    RUN_TEST(test_kstrlen_null);

    RUN_TEST(test_kint_to_string_base_10_normal_zero);


    UNITY_END();

    outb(0xf4, 0x10);
}
