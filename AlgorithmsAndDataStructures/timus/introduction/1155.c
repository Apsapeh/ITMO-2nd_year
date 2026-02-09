#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct Cell {
    char name;
    uint8_t value;
    struct Cell* neighbors[3];
};

#define INIT_CELL_M(_name, _nb1, _nb2, _nb3)                                                            \
    (struct Cell) {                                                                                     \
        .name = _name, .value = 0, .neighbors = { &g_cells[_nb1], &g_cells[_nb2], &g_cells[_nb3] }      \
    }

enum { A, B, C, D, E, F, G, H, CELLS_COUNT };

// clang-format off
static struct Cell g_cells[] = {
        INIT_CELL_M('A', B, D, E),
        INIT_CELL_M('B', A, C, F),
        INIT_CELL_M('C', B, D, G),
        INIT_CELL_M('D', A, C, H),
        INIT_CELL_M('E', A, F, H),
        INIT_CELL_M('F', B, E, G),
        INIT_CELL_M('G', C, F, H),
        INIT_CELL_M('H', D, E, G),
};
// clang-format on


static inline void impossible(void) {
    printf("IMPOSSIBLE\n");
    exit(0);
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

    // Количество дуонов чётно, проверка edge-cases
    uint16_t cells_value_sum = 0;
    for (uint8_t i = 0; i < CELLS_COUNT; ++i) {
        cells_value_sum += g_cells[i].value;
    }

    if (cells_value_sum % 2 || cells_value_sum == 0)
        impossible();


    return 0;
}
