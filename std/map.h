#ifndef STD_MAP_H
#define STD_MAP_H

#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "str.h"
#include "types.h"

/*

map.h - generic hashmap type in C (with string keys)

map(V) - the type of a map with string keys and values of type V.
Until C23, typedef this to something before using (see examples).

** Memory management **
map_init(m)                     -- initialize map
map_free(m)                     -- free all memory
map_clear(m)                    -- clear all keys and values

** Properties **
map_size(m)                     -- number of kv-pairs stored in the map
map_is_empty(m)                 -- is the map empty
map_contains(m, k)              -- does the map contain a key

** Operations **
map_get(m, k)                   -- find a value by key
map_insert(m, k, v)             -- if k exists overwrite, otherwise create new pair
map_remove(k)                   -- removes a pair by key

** Iteration **
map_keys(m)                     -- return array of all keys in the map

*/

#define map(V)              \
    struct {                \
        struct {            \
            void* next;     \
            c_str key;      \
            V value;        \
        }** entries;        \
        usize size;         \
        usize cap;          \
    }*


// predefined types

typedef map(void*)  map_void;
typedef map(char*)  map_cstr;
typedef map(int)    map_int;
typedef map(char)   map_char;
typedef map(float)  map_float;
typedef map(double) map_double;


// starting capacy of a map
#define STD_MAP_STARTING_CAP 10

// when this percentage of the cells are full, the map will be rehashed
#define STD_MAP_MIN_RATIO 0.75

// the map will be rehashed to this times the current capacity
#define STD_MAP_RESIZE_FACTOR 2

// size of any map type
#define STD_MAP_SIZEOF_MAP sizeof(struct{void* e; usize s, c;})

// size of an entry for a specific map
#define STD_MAP_SIZEOF_ENTRY(m) sizeof(**(m)->entries)

// size of value type for a specific map
#define STD_MAP_SIZEOF_V(m) sizeof((**(m)->entries).value)

// offset of a field within an entry struct
#define STD_MAP_E_OFF(x) \
    offsetof(struct { void* next; c_str key; int value; }, x)

// e->next
#define STD_MAP_E_NEXT(e) \
    ((void**)(e))

// e->key
#define STD_MAP_E_KEY(e) \
    ((c_str*)((c_str)(e) + STD_MAP_E_OFF(key)))

// e->value
#define STD_MAP_E_VALUE(e) \
    (((c_str)(e) + STD_MAP_E_OFF(value)))


#define __m_unpack(m) \
    (void***)&(m)->entries, &(m)->size, &(m)->cap, STD_MAP_SIZEOF_V(m)


// initialize map
#define map_init(m)                                                     \
    do {                                                                \
        (m) = malloc(STD_MAP_SIZEOF_MAP);                               \
        (m)->entries = calloc(STD_MAP_STARTING_CAP, sizeof(void*));     \
        (m)->cap = STD_MAP_STARTING_CAP;                                \
        (m)->size = 0;                                                  \
    } while(0)


// free all memory
#define map_free(m)                                         \
    do {                                                    \
        for (usize mdi_ = 0; mdi_ < (m)->cap; mdi_++) {     \
            if ((m)->entries[mdi_] == null) {               \
                continue;                                   \
            }                                               \
            free((m)->entries[mdi_]->next);                 \
            free((m)->entries[mdi_]->key);                  \
        }                                                   \
        free((m)->entries);                                 \
        free((m));                                          \
        (m) = null;                                         \
    } while(0)


// clear all keys and values
#define map_clear(m)    \
    do {                \
        map_free(m);    \
        map_init(m);    \
    } while(0)


// number of kv-pairs stored in the map
#define map_size(m) \
    (m->size)


// is the map empty
#define map_is_empty(m) \
    (m == null || m->size == 0)


// does the map contain a key
#define map_contains(m, k) \
    __m_contains(__m_unpack(m), k)


bool __m_contains(void*** entries, usize* size, usize* cap, usize memsz, c_str key) {
    (void)size;
    (void)memsz;
    usize index = str_hash(key) % *cap;
    void* entry = (*entries)[index];
    while (entry != null) {
        if (str_equals(*STD_MAP_E_KEY(entry), key)) {
            return true;
        }
        entry = *STD_MAP_E_NEXT(entry);
    }
    return false;   
}


// if k exists overwrite, otherwise create new pair
#define map_insert(m, k, v) \
    __m_insert(m, k, v, __LINE__)


#define __m_label(n, l) n ## l


