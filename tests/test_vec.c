#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../std/vec.h"

// Test function for vec_init
void test_vec_init() {
    vec(int) v;
    vec_init(v);
    if (v->data == NULL && v->len == 0 && v->cap == 0) {
        printf("vec_init: PASSED\n");
    } else {
        printf("vec_init: FAILED\n");
    }
}

// Test function for vec_reserve
void test_vec_reserve() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    if (v->cap == 5) {
        printf("vec_reserve: PASSED\n");
    } else {
        printf("vec_reserve: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_truncate
void test_vec_truncate() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    vec_truncate(v, 2);
    if (v->len == 2) {
        printf("vec_truncate: PASSED\n");
    } else {
        printf("vec_truncate: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_compact
void test_vec_compact() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    vec_compact(v);
    if (v->cap == v->len) {
        printf("vec_compact: PASSED\n");
    } else {
        printf("vec_compact: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_clear
void test_vec_clear() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    vec_clear(v);
    if (v->len == 0) {
        printf("vec_clear: PASSED\n");
    } else {
        printf("vec_clear: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_len
void test_vec_len() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    size_t len = vec_len(v);
    if (len == 3) {
        printf("vec_len: PASSED\n");
    } else {
        printf("vec_len: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_is_empty
void test_vec_is_empty() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 0;
    bool is_empty = vec_is_empty(v);
    if (is_empty) {
        printf("vec_is_empty: PASSED\n");
    } else {
        printf("vec_is_empty: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_push
void test_vec_push() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    int val = 42;
    vec_push(v, val);
    if (v->len == 4 && v->data[3] == val) {
        printf("vec_push: PASSED\n");
    } else {
        printf("vec_push: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_pop
void test_vec_pop() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    int val = vec_pop(v);
    if (v->len == 2 && val == v->data[2]) {
        printf("vec_pop: PASSED\n");
    } else {
        printf("vec_pop: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_insert
void test_vec_insert() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    int val = 42;
    vec_insert(v, 1, val);
    if (v->len == 4 && v->data[1] == val) {
        printf("vec_insert: PASSED\n");
    } else {
        printf("vec_insert: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_remove
void test_vec_remove() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    int val = 42;
    vec_push(v, val);
    vec_remove(v, val);
    if (v->len == 3 && v->data[2] != val) {
        printf("vec_remove: PASSED\n");
    } else {
        printf("vec_remove: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_swap
void test_vec_swap() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    int val1 = 42;
    int val2 = 24;
    v->data[0] = val1;
    v->data[2] = val2;
    vec_swap(v, 0, 2);
    if (v->data[0] == val2 && v->data[2] == val1) {
        printf("vec_swap: PASSED\n");
    } else {
        printf("vec_swap: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_extend_from
void test_vec_extend_from() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    int buffer[] = {1, 2, 3};
    size_t n = sizeof(buffer) / sizeof(buffer[0]);
    vec_extend_from(v, buffer, n);
    if (v->len == 3 && v->data[0] == buffer[0] && v->data[1] == buffer[1] && v->data[2] == buffer[2]) {
        printf("vec_extend_from: PASSED\n");
    } else {
        printf("vec_extend_from: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_extend
void test_vec_extend() {
    vec(int) v1;
    vec(int) v2;
    vec_init(v1);
    vec_init(v2);
    vec_reserve(v1, 5);
    vec_push(v1, 1);
    vec_push(v2, 2);
    vec_push(v2, 3);
    vec_push(v2, 4);
    vec_extend(v1, v2);
    if (v1->len == 4 && v1->data[1] == 2 && v1->data[2] == 3 && v1->data[3] == 4) {
        printf("vec_extend: PASSED\n");
    } else {
        printf("vec_extend: FAILED\n");
    }
    vec_free(v1);
    vec_free(v2);
}

// Test function for vec_at
void test_vec_at() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    v->data[1] = 42;
    int val = vec_at(v, 1);
    if (val == 42) {
        printf("vec_at: PASSED\n");
    } else {
        printf("vec_at: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_first
void test_vec_first() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    v->data[0] = 42;
    int val = vec_first(v);
    if (val == 42) {
        printf("vec_first: PASSED\n");
    } else {
        printf("vec_first: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_last
void test_vec_last() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    v->data[2] = 42;
    int val = vec_last(v);
    if (val == 42) {
        printf("vec_last: PASSED\n");
    } else {
        printf("vec_last: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_sort
int cmp_func(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void test_vec_sort() {
    vec(int) v;
    vec_init(v);
    vec_push(v, 4);
    vec_push(v, 2);
    vec_push(v, 1);
    vec_push(v, 3);
    vec_sort(v, cmp_func);
    if (v->data[0] == 1 && v->data[1] == 2 && v->data[2] == 3 && v->data[3] == 4) {
        printf("vec_sort: PASSED\n");
    } else {
        printf("vec_sort: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_find
void test_vec_find() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 4;
    v->data[0] = 1;
    v->data[1] = 2;
    v->data[2] = 3;
    v->data[3] = 4;
    int index;
    vec_find(v, 3, index);
    if (index == 2) {
        printf("vec_find: PASSED\n");
    } else {
        printf("vec_find: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_reverse
void test_vec_reverse() {
    vec(int) v;
    vec_init(v);
    vec_push(v, 1);
    vec_push(v, 2);
    vec_push(v, 3);
    vec_push(v, 4);
    vec_reverse(v);
    if (v->data[0] == 4 && v->data[1] == 3 && v->data[2] == 2 && v->data[3] == 1) {
        printf("vec_reverse: PASSED\n");
    } else {
        printf("vec_reverse: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_iter
void test_vec_iter() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    v->data[0] = 1;
    v->data[1] = 2;
    v->data[2] = 3;
    int sum = 0;
    int i;
    vec_iter(v, i) {
        sum += i;
    }
    if (sum == 6) {
        printf("vec_iter: PASSED\n");
    } else {
        printf("vec_iter: FAILED\n");
    }
    vec_free(v);
}

// Test function for vec_enum
void test_vec_enum() {
    vec(int) v;
    vec_init(v);
    vec_reserve(v, 5);
    v->len = 3;
    v->data[0] = 1;
    v->data[1] = 2;
    v->data[2] = 3;
    int sum = 0;
    int index;
    int i;
    vec_enum(v, index, i) {
        sum += i;
    }
    if (sum == 6) {
        printf("vec_iter: PASSED\n");
    } else {
        printf("vec_iter: FAILED\n");
    }
    vec_free(v);
}

int main() {
    test_vec_init();
    test_vec_reserve();
    test_vec_truncate();
    test_vec_compact();
    test_vec_clear();
    test_vec_len();
    test_vec_is_empty();
    test_vec_push();
    test_vec_pop();
    test_vec_insert();
    test_vec_remove();
    test_vec_swap();
    test_vec_extend_from();
    test_vec_extend();
    test_vec_at();
    test_vec_first();
    test_vec_last();
    test_vec_sort();
    test_vec_find();
    test_vec_reverse();
    test_vec_iter();
    test_vec_enum();
    return 0;
}
   
