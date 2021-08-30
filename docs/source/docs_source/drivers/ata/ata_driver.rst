ata_driver
===========

.. |br| raw:: html

    <br/>

This module has 7 functions that are defined as
follows:

``struct ata_driver_data* ata_driver_init(void);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function initializes the
internal data structures of
a ata driver object. It
returns a pointer to a heap
allocated ata object. The
ata data structure's type
is a predeclared private struct.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        //call any ata driver functions you want to using ``ata_data``

        ata_driver_destroy(ata_data);

        return 0;
    }

|br|

``void write_to_disk(struct ata_driver_data* ata_data, const char* datastream);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function appends a null
terminated string of the current data
of the hard drive (including the
null terminator). It requires a
pointer to a mutable
``struct ata_driver_data`` object
returned from ``ata_driver_init()``.
It also updates the location inside of the HDD
inside of ``ata_data``.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        write_to_disk(ata_data, "Hello World!");

        ata_driver_destroy(ata_data);

        return 0;
    }

|br|

``void write_to_disk_n(struct ata_driver_data* ata_data, const char* datastream, size_t n);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function appends ``n``
bytes of data to the current
data of the hard drive
(not including a null
terminator unless in
the datastream). It
requires a pointer to a mutable
``struct ata_driver_data`` object
returned from ``ata_driver_init()``.
It also updates teh location of the HDD
inside of ``ata_data``.

**Example Usage**::

    #include "ata_driver.h"
    #include "kstdio.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        write_to_disk_n(ata_data, "Hello World!", 6);
        const char \*const first_written_string = read_from_disk(ata_data, 5);

        ata_driver_destroy(ata_data);

        kprintf(first_written_string);

        kfree(first_written_string);

        return 0;
    }

**Output**:

Hello

|br|

``const char* read_from_disk(struct ata_driver_data* ata_data);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function returns a null terminated string
of what was written at the current location
in the HDD (stored in ``ata_data``) without
removing it from the HDD. This string is
heap allocated and can be freed
via normal ``kfree()``.

**Example Usage**::

    #include "ata_driver.h"
    #include "kstdio.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        write_to_disk(ata_driver, "Hello World!");
        const char \*const first_written_string = read_from_disk(ata_driver);

        ata_driver_destroy(ata_data);

        kprintf(first_written_string);

        kfree(first_written_string);

        return 0;
    }

**Output**:

Hello world!

|br|

``const char* read_from_disk_n(struct ata_driver_data* ata_data, size_t n);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function reads ``n``
bytes from the HDD and returns
a null terminated string of ``n``
bytes from the current location
in the HDD (stored in ``ata_data``)
without removing it from the HDD.
This string is heap allocated and can be freed
via normal ``kfree()``.

**Example Usage**::

    #include "ata_driver.h"
    #include "kstdio.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        write_to_disk(ata_driver, "Hello World!");
        const char \*const first_written_string = read_from_disk_n(ata_driver, 5);

        ata_driver_destroy(ata_data);

        kprintf(first_written_string);

        kfree(first_written_string);

        return 0;
    }

**Output**:

Hello

|br|

``void ata_driver_seek(struct ata_driver_data* ata_data, size_t byte_location_on_hdd);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function sets the ata_driver
object's hdd location to byte
``byte_location_on_hdd`` (zero indexed).
It requires a pointer to a mutable
``struct ata_driver_data`` object
returned from ``ata_driver_init()``.

**Example Usage**::

    #include "ata_driver.h"
    #include "kstdio.h"

    int main(void) {
        struct ata_driver \*const ata_data = ata_driver_init();

        write_to_disk(ata_driver, "Hello World!");
        ata_driver_seek(ata_driver, 1);
        const char \*const first_written_string = read_from_disk(ata_driver);

        ata_driver_destroy(ata_data);

        kprintf(first_written_string);

        kfree(first_written_string);

        return 0;

    }

**Output**:

ello World!

|br|

``void ata_driver_destroy(struct ata_driver_data* ata_data);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function frees the memory
allocated by ``ata_driver_init()``.
It requires a pointer to a mutable
``struct ata_driver_data`` object
returned from ``ata_driver_init()``.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        struct ata_driver_data \*const ata_data = ata_driver_init();

        ata_driver_destroy(ata_data);

        return 0;
    }

