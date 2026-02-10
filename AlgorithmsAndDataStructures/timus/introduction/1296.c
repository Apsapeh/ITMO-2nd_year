#include <stdio.h>
#include <stdint.h>

/*
 * Храним текущую сумму и максимальную за всё время сумму
 * Изначально они равны 0
 *
 * Каждое новое число прибавляем к сумме
 *  - Если число положительное, сумма выросла и стала больше максимального, то обновляем максимальное
 *  - Если число отрицательное и сумма стала меньше нуля, то сбрасываем текущую серию (т.е. зануляем
 *    сумму)
 *
 * Результаты:
 *  - 0.001 сек
 *  - 768 КБ
 */

// -30000\n - 7 байт
// 7 * 60001 + 1
static char g_buf[420008];

int main(void) {
    // Читаем весь ввод разом
    const int readed = fread(g_buf, 1, sizeof(g_buf), stdin);
    // Условия останова
    g_buf[readed] = '\n';
    g_buf[readed + 1] = '\0';
    char* cursor = g_buf;

    // Пропускаем первое число, оно нам не нужно
    while (*cursor++ != '\n') {
    }

    register char ch;
    register unsigned char is_minus = 0;
    register int value = 0;

    register int sum = 0;
    register int max_sum = 0;

    // Итерируемся по всему вводу
    while (*cursor) {
        ch = *cursor++;

        switch (ch) {
            // Если это конец строки, значит число закончено и его можно обработать
            case '\n':
                value = is_minus ? -value : value;

                int tmp_sum = sum + value;
                if (tmp_sum > max_sum)
                    max_sum = tmp_sum;

                sum = tmp_sum >= 0 ? tmp_sum : 0;
                value = 0;
                is_minus = 0;
                break;
            // Устанавливаем флаг, если число отрицательное
            case '-':
                is_minus = 1;
                break;
            // Иначе это цифра
            default:
                value = value * 10 + ch - '0';
        }
    }

    // Граничный случай, проблема с тестом 7
    // Если в конце ввода нет ни новой строки, ни возрврата каретки, то принудительно проверяем последнее
    // число
    if (value != 0 || is_minus) {
        value = is_minus ? -value : value;
        int tmp_sum = sum + value;
        if (tmp_sum > max_sum)
            max_sum = tmp_sum;
        sum = tmp_sum >= 0 ? tmp_sum : 0;
    }

    printf("%d\n", max_sum);
}
