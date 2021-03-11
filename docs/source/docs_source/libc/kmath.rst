kmath
==========

.. |br| raw:: html

  <br/>

This header currently contains 6 functions, (to be expanded):

``int8_t ksignbit(int8_t val)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the signbit of the argument ``val`` which is 0 for values >= 0 and 1 for value < 0. Note that this behavior differs significantly from 
a mathematical sign function, which returns -1 for an argument > 0, 0 for an argument of 0, and 1 for an argument > 0 (see below).

|br|

``int8_t ksign(int8_t val)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the aforementioned mathematical sign of the argument ``val``.

|br|

``uint32_t kabs(int64_t val)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the absolute value of the argument ``val``.

|br|

``int64_t kmin(int64_t x, int64_t y)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns whichever argument has the lowest value.

|br|

``int64_t kmax(int64_t x, int64_t y)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns whichever argument has the highest value.


|br|

``int64_t kmod(int64_t x, int64_t y)``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Returns the remainder of ``x / y``
