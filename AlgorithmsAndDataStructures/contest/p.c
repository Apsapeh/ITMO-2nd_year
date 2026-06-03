#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAXN 1000

int g_n;
int g_fuel[MAXN][MAXN];
uint8_t g_visited[MAXN];

enum GraphType {
  Nomral,
  Trans,
};

// Рекурсивно обходим граф в глубину
void dfs(int v, int max_fuel, enum GraphType type) {
  g_visited[v] = 1;
  for (int i = 0; i < g_n; i++) {
    if (!g_visited[i]) {
      // Если t == Normal, проверяем путь v -> i
      // Если t == Trans, проверяем путь i -> v
      int cost = (type == Nomral) ? g_fuel[v][i] : g_fuel[i][v];
      if (cost <= max_fuel) {
        dfs(i, max_fuel, type);
      }
    }
  }
}

// Проверяем, подохдит ли текущий размер бака
uint8_t check(int max_fuel) {
  // Проверяем, что из города 0 можно добраться во все города
  memset(g_visited, 0, sizeof(g_visited));
  dfs(0, max_fuel, Nomral);
  for (int i = 0; i < g_n; i++) {
    if (!g_visited[i])
      return 0;
  }

  // Проверяем, что из всех городов можно добраться до города 0
  memset(g_visited, 0, sizeof(g_visited));
  dfs(0, max_fuel, Trans);
  for (int i = 0; i < g_n; i++) {
    if (!g_visited[i])
      return 0;
  }

  return 1;
}

int main() {
  // Читаем входные данные
  scanf("%d", &g_n);
  for (int i = 0; i < g_n; i++) {
    for (int j = 0; j < g_n; j++) {
      scanf("%d", &g_fuel[i][j]);
    }
  }

  // Бинарный поиск по ответу
  int left = 0;
  int right = 1000000000;
  int ans = right;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (check(mid)) {
      // Запоминаем возможный ответ
      ans = mid;
      // Пробуем найти бак ещё меньше
      right = mid - 1;
    } else {
      // Увеличиваем бак, так как текущего мало
      left = mid + 1;
    }
  }

  printf("%d\n", ans);
  return 0;
}
