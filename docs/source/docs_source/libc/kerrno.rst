kerrno
======

This is an implementation of <errno.h> from libc.

It is just a few macro define constants and the
``errno`` variable.
Please refer to the cppreference docs for errno
for examples of use.


..
  errno is so simple and widely known and
  our implementation is so boring,
  that it isn't necessary to write examples
  or write thorough docs on what errno is.


Differences from the C standard
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The C11 standard defines that ``errno`` must be a macro.
In our implementation,
we just make ``errno`` an ``extern int``.
