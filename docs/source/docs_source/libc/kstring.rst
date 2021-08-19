kstring
=======

.. |br| raw:: html

    <br/>

This module has 17 functions that are defined as
follows:

``void* kmemcpy(void *restrict destination, const void *restrict source, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to copy memory of size num from
memory location source to the 
destination. Memory should not
overlap.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        const char src[13] = "Hello world!";
        char dest[13];
        kmemcpy(dest, src, kstrlen(src)+1);
        kprintf(dest);
        return 0;
    }

**Output**:

Hello world!

|br|

``void* kmemmove(void* destination, const void* source, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to copy memory of size num from
memory location source to the 
destination. Memory can
overlap.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        char src[] = "Hello world!";
        kmemmove(src+4, src+2, 4);
        kprintf(src);
        return 0;
    }

**Output**:

Hellllo rld!

|br|

``void* kmemchr(void* ptr, int32_t value, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to find the first occurrence of
value in memory of size num.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        unsigned char source_buffer[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x70, 0x09, 0x02, 0x03};
        const unsigned char \*const result = kmemchr((unsigned char\*) source_buffer, 0x09, sizeof(source_buffer));
        kprintf("%d", (result == &buffer[5]));
        return 0;
    }

**Output**:

1

|br|

``int32_t kmemcmp(const void* ptr1, const void* ptr2, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to compare memory of size num.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        const char \*const str1 = "abc";
        const char \*const str2 = "cde";
        const int32_t result = kmemcmp(str1, str2, kstrlen(str1));
        kprintf("%d", result);
        return 0;
    }

**Output**:

-1

|br|

``void* kmemset(void* ptr, int32_t value, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to set memory of size num to
value.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        uint32_t buffer[] = {0xDEAD, 0xBEEF, 0x10, 0x54, 0x97, 0x83, 0x12, 0x10};
        const uint32_t \*const result = kmemset(buffer, 0xFF, sizeof(buffer));
        for(int32_t i = 0; i < sizeof(buffer)/sizeof(uint32_t); ++i) {
            kprintf("%x\n", buffer[i]);
        }
        return 0;
    }

**Output**:

ffffffff
ffffffff
ffffffff
ffffffff
ffffffff
ffffffff
ffffffff
ffffffff

|br|

``char* kstrcat(char* destination, const char* source);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to concatenate two strings
(without bounds checking).

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        char buffer[32] = "abc";
        const char \*const result = kstrcat(buffer, "cdef");
        kprintf(result);
        return 0;
    }

**Output**:

abccdef

|br|

``char* kstrncat(char* destination, const char* source, size_t num);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to concatenate two strings
(with bounds checking).

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        char buffer[6] = "abc";
        const char \*const result = kstrncat(buffer, "cdef", sizeof(buffer)/sizeof(char));
        kprintf(result);
        return 0;
    }

**Output**:

abccd

|br|

``char* kstrchr(char* str, int32_t character);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to find the first occurrence of
character in string str
(searches from start of string).

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        char source[] = "aba";
        const char c = 'a';
        const char \*const expected = source;
        const char \*const result = kstrchr(source, c);
        kprintf("%d", (result == expected));
        return 0;
    }

**Output**:

1

|br|

``char* kstrrchr(char* str, int32_t character);``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to find the first occurrence of
character in string str
(searches from end of string).

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"
    #include "stdint.h"

    int main(void) {
        terminal_initialize();
        char source[] = "aba";
        const char c = 'a';
        const char \*const expected = source + 2;
        const char \*const result = kstrrchr(source, c);
        kprintf("%d", (result == expected));
        return 0;
    }

**Output**:

1

|br|


