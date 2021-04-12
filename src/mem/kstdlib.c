#include "kstdlib.h"


#define BYTES_IN_WORD 4
#define MIN_BLOCK_SIZE 8 //4 words for bookkeeping and 4 word payload 
#define WORDS_IN_PAGE (PAGE_SIZE/BYTES_IN_WORD)


uint32_t* first_free_virtual_address; //points to start of first page
uint32_t* last_free_virtual_address; //points to start of last page
size_t number_of_pages_allocated; //TODO might remove `last_free_virtual_address`
uint32_t* head;


//TODO: do this in a way to where it can increase by more than a page at a time (implement brk and sbrk in `paging`)
//asks OS for an extra page
bool increase_memory_pool(void) {
    ++number_of_pages_allocated;
    last_free_virtual_address += PAGE_SIZE;
    return allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
}

uint32_t* allocate_block(const size_t size_to_allocate, uint32_t *const block) {
    kprintf("head: %x\n", (uint32_t)head);
    kprintf("size_to_allocate: %u\n", size_to_allocate);
    if(block[0] & 0x7fffffff <= 2*MIN_BLOCK_SIZE) {
        block[0] |= 0x80000000;
        block[(block[0] & 0x7fffffff)-1] |= 0x80000000;
        if(block[1] != NULL) {
            ((uint32_t*)block[1])[2] = block[2];
        } else {
            head = NULL;
        }
        if(block[2] != NULL) {
            ((uint32_t*)block[2])[1] = block[1];
        }
        block[1] = NULL;
        block[2] = NULL;

        return block+3;
    }

    uint32_t *const new_split_block = block+size_to_allocate; //point to start of new split free block

    new_split_block[0] = block[0]-size_to_allocate; //should maintain the topmost bit

    new_split_block[1] = block[1];
    new_split_block[2] = block[2];

    new_split_block[(block[0]-size_to_allocate)-1] = block[0]-size_to_allocate;

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

    block[size_to_allocate-1] = size_to_allocate;

    return block+3; //points to beginning of payload
}

void dynamic_memory_init(void) {
    number_of_pages_allocated = 1u;
    first_free_virtual_address = get_first_nonreserved_address();
    last_free_virtual_address = first_free_virtual_address;
    allocate_virtual_page(last_free_virtual_address, PT_PRESENT | PT_RW, PD_PRESENT | PD_RW);
    head = last_free_virtual_address;

    head[0] = WORDS_IN_PAGE; //first word of block is size of the block in words (including bookkeeping words like this size),
    //top bit is whether or not it is allocated

    //both of these pointers point to first byte of block
    head[1] = NULL; //prev pointer
    head[2] = NULL; //next pointer

    head[BYTES_IN_WORD-1] = WORDS_IN_PAGE; //duplicate of first word of block
}

//TODO: ask OS if no suitable memory found
void* malloc(const size_t size) {
    const uint32_t size_in_words = (size/BYTES_IN_WORD) + (size%BYTES_IN_WORD > 0) + 4; //the +4 is for the 4 bookkeeping words in a block

    uint32_t* current_block = head;
    while(current_block != NULL) {
        if((current_block[0] & 0x7fffffff) >= size_in_words) {
            //fist fit policy
            return allocate_block(size_in_words, current_block);
        }
    }

    return NULL;
}

void* zeroed_out_malloc(const size_t size) {
    const uint32_t payload_size_in_words = (size/BYTES_IN_WORD) + (size%BYTES_IN_WORD > 0); //is without the bookkeeping words

    kprintf("payload_size_in_words: %i\n", payload_size_in_words);

    uint32_t *const ret = malloc(size);
    if(ret != NULL) {
        kmemset(ret, 0, payload_size_in_words*BYTES_IN_WORD);
    }
    return ret;
}

void free(void *const ptr) {
    //TODO: implement
}
