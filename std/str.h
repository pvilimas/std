#ifndef STD_STR_H
#define STD_STR_H

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "types.h"

// str.h - string library in C
// (compatible with c-strings)

// TODO rewrite character methods without `char[2]{ c, '\0' }

// Types

// string type
typedef struct __str {
    char* chars;
    usize len;
}* str;

// c-string type
typedef char* c_str;

// array of strings
typedef array(str) array_str;

// Methods

str         str_new();                              // empty string
str         str_alloc(usize n);                     // allocate n bytes (+1 for \0), zero out
void        str_free(str s);                        // free memory
str         str_from(c_str cs);                     // deep copy c-string
str         str_copy(str s);                        // deep copy string
c_str       str_chars(str s);                       // returns pointer to self.chars
char        str_at(str s, usize index);             // index, no bounds checking
void        str_write(str s, char c, usize i);      // write character, no bounds checking
void        str_remove(str s, usize i, usize n);    // remove n characters starting at index i
void        str_print(str s);                       // str("my contents")\n
str         str_format(c_str fmt, ...);             // sprintf into a string
c_str       str_format_c(c_str fmt, ...);           // sprintf into a c-string

/*

Generic methods
    S - any string: str or c_str
    X - any value: str, c_str, or char

usize       str_len(S)                          -- length of a string
bool        str_equals(S, S)                    -- compare two strings
usize       str_hash(S)                         -- hash of a string
bool        str_starts_with(S, X)               -- does string start with a value?
bool        str_ends_with(S, X)                 -- does string end with a value?
bool        str_has(str, X)                     -- does string contain a value?
isize       str_find(str, X)                    -- first index of value in string, or -1
usize       str_count(str, X)                   -- count number of times value appears in string
str         str_slice(S, isize, isize)          -- python-style slice, returns deep copy
void        str_append(str, X)                  -- in-place append value
void        str_prepend(str, X)                 -- in-place prepend value
void        str_insert(str, X, usize)           -- in-place insert value at specific index
array_str   str_split(S, X)                     -- split by delimiter
array_str   str_partition(S, X)                 -- python-style partition by delimiter, see below
str         str_join(array_str, X)              -- join a list of strings on a delimiter
str         str_upper(S)                        -- in-place convert to uppercase
str         str_lower(S)                        -- in-place convert to lowercase
str         str_reverse(S)                      -- in-place reverse characters
str         str_repeat(X, usize)                -- construct a string from a value repeated N times

Specific stuff:
- slice, split, partition work as in python
    - slice(s, 5, 8) is like s[5:8] but returns deep copy
    - split and partition with "" or str("") as the delim will split on every character
- partition will always return array of length 3, two cases:
    - delim was found:
        [everything before the first occurrence of delim,
        delim,
        everything after]
    - delim was not found:
        [everything in the original string
        empty string
        empty string]

*/



// GENERIC MACROS
// type-specific function names are mangled to avoid confusion, just use the macros

#define STD_STR_DECL static __attribute__((unused))

// length of a string
#define str_len(s)                  \
    (_Generic((s),                  \
        str: __sls,                 \
        c_str: __slc                \
    )(s))

STD_STR_DECL usize __sls(str);
STD_STR_DECL usize __slc(c_str);

// compare two strings
#define str_equals(a, b)                            \
    (_Generic((a),                                  \
        str: _Generic((b),                          \
            str: __seqss,                           \
            c_str: __seqsc                          \
        ),                                          \
        c_str: _Generic((b),                        \
            str: __seqcs,                           \
            c_str: __seqcc                          \
        )                                           \
    )((a), (b)))

STD_STR_DECL bool __seqss(str, str);
STD_STR_DECL bool __seqsc(str, c_str);
STD_STR_DECL bool __seqcs(c_str, str);
STD_STR_DECL bool __seqcc(c_str, c_str);

// hash of a string
#define str_hash(s)                 \
    (_Generic((s),                  \
        str: __shs,                 \
        c_str: __shc                \
    )(s))

STD_STR_DECL usize __shs(str);
STD_STR_DECL usize __shc(c_str);

// does string start with a value?
#define str_starts_with(s, x)       \
    (_Generic((x),                  \
        str: __ssws,                \
        c_str: __sswc,              \
        int: __sswch                \
    )((s), (x)))

