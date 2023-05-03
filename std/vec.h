#ifndef STD_VEC_H
#define STD_VEC_H

#include "types.h"

#include <stdlib.h>
#include <string.h>

/*

vec.h - generic dynamic array type in C
modified from https://github.com/rxi/vec

vec(T) - the type of a vector. Until C23, typedef this to something before using (see examples)

** Memory management **
vec_init(v)                         -- initialize vector
vec_deinit(v)                       -- free all memory
vec_reserve(v, n)                   -- reserve size for n elements
vec_truncate(v, n)                  -- reduce to just the first n elements
vec_compact(v)                      -- compact vector down to minimum size
vec_clear(v)                        -- len = 0

** Properties **
vec_len(v)                          -- the length of the vector
vec_is_empty(v)                     -- is the vector empty?
vec_at(v, i)                        -- return value at index i
vec_find(v, val, i)                 -- stores index of val in i
vec_first(v)                        -- get first element
vec_last(v)                         -- get last element

** Operations **
vec_push(v, val)                    -- pushes a value onto vector
vec_pop(v)                          -- pops an element off and returns it
vec_insert(v, i, val)               -- insert a value at index i
vec_remove(v, val)                  -- remove first occurrence of val
vec_swap(v, i, j)                   -- swap 2 values
vec_extend(v, v2)                   -- push all elements from another vector
vec_extend_from(v, b, n)            -- push n elements from buffer b
vec_sort(v, fn)                     -- qsort in-place
vec_reverse(v)                      -- reverse elements in-place
vec_splice(v, i, n)                 -- remove n elements starting at index i
vec_swapsplice(v, i, n)             -- and replace with last n elements

** Iteration **
vec_iter(v, t)                      -- stores each value in t
vec_enum(v, i, t)                   -- enumerate: stores each index in i and each value in t

*/

#define vec(T)        \
  struct {            \
    T* data;          \
    usize len;        \
    usize cap;        \
  }*                  \


// predefined types

typedef vec(void*)  vec_void;
typedef vec(char*)  vec_cstr;
typedef vec(int)    vec_int;
typedef vec(char)   vec_char;
typedef vec(float)  vec_float;
typedef vec(double) vec_double;


#define __v_unpack(v) \
  (void**)&(v)->data, &(v)->len, &(v)->cap, sizeof(*(v)->data)


// initialize vector
#define vec_init(v) \
  (v = calloc(1, sizeof(struct {void* data; usize len; usize cap;})))


// free all memory
#define vec_deinit(v)           \
  do {                          \
    free((v)->data);            \
    memset(v, 0, sizeof(*(v))); \
    free((v));                  \
  } while(0)


// reserve size for n elements
#define vec_reserve(v, n) \
  __v_reserve(__v_unpack(v), n)


// reduce to just the first n elements
#define vec_truncate(v, n) \
  ((v)->len = (n) < (v)->len ? (n) : (v)->len)


// compact vector down to minimum size
#define vec_compact(v) \
  __v_compact(__v_unpack(v))


// len = 0
#define vec_clear(v) \
  ((v)->len = 0)


// the length of the vector
#define vec_len(v) \
  ((v)->len)


// is the vector empty?
#define vec_is_empty(v) \
  ((v)->len <= 0)


// return value at index i
#define vec_at(v, i) \
  ((v)->data[i])


// stores index of val in i
#define vec_find(v, val, i)                           \
  do {                                                \
    for ((i) = 0; (i) < (v)->len; (i)++) {            \
      if ((v)->data[(i)] == (val)) break;             \
    }                                                 \
    if ((i) == (v)->len) (i) = -1;                    \
  } while (0)


// get first element
#define vec_first(v) \
  ((v)->data[0])


// get last element
#define vec_last(v) \
  ((v)->data[(v)->len - 1])


// pushes a value onto vector
#define vec_push(v, val) \
  (__v_expand(__v_unpack(v)) ? -1 : ((v)->data[(v)->len++] = (val), 0), 0)


// pops an element off and returns it
#define vec_pop(v) \
  ((v)->data[--(v)->len])


// insert a value at index i
#define vec_insert(v, i, val) \
  (__v_insert(__v_unpack(v), i) ? -1 : ((v)->data[i] = (val), 0), (v)->len++, 0)


// remove first occurrence of val
#define vec_remove(v, val)                                        \
  do {                                                            \
    int idx__;                                                    \
    vec_find(v, val, idx__);                                      \
    if (idx__ != -1) vec_splice(v, idx__, 1);                     \
  } while (0)


