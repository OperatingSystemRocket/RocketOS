#include "osi_memory_allocator.h"


void osi_memory_allocator_init(struct osi_memory_allocator *const allocator, uint32_t *const start_of_virt_mem, const uint32_t number_of_pages) {
    allocator->start_of_virt_mem = start_of_virt_mem;
    allocator->number_of_pages = number_of_pages;

    allocator->allocated_list_head = NULL;
    allocator->allocated_list_tail = NULL;

    struct memory_bookkeeping_node *const memory_node = kmalloc(sizeof(struct memory_bookkeeping_node));
    memory_node->prev = NULL;
    memory_node->next = NULL;
    memory_node->page_index = 0u;
    memory_node->num_of_pages = number_of_pages;
    allocator->free_list_head = memory_node;
    allocator->free_list_tail = memory_node;
}
void* osi_memory_allocator_allocate(struct osi_memory_allocator *const allocator, const uint32_t num_of_pages) {
    if(allocator == NULL || num_of_pages == 0u) {
        return NULL;
    }

    struct memory_bookkeeping_node* current_node = allocator->free_list_head;
    if(current_node == NULL) {
        return NULL;
    }
    while(current_node != NULL) {
        if(current_node->num_of_pages >= num_of_pages) {
            struct memory_bookkeeping_node *const new_allocated_node = kmalloc(sizeof(struct memory_bookkeeping_node));
            new_allocated_node->page_index = current_node->page_index;
            new_allocated_node->num_of_pages = num_of_pages;
            current_node->num_of_pages -= num_of_pages;
            current_node->page_index += num_of_pages;
            if(current_node->num_of_pages == 0u) {
                if(current_node->prev != NULL) {
                    current_node->prev->next = current_node->next;
                }
                if(current_node->next != NULL) {
                    current_node->next->prev = current_node->prev;
                }
                if(current_node == allocator->free_list_head) {
                    allocator->free_list_head = current_node->next;
                }
                if(current_node == allocator->free_list_tail) {
                    allocator->free_list_tail = current_node->prev;
                }
                kfree(current_node);
            }
            struct memory_bookkeeping_node* current_allocated_node = allocator->allocated_list_head;
            if(current_allocated_node == NULL) {
                new_allocated_node->prev = NULL;
                new_allocated_node->next = NULL;
                allocator->allocated_list_head = new_allocated_node;
                allocator->allocated_list_tail = new_allocated_node;
            } else {
                bool has_inserted = false;
                while(current_allocated_node != NULL) {
                    if(new_allocated_node->page_index < current_allocated_node->page_index) {
                        new_allocated_node->prev = current_allocated_node->prev;
                        new_allocated_node->next = current_allocated_node;
                        if(current_allocated_node->prev != NULL) {
                            current_allocated_node->prev->next = new_allocated_node;
                        } else {
                            allocator->allocated_list_head = new_allocated_node;
                        }
                        current_allocated_node->prev = new_allocated_node;
                        has_inserted = true;
                        break;
                    }

                    current_allocated_node = current_allocated_node->next;
                }
                if(!has_inserted) {
                    new_allocated_node->prev = allocator->allocated_list_tail;
                    new_allocated_node->next = NULL;
                    allocator->allocated_list_tail->next = new_allocated_node;
                    allocator->allocated_list_tail = new_allocated_node;
                }
            }
            return (void*)(((uint32_t)allocator->start_of_virt_mem) + (new_allocated_node->page_index*PAGE_SIZE));
        }

        current_node = current_node->next;
    }

    return NULL;
}
void osi_memory_allocator_free(struct osi_memory_allocator *const allocator, const void *const ptr, const uint32_t num_of_pages) {
    if(allocator == NULL || ptr == NULL || num_of_pages == 0u) {
        return;
    }

    const uint32_t u32_ptr = ((uint32_t)ptr) & (~(PAGE_SIZE-1u)); //mask off any bits that are not page aligned
    const uint32_t page_index = (u32_ptr - ((uint32_t)allocator->start_of_virt_mem))/PAGE_SIZE;

    //handle `allocate_list`
    struct memory_bookkeeping_node* current_allocated_node = allocator->allocated_list_head;
    if(current_allocated_node == NULL) {
        return;
    }
    struct memory_bookkeeping_node* new_free_node = NULL;
    while(current_allocated_node != NULL) {
        if(current_allocated_node->page_index <= page_index && page_index < (current_allocated_node->page_index+current_allocated_node->num_of_pages)) {
            if((page_index+num_of_pages) > (current_allocated_node->page_index+current_allocated_node->num_of_pages)) {
                //freeing past the end of the allocated node
                kprintf("freeing past the end of the allocated node\n");
                return;
            }

            if(current_allocated_node->page_index == page_index) {
                if(current_allocated_node->num_of_pages == num_of_pages) {
                    new_free_node = current_allocated_node;
                    if(current_allocated_node->prev != NULL) {
                        current_allocated_node->prev->next = current_allocated_node->next;
                    } else {
                        allocator->allocated_list_head = current_allocated_node->next;
                    }
                    if(current_allocated_node->next != NULL) {
                        current_allocated_node->next->prev = current_allocated_node->prev;
                    } else {
                        allocator->allocated_list_tail = current_allocated_node->prev;
                    }
                } else {
                    //num_of_pages < current_allocated_node->num_of_pages
                    new_free_node = kmalloc(sizeof(struct memory_bookkeeping_node));
                    new_free_node->page_index = page_index;
                    new_free_node->num_of_pages = num_of_pages;

                    current_allocated_node->page_index += num_of_pages;
                    current_allocated_node->num_of_pages -= num_of_pages;
                }
            } else {
                //points to a different place in the block than the beginning of the block
                if((current_allocated_node->page_index+current_allocated_node->num_of_pages) == (page_index+num_of_pages)) {
                    new_free_node = kmalloc(sizeof(struct memory_bookkeeping_node));
                    new_free_node->page_index = page_index;
                    new_free_node->num_of_pages = num_of_pages;

                    current_allocated_node->num_of_pages -= num_of_pages;
                } else {
                    new_free_node = kmalloc(sizeof(struct memory_bookkeeping_node));
                    new_free_node->page_index = page_index;
                    new_free_node->num_of_pages = num_of_pages;

                    struct memory_bookkeeping_node *const second_allocated_node = kmalloc(sizeof(struct memory_bookkeeping_node));
                    second_allocated_node->page_index = page_index+num_of_pages;
                    second_allocated_node->num_of_pages = (current_allocated_node->page_index+current_allocated_node->num_of_pages) - (page_index+num_of_pages);

                    current_allocated_node->num_of_pages = page_index - current_allocated_node->page_index;

                    second_allocated_node->prev = current_allocated_node;
                    second_allocated_node->next = current_allocated_node->next;
                    if(current_allocated_node->next != NULL) {
                        current_allocated_node->next->prev = second_allocated_node;
                    } else {
                        allocator->allocated_list_tail = second_allocated_node;
                    }
                    current_allocated_node->next = second_allocated_node;
                }

                //temporary `prev` and `next` values
                new_free_node->prev = NULL;
                new_free_node->next = NULL;
            }

            break;
        }
        current_allocated_node = current_allocated_node->next;
    }

    //handle `free_list`
    struct memory_bookkeeping_node* current_free_node = allocator->free_list_head;
    if(current_free_node == NULL) {
        allocator->free_list_head = new_free_node;
        allocator->free_list_tail = new_free_node;
        return;
    }
    bool is_inserted = false;
    while(current_free_node != NULL) {
        if(new_free_node->page_index < current_allocated_node->page_index) {
            if(current_free_node->prev != NULL && (current_free_node->prev->page_index+current_free_node->prev->num_of_pages) == new_free_node->page_index) {
                //merge with previous node as it is before the new node
                current_allocated_node->prev->num_of_pages += new_free_node->num_of_pages;

                is_inserted = true;
            }
            if((new_free_node->page_index+new_free_node->num_of_pages) == current_free_node->page_index) {
                //merge with current node as it is after the new node
                if(is_inserted) {
                    //merge with `current_free_node->prev`
                    current_allocated_node->prev->num_of_pages += current_free_node->num_of_pages;

                    //remove current node
                    current_allocated_node->prev->next = current_free_node->next;
                    if(current_free_node->next != NULL) {
                        current_free_node->next->prev = current_allocated_node->prev;
                    } else {
                        allocator->free_list_tail = current_allocated_node->prev;
                    }
                } else {
                    //merge `new_free_node` with `current_free_node`

                    current_allocated_node->page_index = new_free_node->page_index;
                    current_allocated_node->num_of_pages += current_free_node->num_of_pages;

                    is_inserted = true;
                }
            }

            if(!is_inserted) {
                //insert `new_free_node`
                new_free_node->prev = current_free_node->prev;
                if(current_free_node->prev != NULL) {
                    current_free_node->prev->next = new_free_node;
                } else {
                    allocator->free_list_head = new_free_node;
                }
                new_free_node->next = current_free_node;
                current_free_node->prev = new_free_node;

                is_inserted = true;
            } else {
                kfree(new_free_node);
            }

            break;
        }

        current_free_node = current_free_node->next;
    }

    if(!is_inserted) {
        //insert `new_free_node` at end of `free_list`
        new_free_node->prev = allocator->free_list_tail;
        allocator->free_list_tail->next = new_free_node;
        allocator->free_list_tail = new_free_node;
    }
}
