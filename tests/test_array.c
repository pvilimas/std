#include <stdio.h>
#include "../std/array.h"

typedef struct {
    int x;
    int y;
} point;

int main() {
    // create an array of points
    array(point) points;
    array_init(points, 5);

    // fill the array with some values
    for (int i = 0; i < array_len(points); i++) {
        array_at(points, i).x = i;
        array_at(points, i).y = i * i;
    }

    // test array_at function
    point p1 = array_at(points, 2);
    printf("array_at: (%d, %d)\n", p1.x, p1.y);

    // test array_first function
    point first = array_first(points);
    printf("array_first: (%d, %d)\n", first.x, first.y);

    // test array_last function
    point last = array_last(points);
    printf("array_last: (%d, %d)\n", last.x, last.y);

    // Initialize the array with 5 points
    array_init(points, 5);

    // Set the points to some initial values
    for (int i = 0; i < array_len(points); i++) {
        array_at(points, i).x = i;
        array_at(points, i).y = i * i;
    }

    // Print the array before swapping
    printf("Before swapping:\n");
    for (int i = 0; i < array_len(points); i++) {
        printf("(%d, %d) ", array_at(points, i).x, array_at(points, i).y);
    }
    printf("\n");

    // Swap the first and fifth points
    array_swap(points, 0, 4);

    // Print the array after swapping
    printf("After swapping:\n");
    for (int i = 0; i < array_len(points); i++) {
        printf("(%d, %d) ", array_at(points, i).x, array_at(points, i).y);
    }
    printf("\n");

    // Deinitialize the array
    array_deinit(points);

    return 0;
}

