#include <stdio.h>
#include "../std/array.h"

typedef struct {
    int x;
    int y;
} Point;

typedef array(Point) array_point;

int main() {
    array_point points;
    array_init(points, 3);
    for (int i = 0; i < array_len(points); i++) {
        Point p = {i, i * 2};
        array_at(points, i) = p;
    }

    printf("Original array:\n");
    Point p;
    array_iter(points, p) {
        printf("(%d, %d)\n", p.x, p.y);
    }

    array_swap(points, 0, 2);

    printf("\nSwapped array:\n");
    array_iter(points, p) {
        printf("(%d, %d)\n", p.x, p.y);
    }

    array_free(points);

    return 0;
}
