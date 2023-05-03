#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../std/map.h"

typedef struct {
    int id;
    char name[50];
} person;

typedef map(person) map_person;

void print_person(person *p) {
    printf("Person %d: %s\n", p->id, p->name);
}

int main() {
    // Initialize the map
    map_person m;
    map_init(m);

    // Insert some values into the map
    person p1 = {1, "John"};
    map_insert(m, "key1", p1);

    person p2 = {2, "Mary"};
    map_insert(m, "key2", p2);

    person p3 = {3, "Bob"};
    map_insert(m, "key3", p3);

    // Get a value from the map
    person *p = map_get(m, "key2");
    printf("Value for key 'key2': ");
    print_person(p);

    // Remove a value from the map
    map_remove(m, "key2");

    printf("%d\n", m->size);

    // Iterate over the map
    array_cstr keys = map_keys(m);
    for (int i = 0; i < keys->len; i++) {
        char* k = keys->data[i];
        person v = *(person*) map_get(m, keys->data[i]);
        printf("\"%s\" -> %d\n", k, v.id);
    }

    // Free memory and exit
    array_deinit(keys);
    map_deinit(m);
    return 0;
}
