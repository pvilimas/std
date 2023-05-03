#ifndef STD_TYPES_H
#define STD_TYPES_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#define null NULL

typedef int8_t       i8;
typedef int16_t      i16;
typedef int32_t      i32;
typedef int64_t      i64;

typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;

typedef ssize_t      isize;
typedef size_t       usize;

typedef float        f32;
typedef double       f64;

#endif // STD_TYPES_H