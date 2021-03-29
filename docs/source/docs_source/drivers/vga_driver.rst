vga_driver
==========

.. |br| raw:: html

  <br/>

This module has 14 different functions and one enum.
The enum is defined as follows:

``vga_color``
^^^^^^^^^^^^^

This enum should be used
with terminal functions to
set the color of text.

|br|

The functions are defined as follows:

``void terminal_initialize(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be called at the start of 
the program, before any other terminal functions are called.
It is legal to call this function at anytime and as many times as you want.
It will reset the text color (and background color) as well as clear the screen.
See: terminal_resetcolor and terminal_clear

|br|

``void terminal_initialize_test(uint16_t* terminal_buffer_address)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is used to initialize the terminal
at a certain address in memory, used internally
by terminal_initialize. It should not be used
in source code and is purely for use in unit tests.

|br|

``void terminal_clear(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function clears everything in the buffer
replacing them all with spaces and setting
the cursor position to 0,0. Also resets scroll
history.

|br|

``void terminal_scroll_clear(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function clears everything in the buffer
replacing by scrolling the buffer and setting
the cursor position to 0,0. DOES NOT reset scroll
history.

|br|

``void terminal_setcolor(enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function sets the current terminal color.

|br|

``void terminal_resetcolor(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function resets the current terminal color
to the default color, light grey with black background.

|br|

``void terminal_putentryat(char c, enum vga_color color, size_t x, size_t y)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints character c in the listed color
at location (x, y) in the terminal buffer. Used internally
by other terminal functions, shouldn't be explicitly called
except for testing purposes.

|br|

``void terminal_putchar(char c)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints character c with the current terminal color
at the current position of the cursor. Also correctly formats newline
characters and scrolling to make space.

|br|

``void terminal_putchar_color(char c, enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints one character of the specified color at the
current position of the cursor, after the function, the color
is reset to the previous terminal color.

|br|

``void terminal_swapchar(char c)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function replaces the current character in the vga_buffer
with character c, used internally by the terminal.

|br|

``void terminal_swapchar_color(char c, enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function replaces the current character in the vga_buffer
with character c of color color, used internally by the terminal.

|br|

``void terminal_write(const char* text, size_t size)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints the string of text of the given size at the current
position of the cursor. Should not be called explicitly, called internally
by terminal_writestring.

|br|

``void terminal_writestring(const char* text)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints the given string at the current position of the cursor.

|br|

``void terminal_write_color(const char* text, size_t size, enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is identical to terminal_write, however it prints in the given color,
then resets to the previous color. Should not be called explicitly, used internally
by terminal_writestring_color.

|br|

``void terminal_writestring_color(const char* text, enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is identical to terminal_writestring, however it prints in the given
color, then resets the terminal to the previous color.

|br|

``void terminal_scroll_down(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function shifts all lines up one line in the buffer, the top line is saved to
a scroll history, if a scroll history exists, bottom line will be replaced with old
line, if it doesn't exist will be replaced by an empty line.

|br|

``void terminal_scroll_up(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function shifts all lines down one line in the buffer, the bottom line is saved
to a scroll history, if a scroll history exits, the top line will be replace by an old
line, if it doesn't exist, this function will do nothing.

|br|

``void terminal_cursor_up(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the cursor in the
vga_buffer up one row.

|br|

``void terminal_cursor_left(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the cursor in the
vga_buffer left one column.

|br|

``void terminal_cursor_right(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the cursor in the
vga_buffer right one column.

|br|

``void terminal_cursor_down(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the cursor in the
vga_buffer down one row.

|br|

``void terminal_backspace(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the cursor in the
vga_buffer left one column, and deletes
the character at that location.

|br|

``void terminal_updatecursor(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function moves the build in cursor
for qemu to the current position in the
vga_buffer.
