#include <stdio.h>
#include <stdlib.h>
#include "../std/str.h"

int main() {
    // Create a string
    str s1 = str_from("apple,banana,orange");

    // Split the string into a list of strings
    array_str parts = str_split(s1, ',');

    // Repeat each string in the list 3 times
    for (usize i = 0; i < array_len(parts); i++) {
        str_repeat(str_from("b"), 3);
        str_join(parts, 'a');
    }
    printf("%d\n", str_ends_with(s1, "ge"));

    // Join the list of strings into a single string with a newline delimiter
    str s2 = str_join(parts, "\n");

    // Print the original string and the modified string
    printf("Original string: ");
    str_print(s1);
    printf("Modified string: ");
    str_print(s2);

    // Check if the original string is equal to the modified string
    if (str_equals(s1, s2)) {
        printf("The original string and the modified string are equal.\n");
    } else {
        printf("The original string and the modified string are not equal.\n");
    }

    // Format a string with the original and modified strings
    c_str fmt = "The original string is \"%s\" and the modified string is \"%s\".\n";
    c_str output = str_format_c(fmt, str_chars(s1), str_chars(s2));
    printf("%s", output);

    // Free the memory allocated for the strings and list of strings
    str_free(s1);
    str_free(s2);
    array_deinit(parts);

    return 0;
}
