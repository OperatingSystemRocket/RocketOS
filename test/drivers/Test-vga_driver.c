#include <multiboot.h>


#include "unity.h"
#include "vga_driver.h"
#include "kassert.h"
#include "kstring.h"


#define VGA_BUFFER_SIZE 80*25

uint16_t buffer[VGA_BUFFER_SIZE];

void setUp(void) {
    terminal_initialize_test(buffer); //dummy vga driver address so that tests can be written
    terminal_setcolor(VGA_COLOR_RED); //arbitrary color to check against that doesn't deal with background color
}
void tearDown(void) {}


//utility functions:
static void terminal_writing_common_util(const char *const text, const size_t size, const enum vga_color color, const uint16_t offset) {
    TEST_ASSERT_LESS_THAN_UINT(2048u, size);
    uint16_t expected_text[2048u];
    for(uint16_t i = 0u; i < size; ++i) {
        if(text[i] == '\n') {
            const uint8_t default_color = VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4u;
            expected_text[i] = (uint16_t) ' ' | (uint16_t) default_color << 8u;
        } else {
            expected_text[i] = (uint16_t) text[i] | (uint16_t) color << 8u;
        }
    }
    TEST_ASSERT_EQUAL_MEMORY(expected_text, &buffer[offset], size);
}

size_t parse_size_util(const char *const text, const uint8_t column_offset) {
    size_t rows = 0u;
    size_t columns = column_offset;

    for(uint16_t i = 0u; i < kstrlen(text); ++i) {
        if(text[i] != '\n') {
            ++columns;
        } else {
            ++rows;
            columns = 0u;
        }
    }

    return (rows*80u + columns) - column_offset;
}

static void parse_util(const char *const text, char *const ret, const uint8_t column_offset) {
    size_t rows = 0u;
    size_t columns = column_offset;

    for(uint16_t i = 0u; i < kstrlen(text); ++i) {
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
}

void terminal_resetcolor_util(const enum vga_color color) {
    terminal_setcolor(color);

    terminal_resetcolor();

    const char *const text = "This is some \n example\n text\n";
    terminal_writestring(text); //can be any function that does a text write without changing color
    const size_t buffer_size = parse_size_util(text, 0u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, 0u);
    terminal_writing_common_util(parsed_string, buffer_size, VGA_COLOR_LIGHT_GREY | VGA_COLOR_BLACK << 4u, 0u);

    terminal_initialize_test(&buffer[0]);
}

void terminal_setcolor_util(const enum vga_color color) {
    terminal_setcolor(color);

    const char *const text = "This is some \n example\n text\n";
    terminal_writestring(text); //can be any function that does a text write without changing color
    const size_t buffer_size = parse_size_util(text, 0u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, 0u);
    terminal_writing_common_util(parsed_string, buffer_size, color, 0u);

    terminal_initialize_test(&buffer[0]);
}

static void terminal_write_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    const size_t size = kstrlen(text);
    terminal_write(text, size);

    const size_t buffer_size = parse_size_util(text, offset%80u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, offset%80u);
    terminal_writing_common_util(parsed_string, buffer_size, color, offset);
}

static void terminal_writestring_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    terminal_writestring(text);

    const size_t buffer_size = parse_size_util(text, offset%80u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, offset%80u);
    terminal_writing_common_util(parsed_string, buffer_size, color, offset);
}

static void terminal_writestring_color_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    terminal_writestring_color(text, color);

    const size_t buffer_size = parse_size_util(text, offset%80u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, offset%80u);
    terminal_writing_common_util(parsed_string, buffer_size, color, offset);
}

