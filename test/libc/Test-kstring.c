#include <multiboot.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "unity.h"

#include "kstring.h"


void setUp(void) {}
void tearDown(void) {}


void test_kmemcpy_normal(void) {
    const uint32_t source_buffer[] = {0xDEAD, 0xBEEF};
    uint32_t dest_buffer[] = {0x0, 0x0};
    const uint32_t expected_buffer[] = {0xDEAD, 0xBEEF};

    const size_t number_of_elements = sizeof(source_buffer) / sizeof(uint32_t);

    const uint32_t *const result = kmemcpy((uint32_t*)dest_buffer, (const uint32_t*)source_buffer, sizeof(source_buffer));

    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, (const uint32_t*)source_buffer, number_of_elements);
    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, (const uint32_t*)dest_buffer, number_of_elements);
    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, result, number_of_elements);
}

void test_kmemcpy_with_null_destination(void) {
    uint32_t *const dest_buffer = NULL;
    const uint32_t source_buffer[] = {0xDEAD, 0xBEEF};

    const uint32_t *const result = kmemcpy(dest_buffer, (const uint32_t*)source_buffer, sizeof(source_buffer));

    TEST_ASSERT_NULL(dest_buffer);
    TEST_ASSERT_NULL(result);
}

void test_kmemcpy_with_null_source(void) {
    const uint32_t *const source_buffer = NULL;
    uint32_t dest_buffer[] = {0x0, 0x0};

    const uint32_t *const result = kmemcpy((uint32_t*)dest_buffer, source_buffer, sizeof(dest_buffer));

    TEST_ASSERT_NULL(source_buffer);
    TEST_ASSERT_NULL(result);
}


void test_kmemmove_normal_with_complete_overlap(void) {
    uint32_t initial_buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};
    const uint32_t expected_buffer[] = {0xDEAD, 0x54, 0x97, 0x83, 0x12, 0x10, 0x12, 0x10};

    const size_t from_index = 3u;
    const int32_t to_index = 1u;

    const uint32_t *const copy_from = &initial_buffer[from_index];
    uint32_t *const copy_to = &initial_buffer[to_index];

    const size_t number_of_elements = sizeof(initial_buffer)/sizeof(uint32_t);

    const uint32_t *const result = kmemmove(copy_to, copy_from, (number_of_elements-from_index)*sizeof(uint32_t));

    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, (const uint32_t*)initial_buffer, number_of_elements);
    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, &result[-to_index], number_of_elements);
}

void test_kmemmove_with_null_destination(void) {
    uint32_t initial_buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};
    const uint32_t expected_buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};

    const size_t from_index = 3u;

    const uint32_t *const copy_from = &initial_buffer[from_index];
    uint32_t *const copy_to = NULL;

    const size_t number_of_elements = sizeof(initial_buffer)/sizeof(uint32_t);

    const uint32_t *const result = kmemmove(copy_to, copy_from, (number_of_elements-from_index)*sizeof(uint32_t));

    TEST_ASSERT_NULL(copy_to);
    TEST_ASSERT_NULL(result);

    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, (const uint32_t*)initial_buffer, number_of_elements);
}

void test_kmemmove_with_null_source(void) {
    uint32_t initial_buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};
    const uint32_t expected_buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};

    const int32_t to_index = 1u;

    const uint32_t *const copy_from = NULL;
    uint32_t *const copy_to = &initial_buffer[to_index];

    const size_t number_of_elements = sizeof(initial_buffer)/sizeof(uint32_t);

    const uint32_t *const result = kmemmove(copy_to, copy_from, number_of_elements*sizeof(uint32_t));

    TEST_ASSERT_NULL(copy_from);
    TEST_ASSERT_NULL(result);

    TEST_ASSERT_EQUAL_UINT32_ARRAY((const uint32_t*)expected_buffer, (const uint32_t*)initial_buffer, number_of_elements);
}


void test_kmemchr_normal_success(void) {
    unsigned char source_buffer[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x70, 0x09, 0x02, 0x03};

    const unsigned char *const expected_result = &source_buffer[5];

    const unsigned char *const result = kmemchr((unsigned char*) source_buffer, 0x09, sizeof(source_buffer));

    TEST_ASSERT_EQUAL_PTR(expected_result, result);
}

void test_kmemchr_normal_failure(void) {
    unsigned char source_buffer[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x70, 0x09, 0x02, 0x03};

    const unsigned char *const result = kmemchr((unsigned char*) source_buffer, 0x81, sizeof(source_buffer));

    TEST_ASSERT_NULL(result);
}

void test_kmemchr_null(void) {
    unsigned char source_buffer[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x70, 0x09, 0x02, 0x03};

    const unsigned char *const result = kmemchr(NULL, 0x09, sizeof(source_buffer));

    TEST_ASSERT_NULL(result);
}


void test_kmemcmp_normal_less_than(void) {
    const char *const str1 = "abc";
    const char *const str2 = "cde";

    const int32_t result = kmemcmp(str1, str2, kstrlen(str1));

    TEST_ASSERT_EQUAL(-1, result);
}

void test_kmemcmp_normal_equal(void) {
    const char str1[] = "abc";
    const char str2[] = "abc";

    const int32_t result = kmemcmp((const char*)str1, (const char*)str2, kstrlen(str1));

    TEST_ASSERT_EQUAL(0, result);
}

