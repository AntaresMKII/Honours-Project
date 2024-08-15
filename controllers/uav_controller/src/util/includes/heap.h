/**
 * \file heap.h
 *
 * Header file of the heap
 */ 
#ifndef HEAP_H
#define HEAP_H

#define MIN_HEAP 0  ///< Use to set the heap to min heap
#define MAX_HEAP 1  ///< Use to set the heap to max heap

#define get_root_val(h) h->arr[0].val   ///< Returns the root value of the heap
#define get_root_key(h) h->arr[0].key   ///< Returns the root key of the heap

#define pop_root_val(h) heap_extract(h).val ///< Pops the root value of the heap. It removes the root element
#define pop_root_key(h) heap_extract(h).key ///< Pops the root key of the heap. It removes the root element
#define pop_root(h) heap_extract(h)         ///< Pops the root element of the heap

/**
 * The node structure of the heap.
 * It contains one element of the heap,
 * its value and its key
 */ 
typedef struct NODE {
    void* key;
    void* val;
} NODE;

/**
 * The heap strucure containing the
 * array and lenght information as well as the type
 */ 
typedef struct HEAP {
    NODE *arr;
    int arr_len;
    int h_len;
    char type;
} HEAP;

NODE heap_extract(HEAP* h); ///< Extract the root value from the heap

/**
 * Creates a new heap and return a pointer to it.
 * This function requries as arguments an array
 * of elements and keys, both arrays can be empty,
 * the length of the passed data, a comparsion function
 * for both keys and values and the maximum allowed value
 */
HEAP* new_heap(void *data, void **keys, int h_len, char (*k_comp)(void*, void*), char (*v_comp)(void*, void*), void* max_val);

int heap_add(HEAP* h, void* val, void *key);    ///< Add a new value with a specific key to the heap
void heap_destroy(HEAP* h);                     ///< Destroy the heap
char heap_remove(HEAP *h, void* val);           ///< Remove a specific value from the heap or do nothing if the value is not there

#endif // !HEAP_H
