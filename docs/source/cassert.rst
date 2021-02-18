cassert
=======

This module has four macros and is header only.
The four macros are as follows:

1. cassert(x, return_code)

This macro should be used
in non-void functions
for assertions that don't need custom error messages.


2. cassert_void(x)

This macro should be used
in void functions
for assertions that don't need custom error messages.


3. cassert_message(x, message, return_code)

This macro should be used
in non-void functions
for assertions that need custom error messages.


4. cassert_message_void(x, message)

This macro should be used
in void functions
for assertions that need custom error messages.



Reason for the Naming
^^^^^^^^^^^^^^^^^^^^^^

The 'c' prefix to the file and the functions
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
error code that was passed in (or just 'return;'
in the case of the void versions).
