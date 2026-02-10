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
 *  - 0.031 сек
 *  - 384 КБ
 */

int main(void) {
    int count;
    scanf("%d\n", &count);

    int sum = 0;
    int max_sum = 0;
    int value;
    for (int i = 0; i < count; ++i) {
        scanf("%d\n", &value);

        int tmp_sum = sum + value;
        if (tmp_sum > max_sum)
            max_sum = tmp_sum;

        sum = tmp_sum >= 0 ? tmp_sum : 0;
    }

    printf("%d\n", max_sum);

    return 0;
}
