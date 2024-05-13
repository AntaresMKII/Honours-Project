#define MIN_HEAP 0
#define MAX_HEAP 1

#define get_root_val(h) h->arr[0].val
#define get_root_key(h) h->arr[0].key

#define pop_root_val(h) heap_extract(h).val
#define pop_root_key(h) heap_extract(h).key
#define pop_root(h) heap_extract(h)

typedef struct NODE {
    int key;
    void* val;
} NODE;

typedef struct HEAP {
    NODE *arr;
    int arr_len;
    int h_len;
    char type; 
} HEAP;

NODE heap_extract(HEAP* h);

HEAP* new_heap(void *data, int *keys, int h_len, char h_type);

int heap_add(HEAP* h, void* val, int key);

void heap_destroy(HEAP* h);
