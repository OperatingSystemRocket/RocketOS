#include "unity.h"
#include "vga_driver.h"

#include <string.h>
#include <stdlib.h>

#include <stdio.h>


#define VGA_BUFFER_SIZE 80*25

uint16_t buffer[VGA_BUFFER_SIZE];

void setUp(void) {
    terminal_initialize_test(&buffer[0]); //dummy vga driver address so that tests can be written
    terminal_setcolor(VGA_COLOR_RED); //arbitrary color to check against that doesn't deal with background color
}
void tearDown(void) {}


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

void terminal_resetcolor_util(const enum vga_color color) {
    terminal_setcolor(color);

    terminal_resetcolor();

    terminal_writestring("This is some \n example\n text\n"); //can be any function that does a text write without changing color
    char *const parsed_string = parse_util("This is some \n example\n text\n", 0u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4u, 0u);
    free(parsed_string);

    terminal_initialize_test(&buffer[0]);
}

void terminal_setcolor_util(const enum vga_color color) {
    terminal_setcolor(color);

    terminal_writestring("This is some \n example\n text\n"); //can be any function that does a text write without changing color
    char *const parsed_string = parse_util("This is some \n example\n text\n", 0u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), color, 0u);
    free(parsed_string);

    terminal_initialize_test(&buffer[0]);
}

static void terminal_write_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    const size_t size = strlen(text);
    terminal_write(text, size);

    //can't be const since free takes void*
    char *const parsed_string = parse_util(text, offset%80u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), color, offset);
    free(parsed_string);
}

static void terminal_writestring_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    terminal_writestring(text);

    //can't be const since free takes void*
    char *const parsed_string = parse_util(text, offset%80u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), color, offset);
    free(parsed_string);
}

static void terminal_writestring_color_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    terminal_writestring_color(text, color);

    //can't be const since free takes void*
    char *const parsed_string = parse_util(text, offset%80u);
    terminal_writing_common_util(parsed_string, strlen(parsed_string), color, offset);
    free(parsed_string);
}

static void terminal_write_color_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    const size_t size = strlen(text);
    terminal_write_color(text, size, color);

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

void test_terminal_resetcolor(void) {
    terminal_resetcolor_util(VGA_COLOR_LIGHT_BLUE);
    terminal_resetcolor_util(VGA_COLOR_CYAN);
    terminal_resetcolor_util(VGA_COLOR_MAGENTA);
}

void test_terminal_setcolor(void) {
    terminal_setcolor_util(VGA_COLOR_LIGHT_BLUE);
    terminal_setcolor_util(VGA_COLOR_CYAN);
    terminal_setcolor_util(VGA_COLOR_MAGENTA);
}

void test_terminal_putentryat(void) {
    terminal_putentryat('r', VGA_COLOR_BLACK, 54u, 11u);
    TEST_ASSERT_EQUAL_CHAR((uint16_t) 'r' | (uint16_t) VGA_COLOR_BLACK << 8u, buffer[11u*80u + 54u]);

    terminal_putentryat('a', VGA_COLOR_LIGHT_GREEN, 6u, 7u);
    TEST_ASSERT_EQUAL_CHAR((uint16_t) 'a' | (uint16_t) VGA_COLOR_LIGHT_GREEN << 8u, buffer[7u*80u + 6u]);

    terminal_putentryat('u', VGA_COLOR_BLUE, 34u, 24u);
    TEST_ASSERT_EQUAL_CHAR((uint16_t) 'u' | (uint16_t) VGA_COLOR_BLUE << 8u, buffer[24u*80u + 34u]);

    terminal_putentryat('i', VGA_COLOR_RED, 79u, 24u);
    TEST_ASSERT_EQUAL_CHAR((uint16_t) 'i' | (uint16_t) VGA_COLOR_RED << 8u, buffer[24u*80u + 79u]);

    terminal_putentryat('>', VGA_COLOR_CYAN, 79u, 0u);
    TEST_ASSERT_EQUAL_CHAR((uint16_t) '>' | (uint16_t) VGA_COLOR_CYAN << 8u, buffer[0u*80u + 79u]);
}

