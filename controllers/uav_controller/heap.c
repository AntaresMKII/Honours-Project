#include <limits.h>
#include <stdlib.h>

#include "heap.h"

#define parent(i) i/2
#define left(i) 2*i + 1
#define right(i) 2*i + 2
#define key_at(h, i) h[i].key
#define heap_root_node(h) h[0]
#define heap_root_val(h) *h[0].val
#define heap_root_key(h) h[0].key

void flip(NODE* h, int i, int j) {
    NODE tmp;

    tmp = h[i];
    h[i] = h[j];
    h[j] = tmp;
}

void min_heapify(NODE *arr, int h_len, int i) {
    int l, r, a;

    l = left(i);
    r = right(i);

    if ((l < h_len) && (key_at(arr,l) < key_at(arr, i))) {
        a = l;
    }
    else {
        a = i;
    }

    if ((r < h_len) && (key_at(arr, r) < key_at(arr, a))) {
        a = r;
    }

    if (a != i) {
        flip(arr, i, a);
        min_heapify(arr, h_len, a);
    }
}

void max_heapify(NODE *arr, int h_len, int i) {
    int l, r, a;

    l = left(i);
    r = right(i);

    if ((l < h_len) && (key_at(arr,l) > key_at(arr, i))) {
        a = l;
    }
    else {
        a = i;
    }

    if ((r < h_len) && (key_at(arr, r) > key_at(arr, a))) {
        a = r;
    }

    if (a != i) {
        flip(arr, i, a);
        min_heapify(arr, h_len, a);
    }
}

void build_heap(NODE *arr, int n, char type) {
    if (type == MAX_HEAP) {
        for (int i = n/2; i >= 0; i--) {
            max_heapify(arr, n, i);
        }
    }
    else {
        for (int i = n/2; i >= 0; i--) {
            min_heapify(arr, n, i);
        }
    }
}

NODE heap_extract_max(NODE* h, int *h_len) {
    NODE max = heap_root_node(h);

    *h_len = *h_len -1;
    h[0] = h[*h_len];
    max_heapify(h, *h_len, 0);

    return max;
}

NODE heap_extract_min(NODE* h, int *h_len) {
    NODE min = heap_root_node(h);

    *h_len = *h_len -1;
    h[0] = h[*h_len];
    min_heapify(h, *h_len, 0);

    return min;
}

int min_increase_key(NODE* h, NODE *x, int k, int h_len) {
    int i;

    if (x->key < k) {
        return -1;
    }

    x->key = k;

    i = x - h;

    while ((i > 0) && (key_at(h, i) < key_at(h, parent(i)))) {
        flip(h, i, parent(i));
        i = parent(i);
    }

    return 0;
}

int max_increase_key(NODE* h, NODE *x, int k, int h_len) {
    int i;

    if (x->key > k) {
        return -1;
    }

    x->key = k;

    i = x - h;

    while ((i > 0) && (key_at(h, i) > key_at(h, parent(i)))) {
        flip(h, i, parent(i));
        i = parent(i);
    }

    return 0;
}

int min_heap_insert(NODE* h, NODE *x, int arr_len, int *h_len) {
    int k;

    if (*h_len == arr_len) {
        return -1;
    }

    h[*h_len] = *x;

    k = key_at(h, *h_len);

    key_at(h, *h_len) = INT_MAX;

    *h_len = *h_len + 1;

    return max_increase_key(h, &h[*h_len-1], k, *h_len);
}

int max_heap_insert(NODE* h, NODE *x, int arr_len, int *h_len) {
    int k;

    if (*h_len == arr_len) {
        return -1;
    }

    h[*h_len] = *x;

    k = key_at(h, *h_len);

    key_at(h, *h_len) = INT_MIN;

    *h_len = *h_len + 1;

    return max_increase_key(h, &h[*h_len-1], k, *h_len);
}

HEAP* new_heap(void *data, int *keys, int h_len, char h_type) {
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
    h->type = h_type;

    build_heap(h->arr, h->h_len, h->type);

    return h;
}

NODE heap_extract(HEAP* h) {
    if (h->type == MAX_HEAP) {
        return heap_extract_max(h->arr, &h->h_len);
    }
    else {
        return heap_extract_min(h->arr, &h->h_len);
    }
}

int heap_add(HEAP* h, void* val, int key) {
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

    if (h->type == MAX_HEAP) {
        return max_heap_insert(h->arr, &x, h->arr_len, &h->h_len);
    }
    else {
        return min_heap_insert(h->arr, &x, h->arr_len, &h->h_len);
    }
}

void heap_destroy(HEAP* h) {
    free(h->arr);
    free(h);
}
