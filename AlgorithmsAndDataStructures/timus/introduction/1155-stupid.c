// Разрешены могут быть только дуоны первого и третьего уровне
//
// Дуоны первого уровня, это соседние клетки
//
// Дуоны третьего уровня это сосед соседей соседей.
//     А -> B -> C -> D, где A - текущая клетка, D - клетка третьего уровня
//     Т.к. куб, то у каждой клетки есть только одна соседняя клетка третьего уровня
//
// 1. Сначала разрешаем 12 возможных дуонов первого уровня, без дубликатов. Т.е. если проверили AB, то BA
// проверять не будем
//
// 2. Разрешаем 4 дуона третьего уровня
//
// Результаты:
//    0.001 сек
//    376 КБ
//    367 из 2832 (11.02.26 01:30)

#include <stdio.h>

// Оптимизированный вывод за раз
static char g_out_buffer[4 * 1000 + 1] = {0};
static char* g_buffer_cursor = g_out_buffer;


static inline void print_to_buffer(char cell_1, char cell_2, char op) {
    g_buffer_cursor[0] = cell_1;
    g_buffer_cursor[1] = cell_2;
    g_buffer_cursor[2] = op;
    g_buffer_cursor[3] = '\n';
    g_buffer_cursor += 4;
}


#define FIRST_LEVEL(_a, _b, _ac, _bc)                                                                   \
    do {                                                                                                \
        if (_a && _b) {                                                                                 \
            --_a;                                                                                       \
            --_b;                                                                                       \
            print_to_buffer(_ac, _bc, '-');                                                             \
        }                                                                                               \
    } while (0)

#define THIRD_LEVEL(_a, _b, _ac, _bc, _br1c, _br2c)                                                     \
    do {                                                                                                \
        if (_a && _b) {                                                                                 \
            --_a;                                                                                       \
            --_b;                                                                                       \
            print_to_buffer(_br1c, _br2c, '+');                                                         \
            print_to_buffer(_ac, _br1c, '-');                                                           \
            print_to_buffer(_bc, _br2c, '-');                                                           \
        }                                                                                               \
    } while (0)

int main(void) {
    // Инициализация ячеек (камер)

    // clang-format off
    int A, B, C, D, E, F, G, H;
    scanf(
        "%d %d %d %d %d %d %d %d",
        &A, &B, &C, &D, &E, &F, &G, &H
    );
    // clang-format on
    //

    // В обеих частях клетки не соседние между друг другом
    if (A + C + F + H != B + D + E + G) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    while (A | B | C | D | E | F | G | H) {
        FIRST_LEVEL(A, B, 'A', 'B');
        FIRST_LEVEL(A, E, 'A', 'E');
        FIRST_LEVEL(B, C, 'B', 'C');
        FIRST_LEVEL(B, F, 'B', 'F');
        FIRST_LEVEL(C, D, 'C', 'D');
        FIRST_LEVEL(C, G, 'C', 'G');
        FIRST_LEVEL(D, A, 'D', 'A');
        FIRST_LEVEL(D, H, 'D', 'H');
        FIRST_LEVEL(E, F, 'E', 'F');
        FIRST_LEVEL(E, H, 'E', 'H');
        FIRST_LEVEL(F, G, 'F', 'G');
        FIRST_LEVEL(G, H, 'G', 'H');

        THIRD_LEVEL(A, G, 'A', 'G', 'B', 'F');
        THIRD_LEVEL(B, H, 'B', 'H', 'C', 'G');
        THIRD_LEVEL(C, E, 'C', 'E', 'D', 'H');
        THIRD_LEVEL(D, F, 'D', 'F', 'A', 'E');
    }

    printf("%s", g_out_buffer);
    return 0;
}
