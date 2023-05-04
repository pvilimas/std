#include <stdio.h>
#include <assert.h>
#include "../std/str.h"

void test_str_new() {
    str s = str_new();
    assert(s->len == 0);
    assert(s->chars != NULL);
    str_free(s);
}

void test_str_alloc() {
    str s = str_alloc(10);
    assert(s->len == 10);
    assert(s->chars != NULL);
    for (usize i = 0; i < s->len; i++) {
        assert(s->chars[i] == '\0');
    }
    str_free(s);
}

void test_str_from() {
    c_str cs = "hello world";
    str s = str_from(cs);
    assert(s->len == strlen(cs));
    assert(str_equals(s, cs));
    str_free(s);
}

void test_str_copy() {
    str s1 = str_from("hello world");
    str s2 = str_copy(s1);
    assert(s1->len == s2->len);
    assert(str_equals(s1, s2));
    str_free(s1);
    str_free(s2);
}

void test_str_chars() {
    str s = str_from("hello world");
    c_str cs = str_chars(s);
    assert(str_equals(s, cs));
    str_free(s);
}

void test_str_at() {
    str s = str_from("hello world");
    assert(str_at(s, 0) == 'h');
    assert(str_at(s, 6) == 'w');
    str_free(s);
}

void test_str_write() {
    str s = str_from("hello world");
    str_write(s, '!', 11);
    assert(str_equals(s, "hello world!"));
    str_free(s);
}

void test_str_remove() {
    str s = str_from("hello world");
    str_remove(s, 6, 5);
    assert(str_equals(s, "hello "));
    str_free(s);
}

void test_str_format() {
    str s = str_format("pi = %.2f", 3.14159);
    assert(str_equals(s, "pi = 3.14"));
    str_free(s);
}

void test_str_format_c() {
    c_str cs = str_format_c("pi = %.2f", 3.14159);
    assert(strcmp(cs, "pi = 3.14") == 0);
    free(cs);
}

void test_str_len() {
    str s = str_from("hello world");
    assert(str_len(s) == 11);
    str_free(s);
}

void test_str_equals() {
    str s1 = str_from("hello world");
    str s2 = str_from("hello world");
    str s3 = str_from("goodbye world");
    assert(str_equals(s1, s2));
    assert(!str_equals(s1, s3));
    str_free(s1);
    str_free(s2);
    str_free(s3);
}

void test_str_hash() {
    str s = str_from("hello world");
    assert(str_hash(s) == str_hash("hello world"));
    assert(str_hash(s) != str_hash("goodbye world"));
    str_free(s);
}

void test_str_starts_with() {
    str s = str_from("hello world");
    assert(str_starts_with(s, "hello"));
    assert(!str_starts_with(s, "world"));
    str_free(s);
}

void test_str_ends_with() {
    str s = str_from("hello world");
    assert(str_ends_with(s, "world"));
    assert(!str_ends_with(s, "hello"));
    str_free(s);
}

void test_str_has() {
    str s = str_from("hello world");
    bool has_e = str_has(s, 'e');
    bool has_z = str_has(s, 'z');
    if (has_e != true || has_z != false) {
        printf("str_has() failed\n");
    }
    str_free(s);
}

void test_str_find() {
    str s = str_from("hello world");
    isize index_l = str_find(s, 'l');
    isize index_z = str_find(s, 'z');
    if (index_l != 2 || index_z != -1) {
        printf("str_find() failed\n");
    }
    str_free(s);
}

void test_str_count() {
    str s = str_from("hello world");
    usize count_l = str_count(s, 'l');
    usize count_z = str_count(s, 'z');
    if (count_l != 3 || count_z != 0) {
        printf("str_count() failed\n");
    }
    str_free(s);
}

void test_str_slice() {
    str s = str_from("hello world");
    str slice = str_slice(s, 2, 6);
    if (str_equals(slice, "llo ") == false) {
        printf("str_slice() failed\n");
    }
    str_free(s);
    str_free(slice);
}

void test_str_append() {
    str s = str_from("hello");
    str_append(s, ' ');
    str_append(s, 'w');
    str_append(s, 'o');
    str_append(s, 'r');
    str_append(s, 'l');
    str_append(s, 'd');
    if (str_equals(s, "hello world") == false) {
        printf("str_append() failed\n");
    }
    str_free(s);
}

void test_str_prepend() {
    str s = str_from("world");
    str_prepend(s, ' ');
    str_prepend(s, 'h');
    str_prepend(s, 'e');
    str_prepend(s, 'l');
    str_prepend(s, 'l');
    str_prepend(s, 'o');
    if (str_equals(s, "olleh world") == false) {
        printf("str_prepend() failed\n");
    }
    str_free(s);
}

void test_str_insert() {
    str s = str_from("hello");
    str_insert(s, ' ', 2);
    str_insert(s, 'w', 3);
    str_insert(s, 'o', 4);
    str_insert(s, 'r', 5);
    str_insert(s, 'l', 6);
    str_insert(s, 'd', 7);
    if (str_equals(s, "he worldllo") == false) {
        printf("str_insert() failed\n");
    }
    str_free(s);
}

void test_str_split() {
    str s = str_from("hello,world");
    str_list split = str_split(s, ',');
    if (split.count != 2 || str_equals(split.strings[0], "hello") == false ||
        str_equals(split.strings[1], "world") == false) {
        printf("str_split() failed\n");
    }
    str_free(s);
    for (usize i = 0; i < split.count; i++) {
        str_free(split.strings[i]);
    }
}

void test_str_partition() {
    printf("Testing str_partition...\n");

    str s = str_from("this-is-a-string");
    str_list parts = str_partition(s, "-");

    assert(parts.strings[0]->len == 4);
    assert(parts.strings[1]->len == 1);
    assert(parts.strings[2]->len == 11);

    assert(str_equals(parts.strings[0], "this"));
    assert(str_equals(parts.strings[1], "-"));
    assert(str_equals(parts.strings[2], "is-a-string"));

    // clean up
    for (usize i = 0; i < 3; i++) {
        str_free(parts.strings[i]);
    }
    str_free(s);
}

void test_str_join() {
    printf("Testing str_join...\n");

    str_list l = {0};
    l.strings = malloc(sizeof(str) * 4);
    l.count = 4;

    l.strings[0] = str_from("this");
    l.strings[1] = str_from("is");
    l.strings[2] = str_from("a");
    l.strings[3] = str_from("string");

    str s = str_join(l, "");

    assert(s->len == 13);
    assert(str_equals(s, "thisisastring"));

    // clean up
    str_free(s);
    for (usize i = 0; i < l.count; i++) {
        str_free(l.strings[i]);
    }
    free(l.strings);
}

int main() {
    test_str_new();
    test_str_alloc();
    test_str_from();
    test_str_copy();
    test_str_chars();
    test_str_at();
    test_str_write();
    test_str_remove();
    test_str_format();
    test_str_format_c();
    test_str_len();
    test_str_equals();
    test_str_hash();
    test_str_starts_with();
    test_str_ends_with();
    test_str_has();
    test_str_find();
    test_str_count();
    test_str_slice();
    test_str_append();
    test_str_prepend();
    test_str_insert();
    test_str_split();
    test_str_partition();
    test_str_join();
    return 0;
}