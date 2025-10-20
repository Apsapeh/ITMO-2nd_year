#include <stdlib.h>

typedef struct {
  int x;
  int y;
} Point;

typedef enum {
    Circle,
    Square,
    Triangle
} ShapeType;

typedef struct {
    Point p;
    char* name;
    ShapeType type;
} Shape;

typedef struct {
    Shape* shapes;
    int size;
} Container;

Container* create_container();

int add_new_shape(Container*, char*, Point, ShapeType);
int remove_shape_by_index(Container*, size_t);
void print(Container*);