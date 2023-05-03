#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../std/map.h"

typedef struct {
    int x;
    int y;
} Point;

void test_point_map() {
    map(Point) m;
    map_init(m);

    // Test basic insert and get
    Point p1 = {1, 2};
    Point p2 = {3, 4};
    Point p3 = {5, 6};
    map_insert(m, "point1", p1);
    map_insert(m, "point2", p2);
    map_insert(m, "point3", p3);
    Point result = *(Point*) map_get(m, "point2");
    if (result.x != p2.x || result.y != p2.y) {
        printf("Error: Failed basic insert and get test for point map\n");
    }

    // Test overwrite
    Point p4 = {7, 8};
    map_insert(m, "point2", p4);
    result = *(Point*) map_get(m, "point2");
    if (result.x != p4.x || result.y != p4.y) {
        printf("Error: Failed overwrite test for point map\n");
    }

    // Test remove
    map_remove(m, "point1");
    if (map_contains(m, "point1")) {
        printf("Error: Failed remove test for point map\n");
    }

    // Test size and is_empty
    if (map_size(m) != 2 || map_is_empty(m)) {
        printf("Error: Failed size and is_empty test for point map\n");
    }

    // Test clear
    map_clear(m);
    if (map_size(m) != 0 || !map_is_empty(m)) {
        printf("Error: Failed clear test for point map\n");
    }

    // Test update
    Point p5 = {9, 10};
    map_insert(m, "point2", p2);
    map_insert(m, "point3", p3);
    map_insert(m, "point3", p5);
    map_insert(m, "point4", p4);
    if (map_size(m) != 4 || !map_contains(m, "point4")) {
        printf("Error: Failed update test for point map\n");
    }

    // Test iteration
    // char* k; Point v;
    // map_iter(m, k, v) {
    //     printf("%s: (%d, %d)\n", k, v.x, v.y);
    // }

    map_deinit(m);
}

int main() {
    // test_point_map();
    return 0;
}
