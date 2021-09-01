ata_driver
===========

.. |br| raw:: html

    <br/>

This module has 4 functions that are defined as
follows:

``void ata_driver_init(void);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function initializes the
internal data structures of
the ata driver and must be
called before any other ata
driver functions.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        ata_driver_init();

        //call any ata driver functions you want

        return 0;
    }

|br|

``void ata_driver_write_n(const char* data_stream, size_t byte_location, size_t data_stream_size);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function writes ``data_stream_size``
amount of bytes from ``data_stream``
to the HDD at location ``byte_location``.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        ata_driver_init();

        ata_driver_write_n("Hello World!", 0u, 5u); //only wrote "Hello" to the HDD
    }

``void ata_driver_write(const char* data_stream, size_t byte_location);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function is a utility function that is equivalent to calling
``ata_driver_write_n(data_stream, byte_location, strlen(data_stream));``.
``data_stream`` must be a null terminated string.

**Example Usage**::

    #include "ata_driver.h"

    int main(void) {
        ata_driver_init();

        ata_driver_write("Hello", 0u); //"Hello" is written to HDD
    }

|br|

``void ata_driver_read_n(char* bufffer, size_t byte_location, size_t byte_count);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function reads ``byte_count``
bytes from the HDD starting
at ``byte_location`` into the user
allocated buffer ``buffer``.

**Example Usage**::

    #include "ata_driver.h"
    #include "kstdio.h"

    int main(void) {
        ata_driver_init();

        void ata_driver_write("Hello", 0u);
        char buffer[10];
        ata_driver_read_n(buffer, 0u, 5u);
        buffer[5] = '\0'; //ata_driver_read doesn't null terminate the buffer

        kprintf("%s\n", buffer);

        return 0;
    }

**Output**:

Hello

|br|

