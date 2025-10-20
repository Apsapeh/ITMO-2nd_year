#include "shape.h"
#include <stdio.h>

#define ERROR(str) do {error(__LINE__, __FILE__, str);} while (0)

void error(int line, const char* file, const char* str);




int main() {
    Container* container = create_container();

    if (!container)
        return 10;

    printf("Add 1\n");
    add_new_shape(container, "1", (Point){0, 0}, Circle);
    print(container);
    printf("\n\n");

    printf("Add 2\n");
    add_new_shape(container, "2", (Point){0, 0}, Triangle);
    add_new_shape(container, "3", (Point){0, 0}, Circle);
    add_new_shape(container, "4", (Point){0, 0}, Circle);
    print(container);
    printf("\n\n");

    printf("Remove 1\n");
    remove_shape_by_index(container, 0);
    print(container);
    printf("\n\n");

    printf("Remove 2\n");
    remove_shape_by_index(container, 2);
    print(container);
    printf("\n\n");

    printf("Remove 3\n");
    remove_shape_by_index(container, 0);
    remove_shape_by_index(container, 0);
    print(container);
    printf("\n\n");

    printf("Remove 4\n");
    printf("Status: %d\n", remove_shape_by_index(container, 0));
    print(container);
    printf("\n\n");

    printf("Add 3\n");
    add_new_shape(container, "1", (Point){0, 0}, Circle);
    print(container);
    printf("\n\n");

    free(container->shapes);
    free(container);
}


