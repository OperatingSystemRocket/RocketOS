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

``terminal_initialize(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


This function should be called at the start of 
the program, before any other terminal functions are called.

|br|

``terminal_initialize_test(uint16_t *const terminal_buffer_address)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is used to initialize the terminal
at a certain address in memory, used internally
by terminal_initialize.

|br|

``terminal_clear(void)``
^^^^^^^^^^^^^^^^^^^^^^^^

This function clears everything in the buffer
replacing them all with spaces and setting
the cursor position to 0,0. Also resets scroll
history.

|br|

``terminal_scroll_clear(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function clears everything in the buffer
replacing by scrolling the buffer and setting
the cursor position to 0,0. DOES NOT reset scroll
history.

|br|

``terminal_setcolor(enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function sets the current terminal color.

|br|

``terminal_resetcolor(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function resets the current terminal color
to the default color, light grey with black background.

|br|

``terminal_putentryat(char c, enum vga_color color, size_t x, size_t y)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints character c in the listed color
at location (x, y) in the terminal buffer. Used internally
by other terminal functions, shouldn't be explicitly called
except for testing purposes.

|br|

``terminal_putchar(char c)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints character c with the current terminal color
at the current position of the cursor. Also correctly formats newline
characters and scrolling to make space.

|br|

``terminal_putchar_color(const char c, const enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints one character of the specified color at the
current position of the cursor, after the function, the color
is reset to the previous terminal color.

|br|

``terminal_write(const char* text, size_t size)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints the string of text of the given size at the current
position of the cursor. Should not be called explicitly, called internally
by terminal_writestring.

|br|

``terminal_writestring(const char *const text)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints the given string at the current position of the cursor.

|br|

``terminal_write_color(const char *const text, const size_t size, const enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is identical to terminal_write, however it prints in the given color,
then resets to the previous color. Should not be called explicitly, used internally
by terminal_writestring_color.

|br|

``terminal_writestring_color(const char *const text, const enum vga_color color)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is identical to terminal_writestring, however it prints in the given
color, then resets the terminal to the previous color.

|br|

``terminal_scroll_down(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function shifts all lines up one line in the buffer, the top line is saved to
a scroll history, if a scroll history exists, bottom line will be replaced with old
line, if it doesn't exist will be replaced by an empty line.

|br|

``terminal_scroll_up(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function shifts all lines down one line in the buffer, the bottom line is saved
to a scroll history, if a scroll history exits, the top line will be replace by an old
line, if it doesn't exist, this function will do nothing.
