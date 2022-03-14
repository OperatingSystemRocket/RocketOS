#include <utils/algorithms/sort/quicksort.h>


static uint32_t partition_array(uint32_t *const array, const uint32_t lo_index, const uint32_t hi_index) {
    const uint32_t pivot = array[hi_index];
    uint32_t i = lo_index - 1u;
    for(uint32_t j = lo_index; j < hi_index; j++) {
        if(array[j] <= pivot) {
            ++i;
            swap(&array[i], &array[j], sizeof(uint32_t));
        }
    }
    ++i;
    swap(&array[i], &array[hi_index], sizeof(uint32_t));
    return i;
}
void quicksort_array(uint32_t *const array, const uint32_t lo_index, const uint32_t hi_index) {
    if(lo_index >= hi_index) {
        return;
    }
    const uint32_t p = partition_array(array, lo_index, hi_index);
    quicksort_array(array, lo_index, p-1u);
    quicksort_array(array, p+1u, hi_index);
}

static struct memory_bookkeeping_node* partition_linked_list(struct memory_bookkeeping_node *const head, struct memory_bookkeeping_node *const tail) {
    const uint32_t x = head->page_index;

    struct memory_bookkeeping_node* i = tail->prev;

    for (struct memory_bookkeeping_node* j = tail; j != head; j = j->next)
    {
        if (j->page_index <= x)
        {
            i = (i == NULL) ? tail : i->next;
            swap(&(i->page_index), &(j->page_index), sizeof(int));
        }
    }
    i = (i == NULL) ? tail : i->next;
    swap(&(i->page_index), &(head->page_index), sizeof(int));
    return i;
}
static void quicksort_linked_list_impl(struct memory_bookkeeping_node *const head, struct memory_bookkeeping_node *const tail) {
    if(head != NULL && tail != head && tail != head->next)
    {
        struct memory_bookkeeping_node *const p = partition_linked_list(tail, head);
        quicksort_linked_list_impl(tail, p->prev);
        quicksort_linked_list_impl(p->next, head);
    }
}
static struct memory_bookkeeping_node* get_tail(struct memory_bookkeeping_node *const head) {
    struct memory_bookkeeping_node* tail = head;
    while(tail != NULL && tail->next != NULL) {
        tail = tail->next;
    }
    return tail;
}
void quicksort_linked_list_head(struct memory_bookkeeping_node *const head) {
    quicksort_linked_list_impl(head, get_tail(head));
}
void quicksort_linked_list_head_tail(struct memory_bookkeeping_node *const head, struct memory_bookkeeping_node *const tail) {
    quicksort_linked_list_impl(head, tail);
}
