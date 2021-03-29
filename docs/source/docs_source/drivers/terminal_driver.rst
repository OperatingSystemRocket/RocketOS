terminal_driver
===============

.. |br| raw:: html

    <br/>

This module has two viariables and five functions.
The variables are:

``size_t start_of_command``
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Index in the vga_buffer to the start of the current terminal command.

|br|

``size_t end_of_command``
^^^^^^^^^^^^^^^^^^^^^^^^^

Index in the vga_buffer to the end of the current terminal command.

|br|

The functions are:

``void terminal_start(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function prints the terminal prompt, sets start_of_command
and end_of_command, turns on the terminal.

|br|

``void terminal_end(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function updates end_of_command to the
current position of the vga_driver.

|br|

``void terminal_process_command(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function turns off the terminal, and
checks what command was called and runs it,
before turning the terminal back on.

|br|

``void terminal_shift(void)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Utility function that shifts start_of_command
and end_of_command by 80 when the buffers
scrolls down.

|br|

``void get_command(char* final)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function loops over the vga_buffer
from start_of_command to end_of_command,
and creates the command string.
