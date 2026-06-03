#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Хранит само введённое число и индекс, когда оно было введено
struct Number {
  int num;
  int idx;
};

// По какой-то причине,
// struct Number* deque =
//     (struct Number*)malloc(sizeof(struct Number*) * num_count)
// не проходит по clang-format, думаю, что он стоит в 80 символов,
// хотя в файле .clang-format стоит 100
// Пришлось добавить этот костыль
typedef struct Number N;

int main(void) {
  // Читаем ввод
  int num_count, window_size;
  scanf("%d %d", &num_count, &window_size);

  // Выделяем память для деки
  struct Number *deque = (N *)malloc(sizeof(N *) * num_count);
  // Указывает на первый элемент
  struct Number *deque_head = deque;
  // Указывает на следующий за последним элементом элемент
  struct Number *deque_tail = deque;

  for (int i = 0; i < num_count; ++i) {
    // Читаем число и его индекс
    struct Number number;
    number.idx = i;
    scanf(" %d", &number.num);

    // Удаляем все числа с конца деки если они меньше нового числа.
    // Это позволяет всегда держать деку отсортированной,
    // где в голове будет минимум.
    // Т.е пусть дека = [1, 2, 3, 4, 8, 9, 17, 89], ввели число 6
    // Дека станет [1, 2, 3, 4]
    while (deque_tail - deque_head) {
      if (number.num <= (deque_tail - 1)->num) {
        --deque_tail;
      } else {
        break;
      }
    }

    // Если число в голове вышло из окна, то удалем его
    if (deque_tail - deque_head && deque_head->idx + window_size <= i) {
      ++deque_head;
    }

    // Добавляем новое число в хвост деки
    *(deque_tail++) = number;

    // Если окно заполнено, то печатаем его минимум
    // Пропускает первые шаги, когда окно только заполняется
    if (i >= window_size - 1) {
      printf("%d ", deque_head->num);
    }
  }

  printf("\n");

  return 0;
}
