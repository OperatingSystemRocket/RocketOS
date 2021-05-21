#include "kstdlib.h"


#define BYTES_IN_WORD 4
#define MIN_BLOCK_SIZE 8 //4 words for bookkeeping and 4 word payload 
#define WORDS_IN_PAGE (PAGE_SIZE/BYTES_IN_WORD)


uint32_t* first_free_virtual_address; //points to start of first page
uint32_t* last_free_virtual_address; //points to start of last page
size_t number_of_pages_allocated; //TODO might remove `last_free_virtual_address`
uint32_t* head;



int32_t bytes_to_words(uint32_t bytes);


uint32_t get_size(const uint32_t size_word) {
    return size_word & 0x7fffffff;
}

bool get_allocated_bit(const uint32_t size_word) {
    return (size_word & 0x80000000) > 0;
}

void heap_dump(const size_t number_of_pages) {
    kprintf("\n\n\n");
    kprintf("full heap dump\n");

    for(int32_t i = 0; i < 1024*number_of_pages; ++i) {
        const uint32_t *const start_of_heap = get_first_nonreserved_address();
        kprintf("size: %u, is_allocated: %u\n", get_size(start_of_heap[i]), get_allocated_bit(start_of_heap[i]));
    }

    kprintf("\n\n\n");
}

void freelist_dump(const bool increment) {
    static size_t number_of_freelist_dump = 0;

    kprintf("freelist\n");

    uint32_t* current_block = get_head();
    if(get_head() != NULL) {
        kprintf("head is not NULL\n");
    } else {
        kprintf("head is NULL\n");
    }
    while(current_block != NULL) {
        kprintf("number_of_freelist_dump: %u, address of entry: %X, first word size: %u, first word allocated bit: %u, second word: %X, third word: %X, last word size: %u, last word allocated bit: %u\n",
        number_of_freelist_dump, &current_block[0], get_size(current_block[0]), get_allocated_bit(current_block[0]), current_block[1], current_block[2], get_size(current_block[get_size(current_block[0])-1]), get_allocated_bit(current_block[get_size(current_block[0])-1]));
        current_block = current_block[2];
    }

    if(increment) {
        ++number_of_freelist_dump;

        kprintf("\n\n\n");
    }
}


//TODO: do this in a way to where it can increase by more than a page at a time (implement brk and sbrk in `paging`)
//asks OS for an extra page
static bool increase_memory_pool(void) {
    kprintf("increase_memory_pool\n");


    const uint32_t phys_addr = allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);


    if(phys_addr) {
        kprintf("page mapped successfully\n");
        kassert(last_free_virtual_address+WORDS_IN_PAGE > get_first_nonreserved_address(), false);
        if(!get_allocated_bit(last_free_virtual_address[-1])) {
            kprintf("start bigger block\n");

            uint32_t *const new_block_ptr = last_free_virtual_address-last_free_virtual_address[-1];

            new_block_ptr[0] += WORDS_IN_PAGE;
            new_block_ptr[new_block_ptr[0]-1] = new_block_ptr[0];

            kprintf("end bigger block\n");
        } else {
            kprintf("no coalescing\n");

            //front insertion policy
            //TODO: try using end insertion policy
            last_free_virtual_address[0] = 1024u;
            last_free_virtual_address[1] = NULL;
            last_free_virtual_address[2] = head;
            if(last_free_virtual_address[2] != NULL) {
                ((uint32_t*)last_free_virtual_address[2])[1] = &last_free_virtual_address[0];
            }
            last_free_virtual_address[1023] = last_free_virtual_address[0];
            head = &last_free_virtual_address[0];
        }

        ++number_of_pages_allocated;
        last_free_virtual_address += WORDS_IN_PAGE;
    }

    kprintf("last_free_virtual_address: %p\n", last_free_virtual_address);

    return phys_addr;
}

