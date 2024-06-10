#include <limits.h>
#include <stdlib.h>

#include "includes/heap.h"

#define parent(i) i/2
#define left(i) 2*i + 1
#define right(i) 2*i + 2
#define key_at(h, i) h[i].key
#define heap_root_node(h) h[0]
#define heap_root_val(h) *h[0].val
#define heap_root_key(h) h[0].key

char (*key_comp)(void*, void*);
char (*val_comp)(void*, void*);

void* top_val;

void flip(NODE* h, int i, int j) {
    NODE tmp;

    tmp = h[i];
    h[i] = h[j];
    h[j] = tmp;
}

void heapify(NODE *arr, int h_len, int i) {
    int l, r, a;

    l = left(i);
    r = right(i);

    if ((l < h_len) && (key_comp(key_at(arr,l), key_at(arr, i)))) {
        a = l;
    }
    else {
        a = i;
    }

    if ((r < h_len) && (key_comp(key_at(arr, r), key_at(arr, a)))) {
        a = r;
    }

    if (a != i) {
        flip(arr, i, a);
        heapify(arr, h_len, a);
    }
}

void build_heap(NODE *arr, int n) {
    for (int i = n/2; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

NODE heap_extract_head(NODE* h, int *h_len) {
    NODE n = heap_root_node(h);

    *h_len = *h_len -1;
    h[0] = h[*h_len];
    heapify(h, *h_len, 0);

    return n;
}

int increase_key(NODE* h, NODE *x, void* k, int h_len) {
    int i;

    if (key_comp(x->key, k)) {
        return -1;
    }

    x->key = k;

    i = x - h;

    while ((i > 0) && (key_comp(key_at(h, i), key_at(h, parent(i))))) {
        flip(h, i, parent(i));
        i = parent(i);
    }

    return 0;
}

int heap_insert(NODE* h, NODE *x, int arr_len, int *h_len) {
    void* k;

    if (*h_len == arr_len) {
        return -1;
    }

    h[*h_len] = *x;

    k = key_at(h, *h_len);

    key_at(h, *h_len) = top_val;

    *h_len = *h_len + 1;

    return increase_key(h, &h[*h_len-1], k, *h_len);
}

HEAP* new_heap(void *data, void** keys, int h_len, char (*comp)(void*, void*), void* max_val) {
    NODE *arr;
    HEAP *h;
    int arr_len = 2 * h_len;

    arr = (NODE*) malloc(sizeof(NODE) * arr_len);
    if (arr == NULL) {
        return NULL;
    }

    for (int i = 0; i < h_len; i++) {
        arr[i].key = keys[i];
        arr[i].val = &data[i];
    }

    h = (HEAP*) malloc(sizeof(HEAP));
    h->arr = arr;
    h->h_len = h_len;
    h->arr_len = arr_len;

    key_comp = comp;
    top_val = max_val;

    build_heap(h->arr, h->h_len);

    return h;
}

NODE heap_extract(HEAP* h) {
    return heap_extract_head(h->arr, &h->h_len);
}

int heap_add(HEAP* h, void* val, void* key) {
    NODE x, *tmp;

    x.val = val;
    x.key = key;

    if (h->h_len > ((3 * h->arr_len) / 4)) {
        h->arr_len = h->arr_len * 2;
        tmp = (NODE*) malloc(sizeof(NODE) * h->arr_len);
        if (tmp == NULL) {
            return -1;
        }
        for (int i = 0; i < h->h_len; i++) {
            tmp[i] = h->arr[i];
        }
        free(h->arr);
        h->arr = tmp;
    }

    return heap_insert(h->arr, &x, h->arr_len, &h->h_len);
}

void heap_destroy(HEAP* h) {
    free(h->arr);
    free(h);
}

char heap_remove(HEAP *h, void* val) {
    int i;
    for (i = 0; i < h->h_len; i++) {
        if(val_comp(h->arr[i].val, val))
            break;
    }

    if (i == h->h_len) {
        return 0;
    }

    while (i > 0) {
        flip(h->arr, i, i-1);
        i--;
    }

    heap_extract(h);

    return 1;
}
