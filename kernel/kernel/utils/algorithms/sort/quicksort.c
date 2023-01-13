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
