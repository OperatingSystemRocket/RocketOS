#include "test_kernel_early.h"

#include "unity.h"

#include "kstdlib_constants.h"
#include "kstdlib.h"
#include "kstring.h"


void setUp(void) {}
void tearDown(void) {}


static void test_kmalloced_block(uint32_t *const payload_ptr, const uint32_t payload_mem_magic_value, const uint32_t bytes_of_payload, const uint32_t number_of_allocated_words) {
    for(uint32_t i = 0; i < bytes_of_payload; ++i) {
        TEST_ASSERT_EQUAL_HEX8(payload_mem_magic_value, ((unsigned char*)payload_ptr)[i]);
    }

    const uint32_t first_word = payload_ptr[-3];
    TEST_ASSERT_EQUAL_UINT32(1u, get_allocated_bit(first_word));
    TEST_ASSERT_EQUAL_UINT32(number_of_allocated_words, get_size(first_word));

    const uint32_t prev_ptr = payload_ptr[-2];
    const uint32_t next_ptr = payload_ptr[-1];
    TEST_ASSERT_EQUAL_UINT32(0u, prev_ptr);
    TEST_ASSERT_EQUAL_UINT32(0u, next_ptr);

    const uint32_t last_word = payload_ptr[get_size(first_word-4)];
    TEST_ASSERT_EQUAL_UINT32(1u, get_allocated_bit(last_word));
    TEST_ASSERT_EQUAL_UINT32(number_of_allocated_words, get_size(last_word)); 
}

static uint32_t round_up_to_nearest_multiple_of_four(const uint32_t num) {
    return ((num+3)/4u)*4u;
}

static void test_kmalloc_allocation_and_block(const uint32_t num_of_bytes_to_allocate) {
    uint32_t *const payload_ptr = kmalloc(num_of_bytes_to_allocate);

    const uint32_t PAYLOAD_MEM_MAGIC_VALUE = 0xAAu;
    const uint32_t bytes_of_payload = round_up_to_nearest_multiple_of_four(num_of_bytes_to_allocate);

    kmemset(payload_ptr, PAYLOAD_MEM_MAGIC_VALUE, bytes_of_payload);
    test_kmalloced_block(payload_ptr, PAYLOAD_MEM_MAGIC_VALUE, bytes_of_payload, bytes_to_words(num_of_bytes_to_allocate)+4u);

    kfree(payload_ptr);
}

static void test_zeroed_out_kmalloc_allocation_and_block(const uint32_t num_of_bytes_to_allocate) {
    uint32_t *const payload_ptr = zeroed_out_kmalloc(num_of_bytes_to_allocate);

    test_kmalloced_block(payload_ptr, 0x00u, round_up_to_nearest_multiple_of_four(num_of_bytes_to_allocate), bytes_to_words(num_of_bytes_to_allocate)+4u);

    kfree(payload_ptr);
}


void test_kdynamic_memory_init_head_value(void) {
    const uint32_t *const head_ptr = get_head();

    TEST_ASSERT_NOT_NULL(head_ptr);

    TEST_ASSERT_EQUAL_UINT32(WORDS_IN_PAGE, head_ptr[0]);
    TEST_ASSERT_EQUAL_UINT32(0u, head_ptr[1]);
    TEST_ASSERT_EQUAL_UINT32(0u, head_ptr[2]);
    TEST_ASSERT_EQUAL_UINT32(WORDS_IN_PAGE, head_ptr[WORDS_IN_PAGE-1]);
}


void test_kmalloc_size_zero(void) {
    const void *const ptr = kmalloc(0u);

    TEST_ASSERT_NULL(ptr);

    kfree(ptr);
}

void test_kmalloc_size_one(void) {
    test_kmalloc_allocation_and_block(1u);
}

void test_kmalloc_size_four(void) {
    test_kmalloc_allocation_and_block(4u);
}

void test_kmalloc_size_seventeen(void) {
    test_kmalloc_allocation_and_block(17u);
}


void test_zeroed_out_kmalloc_size_zero(void) {
    const void *const ptr = kmalloc(0u);

    TEST_ASSERT_NULL(ptr);

    kfree(ptr);
}

void test_zeroed_out_kmalloc_size_one(void) {
    test_zeroed_out_kmalloc_allocation_and_block(1u);
}

void test_zeroed_out_kmalloc_size_four(void) {
    test_zeroed_out_kmalloc_allocation_and_block(4u);
}

void test_zeroed_out_kmalloc_size_seventeen(void) {
    test_zeroed_out_kmalloc_allocation_and_block(17u);
}


void test_kfree_null(void) {
    kfree(NULL); //cannot test for invalid memory access/corruption, so testing if it hangs or crashes
}

void test_kfree_single_kmalloced_four_byte_block(void) {
    uint32_t *const payload_ptr = kmalloc(4u);

    const uint32_t first_word = payload_ptr[-3];
    const uint32_t last_word = payload_ptr[get_size(first_word-4)];


    kfree(payload_ptr);

    TEST_ASSERT_NOT_EQUAL_UINT32(first_word, payload_ptr[-3]);

    TEST_ASSERT_EQUAL_UINT32(0u, payload_ptr[-2]);
    TEST_ASSERT_EQUAL_UINT32(0u, payload_ptr[-1]);

    TEST_ASSERT_NOT_EQUAL_UINT32(last_word, payload_ptr[get_size(first_word-4)]);
}

void test_kfree_two_kmalloced_four_byte_blocks(void) {
    uint32_t *const first_allocation_payload_ptr = kmalloc(4u);
    uint32_t *const second_allocation_payload_ptr = kmalloc(4u);
    (void) second_allocation_payload_ptr;


    kfree(first_allocation_payload_ptr);

    const uint32_t first_word_first_allocation = first_allocation_payload_ptr[-3];
    TEST_ASSERT_EQUAL_UINT32(0u, get_allocated_bit(first_word_first_allocation));
    TEST_ASSERT_EQUAL_UINT32(5u, get_size(first_word_first_allocation));

    const uint32_t prev_first_allocation = first_allocation_payload_ptr[-2]; 
    TEST_ASSERT_EQUAL_UINT32(0u, prev_first_allocation);

    const uint32_t next_first_allocation = first_allocation_payload_ptr[-1];
    TEST_ASSERT_EQUAL_UINT16(0u, next_first_allocation);
}


void kernel_main(void) {
    serial_init();
    allocate_init();
    paging_init();
    kdynamic_memory_init();

    UNITY_BEGIN();


    RUN_TEST(test_kdynamic_memory_init_head_value);

    RUN_TEST(test_kmalloc_size_zero);
    RUN_TEST(test_kmalloc_size_one);
    RUN_TEST(test_kmalloc_size_four);
    RUN_TEST(test_kmalloc_size_seventeen);

    RUN_TEST(test_zeroed_out_kmalloc_size_zero);
    RUN_TEST(test_zeroed_out_kmalloc_size_one);
    RUN_TEST(test_zeroed_out_kmalloc_size_four);
    RUN_TEST(test_zeroed_out_kmalloc_size_seventeen);

    RUN_TEST(test_kfree_null);
    RUN_TEST(test_kfree_single_kmalloced_four_byte_block);
    RUN_TEST(test_kfree_two_kmalloced_four_byte_blocks);


    UNITY_END();

    outb(0xf4, 0x10);
}
