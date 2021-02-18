cassert
=======

.. |br| raw:: html

  <br/>

This module has four macros and is header only.
The four macros are as follows:

cassert(x, return_code)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in non-void functions
for assertions that don't need custom error messages.

**Example Usage**::

    #include "cassert.h"

    //only available in hosted environment
    #include <printf>

    int func(void) {
        cassert(1 == 2, -1);
    }

    int main(void) {
        terminal_initialize();
        const int x = func();
        printf("%i\n", x);
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">Condition Failed: 1 == 2</font>

-1

|br|

cassert_void(x)
^^^^^^^^^^^^^^^^^^^

This macro should be used
in void functions
for assertions that don't need custom error messages.

**Example Usage**::

    #include "cassert.h"

    void func(void) {
        cassert_void(1 == 2);
    }

    int main(void) {
        terminal_initialize();
        func();
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">Condition Failed: 1 == 2</font>

|br|

cassert_message(x, message, return_code)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in non-void functions
for assertions that need custom error messages.

**Example Usage**::

    #include "cassert.h"

    //only available in hosted environment
    #include <printf>

    int func(void) {
        cassert_message(1 == 2, "arithmetic is broken", -1);
    }

    int main(void) {
        terminal_initialize();
        const int x = func();
        printf("%i\n", x);
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">arithmetic is broken</font>

-1

|br|

cassert_message_void(x, message)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in void functions
for assertions that need custom error messages.

**Example Usage**::

    #include "cassert.h"

    void func(void) {
        cassert_message_void(1 == 2, "arithmetic is broken");
    }

    int main(void) {
        terminal_initialize();
        func();
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">arithmetic is broken</font>

|br|

Reason for the Naming
^^^^^^^^^^^^^^^^^^^^^^

The ``c`` prefix to the file and the functions
is the naming convention
for this implementation/version of libc.

The reason why is that tests are run in userland and
linked against glibc. So, to be able to test our libc
on our host development machines, the prefix is needed.



Differences with Standard <assert.h> in libc
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These assertions do NOT cause terminate to be invoked.
It is often preferable to have assertions, but not terminate
in the kernel.

All of these assertions will log to standard output
(via the vga driver) the error message and then return their
error code that was passed in (or just ``return;``
in the case of the void versions).



Doc Bugs
^^^^^^^^^

The example usages mix freestanding and hosted IO.
This is impossible in actual code.

Fix: Replace with custom printf (cprintf) once
it is written.
