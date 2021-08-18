kstring
=======

.. |br| raw:: html

    <br/>

This module has 17 functions that are defined as
follows:

void* kmemcpy(void\* restrict destination, const void\* restrict source, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to copy memory of size num from
memory location source to the 
destination. Memory should not
overlap.

**Example Usage**::

    #include "kstring.h"
    #include "kstdio.h"

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

void* kmemmove(void* destination, const void* source, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This function should be used
to copy memory of size num from
memory location source to the 
destination. Memory can
overlap.

**Example Usage**::
    
    #include "kstring.h"
    #include "kstdio.h"

    int main(void) {
        terminal_initialize();
        char src[] = "Helle world!";
        kmemmove(src+4, src+7, 1);
        kprintf(src);
        return 0;
    }

**Output**:

Hello world!

|br|

void* kmemch(void* ptr, int value, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

int kmemcmp(const void* ptr1, const void* ptr2, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

void* kmemset(void \*const ptr, const int32_t value, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrcat(char* destination, const char* source);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrncat(char* const destination, const char* source, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrchr(char* str, int character);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrrchr(char* str, int character);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

int32_t kstrcmp(const char* lhs, const char* rhs);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

int32_t kstrncmp(const char* const lhs, const char* const rhs, const size_t sz);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

int kstrcoll(const char* str1, const char* str2);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrcpy(char* destination, const char* source);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kstrncpy(char* destination, const char* source, size_t num);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

size_t kstrspn(const char* str1, const char* str2);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

size_t kstrlen(const char* str);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO

|br|

char* kint_to_string(int64_t input, char* string_ret, size_t ret_size, uint32_t base, bool lowercase);
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

TODO
