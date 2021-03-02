kassert
=======

.. |br| raw:: html

  <br/>

This module has four macros and is header only.
The four macros are as follows:

``kassert(cond, return_code)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in non-void functions
for assertions that don't need custom error messages.

**Example Usage**::

    #include "kassert.h"
    #include "kstdio.h"

    int func(void) {
        kassert(1 == 2, -1);
    }

    int main(void) {
        terminal_initialize();
        const int x = func();
        kprintf("%i\n", x);
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">[file: foo/bar.c :: line: 5]: Condition Failed: 1==2</font>

-1

|br|

``kassert_void(cond)``
^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in void functions
for assertions that don't need custom error messages.

**Example Usage**::

    #include "kassert.h"

    void func(void) {
        kassert_void(1 == 2);
    }

    int main(void) {
        terminal_initialize();
        func();
        return 0;
    }

**Output**:

.. raw:: html

    <font color="red">[file: foo/bar.c :: line: 5]: Condition Failed: 1==2</font>

|br|

``kassert_message(cond, message, return_code)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in non-void functions
for assertions that need custom error messages.

**Example Usage**::

    #include "kassert.h"
    #include "kstdio.h"


    int func(void) {
        kassert_message(1 == 2, "arithmetic is broken", -1);
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

``kassert_message_void(cond, message)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This macro should be used
in void functions
for assertions that need custom error messages.

**Example Usage**::

    #include "kassert.h"

    void func(void) {
        kassert_message_void(1 == 2, "arithmetic is broken");
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

The ``k`` prefix to the file and the functions
is the naming convention
for this implementation/version of libc
including for non-standard functions and extensions.

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

Miscellaneous Notes
^^^^^^^^^^^^^^^^^^^^

All 4 macros will print a newline character
after the message even if the custom message
had no newline at the end.
