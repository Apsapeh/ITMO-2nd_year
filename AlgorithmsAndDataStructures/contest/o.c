#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAXN 101

enum Color {
  UNVISITED,
  ONE,
  TWO,
  COUNT,
};

// Входные данные
static int g_n;
static int g_m;

// Матрица связей емжду лкштами
static uint8_t g_adj[MAXN][MAXN];

// Цвета вершин
static enum Color g_color[MAXN] = {UNVISITED};

// Раскрашиваем граф
uint8_t dfs(int v, enum Color color) {
  g_color[v] = color;

  for (int i = 1; i <= g_n; i++) {
    if (g_adj[v][i]) {
      if (g_color[i] == UNVISITED) {
        // Если сосед не покрашен, красим его в противоположный цвет
        if (!dfs(i, (enum Color)(COUNT - color))) {
          return 0;
        }

      } else if (g_color[i] == g_color[v]) {
        // Если сосед уже покрашен в тот же цвет — разделение невозможно
        return 0;
      }
    }
  }

  return 1;
}

int main(void) {
  // Читаем ввод
  scanf("%d %d", &g_n, &g_m);

  memset(g_adj, 0, sizeof(g_adj));

  // Устанаваливаем свяи
  for (int i = 0; i < g_m; i++) {
    int u;
    int v;
    scanf("%d %d", &u, &v);
    g_adj[u][v] = 1;
    g_adj[v][u] = 1;
  }

  // Граф может быть несвязным, поэтому проверяем всех
  for (int i = 1; i <= g_n; i++) {
    if (g_color[i] == UNVISITED) {
      if (!dfs(i, ONE)) {
        printf("NO\n");
        return 0;
      }
    }
  }

  printf("YES\n");
  return 0;
}
