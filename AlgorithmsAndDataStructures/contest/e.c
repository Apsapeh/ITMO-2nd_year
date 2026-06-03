#include <stdio.h>

static int g_stalls[100000];

int main(void) {
  // Читаем ввод
  int stalls_count, cows_count;
  scanf("%d %d", &stalls_count, &cows_count);

  for (int i = 0; i < stalls_count; ++i) {
    scanf("%d", g_stalls + i);
  }

  // Диапазон бинарного поиска
  int left = 0;
  int right = g_stalls[stalls_count - 1] -
              g_stalls[0]; // Максимальное возможное расстояние

  while (left <= right) {
    // Середина диапазона бинарного поиска
    int mid = left + (right - left) / 2;

    // Изначально равно одному, т.к. у нас всегда в первом стойле стоит корова
    int cows_in_stalls = 1;
    // Аккумулятор
    int acc = 0;
    for (int stall_i = 1; stall_i < stalls_count; ++stall_i) {
      // Аккумулятор, сколько прошли от предыдущего стойла с коровой
      acc += g_stalls[stall_i] - g_stalls[stall_i - 1];

      // Если аккумулятор больше минимальной дистанции, то ставим корову и идём
      // дальше
      if (acc >= mid) {
        ++cows_in_stalls;
        acc = 0;
      }

      // Если все коровы расставлены
      if (cows_in_stalls >= cows_count) {
        break;
      }
    }

    // Если все коровы поместились с таким шагом, то пытаемся взять шаг больше
    // [mid + 1 .. right]
    if (cows_in_stalls >= cows_count) {
      left = mid + 1;
      // Если коровы не поместились с таким шагом, то берём шаг меньше [left ..
      // mid - 1]
    } else {
      right = mid - 1;
    }
  }

  // Печтаем ответ по правому ограничению, либо по левому - 1
  printf("%d\n", right);

  return 0;
}
