#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Сортируем цены в порядке убывания
int qsort_compar(const void *a_in, const void *b_in) {
  return *(uint16_t *)b_in - *(uint16_t *)a_in;
}

int main(void) {
  // Читаем ввод
  int n, k;
  scanf("%d %d", &n, &k);

  // Выделяем массив под цены
  uint16_t *prices = (uint16_t *)malloc(sizeof(uint16_t) * n);

  // Читаем цены
  for (int i = 0; i < n; ++i) {
    scanf("%" SCNu16, prices + i);
  }

  // Сортируем цены в порядке возрастания
  qsort(prices, n, sizeof(uint16_t), qsort_compar);

  // Перебираем все цены
  uint32_t sum = 0;
  for (int i = 0; i < n; ++i) {
    // Каждую НЕ k-ую цену добавляем в сумму
    if ((i + 1) % k) {
      sum += prices[i];
    }
  }

  // Печатаем результат
  printf("%" SCNu32 "\n", sum);

  free(prices);
}