void test_kmemcmp_normal_greater_than(void) {
    const char *const str1 = "cde";
    const char *const str2 = "abc";

    const int32_t result = kmemcmp(str1, str2, kstrlen(str1));

    TEST_ASSERT_EQUAL(1, result);
}

void test_kmemcmp_null_str1(void) {
    const char *const str2 = "abc";

    const int32_t result = kmemcmp(NULL, str2, kstrlen(str2));

    TEST_ASSERT_EQUAL(-2, result);
}

void test_kmemcmp_null_str2(void) {
    const char *const str1 = "cde";

    const int32_t result = kmemcmp(str1, NULL, kstrlen(str1));

    TEST_ASSERT_EQUAL(-2, result);
}


void test_kmemset_normal(void) {
    uint32_t buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};

    const uint32_t *const result = kmemset(buffer, 0xFF, sizeof(buffer));

    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, buffer, sizeof(buffer));
    TEST_ASSERT_EACH_EQUAL_UINT8(0xFF, result, sizeof(buffer));
}

void test_kmemset_null_buffer(void) {
    const size_t number_of_elements = 15u;

    const uint32_t *const result = kmemset(NULL, 0xFF, number_of_elements);

    TEST_ASSERT_NULL(result);
}


void test_kstrcat_empty(void) {
    char buffer[32];
    buffer[0] = '\0';
    const char *const result = kstrcat(buffer, "abc");

    TEST_ASSERT_EQUAL_STRING("abc", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("abc", (const char*)result);
}

void test_kstrcat_normal(void) {
    char buffer[32] = "abc";
    const char *const result = kstrcat(buffer, "cdef");

    TEST_ASSERT_EQUAL_STRING("abccdef", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("abccdef", (const char*)result);
}

void test_kstrcat_null_destination(void) {
    const char *const result = kstrcat(NULL, "abc");

    TEST_ASSERT_NULL(result);
}

void test_kstrcat_null_source(void) {
    char buffer[32] = "abc";
    const char *const result = kstrcat(buffer, NULL);

    TEST_ASSERT_NULL(result);
}


void test_kstrncat_empty(void) {
    char buffer[32];
    buffer[0] = '\0';
    const char *const result = kstrncat(buffer, "abc", 32u);

    TEST_ASSERT_EQUAL_STRING("abc", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("abc", (const char*)result);
}

void test_kstrncat_normal(void) {
    char buffer[32] = "abc";
    const char *const result = kstrncat(buffer, "cdef", 32u);

    TEST_ASSERT_EQUAL_STRING("abccdef", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("abccdef", (const char*)result);
}

void test_kstrncat_empty_overflow(void) {
    char buffer[2];
    buffer[0] = '\0';
    const char *const result = kstrncat(buffer, "abc", 2u);

    TEST_ASSERT_EQUAL_STRING("a", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("a", (const char*)result);
}

void test_kstrncat_normal_overflow(void) {
    char buffer[6] = "abc";
    const char *const result = kstrncat(buffer, "cdef", 6u);

    TEST_ASSERT_EQUAL_STRING("abccd", (const char*)buffer);
    TEST_ASSERT_EQUAL_STRING("abccd", (const char*)result);
}

void test_kstrncat_null_destination(void) {
    const char *const result = kstrncat(NULL, "abc", 32u);

    TEST_ASSERT_NULL(result);
}

void test_kstrncat_null_source(void) {
    char buffer[32] = "abc";
    const char *const result = kstrncat(buffer, NULL, 32u);

    TEST_ASSERT_NULL(result);
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


    RUN_TEST(test_kmemcpy_normal);
    RUN_TEST(test_kmemcpy_with_null_destination);
    RUN_TEST(test_kmemcpy_with_null_source);

    RUN_TEST(test_kmemmove_normal_with_complete_overlap);
    RUN_TEST(test_kmemmove_with_null_destination);
    RUN_TEST(test_kmemmove_with_null_source);

    RUN_TEST(test_kmemchr_normal_success);
    RUN_TEST(test_kmemchr_normal_failure);
    RUN_TEST(test_kmemchr_null);

    RUN_TEST(test_kmemcmp_normal_less_than);
    RUN_TEST(test_kmemcmp_normal_equal);
    RUN_TEST(test_kmemcmp_normal_greater_than);
    RUN_TEST(test_kmemcmp_null_str1);
    RUN_TEST(test_kmemcmp_null_str2);

    RUN_TEST(test_kmemset_normal);
    RUN_TEST(test_kmemset_null_buffer);

    RUN_TEST(test_kstrcat_empty);
    RUN_TEST(test_kstrcat_normal);
    RUN_TEST(test_kstrcat_null_destination);
    RUN_TEST(test_kstrcat_null_source);

    RUN_TEST(test_kstrncat_empty);
    RUN_TEST(test_kstrncat_normal);
    RUN_TEST(test_kstrncat_empty_overflow);
    RUN_TEST(test_kstrncat_normal_overflow);
    RUN_TEST(test_kstrncat_null_destination);
    RUN_TEST(test_kstrncat_null_source);


    UNITY_END();

    outb(0xf4, 0x10);
}
