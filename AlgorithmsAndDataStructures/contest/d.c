#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  // Читаем ввод
  int32_t a, prev_a = -1;
  uint16_t b, c, d;
  uint64_t k;

  scanf("%" SCNd32 " %" SCNu16 " %" SCNu16 " %" SCNu16 " %" SCNu64, &a, &b, &c,
        &d, &k);

  // Итерируемся до k-го дня
  for (uint64_t i = 0; i < k; ++i) {
    // Находим количество бактерий на конец дня
    a = a * b - c;

    //  Заканчиваем, если бактерии закончились
    if (a <= 0) {
      a = 0;
      break;
    }

    // Если бактерий на конец дня больше, чем может поместитьсь в контейнер (d),
    // то оставляем только те, что смогли поместиться
    a = a < d ? a : d;

    // Если число равно предыдущему, то и дальше оно не будут меняться до k-го
    // дня
    if (a == prev_a)
      break;

    prev_a = a;
  }

  printf("%" SCNd32 "\n", a);

  return 0;
}