static uint32_t* allocate_block(const size_t size_to_allocate, uint32_t *const block) {
    if(get_size(block[0]) <= 2*MIN_BLOCK_SIZE) {
        block[0] |= 0x80000000;
        block[(get_size(block[0]))-1] |= 0x80000000;
        if(block[1] != NULL) {
            ((uint32_t*)block[1])[2] = block[2];
        } else {
            head = block[2];
        }
        if(block[2] != NULL) {
            ((uint32_t*)block[2])[1] = block[1];
        }
        block[1] = NULL;
        block[2] = NULL;

        return block+3;
    }

    uint32_t *const new_split_block = block+size_to_allocate; //point to start of new split free block

    new_split_block[0] = get_size(block[0]-size_to_allocate);

    new_split_block[1] = block[1];
    new_split_block[2] = block[2];

    new_split_block[get_size(new_split_block[0])-1] = get_size(new_split_block[0]);

    if(block[1] != NULL) {
        ((uint32_t*)block[1])[2] = &new_split_block[0];
    } else {
        head = &new_split_block[0];
    }
    if(block[2] != NULL) {
        ((uint32_t*)block[2])[1] = &new_split_block[0];
    }
    block[1] = NULL;
    block[2] = NULL;

    block[0] = size_to_allocate | 0x80000000;

    block[size_to_allocate-1] = size_to_allocate | 0x80000000;

    return block+3; //points to beginning of payload
}

void kdynamic_memory_init(void) {
    number_of_pages_allocated = 1u;
    first_free_virtual_address = get_first_nonreserved_address();
    last_free_virtual_address = first_free_virtual_address;
    allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    head = last_free_virtual_address;
    last_free_virtual_address += WORDS_IN_PAGE;

    head[0] = WORDS_IN_PAGE; //first word of block is size of the block in words (including bookkeeping words like this size),
    //top bit is whether or not it is allocated

    //both of these pointers point to first byte of block
    head[1] = NULL; //prev pointer
    head[2] = NULL; //next pointer

    head[WORDS_IN_PAGE-1] = WORDS_IN_PAGE; //duplicate of first word of block
}

//TODO: ask OS if no suitable memory found
void* kmalloc(const size_t size) {
    const uint32_t size_in_words = (size/BYTES_IN_WORD) + (size%BYTES_IN_WORD > 0) + 4; //the +4 is for the 4 bookkeeping words in a block


    do {
        uint32_t* current_block = head;
        while(current_block != NULL) {
            if((get_size(current_block[0])) >= size_in_words) {
                //first fit policy
                //TODO: try using best fit policy
                uint32_t *const ret = allocate_block(size_in_words, current_block)-3;
                kassert_void(ret[1] == NULL && ret[2] == NULL);
                return ret+3;
            }
            current_block = (uint32_t*)current_block[2];
        }
    } while(increase_memory_pool());

    return NULL;
}

void* zeroed_out_kmalloc(const size_t size) {
    const uint32_t payload_size_in_words = bytes_to_words(size); //is without the bookkeeping words

    uint32_t *const ret = kmalloc(size);
    if(ret != NULL) {
        kmemset(ret, 0, payload_size_in_words*BYTES_IN_WORD);
    }
    return ret;
}