void test_terminal_putchar(void) {
    size_t offset = 0u;

    char *const parsed_string = parse_util("\n", offset);
    terminal_putchar('\n');
    terminal_writing_common_util(parsed_string, strlen(parsed_string), VGA_COLOR_RED, offset);
    offset += strlen(parsed_string);
    free(parsed_string);


    terminal_putchar('e');
    terminal_writing_common_util("e", 1u, VGA_COLOR_RED, offset);
    offset += 1u;


    terminal_putchar('g');
    terminal_writing_common_util("g", 1u, VGA_COLOR_RED, offset);
    offset += 1u;


    char *const parsed_string1 = parse_util("\n", offset%80u);
    terminal_putchar('\n');
    terminal_writing_common_util(parsed_string1, strlen(parsed_string1), VGA_COLOR_RED, offset);
    offset += strlen(parsed_string1);
    free(parsed_string1);


    terminal_putchar('p');
    terminal_writing_common_util("p", 1u, VGA_COLOR_RED, offset);
    offset += 1u;
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
    size_t offset = 0u;

    terminal_writestring_util("Hello", VGA_COLOR_RED, offset);
    offset += strlen("Hello");

    terminal_writestring_util("Foo, Bar, and Baz had a party.", VGA_COLOR_RED, offset);
    offset += strlen("Foo, Bar, and Baz had a party.");

    //can't be const char* since free takes void*
    char *const parsed_string = parse_util("This\nis\ndifficult to read\n", offset%80u);
    terminal_writestring_util("This\nis\ndifficult to read\n", VGA_COLOR_RED, offset);
    offset += strlen(parsed_string);
    free(parsed_string);
}

void test_terminal_writestring_color(void) {
    size_t offset = 0u;

    terminal_writestring_color_util("Hello", VGA_COLOR_LIGHT_BLUE, offset);
    offset += strlen("Hello");

    terminal_writestring_color_util("Foo, Bar, and Baz had a party.", VGA_COLOR_CYAN, offset);
    offset += strlen("Foo, Bar, and Baz had a party.");

    //can't be const char* since free takes void*
    char *const parsed_string = parse_util("This\nis\ndifficult to read\n", offset%80u);
    terminal_writestring_color_util("This\nis\ndifficult to read\n", VGA_COLOR_MAGENTA, offset);
    offset += strlen(parsed_string);
    free(parsed_string);
}

void test_terminal_write_color(void) {
    size_t offset = 0u;

    terminal_write_color_util("Hello", VGA_COLOR_LIGHT_BLUE, offset);
    offset += strlen("Hello");

    terminal_write_color_util("Foo, Bar, and Baz had a party.", VGA_COLOR_CYAN, offset);
    offset += strlen("Foo, Bar, and Baz had a party.");

    //can't be const char* since free takes void*
    char *const parsed_string = parse_util("This\nis\ndifficult to read\n", offset%80u);
    terminal_write_color_util("This\nis\ndifficult to read\n", VGA_COLOR_MAGENTA, offset);
    offset += strlen(parsed_string);
    free(parsed_string);
}

void test_terminal_scrolling(void) {
    //should fill up buffer without scrolling it (25 80 character rows are available using vga buffer)
    terminal_writestring("Top line\n");
    for(int32_t i = 0; i < 23; ++i) {
        terminal_writestring("this is a test of scrolling\n");
    }
    terminal_writestring("Bottom line");

    size_t offset = 0u;
    char *const parsed_string_top = parse_util("Top line\n", 0u);
    char *const parsed_string_bottom = parse_util("Bottom line", 0u);
    TEST_ASSERT_EQUAL_INT(80u, strlen(parsed_string_top));

    // check bottom and top are correct
    terminal_writing_common_util(parsed_string_top, strlen(parsed_string_top), VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, strlen(parsed_string_bottom), VGA_COLOR_RED, offset);

    //shouldn't scroll up if no history
    terminal_scroll_up();
    offset = 0u;
    terminal_writing_common_util(parsed_string_top, strlen(parsed_string_top), VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, strlen(parsed_string_bottom), VGA_COLOR_RED, offset);

    //test scrolling down
    terminal_writestring("\nNew bottom");
    char *const parsed_string_second = parse_util("this is a test of scrolling\n", 0u);
    char *const parsed_string_new = parse_util("\nNew bottom", 0u);
    offset = 0u;
    terminal_writing_common_util(parsed_string_second, strlen(parsed_string_second), VGA_COLOR_RED, offset);
    offset = 80u * 23u + strlen(parsed_string_bottom); // start of last line
    terminal_writing_common_util(parsed_string_new, strlen(parsed_string_new), VGA_COLOR_RED, offset);

    //test scrolling up
    terminal_scroll_up();
    offset = 0u;
    terminal_writing_common_util(parsed_string_top, strlen(parsed_string_top), VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, strlen(parsed_string_bottom), VGA_COLOR_RED, offset);

    free(parsed_string_top);
    free(parsed_string_second);
    free(parsed_string_bottom);
    free(parsed_string_new);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_terminal_clear);
    RUN_TEST(test_terminal_setcolor);
    RUN_TEST(test_terminal_resetcolor);
    RUN_TEST(test_terminal_putentryat);
    RUN_TEST(test_terminal_putchar);
    RUN_TEST(test_terminal_write);
    RUN_TEST(test_terminal_writestring);
    RUN_TEST(test_terminal_write_color);
    RUN_TEST(test_terminal_writestring_color);
    RUN_TEST(test_terminal_scrolling);
    UNITY_END();
    return 0;
}
