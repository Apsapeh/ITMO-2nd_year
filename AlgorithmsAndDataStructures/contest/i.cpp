#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <set>

#define LAST_STEP_NOT_SETTED -1

int main(void) {
  // Читаем ввод
  int cars_count_in, floor_capacity_in, steps_count_in;
  scanf("%d %d %d", &cars_count_in, &floor_capacity_in, &steps_count_in);

  // Массив для порядка (шагов), в котором будут использованы машинки
  int *steps_arr = (int *)malloc(sizeof(int) * steps_count_in);

  // Массив следующих шагов
  // Когда машинка из текущего шага будет использована в следующий раз
  // Если больше не будет, то INT_MAX
  int *next_step_for_car_from_current_step =
      (int *)malloc(sizeof(int) * steps_count_in);

  // Читаем ввод шагов и устанавлеваем значение для массива следующих шагов по
  // умолчанию
  for (int i = 0; i < steps_count_in; ++i) {
    int car;
    scanf(" %d", &car);
    steps_arr[i] = car - 1; // Т.к. машинки нумеруются с единицы
    next_step_for_car_from_current_step[i] = INT_MAX;
  }

  // На каком шаге машинка была встречена в прошлый раз
  int *prev_step_for_each_car = (int *)malloc(sizeof(int) * cars_count_in);
  for (int i = 0; i < cars_count_in; ++i) {
    prev_step_for_each_car[i] = LAST_STEP_NOT_SETTED;
  }

  // Заполняем массив следущих шагов
  for (int i = 0; i < steps_count_in; ++i) {
    // Маишнка с текущего шага
    int current_car = steps_arr[i];
    // На каком шаге в последий раз видели машинку
    int current_car_last_step = prev_step_for_each_car[current_car];

    // Обновляем, когда последний раз видели машинку - сейчас
    prev_step_for_each_car[current_car] = i;

    // Если мы уже видели эту машинку, то в то место, где видели,
    // ставим это место
    if (current_car_last_step != LAST_STEP_NOT_SETTED) {
      next_step_for_car_from_current_step[current_car_last_step] = i;
    }
  }

  // Они нам больше не нужны
  free(steps_arr);
  free(prev_step_for_each_car);

  // Ужас, C++
  // Красно чёрное дерево с машинками, которые сейчас на полу.
  // Точнее с шагами, когда мы встретим эти машинки в следующий раз.
  // Если больше не встретим, т.е. INT_MAX, то оно само объединится, удобно
  std::set<int> cars_on_floor;

  int result = 0;
  for (int i = 0; i < steps_count_in; ++i) {
    // Ищем машинку на полу
    auto pos = cars_on_floor.find(i);
    // Нашли, тогда удаляем её
    if (pos != cars_on_floor.end()) {
      cars_on_floor.erase(pos);

      // Не нашли, тогда достаём машинку с полки, возможно ещё убираем другую
    } else {
      // Если на полу нет места, то убираем машинку, котору встретим позже
      // всего, т.е. шаг в конце сета.
      if (cars_on_floor.size() == (size_t)floor_capacity_in) {
        cars_on_floor.erase(std::prev(pos));
      }

      // Это шаг, либо добавление, либо свот
      ++result;
    }

    // Если машинка была, то просто обновляем её следующее местоположение
    // Если не было, то теперь будет
    cars_on_floor.insert(next_step_for_car_from_current_step[i]);
  }

  // Очищаем память
  free(next_step_for_car_from_current_step);

  // ПОБЕДА
  printf("%d\n", result);

  return 0;
}
