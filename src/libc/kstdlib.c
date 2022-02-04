#include "kstdlib.h"

#include "kstdlib_constants.h"


static uint32_t* first_free_virtual_address; //points to start of first page
static uint32_t* last_free_virtual_address; //points to start of last page
static size_t number_of_pages_allocated; //TODO might remove `last_free_virtual_address`
static uint32_t* head;


uint32_t bytes_to_words(uint32_t bytes);
uint32_t round_up_to_nearest_n_power(uint32_t num_to_round, uint32_t multiple);

uint32_t get_size(uint32_t size_word);
bool get_allocated_bit(uint32_t size_word);


//TODO: do this in a way to where it can increase by more than a page at a time (implement brk and sbrk in `paging`)
//asks OS for a physical page
static bool increase_memory_pool(void) {
    if(number_of_pages_allocated < get_max_heap_size()) {
        const uint32_t phys_addr = allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);

        if(phys_addr) {
            kassert(last_free_virtual_address+WORDS_IN_PAGE > (uint32_t*) get_heap_range_start(), false);
            if(!get_allocated_bit(last_free_virtual_address[-1])) {
                uint32_t *const new_block_ptr = last_free_virtual_address-last_free_virtual_address[-1];

                new_block_ptr[0] += WORDS_IN_PAGE;
                new_block_ptr[new_block_ptr[0]-1] = new_block_ptr[0];
            } else {
                //front insertion policy
                //TODO: try using end insertion policy
                last_free_virtual_address[0] = 1024u;
                last_free_virtual_address[1] = (uint32_t) NULL;
                last_free_virtual_address[2] = (uint32_t) head;
                if((uint32_t*) last_free_virtual_address[2] != NULL) {
                    ((uint32_t*)last_free_virtual_address[2])[1] = (uint32_t) &last_free_virtual_address[0];
                }
                last_free_virtual_address[1023] = last_free_virtual_address[0];
                head = &last_free_virtual_address[0];
            }

            ++number_of_pages_allocated;
            last_free_virtual_address += WORDS_IN_PAGE;
        }

        return phys_addr;
    }

    return false; //allocation failed as it has reached the maximum size of the heap
}

static uint32_t* allocate_block(const size_t size_to_allocate, uint32_t *const block) {
    kassert(get_allocated_bit(block[0]) == false, NULL);

    if(get_size(block[0]) <= (size_to_allocate+(2*MIN_BLOCK_SIZE))) {
        block[0] |= 0x80000000;
        block[(get_size(block[0]))-1] |= 0x80000000;
        if((uint32_t*) block[1] != NULL) {
            ((uint32_t*)block[1])[2] = block[2];
        } else {
            head = (uint32_t*) block[2];
        }
        if((uint32_t*) block[2] != NULL) {
            ((uint32_t*)block[2])[1] = block[1];
        }
        block[1] = (uint32_t) NULL;
        block[2] = (uint32_t) NULL;

        return block;
    }

    uint32_t *const new_split_block = block+size_to_allocate; //point to start of new split free block

    new_split_block[0] = get_size(block[0]-size_to_allocate);

    new_split_block[1] = block[1];
    new_split_block[2] = block[2];

    new_split_block[get_size(new_split_block[0])-1] = get_size(new_split_block[0]);

    if((uint32_t*) block[1] != NULL) {
        ((uint32_t*)block[1])[2] = (uint32_t) &new_split_block[0];
    } else {
        head = &new_split_block[0];
    }
    if((uint32_t*) block[2] != NULL) {
        ((uint32_t*)block[2])[1] = (uint32_t) &new_split_block[0];
    }
    block[1] = (uint32_t) NULL;
    block[2] = (uint32_t) NULL;

    block[0] = size_to_allocate | 0x80000000;

    block[size_to_allocate-1] = size_to_allocate | 0x80000000;

    return block;
}

void kdynamic_memory_init(void) {
    number_of_pages_allocated = 1u;
    first_free_virtual_address = get_heap_range_start();
    last_free_virtual_address = first_free_virtual_address;
    allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    head = last_free_virtual_address;
    last_free_virtual_address += WORDS_IN_PAGE;

    head[0] = WORDS_IN_PAGE; //first word of block is size of the block in words (including bookkeeping words like this size),
    //top bit is whether or not it is allocated

    //both of these pointers point to first byte of block
    head[1] = (uint32_t) NULL; //prev pointer
    head[2] = (uint32_t) NULL; //next pointer

    head[WORDS_IN_PAGE-1] = WORDS_IN_PAGE; //duplicate of first word of block
}

