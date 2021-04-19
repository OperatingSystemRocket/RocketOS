#include "kstdlib.h"


#define BYTES_IN_WORD 4
#define MIN_BLOCK_SIZE 8 //4 words for bookkeeping and 4 word payload 
#define WORDS_IN_PAGE (PAGE_SIZE/BYTES_IN_WORD)


uint32_t* first_free_virtual_address; //points to start of first page
uint32_t* last_free_virtual_address; //points to start of last page
size_t number_of_pages_allocated; //TODO might remove `last_free_virtual_address`
uint32_t* head;


static get_size(const uint32_t size_word) {
    return size_word & 0x7fffffff;
}

static get_allocated_bit(const uint32_t size_word) {
    return size_word & 0x80000000;
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

            new_block_ptr[0] += 1024u;
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

    head[BYTES_IN_WORD-1] = WORDS_IN_PAGE; //duplicate of first word of block
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
                return allocate_block(size_in_words, current_block);
            }
            kprintf("block size: %u\n", get_size(current_block[0]));
            kprintf("next block address: %p\n", current_block[2]);
            kprintf("current_block address: %p\n", &current_block[0]);
            current_block = (uint32_t*)current_block[2];
        }
        kprintf("allocation attempted\n");
    } while(increase_memory_pool());

    return NULL;
}

void* zeroed_out_kmalloc(const size_t size) {
    const uint32_t payload_size_in_words = (size/BYTES_IN_WORD) + (size%BYTES_IN_WORD > 0); //is without the bookkeeping words

    uint32_t *const ret = kmalloc(size);
    if(ret != NULL) {
        kmemset(ret, 0, payload_size_in_words*BYTES_IN_WORD);
    }
    return ret;
}

void kfree(const void *const payload_ptr) {
    uint32_t* block_ptr = ((uint32_t*)payload_ptr)-3;

    kassert_void(block_ptr[1] == NULL && block_ptr[2] == NULL); //allocated blocks aren't part of the freelist

    block_ptr[0] &= 0x7fffffff; //set top bit (is_allocated) to 0, block_ptr[0] now just returns the size of the block

    bool in_freelist = false;

    if(!get_allocated_bit(block_ptr[block_ptr[0]])) { //points to the next block's first word
        uint32_t *const prev = (uint32_t*)block_ptr[block_ptr[0] + 1];
        uint32_t *const next = (uint32_t*)block_ptr[block_ptr[0] + 2];
        prev[2] = (uint32_t)&block_ptr[0];
        next[1] = (uint32_t)&block_ptr[0];
        in_freelist = true;

        block_ptr[0] += block_ptr[block_ptr[0]];
        block_ptr[block_ptr[0]-1] = block_ptr[0]; //set last word of the new block to the new coalesced block size

        kassert_void(block_ptr[1] != &block_ptr[0] && block_ptr[2] != &block_ptr[0]);
    }
    if(!get_allocated_bit(block_ptr[-1])) { //points to last word of the previous block
        block_ptr -= block_ptr[-1];
        in_freelist = true;

        block_ptr[0] += block_ptr[block_ptr[0]];
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