void kfree(const void *const payload_ptr) {
    kprintf("kfree called\n");
    kprintf("head: %p\n", head);

    uint32_t* block_ptr = ((uint32_t*)payload_ptr)-3;

    kassert_void(block_ptr[1] == NULL && block_ptr[2] == NULL); //allocated blocks aren't part of the freelist

    block_ptr[0] &= 0x7fffffff; //set top bit (is_allocated) to 0, block_ptr[0] now just returns the size of the block
    block_ptr[block_ptr[0]-1] &= 0x7fffffff;

    bool in_freelist = false;

    if(!get_allocated_bit(block_ptr[block_ptr[0]])) { //points to the next block's first word
        uint32_t *const prev = (uint32_t*)block_ptr[block_ptr[0] + 1];
        uint32_t *const next = (uint32_t*)block_ptr[block_ptr[0] + 2];
        if(prev != NULL) {
            prev[2] = &block_ptr[0];
        } else {
            head = &block_ptr[0];
        }
        if(next != NULL) {
            next[1] = &block_ptr[0];
        }

        in_freelist = true;


        const uint32_t old_index = block_ptr[0];

        block_ptr[1] = prev;
        block_ptr[2] = next;


        block_ptr[0] += block_ptr[block_ptr[0]];


        block_ptr[old_index - 1] = 0;
        block_ptr[old_index] = 0;
        block_ptr[old_index + 1] = 0;
        block_ptr[old_index + 2] = 0;

        block_ptr[block_ptr[0]-1] = block_ptr[0]; //set last word of the new block to the new coalesced block size
        kassert_void(block_ptr[1] != &block_ptr[0] && block_ptr[2] != &block_ptr[0]);
    }
    if(block_ptr > get_first_nonreserved_address() && !get_allocated_bit(block_ptr[-1])) { //points to last word of the previous block
        const uint32_t old_size = block_ptr[0];
        uint32_t *const old_block_ptr = block_ptr;

        uint32_t *const prev = block_ptr[1];
        uint32_t *const next = block_ptr[2];

        if(prev != NULL) {
            prev[2] = next;
        } else if(next != NULL) {
            head = next;
        }
        if(next != NULL) {
            next[1] = prev;
        }


        block_ptr -= block_ptr[-1];
        in_freelist = true;

        old_block_ptr[-1] = 0;
        old_block_ptr[0] = 0;
        old_block_ptr[1] = 0;
        old_block_ptr[2] = 0;

        block_ptr[0] += old_size;
        block_ptr[block_ptr[0]-1] = block_ptr[0];


        kassert_void(block_ptr[1] != &block_ptr[0] && block_ptr[2] != &block_ptr[0]);
    }


    if(!in_freelist) {
        kassert_void(block_ptr[1] == NULL && block_ptr[2] == NULL);
        //add to front of freelist
        block_ptr[1] = NULL;
        block_ptr[2] = head;
        if(block_ptr[2] != NULL) {
            ((uint32_t*)block_ptr[2])[1] = &block_ptr[0];
        }
        head = &block_ptr[0];

        kassert_void(block_ptr[1] != &block_ptr[0] && block_ptr[1] != &block_ptr[0]);
    }
}

