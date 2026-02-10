#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Cell {
    char name;
    uint8_t value;
    uint8_t neighbors_count;
    struct Cell* third_level;
    struct Cell* third_level_bridge[2];
    struct Cell* neighbors[2];
};

#define INIT_CELL_M(_name, _nbc, _nb1, _nb2, _th, _thb_1, _thb_2)                                       \
    (struct Cell) {                                                                                     \
        .name = _name, .value = 0, .neighbors_count = _nbc,                                             \
        .neighbors = {&g_cells[_nb1], &g_cells[_nb2]}, .third_level = &g_cells[_th],                    \
        .third_level_bridge = {                                                                         \
            &g_cells[_thb_1],                                                                           \
            &g_cells[_thb_2]                                                                            \
        }                                                                                               \
    }

enum { A, B, C, D, E, F, G, H, CELLS_COUNT };

// clang-format off
static struct Cell g_cells[] = {
//                        name   neighbors_count   neighbors   3rd-level   path-to-3rd-level
        INIT_CELL_M(  'A',         2,            B, E,         G,            B, F       ),
        INIT_CELL_M(  'B',         2,            C, F,         H,            C, G       ),
        INIT_CELL_M(  'C',         2,            D, G,         E,            D, H       ),
        INIT_CELL_M(  'D',         2,            A, H,         F,            A, E       ),
        INIT_CELL_M(  'E',         2,            F, H,         C,            H, D       ),
        INIT_CELL_M(  'F',         1,            G, 0,         D,            E, A       ),
        INIT_CELL_M(  'G',         1,            H, 0,         A,            F, B       ),
        INIT_CELL_M(  'H',         0,            0, 0,         B,            G, C       ),
};
// clang-format on

static char out_buffer[4 * 1000 + 1] = {0};
static char* buffer_cursor = out_buffer;


static inline void print_to_buffer(char cell_1, char cell_2, char op) {
    buffer_cursor[0] = cell_1;
    buffer_cursor[1] = cell_2;
    buffer_cursor[2] = op;
    buffer_cursor[3] = '\n';
    buffer_cursor += 4;
}


static inline void impossible(void) {
    printf("IMPOSSIBLE\n");
    exit(0);
}


// Разрешены могут быть только дуоны первого и третьего уровне
//
// Дуоны первого уровня, это соседние клетки
//
// Дуоны третьего уровня это сосед соседей соседей.
//     А -> B -> C -> D, где A - текущая клетка, D - клетка третьего уровня
//     Т.к. куб, то у каждой клетки есть только одна соседняя клетка третьего уровня
//
// Массив клеток оптимизирован и не содержит дубликатов. Т.е. если мы проверили AB, то BA уже не будет
// проверяться.
//
// Массив может быть полностью обработан за 12 итераций (количество уникальных соседей) + 8 проверок
// третьего уровня (4 дубликата, но оно никак не влияет на производительность)
//
// Алгоритм O(1), если не брать в расчёт, что можно выполнять операцию только над одним дуоном
//
// 1. Итерируемся по массиву ячеек. Для каждой ячейки, сначала:
//      a. Разрешаем дуоны первого уровня.
//      b. Разрешаем дуоны третьего уровня
//
// 2. Проверям, что все клетки пусты

#define MIN_M(_a, _b) _a < _b ? _a : _b

int main(void) {
    // Инициализация ячеек (камер)

    // clang-format off
    #define CV_M(_c) &(g_cells[_c].value)
    scanf(
        "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu",
        CV_M(A), CV_M(B), CV_M(C), CV_M(D), CV_M(E), CV_M(F), CV_M(G), CV_M(H)
    );
    #undef CV_M
    // clang-format on

    // Считаем сумму и проверяем, что количество дуонов чётно и не равно нулю
    uint16_t cells_value_sum = 0;
    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        cells_value_sum += g_cells[i].value;
    }

    if (cells_value_sum % 2 || cells_value_sum == 0)
        impossible();

    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        // Разрешение дуонов первого уровня
        struct Cell* current_cell = &g_cells[i];
        for (uint8_t n = 0; n < current_cell->neighbors_count; ++n) {
            struct Cell* neighbour_cell = current_cell->neighbors[n];

            // Количество возможных дуонов к разрешению
            uint8_t cell_min = MIN_M(current_cell->value, neighbour_cell->value);

            if (cell_min) {
                for (uint8_t i = 0; i < cell_min; ++i)
                    print_to_buffer(current_cell->name, neighbour_cell->name, '-');

                current_cell->value -= cell_min;
                neighbour_cell->value -= cell_min;
            }
        }


        // Разрешение дуонов третьего уровня
        struct Cell* third_level_cell = current_cell->third_level;

        // Количество возможных дуонов к разрешению
        uint8_t cell_min = MIN_M(current_cell->value, third_level_cell->value);

        if (cell_min) {
            for (uint8_t i = 0; i < cell_min; ++i) {
                print_to_buffer(
                        current_cell->third_level_bridge[0]->name,
                        current_cell->third_level_bridge[1]->name, '+'
                );
                print_to_buffer(current_cell->name, current_cell->third_level_bridge[0]->name, '-');
                print_to_buffer(third_level_cell->name, current_cell->third_level_bridge[1]->name, '-');
            }

            current_cell->value -= cell_min;
            third_level_cell->value -= cell_min;
        }
    }

    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        if (g_cells[i].value)
            impossible();
    }

    printf("%s", out_buffer);
    return 0;
}