#define __m_insert(m, k, v, l)                                  \
    do {                                                        \
        if ((m)->size >= STD_MAP_MIN_RATIO * (m)->cap) {        \
            __m_rh(m);                                          \
        }                                                       \
        isize msi_ = str_hash(k) % (m)->cap;                    \
        while ((m)->entries[msi_] != null) {                    \
            if (str_equals((m)->entries[msi_]->key, (k))) {     \
                (m)->entries[msi_]->value = (v);                \
                goto __m_label(__m_ins_end_, l);                \
            }                                                   \
            (m)->entries[msi_] = (m)->entries[msi_]->next;      \
        }                                                       \
        void* mso_ = (m)->entries[msi_];                        \
        void* mse_ = malloc(STD_MAP_SIZEOF_ENTRY(m));           \
        (m)->entries[msi_] = mse_;                              \
        (m)->entries[msi_]->key = strdup(k);                    \
        (m)->entries[msi_]->value = (v);                        \
        (m)->entries[msi_]->next = mso_;                        \
        (m)->size++;                                            \
        __m_label(__m_ins_end_, l):                             \
    } while(0)    


// find a value by key
#define map_get(m, k) \
    ((void*)STD_MAP_E_VALUE(__m_key_addr(__m_unpack(m), k)))


#define __m_rh(m)                                                       \
    do {                                                                \
        usize mrhnc_ = STD_MAP_RESIZE_FACTOR * (m)->cap;                \
        void** mrhe2_ = (void**) calloc(mrhnc_, sizeof(void*));         \
        for (usize mrhi_ = 0; mrhi_ < (m)->cap; mrhi_++) {              \
            void* e = (m)->entries[mrhi_];                              \
            while (e != null) {                                         \
                usize mrhni_ = str_hash(*STD_MAP_E_KEY(e)) % mrhnc_;    \
                void* mrhnx_ = *STD_MAP_E_NEXT(e);                      \
                *STD_MAP_E_NEXT(e) = mrhe2_[mrhni_];                    \
                mrhe2_[mrhni_] = e;                                     \
                e = mrhnx_;                                             \
            }                                                           \
        }                                                               \
        free((m)->entries);                                             \
        *((void***) &((m)->entries)) = mrhe2_;                          \
        (m)->cap = mrhnc_;                                              \
    } while(0)


void* __m_key_addr(void*** entries, usize* size, usize* cap, usize memsz, c_str key) {
    (void)size;
    (void)memsz;
    usize index = str_hash(key) % *cap;
    void* entry = (*entries)[index];
    while (entry != null) {
        if (str_equals(*STD_MAP_E_KEY(entry), key)) {
            return entry;
        }
        entry = *STD_MAP_E_NEXT(entry);
    }
    return null;
}


// removes a pair by key
#define map_remove(m, k) \
    __m_remove(m, k, __LINE__)


#define __m_remove(m, k, l)                                                 \
    do {                                                                    \
        usize entry_index = str_hash(k) % (m)->cap;                         \
        void* entry = (m)->entries[entry_index];                            \
        void* prev = null;                                                  \
        while (entry != null) {                                             \
            if (str_equals(*STD_MAP_E_KEY(entry), k)) {                     \
                if (prev == null) {                                         \
                    (m)->entries[entry_index] = *STD_MAP_E_NEXT(entry);     \
                } else {                                                    \
                    *STD_MAP_E_NEXT(prev) = *STD_MAP_E_NEXT(entry);         \
                }                                                           \
                free(*STD_MAP_E_KEY(entry));                                \
                free(entry);                                                \
                (m)->size--;                                                \
                goto __m_label(__mr, l);                                    \
            }                                                               \
            prev = entry;                                                   \
            entry = *STD_MAP_E_NEXT(entry);                                 \
        }                                                                   \
        __m_label(__mr, l):                                                 \
    } while(0)


// return array of all keys in the map
#define map_keys(m) \
    __m_keys(__m_unpack(m))


array_cstr __m_keys(void*** entries, usize* size, usize* cap, usize memsz) {
    (void)memsz;
    array_cstr a;
    array_init(a, *size);
    usize i = 0;
    for (usize j = 0; j < *cap; j++) {
        for (void* entry = (*entries)[j]; entry != null; entry = *STD_MAP_E_NEXT(entry)) {
            char* key = *STD_MAP_E_KEY(entry);
            array_write(a, i, key);
            i++;
        }
    }
    return a;
}


#endif // STD_MAP_H