void* krealloc(void *const ptr, const size_t new_size) {
    const uint32_t new_size_in_words = bytes_to_words(new_size); //is without the bookkeeping words

    uint32_t *const block_head = ((uint32_t*)ptr)-3;

    if((bytes_to_words(new_size_in_words)+4) == get_size(block_head[0])) {
        return ptr; //do nothing if already the correct size
    }
    else if((bytes_to_words(new_size_in_words)+4) > get_size(block_head[0])) { //grow allocated block
        uint32_t *const next_block_head = block_head[get_size(block_head[0])];
        if(!get_allocated_bit(next_block_head[0]) && get_size(block_head[0]) + get_size(next_block_head[0]) >= new_size_in_words) {
            const size_t new_next_block_size = next_block_head[0] - (new_size_in_words - get_size(block_head[0]));
            const size_t memory_taken_from_next_block = next_block_head[0] - new_next_block_size;

            if(new_next_block_size <= 2*MIN_BLOCK_SIZE) {
                kprintf("if\n");
                uint32_t *const prev = next_block_head[1];
                uint32_t *const next = next_block_head[2];
                if(prev != NULL) {
                    prev[2] = next;
                    kprintf("prev: %u\n", (uint32_t)prev);
                } else {
                    kprintf("prev is NULL\n");
                }
                if(next != NULL) {
                    next[1] = prev;
                    kprintf("next: %u\n", (uint32_t)next);
                } else {
                    kprintf("next is NULL\n");
                }
                freelist_dump(false);


                block_head[0] += next_block_head[0];
                block_head[get_size(block_head[0]) - 1] = block_head[0];
            }
            else {
                kprintf("else\n");
                uint32_t *const start_of_new_next_block = new_next_block_size+new_next_block_size;
                kprintf("start_of_new_next_block: %p\n", start_of_new_next_block);

                uint32_t *const prev = next_block_head[1];
                start_of_new_next_block[1] = prev;
                if(prev != NULL) {
                    prev[2] = &start_of_new_next_block[0];
                    kprintf("prev: %u\n", (uint32_t)prev);
                } else {
                    kprintf("prev is NULL\n");
                }
                uint32_t *const next = next_block_head[2];
                start_of_new_next_block[2] = next;
                if(next != NULL) {
                    next[1] = &start_of_new_next_block[0];
                    kprintf("next: %u\n", (uint32_t)next);
                } else {
                    kprintf("next is NULL\n");
                }
                freelist_dump(false);


                start_of_new_next_block[0] = new_next_block_size;
                start_of_new_next_block[new_next_block_size - 1] = start_of_new_next_block[0];

                block_head[0] += memory_taken_from_next_block;
                block_head[get_size(block_head[0]) - 1] = block_head[0];           
            }
            return ptr;
        }
        else {
            kprintf("resize\n");
            uint32_t *const new_memory_block = zeroed_out_kmalloc(new_size);
            kmemcpy(new_memory_block, ptr, get_size(block_head[0])-4);
            kfree(ptr);
            return new_memory_block;
        }
    } else { //shrink allocated block
        uint32_t *const next_block_head = &block_head[get_size(block_head[0])];
        kprintf("next_block_head: %u\n", next_block_head[0]);
        const size_t difference_in_size = get_size(block_head[0])-new_size_in_words-4;
        kprintf("difference_in_size: %u\n", difference_in_size);

        kprintf("next_block_head: %X, last_free_virtual_address: %X\n", next_block_head, last_free_virtual_address);

        if(next_block_head < last_free_virtual_address-1 && !get_allocated_bit(next_block_head[0]) && difference_in_size > 2*MIN_BLOCK_SIZE) {
            kprintf("split\n");
            uint32_t *const new_split_block = next_block_head - difference_in_size;
            next_block_head[0] += difference_in_size;
            //block_head[next_block_head[0]-1] = next_block_head[0];
            new_split_block[0] = next_block_head[0];
            new_split_block[1] = next_block_head[1];
            new_split_block[2] = next_block_head[2];
            //block_head[new_split_block[0]-1] = new_split_block[0];

            kprintf("new_split_block[0]: %u\n", get_size(new_split_block[0]));

            uint32_t *const prev = (uint32_t*)block_head[1];
            uint32_t *const next = (uint32_t*)block_head[2];
            if(prev != NULL) {
                prev[2] = (uint32_t)&new_split_block[0];
            }
            else {
                head = &new_split_block[0];
            }

            if(next != NULL) {
                next[1] = (uint32_t)&new_split_block[0];
            }

            kprintf("realloc finished\n");
        }
        return ptr; //no point splitting because new block would be too small to be usable
    }
}

void* zeroed_out_krealloc(void *const ptr, const size_t new_size) {
    if((bytes_to_words(new_size)+4) == get_size(((uint32_t*)ptr)[-3])) {
        return ptr;
    }
    else if((bytes_to_words(new_size)+4) > get_size(((uint32_t*)ptr)[-3])) {
        const uint32_t old_size = get_size(((uint32_t*)ptr)[-3]) - 4;

        uint32_t *const ret = krealloc(ptr, new_size);
        if(ret != NULL) {
            kmemset(ret+old_size, 0, (bytes_to_words(new_size)-4 - old_size)*sizeof(uint32_t));
        }
        return ret;
    } else {
        return krealloc(ptr, new_size);
    }
}

uint32_t* get_head(void) {
    return head;
}
