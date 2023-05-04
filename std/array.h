#ifndef STD_ARRAY_H
#define STD_ARRAY_H

#include "types.h"

#include <stdlib.h>
#include <string.h>

/*

array.h - generic fixed-size array type in C
based on https://github.com/rxi/vec

array(T) - the type of an array. Until C23, typedef this to something before using (see examples).

** Memory management **
array_init(a, n)                    -- initialize array with n elements
array_free(a)                       -- free all memory
array_resize(a, n)                  -- resize to n elements
array_fill(a, val)                  -- fill an array with val

** Properties **
array_len(a)                        -- the length of the array
array_at(a, i)                      -- return value at index i
array_first(a)                      -- get first element
array_last(a)                       -- get last element

** Operations **
array_write(a, i, val)              -- write val to index i
array_swap(a, i, j)                 -- swap 2 values
array_sort(a, fn)                   -- qsort in-place
array_reverse(a)                    -- reverse elements in-place

** Iteration **
array_iter(a, t)                    -- stores each value in t
array_enum(a, i, t)                 -- enumerate: stores each index in i and each value in t

*/

#define array(T)      \
  struct {            \
    T* data;          \
    usize len;        \
  }*                  \


// predefined types

typedef array(void*)  array_void;
typedef array(char*)  array_cstr;
typedef array(int)    array_int;
typedef array(char)   array_char;
typedef array(float)  array_float;
typedef array(double) array_double;


#define __a_unpack(a) \
  (void**)&(a)->data, &(a)->len, sizeof(*(a)->data)


// initialize array
#define array_init(a, n) \
  do {                                                              \
    *(void**)&(a) = calloc(1, sizeof(struct{void* d; usize l;}));   \
    *(void**)&((a)->data) = calloc(n, sizeof( *((a)->data) ));      \
    (a)->len = (n);                                                 \
  } while(0)


// free all memory
#define array_free(a) \
  (free((a)->data), free((a)))


// resize to n elements
#define array_resize(a, n) \
  __a_resize(__a_unpack(a), n)


// fill array with val
#define array_fill(a, v)                                              \
  do {                                                                \
    for (usize __i = 0; __i < (a)->len; __i++) {                      \
      (a)->data[__i] = (v);                                           \
    }                                                                 \
  } while(0)


// the length of the array
#define array_len(a) \
  ((a)->len)


// return value at index i
#define array_at(a, i) \
  ((a)->data[i])


// get first element
#define array_first(a) \
  ((a)->data[0])


// get last element
#define array_last(a) \
  ((a)->data[(a)->len - 1])


// write val to index i
#define array_write(a, i, val) \
  ((a)->data[i] = (val))


// swap 2 values
#define array_swap(a, i, j) \
  __a_swap(__a_unpack(a), i, j)


// qsort in-place
#define array_sort(a, fn) \
  qsort((a)->data, (a)->len, sizeof(*(a)->data), fn)


// reverse elements in-place
#define array_reverse(a)                              \
  do {                                                \
    int __i = (a)->len / 2;                           \
    while (__i--) {                                   \
      array_swap((a), __i, (a)->len - (__i + 1));     \
    }                                                 \
  } while (0)


// stores each value in t
#define array_iter(a, t)                                                      \
  if ((a)->len > 0)                                                           \
  for (usize __i = 0; __i < (a)->len && (((t) = (a)->data[__i]), 1); ++__i)   \


// enumerate: stores each index in i and each value in t
#define array_enum(a, i, t)                                                   \
  if ((a)->len > 0)                                                           \
  for ((i) = 0; (i) < (a)->len && (((t) = (a)->data[(i)]), 1); ++(i))         \


void __a_resize(void** data, usize* len, usize memsz, usize new_size) {
  *data = realloc(*data, (new_size * memsz));
  *len = new_size;
}

void __a_swap(void** data, usize* len, usize memsz, usize idx1, usize idx2) {
  unsigned char *a, *b, tmp;
  usize count;
  (void) len;
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

#endif // STD_ARRAY_H