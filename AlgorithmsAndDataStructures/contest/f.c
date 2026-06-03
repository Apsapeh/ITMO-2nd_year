
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Размер ввода
#define STDIN_SIZE 100 * (100 + 2) + 2

// Ввод
static char g_inputString[STDIN_SIZE] = {0};

// Число (строка)
struct Number {
  uint8_t length;
  char *number;
};

// Введённые числа
static struct Number g_numbers[100];
static uint8_t g_numbersCount = 0;

// Компаратор для чисел
int qsort_compar(const void *a_in, const void *b_in) {
  // Буферы для компаратора
  static char ab[201];
  static char ba[201];

  const struct Number *a = (const struct Number *)a_in;
  const struct Number *b = (const struct Number *)b_in;

  // Заполняем буферы
  memcpy(ab, a->number, a->length);
  memcpy(ab + a->length, b->number, b->length);
  ab[a->length + b->length] = 0;

  memcpy(ba, b->number, b->length);
  memcpy(ba + b->length, a->number, a->length);
  ba[a->length + b->length] = 0;

  // Сравниваем строки
  return strcmp(ba, ab);
}

int main(void) {
  // Читаем ввод
  int readed = fread(g_inputString, 1, STDIN_SIZE, stdin);
  // 100% заканчиваем строку
  g_inputString[readed] = '\n';
  g_inputString[readed + 1] = 0;

  char *cursor = g_inputString;
  char *number_start = cursor;

  while (*cursor) {
    char ch = *(cursor++);

    // Если это конец строки и число непустое
    if (ch == '\0' || ch == '\n' || ch == '\r') {
      // Заканчиваем строку
      *(cursor - 1) = 0;

      // Сохраняем число
      if (cursor - number_start - 1) {
        g_numbers[g_numbersCount].number = number_start;
        g_numbers[g_numbersCount].length = cursor - number_start - 1;
        ++g_numbersCount;
      }

      // Устанавливаем начало нового числа на следующий символ
      number_start = cursor;

      continue;
    }
  }

  // Сортируем по убыванию
  qsort(g_numbers, g_numbersCount, sizeof(struct Number), qsort_compar);

  // Печатаем
  for (int i = 0; i < g_numbersCount; ++i) {
    // printf("%d\n", g_numbers[i].length);
    printf("%s", g_numbers[i].number);
  }
  printf("\n");

  return 0;
}
