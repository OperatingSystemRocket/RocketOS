#include "unity.h"
#include "vga_driver.h"

#include <string.h>
#include <stdlib.h>


#define VGA_BUFFER_SIZE 80*25

uint16_t buffer[VGA_BUFFER_SIZE];

void setUp(void) {
    terminal_initialize_test(&buffer[0]); //dummy vga driver address so that tests can be written
    terminal_setcolor(VGA_COLOR_RED); //arbitrary color to check against that doesn't deal with background color
}
void tearDown(void) {
    terminal_clear();
}


//utility functions:
static void terminal_writing_common_util(const char *const text, const size_t size, const enum vga_color color, const uint16_t offset) {
    uint16_t *const expected_text = malloc(sizeof(uint16_t)*size); //could use a VLA, but I'm not a fan of VLAs
    for(uint16_t i = 0u; i < size; ++i) {
        if(text[i] == '\n') {
            const uint8_t default_color = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4u;
            expected_text[i] = (uint16_t) ' ' | (uint16_t) default_color << 8u;
        } else {
            expected_text[i] = (uint16_t) text[i] | (uint16_t) color << 8u;
        }
    }
    TEST_ASSERT_EQUAL_MEMORY(expected_text, &buffer[offset], size);

    free(expected_text);
}

static char* parse_util(const char *const text, const uint8_t column_offset) {
    size_t rows = 0u;
    size_t columns = column_offset;

    for(uint16_t i = 0u; i < strlen(text); ++i) {
        if(text[i] != '\n') {
            ++columns;
        } else {
            ++rows;
            columns = 0u;
        }
    }

    char *const ret = malloc((rows*80u + columns) - column_offset + 1u); //the extra byte is for the null terminator
    rows = 0u;
    columns = column_offset;

    for(uint16_t i = 0u; i < strlen(text); ++i) {
        if(text[i] != '\n') {
            ret[(rows*80u + columns) - column_offset] = text[i];
            ++columns;
        } else {
            for(uint16_t j = columns; j < 80u; ++j) {
                //use \n that the user can tell if it is a newline space or an actual space in the string
                ret[rows*80u + j - column_offset] = '\n';
            }
            ++rows;
            columns = 0u;
        }
    }

    ret[(rows*80u + columns) - column_offset] = '\0';

    return ret;
}

static void terminal_write_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    const size_t size = strlen(text);
    terminal_write(text, size);

    //can't be const since free takes void*
    char *const parsed_string = parse_util(text, offset%80u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), color, offset);
    free(parsed_string);
}


//tests:
void test_terminal_clear(void) {
    terminal_writestring("Hello");
    terminal_writestring("Foo, Bar, and Baz had a party.");
    terminal_clear();

    for(uint16_t i = 0u; i < VGA_BUFFER_SIZE; ++i) {
        TEST_ASSERT_EQUAL_CHAR(((uint16_t) ' ' | (uint16_t) VGA_COLOR_RED << 8u), buffer[i]);
    }
}

void test_terminal_setcolor(void) {

}

void test_terminal_putentryat(void) {

}

void test_terminal_putchar(void) {

}

void test_terminal_write(void) {
    size_t offset = 0u;

    terminal_write_util("Hello", VGA_COLOR_RED, offset);
    offset += strlen("Hello");

    terminal_write_util("Foo, Bar, and Baz had a party.", VGA_COLOR_RED, offset);
    offset += strlen("Foo, Bar, and Baz had a party.");

    //can't be const char* since free takes void*
    char *const parsed_string = parse_util("This\nis\ndifficult to read\n", offset%80u);
    terminal_write_util("This\nis\ndifficult to read\n", VGA_COLOR_RED, offset);
    offset += strlen(parsed_string);
    free(parsed_string);
}

void test_terminal_writestring(void) {

}

void test_terminal_writestring_color(void) {

}

void test_terminal_write_color(void) {

}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_terminal_clear);
    RUN_TEST(test_terminal_write);
    UNITY_END();
    return 0;
}