static void terminal_write_color_util(const char *const text, const enum vga_color color, const uint16_t offset) {
    const size_t size = kstrlen(text);
    terminal_write_color(text, size, color);

    const size_t buffer_size = parse_size_util(text, offset%80u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util(text, parsed_string, offset%80u);
    terminal_writing_common_util(parsed_string, buffer_size, color, offset);
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

    const size_t buffer_size = parse_size_util("\n", offset%80u);
    kassert_void(buffer_size < 2048u);
    char parsed_string[2048u];
    parse_util("\n", parsed_string, offset%80u);
    terminal_putchar('\n');
    terminal_writing_common_util(parsed_string, buffer_size, VGA_COLOR_RED, offset);
    offset += buffer_size;

    terminal_putchar('e');
    terminal_writing_common_util("e", 1u, VGA_COLOR_RED, offset);
    offset += 1u;

    terminal_putchar('g');
    terminal_writing_common_util("g", 1u, VGA_COLOR_RED, offset);
    offset += 1u;

    const size_t buffer_size1 = parse_size_util("\n", offset%80u);
    kassert_void(buffer_size1 < 2048u);
    char parsed_string1[2048u];
    parse_util("\n", parsed_string1, offset%80u);
    terminal_putchar('\n');
    terminal_writing_common_util(parsed_string1, buffer_size1, VGA_COLOR_RED, offset);
    offset += buffer_size1;

    terminal_putchar('p');
    terminal_writing_common_util("p", 1u, VGA_COLOR_RED, offset);
    offset += 1u;
}

void test_terminal_write(void) {
    size_t offset = 0u;

    terminal_write_util("Hello", VGA_COLOR_RED, offset);
    offset += kstrlen("Hello");

    terminal_write_util("Foo, Bar, and Baz had a party.", VGA_COLOR_RED, offset);
    offset += kstrlen("Foo, Bar, and Baz had a party.");

    const char *const text = "This\nis\ndifficult to read\n";
    const size_t buffer_size = parse_size_util(text, offset%80u);
    terminal_write_util(text, VGA_COLOR_RED, offset);
    offset += buffer_size;
}

void test_terminal_writestring(void) {
    size_t offset = 0u;

    terminal_writestring_util("Hello", VGA_COLOR_RED, offset);
    offset += kstrlen("Hello");

    terminal_writestring_util("Foo, Bar, and Baz had a party.", VGA_COLOR_RED, offset);
    offset += kstrlen("Foo, Bar, and Baz had a party.");

    const char *const text = "This\nis\ndifficult to read\n";
    const size_t buffer_size = parse_size_util(text, offset%80u);
    terminal_writestring_util(text, VGA_COLOR_RED, offset);
    offset += buffer_size;
}

void test_terminal_writestring_color(void) {
    size_t offset = 0u;

    terminal_writestring_color_util("Hello", VGA_COLOR_LIGHT_BLUE, offset);
    offset += kstrlen("Hello");

    terminal_writestring_color_util("Foo, Bar, and Baz had a party.", VGA_COLOR_CYAN, offset);
    offset += kstrlen("Foo, Bar, and Baz had a party.");

    const char *const text = "This\nis\ndifficult to read\n";
    const size_t buffer_size = parse_size_util(text, offset%80u);
    terminal_writestring_color_util(text, VGA_COLOR_MAGENTA, offset);
    offset += buffer_size;
}

void test_terminal_write_color(void) {
    size_t offset = 0u;

    terminal_write_color_util("Hello", VGA_COLOR_LIGHT_BLUE, offset);
    offset += kstrlen("Hello");

    terminal_write_color_util("Foo, Bar, and Baz had a party.", VGA_COLOR_CYAN, offset);
    offset += kstrlen("Foo, Bar, and Baz had a party.");

    const char *const text = "This\nis\ndifficult to read\n";
    const size_t buffer_size = parse_size_util(text, offset%80u);
    terminal_write_color_util(text, VGA_COLOR_MAGENTA, offset);
    offset += buffer_size;
}

void test_terminal_scrolling(void) {
    //should fill up buffer without scrolling it (25 80 character rows are available using vga buffer)
    terminal_writestring("Top line\n");
    for(int32_t i = 0; i < 23; ++i) {
        terminal_writestring("this is a test of scrolling\n");
    }
    terminal_writestring("Bottom line");

    size_t offset = 0u;

    const size_t parsed_string_top_size = parse_size_util("Top line\n", 0u);
    kassert_void(parsed_string_top_size < 2048u);
    char parsed_string_top[2048u];
    parse_util("Top line\n", parsed_string_top, 0u);

    const size_t parsed_string_bottom_size = parse_size_util("Bottom line", 0u);
    kassert_void(parsed_string_bottom_size < 2048u);
    char parsed_string_bottom[2048u];
    parse_util("Bottom line", parsed_string_bottom, 0u);

    TEST_ASSERT_EQUAL_INT(80u, parsed_string_top_size);

    // check bottom and top are correct
    terminal_writing_common_util(parsed_string_top, parsed_string_top_size, VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, parsed_string_bottom_size, VGA_COLOR_RED, offset);

    //shouldn't scroll up if no history
    terminal_scroll_up();
    offset = 0u;
    terminal_writing_common_util(parsed_string_top, parsed_string_top_size, VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, parsed_string_bottom_size, VGA_COLOR_RED, offset);

    //test scrolling down
    terminal_writestring("\nNew bottom");
    const size_t parsed_string_second_size = parse_size_util("this is a test of scrolling\n", 0u);
    kassert_void(parsed_string_second_size < 2048u);
    char parsed_string_second[2048u];
    parse_util("this is a test of scrolling\n", parsed_string_second, 0u);

    const size_t parsed_string_new_size = parse_size_util("\nNew bottom", 0u);
    kassert_void(parsed_string_new_size < 2048u);
    char parsed_string_new[2048u];
    parse_util("\nNew bottom", parsed_string_new, 0u);

    offset = 0u;
    terminal_writing_common_util(parsed_string_second, parsed_string_second_size, VGA_COLOR_RED, offset);
    offset = 80u * 23u + parsed_string_bottom_size; // start of last line
    terminal_writing_common_util(parsed_string_new, parsed_string_new_size, VGA_COLOR_RED, offset);

    //test scrolling up
    terminal_scroll_up();
    offset = 0u;
    terminal_writing_common_util(parsed_string_top, parsed_string_top_size, VGA_COLOR_RED, offset);
    offset = 80u * 24u; // start of last line
    terminal_writing_common_util(parsed_string_bottom, parsed_string_bottom_size, VGA_COLOR_RED, offset);
}


void kernel_early(const uint32_t mboot_magic, const multiboot_info_t *const mboot_header) {
    terminal_initialize();
    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        terminal_writestring_color("Invalid Multiboot Magic!\n", VGA_COLOR_RED);
    } else {
        terminal_writestring("The multiboot structure was loaded properly\n");
    }
}

void kernel_main(void) {
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
    outb(0xf4, 0x10);
}