STD_STR_DECL bool __ssws(str, str);
STD_STR_DECL bool __sswc(str, c_str);
STD_STR_DECL bool __sswch(str, char);

// does string end with a value?
#define str_ends_with(s, x)         \
    (_Generic((x),                  \
        str: __sews,                \
        c_str: __sewc,              \
        int: __sewch                \
    )((s), (x)))

STD_STR_DECL bool __sews(str, str);
STD_STR_DECL bool __sewc(str, c_str);
STD_STR_DECL bool __sewch(str, char);

// does string contain a value?
#define str_has(s, x)               \
    (_Generic((x),                  \
        str: __shass,               \
        c_str: __shasc,             \
        int: __shasch               \
    )((s), (x)))

STD_STR_DECL bool __shass(str, str);
STD_STR_DECL bool __shasc(str, c_str);
STD_STR_DECL bool __shasch(str, char);

// first index of value in string, or -1
#define str_find(s, x)              \
    (_Generic((x),                  \
        str: __sfs,                 \
        c_str: __sfc,               \
        int: __sfch                 \
    )((s), (x)))

STD_STR_DECL isize __sfs(str, str);
STD_STR_DECL isize __sfc(str, c_str);
STD_STR_DECL isize __sfch(str, char);

// count number of times value appears in string
#define str_count(s, x)             \
    (_Generic((x),                  \
        str: __scnts,               \
        c_str: __snctc,             \
        int: __scntch               \
    )((s), (x)))

STD_STR_DECL usize __scnts(str, str);
STD_STR_DECL usize __snctc(str, c_str);
STD_STR_DECL usize __scntch(str, char);

// python-style slice, returns deep copy
#define str_slice(s, i, j)          \
    (_Generic((s),                  \
        str: __ssls,                \
        c_str: __sslc               \
    )((s), (i), (j)))

STD_STR_DECL str __ssls(str, isize, isize);
STD_STR_DECL str __sslc(c_str, isize, isize);

// in-place append value
#define str_append(s, x)            \
    (_Generic((x),                  \
        str: __saps,                \
        c_str: __sapc,              \
        char: __sapch               \
    )((s), (x)))

STD_STR_DECL void __saps(str, str);
STD_STR_DECL void __sapc(str, c_str);
STD_STR_DECL void __sapch(str, char);

// in-place prepend value
#define str_prepend(s, x)           \
    (_Generic((x),                  \
        str: __sprs,                \
        c_str: __sprc,              \
        int: __sprch                \
    )((s), (x)))

STD_STR_DECL void __sprs(str, str);
STD_STR_DECL void __sprc(str, c_str);
STD_STR_DECL void __sprch(str, char);

// in-place insert value at specific index
#define str_insert(s, x, i)         \
    (_Generic((x),                  \
        str: __sinss,               \
        c_str: __sinsc,             \
        int: __sinsch               \
    )((s), (x), (i)))

STD_STR_DECL void __sinss(str, str, usize);
STD_STR_DECL void __sinsc(str, c_str, usize);
STD_STR_DECL void __sinsch(str, char, usize);

// split by delimiter
#define str_split(s, x)                             \
    (_Generic((s),                                  \
        str: _Generic((x),                          \
            str: __ssplss,                          \
            c_str: __ssplsc,                        \
            int: __ssplsch                          \
        ),                                          \
        c_str: _Generic((x),                        \
            str: __ssplcs,                          \
            c_str: __ssplcc,                        \
            int: __ssplcch                          \
        )                                           \
    )((s), (x)))

STD_STR_DECL array_str __ssplss(str, str);
STD_STR_DECL array_str __ssplsc(str, c_str);
STD_STR_DECL array_str __ssplsch(str, char);
STD_STR_DECL array_str __ssplcs(c_str, str);
STD_STR_DECL array_str __ssplcc(c_str, c_str);
STD_STR_DECL array_str __ssplcch(c_str, char);

// python-style partition by delimiter
#define str_partition(s, x)                             \
    (_Generic((s),                                      \
        str: _Generic((x),                              \
            str: __sprtss,                              \
            c_str: __sprtsc,                            \
            int: __sprtsch                              \
        ),                                              \
        c_str: _Generic((x),                            \
            str: __sprtcs,                              \
            c_str: __sprtcc,                            \
            int: __sprtcch                              \
        )                                               \
    )((s), (x)))

