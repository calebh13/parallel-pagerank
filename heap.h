#include <stdlib.h>
#include <inttypes.h>

typedef struct MaxHeap {
    void** arr;
    size_t cur_size;
    size_t max_size;
} MaxHeap;

// If max_size < 0, then it will be unbounded
MaxHeap* MaxHeap_init(size_t max_size);
void MaxHeap_insert(MaxHeap* heap, void* x, int (*cmp)(const void *, const void *));
void* MaxHeap_pop(MaxHeap* heap, int (*compar)(const void *, const void *));
void* MaxHeap_peek(MaxHeap* heap);