#ifndef STD_H
#define STD_H

/*

std.h - small header-only standard library of pure C data types I use for my own stuff, includes:
    - array.h - generic fixed-length array
    - vec.h - generic dynamic array kinda similar to std::vector
    - map.h - generic hashmap with string keys
    - str.h - string library
    - types.h - some type aliases I like to use

- The generic classes define basic primitive types
- You can either `#include "std.h"` or specific file like `#include "std/str.h"`
- See examples for how to use
- All examples and tests written by ChatGPT
- Planning on adding more stuff at some point when I need it

*/

#include "std/types.h"

#include "std/array.h"
#include "std/map.h"
#include "std/str.h"
#include "std/vec.h"

#endif // STD_H