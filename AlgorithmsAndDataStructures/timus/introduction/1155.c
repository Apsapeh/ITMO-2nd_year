#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Cell {
    char name;
    uint8_t value;
    struct Cell* third_level;
    struct Cell* third_level_bridge[2];
    struct Cell* neighbors[3];
};

#define INIT_CELL_M(_name, _nb1, _nb2, _nb3, _th, _thb_1, _thb_2)                                       \
    (struct Cell) {                                                                                     \
        .name = _name, .value = 0, .neighbors = {&g_cells[_nb1], &g_cells[_nb2], &g_cells[_nb3]},       \
        .third_level = &g_cells[_th], .third_level_bridge = {                                           \
            &g_cells[_thb_1],                                                                           \
            &g_cells[_thb_2]                                                                            \
        }                                                                                               \
    }

enum { A, B, C, D, E, F, G, H, CELLS_COUNT };

#define NEIGHBORS_COUNT 3

// clang-format off
static struct Cell g_cells[] = {
//                      name   neighbors   3rd-level   path-to-3rd-level
        INIT_CELL_M('A',    B, D, E,       G,            B, F),
        INIT_CELL_M('B',    A, C, F,       H,            C, G),
        INIT_CELL_M('C',    B, D, G,       E,            D, H),
        INIT_CELL_M('D',    A, C, H,       F,            A, E),
        INIT_CELL_M('E',    A, F, H,       C,            H, D),
        INIT_CELL_M('F',    B, E, G,       D,            E, A),
        INIT_CELL_M('G',    C, F, H,       A,            F, B),
        INIT_CELL_M('H',    D, E, G,       B,            G, C),
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

static inline void debug(void) {
    return;
    printf("\n");
    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        printf("%c - %d\n", g_cells[i].name, g_cells[i].value);
    }
}

// Разрешены могут быть только дуоны первого и третьего уровней
// 1. Сортируем ячейки по сумме количества дуонов в текущей ячейке первого уровня, которые можно
// разрешить за одну итерацию.
//      c.v + c.n[0].v + c.n[1].v + c.n[2].v
//
// 2. Разрешаем для каждой ячейки дуоны первого уровня
//
// 3. Сортируем ячейки по максимальному количеству третьего уровня

int main(void) {
    // Инициализация ячеек (камер)

    // clang-format off
    #define CV_M(_c) (int*) &(g_cells[_c].value)
    scanf(
        "%d %d %d %d %d %d %d %d",
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


    // Разрешение дуонов первого уровня
    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        struct Cell* current_cell = &g_cells[i];
        for (uint8_t n = 0; n < NEIGHBORS_COUNT; ++n) {
            struct Cell* neighbour_cell = current_cell->neighbors[n];

            uint8_t cell_min = current_cell->value < neighbour_cell->value ? current_cell->value
                                                                           : neighbour_cell->value;

            if (cell_min) {
                for (uint8_t i = 0; i < cell_min; ++i)
                    print_to_buffer(current_cell->name, neighbour_cell->name, '-');
            }

            current_cell->value -= cell_min;
            neighbour_cell->value -= cell_min;
        }
    }

    debug();

    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        struct Cell* current_cell = &g_cells[i];
        struct Cell* third_level_cell = current_cell->third_level;

        uint8_t cell_min = current_cell->value < third_level_cell->value ? current_cell->value
                                                                         : third_level_cell->value;

        if (cell_min) {
            for (uint8_t i = 0; i < cell_min; ++i) {
                print_to_buffer(
                        current_cell->third_level_bridge[0]->name,
                        current_cell->third_level_bridge[1]->name, '+'
                );
                print_to_buffer(current_cell->name, current_cell->third_level_bridge[0]->name, '-');
                print_to_buffer(third_level_cell->name, current_cell->third_level_bridge[1]->name, '-');
            }
        }


        current_cell->value -= cell_min;
        third_level_cell->value -= cell_min;
    }

    debug();

    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        if (g_cells[i].value)
            impossible();
    }

    printf("%s", out_buffer);
    return 0;
}
