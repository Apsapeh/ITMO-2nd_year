#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Размер ввода
#define STDIN_SIZE 100001

// Ввод, а потом вывод
static char g_stringBuf[STDIN_SIZE] = {0};

// Пара буква - вес
struct CharWeight {
  char ch;
  uint32_t weight;
};

// Сортируем буквы по весу в порядке убывания
int qsort_compar(const void *a_in, const void *b_in) {
  const struct CharWeight *a = (const struct CharWeight *)a_in;
  const struct CharWeight *b = (const struct CharWeight *)b_in;

  if (b->weight > a->weight)
    return 1;
  if (b->weight < a->weight)
    return -1;
  return 0;
}

int main(void) {
  // Читаем ввод
  scanf("%s", g_stringBuf);

  // Количество букв в введённой строке
  int chars_count[26] = {0};

  // Размер введённой строки
  int input_string_size = 0;

  // Считаем для каждой буквы, сколько раз она встречается
  char *cursor = g_stringBuf;
  while (*cursor) {
    char ch = *(cursor++);
    ++chars_count[ch - 'a'];
    ++input_string_size;
  }

  // Читаем ввод весов букв
  struct CharWeight chars_weight[26];
  for (int i = 0; i < 26; ++i) {
    uint32_t w;
    scanf("%" SCNu32, &w);
    // Устанавливаем вес
    chars_weight[i].ch = (char)i;
    chars_weight[i].weight = w;
  }

  // Сортируем буквы по весу в порядке убывания
  qsort(chars_weight, 26, sizeof(struct CharWeight), qsort_compar);

  // Переиспользуем буфер, в который читали ввод
  // Курсор для заполнения буфера слева направо
  char *left_cursor = g_stringBuf;
  // Курсор для заполнения буфера справа налево
  char *right_cursor = g_stringBuf + input_string_size - 1;

  // Устанавливаем пары, но только один раз, для максимизации суммы
  // Чем больше у буквы вес, тем ближе она к краям
  for (int i = 0; i < 26; ++i) {
    struct CharWeight chw = chars_weight[i];

    // Если у буквы есть пара, то ставим её с краёв
    if (chars_count[(size_t)chw.ch] >= 2) {
      char ch = 'a' + chw.ch;
      *(left_cursor++) = ch;
      *(right_cursor--) = ch;
      // Вычитаем две буквы из количества её в строке. Т.к. мы их расставили с
      // краёв
      chars_count[(size_t)chw.ch] -= 2;
    }
  }

  // Расставляем остальные буквы между двух краёв
  // Порядок не имеет значения, т.к. буквы, у которых есть пара, уже были
  // расставлены в максимальные позиции, осталось расставить их остаток. А
  // буквы, у которых не было пары, в любом случае дают 0
  for (int i = 0; i < 26; ++i) {
    const char ch = 'a' + i;
    while (chars_count[i]) {
      *(left_cursor++) = ch;
      --chars_count[i];
    }
  }

  printf("%s\n", g_stringBuf);

  return 0;
}
