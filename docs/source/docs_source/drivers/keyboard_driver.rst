keyboard_driver
===============

.. |br| raw:: html

    <br/>

This module has one function,
defined as follows:

``void process_keystroke(const char keycode)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is called internally by the
keyboard interrupt, it decodes the keycode
into a printable character.