STD_STR_DECL array_str __sprtss(str, str);
STD_STR_DECL array_str __sprtsc(str, c_str);
STD_STR_DECL array_str __sprtsch(str, char);
STD_STR_DECL array_str __sprtcs(c_str, str);
STD_STR_DECL array_str __sprtcc(c_str, c_str);
STD_STR_DECL array_str __sprtcch(c_str, char);

// join a list of strings on a delimiter
#define str_join(l, x)              \
    (_Generic((x),                  \
        str: __sjs,                 \
        c_str: __sjc,               \
        int: __sjch                 \
    )((l), (x)))

STD_STR_DECL str __sjs(array_str, str);
STD_STR_DECL str __sjc(array_str, c_str);
STD_STR_DECL str __sjch(array_str, char);

// in-place convert to uppercase
#define str_upper(s)                \
    (_Generic((s),                  \
        str: __sups,                \
        c_str: __supc               \
    )(s))

STD_STR_DECL str __sups(str);
STD_STR_DECL str __supc(c_str);

// in-place convert to lowercase
#define str_lower(s)                \
    (_Generic((s),                  \
        str: __slos,                \
        c_str: __sloc               \
    )(s))

STD_STR_DECL str __slos(str);
STD_STR_DECL str __sloc(c_str);

// in-place reverse characters
#define str_reverse(s)              \
    (_Generic((s),                  \
        str: __sres,                \
        c_str: __srec               \
    )(s))

STD_STR_DECL str __sres(str);
STD_STR_DECL str __srec(c_str);

// construct a string from a value repeated N times
#define str_repeat(s, x)            \
    (_Generic((s),                  \
        str: __sreps,               \
        c_str: __srepc,             \
        int: __srepch               \
    )((s), (x)))

STD_STR_DECL str __sreps(str, usize);
STD_STR_DECL str __srepc(c_str, usize);
STD_STR_DECL str __srepch(char, usize);


// DEFINITIONS


// empty string
str str_new() {
    str s = (str) calloc(1, sizeof(struct __str));
    s->chars = (char*) calloc(1, sizeof(char));
    return s;
}

// allocate n bytes, zero out
str str_alloc(usize n) {
    str s = (str) calloc(1, sizeof(struct __str));
    s->chars = (char*) calloc(n + 1, sizeof(char));
    s->len = n;
    return s;
}

// free memory
void str_free(str s) {
    free(s->chars);
    s->chars = null;
    s->len = 0;
    free(s);
}

// deep copy c-string
str str_from(c_str cs) {
    usize len = strlen(cs);
    str s = (str) calloc(1, sizeof(struct __str));
    s->chars = (char*) calloc(len + 1, sizeof(char));
    s->len = len;
    memcpy(s->chars, cs, len);
    return s;
}

// deep copy string
str str_copy(str s) {
    str s2 = (str) calloc(1, sizeof(struct __str));
    s2->chars = (char*) calloc(s->len + 1, sizeof(char));
    s2->len = s->len;
    memcpy(s2->chars, s->chars, s->len);
    return s2;
}

// returns pointer to self.chars
c_str str_chars(str s) {
    return s->chars;
}

// index, no bounds checking
char str_at(str s, usize index) {
    return s->chars[index];
}

// write character, no bounds checking
void str_write(str s, char c, usize i) {
    s->chars[i] = c;
}

// remove n characters starting at index i
void str_remove(str s, usize i, usize n) {
    usize end = i + n;
    if (end >= s->len) {
        end = s->len - 1;
    }
    usize new_len = s->len - n;
    s->chars = (char*)realloc(s->chars, (new_len + 1) * sizeof(char));
    memmove(s->chars + i, s->chars + end + 1, s->len - end);
    s->len = new_len;
}

// str("my contents")\n
void str_print(str s) {
    printf("str(\"%s\")\n", s->chars);
}

// sprintf into a string
str str_format(c_str fmt, ...) {
    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    isize size = vsnprintf(null, 0, fmt, args_copy);
    va_end(args_copy);
    if (size < 0) {
        va_end(args);
        return str_new();
    }
    str result = str_alloc(size);
    vsnprintf(result->chars, size + 1, fmt, args);
    va_end(args);
    return result;
}

// sprintf into a c-string
c_str str_format_c(c_str fmt, ...) {
    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);
    isize size = vsnprintf(null, 0, fmt, args_copy);
    va_end(args_copy);
    if (size < 0) {
        va_end(args);
        return null;
    }
    c_str result = (char*) malloc(size + 1);
    vsnprintf(result, size + 1, fmt, args);
    va_end(args);
    return result;
}


