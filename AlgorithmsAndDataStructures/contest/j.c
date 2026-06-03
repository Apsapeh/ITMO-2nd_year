#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t *g_queue_first_half;
static uint32_t g_queue_first_half_head = 0;
static uint32_t g_queue_first_half_tail = 0;

static uint32_t *g_queue_second_half;
static uint32_t g_queue_second_half_head = 0;
static uint32_t g_queue_second_half_tail = 0;

#define FIRST_HALF_SIZE (g_queue_first_half_tail - g_queue_first_half_head)
#define SECOND_HALF_SIZE (g_queue_second_half_tail - g_queue_second_half_head)

static inline void balance(void) {
  // Если втроая половина очереди больше первой, то в хвост первой половины
  // перемещаем голову второй При вставке в хвост, или удалении
  if (SECOND_HALF_SIZE > FIRST_HALF_SIZE) {
    g_queue_first_half[g_queue_first_half_tail++] =
        g_queue_second_half[g_queue_second_half_head++];

    // Если первая половина очереди больше второй на два гоблина, то в голову
    // второй половины помещаем хвост первой При вставке в середину
  } else if (FIRST_HALF_SIZE > SECOND_HALF_SIZE + 1) {
    g_queue_second_half[--g_queue_second_half_head] =
        g_queue_first_half[--g_queue_first_half_tail];
  }
}

static inline void add_to_tail(uint32_t goblin) {
  // Помещаем гоблина в конец второй половины очереди
  g_queue_second_half[g_queue_second_half_tail++] = goblin;
  balance();
}

static inline void add_to_mid(uint32_t goblin) {
  // Помещаем гоблина в конец первой половины очереди
  g_queue_first_half[g_queue_first_half_tail++] = goblin;
  balance();
}

static inline uint32_t pop_head(void) {
  // Получаем первого гоблина в очереди, перемещая голову вниз
  uint32_t goblin = g_queue_first_half[g_queue_first_half_head++];
  balance();
  return goblin;
}

int main(void) {
  // Читаем ввод
  uint32_t goblins_count;
  scanf("%" SCNu32, &goblins_count);

  // Выделяме память под первую половину очереди, голова может двигаться только
  // вниз
  g_queue_first_half = (uint32_t *)malloc(sizeof(uint32_t) * (goblins_count));
  // Выделяем двойную память под вторую половину очереди, т.к. голова может
  // двигаться в обе стороны при вставке в середину
  g_queue_second_half =
      (uint32_t *)malloc(sizeof(uint32_t) * (goblins_count * 2));

  // Переносим голову в середину массива второйв очереди. Чтоб можно было
  // двигаться вверх
  g_queue_second_half_head = (goblins_count);
  g_queue_second_half_tail = g_queue_second_half_head;

  // Без комментариевг
  for (uint32_t i = 0; i < goblins_count; ++i) {
    char goblin_class;
    uint32_t goblin_idx;
    scanf(" %c", &goblin_class);

    switch (goblin_class) {
    case '+':
      scanf(" %" SCNu32, &goblin_idx);
      add_to_tail(goblin_idx);
      break;
    case '*':
      scanf(" %" SCNu32, &goblin_idx);
      add_to_mid(goblin_idx);
      break;
    // '-'
    default:
      printf("%" SCNu32 "\n", pop_head());
      break;
    };
  }

  free(g_queue_first_half);
  free(g_queue_second_half);

  return 0;
}
