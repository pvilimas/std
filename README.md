## std.h
- small header-only standard library of pure C data types I use for my own stuff, includes:
    - array.h - generic fixed-length array
    - vec.h - generic dynamic array kinda similar to std::vector
    - map.h - generic hashmap with string keys
    - str.h - string library
    - types.h - some type aliases I like to use

- The generic classes define basic primitive types
- You can either `#include "std.h"` or specific file like `#include "std/str.h"`
- See examples for how to use
- Does not use any compiler extensions
- All examples and tests written by ChatGPT
- Planning on adding more stuff at some point when I need it