// GENERIC DEFINITIONS


usize __sls(str s) {
    return s->len;
}

usize __slc(c_str s) {
    return strlen(s);
}

bool __seqss(str a, str b) {
    if (a->len != b->len) {
        return false;
    }
    return !strcmp(a->chars, b->chars);
}

bool __seqsc(str s, c_str cs) {
    return !strcmp(s->chars, cs);
}

bool __seqcs(c_str cs, str s) {
    return !strcmp(s->chars, cs);
}

bool __seqcc(c_str a, c_str b) {
    return !strcmp(a, b);
}

usize __shs(str s) {
    return __shc(s->chars);
}

usize __shc(c_str cs) {
    usize hash = 5381;
    int c;
    while ((c = *cs++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool __ssws(str big, str small) {
    return big->len >= small->len && !strncmp(big->chars, small->chars, small->len);
}

bool __sswc(str big, c_str small) {
    usize small_len = strlen(small);
    return big->len >= small_len && !strncmp(big->chars, small, small_len);
}

bool __sswch(str s, char c) {
    return s->len != 0 && s->chars[0] == c;
}

bool __sews(str big, str small) {
    return big->len >= small->len
        && !strncmp(big->chars + big->len - small->len, small->chars, small->len);
}

bool __sewc(str big, c_str small) {
    usize small_len = strlen(small);
    return !strncmp(big->chars + big->len - small_len, small, small_len);
}

bool __sewch(str s, char c) {
    return s->len != 0 && s->chars[s->len - 1] == c;
}

bool __shass(str big, str small) {
    return strstr(big->chars, small->chars) != null;
}

bool __shasc(str big, c_str small) {
    return strstr(big->chars, small) != null;
}

bool __shasch(str s, char c) {
    return s->len != 0 && strchr(s->chars, c) != null;
}

isize __sfs(str big, str small) {
    if (big->len < small->len) {
        return -1;
    }
    char* result = strstr(big->chars, small->chars);
    if (result == null) {
        return -1;
    }
    return result - big->chars;
}

isize __sfc(str big, c_str small) {
    usize small_len = strlen(small);
    if (big->len < small_len) {
        return -1;
    }
    char* result = strstr(big->chars, small);
    if (result == null) {
        return -1;
    }
    return result - big->chars;
}

isize __sfch(str s, char c) {
    if (s->len == 0) {
        return -1;
    }
    char* result = strchr(s->chars, c);
    if (result == null) {
        return -1;
    }
    return result - s->chars;
}

usize __scnts(str big, str small) {
    usize count = 0;
    char* big_ptr = big->chars;
    while ((big_ptr = strstr(big_ptr, small->chars)) != null) {
        count++;
        big_ptr += small->len;
    }
    return count;
}

usize __snctc(str big, c_str small) {
    usize count = 0;
    usize small_len = strlen(small);
    char* big_ptr = big->chars;
    while ((big_ptr = strstr(big_ptr, small)) != null) {
        count++;
        big_ptr += small_len;
    }
    return count;
}

usize __scntch(str s, char c) {
    usize count = 0;
    for (usize i = 0; i < s->len; i++) {
        if (s->chars[i] == c) {
            count++;
        }
    }
    return count;
}

str __ssls(str s, isize start, isize stop) {
    if (start < 0) {
        start += s->len;
    }
    if (stop < 0) {
        stop += s->len;
    }
    isize len = stop - start;
    if (len < 0) {
        len = 0;
    }
    str s_new = str_alloc(len);
    memcpy(s_new->chars, s->chars + start, len);
    return s_new;
}

str __sslc(c_str cs, isize start, isize stop) {
    usize s_len = strlen(cs);
    if (start < 0) {
        start += s_len;
    }
    if (stop < 0) {
        stop += s_len;
    }
    isize len = stop - start;
    if (len < 0) {
        len = 0;
    }
    str s_new = str_alloc(len);
    memcpy(s_new->chars, cs + start, len);
    return s_new;
}

void __saps(str a, str b) {
    a->chars = (char*) realloc(a->chars, (a->len + b->len + 1) * sizeof(char));
    memcpy(a->chars + a->len, b->chars, b->len + 1);
    a->len += b->len;
}

void __sapc(str s, c_str cs) {
    usize cs_len = strlen(cs);
    s->chars = (char*) realloc(s->chars, (s->len + cs_len + 1) * sizeof(char));
    memcpy(s->chars + s->len, cs, cs_len + 1);
    s->len += cs_len;
}

void __sapch(str s, char c) {
    s->chars = (char*) realloc(s->chars, (s->len + 2) * sizeof(char));
    s->chars[s->len] = c;
    s->chars[s->len + 1] = '\0';
    s->len++;
}

void __sprs(str a, str b) {
    usize new_len = a->len + b->len;
    a->chars = (char*) realloc(a->chars, (new_len + 1) * sizeof(char));
    memmove(a->chars + b->len, a->chars, a->len + 1);
    memcpy(a->chars, b->chars, b->len);
    a->len = new_len;
}

void __sprc(str s, c_str cs) {
    usize cs_len = strlen(cs);
    s->chars = (char*) realloc(s->chars, (s->len + cs_len + 1) * sizeof(char));
    memmove(s->chars + cs_len, s->chars, s->len + 1);
    memcpy(s->chars, cs, cs_len);
    s->len += cs_len;
}

void __sprch(str s, char c) {
    s->chars = (char*) realloc(s->chars, (s->len + 2) * sizeof(char));
    memmove(s->chars + 1, s->chars, s->len + 1);
    s->chars[0] = c;
    s->len++;
}

void __sinss(str big, str small, usize i) {
    if (i > big->len) {
        big->chars = (char*) realloc(big->chars, (big->len + small->len + 1) * sizeof(char));
        strcat(big->chars, small->chars);
        big->len = big->len + small->len;
    } else {
        big->chars = (char*) realloc(big->chars, (big->len + small->len + 1) * sizeof(char));
        memmove(big->chars + i + small->len, big->chars + i, big->len - i + 1);
        memcpy(big->chars + i, small->chars, small->len);
        big->len = big->len + small->len;
    }
}

void __sinsc(str s, c_str cs, usize i) {
    usize cs_len = strlen(cs);
    s->chars = (char*) realloc(s->chars, (s->len + cs_len + 1) * sizeof(char));
    if (i > s->len) {
        strcat(s->chars, cs);
    } else {
        memmove(s->chars + i + cs_len, s->chars + i, s->len - i + 1);
        memcpy(s->chars + i, cs, cs_len);
    }
    s->len += cs_len;
}

void __sinsch(str s, char c, usize i) {
    s->chars = (char*) realloc(s->chars, (s->len + 2) * sizeof(char));
    if (i > s->len) {
        s->chars[s->len] = c;
        s->chars[s->len + 1] = '\0';
    } else {
        memmove(s->chars + i + 1, s->chars + i, s->len - i + 1);
        s->chars[i] = c;
    }
    s->len++;
}

array_str __ssplss(str s, str delim) {
    if (delim->len == 0) {
        array_str a;
        array_init(a, delim->len);
        for (usize i = 0; i < delim->len; i++) {
            str s2 = str_new();
            str_append(s2, str_at(s, i));
            array_write(a, i, s2);
        }
        return a;
    }

    str s_copy = str_copy(s);
    c_str cs = s_copy->chars;
    char **result = null;
    char *token = null;
    usize count = 0;
    token = strstr(cs, delim->chars);
    while (token != null) {
        *token = '\0';
        result = (char**) realloc(result, sizeof(char *) * (count + 1));
        result[count++] = cs;
        cs = token + delim->len;
        token = strstr(cs, delim->chars);
    }
    result = (char**) realloc(result, sizeof(char *) * (count + 2));
    result[count++] = cs;
    result[count] = null;
    array_str a;
    array_init(a, count);
    for(usize i = 0; i < count; i++) {
        array_write(a, i, str_from(result[i]));
    }
    str_free(s_copy);
    return a;
}

array_str __ssplsc(str s, c_str delim) {
    usize delim_len = strlen(delim);
    if (delim_len == 0) {
        array_str a;
        array_init(a, delim_len);
        for (usize i = 0; i < delim_len; i++) {
            array_write(a, i, str_alloc(1));
            str_write(a->data[i], str_at(s, i), i);
        }
        return a;
    }

    str s_copy = str_copy(s);
    c_str cs = s_copy->chars;
    char **result = null;
    char *token = null;
    usize count = 0;
    token = strstr(cs, delim);
    while (token != null) {
        *token = '\0';
        result = (char**) realloc(result, sizeof(char *) * (count + 1));
        result[count++] = cs;
        cs = token + delim_len;
        token = strstr(cs, delim);
    }
    result = (char**) realloc(result, sizeof(char *) * (count + 2));
    result[count++] = cs;
    result[count] = null;
    array_str a;
    array_init(a, count);
    for(usize i = 0; i < count; i++) {
        array_write(a, i, str_from(result[i]));
    }
    str_free(s_copy);
    return a;
}

array_str __ssplsch(str s, char delim) {
    char delim_cs[2] = { delim, '\0' };
    return __ssplsc(s, delim_cs);
}

array_str __ssplcs(c_str cs, str delim) {
    if (delim->len == 0) {
    array_str a;
    array_init(a, delim->len);
        for (usize i = 0; i < delim->len; i++) {
            array_write(a, i, str_alloc(1));
            str_write(a->data[i], cs[i], i);
        }
        return a;
    }

    str s = str_from(cs);
    c_str cs_copy = s->chars;
    char **result = null;
    char *token = null;
    usize count = 0;
    token = strstr(cs_copy, delim->chars);
    while (token != null) {
        *token = '\0';
        result = (char**) realloc(result, sizeof(char *) * (count + 1));
        result[count++] = cs_copy;
        cs_copy = token + delim->len;
        token = strstr(cs_copy, delim->chars);
    }
    result = (char**) realloc(result, sizeof(char *) * (count + 2));
    result[count++] = cs_copy;
    result[count] = null;
    array_str a;
    array_init(a, count);
    for(usize i = 0; i < count; i++) {
        array_write(a, i, str_from(result[i]));
    }
    str_free(s);
    return a;
}

array_str __ssplcc(c_str cs, c_str delim) {
    usize delim_len = strlen(delim);
    if (delim_len == 0) {
    array_str a;
    array_init(a, delim_len);
        for (usize i = 0; i < delim_len; i++) {
            array_write(a, i, str_alloc(1));
            str_write(a->data[i], cs[i], i);
        }
        return a;
    }

    str s = str_from(cs);
    c_str cs_copy = s->chars;
    char **result = null;
    char *token = null;
    usize count = 0;
    token = strstr(cs_copy, delim);
    while (token != null) {
        *token = '\0';
        result = (char**) realloc(result, sizeof(char *) * (count + 1));
        result[count++] = cs_copy;
        cs_copy = token + delim_len;
        token = strstr(cs_copy, delim);
    }
    result = (char**) realloc(result, sizeof(char *) * (count + 2));
    result[count++] = cs;
    result[count] = null;
    array_str a;
    array_init(a, count);
    for(usize i = 0; i < count; i++) {
        array_write(a, i, str_from(result[i]));
    }
    str_free(s);
    return a;
}

array_str __ssplcch(c_str cs, char delim) {
    char delim_cs[2] = { delim, '\0' };
    return __ssplcc(cs, delim_cs);
}

array_str __sprtss(str s, str delim) {
    array_str a;
    array_init(a, 3);

    char* token = strstr(s->chars, delim->chars);
    if (token == NULL) {
        a->data[0] = str_copy(s);
        a->data[1] = str_new();
        a->data[2] = str_new();
        return a;
    }
    
    isize start_index = token - s->chars;
    isize end_index = start_index + delim->len;
    
    a->data[0] = __ssls(s, 0, start_index);
    a->data[1] = __ssls(s, start_index, end_index);
    a->data[2] = __ssls(s, end_index, s->len);

    return a;
}

array_str __sprtsc(str s, c_str delim) {
    array_str a;
    array_init(a, 3);

    char* token = strstr(s->chars, delim);
    if (token == NULL) {
        a->data[0] = str_copy(s);
        a->data[1] = str_new();
        a->data[2] = str_new();
        return a;
    }
    
    isize start_index = token - s->chars;
    isize end_index = start_index + strlen(delim);
    
    a->data[0] = __ssls(s, 0, start_index);
    a->data[1] = __ssls(s, start_index, end_index);
    a->data[2] = __ssls(s, end_index, s->len);

    return a;
}

array_str __sprtsch(str s, char delim) {
    char delim_cs[2] = { delim, '\0' };
    return __sprtsc(s, delim_cs);
}

array_str __sprtcs(c_str cs, str delim) {
    usize cs_len = strlen(cs);
    array_str a;
    array_init(a, 3);

    char* token = strstr(cs, delim->chars);
    if (token == NULL) {
        a->data[0] = str_from(cs);
        a->data[1] = str_new();
        a->data[2] = str_new();
        return a;
    }
    
    isize start_index = token - cs;
    isize end_index = start_index + delim->len;
    
    a->data[0] = __sslc(cs, 0, start_index);
    a->data[1] = __sslc(cs, start_index, end_index);
    a->data[2] = __sslc(cs, end_index, cs_len);

    return a;
}

array_str __sprtcc(c_str cs, c_str delim) {
    usize cs_len = strlen(cs);
    usize delim_len = strlen(delim);
    array_str a;
    array_init(a, 3);

    char* token = strstr(cs, delim);
    if (token == NULL) {
        a->data[0] = str_from(cs);
        a->data[1] = str_new();
        a->data[2] = str_new();
        return a;
    }
    
    isize start_index = token - cs;
    isize end_index = start_index + delim_len;

    a->data[0] = __sslc(cs, 0, start_index);
    a->data[1] = __sslc(cs, start_index, end_index);
    a->data[2] = __sslc(cs, end_index, cs_len);

    return a;
}

array_str __sprtcch(c_str cs, char delim) {
    char delim_cs[2] = { delim, '\0' };
    return __sprtcc(cs, delim_cs);
}


str __sjs(array_str a, str delim) {
    str s = str_new();
    if (a->len == 0) {
        return s;
    }
    for (usize i = 0; i < a->len-1; i++) {
        __saps(s, a->data[i]);
        __saps(s, delim);
    }
    __saps(s, a->data[a->len-1]);
    return s;
}

str __sjc(array_str a, c_str delim) {
    str s = str_new();
    if (a->len == 0) {
        return s;
    }
    for (usize i = 0; i < a->len-1; i++) {
        __saps(s, a->data[i]);
        __sapc(s, delim);
    }
    __saps(s, a->data[a->len-1]);
    return s;
}

str __sjch(array_str a, char delim) {
    str s = str_new();
    if (a->len == 0) {
        return s;
    }
    for (usize i = 0; i < a->len-1; i++) {
        __saps(s, a->data[i]);
        __sapch(s, delim);
    }
    __saps(s, a->data[a->len-1]);
    return s;
}

str __sups(str s) {
    str s2 = str_copy(s);
    for (usize i = 0; i < s2->len; i++) {
        s2->chars[i] = toupper(s->chars[i]);
    }
    return s2;
}

str __supc(c_str cs) {
    str s = str_from(cs);
    for (usize i = 0; i < s->len; i++) {
        s->chars[i] = toupper(cs[i]);
    }
    return s;
}

str __slos(str s) {
    str s2 = str_copy(s);
    for (usize i = 0; i < s->len; i++) {
        s2->chars[i] = tolower(s->chars[i]);
    }
    return s2;
}

str __sloc(c_str cs) {
    str s = str_from(cs);
    for (usize i = 0; i < s->len; i++) {
        s->chars[i] = tolower(cs[i]);
    }
    return s;
}

str __sres(str s) {
    str r = str_copy(s);
    for (usize i = 0, j = r->len-1; i < j; i++, j--) {
        char temp = r->chars[i];
        r->chars[i] = r->chars[j];
        r->chars[j] = temp;
    }
    return r;
}

str __srec(c_str cs) {
    str r = str_from(cs);
    for (usize i = 0, j = r->len-1; i < j; i++, j--) {
        char temp = r->chars[i];
        r->chars[i] = r->chars[j];
        r->chars[j] = temp;
    }
    return r;
}

str __sreps(str s, usize n) {
    str r = str_alloc(s->len * n);
    for (usize i = 0; i < n; i++) {
        memcpy(r->chars + (s->len * i), s->chars, s->len);
    }
    return r;
}

str __srepc(c_str cs, usize n) {
    usize len = strlen(cs);
    str r = str_alloc(len * n);
    for (usize i = 0; i < n; i++) {
        memcpy(r->chars + (len * i), cs, len);
    }
    return r;
}

str __srepch(char c, usize n) {
    str r = str_alloc(n);
    memset(r->chars, (int)c, n);
    return r;
}

#endif // STD_STR_H