#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_PIGS_COUNT 100

enum BankState { UNVISITED, VISITING, VISITED };

// Где лежит ключ от каждой свинки
static uint8_t g_keys[MAX_PIGS_COUNT];

// Состояние каждой свинки (цвет вершины)
static enum BankState g_bankStates[MAX_PIGS_COUNT] = {UNVISITED};

// Результат программы
static uint8_t g_result = 0;

// Рекурсивный обход дерева
void dfs(uint8_t b) {
  enum BankState *bstate = &g_bankStates[b];

  if (*bstate != UNVISITED) {
    // Если мы встретили вершину, которая уже есть в нашем пути
    // значит замыкаем новый цикл
    if (*bstate == VISITING) {
      ++g_result;
    }
    return;
  }

  *bstate = VISITING;
  dfs(g_keys[b]);
  *bstate = VISITED;
}

int main() {
  // Читаем количество свиней
  uint8_t n;
  scanf(" %" SCNu8, &n);

  // Читаем, где находится ключ от каждой свинки
  for (uint8_t i = 0; i < n; ++i) {
    uint8_t k;
    scanf(" %" SCNu8, &k);
    g_keys[i] = k - 1;
  }

  // Рекурсивно обходим все свинки, которые не посетили до этого.
  for (uint8_t i = 0; i < n; ++i) {
    if (g_bankStates[i] == UNVISITED) {
      dfs(i);
    }
  }

  printf("%" PRIu8 "\n", g_result);

  return 0;
}