void* kmalloc(const size_t size) {
    if(size == 0u) return NULL;

    const uint32_t size_in_words = (size/BYTES_IN_WORD) + (size%BYTES_IN_WORD > 0) + 4; //the +4 is for the 4 bookkeeping words in a block

    do {
        uint32_t* current_block = head;
        while(current_block != NULL) {
            if((get_size(current_block[0])) >= size_in_words) {
                //first fit policy
                //TODO: try using best fit policy
                uint32_t *const ret = allocate_block(size_in_words, current_block);
                kassert((uint32_t*) ret[1] == NULL && (uint32_t*) ret[2] == NULL, NULL);

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
    if(payload_ptr == NULL) return;


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

    //this const_cast is intentional and legal as all malloc'ed pointers point to mutable memory
    uint32_t* block_ptr = ((uint32_t*)payload_ptr)-3;

#pragma GCC diagnostic pop
#pragma GCC diagnostic pop


    kassert_void((uint32_t*) block_ptr[1] == NULL && (uint32_t*) block_ptr[2] == NULL); //allocated blocks aren't part of the freelist

    block_ptr[0] &= 0x7fffffff; //set top bit (is_allocated) to 0, block_ptr[0] now just returns the size of the block
    block_ptr[block_ptr[0]-1] &= 0x7fffffff;

    bool in_freelist = false;

    if(!get_allocated_bit(block_ptr[block_ptr[0]])) { //points to the next block's first word
        uint32_t *const prev = (uint32_t*)block_ptr[block_ptr[0] + 1];
        uint32_t *const next = (uint32_t*)block_ptr[block_ptr[0] + 2];
        if(prev != NULL) {
            prev[2] = (uint32_t) &block_ptr[0];
        } else {
            head = &block_ptr[0];
        }
        if(next != NULL) {
            next[1] = (uint32_t) &block_ptr[0];
        }

        in_freelist = true;


        const uint32_t old_index = block_ptr[0];

        block_ptr[1] = (uint32_t) prev;
        block_ptr[2] = (uint32_t) next;


        block_ptr[0] += block_ptr[block_ptr[0]];


        block_ptr[old_index - 1] = 0;
        block_ptr[old_index] = 0;
        block_ptr[old_index + 1] = 0;
        block_ptr[old_index + 2] = 0;

        block_ptr[block_ptr[0]-1] = block_ptr[0]; //set last word of the new block to the new coalesced block size
        kassert_void((uint32_t*) block_ptr[1] != &block_ptr[0] && (uint32_t*) block_ptr[2] != &block_ptr[0]);
    }
    if(block_ptr > (uint32_t*) get_heap_range_start() && !get_allocated_bit(block_ptr[-1])) { //points to last word of the previous block
        const uint32_t old_size = block_ptr[0];
        uint32_t *const old_block_ptr = block_ptr;

        uint32_t *const prev = (uint32_t*) block_ptr[1];
        uint32_t *const next = (uint32_t*) block_ptr[2];

        if(prev != NULL) {
            prev[2] = (uint32_t) next;
        } else if(next != NULL) {
            head = next;
        }
        if(next != NULL) {
            next[1] = (uint32_t) prev;
        }


        block_ptr -= block_ptr[-1];
        in_freelist = true;

        old_block_ptr[-1] = 0;
        old_block_ptr[0] = 0;
        old_block_ptr[1] = 0;
        old_block_ptr[2] = 0;

        block_ptr[0] += old_size;
        block_ptr[block_ptr[0]-1] = block_ptr[0];


        kassert_void((uint32_t*) block_ptr[1] != &block_ptr[0] && (uint32_t*) block_ptr[2] != &block_ptr[0]);
    }


    if(!in_freelist) {
        kassert_void((uint32_t*) block_ptr[1] == NULL && (uint32_t*) block_ptr[2] == NULL);
        //add to front of freelist
        block_ptr[1] = (uint32_t) NULL;
        block_ptr[2] = (uint32_t) head;
        if((uint32_t*) block_ptr[2] != NULL) {
            ((uint32_t*)block_ptr[2])[1] = (uint32_t) &block_ptr[0];
        }
        head = &block_ptr[0];

        kassert_void((uint32_t*) block_ptr[1] != &block_ptr[0] && (uint32_t*) block_ptr[1] != &block_ptr[0]);
    }
}

void* krealloc(void *const ptr, const size_t new_size) {
    const uint32_t new_size_in_words = bytes_to_words(new_size); //is without the bookkeeping words

    uint32_t *const block_head = ((uint32_t*)ptr)-3;

    if((new_size_in_words+4u) == get_size(block_head[0])) {
        return ptr; //do nothing if already the correct size
    }
    else if((new_size_in_words+4u) > get_size(block_head[0])) { //grow allocated block
        uint32_t *const next_block_head = (uint32_t*) block_head[get_size(block_head[0])];
        if((next_block_head < last_free_virtual_address) && !get_allocated_bit(next_block_head[0]) && ((get_size(block_head[0]) + get_size(next_block_head[0])) >= new_size_in_words)) {
            const size_t new_next_block_size = next_block_head[0] - (new_size_in_words - get_size(block_head[0]));
            const size_t memory_taken_from_next_block = next_block_head[0] - new_next_block_size;

            if(new_next_block_size <= 2*MIN_BLOCK_SIZE) {
                uint32_t *const prev = (uint32_t*) next_block_head[1];
                uint32_t *const next = (uint32_t*) next_block_head[2];
                if(prev != NULL) {
                    prev[2] = (uint32_t) next;
                }
                if(next != NULL) {
                    next[1] = (uint32_t) prev;
                }


                block_head[0] += next_block_head[0];
                block_head[get_size(block_head[0]) - 1] = block_head[0];
            }
            else {
                //TODO: investigate if this is correct
                uint32_t *const start_of_new_next_block = (uint32_t*) (next_block_head+new_next_block_size);

                uint32_t *const prev = (uint32_t*) next_block_head[1];
                start_of_new_next_block[1] = (uint32_t) prev;
                if(prev != NULL) {
                    prev[2] = (uint32_t) &start_of_new_next_block[0];
                }
                uint32_t *const next = (uint32_t*) next_block_head[2];
                start_of_new_next_block[2] = (uint32_t) next;
                if(next != NULL) {
                    next[1] = (uint32_t) &start_of_new_next_block[0];
                }


                start_of_new_next_block[0] = new_next_block_size;
                start_of_new_next_block[new_next_block_size - 1] = start_of_new_next_block[0];

                block_head[0] += memory_taken_from_next_block;
                block_head[get_size(block_head[0]) - 1] = block_head[0];           
            }
            return ptr;
        }
        else {
            uint32_t *const new_memory_block = zeroed_out_kmalloc(new_size);
            kmemcpy(new_memory_block, ptr, BYTES_IN_WORD*(get_size(block_head[0])-4));
            kfree(ptr);
            return new_memory_block;
        }
    } else { //shrink allocated block
        uint32_t *const next_block_head = &block_head[get_size(block_head[0])];
        const size_t difference_in_size = get_size(block_head[0])-new_size_in_words-4;


        if(difference_in_size > 2*MIN_BLOCK_SIZE) {
            if(next_block_head < last_free_virtual_address && !get_allocated_bit(next_block_head[0])) {
                uint32_t *const new_split_block = next_block_head - difference_in_size;
                next_block_head[0] += difference_in_size;
                block_head[get_size(block_head[0]) - 1] = 0;
                block_head[0] -= difference_in_size;
                block_head[block_head[0]-1] = block_head[0];
                new_split_block[0] = next_block_head[0];
                new_split_block[1] = next_block_head[1];
                new_split_block[2] = next_block_head[2];
                new_split_block[new_split_block[0]-1] = new_split_block[0];
                next_block_head[0] = 0;


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
            }
            else {
                block_head[0] -= difference_in_size;
                block_head[get_size(block_head[0]) - 1] = block_head[0];

                uint32_t *const new_block_head = block_head + get_size(block_head[0]);
                new_block_head[0] = difference_in_size;
                new_block_head[1] = (uint32_t) NULL;
                new_block_head[2] = (uint32_t) head;
                head[1] = (uint32_t) &new_block_head[0];
                new_block_head[difference_in_size - 1] = difference_in_size;
                head = &new_block_head[0];
            }
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

void* kcalloc(const size_t num, const size_t size) {
    return zeroed_out_kmalloc(num*size);
}

void* uninitialized_kcalloc(const size_t num, const size_t size) {
    return kmalloc(num*size);
}

uint32_t* get_head(void) {
    return head;
}
