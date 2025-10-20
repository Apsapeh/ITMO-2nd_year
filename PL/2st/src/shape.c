#include "shape.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Container* create_container(){
    Container* ct = malloc(sizeof(Container));

    if (ct) {
        ct->size = 0;
        ct->shapes = NULL;
    }

    return ct;
}

int add_new_shape(Container* this, char* name, Point point, ShapeType type) {
    this->shapes = realloc(this->shapes, sizeof(Shape) * ++this->size);
    this->shapes[this->size-1] = (Shape){point, name, type};
    return this->shapes ? 1 : 0;
}

int remove_shape_by_index(Container* this, size_t idx) {
    if (idx >= this->size)
        return 0;

    if (this->size == 1) {
        free(this->shapes);
        this->shapes = NULL;
        this->size = 0;
        return 1;
    }

    if (idx == 0) {
        Shape* new_ptr = malloc(this->size-- * sizeof(Shape)); 
        if (!new_ptr)
            return 0;

        memcpy(new_ptr, this->shapes + 1 , this->size * sizeof(Shape));
        free(this->shapes);
        this->shapes = new_ptr;
    } else if (idx == 0) {
        Shape* new_ptr = malloc(this->size-- * sizeof(Shape)); 
        if (!new_ptr)
            return 0;

        memcpy(new_ptr, this->shapes , this->size * sizeof(Shape));
        free(this->shapes);
        this->shapes = new_ptr;
    } else {
        Shape* new_ptr = malloc(this->size-- * sizeof(Shape)); 
        if (!new_ptr)
            return 0;

        memcpy(new_ptr, this->shapes , idx * sizeof(Shape));
        memcpy(new_ptr + idx + 1, this->shapes , (this->size - idx) * sizeof(Shape));
        free(this->shapes);
        this->shapes = new_ptr;
    }

    return 1;
}

void print(Container* this) {
    for (size_t i = 0; i < this->size; ++i) {
        Shape* shape = this->shapes + i;

        const char* type;
        switch (shape->type) {
            case Circle:
                type = "Circle";
                break;
            case Square:
                type = "Square";
                break;
            case Triangle:
                type = "Triangle";
                break;
            default:
                continue;
        }

        printf(
            "[%lu]\n\tPoint: (%d, %d)\n\tName: %s\n\tType: %s\n\n",
            i,
            shape->p.x,
            shape->p.y,
            shape->name,
            type
        );
    }
}