// swap 2 values
#define vec_swap(v, i, j) \
  __v_swap(__v_unpack(v), i, j)


// push all elements from another vector
#define vec_extend(v, v2) \
  vec_extend_from((v), (v2)->data, (v2)->len)


// push n elements from buffer b
#define vec_extend_from(v, b, n)                                            \
  do {                                                                      \
    int __i, __n = (n);                                                     \
    if (__v_reserve_po2(__v_unpack(v), (v)->len + __n) != 0) break;         \
    for (__i = 0; __i < __n; __i++) {                                       \
      (v)->data[(v)->len++] = (b)[__i];                                     \
    }                                                                       \
  } while (0)


// qsort in-place
#define vec_sort(v, fn) \
  qsort((v)->data, (v)->len, sizeof(*(v)->data), fn)


// reverse elements in-place
#define vec_reverse(v)                                \
  do {                                                \
    int __i = (v)->len / 2;                           \
    while (__i--) {                                   \
      vec_swap((v), __i, (v)->len - (__i + 1));       \
    }                                                 \
  } while (0)


// remove n elements starting at index i
#define vec_splice(v, i, n) \
  (__v_splice(__v_unpack(v), i, n), (v)->len -= (n))


// remove n elements starting at index i and replace with last n elements
#define vec_swapsplice(v, i, n) \
  (__v_swapsplice(__v_unpack(v), i, n), (v)->len -= (n))


// stores each value in t
#define vec_iter(v, t)                                                                      \
  if ((v)->len > 0)                                                                         \
  for (usize __i = 0; __i < (v)->len && (((t) = (v)->data[__i]), 1); ++__i)                 \


// enumerate: stores each index in i and each value in t
#define vec_enum(v, i, t)                                                                   \
  if ((v)->len > 0)                                                                         \
  for (i = 0; i < (v)->len && (((t) = (v)->data[i]), 1); ++i)                               \


int __v_expand(void** data, usize* len, usize* cap, usize memsz) {
  if (*len + 1 > *cap) {
    void *ptr;
    int n = (*cap == 0) ? 1 : *cap << 1;
    ptr = realloc(*data, n * memsz);
    if (ptr == null) return -1;
    *data = ptr;
    *cap = n;
  }
  return 0;
}


int __v_reserve(void** data, usize* len, usize* cap, usize memsz, usize n) {
  (void) len;
  if (n > *cap) {
    void *ptr = realloc(*data, n * memsz);
    if (ptr == null) return -1;
    *data = ptr;
    *cap = n;
  }
  return 0;
}


int __v_reserve_po2(void** data, usize* len, usize* cap, usize memsz, usize n) {
  usize n2 = 1;
  if (n == 0) return 0;
  while (n2 < n) n2 <<= 1;
  return __v_reserve(data, len, cap, memsz, n2);
}


int __v_compact(void** data, usize* len, usize* cap, usize memsz) {
  if (*len == 0) {
    free(*data);
    *data = null;
    *cap = 0;
    return 0;
  } else {
    void *ptr;
    usize n = *len;
    ptr = realloc(*data, n * memsz);
    if (ptr == null) return -1;
    *cap = n;
    *data = ptr;
  }
  return 0;
}


int __v_insert(void** data, usize* len, usize* cap, usize memsz, usize idx) {
  int err = __v_expand(data, len, cap, memsz);
  if (err) return err;
  memmove(*data + (idx + 1) * memsz,
          *data + idx * memsz,
          (*len - idx) * memsz);
  return 0;
}


void __v_splice(void **data, usize *len, usize *cap, usize memsz, usize start, usize count) {
  (void) cap;
  memmove(*data + start * memsz,
          *data + (start + count) * memsz,
          (*len - start - count) * memsz);
}


void __v_swapsplice(void **data, usize *len, usize *cap, usize memsz, usize start, usize count) {
  (void) cap;
  memmove(*data + start * memsz,
          *data + (*len - count) * memsz,
          count * memsz);
}

void __v_swap(void** data, usize* len, usize* cap, usize memsz, usize idx1, usize idx2) {
  unsigned char *a, *b, tmp;
  usize count;
  (void) len;
  (void) cap;
  if (idx1 == idx2) return;
  a = (unsigned char*) *data + idx1 * memsz;
  b = (unsigned char*) *data + idx2 * memsz;
  count = memsz;
  while (count--) {
    tmp = *a;
    *a = *b;
    *b = tmp;
    a++, b++;
  }
}


#endif // STD_VEC_H